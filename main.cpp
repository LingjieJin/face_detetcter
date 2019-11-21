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

			//��ֻ��һ������ʱ����ʼ����


			imshow("result", frame1);

			//�������
			printf("width = %.2f\n", capture.get(CAP_PROP_FRAME_WIDTH));
			printf("height = %.2f\n", capture.get(CAP_PROP_FRAME_HEIGHT));
			printf("fbs = %.2f\n", capture.get(CAP_PROP_FPS));
			printf("brightness = %.2f\n", capture.get(CAP_PROP_BRIGHTNESS));
			printf("contrast = %.2f\n", capture.get(CAP_PROP_CONTRAST));
			printf("saturation = %.2f\n", capture.get(CAP_PROP_SATURATION));
			printf("hue = %.2f\n", capture.get(CAP_PROP_HUE));
			printf("exposure = %.2f\n", capture.get(CAP_PROP_EXPOSURE));
			//��������
			//esc\'q'\'Q'���˳�����
			//'c'������
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
						cv::Point(cvRound((faces[0].x + faces[0].width) * scale), cvRound((faces[0].y + faces[0].height) * scale))));//�ڻҶ�ͼ�н�Ȧ��������������ü���
					cout << faces[0].x << endl;//������face[0].x
					resize(faceROI, myFace, Size(92, 112));//����Ȥ��sizeΪ92*112
					putText(frame, to_string(pic_num), faces[0].tl(), 3, 1.2, (0, 0, 225), 2, 0);//�� faces[0].tl()�����Ͻ�����д���
					string filename = format("%d.jpg", pic_num); //����ڵ�ǰ��Ŀ�ļ�����1-10.jpg ������format����תΪ�ַ���
					imwrite(filename, norm_0_255(myFace));//���ڵ�ǰĿ¼��
					imshow(filename, norm_0_255(myFace));//��ʾ��size�����
					waitKey(500);//�ȴ�500us
					destroyWindow(filename);//:����ָ���Ĵ���
					pic_num++;//��ż�1
				}
			}
		}
	}

	return 0;
}