"""
    Description.
"""


import pygame
import time


#---------------
class GUI():
    """
    Extensive description of class.
    """  

    def __init__(self, q, shared_parameters, environment_parameters):
        
        self.q = q

        self.dimensions_map = environment_parameters.dimensions_map
        self.dimensions_user = environment_parameters.dimensions_user
        self.total_dimensions = self.dimensions_map + self.dimensions_user
        
        self.frame_rate = environment_parameters.frame_rate
        self.title = environment_parameters.title
        self.width_line = environment_parameters.width_line
        self.colors = shared_parameters.gui.colors

        pygame.init()
        self.screen = pygame.display.set_mode(self.total_dimensions)
        self.screen_surface = pygame.Surface(self.total_dimensions)

        self.w_map = self.dimensions_map[0]
        self.w_user = self.dimensions_user[0]
        self.h = self.dimensions_map[1]
        self.surface_map = self.screen_surface.subsurface(pygame.Rect(0, 0,
                            self.w_map, self.h))
        self.surface_map = self.screen_surface.subsurface(pygame.Rect(\
                            self.w_map, 0, self.w_user, self.h))
        self.surface_map.fill(self.colors.wall) 
        self.surface_user.fill(self.colors.user-background) 
        self.button_run = pygame.Rect(50, 120, 100, 50)
        self.button_stop = pygame.Rect(50, 200, 100, 50)
        pygame.display.set_caption(self.title)
        
        # Loop until the user clicks the close button
        self._condition_loop = True  


    #---------------
    @property
    def condition_loop(self):
        return self._condition_loop


    #---------------
    def check_user_event(self):
        for event in pygame.event.get():  # Catch user input
            if event.type == pygame.QUIT: 
                self._condition_loop = True
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:  # 1 is the left mouse button
                    if button_run.collidepoint(list(map(lambda a,b: a-b, event.pos, (self.w_map, 0)))):
                        print("run")
                    if button_stop.collidepoint(list(map(lambda a,b: a-b, event.pos, (self.w_map, 0)))):
                        print("stop")
        return self


    #---------------
    def update(self):
        self.process_queue()

        # Buttons
        pygame.draw.rect(surface_user, self.colors.user-button, button_run)
        text_surf = FONT.render("Run", True, self.colors.user-font)
        text_rect = text_surf.get_rect(center=(10, 10))
        surface_user.blit(text_surf, (75, 135))

        pygame.draw.rect(surface_user, self.colors.user-button, button_stop)
        text_surf = FONT.render("Stop", True, self.colors.user-font)
        text_rect = text_surf.get_rect(center=(50, 25))

        surface_user.blit(text_surf, (75, 215))
        self.screen.blit(self.screen_surface, (0,0))
        pygame.display.update()
        time.sleep(1./self.frame_rate)
        return self
        

    #---------------
    def process_queue(self):
        while not self.q.empty():
            msg = self.q.get()
            start, end, color = parse_msg(msg)
            pygame.draw.line(self.screen_surface, color, start,
                    end, self.width_line)

            
    #---------------
    def quit(self):
        pygame.quit()

    










    



