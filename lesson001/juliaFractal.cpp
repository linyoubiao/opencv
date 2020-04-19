#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
const int trackbar_val_max = 200;
 
class CMandelbrot
{
public:
	CMandelbrot(): x1(0), y1(), ScaleX(), ScaleY(),image(Mat()){}
	/*explicit CMandelbrot(int) : x1(0), y1(0), ScaleX(0), ScaleY(0)
	{}*/
	CMandelbrot(float x1,float y1,float ScaleX,float ScaleY, Mat image)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->ScaleX = ScaleX;
		this->ScaleY = ScaleY;
		this->image = image;
	}
public:
	Mat   getImage(){return image;}
	float getX1(){return x1;}
	float getY1(){return y1;}
	float getScaleX(){return ScaleX;}
	float getScaleY(){return ScaleY;}
private:
	float x1;
	float y1;
	float ScaleX;
	float ScaleY;
	Mat image;
};
//逃逸时间算法实现
int mandelbrot(const complex<float> &z0, const int max,const complex<float> &z1)
{
	complex<float> z = z0;
 
	for(int t = 0; t < max; t++)
	{
		if(z.real() * z.real() + z.imag() * z.imag()> 4.0f) return t;
		z = z * z + z1;
	}
 
	return max;
}
int mandelbrotFormula(const complex<float> &z0,const complex<float> &z1, const int maxIter=255) {
	int value = mandelbrot(z0, maxIter, z1);
	if(maxIter - value == 0)//if value == maxIter, set pixel = 0
	{
		return 0;
	}
	return cvRound(sqrt(value / (float) maxIter) * 255);// in order to overcome linear scaling is make no sense to change of gray
}
void sequentialManelbrot(Mat &img, const float x1, const float y1, const float scaleX, const float scaleY, complex<float> & c = complex<float>(0, 0) ,int flag = 0)
{
	if(flag == 0)
	{
		if(img.channels() == 1)
		{
			for(int i = 0; i < img.rows; i++)
			{
				for(int j = 0; j < img.cols; j++)
				{
					float x0 = j / scaleX + x1;//img x point convert to Mandelbrot set
					float y0 = i / scaleY + y1;//img y point convert to Mandelbrot set
					complex<float> z0(x0,y0);
					uchar value = (uchar)mandelbrotFormula(z0,z0);
					img.ptr<uchar>(i)[j] = value;
 
				}
			}
		}
		else
		{
			for(int i = 0; i < img.rows; i++)
			{
				for(int j = 0; j < img.cols; j++)
				{
					float x0 = j / scaleX + x1;//img x point convert to Mandelbrot set
					float y0 = i / scaleY + y1;//img y point convert to Mandelbrot set
					complex<float> z0(x0,y0);
					uchar value = (uchar)mandelbrotFormula(z0,z0);
					if(value > 150)
						img.at<Vec3b>(i, j) = Vec3b(0, 0, value);
					else if(value > 75)
						img.at<Vec3b>(i, j) = Vec3b(0, saturate_cast<uchar>(value + 75), saturate_cast<uchar>(value+150));
					else if(value > 50)
						img.at<Vec3b>(i, j) = Vec3b(0, value+50, 100);
					else if(value > 10)
						img.at<Vec3b>(i, j) = Vec3b(value+25, value+50, 100);
					else if(value > 0)
						img.at<Vec3b>(i, j) = Vec3b(value+25, 0, 100);
					else
						img.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
				}
 
			}
		}
	}
	else
	{
		if(img.channels() == 1)
		{
			for(int i = 0; i < img.rows; i++)
			{
				for(int j = 0; j < img.cols; j++)
				{
					float x0 = j / scaleX + x1;//img x point convert to Mandelbrot set
					float y0 = i / scaleY + y1;//img y point convert to Mandelbrot set
					complex<float> z0(x0,y0);
					uchar value = (uchar)mandelbrotFormula(z0,c);
					img.ptr<uchar>(i)[j] = value;
 
				}
 
			}
		}
		else
		{
			for(int i = 0; i < img.rows; i++)
			{
				for(int j = 0; j < img.cols; j++)
				{
					float x0 = j / scaleX + x1;//img x point convert to Mandelbrot set
					float y0 = i / scaleY + y1;//img y point convert to Mandelbrot set
					complex<float> z0(x0,y0);
					uchar value = (uchar)mandelbrotFormula(z0,c);
					if(value > 150)
						img.at<Vec3b>(i, j) = Vec3b(0, 0, value);
					else if(value > 75)
						img.at<Vec3b>(i, j) = Vec3b(0, value, saturate_cast<uchar>(value+150));
					else if(value > 50)
						img.at<Vec3b>(i, j) = Vec3b(0, value, 50);
					else if(value > 10)
						img.at<Vec3b>(i, j) = Vec3b(value+25, value, 50);
					else if(value > 0)
						img.at<Vec3b>(i, j) = Vec3b(value+25, 0, 50);
					else
						img.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
				}
 
			}
		}
 
	}
	
}
static float CalibrateRate(int pos)
{
	float rate = pos * 1.0f /  trackbar_val_max;
	if(rate < 0.5)
		rate = -2 * rate;
	else
		rate = 2 * rate;
	return rate;
}
static void on_trackbar_Real(int pos, void * userdata)
{
	int im_pos = getTrackbarPos("imaginary", "fractal");
	CMandelbrot *mm = (CMandelbrot *)userdata;
	float re_rate = CalibrateRate(pos);
	float im_rate = CalibrateRate(im_pos);
#ifdef _DEBUG
	cout << "re_rate" << re_rate << endl;
	cout << "im_rate" << im_rate << endl;
#endif
	sequentialManelbrot(mm->getImage(),mm->getX1(),mm->getY1(),mm->getScaleX(),mm->getScaleY(),complex<float>(re_rate,im_rate),1);
	imshow("fractal",mm->getImage());
}
static void on_trackbar_Imaginary(int pos , void * userdata)
{
	int re_pos = getTrackbarPos("real", "fractal");
	CMandelbrot *mm = (CMandelbrot *)userdata;
	float re_rate = CalibrateRate(re_pos);
	float im_rate = CalibrateRate(pos);
#ifdef _DEBUG
	cout << "re_rate" << re_rate << endl;
	cout << "im_rate" << im_rate << endl;
#endif
	
	sequentialManelbrot(mm->getImage(),mm->getX1(),mm->getY1(),mm->getScaleX(),mm->getScaleY(),complex<float>(re_rate,im_rate),1);
	imshow("fractal",mm->getImage());
}
int main()
{
	namedWindow("fractal",1);
	Mat mandelbrotImg2(800,600,CV_8UC3);
	float x1 = -1.4f, x2 = 1.16f;
	float y1 = -1.2f, y2 = 1.2f;
	float scaleX = mandelbrotImg2.cols / (x2 - x1);
	float scaleY = mandelbrotImg2.rows / (y2 - y1);
	CMandelbrot mm(x1,y1,scaleX,scaleY,mandelbrotImg2);
	int re = 40,im = 40;
	createTrackbar("real","fractal",&re,trackbar_val_max,on_trackbar_Real,&mm);
	createTrackbar("imaginary","fractal",&im,trackbar_val_max,on_trackbar_Imaginary,&mm);
	waitKey();
}