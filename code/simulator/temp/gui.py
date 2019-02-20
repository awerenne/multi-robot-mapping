import pygame
import time
import math
import sys
import numpy as np 

sys.path.append('../master/')
from maze import Maze
from random import randint
from master import Master
from slave import Slave
from constants import *




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
            if grid[y,x] == WALL:
                color = WHITE_RGB
            elif grid[y,x] == EMPTY:
                color = BLACK_RGB
            elif grid[y,x] == SLAVE:
                color = BLUE_RGB
            elif grid[y,x] == UNEXPLORED:
                color = GREY_RGB
            elif grid[y,x] == PATH:
                color = GREEN_RGB
            
            top_left_x = (x - w_grid/2)*scaling_factor + center_x
            top_left_y = (y - h_grid/2)*scaling_factor + center_y
            pygame.draw.rect(surface, color, [top_left_x, top_left_y, scaling_factor, scaling_factor])


def start_simulator():

    # Initialize simulation
    maze = Maze.load_from_file("mazes/small.txt")
    master = Master()
    (slave1_id, slave2_id) = maze.generate_slaves(2)
    slave1 = Slave(maze, slave1_id)
    slave1.connect(master)
    slave2 = Slave(maze, slave2_id)
    slave2.connect(master)


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
    frame_rate = 2

    while not done:
        for event in pygame.event.get():  # User did something
            if event.type == pygame.QUIT:  # If user clicked close
                done = True

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

        # Slaves moving
        slave1.run()

        sub_surface_maze.fill(WHITE_RGB)
        pygame.draw.line(sub_surface_maze, GREEN_RGB, (WIDTH/2,0),
                        (WIDTH/2,WIDTH), 30)
        (maze_grid, maze_dimensions) = maze.get_grid()
        draw_grid(sub_surface_maze, (WIDTH/2, HEIGHT), maze_grid, maze_dimensions)
        screen.blit(sub_surface_maze, (0,0))
        time.sleep(1./frame_rate)  # Limit frame rate
        pygame.display.flip()

        slave2.run()


    # Be IDLE friendly. If you forget this line, the program will 'hang' on exit.
    pygame.quit()

start_simulator()
