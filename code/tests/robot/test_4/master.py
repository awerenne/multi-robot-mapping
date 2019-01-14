"""
    Description.
"""


import serial
import time

serial_port = '/dev/cu.DSDTECHHC-05-DevB';
baud_rate = 9600; 
ser = serial.Serial(serial_port, baud_rate)


# ------------
def test_4_1():
    delay = 0.5 
    T = 20
    t = 0
    id_master = 0
    seq_number = 0
    instruction = 2
    while t < T:
        msg = "<" + str(id_master) + "," + str(seq_number) + \
            "," + str(instruction) + ">"
        seq_number += 1
        ser.write(msg.encode())
        time.sleep(delay)
        t += delay


# ------------
def test_4_2():
    write_to_file_path = "measures_test_2.csv";
    output_file = open(write_to_file_path, "w+");
    output_file.write("frequency_slave;arrival_time_step_master;seq_number")
    init_t = time.time()
        
    while True:
        raw_msg = ser.readline().decode("utf-8");
        t = time.time()-init_t
        print("Raw msg:" + str(raw_msg))
        
        # Check validity
        raw_msg = raw_msg.rstrip()  # remove end of line characters
        if (len(raw_msg) < 1 or raw_msg[0] != '<' or raw_msg[-1] != '>' ):
            print("Non-valid message")
            continue

        # Parse message
        parsed_msg = raw_msg[1:-1]  # remove <> symbols
        parsed_msg = parsed_msg.split(',')
        print("Parsed msg:" + str(parsed_msg))
        # output = str(frequency_slave) + ";" + str(arrival_time_step_master) + ";" + str(seq_number)
        # output_file.write(output);


# ------------
if __name__ == "_main__":
    test_4_1()
    # test_4_2()











    