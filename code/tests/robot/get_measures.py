##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial
import subprocess
import sys
import time

def read_data():
    serial_port = '/dev/cu.wchusbserial14120';
    # serial_port = '/dev/cu.wchusbserial14130';
    # serial_port = '/dev/cu.wchusbserial1d1120';
    baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
    write_to_file_path = "measures.txt";

    output_file = open(write_to_file_path, "w+");
    ser = serial.Serial(serial_port, baud_rate)
    while True:
        line = ser.readline();
        line = line.decode("utf-8") #ser.readline returns a binary, convert to string
        print(line);
        output_file.write(line);

def read_via_bleutooth():
    ser = serial.Serial('/dev/cu.HC-05-DevB', 9600, timeout=0.1)
    write_to_file_path = "measures.txt";
    output_file = open(write_to_file_path, "w+");
    seq_number = -1
    while True:
        time.sleep(0.05)
        try: 
            msg = ser.readline().decode("utf-8").rstrip();
            if (len(msg) < 1 or msg[0] != '<' or msg[-1] != '>' ): continue
            msg = msg[1:-1]
            msg = msg.split('/')
        except: continue
        if len(msg) != 3: continue
        print(msg)
        if seq_number >= int(msg[1]): continue
        seq_number = int(msg[1])
        msg = msg[2].split(';')
        if len(msg) != 3: continue
        print(msg[0]+";"+msg[1]+";"+msg[2]);
        output_file.write(msg[0]+";"+msg[1]+";"+msg[2]+"\n");
        output_file.flush()

if __name__ == '__main__':
    if len(sys.argv) <= 1: read_data()
    else: read_via_bleutooth()