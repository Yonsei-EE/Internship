#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int main() 
{
    int w, h;
    int i, j;
	double s;
    int w1=512,h1=512;

    int x=0,y=0;
    double theta, theta1;
    double r;

	Mat img1 = imread("baboon.jpg", 1);
	w = img1.rows;
	h = img1.cols;

    Mat img(h, w, CV_8UC3);
	
	scanf("%lf", &theta);

	for(i=0; i<h1; i++)
	{
		for(j=0; j<w1; j++)
		{
		    r=sqrt(pow(i-256,2) + pow(256-j,2));

		    theta1 = atan2(256-i ,256-j)+3.141592;
                    
                    s = theta * (3.141592 / 180 );

		    x=cos(theta1-s)*r;
		    y=sin(theta1-s)*r;

		    if( (x+256 > 0 && x+256 < h1) && (y+256 > 0 && y+256 < w1) )
		    {
		    	img.at<Vec3b>(x+256,y+256)[0] = img1.at<Vec3b>(j,i)[0];
		    	img.at<Vec3b>(x+256,y+256)[1] = img1.at<Vec3b>(j,i)[1];
		    	img.at<Vec3b>(x+256,y+256)[2] = img1.at<Vec3b>(j,i)[2];
		    }
		    /*else
		    {
			img.at<Vec3b>(j,i)[0] = 0;
		    	img.at<Vec3b>(j,i)[1] = 0;
		    	img.at<Vec3b>(j,i)[2] = 0;
    	    	    }*/
		}
    }

	for(i=0; i<h1; i++)
	{
		for(j=0; j<w1; j++)
		{
			img1.at<Vec3b>(j,i)[0] = img.at<Vec3b>(j,i)[0];
			img1.at<Vec3b>(j,i)[1] = img.at<Vec3b>(j,i)[1];
			img1.at<Vec3b>(j,i)[2] = img.at<Vec3b>(j,i)[2];
		    
		}
	}
        

	imshow("copy", img1);
	imshow("paste", img);	
	imwrite("paste.jpg", img);
	waitKey(0);

	return 0;





	// 반지름을 구한다음 내가 돌리고 싶은 각도를 계산한다 그리고 
	// 내가 돌리고자 하는 위치의 좌표를 cos, sin을 이용하면 구할수 있다. 그때의 각은 오리지널의 각도에서 내가 돌리고자하는 각도의 차이이다.











}
