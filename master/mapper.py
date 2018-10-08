from dijkstra import Dijkstra


class Mapper:

    def __init__(self):
        # Referential of map
        self.origin_x = 0
        self.origin_y = 0
        self.origin_orientation_x = 'E'
        self.origin_orientation_y = 'N'

        # Initial robot position in map referential
        self.robot_x = self.origin_x
        self.robot_y = self.origin_y
        self.robot_orientation = 'N'

        # Map (= set of edges) initialization
        # an edge is saved in the following form: 
        # ((start_x, start_y), (end_x, end_y))
        self.edges = []


    def get_origin(self):
        return (self.origin_x, self.origin_y,
            self.origin_orientation_x, self.origin_orientation_y)


    def get_robot_position(self):
        return (self.robot_x, self.robot_y)


    def get_map(self):
        origin = self.get_origin()
        return (origin, self.edges)


    def get_path_to_origin(self):
        start = self.get_robot_position()
        end = (self.origin_x, self.origin_y)
        shortest_path = self.get_shortest_path(start, end)
        origin = self.get_origin()
        return (origin, shortest_path)


    def update_map(self, traveled_distance, taken_direction):

        # Compute new position of robot in map referential
        robot_new_x = self.robot_x
        robot_new_y = self.robot_y
        if self.robot_orientation == self.origin_orientation_x:
            robot_new_x += traveled_distance
        elif self.opposite_orientation(self.robot_orientation,
                self.origin_orientation_x):
            robot_new_x -= traveled_distance
        elif self.robot_orientation == self.origin_orientation_y:
            robot_new_y += traveled_distance
        elif self.opposite_orientation(self.robot_orientation,
                self.origin_orientation_y):
            robot_new_y -= traveled_distance

        # Add new edge to the map (corresponding to the traveled distance)
        self.edges.append(( (self.robot_x, self.robot_y),
                            (robot_new_x, robot_new_y)))

        # Compute new orientation of robot in map referential
        robot_new_orientation = self.new_orientation(self.robot_orientation,
            taken_direction)

        # Updates saved robot position and orientation
        self.robot_x = robot_new_x
        self.robot_y = robot_new_y
        self.robot_orientation = robot_new_orientation
        return True


    def opposite_orientation(self, orientation_a, orientation_b):
        if orientation_a == 'N' and orientation_b == 'S':
            return True
        elif orientation_a == 'S' and orientation_b == 'N':
            return True
        elif orientation_a == 'E' and orientation_b == 'W':
            return True
        elif orientation_a == 'W' and orientation_b == 'E':
            return True
        return False


    def new_orientation(self, current_orientation, direction_of_move):
        if current_orientation == 'N' and direction_of_move == "straight":
            return 'N'
        elif current_orientation == 'N' and direction_of_move == "uturn":
            return 'S'
        elif current_orientation == 'N' and direction_of_move == "left":
            return 'W'
        elif current_orientation == 'N' and direction_of_move == "right":
            return 'E'

        elif current_orientation == 'S' and direction_of_move == "straight":
            return 'S'
        elif current_orientation == 'S' and direction_of_move == "uturn":
            return 'N'
        elif current_orientation == 'S' and direction_of_move == "left":
            return 'E'
        elif current_orientation == 'S' and direction_of_move == "right":
            return 'W'

        elif current_orientation == 'W' and direction_of_move == "straight":
            return 'W'
        elif current_orientation == 'W' and direction_of_move == "uturn":
            return 'E'
        elif current_orientation == 'W' and direction_of_move == "left":
            return 'S'
        elif current_orientation == 'W' and direction_of_move == "right":
            return 'N'

        elif current_orientation == 'E' and direction_of_move == "straight":
            return 'E'
        elif current_orientation == 'E' and direction_of_move == "uturn":
            return 'W'
        elif current_orientation == 'E' and direction_of_move == "left":
            return 'N'
        elif current_orientation == 'E' and direction_of_move == "right":
            return 'S'


    def get_shortest_path(self, start, end):
        solver = Dijkstra(self.edges)
        return solver.shortest_path(start, end)





