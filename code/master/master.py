
from threading import Thread
from abc import ABCMeta, abstractmethod
from queue import Queue
import time
from map import Map
from messenger import MessengerSimul, MessengerReal
from utils import *

#---------------
class Master(Thread, metaclass=ABCMeta):
    def __init__(self, params, queues, lambda_=None):  
        Thread.__init__(self)
        self.travels = {}
        self.params = params
        self.q = queues
        self.id_robots = self.params.robots.keys()
        self.n_robots = len(self.id_robots)
        self.build_messenger().build_map()
        self.first_cycle = {}
        self.last_instruction = {}
        for id_ in self.id_robots:
            self.first_cycle[id_] = True
            self.last_instruction[id_] = 0
        self.stopped = {1:True, 2: True}
        self.flag_recovery = False
        self.meta_recovery = {}
        self._finished = False


    #------
    @property
    def finished(self):
        return self._finished
    
    #------
    def build_map(self):
        self.map = Map(self.params)  
        self.send_summary_to_gui(self.map.summary)
        return self
        
    #------
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

    #------
    def run(self):
        self.messenger.start()
        robots_thread = Thread(target=self.receive_information_from_robot).start()
        gui_thread = Thread(target=self.receive_user_request_from_gui).start()  

    #------
    def receive_information_from_robot(self):
        while True:   
            directive = self.q.messenger2master.get()  
            if directive.type_directive == "information":
                print("R: "+ str(directive))
                self.process_information(directive.id_robot, directive.information)

    #------
    def receive_user_request_from_gui(self):
        while True:        
            directive = self.q.gui2master.get()
            if directive.type_directive == "request_run":
                self.run_robots()
            elif directive.type_directive == "request_stop":
                self.stop_robots()
            elif directive.type_directive == "request_increment":
                self.increment_speed()

    #------ 
    def send_instruction_to_robot(self, id_robot, instruction):
        directive = {
            "type_directive": "instruction",
            "id_robot": id_robot,
            "instruction": instruction
        }
        self.last_instruction[id_robot] = instruction
        if instruction == 0:
            self.stopped[id_robot] = True
        else: 
            self.stopped[id_robot] = False
        print("M: " + str(directive))
        print()
        self.q.master2messenger.put(Container(directive))

    #------
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

    #------
    def run_robots(self):
        self.send_instruction_to_robot(1, self.direction2instruction("straight"))

    #------ 
    def stop_robots(self):
        for id_robot in self.id_robots:
            self.send_instruction_to_robot(id_robot,
                    self.direction2instruction("stop"))

    #------
    def increment_speed(self):
        for id_robot in self.id_robots:
            self.send_instruction_to_robot(id_robot, 5)

    #------
    def process_information(self, id_robot, information):
        if information.distance == -1:
            self.send_instruction_to_robot(id_robot, self.last_instruction[id_robot])

        def onetotwo(id_):
            if id_ == 1: 
                return 2
            return 1

        """ Robot encounter detection """
        if information.type_intersection == -1:
            if self.n_robots == 1:
                self.send_instruction_to_robot(1, self.direction2instruction("stop"))
                return
            self.send_instruction_to_robot(1, self.direction2instruction("stop"))
            self.send_instruction_to_robot(2, self.direction2instruction("stop"))
            time.sleep(1)
            if self._finished:
                return
            self.flag_recovery = True
            last_position = self.map.get_robot_position(id_robot)
            id_mover = id_robot
            if self.map.type_intersection(last_position) == 6:
                id_mover = onetotwo(id_robot)
            id_stopper = onetotwo(id_mover)      
            position_to_mask = self.map.get_robot_position(id_mover)
            self.meta_recovery = {'mover': id_mover, 'stopper': id_stopper, 'step': 0, 'to_mask': position_to_mask}
            self.map.turn_robot(id_mover, "uturn")
            self.send_instruction_to_robot(id_mover, self.direction2instruction("uturn"))
            self.send_summary_to_gui(self.map.summary)
            self.meta_recovery['step'] += 1
            return

        """ Robot encounter recovery """
        if self.flag_recovery:
            id_mover = self.meta_recovery['mover']
            if id_robot != id_mover:
                return
            if self.meta_recovery['step'] == 1:
                if id_robot in self.travels.keys():
                    self.travels.pop(id_robot)
                directions = self.map.explored_directions(id_robot)
                decision = "uturn"
                if "left" in directions:
                    decision = "left"
                if "straight" in directions:
                    decision = "straight"
                if "right" in directions:
                    decision = "right"
                self.map.turn_robot(id_robot, decision)
                self.send_instruction_to_robot(id_robot, self.direction2instruction(decision))
                self.send_summary_to_gui(self.map.summary)
                self.meta_recovery['step'] += 1
                return

            if self.meta_recovery['step'] == 2:
                if id_robot in self.travels.keys():
                    dist = self.travels.pop(id_robot)  # discard if already explored
                else:
                    dist = information.distance
                self.map.update(id_robot, information.type_intersection, dist)
                direction = self.make_decision(id_robot)
                self.map.turn_robot(id_robot, direction)
                instruction = self.direction2instruction(direction)
                self.flag_recovery = False
                self.send_instruction_to_robot(id_robot, instruction)
                self.send_instruction_to_robot(self.meta_recovery['stopper'], self.direction2instruction("straight"))
                self.send_summary_to_gui(self.map.summary)
                return

        if id_robot in self.travels.keys():
            dist = self.travels.pop(id_robot)  # discard if already explored
        else:
            dist = information.distance
        if self.first_cycle[id_robot]:  # Discard distance
            if id_robot == 1:  # Turn left
                self.map.turn_robot(id_robot, "left")
                self.send_instruction_to_robot(1, self.direction2instruction("left"))
                # time.sleep(0.05)
                self.map.set_robot_pose(1, (0,40), 3)
                self.send_summary_to_gui(self.map.summary)
                self.first_cycle[id_robot] = False
                return
            else:
                self.map.update(id_robot, information.type_intersection, 40)
            self.first_cycle[id_robot] = False
        else:
            if (id_robot + 1) in self.id_robots and self.first_cycle[id_robot+1]:
                self.send_instruction_to_robot(id_robot+1, self.direction2instruction("straight"))
                time.sleep(6.5) # 0.25
            self.map.update(id_robot, information.type_intersection, dist)
        ids = [id_robot] 
        if self.stopped[onetotwo(id_robot)] and not self.flag_recovery:
            ids += [onetotwo(id_robot)]
        for id_ in ids:
            direction = self.make_decision(id_)
            self.map.turn_robot(id_, direction)
            instruction = self.direction2instruction(direction)
            self.send_instruction_to_robot(id_, instruction)
        self.send_summary_to_gui(self.map.summary)

    #------ 
    def direction2instruction(self, direction):
        return {"stop": 0, "straight": 1, "left": 2, "right": 3,
            "uturn": 4}[direction]

    #------
    @abstractmethod
    def make_decision(self, id_robot):
        pass



