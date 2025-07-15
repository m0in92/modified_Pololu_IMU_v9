import math
import time

from vpython import canvas, color, vector, box, arrow, cylinder
import serial


# Constants
GRAD2RAD: float = 3.141592/180.0

# Default directions
up_direction: vector = vector(0,0,1)
forward_direction: vector = vector(1,0,0)

# Canvas object is automatically created
main_canvas: canvas = canvas(background = color.black)
main_canvas.title = "IMU Heading"
main_canvas.select()
main_canvas.up = vector(0,0,1)
main_canvas.forward = forward_direction

x_init: float = 1
y_init: float = 0
mag_arrow: float = 1
main_arrow: arrow = arrow(pos=vector(0,0,0), axis=forward_direction, shaft_width=0.06, fixed_width=1)

ser = serial.Serial(port='COM9',baudrate=9600, timeout=1)
roll=0
pitch=0
yaw=0
while 1:
    line: bytes = ser.readline()
    line: str = line.decode("utf-8")
    if line.find("!ANG:") != -1:
        line = line.replace("!ANG:", "")
        data: list = line.split(",")
        if len(data) > 2:
            try:
                roll: float = float(data[0]) * GRAD2RAD
                pitch: float = float(data[1]) * GRAD2RAD
                yaw: float = float(data[2]) * GRAD2RAD
            except:
                ValueError("Unextractable data.")
            axis=vector(math.cos(pitch)*math.cos(yaw),
                        -math.cos(pitch)*math.sin(yaw),
                        math.sin(pitch)) 
            up=vector(math.sin(roll)*math.sin(yaw)+math.cos(roll)*math.sin(pitch)*math.cos(yaw), 
                      math.sin(roll)*math.cos(yaw)-math.cos(roll)*math.sin(pitch)*math.sin(yaw),
                      -math.cos(roll)*math.cos(pitch))
            main_arrow.axis = axis
            main_arrow.up = up
            main_arrow.length=0.8
ser.close()
