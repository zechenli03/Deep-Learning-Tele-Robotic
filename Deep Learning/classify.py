from __future__ import print_function
import paho.mqtt.client as mqtt
import numpy as np
from PIL import Image
from prediction import classify
from tensorflow.python.keras.models import load_model

TMP_FILE = "/tmp/tmp1.jpg"
dict = {0:'Backward', 1:'Forward', 2:'left', 3:'Right',4:'Stop'}

model = load_model('cat10086.hd5')
#model = load_model('cattrain3.hd5')
def load_image(image):
    img = Image.open(image)
    img = img.resize((249,249))
    imgarray = np.array(img)/255.0
    final = np.expand_dims(imgarray,axis=0)
    return final


def predict(imgarray, dict):
    return classify(model,imgarray)


def on_connect(client, userdata, flags, rc):
    print("Connected with result code %d."%(rc))
    client.subscribe("IMAGE/classify")


def on_message(client, userdata, msg):
    print("Received message. Writting to %s."%(TMP_FILE))
    img = msg.payload

    with open(TMP_FILE, "wb") as f:
        f.write(img)

    imgarray = load_image(TMP_FILE)

    label,prob,index = predict(imgarray, dict)

    print("Classified as %s with certainty %3.4f."%(label, prob))

    client.publish("IMAGE/predict", label+":"+str(prob)+":"+str(index))


def setup(host_name):
    global dict
    global client

    client = mqtt.Client()
    client.username_pw_set(username="Ryan_Lee",password="Lzc970303")
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(host_name)
    client.loop_start()


def main():
    setup("team8.sws3009.bid")
    while True:
        pass


if __name__ == '__main__':
    main()
