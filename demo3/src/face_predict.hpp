#ifndef face_predict_hpp
#define face_predict_hpp

#include <opencv2/opencv.hpp>

#include <opencv2/face.hpp>
#include <opencv2/face/facerec.hpp>

#include <string>

using std::string;

class TFacePredict
{
    public:
        cv::Ptr<cv::face::EigenFaceRecognizer> face_predict;
    public:
        TFacePredict();
        ~TFacePredict();
        TFacePredict(string path);
        //
        
};



#endif