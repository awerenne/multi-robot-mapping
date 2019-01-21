"""
    Description.
"""


import threading 
from abc import ABCMeta, abstractmethod
from queue import Queue

from map import Map
from messenger import Messenger
from utils import Container


# TODO: bi-directional queue, more elegant (very low priority)
# TODO: proper thread/general quitting (very low priority)
# TODO: parallelization of user input and robot/map/gui update (low priority)
# TODO: safe threading with locks (very low priority)
# TODO: map instruction strings to int (byte in arduino) (low priority)
# TODO: make decision 


#---------------
class Master(metaclass=ABCMeta, threading.Thread):
    """
    Blabla.
    """

    def __init__(self, queues_gui, parameters):  
        self.map = Map(parameters)  
        self.n_robots = parameters.n_robots
        self.id_robots = [(i+1) for i in range(self.n_robots)]
        self.queues_gui = queues 
        queues_messenger = {
            "messenger2master": Queue(),
            "master2messenger": Queue()}
        self.messenger = Messenger(queues_messenger, parameters)


    #---------------
    def run(self):
        while True:       
            receive_information_from_robot()
            receive_user_request_from_gui()


    #--------------- 
    def receive_information_from_robot(self):
        q = self.queues_messenger["messenger2master"]
        if len(q) > 0:
            directive = q.get()
            if directive.type_directive == "information":
                self.process_information(self.directive.id_robot,
                        self.directive.information)


    #--------------- 
    def receive_user_request_from_gui(self):
        q = self.queues_gui["gui2master"]
        if len(q) > 0:
            directive = q.get()
            if directive.type_directive == "request_run":
                self.run_robots()
            if directive.type_directive == "request_stop":
                self.stop_robots()


    #--------------- 
    def send_instruction_to_robot(self, id_robot, instruction):
        q = self.queues_messenger["master2messenger"]
        directive = {
            "type_directive": "instruction",
            "id_robot": id_robot,
            "instruction": instruction
        }
        q.put(Container(directive))


    #--------------- 
    def send_summary_to_gui(self, id_robot, summary):
        q = self.queues_messenger["master2gui"]
        (edges, frontiers, robots) = summary
        directive = {
            "type_directive": "update_view",
            "summary": {
                "edges": edges,
                "frontiers": frontiers,
                "robots": robots
            }
        }
        q.put(Container(directive))


    #--------------- 
    def run_robots(self):
        for id_robot in self.id_robots:
            self.send_instruction_to_robot(id_robot, "straight")


    #--------------- 
    def stop_robots(self):
        for id_robot in self.id_robots:
            self.send_instruction_to_robot(id_robot, "stop")


    #--------------- 
    def process_information(self, id_robot, information):
        # Update map
        self.map.update(id_robot, information.type_intersection,
                information.distance)
        instruction = self.make_decision(id_robot)
        self.map.turn(id_robot, instruction)

        # Update physical robot
        self.send_instruction_to_robot(id_robot, instruction)
        
        # Update GUI
        self.send_summary_to_gui(self.map.summary)


    #---------------
    @abstractmethod
    def make_decision(self):
        pass



#---------------
class NaiveMaster(Master):
    """    
    Description.
    """

    def __init__(self):
        super().__init__()


    def make_decision(self):
        self.send_instruction_to_robot(id_robot, "straight")
        # TODO

 # #---------------
 #    def make_decision(self):
 #        # TODO turn robot
 #        # TODO pop from target
 #        if self.map.is_robot_at_frontier(id_robot):
 #            directions = self.map.unexplored_directions(id_robot)
 #            if id_robot in self.targets:
 #                del self.targets[id_robot]

 #            # Left Hand Rule
 #            if directions["left"] in directions:
 #                return "left"
 #            if directions["front"] in directions:
 #                return "front"
 #            if directions["right"] in directions:
 #                return "right"
 #            else:
 #                return "stop"

 #        # Robot is already on the way to previously assigned frontier
 #        if id_robot in self.targets:
 #            (directions, _) = self.map.shortest_path(self.robot_position(id_robot),
 #                    self.targets[id_robot])
 #            return directions[0]

 #        # Assign new frontier to robot, and direct robot towards this target
 #        frontiers = self.map.frontiers
 #        nearest_frontier = self.map.nearest_frontier()
 #        (directions, _) = self.map.shortest_path(self.robot_position(id_robot),
 #                    nearest_frontier)
 #        self.targets[id_robot] = nearest_frontier
 #        return directions[0]

 #    #---------------
 #    def nearest_frontier(self, id_robot):
 #        assert is_robot(id_robot)
 #        min_distance = float('Inf')
 #        position = self._robots[id_robot].position
 #        nearest_frontier = None
 #        for frontier in self._frontiers:
 #            h = self.manhattan_distance(position, frontier)
 #            if min_distance > h:
 #                min_distance = h
 #                nearest_frontier = frontier
 #        return nearest_frontier













    