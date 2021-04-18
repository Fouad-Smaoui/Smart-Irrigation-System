import RPi.GPIO as GPIO
import time
 
#setting up gpio 
pin = [21]
GPIO.setmode(GPIO.BCM)
GPIO.setup(pin, GPIO.IN)
 
def callback(pin):
        if GPIO.input(pin):
				print "No Water Detected!"
				x=x+1
				
        else:
                print "Water Detected!"
				
GPIO.add_event_detect(pin, GPIO.BOTH, bouncetime=300)
GPIO.add_event_callback(pin, callback)  

while True:
        time.sleep(1)

