"""
    Description.
"""


from threading import Thread
from abc import ABCMeta, abstractmethod
from queue import Queue

from map import Map
from messenger import MessengerSimul, MessengerReal
from utils import Container, manhattan_distance


# TODO: proper thread/general quitting - join (low priority)


#---------------
class Master(Thread, metaclass=ABCMeta):
    """
    Blabla.
    """

    def __init__(self, params, queues):  
        Thread.__init__(self)
        self.travels = {}
        self.params = params
        self.q = queues
        self.id_robots = self.params.robots.keys()
        self.build_messenger().build_map()


    #---------------
    def build_map(self):
        self.map = Map(self.params)  
        self.send_summary_to_gui(self.map.summary)
        return self
        

    #---------------
    def build_messenger(self):
        self.q["messenger2master"] = Queue()
        self.q["master2messenger"] = Queue()
        q_messenger = Container(self.q)

        del self.q["messenger2robots"]
        del self.q["robots2messenger"]
        del q_messenger["gui2master"]
        del q_messenger["master2gui"]
        self.q = Container(self.q)

        if self.params.is_simulation:
            self.messenger = MessengerSimul(self.params, Container(q_messenger))
        else:
            self.messenger = MessengerReal(self.params, Container(q_messenger))
        return self


    #---------------
    def run(self):
        self.messenger.start()
        robots_thread = Thread(target=self.receive_information_from_robot).start()
        gui_thread = Thread(target=self.receive_user_request_from_gui).start()  


    #--------------- 
    def receive_information_from_robot(self):
        while True:   
            directive = self.q.messenger2master.get()  
            if directive.type_directive == "information":
                self.process_information(directive.id_robot, directive.information)


    #--------------- 
    def receive_user_request_from_gui(self):
        while True:        
            directive = self.q.gui2master.get()
            if directive.type_directive == "request_run":
                self.run_robots()
            elif directive.type_directive == "request_stop":
                self.stop_robots()
            elif directive.type_directive == "request_increment":
                self.increment_speed()


    #--------------- 
    def send_instruction_to_robot(self, id_robot, instruction):
        directive = {
            "type_directive": "instruction",
            "id_robot": id_robot,
            "instruction": instruction
        }
        self.q.master2messenger.put(Container(directive))


    #--------------- 
    def send_summary_to_gui(self, summary):
        (edges, frontiers, robots) = summary
        directive = {
            "type_directive": "update_summary",
            "summary": {
                "edges": edges,
                "frontiers": frontiers,
                "robots": robots
            }
        }
        self.q.master2gui.put(Container(directive))


    #--------------- 
    def run_robots(self):
        for id_robot in self.id_robots:
            self.send_instruction_to_robot(id_robot,
                    self.direction2instruction("straight"))


    #--------------- 
    def stop_robots(self):
        for id_robot in self.id_robots:
            self.send_instruction_to_robot(id_robot,
                    self.direction2instruction("stop"))


    #--------------- 
    def increment_speed(self):
        for id_robot in self.id_robots:
            self.send_instruction_to_robot(id_robot, 5)


    #--------------- 
    def process_information(self, id_robot, information):
        if id_robot in self.travels.keys():
            dist = self.travels.pop(id_robot)  # discard if already explored
        else:
            dist = information.distance
        self.map.update(id_robot, information.type_intersection, dist)
        direction = self.make_decision(id_robot)
        self.map.turn_robot(id_robot, direction)
        instruction = self.direction2instruction(direction)
        self.send_instruction_to_robot(id_robot, instruction)
        self.send_summary_to_gui(self.map.summary)


    #--------------- 
    def direction2instruction(self, direction):
        return {"stop": 0, "straight": 1, "left": 2, "right": 3,
            "uturn": 4}[direction]


    #---------------
    @abstractmethod
    def make_decision(self, id_robot):
        pass



#---------------
class NaiveMaster(Master):
    """    
    Description.
    """

    def __init__(self, params, queues):
        super().__init__(params, queues)
        self.targets = {}


    #---------------
    def make_decision(self, id_robot):
        if self.map.is_robot_at_frontier(id_robot):
            self.remove_target(id_robot)
            directions = self.map.unexplored_directions(id_robot)
            return self.left_hand_rule(directions)

        if self.target_reached(id_robot):
            self.remove_target(id_robot)

        if not self.target_assigned(id_robot):
            self.assign_target_to_robot(id_robot)

        return self.next_direction_to_target(id_robot)
   

    #---------------
    def left_hand_rule(self, directions):
        if "left" in directions:
            return "left"
        if "straight" in directions:
            return "straight"
        if "right" in directions:
            return "right"
        return "uturn"


    #---------------
    def remove_target(self, id_robot):
        if self.target_assigned(id_robot):
            del self.targets[id_robot]


    #---------------
    def target_reached(self, id_robot):
        return self.target_assigned(id_robot) and \
            self.targets[id_robot] == self.map.get_robot_position(id_robot)


    #---------------
    def target_assigned(self, id_robot):
        return id_robot in self.targets.keys() 


    #---------------
    def get_remaining_targets(self):
        A = set(self.map.frontiers)
        B = set(self.targets.values()) 
        return list(A.difference(B))


    #---------------
    def assign_target_to_robot(self, id_robot):
        start_ = self.map.get_robot_position(id_robot)
        remaining_targets = self.get_remaining_targets()
        if len(remaining_targets) == 0:
            self.targets[id_robot] = self.nearest(start_, self.map.frontiers)
        self.targets[id_robot] = self.nearest(start_, remaining_targets)


    #---------------
    def nearest(self, start_, positions):
        distances = [manhattan_distance(start_, x) for x in positions]
        return positions[distances.index(min(distances))]


    #---------------
    def next_direction_to_target(self, id_robot):
        assert self.target_assigned(id_robot)
        start_ = self.map.get_robot_position(id_robot)
        end_ = self.targets[id_robot]
        if start_ == end_:
            self.remove_target(id_robot)
            return self.make_decision(id_robot)

        neighbor = self.map.shortest_path(start_, end_, manhattan_distance)[1]
        
        # TODO: CHANGE and put in map
        sx, sy = start_
        nx, ny = neighbor
        current_orientation = self.map.get_robot_orientation(id_robot)
        if ny > sy:
            target_orientation = 0
        elif ny < sy:
            target_orientation = 2
        elif nx > sx:
            target_orientation = 1
        elif nx < sx:
            target_orientation = 3
        else:
            assert False
        direction = self.map.get_robot(id_robot).or2dir(current_orientation, target_orientation)

        # print("Current position: " + str(start_))
        # print("Target position: " + str(end_))
        # print("Next position: " + str(neighbor))

        self.travels[id_robot] = manhattan_distance(start_, neighbor)
        return direction


        













    