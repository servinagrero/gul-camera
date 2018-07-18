#Author Fernando Cerezal (fernando@cerezal.org)
#License: Public Domain

import cv2
import sys
import serial
import time

# this is designed to be used with an arduino with opencv-data and opencv runtime packages installed

# /dev/ttyACM0 is the arduino atached port
ser = serial.Serial('/dev/ttyACM0', 115200)

# opencv-data should be installed for geting this file
cascPath = "/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascPath)

video_capture = cv2.VideoCapture(1)

# vertical limits. Otherway servo could not have force to restore position
maxV = 120
minV = 50

# initial parameters
max_size = 0
bigger_face = 0
desviation_h = 0
desviation_v = 0
x = 0
y = 0
w = 0
h = 0
write = False
read = 0

# initial position. It should be 90 degrees vertical and 90 degrees horizontal, but cheap servos are not perfect and manual mounting is not perfect.
current_h = 90
current_v = 85

# Moving parameters
step = 1  # every loop ask for moving "step" degrees. Increase this for more agressive movement. Higher values can lead to oscillation movement.
threshold = 50  # number of pixels the face should move from center to perform movement. Lower values can lead to oscillation movement.

while True:
    # Capture a frame
    ret, frame = video_capture.read()

    # ser.write("h 0"+"\n")

    #  cv2.flip(frame, a) # if camera is upside down

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  # to grayscale

    # this returns an array of squares with position and dimensions
    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.3,
        minNeighbors=5,
        minSize=(30, 30),
        flags=cv2.cv.CV_HAAR_SCALE_IMAGE)

    if len(faces) != 0:

        # Draw a rectangle around the faces
        for (x, y, w, h) in faces:
            if w * h > max_size:
                max_size = w * h
                bigger_face = (x, y, w, h)

        # this draw a rectangle over selected face. It can be commented
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # calculate center of image
        vmiddle = frame.shape[0] / 2
        hmiddle = frame.shape[1] / 2

        # calculate desviation and send commands to arduino
        desviation_h = hmiddle - (x + w / 2)
        # print("dev_h: "+str(desviation_h));
        if desviation_h > threshold:
            # print("desv + h"+str(desviation_h))
            current_h += step
            if current_h >= 180:
                current_h = 180
                ser.write("h " + str(current_h) + "\r\n")
                write = True
                # print("h "+str(current_h));
        elif desviation_h < -1 * threshold:
            # print("desv - h "+str(desviation_h))
            current_h -= step
            if current_h <= 1:
                current_h = 1
                ser.write("h " + str(current_h) + "\r\n")
                write = True
                # print("h "+str(current_h));

        desviation_v = vmiddle - (y + h / 2)
        # print("dev_v: "+str(desviation_v));
        if desviation_v < -1 * threshold:
            # print("desv + v "+str(desviation_v))
            current_v += step
            if current_v >= maxV:
                current_v = maxV
                ser.write("v " + str(current_v) + "\r\n")
                write = True
                # print("v "+str(current_v));
        elif desviation_v > threshold:
            # print("desv - v "+str(desviation_v))
            current_v -= step
            if current_v <= minV:
                current_v = minV
                ser.write("v " + str(current_v) + "\r\n")
                write = True
                # print("v "+str(current_v));

        # arduino can be much more slow than computer, so
        # arduino sends a "k" when an order is complete and it is ready.
        # send too much orders to arduino with wait to complete can lead to malfunction
        if write:
            read = ser.readline().rstrip()
            # print("read###"+read+"###")
            while read != "k":
                read = ser.readline()
                # print("waiting "+read)
                time.sleep(0.1)
                # print("end waiting")
            write = False

    # print("x: {0} y: {1}").format(desviation_h,desviation_v)
    # Display the resulting frame. It can be commented
    cv2.imshow('Video', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()
