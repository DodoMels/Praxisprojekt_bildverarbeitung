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


    bool calchist(Image image, std::vector<Circle> circles, int distance);
    std::vector<float> getLine();
    float getGraySubpix(const cv::Mat& img, float x_in, float y_in);
    cv::Vec3b getColorSubpix(const cv::Mat& img, cv::Point2f pt);
    void drawProfile(std::vector<float> profile);
    float getGraySubpix(const cv::Mat& img, cv::Point2f pt);
    void check_screw(bool ,std::vector<float> lines);
    cv::Mat img;
    cv::Point up;
    cv::Point down;

private:
    bool m_flagFound;


};

#endif // HISTOGRAMM_H

