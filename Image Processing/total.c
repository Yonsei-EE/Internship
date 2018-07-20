#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <math.h>
using namespace std;
using namespace cv;
Mat circle(Mat inmat);
Mat srm(Mat inmat);
int stackcheck(float arr[],float angle, int cnt);
Mat spacecheck(int* cnt,Mat board,Mat check,int x, int y);
Mat resinrota(Mat inmat, float angle,float rate);
Mat resize(Mat inmat, float rate);
Mat rotation(Mat inmat);
Mat rota(Mat inmat);
Mat ovw(Mat inmat);
Mat stitch(Mat inmat);
Mat Rhom(Mat inmat);
Mat Dia(Mat inmat);
int interpolation(Mat inmat,float inputx,float inputy, int rgb);
int main()
{
 Mat image, image2;
 int num;
 float angle,rate;
 float pi = 3.141592;
 image =imread("baboon.jpg");
 printf("- Choose the Image process -\n");
 printf("1. Image Rotation\n");
 printf("2. Overwrite\n");
 printf("3. Resize\n");
 printf("4. Stitching\n");
 printf("5. Resize + Rotation\n");
 printf("6. Rotation with Background extension\n");
 printf("7. Shape transform to Diamond\n");
 printf("8. Shape transform to Rhombus\n");
 printf("9. Shape transform to Circle\n");
 printf("10. Plot with in a round\n");
 printf("Your choice : ");
 scanf("%d",&num);
 switch(num)
{
 case 1 :
 image2 = rota(image);
 break;
 case 2 :
 image2 = ovw(image);
 break;
 case 3 :
 printf("rate : ");
 scanf("%f", &rate);
 image2 = resize(image,rate);
 break;
 case 4 :
 image2 = stitch(image);
 break;
 case 5 :
  printf("angle : ");
  scanf("%f",&angle);
  printf("rate : ");
  scanf("%f", &rate);
 image2 = resinrota(image,angle,rate);
 break;
 case 6 :
 image2 = rotation(image);
 break;
 case 7 :
 image2 = Dia(image);
 break;
 case 8 :
 image2 = Rhom(image);
 break;
 case 9 :
 image2 = circle(image);
 break;
 case 10 :
 image2 = srm(image);
 break;
}
 namedWindow("Image",1);
 imshow("Image", image2);
imwrite("kk.jpg", image2);
 waitKey(0);
 return 0;
}
Mat circle(Mat inmat)
{
int x0,y0,i,j,k;
unsigned int sumru,sumgu,sumbu,sumrd,sumgd,sumbd,xlong;
x0 = inmat.size().width/2;
y0 = inmat.size().height/2;
Mat outmat(inmat.size().height, inmat.size().width, CV_8UC3);
float theta;
for(i = 0; i<inmat.size().height/2+1;i++)
{
 if(i!=y0)
 {
  theta = acos(((float)y0-(float)i)/(float)y0);
  xlong = y0*sin(theta)*2+1;
  for(j = 0; j< xlong; j++)
  {
   sumru = 0,sumgu = 0,sumbu = 0, sumrd = 0 , sumgd = 0, sumbd = 0;
   for(k = 0; k< inmat.size().width/xlong;k++)
   {
    sumru += inmat.at<Vec3b>(i,j*inmat.size().width/xlong+k)[0];
    sumgu += inmat.at<Vec3b>(i,j*inmat.size().width/xlong+k)[1];
    sumbu += inmat.at<Vec3b>(i,j*inmat.size().width/xlong+k)[2];
    sumrd += inmat.at<Vec3b>(inmat.size().height-i,j*inmat.size().width/xlong+k)[0];
    sumgd += inmat.at<Vec3b>(inmat.size().height-i,j*inmat.size().width/xlong+k)[1];
    sumbd += inmat.at<Vec3b>(inmat.size().height-i,j*inmat.size().width/xlong+k)[2];
   }
   outmat.at<Vec3b>(i,x0-round(xlong/2)+j+1)[0] = sumru/(inmat.size().width/xlong);
   outmat.at<Vec3b>(i,x0-round(xlong/2)+j+1)[1] = sumgu/(inmat.size().width/xlong);
   outmat.at<Vec3b>(i,x0-round(xlong/2)+j+1)[2] = sumbu/(inmat.size().width/xlong);
   outmat.at<Vec3b>(inmat.size().height-i,x0-round(xlong/2)+j+1)[0] = sumrd/(inmat.size().width/xlong);
   outmat.at<Vec3b>(inmat.size().height-i,x0-round(xlong/2)+j+1)[1] = sumgd/(inmat.size().width/xlong);
   outmat.at<Vec3b>(inmat.size().height-i,x0-round(xlong/2)+j+1)[2] = sumbd/(inmat.size().width/xlong);
   //printf("(%d,%f)\n",i,x0-round(xlong/2)+j+1);
  }
 }
 else
 {
  for(k = 0; k<inmat.size().width;k++)
  {
   outmat.at<Vec3b>(i,k)[0] = inmat.at<Vec3b>(i,k)[0];
   outmat.at<Vec3b>(i,k)[1] = inmat.at<Vec3b>(i,k)[1];
   outmat.at<Vec3b>(i,k)[2] = inmat.at<Vec3b>(i,k)[2];
   
  }
 }
}
return outmat;
}
Mat srm(Mat inmat)
{
float stack[200];
int cnt,stcnt;
int num;
int x,y,i,j,k,l,cx,cy;
bool flag = true;
float pi = 3.141592;
float sizerate = 0.125;
float dangle = 90;
float anglepoint = 0, radiangle;
float r0,r;
stcnt = 0;
printf("Diameter parameter(image num) : ");
scanf("%d",&num);
r0 = sqrt(pow(inmat.size().width*0.125/2,2)+pow(inmat.size().height*0.125/2*num,2));
    printf("%f\n",r0);
    //printf("%f",inmat.size().width*0.125/2);

Mat outmat(r0*2, r0*2, CV_8UC3);
Mat resizemat = resize(inmat,0.125);
cy = outmat.size().height/2+1;
cx = outmat.size().width/2+1;

printf("(%d , %d)\n\n",outmat.size().height,outmat.size().width);
//printf("(%d , %d)\n",outmat.size().height/2+1,outmat.size().width/2+1);

for(i = 0; i<outmat.size().width; i++)
{
 for(j = 0; j<outmat.size().height; j++)
 {
  outmat.at<Vec3b>(j,i)[0] = 0;
  outmat.at<Vec3b>(j,i)[1] = 0;
  outmat.at<Vec3b>(j,i)[2] = 0;
 }
}
printf("(%d , %d)\n\n",outmat.size().width,outmat.size().height);

if(num%2 == 1||num==2)
{
 for(k=0;k<resizemat.size().width;k++)
 {
  for(l = 0;l<resizemat.size().height;l++)
  {
   outmat.at<Vec3b>(cy-resizemat.size().height/2 + k,cx-resizemat.size().width/2 + l)[0] = resizemat.at<Vec3b>(k,l)[0];
   outmat.at<Vec3b>(cy-resizemat.size().height/2 + k,cx-resizemat.size().width/2 + l)[1] = resizemat.at<Vec3b>(k,l)[1];
   outmat.at<Vec3b>(cy-resizemat.size().height/2 + k,cx-resizemat.size().width/2 + l)[2] = resizemat.at<Vec3b>(k,l)[2];
    //printf("(%d , %d)",cx-resizemat.size().width/2 + l,cy-resizemat.size().height/2 + k);
  }
 }
}
else
{
 for(k=0;k<resizemat.size().width;k++)
 {
  for(l = 0;l<resizemat.size().height;l++)
  {
   outmat.at<Vec3b>(cy-resizemat.size().height + k,cx-resizemat.size().width + l)[0] = resizemat.at<Vec3b>(k,l)[0];
   outmat.at<Vec3b>(cy-resizemat.size().height + k,cx-resizemat.size().width + l)[1] = resizemat.at<Vec3b>(k,l)[1];
   outmat.at<Vec3b>(cy-resizemat.size().height + k,cx-resizemat.size().width + l)[2] = resizemat.at<Vec3b>(k,l)[2];
    //printf("(%d , %d)",cy-resizemat.size().height + k,cx-resizemat.size().width + l);
    //printf("(%d , %d)",resizemat.size().width,resizemat.size().height);
  }
 }
 for(k=0;k<resizemat.size().width;k++)
 {
  for(l = 0;l<resizemat.size().height;l++)
  {
   outmat.at<Vec3b>(cy-resizemat.size().height + k,cx+l)[0] = resizemat.at<Vec3b>(k,l)[0];
   outmat.at<Vec3b>(cy-resizemat.size().height + k,cx+l)[1] = resizemat.at<Vec3b>(k,l)[1];
   outmat.at<Vec3b>(cy-resizemat.size().height + k,cx+l)[2] = resizemat.at<Vec3b>(k,l)[2];
    //printf("(%d , %d)",cx+l,cy-resizemat.size().height + k);
  }
 }
 for(k=0;k<resizemat.size().width;k++)
 {
  for(l = 0;l<resizemat.size().height;l++)
  {
   outmat.at<Vec3b>(cy + k,cx-resizemat.size().width + l)[0] = resizemat.at<Vec3b>(k,l)[0];
   outmat.at<Vec3b>(cy + k,cx-resizemat.size().width + l)[1] = resizemat.at<Vec3b>(k,l)[1];
   outmat.at<Vec3b>(cy + k,cx-resizemat.size().width + l)[2] = resizemat.at<Vec3b>(k,l)[2];
    //printf("(%d , %d)", cx-resizemat.size().width + l,cy + k);
  }
 }
 for(k=0;k<resizemat.size().width;k++)
 {
  for(l = 0;l<resizemat.size().height;l++)
  {
   outmat.at<Vec3b>(cy + k,cx + l)[0] = resizemat.at<Vec3b>(k,l)[0];
   outmat.at<Vec3b>(cy + k,cx + l)[1] = resizemat.at<Vec3b>(k,l)[1];
   outmat.at<Vec3b>(cy + k,cx + l)[2] = resizemat.at<Vec3b>(k,l)[2];
    //printf("(%d , %d)",cx + l,cy + k);
  }
 }
}
while(flag)
{
 anglepoint = 0;
 cnt = 0;
 while(anglepoint != 360)
 {
  if(stackcheck(stack,anglepoint,stcnt) == 0)
  {
   radiangle = anglepoint/180*pi;
   resizemat = resinrota(inmat,anglepoint,0.125);
   for(r = 0; r<outmat.size().width/2; r++)
   {
    outmat = spacecheck(&cnt,outmat,resizemat,(int)(r*cos(pi/2-radiangle)+outmat.size().width/2),outmat.size().height/2-(int)r*sin(pi/2-radiangle));
   }
   stack[stcnt++] = anglepoint;
  }
   anglepoint = anglepoint +dangle;
 }
 dangle = dangle/2;
 if(cnt == 0)
 {
  flag = false;
 }
}
return outmat;
}

