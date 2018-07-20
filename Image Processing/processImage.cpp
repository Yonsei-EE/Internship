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
	if(task ==0 || task > 3) {
		theta_out = task;
	}
	else {
		while (theta_out < 0 || theta_out > 360)
		{
			cout << "Enter rotation angle in degrees." << endl;
			cin >> theta_out;
		}
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
	if (task < 3)
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
	if(task == 2)
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

Mat addRotation(Mat img, Mat imOut, double theta_out, int pos_x, int pos_y)
{
	int width, height, centX_original, centY_original, centX_out, centY_out;
	double theta_original, r, p, phi;

	// calculate parameters of original image
	theta_out = theta_out * (pi / 180);
	theta_original = atan2(img.rows, img.cols);
	r = sqrt(pow(img.rows, 2) + pow(img.cols, 2));
	centX_original = img.cols / 2;
	centY_original = img.rows / 2;

	// rotation matrix
    	Mat R = (Mat_<double>(2,2) << cos(theta_out), -sin(theta_out), sin(theta_out), cos(theta_out));

	// calculate parameters for output image
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
	
	centX_out = width / 2;
	centY_out = height / 2;

	// create output image
	Mat P;
	int x, y;
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			P = (Mat_<double>(2,1) << j-centX_out, i-centY_out);
			P = R*P;
			x = P.at<double>(0,0)+centX_original;
			y = P.at<double>(1,0)+centY_original;
			if(x>=0 && y>=0 && x<=img.cols && y<=img.rows)
				imOut.at<Vec3b>(i+pos_y,j+pos_x) = img.at<Vec3b>(y,x);
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

Mat Final(Mat img) {
	int i, j, width = 800, height = 800;
	Mat imOut(height, width, CV_8UC3, Scalar(0,0,0));
	Mat temp = scale(img, 100, 100);
	for(i=3; i<5; i++) {
		for(j=3; j<5; j++) {
			temp.copyTo(imOut(Rect(100*i,100*j,100,100)));
		}
	}
	for(i=0;i<3;i++) {
		temp.copyTo(imOut(Rect(350,100*i,100,100)));
		for(j=0;j<3;j++) {
			addRotation(temp, imOut, 45, (300-350/sqrt(2))+(100/sqrt(2))*j, (300-350/sqrt(2))+(100/sqrt(2))*j);
			addRotation(temp, imOut, 135, (300-350/sqrt(2))+(100/sqrt(2))*j, (500+150/sqrt(2))-(100/sqrt(2))*j);
			addRotation(temp, imOut, 225, (500+150/sqrt(2))-(100/sqrt(2))*j, (500+150/sqrt(2))-(100/sqrt(2))*j);
			addRotation(temp, imOut, 315, (500+150/sqrt(2))-(100/sqrt(2))*j, (300-350/sqrt(2))+(100/sqrt(2))*j);
		}
	}
	temp = rotation(img,90);
	temp = scale(temp, 100, 100);
	for(i=0;i<3;i++)
		temp.copyTo(imOut(Rect(100*i,350,100,100)));
	temp = rotation(img,180);
	temp = scale(temp, 100, 100);
	for(i=0;i<3;i++)
		temp.copyTo(imOut(Rect(350,100*i+500,100,100)));
	temp = rotation(img,270);
	temp = scale(temp, 100, 100);
	for(i=0;i<3;i++)
		temp.copyTo(imOut(Rect(100*i+500,350,100,100)));

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
		imOut = rotation(image, 1); // rotate image, crop overflow
	else if (task == "-rf")
		imOut = rotation(image, 2); // rotate image, background extension
	else if (task == "-re")
		imOut = rotation(image, 3); // rotate image, background fill
	else if (task == "-rs")
		imOut = rotation(image, atoi(argv[3])); // rotate image, initial theta
	else if (task == "-s")
		imOut = scale(image, atoi(argv[3]), atoi(argv[4]));
	else if (task == "-d")
		imOut = diamond(image);
	else if (task == "-c")
		imOut = Circle(image);
	else if (task == "-f")
		imOut = Final(image);

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", imOut);

	waitKey(0);
	return 0;
}
