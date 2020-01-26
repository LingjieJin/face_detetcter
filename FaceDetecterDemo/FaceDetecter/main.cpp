//
//  main.cpp
//  FaceDetecter
//
//  Created by kIng_martin on 2019/10/8.
//  Copyright © 2019 Jin Lingjie. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        return -1;
    }
    
    
    Mat frame;
    Mat gray;
    
    CascadeClassifier face_cascade;
    
    face_cascade.load("/usr/local/Cellar/opencv/4.0.1/share/opencv4/lbpcascades/lbpcascade_frontalface.xml");
    CascadeClassifier eye_cascade;
    eye_cascade.load("/usr/local/Cellar/opencv/4.0.1/share/opencv4/haarcascades/haarcascade_eye.xml");
    
    createFaceDetectionMaskGenerator()
    while (1)
    {
        cap >> frame;
        
        vector<Rect> faces;
        vector<Rect> eyes;
        
        cvtColor(frame, gray, COLOR_BGR2GRAY);
//        equalizeHist(gray, gray);
        
        imshow("src", gray);
        
        face_cascade.detectMultiScale(gray, faces, 1.1, 2, 1, Size(), Size());
        eye_cascade.detectMultiScale(gray, eyes, 1.1, 2, 0, Size(), Size());
        
        for(auto i = 0; i < faces.size(); i++ )
        {
            rectangle(frame, faces[i], Scalar(0,0,255), 2, 8, 0);
        }
        
        for (auto i =0; i <eyes.size(); i++)
        {
            rectangle(frame, eyes[i], Scalar(0,255,0), 2, 8, 0);
        }
        
        namedWindow("result", WINDOW_AUTOSIZE);
        imshow("result", frame);
        
        waitKey(25);
        
        
    }
    
    return 0;
    
    
    
}
