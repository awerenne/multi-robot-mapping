"""
    Description.
"""

import serial
import time
import threading
import math


# Constants
TEST = 3
ID_MASTER = 0
SERIAL_PORT = '/dev/cu.DSDTECHHC-05-DevB';
BAUD_RATE = 9600; 


# ------------
def send(ser):
    while True:
        id_master = 0
        seq_number = 0
        instruction = int(input("instruction: "))
        msg = "<" + str(id_master) + "/" + str(seq_number) + \
            "/" + str(instruction) + ">"
        seq_number += 1
        ser.write(msg.encode())
        time.sleep(0.5)


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


# ------------
def receive_file(ser):
    write_to_file_path = "data/measures_test_" + str(TEST) + ".csv";
    output_file = open(write_to_file_path, "w+");
    while True:
        raw_msg = ser.readline().decode("utf-8");
        
        # Check validity
        raw_msg = raw_msg.rstrip()  # remove end of line characters
        if (len(raw_msg) < 1 or raw_msg[0] != '<' or raw_msg[-1] != '>' ):
            return

        # Parse message
        parsed_msg = raw_msg[1:-1]  # remove <> symbols
        data = parsed_msg.split('/')[-1]
        output_file.write(data)
        time.sleep(0.5)


# ------------
if __name__ == "__main__":
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

    try:
        # Reception of messages from robot are catched on separate thread
        if TEST == 2 or TEST == 3:
            t1=threading.Thread(target=receive_terminal, args=[ser]).start()  
        else:
            t1=threading.Thread(target=receive_file, args=[ser]).start()  
    except:
        print("Error: unable to start reception thread")

    send(ser)






