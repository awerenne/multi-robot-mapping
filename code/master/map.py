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

from astar import Astar
from utils import Container


#---------------
class Map:
    """ 
    Describe class. coordinates are in the form xy center otherwise 
    mentionned.
    """

    def __init__(self, init_robots):
        # Read constants
        stream = open('../config/config.yaml', 'r')
        self.constants = Container(load(stream, Loader=Loader)).map

        # Nodes of the map as {id_node: (x,y)}
        # and helper to help go back from coordinates to node id 
        self.nodes2coords = {}
        self.coords2nodes = {}
        self.last_used_id_nodes = -1

        # Set of edges as {id_edge: (id_node_a,id_node_b)}
        self.edges = {}
        self.last_used_id_edges = -1

        # Bi-directional transformation between orientation string and index 
        # N <-> 0, E <-> 1, S <-> 2, W <-> 3
        self.str2or = {'N': 0, 'E': 1, 'S': 2, 'W': 3}
        self.or2str = {0: 'N', 1: 'E', 2: 'S', 3: 'W'}

        # Set of frontiers as {id_node: (boolean vector)}
        # vector[idx] = True if this orientation of frontier is explored
        self.frontiers = {}

        # Structure in the following form 
        # {id_robot: (x,y,orientation)}
        self.robots = init_robots  

        # Initialize structures
        for robot_id, robot_pose in self.robots.items():
            self.add_node(robot_pose)


    #---------------
    def add_node(self, x, y, type_):
        """
        If node already existing than we just update the frontier
        """

        (x, y, orientation) = robot_pose
        new_coords = self.make_key(x,y)
        new_node = self.coords2nodes.get(new_coords)
        if new_node != None:  # Node already existing
            self.update_frontier(new_node, orientation)
            return

        new_node = last_used_id_nodes + 1
        last_used_id_nodes += 1
        self.nodes2coords[new_node] = (x, y)
        self.coords2nodes[new_coords] = new_node
        self.add_frontier(new_node, orientation, type_)


    #---------------
    def add_frontier(self, node, orientation, type_):
        """
        Save which direction is already explored at frontier
        (note that a wall is also seen as explored).
        """

        # Create vector and init the coming from direction to True 
        temp = self.transform_orientation(orientation, "uturn")
        v[temp] = True

        # Get which direction are walls
        (wall_left, wall_front, wall_right) = (False, False, False) 
        if type_ == 'B':
            # wall @right
            temp = self.transform_orientation(orientation, "right")
            v[temp] = True
        elif type_ == 'C':
            # wall @left
            temp = self.transform_orientation(orientation, "right")
            v[temp] = True
        elif type_ == 'D':
            # wall @front
            temp = orientation
            v[temp] = True
        elif type_ == 'E':
            # wall @front
            temp = orientation
            v[temp] = True
            # wall @right
            temp = self.transform_orientation(orientation, "right")
            v[temp] = True
        elif type_ == 'F':
            # wall @front
            temp = orientation
            v[temp] = True
            # wall @left
            temp = self.transform_orientation(orientation, "right")
            v[temp] = True
        elif type_ == 'G':
            # dead-end (everything explored - not a frontier)
            return 

        # Set directions where wall to True
        self.frontiers[node] = v


    #---------------
    def update_frontier(self, node, orientation):
        """
        Save new explored directions and check if node is still 
        a frontier.
        """

        # Check if frontier already existing
        v = self.frontiers(node)
        temp = self.transform_orientation(orientation, "uturn")
        v[temp] = True
        if all(v):  # All direction of frontier are explored
            self.frontiers.pop(node)
            return

        self.frontiers[node] = v


    #---------------
    def make_key(self, x, y):
        return str(x) + "," + str(y) 


    #---------------
    def update_position(self, robot, distance):
        (x, y, orientation) = self.robots[robot]
        string_orientation = self.str2or[orientation]
        (new_x, new_y) = (x, y) 
        if string_orientation == 'N':
            new_y += distance
        elif string_orientation == 'S':
            new_y -= distance
        elif string_orientation == 'E':
            new_x += distance
        elif string_orientation == 'W':
            new_x -= distance
        self.robots[robot] = (new_x, new_y, orientation)


    # ------------
    def update_orientation(self, robot, direction):
        (x, y, orientation) = self.robots[robot]
        new_orientation = self.transform_orientation(orientation, direction)
        self.robots[robot] = (x, y, new_orientation)


    # ------------
    def transform_orientation(self, orientation, direction):
        if direction == "left":
            idx = ((idx - 1) + 4) % 4
        elif direction == "right":
            idx = ((idx + 1) + 4) % 4
        elif direction == "uturn":
            idx = ((idx + 2) + 4) % 4
        return new_orientation


    # ------------
    def get_pose(self, robot):
        return self.robots[robot]


    # ------------
    def get_frontiers(self):
        return list(map(lambda x : self.nodes2coords[x], frontiers.keys()))


    # ------------
    def is_frontier(self, node):
        return self.frontiers[node] != None


    # ------------
    def get_unexplored_directions(self, robot):
        (x, y, orientation) = self.robots[robot]
        node = self.coords2nodes(self.make_key(x,y))
        if node == None or self.is_frontier(node) == True:
            return None

        v = self.frontiers[node]
        unexplored_directions = ["front", "left", "right"]
        for i, explored in enumerate(v):
            if explored:
                continue
            elif i == orientation:
                unexplored_directions.remove("front")
            elif i+1 == orientation
                unexplored_directions.remove("left")
            elif i-1 == orientation
                unexplored_directions.remove("right")
        
        return unexplored_directions








