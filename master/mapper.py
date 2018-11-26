from dijkstra import Dijkstra
import numpy as np
# Make an utils file
from constants import *

temp_map = {
    'A': {
        'N': [(-1, 0), (0, -1), (1, 0)],
        'E': [(0, -1), (1, 0), (0, 1)],
        'S': [(-1, 0), (0, -1), (1, 0)],
        'W': [(-1, 0), (0, -1), (0, 1)]
    },
    'B': {
        'N': [(-1, 0), (0, -1)],
        'E': [(0, -1), (1, 0)],
        'S': [(1, 0), (0, 1)],
        'W': [(-1, 0), (0, 1)]
    },
    'C': {
        'N': [(0, -1), (1, 0)],
        'E': [(1, 0), (0, 1)],
        'S': [(-1, 0), (0, 1)],
        'W': [(-1, 0), (0, -1)]
    },
    'D': {
        'N': [(-1, 0), (1, 0)],
        'E': [(0, -1), (0, 1)],
        'S': [(-1, 0), (1, 0)],
        'W': [(0, -1), (1, 0)]
    },
    'E': {
        'N': [(-1, 0)],
        'E': [(0, -1)],
        'S': [(1, 0)],
        'W': [(0, 1)]
    },
    'F': {
        'N': [(1, 0)],
        'E': [(0, 1)],
        'S': [(-1, 0)],
        'W': [(0, -1)]
    },
    'G': {
        'N': [],
        'E': [],
        'S': [],
        'W': []
    },
}


    
class Mapper:

    def __init__(self, mapper_id, slave_id, parameters=None):
        self._id = mapper_id
        if parameters is None:
            self.normal_init(slave_id)
        else:
            self.merge_init(parameters)

    def __eq__(self, other):
        return self._id == other._id
        

    def normal_init(self, slave_id):
        # Referential of map (! origin is at center of grid !)
        self.axis_orientation = {
            "x": 'E',
            "-x": 'W',
            "y": 'N',
            "-y": 'S'
        }

        # Initial slave pose
        self.slaves = {}
        self.slaves[slave_id] = {
            "x": 0,
            "coming_from_x": 0,
            "y": 0,
            "coming_from_y": 0,
            "orientation": 'N', 
            "slave_meeting": False
        }

        # Map data structures
        self.first_not_used_id = 0
        self.vertices_coords_to_id = {"0,0": self.first_not_used_id}
        self.vertices_id_to_coords = {self.first_not_used_id: (0,0)}
        self.first_not_used_id += 1
        self.edges = {} 
        self.dimension = (47,47)
        self.grid = np.zeros(self.dimension, WALL_TYPE) 

    def get_slave(self, slave_id):
        return self.slaves[slave_id]

    def merge_init(self, parameters):
        # Referential of map (! origin is at center of grid !)
        self.axis_orientation = {
            "x": 'E',
            "-x": 'W',
            "y": 'N',
            "-y": 'S'
        }

        # Initial slave pose
        self.slaves = parameters["slaves"]

        # Map data structures
        self.first_not_used_id = parameters["first_not_used_id"]
        self.vertices_coords_to_id = parameters["vertices_coords_to_id"]
        self.vertices_id_to_coords = parameters["vertices_id_to_coords"]
        self.edges = parameters["edges"]
        self.dimension = (47,47)
        self.grid = parameters["grid"]


    def get_parameters(self):
        parameters = {}
        parameters["slaves"] = self.slaves

        # Map data structures
        parameters["first_not_used_id"] = self.first_not_used_id 
        parameters["vertices_coords_to_id"] = self.vertices_coords_to_id 
        parameters["vertices_id_to_coords"] = self.vertices_id_to_coords 
        parameters["edges"] = self.edges 
        parameters["grid"] = self.grid 

        parameters["dimension"] = self.dimension
        return parameters


    def get_slave_pose(self, slave_id):
        return (self.axis_orientation, self.slaves[slave_id])


    def get_grid(self):
        return (self.grid, self.dimension)


    def update_map(self, slave_id, type_intersection, traveled_distance):
        # Compute new position of slave
        slave = self.slaves[slave_id]
        new_x = slave["x"]
        new_y = slave["y"]
        if slave["orientation"] == self.axis_orientation["x"]:
            new_x += traveled_distance
        elif slave["orientation"] == self.axis_orientation["-x"]:
            new_x -= traveled_distance
        elif slave["orientation"] == self.axis_orientation["y"]:
            new_y += traveled_distance
        else:
            new_y -= traveled_distance

        # Update 
        slave["coming_from_x"] = slave["x"]
        slave["coming_from_y"] = slave["y"]
        slave["x"] = new_x
        slave["y"] = new_y
        if not slave["slave_meeting"]:
            self.update_vertices(slave)
            is_new_edge = self.update_edges(slave, traveled_distance)
            if is_new_edge:
                self.update_grid(slave, type_intersection)
            slave["slave_meeting"] = False


    def update_vertices(self, slave):
        key = str(slave["x"]) + "," + str(slave["y"])
        vertice_id = self.vertices_coords_to_id.get(key)
        if vertice_id != None:
            return vertice_id
        new_vertice_id = self.first_not_used_id
        self.vertices_coords_to_id[key] = new_vertice_id 
        self.vertices_id_to_coords[new_vertice_id] = (slave["x"], slave["y"])
        self.first_not_used_id += 1


    def update_edges(self, slave, traveled_distance):
        key_coming_from = str(slave["coming_from_x"]) + "," + str(slave["coming_from_y"])
        key_current = str(slave["x"]) + "," + str(slave["y"])
        v_coming_from = self.vertices_coords_to_id[key_coming_from]
        v_current = self.vertices_coords_to_id[key_current]
        
        key = str(v_coming_from) + "," + str(v_current)
        inversed_key = str(v_current) + "," + str(v_coming_from)
        distance = self.edges.get(key)
        if distance != None:
            return False
        self.edges[key] = traveled_distance
        self.edges[inversed_key] = traveled_distance
        return True


    def update_grid(self, slave, type_intersection):
        (start_x, start_y) = (slave["coming_from_x"], slave["coming_from_y"])
        (end_x, end_y) = (slave["x"], slave["y"])
        (start_i, start_j) = self.get_grid_coords(start_x, start_y)
        (end_i, end_j) = self.get_grid_coords(end_x, end_y)
        a_i = min((start_i, end_i))
        b_i = max((start_i, end_i))
        a_j = min((start_j, end_j))
        b_j = max((start_j, end_j))
        self.grid[a_i:b_i+1,a_j:b_j+1] = EMPTY
        neighbors = temp_map[type_intersection][slave["orientation"]]
        for neighbor in neighbors:
            (j, i) = neighbor
            i += end_i
            j += end_j
            if (self.grid[i,j] == EMPTY):
                continue
            self.grid[i,j] = UNEXPLORED
        

    def get_grid_coords(self, x, y):
        (center_i, center_j) = (int(self.dimension[0]/2), int(self.dimension[1]/2))
        i = -y + center_i
        j = x + center_j
        return (int(i), int(j))


    def check_surroundings(self, x, y, orientation):
        (i, j) = self.get_grid_coords(x, y)
        surroundings = {
            'N': WALL, 
            'S': WALL, 
            'E': WALL, 
            'W': WALL}
        if j-1 >= 0:
            surroundings['W'] = self.grid[i,j-1]
        if j+1 <= 50:
            surroundings['E'] = self.grid[i,j+1]
        if i-1 >= 0:
            surroundings['N'] = self.grid[i-1,j]
        if i+1 <= 50:
            surroundings['S'] = self.grid[i+1,j]

        if orientation == 'N':
            surroundings["left"] = surroundings.pop('W')
            surroundings["top"] = surroundings.pop('N')
            surroundings["right"] = surroundings.pop('E')
            surroundings["bottom"] = surroundings.pop('S')
        elif orientation == 'E':
            surroundings["left"] = surroundings.pop('N')
            surroundings["top"] = surroundings.pop('E')
            surroundings["right"] = surroundings.pop('S')
            surroundings["bottom"] = surroundings.pop('W')
        elif orientation == 'S':
            surroundings["left"] = surroundings.pop('E')
            surroundings["top"] = surroundings.pop('S')
            surroundings["right"] = surroundings.pop('W')
            surroundings["bottom"] = surroundings.pop('N')
        elif orientation == 'W':
            surroundings["left"] = surroundings.pop('S')
            surroundings["top"] = surroundings.pop('W')
            surroundings["right"] = surroundings.pop('N')
            surroundings["bottom"] = surroundings.pop('E')

        return surroundings


    def update_orientation(self, slave_id, move):
        current_orientation  = self.slaves[slave_id]["orientation"]
        if current_orientation == 'N' and move == "straight":
            new_orientation = 'N'
        elif current_orientation == 'N' and move == "uturn":
            new_orientation = 'S'
        elif current_orientation == 'N' and move == "left":
            new_orientation = 'W'
        elif current_orientation == 'N' and move == "right":
            new_orientation = 'E'

        elif current_orientation == 'S' and move == "straight":
            new_orientation = 'S'
        elif current_orientation == 'S' and move == "uturn":
            new_orientation = 'N'
        elif current_orientation == 'S' and move == "left":
            new_orientation = 'E'
        elif current_orientation == 'S' and move == "right":
            new_orientation = 'W'

        elif current_orientation == 'W' and move == "straight":
            new_orientation = 'W'
        elif current_orientation == 'W' and move == "uturn":
            new_orientation = 'E'
        elif current_orientation == 'W' and move == "left":
            new_orientation = 'S'
        elif current_orientation == 'W' and move == "right":
            new_orientation = 'N'

        elif current_orientation == 'E' and move == "straight":
            new_orientation = 'E'
        elif current_orientation == 'E' and move == "uturn":
            new_orientation = 'W'
        elif current_orientation == 'E' and move == "left":
            new_orientation = 'N'
        elif current_orientation == 'E' and move == "right":
            new_orientation = 'S'

        self.slaves[slave_id]["orientation"] = new_orientation


    def frontier_rule(self, surroundings):
        if surroundings["left"] == UNEXPLORED:
            return "left"
        elif surroundings["top"] == UNEXPLORED:
            return "straight"
        elif surroundings["right"] == UNEXPLORED:
            return "right"
        elif surroundings["bottom"] == UNEXPLORED:
            return "uturn"
        return "stop"


    def get_frontiers(self):
        frontiers = []
        for v_id, v_coords in self.vertices_id_to_coords.items():
            (x, y) = v_coords
            surroundings = self.check_surroundings(x, y, 'N')
            for orientation, value in surroundings.items():
                if value == UNEXPLORED:
                    frontiers.append(v_id)
                    break
        return frontiers


    def get_shortest_path(self, start, end):
        V = []
        for vertice_id in self.vertices_id_to_coords:
            V.append(vertice_id)

        E = []
        for key, value in self.edges.items():
            (a, b) = key.split(",")
            E.append([int(a), int(b), value])
        solver = Dijkstra(V, E)
        return solver.dijkstra(start, end)   


    def edge_to_move(self, edge):
        (v_start, v_end) = edge
        coords_start = self.vertices_id_to_coords[v_start]
        coords_end = self.vertices_id_to_coords[v_end]
        if (coords_start[0] > coords_end[0]):
            return "right"
        elif (coords_start[0] <= coords_end[0]):
            return "left"
        elif (coords_start[1] > coords_end[1]):
            return "straight"
        elif (coords_start[1] <= coords_end[1]):
            return "uturn"
        return "stop"







