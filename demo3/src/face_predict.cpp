#include "face_predict.hpp"

TFacePredict::TFacePredict()
{
    face_predict = cv::face::EigenFaceRecognizer::create();
}

TFacePredict::~TFacePredict()
{

}

TFacePredict::TFacePredict(string path)
{
    face_predict = cv::face::EigenFaceRecognizer::create();
    face_predict->read(path);
}