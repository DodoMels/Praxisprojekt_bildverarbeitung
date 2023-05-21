#ifndef HISTOGRAMM_H
#define HISTOGRAMM_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
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
#include "image.h"


class Histogramm
{
public:
    // Constructor
    explicit Histogramm();

    // Destructor
    virtual ~Histogramm();


    bool calchist(Image image, std::vector<Circle> circles, int distanceBigScrews, int distanceConector);
    std::vector<float> getLine(cv::Point circle1,cv::Point circle2);
    float getAngle();
    float getGraySubpix(const cv::Mat& img, float x_in, float y_in);
    cv::Vec3b getColorSubpix(const cv::Mat& img, cv::Point2f pt);
    void drawProfile(std::vector<float> profile, std::string imagename);
    std::vector<float> getNewPoints(std::vector <int> firstCircle, std::vector<int> secondCircle, int distanceConectorToDsub);

    std::vector<float> getNewPointsNegativ(std::vector<int> firstCircle, std::vector<int> secondCircle, int distanceConectorToDsub);
    int getMaxPeak(std::vector<float> lines,int pixelStart, int pixelEnd, int valueAroundMidle);

    float getGraySubpix(const cv::Mat& img, cv::Point2f pt);
   
    cv::Mat img;
    cv::Point up;
    cv::Point down;
    cv::Point parallel_up;
    cv::Point parallel_down;

private:
    bool m_flagFound;


};

#endif // HISTOGRAMM_H

