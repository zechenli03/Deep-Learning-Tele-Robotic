import serial #module for serial port communication

ser = serial.Serial('/dev/ttyS0', 9600, timeout=1) #Linux系统使用com0口连接串行口

#Note: Serial port read/write returns "byte" instead of "str" 
ser.write("testing serial connection\n".encode('utf-8'))
ser.write("sending via RPi\n".encode('utf-8'))

try:
    while 1:
        user = input() + '\n';
        ser.write(user.encode('utf-8'));
        response = ser.readline()
        print(response.decode('utf-8'))
        if(response.decode('utf-8')=='K'):
            user='A'
            ser.write(user.encode('utf-8'))
            res=ser.readline()
            if(response.decode('utf-8')=='C'):
                print(res.decode('utf-8'))
                print('Arduino Online')
                break;
            else :
                print('Arduino Missed')
        else :
            print('Arduino Missed')
except KeyboardInterrupt:
    ser.close()
      
