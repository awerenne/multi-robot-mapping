"""
    Description.
"""


import pygame
import time
import math
import sys
import numpy as np 
import threading
from queue import Queue
from random import randint



q = Queue()


# ------------
def f_1():
    q.put("Z;10")
    time.sleep(1)
    q.put("Z;10")
    time.sleep(1)
    q.put("Z;10")
    time.sleep(1)
    q.put("A;10")
    time.sleep(1)
    q.put("Z;10")
    time.sleep(1)
    q.put("Z;10")
    time.sleep(1)
    q.put("C;10")
    time.sleep(1)
    q.put("Z;10")
    time.sleep(1)
    q.put("Z;20")
    time.sleep(1)


# ------------
def parse_msg(msg):
    (type_, distance) = msg.split(';')
    distance = int(distance)
    return (type_, distance)


# ------------
def update_position(distance, orientation, previous_position):
    (i, j) = previous_position 
    if orientation == 'N':
        j -= distance
    elif orientation == 'S':
        j += distance
    elif orientation == 'E':
        i += distance
    elif orientation == 'W':
        i -= distance
    current_position = (i, j)
    return current_position



# ------------
def update_orientation(type_, orientation, or2idx, idx2or):
    idx = or2idx[orientation]
    if type_ == 'A':
        idx = ((idx - 1) + 4) % 4
    elif type_ == 'B':
        idx = ((idx - 1) + 4) % 4
    elif type_ == 'C':
        idx = ((idx + 1) + 4) % 4
    elif type_ == 'D':
        idx = ((idx - 1) + 4) % 4
    elif type_ == 'E':
        idx = ((idx - 1) + 4) % 4
    elif type_ == 'F':
        idx = ((idx + 1) + 4) % 4
    elif type_ == 'G':
        idx = ((idx + 2) + 4) % 4
    elif type_ == 'H':
        idx = 0
    orientation = idx2or[idx]
    return orientation


# ------------
def gui():

    WIDTH = 600
    HEIGHT = 400
    previous_position = (WIDTH/2, HEIGHT/2)
    current_position = (WIDTH/2, HEIGHT/2)
    orientation = 'N'
    idx2or = {0: 'N', 1: 'E', 2: 'S', 3: 'W'}
    or2idx = {'N': 0, 'E': 1, 'S': 2, 'W': 3}
    WHITE_RGB = (255, 255, 255)
    BLACK_RGB = (0, 0, 0) 

    # Initialize display
    pygame.init()
    screen_resolution = (WIDTH, HEIGHT)
    screen = pygame.display.set_mode(screen_resolution)
    screen_surface = pygame.Surface(screen_resolution)
    screen_surface.fill(WHITE_RGB)

    # Set title of screen
    pygame.display.set_caption("Map")

    # Loop until the user clicks the close button
    done = False

    # Used to manage how fast the screen updates (per second)
    frame_rate = 2

    while not done:
        for event in pygame.event.get():  # User did something
            if event.type == pygame.QUIT:  # If user clicked close
                done = True 

        # Process all the new information in the queu (FIFO)
        while not q.empty():
            msg = q.get()
            type_, distance = parse_msg(msg)
            current_position = update_position(distance, orientation,
                    previous_position)
            if type_ != 'Z':
                orientation = update_orientation(type_, orientation,
                        or2idx, idx2or)
            print(orientation)
            pygame.draw.line(screen_surface, BLACK_RGB, previous_position,
                            current_position, 5)
            previous_position = current_position

        # Update screen
        screen.blit(screen_surface, (0,0))
        pygame.display.flip()
        time.sleep(1./frame_rate)  # Limit frame rate

    # Be IDLE friendly. If you forget this line, the program will 'hang' on exit.
    pygame.quit()


# ------------
# Create two threads: one for the user interface and the second for reception 
# messages
try:
    t1=threading.Thread(target=f_1).start()
except:
    print("Error: unable to start thread")

gui()














