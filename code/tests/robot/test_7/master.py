"""
    Description.
"""


import serial
import time


serial_port = '/dev/cu.DSDTECHHC-05-DevB';
baud_rate = 9600; 
ser = serial.Serial(serial_port, baud_rate)


# ------------
def send_pid():
    kp = 0
    kd = 0
    ki = 0
    while True:
        input_ = input("instruction,kp,kd,ki: ").split(',')
        instruction = int(input_[0])
        if len(input_) > 1:  # Update PID parameters
            kp = float(input_[1])
            kd = float(input_[2])
            ki = float(input_[3])
        msg = "<" + str(instruction) + "," + str(kp) + "," + str(kd) + \
                "," + str(ki) + ">"
        ser.write(msg.encode())


# ------------
if __name__ == "__main__":
    send_pid()