int stackcheck(float arr[],float angle, int cnt)
{
 int i;
 for(i = 0; i<cnt;i++)
 {
  if(angle == arr[i])
  {
   return 1;
  }
 }
 return 0;
}

Mat spacecheck(int* cnt, Mat board,Mat check,int x, int y)
{
Mat copymat;
int i,j;
int height = check.size().height;
int width = check.size().width;
board.copyTo(copymat);
bool ch = false;
bool overflow = (x-check.size().width/2<0)||(y-check.size().height/2<0)||(x+check.size().width/2>(board.size().width-1))||(y+check.size().height/2>(board.size().height-1));
if(!overflow)
{
 for(i = 0; i<check.size().width;i++)
 {
  for(j = 0; j<check.size().height;j++)
  {
   if(check.at<Vec3b>(j,i)[0] == 0&&check.at<Vec3b>(j,i)[1] == 0&&check.at<Vec3b>(j,i)[2] == 0 )
   {
   }
   else
   {
     if(board.at<Vec3b>(y-height/2+j,x-width/2+i)[0]!=0&&board.at<Vec3b>(y-height/2+j,x-width/2+i)[1]!=0&&board.at<Vec3b>(y-height/2+j,x-width/2+i)[2]!=0)
       {
            ch = true;
       }
     else if(sqrt(pow(y-height/2+j-board.size().height/2,2)+pow(x-width/2+i-board.size().width/2,2))>board.size().width/2+5)
       {
            ch = true;
       }
     else
       {
            copymat.at<Vec3b>(y-height/2+j,x-width/2+i)[0] = check.at<Vec3b>(j,i)[0];
            copymat.at<Vec3b>(y-height/2+j,x-width/2+i)[1] = check.at<Vec3b>(j,i)[1];
            copymat.at<Vec3b>(y-height/2+j,x-width/2+i)[2] = check.at<Vec3b>(j,i)[2];
        
        //printf("(%d , %d)",x-width/2+i,y-height/2+j);
       }
   }
  }
 }
}
if(ch||overflow)
{
 return board;
}
else
{
 *cnt = *cnt + 1;
 return copymat;
}
}

