

class Sensors:

    def __init__(self, maze, id):
        self.maze = maze
        self.id = id


    def get_sensors_vector(self):
        return self.maze.get_sensors_vector(self.id)


    def get_situation(self):
        (current_path, left_path, right_path, front_path) = self.get_sensors_vector()

        if (left_path == 0 and current_path == 1 and
            right_path == 0 and front_path == 1):
            return "default"
        else:
            return "intersection"


    def get_type_intersection(self):
        (current_path, left_path, right_path, front_path) = self.get_sensors_vector()

        if (left_path and front_path and right_path):
            return('A')
        elif (left_path and front_path and not right_path):
            return('B')
        elif (not left_path and front_path and right_path):
            return('C')
        elif (left_path and not front_path and right_path):
            return('D')
        elif (left_path and not front_path and not right_path):
            return('E')
        elif (not left_path and not front_path and right_path):
            return('F')
        elif (not left_path and not front_path and not right_path):
            return('G')
        return('')


class Actuators:

    def __init__(self, maze, id):
        self.maze = maze
        self.id = id


    def go_left(self):
        self.maze.turn_left(self.id)
        self.go_straight()


    def go_straight(self):
        self.maze.go_straight(self.id)


    def go_right(self):
        self.maze.turn_right(self.id)
        self.go_straight()


    def go_back(self):
        self.maze.turn_back(self.id)
        self.go_straight()


    def stop(self):
        return None


class Slave:

    def __init__(self, maze, id):
        self.stop_signal = False
        self.id = id
        self.sensors = Sensors(maze, id)
        self.actuators = Actuators(maze, id)
        self.distance = 0


    def connect(self, master):
        self.master = master
        self.master.connect(self.id)


    def send_msg(self, type_, content):
        msg = {
            "src": self.id,
            "type": type_,
            "content": content
        }
        return self.master.deliver_msg(msg)


    def run(self):
        if not self.stop_signal:
            situation = self.sensors.get_situation()

            if situation == "default":
                self.actuators.go_straight()
                self.distance += 1

            elif situation == "intersection":
                self.actuators.stop()

                type_intersection = self.sensors.get_type_intersection()
                content = {
                    "type-intersection": type_intersection,
                    "traveled-distance": self.distance}
                response = self.send_msg("map-update", content)

                new_direction = response["content"]["new-direction"]
                if new_direction == "left":
                    self.actuators.go_left()
                elif new_direction == "straight":
                    self.actuators.go_straight()
                elif new_direction == "right":
                    self.actuators.go_right()
                elif new_direction == "uturn":
                    self.actuators.go_back()
                else:
                    self.actuators.stop()
                    self.stop_signal = True

                self.distance = 1

            elif situation == "unknown":
                self.actuators.stop()
                content = {"error-code": "unrecognized-situtation"}
                response = self.send_msg(self.master_id, "error", content)
                self.stop_signal = True










