#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
// #include <Serial.h>

#include "GPIO.h"

using namespace cv;

// Function Definitions
void detect_faces( Mat frame, std::vector<Rect> faces, Mat* camera_frame, Point* coord );

// Global variables 
String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
int face_threshold = 50;
RNG rng(12345);

int main() {
  
        VideoCapture stream1(0); // Access the camera /dev/video0
    
        std::fstream serial_ard;
        serial_ard.open("/dev/ttyACM0", std::ios::out | std::ios::binary );

        if ( !serial_ard.is_open() ) {
                std::cout << "Cannot connect to arduino" << std::endl;
                return -1;
        } 

        // Serial serial("COM0");

        // Check if the webcam has been initialized
        if (!stream1.isOpened()){
                std::cout << "Cannot open camera" << std::endl;
                return -1;
        }

        // Load the cascades
        if( !face_cascade.load( face_cascade_name ) )
        {
                std::cout << "Could not load face cascade" << std::endl;
                return -1;
        }

        // Creates a window for the recording
        // namedWindow("Grabación", CV_WINDOW_AUTOSIZE);

        // Dimensions of the video recorging
        double dWidth = stream1.get( CV_CAP_PROP_FRAME_WIDTH );
        double dHeight = stream1.get( CV_CAP_PROP_FRAME_HEIGHT );
        std::cout << "Frame size: " << dWidth << "x" << dHeight << std::endl;

        Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

        // Name of the file to write
        // grabacion + _ + day + time at start
        time_t temp_time = time( NULL );
        struct tm *date = localtime( &temp_time );
        std::string file_name = "Grabacion_" +
                std::to_string(date->tm_mday) + "_" +
                std::to_string(date->tm_hour) + ":" +
                std::to_string(date->tm_min) + ":" +
                std::to_string(date->tm_sec) + ".avi";

        /*
         * Create a VideWriter object to write the images into a file
         * CV_FOURCC is the 4 byte code to select the video codec
         */
        VideoWriter oVideoWriter( file_name, CV_FOURCC('P','I','M','1'), 20, frameSize, true );

        // Check if we can write the video feed to the file
        if( !oVideoWriter.isOpened() )
        {
                std::cout << "Failed to write to video" << std::endl;
                return -1;
        } 

        // Stores the coordinates of center of the biggest face
        // Rect face_pos;
        Point coord;

        // Main loop
        while ( true )
        {
                // Treats the video feed as a series of images
                // Each image is handled as a matrix
                std::vector<Rect> faces;
                Mat cameraFrame;
                Mat frame_gray;
     
                stream1.read( cameraFrame ); // Reads image from camera

                if( !stream1.read( cameraFrame ) )
                {
                        std::cout << "Failed to read frame" << std::endl;
                        return -1;
                }
        
                oVideoWriter.write( cameraFrame ); // Write the image to file

                // Convert the image to gray scale
                cvtColor( cameraFrame, frame_gray, CV_BGR2GRAY );
                equalizeHist( frame_gray, frame_gray );

                // Detect the faces on the current frame
                detect_faces( frame_gray, faces, &cameraFrame, &coord );
                imshow( "Camera", cameraFrame ); // Shows the image in the window

                // Send the data to the arduino
                // serial.write((uint8_t *)&coord, sizeof(coord));

                // Wait for the user to press Scape
                if ( waitKey(10) == 27 )
                {
                        stream1.release();
                        destroyAllWindows();
                        break;
                }
        
        }

        return 0;
}

/*
 * Accepts the current frame as a parameter
 * Returns the coordinates of the nearest face in the vision
 */

void detect_faces( Mat frame, std::vector<Rect> faces, Mat* camera_frame, Point* coord )
{

        // Detect faces
        face_cascade.detectMultiScale( frame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

        cv::Rect face_nearest; //Stores the nearest face
  
        /*
         * For all the faces that are detected we need the biggest one
         * Clasify all faces but use the nearest one
         */

        for( size_t i = 0; i < faces.size(); ++i )                                                                                          
        {
                if( faces[i].width > face_nearest.width && faces[i].height > face_nearest.height && face_nearest != faces[i] )
                {
                        face_nearest = faces[i];
                }
        }

        Point center( face_nearest.x + face_nearest.width*0.5, face_nearest.y + face_nearest.height*0.5);
        std::cout << "Face at x:" << face_nearest.x << " y:" << face_nearest.y << std::endl;
        ellipse( *camera_frame, center, Size( face_nearest.width*0.5, face_nearest.height*0.5 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

        // Calculates the distance from the center of the face to the center of the frame
        face_nearest.x = (camera_frame->cols) - face_nearest.x;
        face_nearest.y = (camera_frame->rows) - face_nearest.y ;

        if ( face_nearest.x > face_threshold || face_nearest.y > face_threshold )
        {
                coord->x = face_nearest.x;
                coord->y = face_nearest.y;
                
        }

        std::cout << "Distance x:" << face_nearest.x << " y:" << face_nearest.y << "\n" << std::endl;
}
