/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include<math.h>
using namespace cv;
using namespace cv::face;
using namespace std;
static Mat norm_0_255(InputArray _src) {
	Mat src = _src.getMat();
	// Create and return normalized image:
	Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(Error::StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		cout << path << endl;
		getline(liness, classlabel);
		cout << classlabel << endl;
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}
int main(int argc, const char* argv[]) {
	// Check for valid command line arguments, print usage
	// if no arguments were given.
	if (argc < 2) {
		cout << "usage: " << argv[0] << " <csv.ext> <output_folder> " << endl;
		exit(1);
	}
	string output_folder = ".";
	if (argc == 3) {
		output_folder = string(argv[2]);
	}
	// Get the path to your CSV.
	string fn_csv = string(argv[1]);
	// These vectors hold the images and corresponding labels.
	vector<Mat> images;
	vector<int> labels;
	// Read in the data. This can fail if no valid
	// input filename is given.
	try {
		read_csv(fn_csv, images, labels);
		cout << "read csv file sucess" << endl;
	}
	catch (const cv::Exception & e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		// nothing more we can do
		exit(1);
	}
	// Quit if there are not enough images for this demo.
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(Error::StsError, error_message);
	}

	for (int i = 0; i < images.size(); i++)
	{
		if (images[i].size() != Size(92, 112))
		{
			cout << i << endl;
			cout << images[i].size() << endl;
		}
	}

	// Get the height from the first image. We'll need this
	// later in code to reshape the images to their original
	// size:
	int height = images[0].rows;
	// The following lines simply get the last images from
	// your dataset and remove it from the vector. This is
	// done, so that the training data (which we learn the
	// cv::BasicFaceRecognizer on) and the test data we test
	// the model with, do not overlap.
	Mat test = imread("D:\\Facerecognize\\test_face2.jpg",0);
	Mat test1 = test.clone();
	Mat testSample;
	resize(test1, testSample,Size(92,112));
	int testLabel = labels[350];
	cout << testLabel << endl;
	images.pop_back();
	labels.pop_back();
	// The following lines create an Eigenfaces model for
	// face recognition and train it with the images and
	// labels read from the given CSV file.
	// This here is a full PCA, if you just want to keep
	// 10 principal components (read Eigenfaces), then call
	// the factory method like this:
	//
	//      EigenFaceRecognizer::create(10);
	//
	// If you want to create a FaceRecognizer with a
	// confidence threshold (e.g. 123.0), call it with:
	//
	//      EigenFaceRecognizer::create(10, 123.0);
	//
	// If you want to use _all_ Eigenfaces and have a threshold,
	// then call the method like this:
	//
	//      EigenFaceRecognizer::create(0, 123.0);
	//
	Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
	//model->read("D:\\Facerecognize\\MyFacePCAModel.xml");
	model->train(images, labels);
	model->save("D:\\Facerecognize\\MyFacePCAModel2.xml");

	/*Ptr<BasicFaceRecognizer> model1 = FisherFaceRecognizer::create();
	model1->train(images, labels);
	model1->save("D:\\Facerecognize\\MyFaceFisherModel.xml");*/

	/*Ptr<LBPHFaceRecognizer> model2 = LBPHFaceRecognizer::create();
	model2->train(images, labels);
	model2->save("D:\\Facerecognize\\MyFaceLBPHModel.xml");*/
	// The following line predicts the label of a given
	// test image:
	int predictedLabel = model->predict(testSample);
	//int predictedLabel1 = model1->predict(testSample);
	//int predictedLabel2 = model2->predict(testSample);
	//
	// To get the confidence of a prediction call the model with:
	//
	//      int predictedLabel = -1;
	//      double confidence = 0.0;
	//      model->predict(testSample, predictedLabel, confidence);
	//
	cout << "1" << endl;
	string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
	//string result_message1 = format("Predicted class = %d / Actual class = %d.", predictedLabel1, testLabel);
	//string result_message2 = format("Predicted class = %d / Actual class = %d.", predictedLabel2, testLabel);
	cout << result_message << endl;
	//cout << result_message1 << endl;
	//cout << result_message2 << endl;
	// Here is how to get the eigenvalues of this Eigenfaces model:
	//Mat eigenvalues = model->getEigenValues();
	// And we can do the same to display the Eigenvectors (read Eigenfaces):
	//Mat W = model->getEigenVectors();
	// Get the sample mean from the training data
	//Mat mean = model->getMean();
	// Display or save:
	/*if (argc == 2) {
		imshow("mean", norm_0_255(mean.reshape(1, images[0].rows)));
	}
	else {
		imwrite(format("%s/mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, images[0].rows)));
	}*/
	// Display or save the Eigenfaces:
	/*for (int i = 0; i < min(10, W.cols); i++) {
		string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
		cout << msg << endl;
		// get eigenvector #i
		Mat ev = W.col(i).clone();
		// Reshape to original size & normalize to [0...255] for imshow.
		Mat grayscale = norm_0_255(ev.reshape(1, height));
		// Show the image & apply a Jet colormap for better sensing.
		Mat cgrayscale;
		applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
		// Display or save:
		if (argc == 2) {
			imshow(format("eigenface_%d", i), cgrayscale);
		}
		else {
			imwrite(format("%s/eigenface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
		}
	}*/
	// Display or save the image reconstruction at some predefined steps:
	/*for (int num_components = min(W.cols, 10); num_components < min(W.cols, 300); num_components += 15) {
		// slice the eigenvectors from the model
		Mat evs = Mat(W, Range::all(), Range(0, num_components));
		Mat projection = LDA::subspaceProject(evs, mean, images[0].reshape(1, 1));
		Mat reconstruction = LDA::subspaceReconstruct(evs, mean, projection);
		// Normalize the result:
		reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));
		// Display or save:
		if (argc == 2) {
			imshow(format("eigenface_reconstruction_%d", num_components), reconstruction);
		}
		else {
			imwrite(format("%s/eigenface_reconstruction_%d.png", output_folder.c_str(), num_components), reconstruction);
		}
	}*/
	// Display if we are not writing to an output folder:
	if (argc == 2) {
		waitKey(0);
	}
	return 0;
}