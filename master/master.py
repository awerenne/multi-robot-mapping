from mapper import Mapper
from messenger import Messenger
import pygame
import time
import numpy as np
import cv2

# Define some colors
WHITE = 0
BLACK = 1
WORKER = 2
DESTINATION = 3
PATH = 4

class Master:

    def __init__(self):
        self.id = -1
        self.workers = []
        self.mappers = {}
        self.messengers = {}
        self.timers = {}
        

        self.free_specific_ports = [33, 45]
        self.time_limit = 20  # in seconds

    
    def get_id(self):
        return self.id


    def connect(self, messenger, worker_id):
        self.messengers[worker_id] = messenger
        self.mappers[worker_id] = Mapper()
        self.timers[worker_id] = time.time()
        self.workers.append(worker_id)


    def receive_msg(self, sender_id, type_message, content_message):
        if type_message == "map_info":
            traveled_distance = content_message["traveled_distance"]
            taken_direction = content_message["taken_direction"]
            error_flag = self.update_map(sender_id, traveled_distance,
                taken_direction)

            # Limit active time of a worker (test)
            if time.time() - self.timers[sender_id] > self.time_limit:
                self.send_msg(sender_id, "instruction", "stop")


    def send_msg(self, receiver_id, type_message, content_message):
        self.messengers[receiver_id].send(receiver_id, type_message,
            content_message)


    def get_grid(self, worker_id):
        mapper = self.mappers[worker_id]
        grid = np.zeros((30,30), np.uint8)
        (origin, edges) = mapper.get_map()
        for edge in edges:
            (start_x, start_y) = edge[0]
            (start_x, start_y) = (start_x + 15, -start_y + 15)
            (end_x, end_y) = edge[1]
            (end_x, end_y) = (end_x + 15, -end_y + 15)
            cv2.line(grid, (start_x, start_y), (end_x, end_y), BLACK, 1)
        
        # print(grid)
        # while True:
        #     a =5
        return (grid, grid.shape)


    def draw_map(self, map, surface, resolution):
        scaling_factor = 20
        (w,h) = resolution
        (referential, edges) = map
        center_ij = (h/2, w/2)
        print(center_ij)

        for edge in edges:
            (start_xy, end_xy) = edge
            start_ij = self.coords_to_screen_format(start_xy, center_ij)
            end_ij = self.coords_to_screen_format(end_xy, center_ij)
            print((start_ij, end_ij))

            pygame.draw.line(surface, BLACK, start_ij, end_ij, scaling_factor)


    def draw_path(self, path, surface, resolution):
        scaling_factor = 80
        (w,h) = resolution
        center_ij = (h/2, w/2)

        prev_xy = path[0]
        prev_ij = self.coords_to_screen_format(prev_xy, center_ij)
        for vertice_xy in path:
            vertice_ij = self.coords_to_screen_format(vertice_xy, center_ij)
            pygame.draw.line(surface, PATH, prev_ij, vertice_ij, 40)
            prev_ij = vertice_ij


    def coords_to_screen_format(self, coords, center):
        (x,y) = coords
        (center_i, center_j) = center
        i = -x + center_i
        j = x + center_j
        return (int(i), int(j))


    def update_map(self, worker_id, traveled_distance, taken_direction):
        error_flag = self.mappers[worker_id].update_map(traveled_distance,
            taken_direction)
        return error_flag