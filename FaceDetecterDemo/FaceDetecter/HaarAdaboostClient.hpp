//
//  HaarAdaboostClient.hpp
//  FaceDetecter
//
//  Created by kIng_martin on 2019/10/8.
//  Copyright © 2019 Jin Lingjie. All rights reserved.
//

#ifndef HaarAdaboostClient_hpp
#define HaarAdaboostClient_hpp

#include <stdio.h>

#include <opencv2/opencv.hpp>

#include <string>

#define OPENCV_FRONTALFACE_ALT  "haarcascade_frontalface_alt.xml"
#define OPENCV_FRONTALFACE_ALT_2    "haarcascade_frontalface_alt2.xml"
#define OPENCV_EYE  "haarcascade_eye.xml"


class HaarAdaboostClient
{
public:
    cv::CascadeClassifier cascade;
public:
    HaarAdaboostClient();
    HaarAdaboostClient(string path);
    ~HaarAdaboostClient();
    
};

#endif /* HaarAdaboostClient_hpp */