Mat ovw(Mat inmat)
{
float pi = 3.141592;
float r,x,y,i,j,theta,theta2,cx,cy,t,x0,y0,angle;
int dx, dy;
int k = 0;
 printf("rotation angle : ");
 scanf("%f", &angle);
 angle = 3.141592/180*angle;
x0 = inmat.size().width/2;
y0 = inmat.size().height/2;
Mat outmat;
inmat.copyTo(outmat);
 for(i = 0; i<inmat.size().width; i++)
 {
         for(j= 0; j<inmat.size().height; j++)
        {
                 x = i-x0;
                 y = -(j-y0);
                 if(x != 0)
                 {
           r = sqrt(x*x + y*y);
                theta = atan(abs(y/x));
                   if(x>0&&y>0)
                  {
                   theta2 = theta+angle;
                   }
    if(x<0&&y>0)
                  {
                   theta2 = pi - theta+angle;
                   }
                  if(x<0&&y<0)
                  {
                     theta2 = pi + theta+angle;
                  }
    if(x>0&&y<0)
                  {
                     theta2 = -theta+angle;
                  }
                  if(y == 0)
                  {
     if(x > 0)
     {
                      theta2 = angle;
     }
     else
     {
      theta2 = pi+angle;
     }                  
    }
                 }
                 else
                 {
                     if(y>=0)
                    {
                          theta2 = pi/2+angle;
                          r = abs(y);
                     }
                     else
                     {
                          theta2 = 3*pi/2+angle;
                          r = abs(y);
                     }
                 }
                 cx = round(x0 + r*cos(theta2));
                 cy = round(y0 - r*sin(theta2));
                 dx = (int)cx;
                 dy = (int)cy;
   if(dx>0&&dy>0&&dx<inmat.size().width&&dy<inmat.size().height)
   {
    if(inmat.at<Vec3b>(cy,cx)[0] != 0&&inmat.at<Vec3b>(cy,cx)[1] != 0&&inmat.at<Vec3b>(cy,cx)[2] != 0)
    {
     //printf("%d\n",++k);
                   outmat.at<Vec3b>(j,i)[0] = inmat.at<Vec3b>(cy-1,cx)[0];
                   outmat.at<Vec3b>(j,i)[1] = inmat.at<Vec3b>(cy-1,cx)[1];
                   outmat.at<Vec3b>(j,i)[2] = inmat.at<Vec3b>(cy-1,cx)[2];
    }   
   }
         }
 }
return outmat;
}
Mat Dia(Mat inmat)
{
int x0,y0,xlong,i,j,k;
unsigned int sumru,sumgu,sumbu,sumrd,sumgd,sumbd;
x0 = inmat.size().width/2;
y0 = inmat.size().height/2;
Mat outmat(inmat.size().height, inmat.size().width, CV_8UC3);
for(i = 0; i<inmat.size().height/2+1;i++)
{
 if(i!=y0)
 {
  xlong = i*2*x0/y0+1;
  for(j = 0; j< xlong; j++)
  {
   sumru = 0,sumgu = 0,sumbu = 0, sumrd = 0 , sumgd = 0, sumbd = 0;
   for(k = 0; k< inmat.size().width/xlong;k++)
   {
    sumru += inmat.at<Vec3b>(i,j*inmat.size().width/xlong+k)[0];
    sumgu += inmat.at<Vec3b>(i,j*inmat.size().width/xlong+k)[1];
    sumbu += inmat.at<Vec3b>(i,j*inmat.size().width/xlong+k)[2];
    sumrd += inmat.at<Vec3b>(inmat.size().height-i,j*inmat.size().width/xlong+k)[0];
    sumgd += inmat.at<Vec3b>(inmat.size().height-i,j*inmat.size().width/xlong+k)[1];
    sumbd += inmat.at<Vec3b>(inmat.size().height-i,j*inmat.size().width/xlong+k)[2];
   }
   outmat.at<Vec3b>(i,x0-round(xlong/2)+j+1)[0] = sumru/(inmat.size().width/xlong);
   outmat.at<Vec3b>(i,x0-round(xlong/2)+j+1)[1] = sumgu/(inmat.size().width/xlong);
   outmat.at<Vec3b>(i,x0-round(xlong/2)+j+1)[2] = sumbu/(inmat.size().width/xlong);
   outmat.at<Vec3b>(inmat.size().height-i,x0-round(xlong/2)+j+1)[0] = sumrd/(inmat.size().width/xlong);
   outmat.at<Vec3b>(inmat.size().height-i,x0-round(xlong/2)+j+1)[1] = sumgd/(inmat.size().width/xlong);
   outmat.at<Vec3b>(inmat.size().height-i,x0-round(xlong/2)+j+1)[2] = sumbd/(inmat.size().width/xlong);
   //printf("(%d,%f)\n",i,x0-round(xlong/2)+j+1);
  }
 }
 else
 {
  for(k = 0; k<inmat.size().width;k++)
  {
   outmat.at<Vec3b>(i,k)[0] = inmat.at<Vec3b>(i,k)[0];
   outmat.at<Vec3b>(i,k)[1] = inmat.at<Vec3b>(i,k)[1];
   outmat.at<Vec3b>(i,k)[2] = inmat.at<Vec3b>(i,k)[2];
   
  }
 }
}
return outmat;
}

