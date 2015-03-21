#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



using namespace std;
using namespace cv;
Mat src;
const double pi = 3.14159265359;
double c = 0;
Vec3b getColor(double phi, double theta) {

	double u, v;
	double dx, dy, dz;
	dx = -sin(phi) * cos(theta);
	dy = -sin(phi) * sin(theta);
	dz = -cos(phi);
	u = 0.5 + atan2(dz, dx) / (2 * pi);
	v = 0.5 - asin(dy) / pi;
	u *= src.cols;
	v *= src.rows;
	return src.at<Vec3b>(Point((int)u, (int)v));
}

Vec3b getColor(double dx, double dy, double dz) {
	double dist = sqrt(dx * dx + dy * dy + dz * dz);
	dx /= dist;
	dy /= dist;
	dz /= dist;
	double u, v;
	u = 0.5 + atan2(dz, dx) / (2 * pi);
	v = 0.5 - asin(dy) / pi;
	u *= src.cols;
	v *= src.rows;
	//circle(src, Point((int)u, (int)v), 2, Scalar(255, 0, 0));
	
	return src.at<Vec3b>(Point((int)u, (int)v));
}

void rotateY(double phi, double &x, double &y, double &z)
{
	double a = x, b = y, c = z;
	x = a*cos(phi) + c*sin(phi);
	z = -a*sin(phi) + c*cos(phi);
}

void rotateX(double phi, double &x, double &y, double &z)
{
	double a = x, b = y, c = z;
	y = b * cos(phi) - c * sin(phi);
	z = b * sin(phi) + c * cos(phi);
}

void rotateZ(double phi, double &x, double &y, double &z)
{
	double a = x, b = y, c = z;

	x = a*cos(phi) - b*sin(phi);
	y = a*sin(phi) + b*cos(phi);
}

int main() {
	//src = imread("C:\\Users\\user\\Downloads\\Telegram Desktop\\PANO_20150315_123330.jpg", 1);

	VideoCapture cap("C:\\Users\\user\\Downloads\\vid.mp4");

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return 0;
	}

	cap >>(src);
	imwrite("src.png", src);

	int w = 800, h = 400;
	Mat res(h, w, src.type());
	double R;
	int i, j;
	R = src.cols / (pi * 2);
	Vec3f norm(0, 0, 0);
	double phi = 0, theta = 0;

	
	while (1) {
		cap.read(src);
		Vec3f cur = norm;
		for (i = 0; i < h; ++i) {
			for (j = 0; j < w; ++j) {
				double a, b, c;
				a = R;
				b = i - h / 2;
				c = j - w / 2;
				//phi = 1;
				//theta = 1;
				while (theta > pi)theta -= pi;
				while (phi > 2 * pi)phi -= 2 * pi;

				//X axis
				//b = b * cos(phi) - c * sin(phi);
				//c = b * cos(phi) + c * sin(phi);

				//Y axis
				//a = a*cos(phi) + c*sin(phi);
				//c = -R*sin(phi) + c*cos(phi);

				//Z axis
				//a=a*cos(phi)-b*sin(phi);
				//b=a*cos(phi)+b*sin(phi);

				rotateZ(theta, a, b, c);

				rotateY(phi, a, b, c);
				
				res.at<Vec3b>(h - 1 - i, j) = getColor(a, b, c);
			}
		}
		//imwrite("pano2.jpg", src);
		//break;
		imshow("res", res);
		char key = waitKey(30);
		double step = 0.05;
		if (key == 27)
			break;
		else if (key == 'a')
			phi += step;
		else if (key == 'd')
			phi -= step;
		else if (key == 'w')
			theta += step;
		else if (key == 's')
			theta -= step;
	}
	return 0;
}