#include "face_detecter.hpp"

#include <time.h>


TFaceDetecter::TFaceDetecter(string path)
{
    face_detecter = cv::CascadeClassifier(path);
}

TFaceDetecter::~TFaceDetecter()
{
    
}


vector<cv::Rect> TFaceDetecter::detectFace(cv::Mat &img_gray)
{
    double t =0;
    vector<cv::Rect> rects;
    t = (double)cv::getTickCount();
    //
    /*
    CASCADE_DO_CANNY_PRUNING=1, 利用canny边缘检测来排除一些边缘很少或者很多的图像区域
    CASCADE_SCALE_IMAGE=2, 正常比例检测
    CASCADE_FIND_BIGGEST_OBJECT=4, 只检测最大的物体
    CASCADE_DO_ROUGH_SEARCH=8 初略的检测
    */
    face_detecter.detectMultiScale(img_gray, rects, 1.1, 2, \
                                    0 | cv::CASCADE_DO_CANNY_PRUNING | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT | cv::CASCADE_DO_ROUGH_SEARCH, \
                                    cv::Size(100,100), cv::Size(500,500));


    printf( "detection time = %f ms\n", ((double)cv::getTickCount() - t)/cv::getTickFrequency() * 1000);

    return rects;
}
