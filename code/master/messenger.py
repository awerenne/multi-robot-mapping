""" 
    Description.
"""


import serial
import time
import threading
from queue import Queue


# TODO: proper thread quitting (very low priority)


# ------------
class Messenger(Thread):
    """
    For the moment do as if only one robot. Blabla.
    """

    def __init__(self, queue_to_master, queue_from_master):
        self.queue_to_master = queue_to_master
        self.queue_from_master = queue_from_master
        self.serial_port = '/dev/cu.DSDTECHHC-05-DevB';  # TODO: put in config file
        self.baud_rate = 9600;
        self.ser = serial.Serial(self.serial_port, self.baud_rate)
        self.seq_number_sending = 0
        self.seq_number_receiving = 0
        self.id = 0

    # ------------
    def run(self):
        receiver_thread = threading.Thread(target=self.receive).start()
        sender_thread = threading.Thread(target=self.send).start()

    # ------------
    def send(self, instruction):
        while True:
            if not self.queue_from_master.empty():
                directive = self.queue_from_master.get()
                msg = "<" + str(self.id) + "/" + str(self.seq_number_sending) + \
                    "/" + str(directive.instruction) + ">"
                self.ser.write(msg.encode())
                self.seq_number_sending += 1
                time.sleep(0.5)

    # ------------
    def receive(self):
        while True:
            raw_msg = self.ser.readline().decode("utf-8");
            
            # Delimiting
            raw_msg = raw_msg.rstrip()  # remove end of line characters
            if (len(raw_msg) < 1 or raw_msg[0] != '<' or raw_msg[-1] != '>' ):
                continue  # TODO change - handle error
            delimited_msg = raw_msg[1:-1]  # remove '<', '>' delimeters

            (id_robot, seq_number_receiving, information) = delimited_msg.split('/')

            # Error checking (TODO: checksum and better valid seq number check)
            if seq_number_receiving < self.seq_number_receiving:
                continue
            self.seq_number_receiving = seq_number_receiving

            directive = make_directive(id_robot, information)
            self.queue_to_master.put(directive)
            time.sleep(0.5)

    # ------------
    def make_directive(self, id_robot, information):
        (type_intersection, distance) = information.split(';')
        directive = {
            "type_directive": "update"
            "id_robot": id_robot,
            "type_intersection": int(type_intersection), 
            "distance": int(distance)  # TODO: round to nearest!!!
        }
        return Container(directive)






















        