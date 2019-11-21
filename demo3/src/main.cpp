#include "face_detecter.hpp"
#include "face_predict.hpp"


#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

#define HAAR_PATH "./face_detect.xml"
#define FACE_PREDICT_PATH "./face_predict.xml"

#define TRAIN_DATA_SIZE cv::Size()

#define DETECT_AREA cv::Rect(200,100,624,568)

#define FRAME_WIDTH 1280
#define FRAME_HEIGHT 600

int main(int argc, const char** argv)
{
    string haarcascadePath;
    string facepredictPath;

    if(argc == 1)
    {
        // 默认打开
        haarcascadePath = HAAR_PATH;
        facepredictPath = FACE_PREDICT_PATH;
    }
    else if(argc == 3)
    {
        // 带参数打开
        haarcascadePath = argv[1];
        facepredictPath = argv[2];
    }
    else
    {
        printf("Error Input.\nExample: ./main or \n ./main ./face_detect.xml ./face_predict.xml");
        return 0;
    }

    TFaceDetecter facedetecter(haarcascadePath);
    // TFacePredict facepredict(facepredictPath);

    cv::VideoCapture cap ;
    cap.open(0);
    // cap.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	// cap.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    // cap.set(cv::CAP_PROP_FPS, 25);


    cv::Mat frame;

    cv::namedWindow("src", cv::WINDOW_NORMAL);
    cv::moveWindow("src", 0, 0);
    cv::setWindowProperty("src", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    int frame_count = 0;

    vector<cv::Rect> rects;

    while (1)
    {
        if(!cap.isOpened())
        {
            printf("Capture didn't work.\n");
            return 0;
        }
        /*
        视频流输出
        */
        cap.read(frame);
        frame_count ++;

        if(frame.empty())
        {
            printf("empty image.\n");
            continue;
        }

        if (frame_count != 30)
        {
            if(rects.size() > 0)
            {
                for (size_t i = 0; i < rects.size(); i++)
                {
                    // 绘制红色方框
                    cv::rectangle(frame, rects[i], cv::Scalar(0,0,255), 1, 8);
                    
                }
            }

            std::ostringstream ostr;
            double fps = cap.get(cv::CAP_PROP_FPS);
            ostr << "cap width " << frame.cols << "height " << frame.rows << "FPS " << fps << std::endl;
            cv::String testinfo = ostr.str();
            cv::putText(frame, testinfo, cv::Point(10, 50), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,0), 1, 8, 0);

            // cv::rectangle(frame, DETECT_AREA, cv::Scalar(0, 0, 0), 2, 8);
            cv::imshow("src", frame);

            char c =(char) cv::waitKey(40);
            if (c == 27 || c == 'q' || c == 'Q')
            {
                break;
            }
            continue;
        }


        /*
        人脸识别部分
        */
        frame_count = 0;

        // cv::Mat samllImage;
        // samllImage = frame(DETECT_AREA);

        cv::Mat src_gray;
        
        cv::cvtColor(frame, src_gray, cv::COLOR_BGR2GRAY);
        
        // 暂定是否需要优化
        cv::equalizeHist(src_gray, src_gray); // 比较耗时

        rects = facedetecter.detectFace(src_gray);

        if (rects.size() <= 0)
        {
            // 没有检测到人脸
            continue;
        }
        
        for (size_t i = 0; i < rects.size(); i++)
        {
            // 绘制红色方框
            cv::rectangle(frame, rects[i], cv::Scalar(0,0,255), 1, 8);

            // cv::Mat roi = src_gray(rects[i]);
            // // 人脸识别时候 需要把图像变成训练图像大小
            // cv::resize(roi, roi, TRAIN_DATA_SIZE);
            // int result = facepredict.face_predict->predict(roi);

            // if (result)
            // {
            //     // 如果检测到有该人 绘制绿色框
            //     cv::rectangle(samllImage, rects[i], cv::Scalar(0,255,0), 1, 8);
            // }

            std::ostringstream ostr;
            double fps = cap.get(cv::CAP_PROP_FPS);
            ostr << "cap width " << frame.cols << "height " << frame.rows << "FPS " << fps << std::endl;
            cv::String testinfo = ostr.str();
            cv::putText(frame, testinfo, cv::Point(10, 50), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,0), 1, 8, 0);
        }

        // cv::rectangle(frame, DETECT_AREA, cv::Scalar(0, 0, 0), 2, 8);
        cv::imshow("src", frame);

    }
    
    // end
    cap.release();
    cv::destroyAllWindows();

    return 0;
}