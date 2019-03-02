"""
    Description.
"""


import serial
import time

SERIAL_PORT = '/dev/cu.HC-05-DevB';
BAUD_RATE = 9600; 


# ------------
def receive_terminal(ser):
    while True:
        time.sleep(0.5)
        raw_msg = ser.readline().decode("utf-8").rstrip();
        if (len(raw_msg) < 1 or raw_msg[0] != '<' or raw_msg[-1] != '>' ):
            continue
        parsed_msg = raw_msg[1:-1].split('/')
        print("Message:" + str(parsed_msg))
        

# ------------
# def receive_terminal(ser):
#     while True:
#         time.sleep(0.5)
#         raw_msg = ser.readline().decode("utf-8").rstrip();
#         if (len(raw_msg) < 1): continue
#         print("Message:" + str(raw_msg))


# ------------
if __name__ == "__main__":
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
    receive_terminal(ser)


# ------------
# def receive_file(ser):
#     write_to_file_path = "data/measures_test_" + str(TEST) + ".csv";
#     output_file = open(write_to_file_path, "w+");
#     while True:
#         raw_msg = ser.readline().decode("utf-8");
        
#         # Check validity
#         raw_msg = raw_msg.rstrip()  # remove end of line characters
#         if (len(raw_msg) < 1 or raw_msg[0] != '<' or raw_msg[-1] != '>' ):
#             return

#         # Parse message
#         parsed_msg = raw_msg[1:-1]  # remove <> symbols
#         data = parsed_msg.split('/')[-1]
#         output_file.write(data)
#         time.sleep(0.5)














