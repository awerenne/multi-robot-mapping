
from threading import Thread
import time


# ---------------
class Robot(Thread):
    
    def __init__(self, id_, queues, environment):
        Thread.__init__(self)
        self.id_ = id_
        self.q = queues
        self.stop = True
        self.distance = 0
        self.seq_send = 0
        self.environment = environment

    # -------
    def run(self):
        while True:
            time.sleep(0.1)
            if len(self.q.messenger2robots.queue) > 0 and \
                self.next_is_for_me(self.q.messenger2robots):
                    msg = self.q.messenger2robots.get_nowait()
                    if not msg is None:
                        self.execute_instruction(msg)
                    continue
            if not self.stop:  
                self.move()
                self.distance += 5
                intersection = self.sensor_read()
                if not intersection is None:
                    self.send_information(intersection)
                    if intersection != -1:
                        self.distance = 0
                    self.stop = True

    # -------
    def execute_instruction(self, msg):
        _, instruction = self.parse_instruction(msg)
        if instruction == 0:
            self.stop = True
        elif instruction == 1:
            self.stop = False
        elif instruction == 2:
            self.turn_left()
            self.stop = False
        elif instruction == 3:
            self.turn_right()
            self.stop = False
        elif instruction == 4:
            self.uturn()
            self.stop = False
        return self

    # -------
    def parse_instruction(self, msg):
        msg = msg[1:-1]  # remove delimeters
        (id_robot, seq_number, instruction) = msg.split('/') 
        return int(id_robot), int(instruction)

    # -------
    def next_is_for_me(self, instruction):
        id_robot, _ = self.parse_instruction(self.q.messenger2robots.queue[0])
        return id_robot == self.id_

    # -------
    def send_information(self, intersection):
        msg = "<" + str(self.id_) + "/" + str(self.seq_send) + "/" + \
                str(intersection) + ";" + str(self.distance) + ">"
        self.q.robots2messenger.put(msg)
        self.seq_send += 1
        return self

    # -------
    def move(self):
        self.environment.move(self.id_)
        return self

    # -------
    def turn_left(self):
        self.environment.turn_left(self.id_)
        return self

    # -------
    def turn_right(self):
        self.environment.turn_right(self.id_)
        return self

    # -------
    def uturn(self):
        self.environment.uturn(self.id_)
        return self

    # -------
    def sensor_read(self):
        path_left, path_front, path_right = self.environment.sensor_read(self.id_)
        if path_left == -1 and path_front == -1 and path_right == -1:
            return -1

        if path_left and path_right and path_front: 
            return 0
        if path_left and not path_right and path_front: 
            return 1
        if not path_left and path_right and path_front: 
            return 2
        if path_left and path_right and not path_front: 
            return 3
        if path_left and not path_right and not path_front: 
            return 4
        if not path_left and path_right and not path_front: 
            return 5
        if not path_left and not path_right and not path_front: 
            return 6
        return None


        













    