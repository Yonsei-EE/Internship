#define pi 3.141592

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace cv;
using namespace std;

Mat scale(Mat img, int width, int height) {
	Mat imOut(height, width, CV_8UC3);
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			imOut.at<Vec3b>(i,j) = img.at<Vec3b>(i*img.rows/height,j*img.cols/width);
		}
	}

	return imOut;
}

Mat rotation(Mat img, int task)
{
	int width, height, centX_original, centY_original, centX_out, centY_out;
	double theta_original, theta_out = -1, r, p, phi;

	// receive rotation angle
	while (theta_out < 0 || theta_out > 360)
	{
		cout << "Enter rotation angle in degrees." << endl;
		cin >> theta_out;
	}

	// calculate parameters of original image
	theta_out = theta_out * (pi / 180);
	theta_original = atan2(img.rows, img.cols);
	r = sqrt(pow(img.rows, 2) + pow(img.cols, 2));
	centX_original = img.cols / 2;
	centY_original = img.rows / 2;

	// rotation matrix
    	Mat R = (Mat_<double>(2,2) << cos(theta_out), -sin(theta_out), sin(theta_out), cos(theta_out));

	// calculate parameters for output image
	if (task < 2)
	{
		width = img.cols;
		height = img.rows;
	}
	else
	{
		if (theta_out < pi/2 || theta_out > pi && theta_out < pi*3/2)
		{
			width = abs(r * cos(theta_original - theta_out));
			height = abs(r * sin(theta_original + theta_out));
		}
		else
		{
			width = abs(r * cos(theta_original + theta_out));
			height = abs(r * sin(theta_original - theta_out));
		}
	}
	centX_out = width / 2;
	centY_out = height / 2;

	// create output image
	Mat imOut(height, width, CV_8UC3, Scalar(0,0,0));
	if(task == 1)
		imOut = img.clone();
	Mat P;
	int x, y;
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			P = (Mat_<double>(2,1) << j-centX_out, i-centY_out);
			P = R*P;
			x = P.at<double>(0,0)+centX_original;
			y = P.at<double>(1,0)+centY_original;
			if(x>=0 && y>=0 && x<=img.cols && y<=img.rows)
				imOut.at<Vec3b>(i,j) = img.at<Vec3b>(y,x);
		}
	}

	return imOut;
}

Mat diamond(Mat img) {
	int width=-1;
	Mat imOut(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));
	for(int i=0; i<img.rows; i++) {
		if(i<img.rows/2)
			width = width + 2;
		else
			width = width - 2;
		for(int j=0; j<width; j++) {
			imOut.at<Vec3b>(i,j+(img.cols-width)/2) = img.at<Vec3b>(i,j*img.cols/width);
		}
	}

	return imOut;
}

Mat Circle(Mat img) {
	int width;
	int r = min(img.rows/2, img.cols/2);
	Mat imOut(r*2, r*2, CV_8UC3, Scalar(0,0,0));
	for(int i=0; i<r*2; i++) {
		width = 2*sqrt(pow(r,2)-pow(r-i,2));
		for(int j=0; j<width; j++) {
			imOut.at<Vec3b>(i,j+(img.cols-width)/2) = img.at<Vec3b>(i*img.rows/r/2,j*img.cols/width);
		}
	}

	return imOut;
}

int main(int argc, char **argv)
{
	Mat image, imOut;

	if (argc < 3)
	{
		cout << "Please enter two arguements for your image." << endl;
		return -1;
	}

	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	string task = argv[2];
	if (task == "-r")
		imOut = rotation(image, 0); // rotate image, crop overflow
	else if (task == "-rf")
		imOut = rotation(image, 1); // rotate image, background extension
	else if (task == "-re")
		imOut = rotation(image, 2); // rotate image, background fill
	else if (task == "-s")
		imOut = scale(image, atoi(argv[3]), atoi(argv[4]));
	else if (task == "-d")
		imOut = diamond(image);
	else if (task == "-c")
		imOut = Circle(image);

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", imOut);

	waitKey(0);
	return 0;
}
