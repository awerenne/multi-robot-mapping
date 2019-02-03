"""
    Test: Static interface (no robots moving) with responsive buttons
"""


from queue import Queue
import pygame
import sys
import time
sys.path.append('../../master/')
from utils import Parameters, Container, xy2ij, center2xy




class GUI():
    """
    Extensive description of class.
    """  

    def __init__(self, params, queues, ground_truth=None):
        
        self.q = queues
        self.load_params(params)
        self.init_display()
        self.init_map_meta()
        self._close = False


    #---------------
    @property
    def close(self):
        return self._close
    

    #---------------
    def load_params(self, params):
        self.dimensions_screen = (1000,500)
        self.dimensions_map = (500,400)
        self.dimensions_user = (300,400)
        (self.w, self.h) = (1000,600)
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
        self.surface_map = self.surface_screen.subsurface(pygame.Rect(50, 50,
                            self.wm, self.hm))
        self.surface_user = self.surface_screen.subsurface(pygame.Rect(650,
                            50, self.wu, self.hu))
        self.surface_screen.fill((45,45,45)) 
        self.surface_map.fill((255,255,255)) 
        self.surface_user.fill((45,45,45)) 

        self.button_run = pygame.Rect((50, 50, 50, 50))
        self.button_stop = pygame.Rect((125, 50, 50, 50))
        self.button_ = pygame.Rect((200, 50, 50, 50))
        self.timer = pygame.Rect((50, 125, 200, 50))
        self.log = pygame.Rect((50, 200, 200, 150))

        pygame.display.set_caption(self.title)
        self.font_timer = pygame.font.SysFont(self.font_name, 35)
        self.font_log = pygame.font.SysFont(self.font_name, self.font_size)
        
        for y in range(0, self.hm, self.block_size):
            for x in range(0, self.wm, self.block_size):
                rect = pygame.Rect(x, y, self.block_size, self.block_size)
                pygame.draw.rect(self.surface_map, (235,235,235), rect, 1)

        self.t_start = time.perf_counter()


    #---------------
    def init_map_meta(self):
        # TODO fix myself the values
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
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                self.handle_click(event.pos)
        return self


    #---------------
    def handle_click(self):
        pass


    #---------------
    def is_mouse_on_button(self, position, button):
        relative_position = (position[0]-650, position[1]-50) 
        if button.collidepoint(relative_position):
            return True
        return False


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
            directive = self.q.master2gui.get(timeout=1)
            if directive == None:
                 self.updated_view_map = True
            elif directive.type_directive == "update_summary":
                self.edges = directive.summary.edges
                self.frontiers = directive.summary.frontiers
                self.robots = directive.summary.robots
                self.updated_view_map = False  
            
    #---------------
    def draw_user_interface(self):
        self.draw_buttons()
        self.screen.blit(self.surface_user, (self.wm, 0))


    #---------------
    def draw_buttons(self):
        img_run = pygame.image.load('run_small.png').convert_alpha()  
        if self.is_mouse_on_button(pygame.mouse.get_pos(), self.button_run):
            color = (228,106,104)
        else:
            color = (171,171,171)
        pygame.draw.rect(self.surface_user, color, self.button_run)
        self.surface_user.blit(img_run, (60, 50, 50, 50))

        img_stop = pygame.image.load('stop_small.png').convert_alpha() 
        if self.is_mouse_on_button(pygame.mouse.get_pos(), self.button_stop):
            color = (228,106,104)
        else:
            color = (171,171,171)
        pygame.draw.rect(self.surface_user, color, self.button_stop)
        self.surface_user.blit(img_stop, (140, 50, 50, 50))

        # pygame.draw.rect(self.surface_user, (171,171,171),
        #         self.button_)

        pygame.draw.rect(self.surface_user, (45,45,45),
                self.timer)

        diff = time.perf_counter()-self.t_start
        def timer_format(x):
            return "0"+str(x) if x < 10 else str(x)
        minutes = timer_format(int(diff/60)%60)
        seconds = timer_format(int(diff)%60)

        string_time = "00:" + minutes + ":" + seconds
        text_surf = self.font_timer.render(string_time, True, self.colors.user.font)
        self.surface_user.blit(text_surf, (65, 130, 200, 50))

        pygame.draw.rect(self.surface_user, (171,171,171),
                self.log)
        text_surf = self.font_log.render("Log...", True, self.colors.user.font)
        self.surface_user.blit(text_surf, (75, 225))

        
    #---------------
    def draw_map(self):
        if self.updated_view_map:
            return
        self.draw_edges().draw_frontiers().draw_robots()
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
            a = self.rescale(a)
            a = xy2ij(center2xy(a, dim), dim)
            b = self.rescale(b)
            b = xy2ij(center2xy(b, dim), dim)
            # a = self.togui(a)
            # b = self.togui(b)
            pygame.draw.line(self.surface_map, color, a, b, width)
        return self


    def rescale(self, xy):
        def f(x):
            offset = int(self.block_size/2)
            return x*self.scale + offset
        return list(map(f, xy))


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
            # (i, j) = self.togui((i,j))
            rect = pygame.Rect(i-self.block_size/2, j-self.block_size/2, self.block_size+1, self.block_size+1)
            pygame.draw.rect(self.surface_map, color, rect)
        return self


    #---------------
    def draw_robots(self):
        if self.robots == None or len(self.robots) == 0:
            return self
        color = self.colors.robot
        radius = self.radius_robot
        dim = self.dimensions_map
        for robot in self.robots:
            (xy, orientation) = robot
            (x,y) = xy
            if orientation == 0: xy = (x,y+5)
            if orientation == 1: xy = (x+5,y)
            if orientation == 2: xy = (x,y-5)
            if orientation == 3: xy = (x-5,y)
            (x,y) = self.rescale(xy)
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



#---------------
def make_queues():
    q = {
        "master2gui": Queue(),
        "gui2master": Queue()
    }
    return Container(q)


#---------------
def simul(q):
    directive = {
        "type_directive": "update_summary",
        "summary": {
            "edges": [((10,10),(10,20)), ((10,20),(-30,20))],
            "frontiers": [(-35,20)],
            "robots": [((-30,20),3)]
        }
    }
    q.master2gui.put(Container(directive))



#---------------
if __name__ == '__main__':

    params = Parameters("../../config/config.yaml")
    q = make_queues()
    simul(q)
    gui = GUI(params, q, None)

    while not gui.close:
        gui.check_user_event().update()


  









