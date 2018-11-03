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

    def __init__(self):
        # Referential of map (! origin is at center of grid !)
        self.axis_orientation = {
            "x": 'E',
            "-x": 'W',
            "y": 'N',
            "-y": 'S'
        }

        # Initial robot pose
        self.robot_pose = {
            "x": 0,
            "y": 0,
            "orientation": 'N'
        }

        # Map data structures
        self.first_not_used_id = 0
        self.current_vertice_id = self.first_not_used_id
        self.first_not_used_id += 1
        self.vertices_coords_to_id = {"0,0": self.current_vertice_id}
        self.vertices_id_to_coords = {self.current_vertice_id: (0,0)}
        self.edges = {} 
        self.grid = np.zeros((24,24), WALL_TYPE) 


    def get_robot_pose(self):
        return (self.axis_orientation, self.robot_pose)


    def get_grid(self):
        return (self.grid, (24,24))


    def update_map(self, robot_id, type_intersection, traveled_distance):

        # Compute new position of robot 
        new_x = self.robot_pose["x"]
        new_y = self.robot_pose["y"]
        if self.robot_pose["orientation"] == self.axis_orientation["x"]:
            new_x += traveled_distance
        elif self.robot_pose["orientation"] == self.axis_orientation["-x"]:
            new_x -= traveled_distance
        elif self.robot_pose["orientation"] == self.axis_orientation["y"]:
            new_y += traveled_distance
        else:
            new_y -= traveled_distance

        # Update 
        new_vertice_id = self.update_vertices(new_x, new_y)
        self.update_edges(new_vertice_id, traveled_distance)
        self.update_grid(new_vertice_id, type_intersection)
        self.robot_pose["x"] = new_x
        self.robot_pose["y"] = new_y
        self.current_vertice_id = new_vertice_id
        return 0


    def get_next_move(self, robot_id):
        surroundings = self.check_surroundings(self.robot_pose["x"],
            self.robot_pose["y"])
        print(surroundings)
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
            self.update_orientation(next_move)
            return next_move

        if is_frontier:
            next_move = self.frontier_rule(surroundings)
            self.update_orientation(next_move)
            return next_move

        frontiers = self.get_frontiers()
        min_distance = np.inf
        min_path = []
        if frontiers == None:
            return "stop"
        for frontier in frontiers:
            (distance, path) = self.get_shortest_path(self.current_vertice_id,
                frontier)
            if distance < min_distance:
                min_path = path
        if len(min_path) < 2:
            return "stop"

        next_move = self.edge_to_move((min_path[0], min_path[1]))
        self.update_orientation(next_move)
        return next_move


    def update_vertices(self, new_x, new_y):
        key = str(new_x) + "," + str(new_y)
        vertice_id = self.vertices_coords_to_id.get(key)
        if vertice_id != None:
            return vertice_id
        new_vertice_id = self.first_not_used_id
        self.vertices_coords_to_id[key] = new_vertice_id 
        self.vertices_id_to_coords[new_vertice_id] = (new_x, new_y)
        self.first_not_used_id += 1
        return new_vertice_id


    def update_edges(self, new_vertice_id, traveled_distance):
        key = str(self.current_vertice_id) + "," + str(new_vertice_id)
        inversed_key = str(new_vertice_id) + "," + str(self.current_vertice_id)
        distance = self.edges.get(key)
        if distance != None:
            return
        self.edges[key] = traveled_distance
        self.edges[inversed_key] = traveled_distance


    def update_grid(self, new_vertice_id, type_intersection):
        (start_x, start_y) = self.vertices_id_to_coords[self.current_vertice_id]
        (end_x, end_y) = self.vertices_id_to_coords[new_vertice_id]
        (start_i, start_j) = self.get_grid_coords(start_x, start_y)
        (end_i, end_j) = self.get_grid_coords(end_x, end_y)
        a_i = min((start_i, end_i))
        b_i = max((start_i, end_i))
        a_j = min((start_j, end_j))
        b_j = max((start_j, end_j))
        self.grid[a_i:b_i+1,a_j:b_j+1] = EMPTY
        neighbors = temp_map[type_intersection][self.robot_pose["orientation"]]
        for neighbor in neighbors:
            (j, i) = neighbor
            i += end_i
            j += end_j
            if (self.grid[i,j] == EMPTY):
                continue
            self.grid[i,j] = UNEXPLORED
        # print(self.grid[20:30,20:30])
        

    def get_grid_coords(self, x, y):
        (center_i, center_j) = (12,12)
        i = -y + center_i
        j = x + center_j
        return (int(i), int(j))


    def check_surroundings(self, x, y):
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

        if self.robot_pose["orientation"] == 'N':
            surroundings["left"] = surroundings.pop('W')
            surroundings["top"] = surroundings.pop('N')
            surroundings["right"] = surroundings.pop('E')
            surroundings["bottom"] = surroundings.pop('S')
        elif self.robot_pose["orientation"] == 'E':
            surroundings["left"] = surroundings.pop('N')
            surroundings["top"] = surroundings.pop('E')
            surroundings["right"] = surroundings.pop('S')
            surroundings["bottom"] = surroundings.pop('W')
        elif self.robot_pose["orientation"] == 'S':
            surroundings["left"] = surroundings.pop('E')
            surroundings["top"] = surroundings.pop('S')
            surroundings["right"] = surroundings.pop('W')
            surroundings["bottom"] = surroundings.pop('N')
        elif self.robot_pose["orientation"] == 'W':
            surroundings["left"] = surroundings.pop('S')
            surroundings["top"] = surroundings.pop('W')
            surroundings["right"] = surroundings.pop('N')
            surroundings["bottom"] = surroundings.pop('E')

        return surroundings


    def update_orientation(self, move):
        current_orientation = self.robot_pose["orientation"]
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

        self.robot_pose["orientation"] = new_orientation


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
            surroundings = self.check_surroundings(x, y)
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
        print(V)
        print(E)
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







