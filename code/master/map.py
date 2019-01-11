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
    TODO: better comments
    Describe class. coordinates are in the form xy center otherwise 
    mentionned.
    Descirbe all internal variables
    # Nodes of the map as {id_node: (x,y)}
    # and helper to help go back from coordinates to node id 
    # Set of edges as {id_edge: (id_node_a,id_node_b)}
    # Set of frontiers as {id_node: (boolean vector)}
    # vector[idx] = True if this orientation of frontier is explored
    # Structure in the following form 
    # {id_robot: (x,y,orientation)}
    """

    def __init__(self, robots):

        # Read parameters in config file
        stream = open('../config/config.yaml', 'r')
        self.parameters = Container(load(stream, Loader=Loader))
        self.dimensions = self.parameters.map.dimensions
        self.orientation_str2idx = self.parameters.map.orientation_str2idx
        self.orientation_idx2str = self.parameters.map.orientation_idx2str

        assert check_valid_positions(robots)
        self._robots = robots 
        
        self.nodes2coords = {}
        self.coords2nodes = {}
        self.last_used_id_nodes = -1
        self.edges = {}
        self.last_used_id_edges = -1
        self._frontiers = {}  

        # Starting positions of robots are first nodes of the map
        for robot_id, robot_pose in self._robots.items():
            self.add_node(robot_pose)


    # ------------
    @property
    def robots(self):
        return self._robots

        
    # ------------
    @property
    def frontiers(self):
        return list(map(lambda x : self.nodes2coords[x], self._frontiers.keys()))


    #---------------
    def check_valid_positions(self, robots):
        """
        Checks if robot positions are within the dimensions of the map and
        if the orientations are defined.
        """

        for _, robot_pose in self._robots.items():
            (x, y, orientation) = robot_pose
            if orientation < 0 or orientation > 3:
                return False
            (x, y) = to_axis_xy_non_centered(x, y)
            (x_max, y_max) = self.dimensions
            if x < 0 or x > x_max-1:
                return False
            if y < 0 or y > y_max-1:
                return False
        return True


    #---------------
    def is_existing_node(self, x, y):
        return self.coords2nodes.get((x,y)) == None  


    # ------------
    def is_frontier(self, node):
        return self._frontiers[node] != None


    #---------------
    def update_position(self, robot, distance):
        (x, y, orientation_idx) = self._robots[robot]
        orientation_str = self.orientation_str2idx[orientation]
        (new_x, new_y) = (x, y) 

        if orientation_str == 'N':
            new_y += distance
        elif orientation_str == 'S':
            new_y -= distance
        elif orientation_str == 'E':
            new_x += distance
        elif orientation_str == 'W':
            new_x -= distance

        self._robots[robot] = (new_x, new_y, orientation_idx)


    # ------------
    def update_orientation(self, robot, direction):
        (x, y, orientation_idx) = self._robots[robot]
        new_orientation_idx = self.turn_orientation(orientation_idx, direction)
        self._robots[robot] = (x, y, new_orientation_idx)


    #---------------
    # TODO: add operator
    def add_node(self, x, y, type_):
        """
        Add node to the set if non existing and add it as a frontier 
        node. If the node already exist - a robot travelled already passed this
        node - we update the frontier. 
        """

        (x, y, orientation) = robot_pose
        if self.is_existing_node(x, y):  
            self.update_frontier(new_node, orientation)
            return self

        new_node = last_used_id_nodes + 1
        last_used_id_nodes += 1
        self.nodes2coords[new_node] = (x,y)
        self.coords2nodes[(x,y)] = new_node
        self.add_frontier(new_node, orientation, type_)
        return self


    #---------------
    # TODO: add operator
    def add_frontier(self, node, orientation, type_):
        """
        Save which direction is already explored at frontier
        (note that a wall is also seen as explored).
        """

        # Create vector and init the coming from direction to True 
        temp = self.turn_orientation(orientation, "uturn")
        v[temp] = True

        # Get which direction are walls
        (wall_left, wall_front, wall_right) = (False, False, False) 
        if type_ == 'B':
            # wall @right
            temp = self.turn_orientation(orientation, "right")
            v[temp] = True
        elif type_ == 'C':
            # wall @left
            temp = self.turn_orientation(orientation, "right")
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
            temp = self.turn_orientation(orientation, "right")
            v[temp] = True
        elif type_ == 'F':
            # wall @front
            temp = orientation
            v[temp] = True
            # wall @left
            temp = self.turn_orientation(orientation, "right")
            v[temp] = True
        elif type_ == 'G':
            # dead-end (everything explored - not a frontier)
            return 

        # Set directions where wall to True
        self._frontiers[node] = v


    #---------------
    def update_frontier(self, node, orientation):
        """
        Update vector of directions with the direction we are coming from e.g.
            
            if the robot is oriented to the north while it is
            arriving at a node, we know the south direction of the
            frontier node is explored

        Once this is done we check if node is still considered as a frontier
        (if some directions still need to be explored).
        """

        v = self._frontiers[node]
        coming_from = self.turn_orientation(orientation, "uturn")
        v[coming_from] = True
        self._frontiers[node] = v

        # Check if all direction of frontier are explored
        if all(v):  
            self._frontiers.pop(node)

        return self


    # ------------
    def turn_orientation(self, orientation_idx, direction):
        if direction == "left":
            new_orientation_idx = ((orientation_idx - 1) + 4) % 4
        elif direction == "right":
            new_orientation_idx = ((orientation_idx + 1) + 4) % 4
        elif direction == "uturn":
            new_orientation_idx = ((orientation_idx + 2) + 4) % 4
        return new_orientation_idx

    
    # ------------
    def get_neighborhood(self, robot):
        """
        Neigborhood of a robot is a dictionary of free directions. 

            e.g. a robot for which the only paths that are free are left and
            right will return: 
            {"left": True, "front": False, "right": True, "back": False}
        
        If not a valid a request we return None.
        """
        
        (x, y, orientation_idx) = self._robots[robot]
        if self.is_existing_node(x,y):
            return None
        node = self.coords2nodes[x,y]

        v = self._frontiers[node]
        neighborhood = {} 

        # What direction do we need to take to go from start orientation to
        # an end direction
        def map_turn_to_direction(start_orientation, end_orientation):
            if start_orientation == end_orientation:
                return "front"
            elif start_orientation == end_orientation+1:
                return "right"
            elif start_orientation == end_orientation-1:
                return "left"
            return "back"

        for end_orientation_idx, is_explored in enumerate(v):
            direction = map_turn_to_direction(orientation_idx,
                    end_orientation_idx)
            neighborhood[direction] = is_explored
            
        return neighborhood








