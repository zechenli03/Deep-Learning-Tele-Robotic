from picamera import PiCamera, Color
from time import sleep

#######################################
#Initialization
#######################################

demoCamera = PiCamera()


#######################################
#Simple Preview
#######################################

##demoCamera.start_preview()
##sleep(10)
##demoCamera.stop_preview()


########################################
#Rotation
########################################

##demoCamera.rotation = 180
##demoCamera.start_preview()
##sleep(10)
##demoCamera.stop_preview()


########################################
#Take picture
########################################

##demoCamera.start_preview()
##sleep(5)
##demoCamera.capture('/home/pi/Desktop/sample.jpg')
##demoCamera.stop_preview()


########################################
#Record Video
########################################

##demoCamera.start_preview()
##demoCamera.start_recording('/home/pi/Desktop/sampleVideo.h264')
##sleep(5)
##demoCamera.stop_recording()
##demoCamera.stop_preview()

########################################
#Annotation
########################################

demoCamera.start_preview()
demoCamera.annotate_background = Color('white')
demoCamera.annotate_foreground = Color('red')
demoCamera.annotate_text = " SWS3009B - 2018"
sleep(5)
demoCamera.capture('/home/pi/Desktop/classPhoto.jpg')
demoCamera.stop_preview()

