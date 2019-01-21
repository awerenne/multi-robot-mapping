"""
    Defining the class containing all the information gathered by the 
    robots regarding the map.
"""


import numpy as np
from yaml import load
try:
    from yaml import CLoader as Loader
except ImportError:
    from yaml import Loader

from graph import Graph, Node
from utils import Container, PriorityQueue, center2xy


# TODO transform unexplored orientations to directions


#---------------
class Map:
    """ 
    Describe class & parameters. coordinates are in the form xy center otherwise 
    mentionned.
    """

    def __init__(self, parameters_shared, parameters_real_world):

        self.dimensions = parameters_real_world.dimensions_map
        self.graph = Graph()
        self._frontiers = set() 
        self._robots = {}

        # Starting positions of robots are initial nodes of the map
        for robot_id, robot_pose in parameters_real_world.robots.items():
            robot = Robot(robot_id, robot_pose)
            assert self.is_valid_position(robot.position)
            self._robots[robot_id] = robot
            self.graph.new_node(robot.position, robot.orientation, 7)
            self.new_frontier(robot.position)


    #---------------
    @property
    def frontiers(self):
        return list(self._frontiers)


    #---------------
    @property
    def summary(self):
        edges = self.graph.edges
        frontiers = self.frontiers
        robots = [(robot.position, robot.orientation) for robot in self._robots]
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
        return type_intersection >= 0 and type_intersection <= 8


    #---------------
    def get_robot_position(self, id_robot):
        assert is_robot(id_robot)
        return self._robots[id_robot].position


    #---------------
    def is_robot(self, id_robot):
        return self._robots.get(id_robot) != None


    #---------------
    def is_node(self, xy):
        return self.graph.get_node(xy) != None  


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
        if (len(node.unexplored_directions()) == 0):
            self._frontiers.remove(frontier)


    #---------------
    def update(self, id_robot, type_intersection, distance):
        assert self.is_robot(id_robot)
        assert self.is_valid_intersection(type_intersection)
        assert distance >= 0

        robot = self._robots[id_robot].travel(distance)
        assert self.is_valid_position(robot.position)

        self.graph.new_node(robot.position, robot.orientation, type_intersection)
        self.new_frontier(robot.position)


    #---------------
    def turn_robot(self, id_robot, direction):
        self._robots[id_robot].turn(direction)


    #---------------
    def is_robot_at_frontier(self, id_robot):
        assert self.is_robot(id_robot)
        return self.is_frontier(self._robots[id_robot].position)


    #---------------
    def unexplored_directions(self, id_robot):
        assert self.is_robot(id_robot)
        position = self._robots[id_robot].position
        return self.graph.get_node(position).unexplored_directions()


    #---------------
    def shortest_path(self, start_, end_, heuristic):  
        assert self.is_node(start_) and self.is_node(end_)
        start_ = self.graph.get_node(start_)
        end_ = self.graph.get_node(end_)
        self.graph.reset()
        priority_frontiers = PriorityQueue()

        # A* shortest path
        priority_frontiers.put(start_, 0.0)
        while not priority_frontiers.is_empty():
            current = priority_frontiers.get()
            if current == target:
                break

            for (neighbor, weight) in current.neighbors:
                g = current.cost + weight
                if not neighbor.visited or g <= neighbor.cost:  
                    neighbor.cost = g
                    neighbor.parent = current
                    f = g + heuristic(neighbor.position, end_.position)
                    priority_frontiers.put(neighbor, f)
                neighbor.visited = True

        # Unrol shortest path
        path = []
        while (node != None):
            path.append(node.coords)
            node = node.parent
        return reverse(path)


    
#---------------
class Robot:
    """
    Blabla. Origins are same as associated map.
    """

    def __init__(self, id_robot, robot_pose):
        (x, y, orientation) = robot_pose
        self._id = id_robot
        self._x = x
        self._y = y
        assert orientation > 0 and orientation <= 3
        self._orientation = orientation
        self.map_orientation = {0: 'N', 1: 'E', 2: 'S', 3: 'W'} 


    #---------------
    @property
    def id(self):
        return _id


    #---------------
    @property
    def position(self):
        return (self._x, self.y)


    #---------------
    @property
    def orientation(self):
        return self._orientation


    #---------------
    def travel(self, distance): 
        if self._orientation == map_orientation['N']:
            self._y += distance
        elif self._orientation == map_orientation['S']:
            self._y -= distance
        elif self._orientation == map_orientation['E']:
            self._x += distance
        elif self._orientation == map_orientation['W']:
            self._x -= distance
        return self


    #---------------
    def turn(self, direction):
        if direction == "left":  # ... <- N <- E <- S <- W <- ...
            self._orientation = (self._orientation + 3) % 4
        elif direction == "right":  # ... -> N -> E -> S -> W -> ...
            self._orientation = (self._orientation + 5) % 4
        elif direction == "uturn":  # N <-> S, E <-> W
            self._orientation = (self._orientation + 6) % 4
        return self














