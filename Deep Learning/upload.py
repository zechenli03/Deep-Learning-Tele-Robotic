from __future__ import print_function
import paho.mqtt.client as mqtt
import time
import os
#username_pw_set(username="caden",password="team8")
resp_callback=None

def on_connect(client,userdata,flags,rc):
        print("Connected.Result code is %d."%(rc))
        client.subscribe("IMAGE/predict")

def on_message(client,userdata,msg):
        print("Received message from server.")

#       str=msg.payload.split(":")
        str = msg.payload.decode('utf-8')
        str=str.split(':')

        if resp_callback is not None:
            resp_callback(str[0],float(str[1]),int(str[2]))

def setup(hostname):
        global client
        client=mqtt.Client()
        client.username_pw_set(username="Ryan_Lee",password="Lzc970303")
        client.on_connect=on_connect
        client.on_message=on_message
        client.connect(hostname)
        client.loop_start()

def load_image(filename):
        with open(filename,"rb")as f:
            data=f.read()
        return data

def send_image(filename):
        img=load_image(filename)
        client.publish("IMAGE/classify",img)
        time.sleep(1)

def resp_handler(label,prob,index):
        print("\n -- Response -- \n\n")
        print("Label:%s"%(label))
        print("Probability:%3.4f"%(prob))
        print("Index:%d"%(index))

def pic_num(filepath):
        return len(os.listdir(filepath))

def main():
        global resp_callback

        setup("team8.sws3009.bid")
        resp_callback=resp_handler
        print("Waiting 2 seconds before sending.")
        time.sleep(2)
#        a = os.getcwd()
        num1 = pic_num("./Cats")
#        print("num_now:%d" % (num1))
        for i in range(1, num1+1):
            time.sleep(2)
            print("-------------------------------------------------")
            print("Sending %d.jpg" % (i))
            print("-------------------------------------------------")
            send_image("./Cats/%d.jpg" %(i))
            print('')
        while (1):
            num = pic_num('./Cats')
            if num > num1:
                for i in range(num1 + 1, num + 1):
                    time.sleep(2)
                    print("-------------------------------------------------")
                    print("Sending %d.jpg" %(i))
                    print("-------------------------------------------------")
                    send_image("./Cats/%d.jpg" %(i))
                    print('')
                num1 = num
        while True:
             pass

if __name__ == '__main__':
    main()
