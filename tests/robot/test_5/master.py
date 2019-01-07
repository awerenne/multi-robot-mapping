##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial
import time


serial_port = '/dev/cu.DSDTECHHC-05-DevB';
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
ser = serial.Serial(serial_port, baud_rate)


def read_test():
    write_to_file_path = "measures_test_2.csv";
    output_file = open(write_to_file_path, "w+");
    output_file.write("frequency_slave;arrival_time_step_master;seq_number")
    reference_time_step = time.time()
        
    while True:
        line = ser.read(20);
        # line = ser.readline();
        line = line.decode("utf-8") #ser.readline returns a binary, convert to string
        arrival_time_step_master = time.time()-reference_time_step
        print(line)
        # print(type(line))
        # msg = line.split('<')[0]
        # print(msg)
        msg = line.split(',')
        print(msg)
        # output = str(frequency_slave) + ";" + str(arrival_time_step_master) + ";" + str(seq_number)
        # output_file.write(output);


def write_test():
    for i in range(1000):
        msg = "<1," + str(i) + ",2>"
        ser.write(msg.encode())
        time.sleep(0.5)


read_test()
# write_test()