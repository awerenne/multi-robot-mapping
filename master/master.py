from mapper import Mapper
import pygame
import time
import numpy as np
import cv2
from constants import *
from utils import *

class Master:

    def __init__(self):
        self.id = -1
        self.mappers = {}
        self.unused_mapper_id = 0


    def connect(self, slave_id):
        self.mappers[slave_id] = Mapper(self.unused_mapper_id, slave_id)
        self.unused_mapper_id += 1


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
            mapper.update_map(src, type_intersection,
                traveled_distance)

            new_direction = self.get_next_move(src)
            # print(new_direction)
            new_content = {"new-direction": new_direction}
            return self.make_msg("policy-update", new_content)

        elif msg['type'] == "error":
            new_content = {"new-direction": "stop"}
            return self.make_msg("policy-update", new_content)

        elif msg['type'] == "robot-encounter":
            print("robot-encounter")
            ref_id = src
            ref_distance = content["traveled-distance"]
            ref_mapper = mapper

            ext_id = content["other-id"]
            ext_distance = content["traveled-distance"] # Change!! Find a way to get distance of other_id
            ext_mapper = self.mappers[ext_id]

            if (ref_mapper == ext_mapper):  # Already merged
                print(src)
                # HERE NOT!!
                print("uturna")
                mapper.update_orientation(src, "uturn")
                new_content = {"new-direction": "uturn"}
                return self.make_msg("policy-update", new_content)

            ref_params = ref_mapper.get_parameters()
            ext_params = ext_mapper.get_parameters()

            merged_params = transform(ref_id, ref_distance, ref_params,
                ext_id, ext_distance, ext_params)
            merged_mapper = Mapper(self.unused_mapper_id, ref_id, merged_params)
            self.unused_mapper_id += 1

            merged_mapper.update_orientation(ref_id, "uturn")

            self.mappers.pop(ref_id)
            self.mappers[ref_id] = merged_mapper
            self.mappers.pop(ext_id)
            self.mappers[ext_id] = merged_mapper
            print(ref_id)
            print("uturn")
            new_content = {"new-direction": "uturn"}
            return self.make_msg("policy-update", new_content)


    def get_grid(self, slave_id):
        return self.mappers[slave_id].get_grid()


    def get_next_move(self, slave_id):
        mapper = self.mappers[slave_id]
        slave = mapper.get_slave(slave_id)
        surroundings = mapper.check_surroundings(slave["x"], slave["y"], slave["orientation"])
        is_frontier = False
        is_deadend = False
        temp_sum = 0
        for key, value in surroundings.items():
            temp_sum += value
            if value == UNEXPLORED:
                is_frontier = True
                break
        if temp_sum == EMPTY: 
            next_move = "uturn"
            mapper.update_orientation(slave_id, next_move)
            return next_move

        if is_frontier:
            next_move = mapper.frontier_rule(surroundings)
            mapper.update_orientation(slave_id, next_move)
            return next_move

        frontiers = mapper.get_frontiers()
        min_distance = np.inf
        min_path = []
        if frontiers == None:
            return "stop"
        current_vertice_key = str(slave["x"])+","+str(slave["y"])
        current_vertice_id = mapper.vertices_coords_to_id[current_vertice_key]
        for frontier in frontiers:
            (distance, path) = mapper.get_shortest_path(current_vertice_id,
                frontier)
            if distance < min_distance:
                min_path = path
        if len(min_path) < 2:
            return "stop"

        next_move = mapper.edge_to_move((min_path[0], min_path[1]))
        mapper.update_orientation(slave_id, next_move)
        return next_move







    