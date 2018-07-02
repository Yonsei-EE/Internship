#include <cv.h>
#include <highgui.h>
int main()

{
        int i,j;
        CvPoint *pt2[2];
        IplImage *baduk;
        baduk = cvCreateImage(cvSize(880,880), IPL_DEPTH_8U, 3);
        cvSet(baduk, CV_RGB(255,255,255));
        cvRectangle(baduk, cvPoint(40,40),cvPoint(840,840),CV_RGB(0,0,0));
        for(i = 0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			cvRectangle(baduk, cvPoint(140+i*200,40+j*200), cvPoint(240+i*200,140+j*200),CV_RGB(0,0,0),-1,CV_AA);
			cvRectangle(baduk, cvPoint(i*200+40,140+j*200), cvPoint(140+i*200,240+j*200),CV_RGB(0,0,0),-1,CV_AA);
		}
	}
        cvNamedWindow("Drawing Baduk board",CV_WINDOW_AUTOSIZE);
        cvShowImage("Drawing Baduk board",baduk);
        cvWaitKey(0);

        cvDestroyAllWindows();
        cvReleaseImage(&baduk);
        return 0;
}
