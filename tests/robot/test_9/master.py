##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial
import time


serial_port = '/dev/cu.DSDTECHHC-05-DevB';
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
ser = serial.Serial(serial_port, baud_rate)


def write_test():
    kp = 0
    kd = 0
    ki = 0
    while True:
        text = input("mode,kp,kd,ki: ")
        cmd = text.split(',')
        mode = int(cmd[0])
        if len(cmd) > 1:  # Update PID parameters
            kp = float(cmd[1])
            kd = float(cmd[2])
            ki = float(cmd[3])
        msg = "<" + str(mode) + "," + str(kp) + "," + str(kd) + "," + str(ki) + ">"
        ser.write(msg.encode())


write_test()