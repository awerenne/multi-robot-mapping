import serial
import time

serial_port = '/dev/cu.DSDTECHHC-05-DevB';
baud_rate = 9600; 
ser = serial.Serial(serial_port, baud_rate)


######################
def send_instruction():
    id_master = 0
    seq_number = 0
    instruction = int(input("instruction: "))
    msg = "<" + str(id_master) + "," + str(seq_number) + \
        "," + str(instruction) + ">"
    seq_number += 1
    ser.write(msg.encode())


######################
def receive_type_intersection():
    raw_msg = ser.readline().decode("utf-8");
    
    # Check validity
    raw_msg = raw_msg.rstrip()  # remove end of line characters
    if (len(raw_msg) < 1 or raw_msg[0] != '<' or raw_msg[-1] != '>' ):
        return

    # Parse message
    parsed_msg = raw_msg[1:-1]  # remove <> symbols
    parsed_msg = parsed_msg.split(',')
    print("Parsed msg:" + str(parsed_msg))


######################


send_instruction()
while True:
    receive_type_intersection()