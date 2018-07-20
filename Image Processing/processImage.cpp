#define pi 3.141592

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

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
	if (theta_out == 0)
		theta_out = 360;

	// calculate parameters of original image
	theta_out = theta_out * (pi / 180);
	theta_original = atan2(img.rows, img.cols);
	r = sqrt(pow(img.rows, 2) + pow(img.cols, 2));
	centX_original = img.cols / 2;
	centY_original = img.rows / 2;

    Mat C = (Mat_<double>(2,2) << cos(theta_out), -sin(theta_out), sin(theta_out), cos(theta_out));
	Mat A = (Mat_<double>(2,1) << 1, 1);
	cout << C*A << endl;

	// calculate parameters for output image
	if (task == 0)
	{
		width = img.cols;
		height = img.rows;
	}
	else if (task == 1)
	{
		if (theta_out < pi)
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
	Mat imOut(height, width, CV_8UC3);
	int x, y;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			p = sqrt(pow(j - centX_original, 2) + pow(i - centY_original, 2));
			phi = atan2(i - centY_original, j - centX_original);
			x = p * cos(phi + theta_out) + centX_out;
			y = p * sin(phi + theta_out) + centY_out;
			if (x >= 0 && y >= 0 && x < width && y < height)
			{
				imOut.at<Vec3b>(y, x) = img.at<Vec3b>(i, j);
			}
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (!norm(imOut.at<Vec3b>(i, j), CV_L2))
			{
				imOut.at<Vec3b>(i,j) = ((norm(imOut.at<Vec3b>(i,j-1)) > norm(imOut.at<Vec3b>(i,j+1))) ? imOut.at<Vec3b>(i,j-1) : imOut.at<Vec3b>(i,j+1));
			}
		}
	}

	return imOut;
}

int main(int argc, char **argv)
{
	Mat image, imOut;

	if (argc != 3)
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
	else if (task == "-re")
		imOut = rotation(image, 1); // rotate image, background extension
	else if (task == "-rf")
		imOut = rotation(image, 2); // rotate image, background fill

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", imOut);

	waitKey(0);
	return 0;
}
