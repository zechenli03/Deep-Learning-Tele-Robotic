from __future__ import print_function
from tensorflow.python.keras.models import load_model
import tensorflow as tf

import numpy as np
from PIL import Image

MODEL_NAME = 'cat10086.hd5'
#MODEL_NAME = 'cattrain3.hd5'
dict={0:'Backward', 1:'Forward', 2:'Left', 3:'Right',4:'Stop'}

graph = tf.get_default_graph()


def classify(model,image):
	global graph
	with graph.as_default():
		result = model.predict(image)
		themax = np.argmax(result)

	return (dict[themax],result[0][themax],themax)

def load_image(image_fname):
	img = Image.open(image_fname)
	img = img.resize((249,249))
	imgarray = np.array(img)/255.0
	final = np.expand_dims(imgarray, axis=0)
	return final

def main():
	model = load_model(MODEL_NAME)
	img = load_image("c.JPG")
	label,prob,_ = classify(model,img)

	print("We think with certainty %3.2f that it is %s."%(prob, label))

if __name__=="__main__":
	main()

