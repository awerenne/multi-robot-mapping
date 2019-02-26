"""
    GUI for manual user interface (with communication to/from robot).
"""


"""
    Description.
"""


import pygame
import time
import serial

serial_port = '/dev/cu.DSDTECHHC-06-DevB';
baud_rate = 9600; 
ser = serial.Serial(serial_port, baud_rate)

# ------------
def send_instruction(instruction):
    msg = "<1/0/" + str(instruction) + "/0/0/0/0>"
    print(msg)
    ser.write(msg.encode())


# ------------
class GUI():
    
    def __init__(self):
        self.init_params().init_display()
        self._close = False


    #---------------
    @property
    def close(self):
        return self._close
    

    #---------------
    def init_params(self):
        self.dimensions_screen = (400,300)
        self.dimensions_user = (300,200)
        (self.w, self.h) = self.dimensions_screen
        (self.wu, self.hu) = self.dimensions_user

        self.rect_run = [50, 120, 100, 50]
        self.rect_stop = [50, 200, 100, 50]

        self.frame_rate = 20
        self.delay = 1.0 / self.frame_rate  # Unit: seconds
        
        self.title = "COEX"
        self.width_line = 4
        self.colors = {
            "background": [120, 120, 120],
            "button": [120, 0, 0],
            "font": [255, 255, 255]
        }
        self.font_name = "couriernew"
        self.font_size = 15
        return self
        

    #---------------
    def init_display(self):
        pygame.init()

        self.screen = pygame.display.set_mode(self.dimensions_screen)
        self.surface_screen = pygame.Surface(self.dimensions_screen)
        self.surface_user = self.surface_screen.subsurface(pygame.Rect(50,
                            50, self.wu, self.hu))
        self.surface_screen.fill((45,45,45)) 
        self.surface_user.fill((45,45,45)) 

        self.icon_up = pygame.image.load('icons/up-small.png').convert_alpha()  
        self.rect_up = pygame.Rect((110, 10, 70, 50))
        self.icon_left = pygame.image.load('icons/left-small.png').convert_alpha()  
        self.rect_left = pygame.Rect((50, 70, 70, 50))
        self.icon_stop = pygame.image.load('icons/stop_small.png').convert_alpha() 
        self.rect_stop = pygame.Rect((125, 70, 45, 50))
        self.icon_right = pygame.image.load('icons/right-small.png').convert_alpha()
        self.rect_right = pygame.Rect((175, 70, 70, 50))
        self.icon_down = pygame.image.load('icons/down-small.png').convert_alpha()  
        self.rect_down = pygame.Rect((110, 130, 70, 50))
        
        self.timer = pygame.Rect((50, 200, 200, 50))

        pygame.display.set_caption(self.title)
        self.font_timer = pygame.font.SysFont(self.font_name, 35)
        
        self.t_start = time.perf_counter()
        return self


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
    def handle_click(self, position):
        if self.is_mouse_on_button(position, self.rect_left):
            send_instruction(3)
        elif self.is_mouse_on_button(position, self.rect_right):
            send_instruction(2)
        elif self.is_mouse_on_button(position, self.rect_down):
            send_instruction(4)
        elif self.is_mouse_on_button(position, self.rect_stop):
            send_instruction(0)
        elif self.is_mouse_on_button(position, self.rect_up):
            send_instruction(1)


    #---------------
    def is_mouse_on_button(self, position, button):
        relative_position = (position[0]-50, position[1]-50) 
        if button.collidepoint(relative_position): return True
        return False


    #---------------
    def update(self):
        self.draw_user_interface()
        self.screen.blit(self.surface_screen, (0,0))
        pygame.display.update()
        return self
          

    #---------------
    def draw_user_interface(self):
        self.draw_buttons()
        self.draw_timer()
        self.screen.blit(self.surface_user, (0, 0))


    #---------------
    def draw_buttons(self):
        self.draw_button(self.rect_up, self.icon_up)
        self.draw_button(self.rect_down, self.icon_down)
        self.draw_button(self.rect_left, self.icon_left)
        self.draw_button(self.rect_right, self.icon_right)
        self.draw_button(self.rect_stop, self.icon_stop)
        


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
        pygame.draw.rect(self.surface_user, (45,45,45), self.timer)
        delayed_time = time.perf_counter() - self.t_start
        string_time = self.timer_format(delayed_time)
        text_surf = self.font_timer.render(string_time, True, self.colors["font"])
        self.surface_user.blit(text_surf, (self.timer.x+15, self.timer.y+5,
                self.timer.w, self.timer.h))
    

    #---------------
    def timer_format(self, t):
        def format(x): return "0"+str(x) if x < 10 else str(x)
        hours = format(int(t/3600))
        minutes = format(int(t/60)%60)
        seconds = format(int(t)%60)
        return hours + ":" + minutes + ":" + seconds


    #---------------
    def draw_log(self):
        pygame.draw.rect(self.surface_user, (171,171,171), self.log)
        text_surf = self.font_log.render("Log...", True, self.colors["font"])
        self.surface_user.blit(text_surf, (self.log.x+25, self.log.y+25))



# ------------
if __name__ == "__main__":
    gui = GUI()
    while not gui.close:
        gui.check_user_event().update()


























