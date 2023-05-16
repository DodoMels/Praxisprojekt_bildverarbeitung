#include "histogramm.h"
#include "circle.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <array>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <algorithm> 
#include <vector>



Histogramm::Histogramm() {

}

Histogramm::~Histogramm() {
}



void Histogramm::calchist() {


	img = cv::imread("../Praxisprojekt_bildverarbeitung/Device_net_mit_abdeckung.jpg", 0);

	// Vektor der funktion bekommt
	std::vector<int> innerscrew(4);
	innerscrew[0] = 466;
	innerscrew[1] = 402;
	innerscrew[2] = 228;
	innerscrew[3] = 416;

	/*	innerscrew[0] = 378;
	innerscrew[1] = 260;
	innerscrew[2] = 380;
	innerscrew[3] = 492;*/
	// Variabeln
	std::vector<int> vec_positiv(2);
	std::vector<int> vec_negativ(2);
	int counter_back = 0;
	int counter_front = 0;
	int d = 7;
	bool screw{};
	cv::Point test;
	cv::Point center1(innerscrew[0], innerscrew[1]);
	cv::Point center2(innerscrew[2], innerscrew[3]);
	cv::LineIterator it(img, center1, center2, 8);

	// Linie in beide richtungen verlängern 
	vec_positiv[0] = center2.x + d * ((center2.x-center1.x)/ (sqrt(abs(center2.x - center1.x) ^ 2 + abs(center2.y - center1.y) ^ 2)));
	vec_positiv[1] = center2.y + d * ((center2.y-center1.y)/ (sqrt(abs(center2.x - center1.x) ^ 2 + abs(center2.y - center1.y) ^ 2)));
	vec_negativ[0] = center1.x - d * ((center2.x - center1.x) / (sqrt(abs(center2.x - center1.x) ^ 2 + abs(center2.y - center1.y) ^ 2)));
	vec_negativ[1] = center1.y - d * ((center2.y - center1.y) / (sqrt(abs(center2.x - center1.x) ^ 2 + abs(center2.y - center1.y) ^ 2)));

	// Darstellung auf Bild	
	up.x = vec_positiv[0];
	up.y = vec_positiv[1];
	down.x = vec_negativ[0];
	down.y = vec_negativ[1];

	cv::line(img, center1, center2, cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
	cv::circle(img, up, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
	cv::circle(img, down, 5, cv::Scalar(0, 255, 0), -1, 8, 0);
	//cv::imshow("img", img);
	//cv::waitKey(0);
	std::vector<float> profile = getLine();
	drawProfile(profile);

	int p_Len = (int)profile.size();
	int i = 0;
	float total = 0;

	for (size_t i = 0; i <p_Len; i++)
	{
		total += profile[i];
	}
	
	float median = (total / profile.size())+20;

	for (size_t i = 0; i < 20; i++)
	{
		if (profile[i] > median)
		{
			counter_front += 1;
			std::cout << "True front" << profile[i] << std::endl;
		}
		else
		{
			std::cout << "False front" << profile[i] << std::endl;
		}
	}

	for (size_t i = profile.size()-20; i < profile.size(); i++)
	{
		if (profile[i] > median)
		{
			counter_back += 1;
			std::cout << "True end" << profile[i] << std::endl;
		}
		else
		{
			std::cout << "false end" << profile[i] << std::endl;
		}
	}
	std::cout << "counter back" << counter_back << "counter_front" << counter_front << std::endl;
	std::cout << "median"<<median << std::endl;
	if (counter_back > 10 && counter_front > 10)
	{
		cv::Point text_position(80, 80);//Declaring the text position//
		int font_size = 1;//Declaring the font size//
		cv::Scalar font_Color(0, 0, 0);//Declaring the color of the font//
		int font_weight = 2;//Declaring the font weight//
		cv::putText(img, "TRUE hat eine schraube", text_position, cv::FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);//Putting the text in the matrix//
		cv::imshow("Image", img);//Showing the image//
		cv::waitKey(0);//Wait for Keystroke//
	}
	else
	{
		cv::Point text_position(80, 80);//Declaring the text position//
		int font_size = 1;//Declaring the font size//
		cv::Scalar font_Color(0, 0, 0);//Declaring the color of the font//
		int font_weight = 2;//Declaring the font weight//
		cv::putText(img, "FALSE hat keine schraube", text_position, cv::FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);//Putting the text in the matrix//
		cv::imshow("Image", img);//Showing the image//
		cv::waitKey(0);//Wait for Keystroke//
	}
	
}

std::vector<float> Histogramm::getLine()
{
	std::vector<float> lineProfile;
	float len = (float)sqrt(pow((float)up.x - (float)down.x, 2.0) + pow((float)up.y - (float)down.y, 2.0));
	float mx = (up.x - down.x) / len;
	float my = (up.y - down.y) / len;
	std::cout << mx << "; " << my << "(angle=" << 180.f * atan2(my, mx) / 3.141f << ")" << std::endl;
	for (int k = 0; k < len; k++)
	{
		lineProfile.push_back(getGraySubpix(img, (float)down.x + mx * (float)k, (float)down.y + my * (float)k));
	}
	return(lineProfile);
}

float Histogramm::getGraySubpix(const cv::Mat& img, float x_in, float y_in)
{
	// cout << "getGraySubpix @ " << x_in << "; " << y_in << std::endl;
	int x = (int)x_in;
	int y = (int)y_in;

	int x0 = cv::borderInterpolate(x, img.cols, cv::BORDER_REFLECT_101);
	int x1 = cv::borderInterpolate(x + 1, img.cols, cv::BORDER_REFLECT_101);
	int y0 = cv::borderInterpolate(y, img.rows, cv::BORDER_REFLECT_101);
	int y1 = cv::borderInterpolate(y + 1, img.rows, cv::BORDER_REFLECT_101);

	float a = x_in - (float)x; // pixel fraction in x-direction
	float b = y_in - (float)y; // pixel fraction in y-direction

	float intp = (float)(((1.f - a) * (float)img.at<uchar>(y0, x0) + a * (float)img.at<uchar>(y0, x1)) * (1.f - b)
		+ ((1.f - a) * (float)img.at<uchar>(y1, x0) + a * (float)img.at<uchar>(y1, x1)) * b);
	// cout << (int)img.at<uchar>(y0, x0) << ", " << (int)img.at<uchar>(y0, x1) << ", " << (int)img.at<uchar>(y1, x0) << ", " << (int)img.at<uchar>(y1, x1) << ". (" << intp << ")" << std::endl;
	return(intp);
}

cv::Vec3b Histogramm::getColorSubpix(const cv::Mat& img, cv::Point2f pt)
{
	assert(!img.empty());
	assert(img.channels() == 3);

	int x = (int)pt.x;
	int y = (int)pt.y;

	int x0 = cv::borderInterpolate(x, img.cols, cv::BORDER_REFLECT_101);
	int x1 = cv::borderInterpolate(x + 1, img.cols, cv::BORDER_REFLECT_101);
	int y0 = cv::borderInterpolate(y, img.rows, cv::BORDER_REFLECT_101);
	int y1 = cv::borderInterpolate(y + 1, img.rows, cv::BORDER_REFLECT_101);

	float a = pt.x - (float)x;
	float c = pt.y - (float)y;

	uchar b = (uchar)cvRound((img.at<cv::Vec3b>(y0, x0)[0] * (1.f - a) + img.at<cv::Vec3b>(y0, x1)[0] * a) * (1.f - c)
		+ (img.at<cv::Vec3b>(y1, x0)[0] * (1.f - a) + img.at<cv::Vec3b>(y1, x1)[0] * a) * c);
	uchar g = (uchar)cvRound((img.at<cv::Vec3b>(y0, x0)[1] * (1.f - a) + img.at<cv::Vec3b>(y0, x1)[1] * a) * (1.f - c)
		+ (img.at<cv::Vec3b>(y1, x0)[1] * (1.f - a) + img.at<cv::Vec3b>(y1, x1)[1] * a) * c);
	uchar r = (uchar)cvRound((img.at<cv::Vec3b>(y0, x0)[2] * (1.f - a) + img.at<cv::Vec3b>(y0, x1)[2] * a) * (1.f - c)
		+ (img.at<cv::Vec3b>(y1, x0)[2] * (1.f - a) + img.at<cv::Vec3b>(y1, x1)[2] * a) * c);

	return cv::Vec3b(b, g, r);
}

void Histogramm::drawProfile(std::vector<float> profile)
{
	int p_Len = (int)profile.size();
	int p_w = 512; int p_h = 400;
	float ratio = 255.f / p_h;
	float bin_w = (float)p_w / (float)p_Len;

	cv::Mat profileImage(p_h, p_w, CV_8UC3, cv::Scalar(255, 255, 255));
	// normalize(profile, profile, 0, p_w, NORM_MINMAX, -1, Mat());

	std::vector <int> x(profile.size());
	

	for (int i = 1; i < p_Len; i++)
	{
		cv::line(profileImage, cv::Point((int)(bin_w * (i - 1)), p_h - cvRound(profile[cvRound(i - 1)] / ratio)),
			cv::Point((int)(bin_w * i), p_h - cvRound(profile[cvRound(i)] / ratio)),
			cv::Scalar(255, 0, 0), 2, 8, 0);
		
	}
	cv::imshow("Profile", profileImage);
}

float Histogramm::getGraySubpix(const cv::Mat& img, cv::Point2f pt)
{
	return getGraySubpix(img, pt.x, pt.y);
}

void Histogramm::check_screw(bool, std::vector<float> lines)
{
	int i = 1;
	double total;
	for (i = 1; i < lines.size(); i++);
	{
		total =+ lines[i];
		std::cout << lines[i] << "\n" << std::endl;
	}
	float median = total / lines.size();
	std::cout << median << std::endl;



}


