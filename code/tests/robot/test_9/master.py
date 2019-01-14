"""
    Master of simple left-hand-rule system. It does sends and receives the 
    following:
        - SENDS start/stop instruction
        - RECEIVES info about encounterd obstacle, detected intersection and
            travelled distance
    All of this information is displayed via a Graphical User Interface (GUI).
"""


import serial
import time
import pygame
import threading
from queue import Queue
import math


# Shared queue
q = Queue()

# Define constants
WIDTH = 600
HEIGHT = 400
WIDTH_USER = 200
ORIENTATION_int2str = {0: 'N', 1: 'E', 2: 'S', 3: 'W'}
ORIENTATION_str2int = {'N': 0, 'E': 1, 'S': 2, 'W': 3}
WHITE_RGB = (255, 255, 255)
BLACK_RGB = (0, 0, 0) 
BLUE_RGB = (0, 50, 200) 
RED_RGB = (200, 50, 0) 
GREEN_RGB = (50, 200, 0)
ID_MASTER = 0
SERIAL_PORT = '/dev/cu.DSDTECHHC-05-DevB';
BAUD_RATE = 9600; 


# ------------
def send(ser, seq_number, instruction):
    msg = "<" + str(ID_MASTER) + "," + str(seq_number) + \
        "," + str(instruction) + ">"
    ser.write(msg.encode())


# ------------
def receive(ser):
    while True:
        raw_msg = ser.readline().decode("utf-8");
        
        # Error checking
        raw_msg = raw_msg.rstrip()  # remove end of line characters
        if (len(raw_msg) < 1 or raw_msg[0] != '<' or raw_msg[-1] != '>' ):
            continue  # TODO change - handle error
        valid_msg = raw_msg[1:-1]  # remove '<', '>' delimeters

        # Process
        (src, seq_number, info) = valid_msg.split(',')

        #  TODO: check seq_number and src

        q.put(info)
        time.sleep(0.5)


# ------------
def parse_msg(msg):
    (type_, distance) = msg.split(';')
    distance = math.floor(float(distance))
    return (type_, distance)


# ------------
def update_position(distance, orientation_str, previous_position):
    (i, j) = previous_position 
    if orientation_str == 'N':
        j -= distance
    elif orientation_str == 'S':
        j += distance
    elif orientation_str == 'E':
        i += distance
    elif orientation_str == 'W':
        i -= distance
    current_position = (i, j)
    return current_position


# ------------
def update_orientation(type_, orientation_int):
    or_str = ORIENTATION_int2str[orientation_int]
    if type_ == 'A':
        or_str = ((or_str - 1) + 4) % 4
    elif type_ == 'B':
        or_str = ((or_str - 1) + 4) % 4
    elif type_ == 'C':
        or_str = ((or_str + 1) + 4) % 4
    elif type_ == 'D':
        or_str = ((or_str - 1) + 4) % 4
    elif type_ == 'E':
        or_str = ((or_str - 1) + 4) % 4
    elif type_ == 'F':
        or_str = ((or_str + 1) + 4) % 4
    elif type_ == 'G':
        or_str = ((or_str + 2) + 4) % 4
    elif type_ == 'H':
        or_str = 0
    or_int = ORIENTATION_str2int[or_str]
    return or_int


# ------------
def gui(ser):

    # Initial Bluetooth communication
    seq_number = 0 

    # Initialize display
    pygame.init()
    font = pygame.font.Font("freesansbold.ttf", 25)
    screen_resolution = (WIDTH + WIDTH_USER, HEIGHT)
    screen = pygame.display.set_mode(screen_resolution)
    screen_surface = pygame.Surface(screen_resolution)
    surface_maze = screen_surface.subsurface(pygame.Rect(0, 0,
                            WIDTH, HEIGHT))
    surface_maze.fill(WHITE_RGB)
    surface_user = screen_surface.subsurface(pygame.Rect(WIDTH, 0,
                            WIDTH_USER, HEIGHT))
    surface_user.fill(BLUE_RGB)
    button_run = pygame.Rect(50, 120, 100, 50)
    button_stop = pygame.Rect(50, 200, 100, 50)

    # Set title of screen
    pygame.display.set_caption("Map")

    # Loop until the user clicks the close button
    done = False

    # Used to manage how fast the screen updates (per second)
    frame_rate = 2

    # Initialize robot
    previous_position = (WIDTH/2, HEIGHT/2)
    current_position = (WIDTH/2, HEIGHT/2)
    orientation_int = 0

    while not done:
        for event in pygame.event.get():  # User did something
            if event.type == pygame.QUIT:  # If user clicked close
                done = True 
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:  # 1 is the left mouse button
                    if button_run.collidepoint(list(map(lambda a,b: a-b,
                            event.pos, (WIDTH, 0)))):
                        send(ser, seq_number, 1)
                        seq_number += 1
                    if button_stop.collidepoint(list(map(lambda a,b: a-b,
                            event.pos, (WIDTH, 0)))):
                        send(ser, seq_number, 0)
                        seq_number += 1

        # Process all received messages
        while not q.empty():
            msg = q.get()
            type_, distance = parse_msg(msg)
            print(msg)
            current_position = update_position(distance*2,
                    ORIENTATION_int2str[orientation_int], previous_position)
            if type_ != 'Z':
                orientation = update_orientation(type_, orientation_int)
            # TODO: obstacle (type == Y)
            pygame.draw.line(surface_maze, BLACK_RGB, previous_position,
                            current_position, 10)
            previous_position = current_position

        # Buttons
        pygame.draw.rect(surface_user, RED_RGB, button_run)
        text_surf = font.render("Run", True, WHITE_RGB)
        text_rect = text_surf.get_rect(center=(10, 10))
        surface_user.blit(text_surf, (75, 135))

        pygame.draw.rect(surface_user, RED_RGB, button_stop)
        text_surf = font.render("Stop", True, WHITE_RGB)
        text_rect = text_surf.get_rect(center=(50, 25))
        surface_user.blit(text_surf, (75, 215))

        # Update screen
        screen.blit(screen_surface, (0,0))
        pygame.display.flip()
        time.sleep(1./frame_rate)  # Limit frame rate

    pygame.quit()


# ------------
if __name__ == "__main__":
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
    try:
        # Reception of messages from robot are catched on separate thread
        t1=threading.Thread(target=receive, args=[ser]).start()  
    except:
        print("Error: unable to start reception thread")

    gui(ser)














