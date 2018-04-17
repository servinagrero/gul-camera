#include <iostream>
#include <sstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "GPIO.h"

using namespace std;
using namespace cv;
// Pins capables of PWM are pin 12th and pin 17th

 /** Global variables */
String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
RNG rng(12345);

int main() {
    VideoCapture stream1(0); // Access the camera /dev/video0

    // Check if the webcam has been initialized
    if (!stream1.isOpened()){
        cout << "Cannot open camera" << endl;
        return -1;
    }

    // Load the cascades
    if( !face_cascade.load( face_cascade_name ) )
    {
      cout << "Could not load face cascade" << endl;
      return -1;
    }
    
    if( !eyes_cascade.load( eyes_cascade_name ) )
    {
      cout << "Could not load eyes cascade" << endl;
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

    // Main loop
    while (true) {
        /* 
         * Treats the video feed as a series of images
         * Each image is handled as a matrix
         */
            
        std::vector<Rect> faces;
        Mat cameraFrame;
        Mat frame_gray;
        
        stream1.read(cameraFrame); // Reads image from camera
        oVideoWriter.write(cameraFrame); // Write the image to file

        cvtColor( cameraFrame, frame_gray, CV_BGR2GRAY );
        equalizeHist(frame_gray, frame_gray);

        // Detect faces
        face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

        for( size_t i = 0; i < faces.size(); ++i)
        {
                Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
                ellipse( cameraFrame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

                Mat faceROI = frame_gray( faces[i] );
                std::vector<Rect> eyes;

                //Detect eyes for each face
                eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
                
                for(size_t j = 0; j < eyes.size(); ++j)
                {
                        Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
                        int radius = cvRound( (eyes[j].width+eyes[j].height)*0.25 );
                        circle( cameraFrame, center, radius, Scalar(255, 0, 0), 4, 8, 0 );
                }
        }
        
        imshow("Camera", cameraFrame); // Shows the image in the window


        if (waitKey(10) == 27) break; // Wait for the user to press Scape

    }

return 0;
}
