"""
    Description.
"""


import pygame
import time
import sys
from utils import Container, xy2ij, center2xy

class GUI():
    """
    Extensive description of class.
    """  

    def __init__(self, params, queues, environment):
        
        self.q = queues
        self.load_params(params)
        self.init_display()
        self.init_map_meta()
        self.ground_truth = environment.ground_truth
        self.environment = environment
        self._close = False


    #---------------
    @property
    def close(self):
        return self._close
    

    #---------------
    def load_params(self, params):
        self.dimensions_screen = (1025,500)
        self.dimensions_map = (700,450)
        self.dimensions_user = (300,400)
        (self.w, self.h) = self.dimensions_screen
        (self.wm, self.hm) = self.dimensions_map
        (self.wu, self.hu) = self.dimensions_user

        self.scale = 5  # A0 paper size -> map in GUI
        self.discretization = 5
        self.block_size = self.scale * self.discretization  # Must be uneven to have center!!

        self.rect_run = params.gui.rect_run
        self.rect_stop = params.gui.rect_stop

        self.frame_rate = params.gui.frame_rate
        self.delay = 1.0 / self.frame_rate  # Unit: seconds
        
        self.title = params.gui.title
        self.width_line = params.gui.width_line
        self.width_frontier = params.gui.width_frontier
        self.radius_robot = params.gui.radius_robot
        self.colors = params.gui.colors
        self.font_name = params.gui.font.name
        self.font_size = params.gui.font.size
        

    #---------------
    def init_display(self):
        # assert self.valid_dimensions()
        pygame.init()

        self.screen = pygame.display.set_mode(self.dimensions_screen)
        self.surface_screen = pygame.Surface(self.dimensions_screen)
        self.surface_map = self.surface_screen.subsurface(pygame.Rect(25, 25,
                            self.wm, self.hm))
        self.surface_user = self.surface_screen.subsurface(pygame.Rect(725,
                            50, self.wu, self.hu))
        self.surface_screen.fill((45,45,45)) 
        self.surface_user.fill((45,45,45)) 

        self.icon_run = pygame.image.load('icons/run_small.png').convert_alpha()  
        self.rect_run = pygame.Rect((50, 50, 50, 50))
        self.icon_stop = pygame.image.load('icons/stop_small.png').convert_alpha() 
        self.rect_stop = pygame.Rect((125, 50, 50, 50))

        self.rect_increment = pygame.Rect((200, 50, 50, 50))
        
        self.timer = pygame.Rect((50, 125, 200, 50))
        self.log = pygame.Rect((50, 200, 200, 150))

        pygame.display.set_caption(self.title)
        self.font_timer = pygame.font.Font(None, self.font_size)
        self.font_log = pygame.font.Font(None, self.font_size)
        
        self.reset_map()
        self.t_start = None


    #---------------
    def reset_map(self):
        self.surface_map.fill((255,255,255)) 
        for y in range(0, self.hm, self.block_size):
            for x in range(0, self.wm, self.block_size):
                rect = pygame.Rect(x, y, self.block_size, self.block_size)
                pygame.draw.rect(self.surface_map, (235,235,235), rect, 1)


    #---------------
    def init_map_meta(self):
        self.edges = {}
        self.frontiers = {}
        self.robots = {}
        self.updated_view_map = False  # Avoids update of screen when not needed


    #---------------
    def check_user_event(self):
        for event in pygame.event.get():  
            if event.type == pygame.QUIT: 
                self._close = True
                pygame.quit()
                sys.exit()
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                self.handle_click(event.pos)
        return self


    #---------------
    def handle_click(self, position):
        if self.is_mouse_on_button(position, self.rect_run):
            self.send_user_request_to_master("run")
            self.t_start = time.perf_counter()
        elif self.is_mouse_on_button(position, self.rect_stop):
            self.send_user_request_to_master("stop")
        elif self.is_mouse_on_button(position, self.rect_increment):
            self.send_user_request_to_master("increment")


    #---------------
    def is_mouse_on_button(self, position, button):
        relative_position = (position[0]-725, position[1]-50) 
        if button.collidepoint(relative_position):
            return True
        return False


    #---------------
    def rescale(self, xy):
        def f(x):
            offset = int(self.block_size/2)
            return int(x*self.scale/2 + offset)
        return list(map(f, xy))


    #--------------- 
    def send_user_request_to_master(self, request):
        directive = {"type_directive": "request_" + request}
        self.q.gui2master.put(Container(directive))


    #---------------
    def update(self):
        self.receive_directive_from_master()
        self.draw_user_interface()
        self.draw_map()
        self.screen.blit(self.surface_screen, (0,0))
        pygame.display.update()
        return self
          

    #---------------
    def receive_directive_from_master(self):
        if not self.q.master2gui.empty():
            # directive = self.q.master2gui.get(timeout=1)
            directive = self.q.master2gui.get_nowait()
            if directive != None and directive.type_directive == "update_summary":
                # print(directive)
                self.edges = directive.summary.edges
                self.frontiers = directive.summary.frontiers
                self.robots = directive.summary.robots
        self.updated_view_map = False  
            

    #---------------
    def draw_user_interface(self):
        self.draw_buttons()
        self.draw_timer()
        self.draw_log()
        self.screen.blit(self.surface_user, (self.wm, 0))


    #---------------
    def draw_buttons(self):
        self.draw_button(self.rect_run, self.icon_run)
        self.draw_button(self.rect_stop, self.icon_stop)
        self.draw_button(self.rect_increment)


    #---------------
    def draw_button(self, rect, icon=None):    
        if self.is_mouse_on_button(pygame.mouse.get_pos(), rect):
            color = (228,106,104)
        else:
            color = (171,171,171)
        pygame.draw.rect(self.surface_user, color, rect)
        if icon != None:
            self.surface_user.blit(icon, (rect.x+12, rect.y, rect.w, rect.h))


    #---------------
    def draw_timer(self):
        pygame.draw.rect(self.surface_user, (45,45,45),
                self.timer)
        if self.t_start == None: delayed_time = 0
        else: delayed_time = time.perf_counter() - self.t_start
        string_time = self.timer_format(delayed_time)
        text_surf = self.font_timer.render(string_time, True, self.colors.user.font)
        self.surface_user.blit(text_surf, (self.timer.x+15, self.timer.y+5,
                self.timer.w, self.timer.h))
    

    #---------------
    def timer_format(self, t):
        def format(x):
            return "0"+str(x) if x < 10 else str(x)
        hours = format(int(t/3600))
        minutes = format(int(t/60)%60)
        seconds = format(int(t)%60)
        return hours + ":" + minutes + ":" + seconds


    #---------------
    def draw_log(self):
        pygame.draw.rect(self.surface_user, (171,171,171), self.log)
        text_surf = self.font_log.render("Log...", True, self.colors.user.font)
        self.surface_user.blit(text_surf, (self.log.x+25, self.log.y+25))

        
    #---------------
    def draw_map(self):
        if self.updated_view_map:
            return
        self.reset_map()
        self.draw_ground_truth().draw_edges().draw_robots()
        self.screen.blit(self.surface_map, (0, 0))
        self.updated_view_map = True


    #---------------
    def draw_edges(self):
        if self.edges == None or len(self.edges) == 0:
            return self
        color = self.colors.free
        width = self.width_line
        dim = self.dimensions_map
        for (a, b) in self.edges:
            if a == b:  # null edge
                continue
            a = self.rescale(a)
            a = xy2ij(center2xy(a, dim), dim)
            b = self.rescale(b)
            b = xy2ij(center2xy(b, dim), dim)
            pygame.draw.line(self.surface_map, color, a, b, width)
        return self


    #---------------
    def draw_frontiers(self):
        if self.frontiers == None or len(self.frontiers) == 0:
            return self
        color = self.colors.frontier
        width = self.width_frontier
        dim = self.dimensions_map
        for xy in self.frontiers:
            xy = self.rescale(xy)
            (i, j) = xy2ij(center2xy(xy, dim), dim)
            rect = pygame.Rect(i-self.block_size/2, j-self.block_size/2,
                    self.block_size+1, self.block_size+1)
            pygame.draw.rect(self.surface_map, color, rect)
        return self

    #---------------
    def draw_ground_truth(self):
        if self.ground_truth == None or len(self.ground_truth) == 0:
            return self
        color = self.colors.frontier
        width = self.width_line
        dim = self.dimensions_map
        for (a, b) in self.ground_truth:
            if a == b:  # null edge
                continue
            a = self.rescale(a)
            a = xy2ij(center2xy(a, dim), dim)
            b = self.rescale(b)
            b = xy2ij(center2xy(b, dim), dim)
            pygame.draw.line(self.surface_map, color, a, b, width)
        return self


    #---------------
    def draw_robots(self):
        if self.robots == None or len(self.robots) == 0:
            return self
        color = self.colors.robot
        radius = self.radius_robot
        dim = self.dimensions_map
        myrobots = self.environment.get_robots()
        for robot in myrobots:
        # for robot in self.robots:
            # (xy, orientation) = robot
            # (x,y) = xy
            (x, y, orientation) = robot
            (x,y) = self.rescale((x,y))
            xy = (x+1,y+1)
            ij = xy2ij(center2xy(xy, dim), dim)
            (i,j) = ij
            pygame.draw.circle(self.surface_map, color, ij, radius)
            base = [(i,j-3), (i+3,j), (i,j+3), (i-3,j)]
            tops = [(i,j-7), (i+7,j), (i,j+7), (i-7,j)]
            real = []
            for i in range(4):
                if i == orientation: real.append(tops[i])
                elif abs(i-orientation) != 2: real.append(base[i])
            pygame.draw.polygon(self.surface_map, (50,50,50), real)
        return self


    
        
        

    


        

    










    



