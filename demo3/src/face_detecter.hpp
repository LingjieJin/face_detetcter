#ifndef face_detecter_hpp
#define face_detecter_hpp

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

using std::string;
using std::vector;

class TFaceDetecter
{
    public:
        cv::CascadeClassifier face_detecter;
    public:
        TFaceDetecter(string path);
        ~TFaceDetecter();
        vector<cv::Rect> detectFace(cv::Mat &img_gray);
};


#endif