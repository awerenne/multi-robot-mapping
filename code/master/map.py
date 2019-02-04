"""
    Defining the class containing all the information gathered by the 
    robots regarding the map.
"""


import numpy as np
from copy import deepcopy

from graph import Graph, Node
from utils import PriorityQueue, center2xy


# TODO all the special manipulations of map/graph like turn, rotate_axis, 
#   need to me summarized in big test file (because it's tricky) (low priority)


#---------------
class Map:
    """ 
    Describe class & parameters. coordinates are in the form xy center otherwise 
    mentionned.
    """

    def __init__(self, params):

        self.dimensions = params.dimensions
        map_orientation = params.helpers.map_orientation
        map_type2edges = params.helpers.map_type2edges
        self.graph = Graph(map_orientation, map_type2edges)
        self._frontiers = set() 
        self._robots = {}

        # Starting positions of robots are initial nodes of the map
        for robot_id, robot_information in params.robots.items():
            robot_pose = robot_information['pose']
            robot = Robot(robot_id, robot_pose, map_orientation)
            assert self.is_valid_position(robot.position)
            self._robots[robot_id] = robot
            self.update(robot_id, 7, 0)   


    #---------------
    @property
    def frontiers(self):
        temp = deepcopy(self._frontiers)
        for frontier in temp:
            self.update_frontier(frontier)
        return list(self._frontiers)


    #---------------
    @property
    def frontiers_gui(self):
        frontiers = []
        for frontier in iter(self._frontiers):
            node = self.graph.get_node(frontier)
            (x, y) = node.position
            orientations = node.unexplored_orientations
            for orientation in orientations:
                if orientation == 0: frontiers.append((x,y+5))
                elif orientation == 1: frontiers.append((x+5,y))
                elif orientation == 2: frontiers.append((x,y-5))
                elif orientation == 3: frontiers.append((x-5,y))
        return frontiers


    #---------------
    @property
    def summary(self):
        edges = self.graph.edges
        frontiers = self.frontiers_gui
        robots = [(robot.position, robot.orientation) for robot in self._robots.values()]
        return (edges, frontiers, robots)


    #---------------
    def is_valid_position(self, xy):
        (x, y) = center2xy(xy, self.dimensions)
        (x_max, y_max) = self.dimensions
        if x < 0 or x > x_max-1:
            return False
        if y < 0 or y > y_max-1:
            return False
        return True


    #---------------
    def is_valid_intersection(self, type_intersection):
        return type_intersection >= 0 and type_intersection <= 9


    #---------------
    def get_robot(self, id_robot):
        assert self.is_robot(id_robot)
        return self._robots[id_robot]


    #---------------
    def get_robot_position(self, id_robot):
        assert self.is_robot(id_robot)
        return self._robots[id_robot].position

    #---------------
    def get_robot_orientation(self, id_robot):
        assert self.is_robot(id_robot)
        return self._robots[id_robot].orientation


    #---------------
    def is_robot(self, id_robot):
        return self._robots.get(id_robot) != None


    #---------------
    def is_node(self, xy):
        return self.graph.is_node(xy)


    #---------------
    def is_frontier(self, xy):
        return xy in self._frontiers


    #---------------
    def new_frontier(self, frontier):
        assert(self.is_node(frontier))
        if frontier not in self._frontiers:  
            self._frontiers.add(frontier)
        self.update_frontier(frontier)


    #---------------
    def update_frontier(self, frontier):
        node = self.graph.get_node(frontier)
        if (len(node.unexplored_orientations) == 0):
            self._frontiers.remove(frontier)


    #---------------
    def update(self, id_robot, type_intersection, distance):
        assert self.is_valid_intersection(type_intersection) and distance >= 0
        robot = self.get_robot(id_robot)
        previous_position = robot.position
        robot.travel(distance)
        assert self.is_valid_position(robot.position)

        going_to = robot.orientation
        coming_from = (robot.orientation + 2) % 4

        self.graph.new_node(robot.position, robot.orientation,
                type_intersection)
        self.graph.new_edge(previous_position, going_to, robot.position,
                coming_from, distance)
        self.new_frontier(robot.position)


    #---------------
    def turn_robot(self, id_robot, direction):
        return self.get_robot(id_robot).turn(direction)


    #---------------
    def is_robot_at_frontier(self, id_robot):
        return self.is_frontier(self.get_robot(id_robot).position)


    #---------------
    def unexplored_directions(self, id_robot):
        robot = self.get_robot(id_robot)
        position = robot.position
        orientation_robot = robot.orientation
        node = self.graph.get_node(position)
        orientation_edges = node.unexplored_orientations
        return [Robot.or2dir(orientation_robot, o) for o in orientation_edges]
    

    #---------------
    def shortest_path(self, start_, end_, heuristic):  
        assert self.is_node(start_) and self.is_node(end_)
        self.graph.reset()
        start_ = self.graph.get_node(start_)
        end_ = self.graph.get_node(end_)
        pq = PriorityQueue()

        # A* shortest path
        start_.cost = 0
        start_.visited = True
        pq.put(start_, 0.0)
        while not pq.is_empty():
            current = pq.get()
            if current.position == end_.position:
                break
            for (neighbor, weight) in current.neighbors:
                g = current.cost + weight
                if not neighbor.visited or g <= neighbor.cost:  
                    neighbor.cost = g
                    neighbor.parent = current
                    f = g + heuristic(neighbor.position, end_.position)
                    pq.put(neighbor, f)
                neighbor.visited = True

        # Unrol shortest path
        path = []
        node = end_
        while (node != None):
            path.append(node.position)
            if node == start_: break
            node = node.parent
        path.reverse()
        # print("Path: " + str(path))
        assert node == start_
        return path


    
#---------------
class Robot:
    """
    Blabla. Origins are same as associated map.
    """

    def __init__(self, id_robot, robot_pose, map_orientation):
        (x, y, orientation) = robot_pose
        self._id = id_robot
        self._x = x
        self._y = y
        assert orientation >= 0 and orientation <= 3
        self._orientation = orientation
        self.map_orientation = map_orientation 


    #---------------
    @staticmethod
    def or2dir(current_orientation, target_orientation):
        """ 
        Determines which direction to take for turning to specified orientation. 
        """   

        for direc in ["left", "right", "uturn", "straight"]:
            if Robot.dir2or(direc, current_orientation) == target_orientation:
                return direc
        assert False


    #---------------
    @staticmethod
    def dir2or(direction, orientation):
        if direction == "left":  # ... <- N <- E <- S <- W <- ...
            return (orientation + 3) % 4
        elif direction == "right":  # ... -> N -> E -> S -> W -> ...
            return (orientation + 5) % 4
        elif direction == "uturn":  # N <-> S, E <-> W
            return (orientation + 6) % 4
        return orientation


    #---------------
    @property
    def id(self):
        return _id


    #---------------
    @property
    def position(self):
        return (self._x, self._y)


    #---------------
    @property
    def orientation(self):
        return self._orientation


    #---------------
    def travel(self, distance): 
        if self._orientation == self.map_orientation['N']:
            self._y += distance
        elif self._orientation == self.map_orientation['S']:
            self._y -= distance
        elif self._orientation == self.map_orientation['E']:
            self._x += distance
        elif self._orientation == self.map_orientation['W']:
            self._x -= distance
        return self


    #---------------
    def turn(self, direction):
        self._orientation = Robot.dir2or(direction, self._orientation)
        return self
