Mat stitch(Mat inmat)
{
int i,j,k,l,x,y,sumr,sumg,sumb,rate;
sumr =0; sumg = 0; sumb =0;
int sizew, sizeh,check;
int inx, iny;
int widthx, heighty;
 printf("stitch rate : ");
 scanf("%d", &rate);
Mat outmat(inmat.size().height, inmat.size().width, CV_8UC3);
widthx = inmat.size().width/rate;
heighty = inmat.size().height/rate;
for(i = 0;i<inmat.size().width; i++)
{
 for(j = 0;j<inmat.size().height; j++)
 {
  sumr =0; sumg = 0; sumb =0;
  x = i%widthx;
  y = j%heighty;
  for(k=0;k<rate;k++)
  {
   for(l=0;l<rate;l++)
   {
    sumr += inmat.at<Vec3b>(y*rate+k,x*rate+l)[0];
    sumg += inmat.at<Vec3b>(y*rate+k,x*rate+l)[1];
    sumb += inmat.at<Vec3b>(y*rate+k,x*rate+l)[2];
   }
  }
  outmat.at<Vec3b>(j,i)[0] = sumr/(rate*rate);
  outmat.at<Vec3b>(j,i)[1] = sumg/(rate*rate);
  outmat.at<Vec3b>(j,i)[2] = sumb/(rate*rate);
 }
}
return outmat;
}

