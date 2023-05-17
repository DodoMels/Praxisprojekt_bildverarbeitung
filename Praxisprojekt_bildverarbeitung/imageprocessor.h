#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "image.h"
#include "circle.h"
#include "line.h"
#include "scalepixel.h"
#include <iostream>

class ImageProcessor {
public:
    // Constructor
    explicit ImageProcessor(cv::VideoCapture& captureWebcam);

    // Deconstructor
    virtual ~ImageProcessor();

    // Process the image to find the inner circles
    std::vector<Circle> processImageToFindInnerCircles(std::string path);

    // Find outer circle
    void findOuterCircle();

    // Find inner circle
    void findInnerCircles();

    std::vector<Circle> extractCirclesFromCvVec3i(std::vector<cv::Vec3f> circles);

    void showTheFoundCircles(std::vector<Circle> circles);

    // Get the radius of the outer circle
    int getRadiusOuterCircle();
    // Get the distance of the D-Sub screws
    int getDistanceDSub();
    std::vector<int> getCenterOuterCircle();
private:
    int MAX_LENGHT_IMAGE = 480;
    int MAX_RADIUS_POSSIBLE = MAX_LENGHT_IMAGE / 2;//(MAX_LENGHT_IMAGE / 2);
    int m_radiusOuterCirclePixel;
    int m_exactDistanceDSubScrews;
    std::vector<Circle> m_innerCircles;

    std::vector<int> m_centerOuterCircle;

    Image m_image;

    Line m_lineInnerCircles;
    cv::VideoCapture* m_captureWebcam;
    ScalePixel m_scalePixel;
};

#endif // IMAGEPROCESSOR_H
