import serial
import time

import time
import serial
ser = serial.Serial(“/dev/ttyS0”, baudrate = 9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=1)
counter=0
while 1:
    ser.write(b’Write counter: %d \n’%(counter))
    print(“Printed”)
    time.sleep(1)
    counter += 1