Mat rotation(Mat inmat)
{
float pi = 3.141592;
float r0,r,x,y,i,j,theta,theta2,cx,cy,t,xi0,yi0,xo0,yo0,xl,yl,xr,yr,theta0,angle;
int dx, dy, widtho,heighto;
int k = 0;
 printf("rotation angle : ");
 scanf("%f", &angle);
 angle = 3.141592/180*angle;
xi0 = inmat.size().width/2;
yi0 = inmat.size().height/2;
theta0 = atan(abs(yi0/xi0));
r0 = sqrt(xi0*xi0+yi0*yi0);
xl = floor(r0*cos(pi-theta0-angle))+1;
yl = floor(r0*sin(pi-theta0-angle))+1;
xr = floor(r0*cos(theta0-angle))+1;
yr = floor(r0*sin(theta0-angle))+1;
if(abs(xl)>abs(xr))
{
 widtho = abs(xl);
}
else
{
 widtho = abs(xr);
}
if(abs(yl)>abs(yr))
{
 heighto = abs(yl);
}
else
{
 heighto = abs(yr);
}
xo0 = widtho;
yo0 = heighto;
Mat outmat(heighto*2, widtho*2, CV_8UC3);
for(i = 0; i<widtho*2; i++)
{
        for(j= 0; j<heighto*2; j++)
      {
                x = i-xo0;
                y = -(j-yo0);
                if(x != 0)
                {
          r = sqrt(x*x + y*y);
        theta = atan(abs(y/x));
                 if(x>0&&y>0)
                 {
                  theta2 = theta+angle;
                 }
   if(x<0&&y>0)
                 {
                  theta2 = pi - theta+angle;
                 }
                 if(x<0&&y<0)
                 {
                  theta2 = pi + theta+angle;
                 }
   if(x>0&&y<0)
                 {
                  theta2 = -theta+angle;
                 }
                 if(y == 0)
                 {
    if(x > 0)
    {
                   theta2 = angle;
    }
    else
    {
     theta2 = pi+angle;
    }                  
   }
  }
         else
         {
                if(y>=0)
                 {
                         theta2 = pi/2+angle;
                         r = abs(y);
                 }
                 else
                 {
                        theta2 = 3*pi/2+angle;
                        r = abs(y);
                 }
         }
         cx = round(xi0 + r*cos(theta2));
         cy = round(yi0 - r*sin(theta2));
         dx = (int)cx;
         dy = (int)cy;
  if(dx>0&&dy>0&&dx<inmat.size().width&&dy<inmat.size().height)
  {
   //printf("%d\n",++k);
                 outmat.at<Vec3b>(j,i)[0] = inmat.at<Vec3b>(cy,cx)[0];
                 outmat.at<Vec3b>(j,i)[1] = inmat.at<Vec3b>(cy,cx)[1];
                 outmat.at<Vec3b>(j,i)[2] = inmat.at<Vec3b>(cy,cx)[2];
  }
        }
}
return outmat;
}
Mat rota(Mat inmat)
{
float pi = 3.141592;
float r,x,y,i,j,theta,theta2,cx,cy,t,x0,y0,angle;
int dx, dy;
int k = 0;
x0 = inmat.size().width/2;
y0 = inmat.size().height/2;
 printf("rotation angle : ");
 scanf("%f", &angle);
 angle = 3.141592/180*angle;
Mat outmat(inmat.size().height, inmat.size().width, CV_8UC3);
 for(i = 0; i<inmat.size().width; i++)
 {
         for(j= 0; j<inmat.size().height; j++)
        {
                 x = i-x0;
                 y = -(j-y0);
                 if(x != 0)
                 {
           r = sqrt(x*x + y*y);
                theta = atan(abs(y/x));
                   if(x>0&&y>0)
                  {
                   theta2 = theta+angle;
                   }
    if(x<0&&y>0)
                  {
                   theta2 = pi - theta+angle;
                   }
                  if(x<0&&y<0)
                  {
                     theta2 = pi + theta+angle;
                  }
    if(x>0&&y<0)
                  {
                     theta2 = -theta+angle;
                  }
                  if(y == 0)
                  {
     if(x > 0)
     {
                      theta2 = angle;
     }
     else
     {
      theta2 = pi+angle;
     }                  
    }
                 }
                 else
                 {
                     if(y>=0)
                    {
                          theta2 = pi/2+angle;
                          r = abs(y);
                     }
                     else
                     {
                          theta2 = 3*pi/2+angle;
                          r = abs(y);
                     }
                 }
                 cx = round(x0 + r*cos(theta2));
                 cy = round(y0 - r*sin(theta2));
                 dx = (int)cx;
                 dy = (int)cy;
   if(dx>0&&dy>0&&dx<inmat.size().width&&dy<inmat.size().height)
   {
    //printf("%d\n",++k);
                  outmat.at<Vec3b>(j,i)[0] = inmat.at<Vec3b>(cy-1,cx)[0];
                  outmat.at<Vec3b>(j,i)[1] = inmat.at<Vec3b>(cy-1,cx)[1];
                  outmat.at<Vec3b>(j,i)[2] = inmat.at<Vec3b>(cy-1,cx)[2];
   }
         }
 }
return outmat;
}
Mat Rhom(Mat inmat)
{
int i,j,k;
float angle,xmax,pi;
unsigned int ylong;
pi = 3.141592;
printf("Input the angle : ");
scanf("%f",&angle);
angle = (90-angle)*pi/180;
xmax = inmat.size().height/tan(angle)+inmat.size().width;
Mat outmat(inmat.size().height, xmax, CV_8UC3);
for(i = 0; i<inmat.size().height; i++)
{
 for(j = 0; j<inmat.size().width; j++)
 {
   outmat.at<Vec3b>(i,(inmat.size().height-i)/tan(angle)+j)[0] = inmat.at<Vec3b>(i,j)[0];
   outmat.at<Vec3b>(i,(inmat.size().height-i)/tan(angle)+j)[1] = inmat.at<Vec3b>(i,j)[1];
   outmat.at<Vec3b>(i,(inmat.size().height-i)/tan(angle)+j)[2] = inmat.at<Vec3b>(i,j)[2];
 }
}
return outmat;
}

