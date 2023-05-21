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




Histogramm::Histogramm() {
    m_flagFound = false;
}

Histogramm::~Histogramm() {
}



bool Histogramm::calchist(Image image, std::vector<Circle> circles, int distanceBigScrews, int distanceConector) {


    //img = cv::imread("../Praxisprojekt_bildverarbeitung/Profibus.jpg", 0);
    img = image.getGrayscaleImage();

    std::vector<int> first = circles.at(0).getCenter();
    std::vector<int> second = circles.at(1).getCenter();
    // Variabeln
    float vBetragfirst = (float) sqrt((first.at(0) * first.at(0)) + (first.at(1)*first.at(1)));
    float vBetragsecond = (float)sqrt((second.at(0) * second.at(0)) + (second.at(1) * second.at(1)));
    std::vector<int> vec_positiv(2);
    std::vector<int> vec_negativ(2);
    int counter_back = 0;
    int counter_front = 0;
    int findrightconector = 0;
    float medianForBlueColor = 0;
    int dsubStartPixelAroundMidle = 30;
    int deviceNetStartPixelMidle = 15;

    int d = distanceBigScrews;
    bool screw{};
    cv::Point test;
    cv::Point center1(first.at(0), first.at(1));
    cv::Point center2(second.at(0), second.at(1));
    //cv::LineIterator it(img, center1, center2, 8);
    // Linie in beide richtungen verl�ngern
    vec_positiv[0] = static_cast<float>(center2.x) + static_cast<float>(d) * (static_cast<float>(center2.x-center1.x)/ (std::sqrt(static_cast<float>(((center2.x - center1.x)*(center2.x - center1.x)) + ((center2.y - center1.y)*(center2.y - center1.y))))));
    vec_positiv[1] = static_cast<float>(center2.y) + static_cast<float>(d) * (static_cast<float>(center2.y-center1.y)/ (std::sqrt(static_cast<float>(((center2.x - center1.x)*(center2.x - center1.x)) + ((center2.y - center1.y)*(center2.y - center1.y))))));
    vec_negativ[0] = static_cast<float>(center1.x) - static_cast<float>(d) * (static_cast<float>(center2.x - center1.x) / (std::sqrt(static_cast<float>(((center2.x - center1.x)*(center2.x - center1.x)) + ((center2.y - center1.y)*(center2.y - center1.y))))));
    vec_negativ[1] = static_cast<float>(center1.y) - static_cast<float>(d) * (static_cast<float>(center2.y - center1.y) / (std::sqrt(static_cast<float>(((center2.x - center1.x)*(center2.x - center1.x)) + ((center2.y - center1.y)*(center2.y - center1.y))))));

    // Darstellung auf Bild
    up.x = vec_positiv[0];
    up.y = vec_positiv[1];
    down.x = vec_negativ[0];
    down.y = vec_negativ[1];

    //Vektor parallel verschiebn
    std::vector<int> vec_conector_positon_positiv(2);
    std::vector<int> vec_conector_positon_negativ(2);
    std::vector<int> vec_parallel_positiv(2);
    std::vector<int> vec_parallel_negativ(2);

    //vec_conector_positon_positiv[0] = distanceConector * (first.at(0) / vBetragfirst);
    //vec_conector_positon_positiv[1] = distanceConector * (first.at(1) / vBetragfirst);
    //vec_conector_positon_negativ[0] = distanceConector * (second.at(0) / vBetragsecond);
    //vec_conector_positon_negativ[1] = distanceConector * (second.at(1) / vBetragsecond);

    /*
    //for (size_t i = 0; i < vec_conector_positon_negativ.size(); i++)
    {
        vec_parallel_positiv[i] = vec_conector_positon_positiv[i] + first.at(i);
        vec_parallel_negativ[i] = vec_conector_positon_negativ[i] + second.at(i);
    }
    */

    std::vector<float> newPoints = getNewPoints(first, second,distanceConector);
    parallel_up.x = newPoints.at(0);
    parallel_up.y = newPoints.at(1);
    parallel_down.x = newPoints.at(2);
    parallel_down.y = newPoints.at(3);

    cv::circle(img, parallel_up, 2, cv::Scalar(0, 255, 0), -1, 8, 0);
    cv::circle(img, parallel_down, 2, cv::Scalar(0, 255, 0), -1, 8, 0);
    cv::line(img, center1, center2, cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
    cv::circle(img, up, 10, cv::Scalar(0, 255, 0), -1, 8, 0);
    cv::circle(img, down, 10, cv::Scalar(0, 255, 0), -1, 8, 0);

    std::vector<float> conector = getLine(parallel_up, parallel_down);
    drawProfile(conector, "conector");
    std::vector<float> profile = getLine(up,down);
    drawProfile(profile,"Screws");

    if (int distance = getMaxPeak(conector,0,0,dsubStartPixelAroundMidle) < 10)
    {
        std::vector<float> newPoints = getNewPointsNegativ(first, second, distanceConector);
        parallel_up.x = newPoints.at(0);
        parallel_up.y = newPoints.at(1);
        parallel_down.x = newPoints.at(2);
        parallel_down.y = newPoints.at(3);
        cv::circle(img, parallel_up, 2, cv::Scalar(0, 255, 0), -1, 8, 0);
        cv::circle(img, parallel_down, 2, cv::Scalar(0, 255, 0), -1, 8, 0);
        std::vector<float> conector = getLine(parallel_up, parallel_down);
        drawProfile(conector, "conector otherside");

        if (int distance =getMaxPeak(conector,0,0,dsubStartPixelAroundMidle) < 10)
        {
            cv::Point text_position(40, 40);//Declaring the text position//
            int font_size = 1;//Declaring the font size//
            cv::Scalar font_Color(0, 0, 0);//Declaring the color of the font//
            int font_weight = 2;//Declaring the font weight//
            cv::putText(img, "DSUB 15", text_position, cv::FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);//Putting the text in the matrix//
            cv::imshow("Image", img);//Showing the image//
            //cv::waitKey(0);//Wait for Keystroke//
        }
        else 
        {
            if (int distance = getMaxPeak(conector, 0, 0, deviceNetStartPixelMidle) < 10)
            {
                cv::Point text_position(40, 40);//Declaring the text position//
                int font_size = 1;//Declaring the font size//
                cv::Scalar font_Color(0, 0, 0);//Declaring the color of the font//
                int font_weight = 2;//Declaring the font weight//
                cv::putText(img, "Devicenet", text_position, cv::FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);//Putting the text in the matrix//
                cv::imshow("Image", img);//Showing the image//
                //cv::waitKey(0);//Wait for Keystroke//
            }
            else
            {
                cv::Point text_position(40, 40);//Declaring the text position//
                int font_size = 1;//Declaring the font size//
                cv::Scalar font_Color(0, 0, 0);//Declaring the color of the font//
                int font_weight = 2;//Declaring the font weight//
                cv::putText(img, "Ethercat", text_position, cv::FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);//Putting the text in the matrix//
                cv::imshow("Image", img);//Showing the image//
                //cv::waitKey(0);//Wait for Keystroke//

            }


        }
    }
    
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
        if (profile[i] > 120)
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
        if (profile[i] > 120)
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
    if (counter_back > 1 && counter_front > 1)
    {
        cv::Point text_position(80, 80);//Declaring the text position//
        int font_size = 1;//Declaring the font size//
        cv::Scalar font_Color(0, 0, 0);//Declaring the color of the font//
        int font_weight = 2;//Declaring the font weight//
        cv::putText(img, "TRUE hat eine schraube", text_position, cv::FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);//Putting the text in the matrix//
        cv::imshow("Image", img);//Showing the image//
        //cv::waitKey(0);//Wait for Keystroke//
    }
    else
    {
        cv::Point text_position(80, 80);//Declaring the text position//
        int font_size = 1;//Declaring the font size//
        cv::Scalar font_Color(0, 0, 0);//Declaring the color of the font//
        int font_weight = 2;//Declaring the font weight//
        cv::putText(img, "FALSE hat keine schraube", text_position, cv::FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);//Putting the text in the matrix//
        cv::imshow("Image", img);//Showing the image//
        //cv::waitKey(0);//Wait for Keystroke//
    }
    return m_flagFound;
}

std::vector<float> Histogramm::getNewPoints(std::vector<int> firstCircle, std::vector<int> secondCircle,int distanceConectorToDsub)
{

    float slope = static_cast<float>(secondCircle.at(1) - firstCircle.at(1)) / static_cast<float>(secondCircle.at(0) - firstCircle.at(0));
    float perpendicular = -1 / slope;

    float x_new = firstCircle.at(0) + (distanceConectorToDsub / sqrt(1 + perpendicular * perpendicular));
    float y_new = firstCircle.at(1) + (distanceConectorToDsub * perpendicular / sqrt(1 + perpendicular * perpendicular));
    float x_newSecond = secondCircle.at(0) + (distanceConectorToDsub / sqrt(1 + perpendicular * perpendicular));
    float y_newSecond = secondCircle.at(1) + (distanceConectorToDsub * perpendicular / sqrt(1 + perpendicular * perpendicular));
    std::vector<float> newpoints{ x_new,y_new,x_newSecond,y_newSecond };
    
    return newpoints;
}
std::vector<float> Histogramm::getNewPointsNegativ(std::vector<int> firstCircle, std::vector<int> secondCircle, int distanceConectorToDsub)
{
    float slope = static_cast<float>(secondCircle.at(1) - firstCircle.at(1)) / static_cast<float>(secondCircle.at(0) - firstCircle.at(0));
    float perpendicular = -1 / slope;

    float x_new = firstCircle.at(0) - (distanceConectorToDsub / sqrt(1 + perpendicular * perpendicular));
    float y_new = firstCircle.at(1) - (distanceConectorToDsub * perpendicular / sqrt(1 + perpendicular * perpendicular));
    float x_newSecond = secondCircle.at(0) - (distanceConectorToDsub / sqrt(1 + perpendicular * perpendicular));
    float y_newSecond = secondCircle.at(1) - (distanceConectorToDsub * perpendicular / sqrt(1 + perpendicular * perpendicular));
    std::vector<float> newpoints{ x_new,y_new,x_newSecond,y_newSecond };

    return newpoints;
}


int Histogramm::getMaxPeak(std::vector<float> lines, int pixelStart, int pixelEnd, int valueAroundMidle)
{
    int midle = lines.size() / 2;
    int min = 100;
    int max = 0;

    for (size_t i = midle - valueAroundMidle; i < midle + valueAroundMidle; i++)
    {
        if (min > lines.at(i))
        {
            min = lines.at(i);
        }
        if (max < lines.at(i))
        {
            max = lines.at(i);
        }
    }
    int maxDistance = max - min;

    return maxDistance;
}

std::vector<float> Histogramm::getLine(cv::Point circle1, cv::Point circle2)
{
    std::vector<float> lineProfile;
    float len = (float)sqrt(pow((float)circle1.x - (float)circle2.x, 2.0) + pow((float)circle1.y - (float)circle2.y, 2.0));
    float mx = static_cast<float>(circle1.x - circle2.x) / static_cast<float>(len);
    float my = static_cast<float>(circle1.y - circle2.y) / static_cast<float>(len);
    std::cout << mx << "; " << my << " (angle=" << 180.f * atan2(my, mx) / 3.141f << ")" << std::endl;
    for (int k = 0; k < len; k++)
    {
        lineProfile.push_back(getGraySubpix(img, static_cast<float>(circle2.x) + (mx * static_cast<float>(k)), static_cast<float>(circle2.y) + (my * static_cast<float>(k))));
    }
    return(lineProfile);
}

float Histogramm::getAngle()
{
    float len = (float)sqrt(pow((float)up.x - (float)down.x, 2.0) + pow((float)up.y - (float)down.y, 2.0));
    float mx = (up.x - down.x) / len;
    float my = (up.y - down.y) / len;
    float alpha = 180.f * atan2(my, mx) / 3.141f;
    std::cout << mx << "; " << my << "(angle=" << 180.f * atan2(my, mx) / 3.141f << ")" << std::endl;
    return (alpha);
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

void Histogramm::drawProfile(std::vector<float> profile, std::string imagename)
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
    cv::imshow(imagename, profileImage);
}


float Histogramm::getGraySubpix(const cv::Mat& img, cv::Point2f pt)
{
    return getGraySubpix(img, pt.x, pt.y);
}
