#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "GPIO.h"

using namespace std;
using namespace cv;
// Pin capable of PWM is pin 12th

int main() {
    VideoCapture stream1(0); // Access the webcam /dev/video0

    // Check if the webcam has been initialized
    if (!stream1.isOpened()){
        cout << "Cannot open camera";
    }

    // Infinite loop to get the video feed
    while (true) {
        /* 
         * Treats the video feed as a series of images
         * Each image is handled as a matrix
         */
        Mat cameraFrame;
        stream1.read(cameraFrame);
        imshow("Camera", cameraFrame);

        if (waitKey(30) >= 0) break;

    }

return 0;
}