Mat resinrota(Mat inmat,float angle, float rate)
{
float pi = 3.141592;
float r0,r,x,y,i,j,theta,theta2,cx,cy,t,xi0,yi0,xo0,yo0,xl,yl,xr,yr,theta0;
int dx, dy, widtho,heighto,l;
int k = 0;
 angle = pi/180*angle;
xi0 = inmat.size().width/2;
yi0 = inmat.size().height/2;
theta0 = atan(abs(yi0/xi0));
r0 = sqrt(xi0*xi0+yi0*yi0);
xl = floor(r0*cos(pi-theta0-angle))+1;
yl = floor(r0*sin(pi-theta0-angle))+1;
xr = floor(r0*cos(theta0-angle))+1;
yr = floor(r0*sin(theta0-angle))+1;
if(abs(xl)>=abs(xr))
{
 widtho = abs(xl);
}
else
{
 widtho = abs(xr);
}
if(abs(yl)>=abs(yr))
{
 heighto = abs(yl);
}
else
{
 heighto = abs(yr-1);
}
xo0 = widtho;
yo0 = heighto;
Mat outmat(heighto*2, widtho*2, CV_8UC3);
for(k=0;k<outmat.size().width;k++)
{
 for(l = 0;l<outmat.size().height;l++)
 {
  outmat.at<Vec3b>(l,k)[0]=0;
  outmat.at<Vec3b>(l,k)[1]=0;
  outmat.at<Vec3b>(l,k)[2]=0;
 }
}
for(i = 0; i<widtho*2; i++)
{
        for(j= 0; j<heighto*2; j++)
      {
                x = i-xo0;
                y = -(j-yo0);
                if(x != 0)
                {
          r = sqrt(x*x + y*y);
        theta = atan(abs(y/x));
                 if(x>0&&y>0)
                 {
                  theta2 = theta+angle;
                 }
   if(x<0&&y>0)
                 {
                  theta2 = pi - theta+angle;
                 }
                 if(x<0&&y<0)
                 {
                  theta2 = pi + theta+angle;
                 }
   if(x>0&&y<0)
                 {
                  theta2 = -theta+angle;
                 }
                 if(y == 0)
                 {
    if(x > 0)
    {
                   theta2 = angle;
    }
    else
    {
     theta2 = pi+angle;
    }                  
   }
  }
         else
         {
                if(y>=0)
                 {
                         theta2 = pi/2+angle;
                         r = abs(y);
                 }
                 else
                 {
                        theta2 = 3*pi/2+angle;
                        r = abs(y);
                 }
         }
         cx = round(xi0 + r*cos(theta2));
         cy = round(yi0 - r*sin(theta2));
         dx = (int)cx;
         dy = (int)cy;
  if(dx>0&&dy>0&&dx<inmat.size().width&&dy<inmat.size().height)
  {
                 outmat.at<Vec3b>(j,i)[0] = inmat.at<Vec3b>(cy,cx)[0];
                 outmat.at<Vec3b>(j,i)[1] = inmat.at<Vec3b>(cy,cx)[1];
                 outmat.at<Vec3b>(j,i)[2] = inmat.at<Vec3b>(cy,cx)[2];
  }
        }
}
Mat outmat1 = resize(outmat,rate);
return outmat1;
}

