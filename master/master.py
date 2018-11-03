from mapper import Mapper
import pygame
import time
import numpy as np
import cv2
from constants import *

class Master:

    def __init__(self):
        self.id = -1
        self.mappers = {}


    def connect(self, slave_id):
        self.mappers[slave_id] = Mapper()


    def make_msg(self, type_, content):
        msg = {
            "src": self.id,
            "type": type_,
            "content": content
        }
        return msg


    def deliver_msg(self, msg):
        src = msg["src"]
        mapper = self.mappers[src]
        content = msg["content"]

        if msg["type"] == "map-update":
            type_intersection = content["type-intersection"]
            traveled_distance = content["traveled-distance"]
            print((type_intersection, traveled_distance))
            return_code = mapper.update_map(src, type_intersection,
                traveled_distance)

            if (return_code < 0):
                new_content = {"new-direction": "stop"}
                return self.make_msg("policy-update", new_content)

            new_direction = mapper.get_next_move(src)
            print(new_direction)
            new_content = {"new-direction": new_direction}
            return self.make_msg("policy-update", new_content)

        elif msg['type'] == "error":
            new_content = {"new-direction": "stop"}
            return self.make_msg("policy-update", new_content)


    def get_grid(self, slave_id):
        return self.mappers[slave_id].get_grid()







    