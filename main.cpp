#include <iostream>
#include <sstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "GPIO.h"

using namespace std;
using namespace cv;
// Pin capable of PWM is pin 12th

int main() {
    VideoCapture stream1(0); // Access the camera /dev/video0

    // Check if the webcam has been initialized
    if (!stream1.isOpened()){
        cout << "Cannot open camera" << endl;
        return -1;
    }

    // Creates a window for the recording
    // namedWindow("Grabación", CV_WINDOW_AUTOSIZE);


    // Dimensions of the video recorging
    double dWidth = stream1.get(CV_CAP_PROP_FRAME_WIDTH);
    double dHeight = stream1.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "Frame size: " << dWidth << "x" << dHeight << endl;

    Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

    VideoWriter oVideoWriter("grabacion.avi", CV_FOURCC('P','I','M','1'), 30, frameSize, true);

    // Check if we can write the video feed to the file
    if( !oVideoWriter.isOpened())
    {
        cout << "Failed to write to video" << endl;
        return -1;
    }
    // Infinite loop to get the video feed
    while (true) {
        /* 
         * Treats the video feed as a series of images
         * Each image is handled as a matrix
         */
        Mat cameraFrame;
        stream1.read(cameraFrame);

        oVideoWriter.write(cameraFrame);
        imshow("Camera", cameraFrame);

        if (waitKey(10) == 27) break; // Wait for the user to press Scape

    }

return 0;
}