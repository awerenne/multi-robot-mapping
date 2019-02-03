"""
    Test: Static interface (no robots moving) with responsive buttons
"""


from queue import Queue
import pygame
import sys
sys.path.append('../../master/')
from utils import Parameters, Container


# Static design


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
        self.font_timer = pygame.font.Font(self.font_name, 45)
        self.font_log = pygame.font.Font(self.font_name, self.font_size)

        block_size = 50  # Corresponds to discretization factor
        for y in range(0, self.hm, block_size):
            for x in range(0, self.wm, block_size):
                rect = pygame.Rect(x, y, block_size, block_size)
                pygame.draw.rect(self.surface_map, (235,235,235), rect, 1)


    #---------------
    def init_map_meta(self):
        # TODO fix myself the values
        self.edges = {}
        self.frontiers = {}
        self.robots = {}
        self.updated_view_map = False  # Avoids update of screen when not needed


    #---------------
    def compute_scale(self, dim_view, dim_real):
        self.scale = 1
        # TODO !!


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
            print("run")
        elif self.button_stop.collidepoint(relative_position):
            print("stop")


    #---------------
    def update(self):
        self.draw_user_interface()
        self.draw_map()
        self.screen.blit(self.surface_screen, (0,0))
        pygame.display.update()
        return self
            
            
    #---------------
    def draw_user_interface(self):
        self.draw_buttons()
        self.screen.blit(self.surface_user, (self.wm, 0))


    #---------------
    def draw_buttons(self):
        img_run = pygame.image.load('run_small.png').convert_alpha()  
        pygame.draw.rect(self.surface_user, (171,171,171),
                self.button_run)
        self.surface_user.blit(img_run, (60, 50, 50, 50))

        img_stop = pygame.image.load('stop_small.png').convert_alpha() 
        pygame.draw.rect(self.surface_user, (171,171,171),
                self.button_stop)
        self.surface_user.blit(img_stop, (140, 50, 50, 50))

        pygame.draw.rect(self.surface_user, (171,171,171),
                self.button_)

        # pygame.draw.rect(self.surface_user, (171,171,171),
        #         self.timer)
        text_surf = self.font_timer.render(str(50)+":"+str(10)+":"+str(10), True, self.colors.user.font)
        self.surface_user.blit(text_surf, (60, 130, 200, 50))

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



#---------------
def make_queues():
    q = {
        "master2gui": Queue(),
        "gui2master": Queue()
    }
    return Container(q)


#---------------
if __name__ == '__main__':

    params = Parameters("../../config/config.yaml")
    q = make_queues()
    gui = GUI(params, q, None)

    while not gui.close:
        gui.check_user_event().update()


  









