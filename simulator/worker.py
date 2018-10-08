from messenger import Messenger


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
            return "normal"
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


class Worker:

    def __init__(self, maze, id):
        self.stop_signal = False
        self.id = id
        self.sensors = Sensors(maze, id)
        self.actuators = Actuators(maze, id)
        self.messenger = None
        self.distance = 0
        

    def get_id(self):
        return self.id


    def connect(self, messenger, master_id):
        self.master_id = master_id
        self.messenger = messenger


    def receive_msg(self, sender_id, type_message, content_message):
        if type_message == "instruction":
            self.stop_signal = True


    def send_msg(self, receiver_id, type_message, content_message):
        self.messenger.send(receiver_id, type_message,
            content_message)


    def run(self):
        if not self.stop_signal:
            situation = self.sensors.get_situation()

            if situation == "normal":
                self.actuators.go_straight()

            elif situation == "not-valid":
                self.actuators.stop()
                self.stop_signal = True

            elif situation == "intersection":
                self.actuators.stop()
                type_intersection = self.sensors.get_type_intersection()
                new_direction = self.direction(type_intersection)
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

                # Send message to master (map information)
                content_message = {"traveled_distance": self.distance,
                    "taken_direction": new_direction}
                self.send_msg(self.master_id, "map_info", content_message)
                self.distance = 0


    def direction(self, type_intersection):
        if type_intersection == 'A':
            return "left"
        elif type_intersection == 'B':
            return "left"
        elif type_intersection == 'C':
            return "straight"
        elif type_intersection == 'D':
            return "left"
        elif type_intersection == 'E':
            return "left"
        elif type_intersection == 'F':
            return "right"
        elif type_intersection == 'G':
            return "uturn"
        return "stop"
