"""
    Description.
"""


import pygame
import time
from utils import Container, xy2ij, center2xy


# TODO: params in draw_buttons() in config file
# TODO: apply scaling during drawing
# TODO: cool looking GUI


#---------------
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
        self.dimensions_screen = params.gui.dimensions_screen
        self.dimensions_map = params.gui.dimensions_map
        self.dimensions_user = params.gui.dimensions_user
        (self.w, self.h) = self.dimensions_screen
        (self.wm, self.hm) = self.dimensions_map
        (self.wu, self.hu) = self.dimensions_user
        self.compute_scale(self.dimensions_map,
                params.dimensions)
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
        self.surface_map = self.surface_screen.subsurface(pygame.Rect(0, 0,
                            self.wm, self.hm))
        self.surface_user = self.surface_screen.subsurface(pygame.Rect(self.wm,
                            0, self.wu, self.hu))
        self.surface_map.fill(self.colors.background) 
        self.surface_user.fill(self.colors.user.background) 

        self.button_run = pygame.Rect(self.rect_run)
        self.button_stop = pygame.Rect(self.rect_stop)

        pygame.display.set_caption(self.title)
        self.font = pygame.font.Font(self.font_name, self.font_size)


    #---------------
    def init_map_meta(self):
        self.edges = {}
        self.frontiers = {}
        self.robots = {}
        self.updated_view_map = False  # Avoids update of screen when not needed


    #---------------
    def compute_scale(self, dim_view, dim_real):
        self.scale = 1
        # TODO


    #---------------
    def check_user_event(self):
        for event in pygame.event.get():  
            if event.type == pygame.QUIT: 
                self._close = True
                pygame.quit()
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                self.handle_button_click(event.pos)
        return self


    #---------------
    def handle_button_click(self, click_position):
        relative_position = (click_position[0]-self.wm, click_position[1]) 
        if self.button_run.collidepoint(relative_position):
            self.send_user_request_to_master("run")
        elif self.button_stop.collidepoint(relative_position):
            self.send_user_request_to_master("stop")


    #--------------- 
    def send_user_request_to_master(self, request):
        directive = {"type_directive": "request_" + request}
        self.q.gui2master.put(Container(directive))


    #---------------
    def update(self):
        time.sleep(self.delay)
        if self.q.master2gui.empty():
            return self
        self.receive_directive_from_master()
        self.draw_user_interface()
        self.draw_map()
        self.screen.blit(self.surface_screen, (0,0))
        pygame.display.update()
        return self


    #---------------
    def receive_directive_from_master(self):
        if not self.q.master2gui.empty():
            directive = self.q.master2gui.get()
            print(directive)
            if directive.type_directive == "update_summary":
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
        pygame.draw.rect(self.surface_user, self.colors.user.button,
                self.button_run)
        text_surf = self.font.render("run", True, self.colors.user.font)
        text_rect = text_surf.get_rect(center=(10, 10))
        self.surface_user.blit(text_surf, (75, 135))

        pygame.draw.rect(self.surface_user, self.colors.user.button,
                self.button_stop)
        text_surf = self.font.render("stop", True, self.colors.user.font)
        text_rect = text_surf.get_rect(center=(50, 25))  
        self.surface_user.blit(text_surf, (75, 215))

        
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
            a = xy2ij(center2xy(a, dim), dim)
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
        for (x, y) in self.frontiers:
            (i, j) = xy2ij(center2xy((x,y), dim), dim)
            rect = pygame.Rect(i-width/2, j-width/2, width, width)
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
            ij = xy2ij(center2xy(xy, dim), dim)
            pygame.draw.circle(self.surface_map, color, ij, radius)
        return self










        
        
        

    


        

    










    



