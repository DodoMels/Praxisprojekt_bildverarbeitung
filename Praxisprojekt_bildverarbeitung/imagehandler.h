#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include "imageprocessor.h"
#include "histogramm.h"

class ImageHandler
{
public:
    // Constructor
    explicit ImageHandler(cv::VideoCapture& captureWebcam);

    // Deconstructor:
    virtual ~ImageHandler();

    // Evaluate the sensor
    void evaluateSensor(std::string path);

    // Find the two D-Sub screws
    void findDSubScrews(std::string path);

    void safeCirclesInTolerance();

    // Calculate the average distance of the D-Sub Screws
    void calculateAverageDistanceDSub();

    // Get the iterators of two screws wich are in the tolerance
    void getCirclesInTolerance();

    void getOuterCircle();

    void averageCircles();
    void averageOuterCircle();
    void averageDSubScrews();

private:
    int MINIMUM_MATCHING_CIRCLES;
    int TOLERANCE_DISTANCE_DSUB = 10;
    int TOLERANCE_CENTER_DSUB = 5;
    int TOLERANCE_DSUB = 20;

    int m_averageDistanceDSub;

    ImageProcessor m_imageProcessor1;
    ImageProcessor m_imageProcessor2;
    ImageProcessor m_imageProcessor3;
    ImageProcessor m_imageProcessor4;
    ImageProcessor m_imageProcessor5;
    std::vector<ImageProcessor> m_imageProcessorPool;

    std::vector<std::vector<Circle>> m_innerCirclesPool;
    std::vector<Circle> m_dSubScrews;
    std::vector<std::vector<std::vector<int>>> m_iteratorCirclesInTolerance;
    std::vector<std::vector<int>> m_centerOuterCircles;
    std::vector<int> m_radiusOuterCircles;
    Circle m_outerCircle;
    std::vector<Circle> m_dSubscrews;
    std::vector<std::vector<Circle>> m_circlesInTolerance;

    Image m_imageAverage;
    ScalePixel m_scalePixelAverage;

    Histogramm m_histogramAverage;
    bool m_flagFound;
};

#endif // IMAGEHANDLER_H
