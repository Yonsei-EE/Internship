#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>
#include <math.h>

using namespace cv;

int get_h(int r, int g, int b);
int get_s(int r, int g, int b);
int get_v(int r, int g, int b);
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );
void on_trackbar( int _val);
int main()
{
	int i,j,k;
	float r,g,b,h,s,v;
	Mat image=imread("colorbar.png");
//	Mat image=imread("baboon.png");
	Mat image2, image4, image5;
//	int he = 512;
//	int wi = 512;
	int he = 161;
	int wi = 743;
 	namedWindow("Image2",1);
     	int Hmin = 0;
	createTrackbar("Hmin", "Image2", &Hmin, 360);
     	int Smin = 0;
	createTrackbar("Smin", "Image2", &Smin, 255);
     	int Vmin = 0;
	createTrackbar("Vmin", "Image2", &Vmin, 255);
     	int Hmax = 360;
	createTrackbar("Hmax", "Image2", &Hmax, 360);
     	int Smax = 255;
	createTrackbar("Smax", "Image2", &Smax, 255);
     	int Vmax = 255;
	createTrackbar("Vmax", "Image2", &Vmax, 255);
	int rth[he][wi][3];
	Mat image3(he,wi, CV_8UC3);
	while(true)
	{
		for(i = 0; i<image.size().width; i++)
		{
			for(j = 0; j<image.size().height; j++)
			{
				RGBtoHSV((float)image.at<Vec3b>(j,i)[0], (float)image.at<Vec3b>(j,i)[1], (float)image.at<Vec3b>(j,i)[2], &h, &s, &v);
				if(h > Hmax)
				{
					h = 0;
					s = 0;
					v = 0;
				}
				if(s > Smax)
				{
					h = 0;
					s = 0;
					v = 0;
				}
				if(v > Vmax)
				{
					h = 0;
					s = 0;
					v = 0;
				}
				if(h < Hmin)
				{
					h = 0;
					s = 0;
					v = 0;
				}
				if(s < Smin)
				{
					h = 0;
					s = 0;
					v = 0;
				}
				if(v < Vmin)
				{
					h = 0;
					s = 0;
					v = 0;
				}
				h = h/2;
				s = s*255;
				v = v;
				
				rth[j][i][0] = (int)round(h);
				rth[j][i][1] = (int)round(s);
				rth[j][i][2] = (int)round(v);
			}		
		}
		for(i = 0; i<wi; i++)
		{
			for(j = 0; j<he; j++)	

			{
				HSVtoRGB(&r, &g, &b, (float)rth[j][i][0]*2, (float)rth[j][i][1]/255, (float)rth[j][i][2]);
				image3.at<Vec3b>(j,i)[0] = (int)round(r);
				image3.at<Vec3b>(j,i)[1] = (int)round(g);
				image3.at<Vec3b>(j,i)[2] = (int)round(b);
			}
		}
      		int iKey = waitKey(50);
          	if (iKey == 27)
          	{
               		break;
          	}
 		imshow("Image2", image3);
	}

}
/*
void on_trackbar( int _val)
{
	int rth[512][512][3];
	Mat image3(512,512, CV_8UC3);

	int i,j;
	float h,s,v,r,g,b;
	Mat image=imread("baboon.png");
	for(i = 0; i<image.size().width; i++)
	{
		for(j = 0; j<image.size().height; j++)
		{
			RGBtoHSV((float)image.at<Vec3b>(j,i)[0], (float)image.at<Vec3b>(j,i)[1], (float)image.at<Vec3b>(j,i)[2], &h, &s, &v);
			rth[j][i][0] = round(h/2);
			rth[j][i][1] = round(s*255);
			rth[j][i][2] = round(v);	
		}	
	}
	for(i = 0; i<512; i++)
	{
		for(j = 0; j<512; j++)

		{
			HSVtoRGB(&r, &g, &b, (float)rth[j][i][0]*2, (float)rth[j][i][1]/255, (float)rth[j][i][2]);
			image3.at<Vec3b>(j,i)[0] = (int)round(r);
			image3.at<Vec3b>(j,i)[1] = (int)round(g);
			image3.at<Vec3b>(j,i)[2] = (int)round(b);
		}
	}

 imshow( "Image2", image3 );
}
*/
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;

	if(r>=g && r>= b)
	{
		max = r;
		if(g>=b)
		{
			min = b;
		}
		else
		{
			min = g;
		}
	}
	else if(g>=b && g> r)
	{
		max = g;
		if(r>=b)
		{
			min = b;
		}
		else
		{
			min = r;
		}
	}
	else
	{
		max = b;
		if(r>g)
		{
			min = g;
		}
		else
		{
			min = r;
		}
	}
	*v = max;				// v

	delta = max - min;

	if( max != 0 )
		{
			*s = delta / max;		// s
		}
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if( r == max )
	{
		*h = ( g - b ) / delta;		// between yellow & magenta
	}
	else if( g == max )
	{
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	}
	else
	{
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan
	}
	*h *= 60;				// degrees
	if( *h < 0 )
	{
		*h += 360;
	}
}

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}

}
