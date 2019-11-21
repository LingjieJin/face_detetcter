#include"face_detect.h"
#include<iostream>

string cascadeName;
int main(int argc, const char** argv)
{
	VideoCapture capture;
	Mat frame, image, myFace;
	int pic_num = 1;
	string inputName;
	bool tryflip;
	CascadeClassifier cascade, nestedCascade;
	double scale;
	cv::CommandLineParser parser(argc, argv,
		"{help h||}"
		"{cascade|D:\\opencv-411\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml|}"
		"{scale|2|}{try-flip||}{@filename||}"
	);

	if (parser.has("help"))
	{
		help();
		return 0;
	}
	cascadeName = parser.get<string>("cascade");
	scale = parser.get<double>("scale");
	if (scale < 1)
		scale = 1;
	tryflip = parser.has("try-flip");
	inputName = parser.get<string>("@filename");
	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}

	if (!cascade.load(cascadeName))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		help();
		return -1;
	}
	if (inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1))
	{
		int camera = inputName.empty() ? 0 : inputName[0] - '0';
		if (!capture.open(camera))
			cout << "Capture from camera #" << camera << " didn't work" << endl;
	}
	else if (inputName.size())
	{
		image = imread(inputName, 1);
		if (image.empty())
		{
			if (!capture.open(inputName))
				cout << "Could not read " << inputName << endl;
		}
	}

	if (capture.isOpened())
	{
		cout << "Video capturing has been started ..." << endl;
		for (;;)
		{
			capture >> frame;
			if (frame.empty())
				break;
			Mat frame1 = frame.clone();
			double t = 0;
			vector<Rect> faces = detect_face(frame1,cascade,scale,tryflip);

			//当只有一个人脸时，开始拍照


			imshow("result", frame1);

			//相机参数
			printf("width = %.2f\n", capture.get(CAP_PROP_FRAME_WIDTH));
			printf("height = %.2f\n", capture.get(CAP_PROP_FRAME_HEIGHT));
			printf("fbs = %.2f\n", capture.get(CAP_PROP_FPS));
			printf("brightness = %.2f\n", capture.get(CAP_PROP_BRIGHTNESS));
			printf("contrast = %.2f\n", capture.get(CAP_PROP_CONTRAST));
			printf("saturation = %.2f\n", capture.get(CAP_PROP_SATURATION));
			printf("hue = %.2f\n", capture.get(CAP_PROP_HUE));
			printf("exposure = %.2f\n", capture.get(CAP_PROP_EXPOSURE));
			//按键操作
			//esc\'q'\'Q'：退出程序
			//'c'：拍照
			char c = (char)waitKey(10);
			if (c == 27 || c == 'q' || c == 'Q')
				break;
			if (c == 'c') {
				if (faces.size() == 1)
				{

					Mat frame_src = frame.clone();
					Mat frame_gray;
					cvtColor(frame_src, frame_gray, COLOR_BGR2GRAY);
					Mat faceROI = frame_gray(Rect(cv::Point(cvRound(faces[0].x * scale), cvRound(faces[0].y * scale)),
						cv::Point(cvRound((faces[0].x + faces[0].width) * scale), cvRound((faces[0].y + faces[0].height) * scale))));//在灰度图中将圈出的脸所在区域裁剪出
					cout << faces[0].x << endl;//测试下face[0].x
					resize(faceROI, myFace, Size(92, 112));//将兴趣域size为92*112
					putText(frame, to_string(pic_num), faces[0].tl(), 3, 1.2, (0, 0, 225), 2, 0);//在 faces[0].tl()的左上角上面写序号
					string filename = format("%d.jpg", pic_num); //存放在当前项目文件夹以1-10.jpg 命名，format就是转为字符串
					imwrite(filename, norm_0_255(myFace));//存在当前目录下
					imshow(filename, norm_0_255(myFace));//显示下size后的脸
					waitKey(500);//等待500us
					destroyWindow(filename);//:销毁指定的窗口
					pic_num++;//序号加1
				}
			}
		}
	}

	return 0;
}