import RPi.GPIO as GPIO
import time
import os

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(24, GPIO.OUT)

GPIO.output(24, GPIO.LOW)
        

time.sleep(30)


GPIO.output(24, GPIO.HIGH)
        

time.sleep(60)

import Auto

