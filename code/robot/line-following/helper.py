"""
    Description.
"""


import serial
import time


serial_port = '/dev/cu.DSDTECHHC-05-DevB';
baud_rate = 9600; 
ser = serial.Serial(serial_port, baud_rate)


# ------------
def send_instruction():
    id_master = 0
    seq_number = 0
    while True:
        instruction = int(input("instruction: "))
        msg = "<" + str(id_master) + "," + str(seq_number) + \
            "," + str(instruction) + ">"
        seq_number += 1
        ser.write(msg.encode())


# ------------
if __name__ == "__main__":
    send_instruction()