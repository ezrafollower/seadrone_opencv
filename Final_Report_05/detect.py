import cv2
import numpy as np
from picamera.array import PiRGBArray
from picamera import PiCamera
import serial
import rospy
import time

def nothing(x):
	pass

cv2.namedWindow("Trackbars")

cv2.createTrackbar("LH", "Trackbars", 0, 255, nothing)
cv2.createTrackbar("LS", "Trackbars", 0, 255, nothing)
cv2.createTrackbar("LV", "Trackbars", 0, 255, nothing)
cv2.createTrackbar("UH", "Trackbars", 0, 255, nothing)
cv2.createTrackbar("US", "Trackbars", 0, 255, nothing)
cv2.createTrackbar("UV", "Trackbars", 0, 255, nothing)

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size= (640, 480))
font = cv2.FONT_HERSHEY_COMPLEX
s = serial.Serial("/dev/ttyUSB0", 9600)


for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	image = frame.array
	blurred_image = cv2.GaussianBlur(image, (5, 5), 0)
	hsv = cv2.cvtColor(blurred_image, cv2.COLOR_BGR2HSV)
	
	# detect red
	'''
	LH = cv2.getTrackbarPos("LH", "Trackbars")
	LS = cv2.getTrackbarPos("LS", "Trackbars")
	LV = cv2.getTrackbarPos("LV", "Trackbars")
	UH = cv2.getTrackbarPos("UH", "Trackbars")
	US = cv2.getTrackbarPos("US", "Trackbars")
	UV = cv2.getTrackbarPos("UV", "Trackbars")
	lower_red = np.array([LH, LS, LV])
	upper_red = np.array([UH, US, UV])
	'''	
	#lower_red = np.array([155, 55, 55])
	#upper_red = np.array([190, 255, 255])
	lower_red = np.array([0, 40, 51])
	upper_red = np.array([4, 255, 255])
	mask_red = cv2.inRange(hsv, lower_red, upper_red)
	
	# detect green
	lower_green = np.array([52, 135, 55])
	upper_green = np.array([85, 255, 255])
	mask_green = cv2.inRange(hsv, lower_green, upper_green)

	# detect blue
	lower_blue = np.array([89, 127, 61])
	upper_blue = np.array([168, 255, 255])
	mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)
	
	mask = mask_red + mask_green + mask_blue

	result = cv2.bitwise_and (image, image, mask=mask)	
	act = "0"
	s.write(act)

	_, contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

	for contour in contours:
		M = cv2.moments(contour)
		
		area = cv2.contourArea(contour)
		if area > 20000:
			cx = int(M['m10']/M['m00'])
			cy = int(M['m01']/M['m00'])
			peri = cv2.arcLength(contour, True)
			approx = cv2.approxPolyDP(contour, 0.03*peri, True)
			cn = 0
			cs = 0


			if len (approx) == 3:
				shape = "Triangle"
				cs = 2
			elif len (approx) == 4:
				shape = "Rectangle"
				cs = 3
			elif 5 <= len (approx) <= 8:
				shape = "Circle"
				cs = 1
			else:
				shape = "not detect shape"
				cs = 0
				
		
			c = image[cy][cx]
			if c[0] > c[1] and c[0] > c[2] and c[0] > 80:
				color = "Blue"
				cn = 3
			elif c[1] > c[0] and c[1] > c[2] and c[1] > 80:
				color = "Green"
				cn = 2
			elif c[2] > c[0] and c[2] > c[1] and c[2] > 80:
				color = "Red"
				cn = 1
			else:
				color = "not detect color"
				cn = 0


			if cs==1 and cn==1:	
				act = "1"		
				s.write(act)
			elif cs==1 and cn==2:
				act = "2"				
				s.write(act)
			elif cs==1 and cn==3:
				act = "3"
				s.write(act)			
			elif cs==2 and cn==1:
				act = "4"
				s.write(act)
			elif cs==2 and cn==2:
				act = "5"
				s.write(act)
			elif cs==2 and cn==3:
				act = "6"
				s.write(act)
			elif cs==3 and cn==1:
				act = "7"
				s.write(act)
			elif cs==3 and cn==2:
				act = "8"
				s.write(act)
			elif cs==3 and cn==3:
				act = "9"
				s.write(act)
			else:
				act = "0"
				s.write(act)
			
			#cn = 0
			#cs = 0
			s.write(act)
			#print(act)
			print(s.readline())
			#print("shape: %d / color: %d"%(cs,cn))
			cv2.drawContours(result, contour, -1, (0, 255, 0), 3)
			text = "{} / {} / {}".format(color, shape, act)
			cv2.putText(image, text, (cx, cy), font, 0.5, (255, 255, 255), 2)			
			cv2.putText(result, text, (cx, cy), font, 0.5, (255, 255, 255), 2)
			#print(s.readline())
			#time.sleep(1)
			
		

	cv2.imshow("frame", image)
	#cv2.imshow("mask", mask)
	#cv2.imshow("mask_red", mask_red)
	#cv2.imshow("mask_green", mask_green)
	#cv2.imshow("mask_blue", mask_blue)
	cv2.imshow("result", result)

	key = cv2.waitKey(1)
	rawCapture.truncate(0)
	if key == 27:
		break

cv2.destroyAllWindows()
