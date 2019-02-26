import serial
import time
import threading
import math


# Constants
ID_MASTER = 0
SERIAL_PORT = '/dev/cu.DSDTECHHC-06-DevB';
BAUD_RATE = 9600; 


# ------------
def receive_terminal(ser):
    while True:
        raw_msg = ser.readline().decode("utf-8");
        
        # Check validity
        raw_msg = raw_msg.rstrip()  # remove end of line characters
        if (len(raw_msg) < 1 or raw_msg[0] != '<' or raw_msg[-1] != '>' ):
            return

        # Parse message
        parsed_msg = raw_msg[1:-1]  # remove <> symbols
        parsed_msg = parsed_msg.split('/')
        print("Parsed msg:" + str(parsed_msg))
        time.sleep(0.5)

ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
receive_terminal(ser)