#---------------
class NaiveMaster(Master):
    def __init__(self, params, queues, lambda_=0):
        super().__init__(params, queues)
        self.targets = {}
        self.lambda_ = lambda_

    #------
    def make_decision(self, id_robot):
        for id_, target in self.targets.items():
            if (id_ != id_robot) and (target != self.map.get_robot_position(id_robot)):
                del self.targets[id_]
                break
        if self.map.is_robot_at_frontier(id_robot):
            self.remove_target(id_robot)
            directions = self.map.unexplored_directions(id_robot)
            return self.left_hand_rule(directions)
        if self.target_reached(id_robot):
            self.remove_target(id_robot)
        if not self.target_assigned(id_robot):
            self.assign_target_to_robot(id_robot)
        return self.next_direction_to_target(id_robot)
   
    #------
    def left_hand_rule(self, directions):
        if "left" in directions:
            return "left"
        if "straight" in directions:
            return "straight"
        if "right" in directions:
            return "right"
        return "uturn"

    #------
    def remove_target(self, id_robot):
        if self.target_assigned(id_robot):
            del self.targets[id_robot]

    #------
    def target_reached(self, id_robot):
        return self.target_assigned(id_robot) and \
            self.targets[id_robot] == self.map.get_robot_position(id_robot)

    #------
    def target_assigned(self, id_robot):
        return id_robot in self.targets.keys() 

    #------
    def get_remaining_targets(self):
        A = set(self.map.frontiers)
        B = set(self.targets.values()) 
        return list(A.difference(B))

    #------
    def assign_target_to_robot(self, id_robot):
        start_ = self.map.get_robot_position(id_robot)
        if len(self.id_robots) <= 1:
            other_robot = start_
        elif id_robot == 1: 
            other_robot = self.map.get_robot_position(2)
        else: 
            other_robot = self.map.get_robot_position(1)
        remaining_targets = self.get_remaining_targets()
        if len(remaining_targets) == 0 and len(self.map.frontiers) > 0:
            self.targets[id_robot] = self.nearest(start_, self.map.frontiers, other_robot)
            return
        if len(self.map.frontiers) == 0:
            self.targets[id_robot] = (0,0)
            return
        self.targets[id_robot] = self.nearest(start_, remaining_targets, other_robot)

    #------
    def nearest(self, start_, positions, other_robot):
        costs = [heuristic(start_, x, other_robot, self.lambda_) for x in positions]
        if len(costs) == 0:
            return start_
        return positions[costs.index(min(costs))]

    #------
    def get_undesired(self, id_robot):
        if self.n_robots == 1:
            return None
        if self.flag_recovery:
            return self.meta_recovery['to_mask']
        if self.stopped[id_robot]:
            return self.map.get_robot_position(id_robot)
        orien = self.map.get_robot_orientation(id_robot)
        next_ = self.map.get_next_neighbor(self.map.get_robot_position(id_robot), orien)
        if next_ is None:
            (x, y) = self.map.get_robot_position(id_robot)
            if orien == 0:
                next_ = (x, y+20)
            elif orien == 2:
                next_ = (x, y-20)
            elif orien == 1:
                next_ = (x+20, y)
            elif orien == 3:
                next_ = (x-20, y)
            if self.map.is_node(next_):
                return next_
            return None
        return next_

    #------
    def next_direction_to_target(self, id_robot):
        assert self.target_assigned(id_robot)
        start_ = self.map.get_robot_position(id_robot)
        end_ = self.targets[id_robot]
        if start_ == end_:
            self.remove_target(id_robot)
            return "stop"
        if id_robot == 1:
            undesired = self.get_undesired(2)
        else: 
            undesired = self.get_undesired(1)
        neighbor = self.map.shortest_path(start_, end_, manhattan_distance, undesired)
        if neighbor is None:
            return "stop"
            # return self.left_hand_rule(self.map.explored_directions(id_robot))
        neighbor = neighbor[1]
        if neighbor == (0,0):
            self._finished = True

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
        self.travels[id_robot] = manhattan_distance(start_, neighbor)
        return direction


        













    