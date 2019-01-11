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

        self.dimensions = environment_parameters.dimensions
        self.frame_rate = environment_parameters.frame_rate
        self.title = environment_parameters.title
        self.width_line = environment_parameters.width_line
        self.colors = shared_parameters.gui.colors

        pygame.init()
        self.screen = pygame.display.set_mode(self.dimensions)
        self.screen_surface = pygame.Surface(self.dimensions)
        self.screen_surface.fill(self.colors.wall) 
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
        return self


    #---------------
    def update(self):
        self.process_queue()
        self.screen.blit(self.screen_surface, (0,0))
        pygame.display.flip()
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

    










    