Mat resize(Mat inmat, float rate)
{
int i,j;
int sizew, sizeh,check;
float inx, iny;
sizew = floor(inmat.size().width*rate);
sizeh = floor(inmat.size().height*rate);
Mat outmat(sizew, sizeh, CV_8UC3);
for(i = 0;i<sizew; i++)
{
 for(j = 0;j<sizeh; j++)
 {
  inx = (float)i / rate;
  iny = (float)j / rate;
  outmat.at<Vec3b>(j,i)[0] = interpolation(inmat,inx,iny,0);
  outmat.at<Vec3b>(j,i)[1] = interpolation(inmat,inx,iny,1);
  outmat.at<Vec3b>(j,i)[2] = interpolation(inmat,inx,iny,2);
 }
}
return outmat;
}

int interpolation(Mat inmat,float inputx,float inputy, int rgb)
{
 float u,d;
 float underinx = fmod(inputx,1);
 float underiny = fmod(inputy,1);
 int upinx = inputx-underinx;
 int upiny = inputy-underiny;
 if(underinx ==0 && underiny == 0)
 {
  return inmat.at<Vec3b>(upiny,upinx)[rgb];
 }
 else if(underinx == 0)
 {
  return (inmat.at<Vec3b>(upiny,upinx)[rgb]*(1-underiny)+inmat.at<Vec3b>(upiny+1,upinx)[rgb]*(underiny));
 }
 else if(underiny == 0)
 {
  return (inmat.at<Vec3b>(upiny,upinx)[rgb]*(1-underinx)+inmat.at<Vec3b>(upiny,upinx+1)[rgb]*(underinx));
 }
 else
 {
  u = (inmat.at<Vec3b>(upiny,upinx)[rgb]*(1-underinx)+inmat.at<Vec3b>(upiny,upinx+1)[rgb]*(underinx));
  d = (inmat.at<Vec3b>(upiny+1,upinx)[rgb]*(1-underinx)+inmat.at<Vec3b>(upiny+1,upinx+1)[rgb]*(underinx));
  return (u*(1-underiny)+d*(underiny));
 }
}
