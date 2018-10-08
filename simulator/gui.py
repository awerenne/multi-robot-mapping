import pygame
import time
import math
# import tkinter as tk
import sys
sys.path.append('../master/')

from maze import Maze
from random import randint
from master import Master
from worker import Worker
from messenger import Messenger

#  TO remove
import numpy as np 

WHITE = 0
WHITE_RGB = (255, 255, 255)
BLACK = 1
BLACK_RGB = (0, 0, 0) 
RED = 2
RED_RGB = (255, 255, 0)
BLUE = 3
BLUE_RGB = (0, 0, 255)
GREEN = 4
GREEN_RGB = (0, 255, 0)

def draw_grid(surface, resolution, grid, grid_dimensions):
    # Compute transformation parameters to properly scale and center grid to 
    # screen
    grid = np.array(grid)
    (h_grid, w_grid) = grid_dimensions
    (w, h) = resolution
    (center_x, center_y) = (w/2, h/2)
    (used_w, used_h) = (math.floor(0.8 * w), math.floor(0.8 * h))
    scaling_factor = min(math.floor(used_w/w_grid), math.floor(used_h/h_grid)) 
    
    # Draw grid
    for y in range(h_grid):
        for x in range(w_grid):

            if grid[y,x] == WHITE:
                color = WHITE_RGB
            elif grid[y,x] == BLACK:
                color = BLACK_RGB
            elif grid[y,x] == RED:
                color = RED_RGB
            elif grid[y,x] == BLUE:
                color = BLUE_RGB
            elif grid[y,x] == GREEN:
                color = GREEN_RGB
            
            top_left_x = (x - w_grid/2)*scaling_factor + center_x
            top_left_y = (y - h_grid/2)*scaling_factor + center_y
            pygame.draw.rect(surface, color, [top_left_x, top_left_y, scaling_factor, scaling_factor])


def start_simulator():

    # Initialize simulation
    maze = Maze.load_from_file("mazes/small.txt")
    master = Master()
    (worker1_id, worker2_id) = maze.generate_workers(2)
    worker1 = Worker(maze, worker1_id)
    worker2 = Worker(maze, worker2_id)
    messenger1 = Messenger(master, worker1)
    messenger2 = Messenger(master, worker2)

    # Initialize display
    pygame.init()
    WIDTH = 1000
    HEIGHT = 800
    screen_resolution = (WIDTH, HEIGHT)
    screen = pygame.display.set_mode(screen_resolution)

    # Vertical split of screen
    screen_surface = pygame.Surface(screen_resolution)
    sub_surface_maze = screen_surface.subsurface(pygame.Rect(0, 0,
                            WIDTH/2, HEIGHT))
    sub_surface_map_1 = screen_surface.subsurface(pygame.Rect(WIDTH/2, 0,
                            WIDTH/2, HEIGHT/2))
    sub_surface_map_2 = screen_surface.subsurface(pygame.Rect(WIDTH/2, HEIGHT/2,
                            WIDTH/2, HEIGHT/2))

    # Set title of screen
    pygame.display.set_caption("Simulation")

    # Loop until the user clicks the close button
    done = False

    # Used to manage how fast the screen updates (per second)
    frame_rate = 1

    while not done:
        for event in pygame.event.get():  # User did something
            if event.type == pygame.QUIT:  # If user clicked close
                done = True

        worker1.run()
        worker2.run()

        # Set the backgrounds
        sub_surface_maze.fill(WHITE_RGB)
        sub_surface_map_1.fill(WHITE_RGB)
        sub_surface_map_2.fill(WHITE_RGB)

        # Drawing separation lines
        pygame.draw.line(sub_surface_maze, GREEN_RGB, (WIDTH/2,0),
                        (WIDTH/2,WIDTH), 30)
        pygame.draw.line(sub_surface_map_1, GREEN_RGB, (0,HEIGHT/2),
                        (WIDTH/2,HEIGHT/2), 30)
        pygame.draw.line(sub_surface_map_2, GREEN_RGB, (WIDTH/2,0),
                        (WIDTH,0), 30)

        # Get new displays
        (maze_grid, maze_dimensions) = maze.get_grid()
        draw_grid(sub_surface_maze, (WIDTH/2, HEIGHT), maze_grid, maze_dimensions)
        (map_1_grid, map_1_dimensions) = master.get_grid(0)
        draw_grid(sub_surface_map_1, (WIDTH/2, HEIGHT/2), map_1_grid, map_1_dimensions)
        (map_2_grid, map_2_dimensions) = master.get_grid(1)
        draw_grid(sub_surface_map_2, (WIDTH/2, HEIGHT/2), map_2_grid, map_2_dimensions)

        # Update screen
        screen.blit(sub_surface_maze, (0,0))
        screen.blit(sub_surface_map_1, (WIDTH/2, 0))
        screen.blit(sub_surface_map_2, (WIDTH/2, HEIGHT/2))
        pygame.display.flip()
        time.sleep(1./frame_rate)  # Limit frame rate



    # Be IDLE friendly. If you forget this line, the program will 'hang' on exit.
    pygame.quit()

start_simulator()
