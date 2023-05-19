#include "imagehandler.h"

ImageHandler::ImageHandler(cv::VideoCapture& captureWebcam) :
    m_imageProcessor1(captureWebcam),
    m_imageProcessor2(captureWebcam),
    m_imageProcessor3(captureWebcam),
    m_imageProcessor4(captureWebcam),
    m_imageProcessor5(captureWebcam) {
    m_imageAverage.setCaptureWebcam(captureWebcam);
    m_imageProcessorPool.push_back(m_imageProcessor1);
    m_imageProcessorPool.push_back(m_imageProcessor2);
    m_imageProcessorPool.push_back(m_imageProcessor3);
    m_imageProcessorPool.push_back(m_imageProcessor4);
    m_imageProcessorPool.push_back(m_imageProcessor5);
    MINIMUM_MATCHING_CIRCLES = (m_imageProcessorPool.size() / 2) + 1;

}

ImageHandler::~ImageHandler() {
}

void ImageHandler::evaluateSensor(std::string path) {
    findDSubScrews(path);
}

void ImageHandler::findDSubScrews(std::string path) {
    std::vector<std::vector<int>> temp (2, std::vector<int> (1, 0));
    for (size_t i = 0; i < m_imageProcessorPool.size(); i++) {
        ImageProcessor& imageProzessor = m_imageProcessorPool[i];
        m_innerCirclesPool.push_back(imageProzessor.processImageToFindInnerCircles(path));
        m_iteratorCirclesInTolerance.push_back(temp);
        m_centerOuterCircles.push_back(imageProzessor.getCenterOuterCircle());
        m_radiusOuterCircles.push_back(imageProzessor.getRadiusOuterCircle());
    }
    calculateAverageDistanceDSub();
    getCirclesInTolerance();
    averageCircles();

    m_imageAverage.editImage("Webcam");
    m_imageAverage.showCircles(m_dSubScrews);
    m_imageAverage.showCircles(m_outerCircle);
    //m_imageAverage.showImage("Average image", m_imageAverage.getSharpendGrayscaleImageGaussian());

    m_scalePixelAverage.calculateMmInPixels(m_outerCircle.getRadius());
    //m_pImageAverage = &m_imageAverage;

    m_flagFound = m_histogramAverage.calchist(m_imageAverage, m_dSubScrews, m_scalePixelAverage.getDistanceLidInPixel(),m_scalePixelAverage.getDistanceConectorInPixel());

    /*for (size_t i = 0; i < m_imageProcessorPool.size(); i++) {
        ImageProcessor& imageProzessor = m_imageProcessorPool[i];
        imageProzessor.showTheFoundCircles(m_circlesInTolerance.at(i));
    }*/




    /*
    for (size_t i = 0; i < m_extractedCircles.size(); i++) {
        startSecondForLoop++;
        std::vector<int> centerFirstCircle = m_extractedCircles.at(i).getCenter();
        for (size_t j = startSecondForLoop; j < m_extractedCircles.size(); j++) {
            std::vector<int> centerSecondCircle = m_extractedCircles.at(j).getCenter();
            distanceCircleCenters = sqrt(((centerFirstCircle.at(0) - centerSecondCircle.at(0)) * (centerFirstCircle.at(0) - centerSecondCircle.at(0))) + ((centerSecondCircle.at(1) - centerSecondCircle.at(1)) * (centerSecondCircle.at(1) - centerSecondCircle.at(1))));
            int difference = std::abs(m_exactDistanceDSubScrews - distanceCircleCenters);
            if (difference < smallestDiffrence) {
                //if (m_extractedCircles.at(j).getRadius() < (m_extractedCircles.at(i).getRadius() + 1) && (m_extractedCircles.at(i).getRadius() - 1) < m_extractedCircles.at(j).getRadius()) {
                if (m_extractedCircles.at(i).getRadius() == m_extractedCircles.at(j).getRadius()) {
                    smallestDiffrence = difference;
                    bestIndexFirstCircle = i;
                    bestIndexSecondCircle = j;
                }
            }
        }
    }
    m_dSubScrews.push_back(m_extractedCircles.at(bestIndexFirstCircle));
    m_dSubScrews.push_back(m_extractedCircles.at(bestIndexSecondCircle));*/
}

void ImageHandler::calculateAverageDistanceDSub() {
    int sum = 0;
     for (size_t i = 0; i < m_imageProcessorPool.size(); i++) {
         sum += m_imageProcessorPool.at(i).getDistanceDSub();
     }
     m_averageDistanceDSub = sum / m_imageProcessorPool.size();
}

void ImageHandler::getCirclesInTolerance() {
    for (size_t i = 0; i < m_innerCirclesPool.size(); i++) {
        int startSecondForLoop = 0;
        int distanceCircleCenters = 0;
        int distanceFirstCircleCenterToOuterCircleCenter = 0;
        int distanceSecondCircleCenterToOuterCircleCenter = 0;
        int delta = 0;
        for (size_t j = 0; j < m_innerCirclesPool.at(i).size(); j++) {
            startSecondForLoop++;
            std::vector<int> centerFirstCircle = m_innerCirclesPool.at(i).at(j).getCenter();
            distanceFirstCircleCenterToOuterCircleCenter = sqrt(((centerFirstCircle.at(0) - m_centerOuterCircles.at(i).at(0)) * (centerFirstCircle.at(0) - m_centerOuterCircles.at(i).at(0))) + ((centerFirstCircle.at(1) - m_centerOuterCircles.at(i).at(1)) * (centerFirstCircle.at(1) - m_centerOuterCircles.at(i).at(1))));
            for (size_t k = startSecondForLoop; k < m_innerCirclesPool.at(i).size(); k++) {
                std::vector<int> centerSecondCircle = m_innerCirclesPool.at(i).at(k).getCenter();
                distanceSecondCircleCenterToOuterCircleCenter = sqrt(((centerSecondCircle.at(0) - m_centerOuterCircles.at(i).at(0)) * (centerSecondCircle.at(0) - m_centerOuterCircles.at(i).at(0))) + ((centerSecondCircle.at(1) - m_centerOuterCircles.at(i).at(1)) * (centerSecondCircle.at(1) - m_centerOuterCircles.at(i).at(1))));
                delta = std::abs(distanceFirstCircleCenterToOuterCircleCenter - distanceSecondCircleCenterToOuterCircleCenter);
                distanceCircleCenters = sqrt(((centerFirstCircle.at(0) - centerSecondCircle.at(0)) * (centerFirstCircle.at(0) - centerSecondCircle.at(0))) + ((centerFirstCircle.at(1) - centerSecondCircle.at(1)) * (centerFirstCircle.at(1) - centerSecondCircle.at(1))));
                if ((m_averageDistanceDSub - TOLERANCE_DISTANCE_DSUB) < distanceCircleCenters && distanceCircleCenters < (m_averageDistanceDSub + TOLERANCE_DISTANCE_DSUB)) {
                    if (delta <= TOLERANCE_DISTANCE_DSUB) {
                        m_iteratorCirclesInTolerance.at(i).at(0).push_back(j);
                        m_iteratorCirclesInTolerance.at(i).at(1).push_back(k);
                    }
                }
            }

        }
    }
    safeCirclesInTolerance();
}

void ImageHandler::safeCirclesInTolerance() {
    for (size_t i = 0; i < m_iteratorCirclesInTolerance.size(); i++) {
        int counter = 0;
        Circle emptyCircle;
        std::vector<Circle> temp(((m_iteratorCirclesInTolerance.at(i).at(0).size() - 1) * 2), emptyCircle);
        for (size_t j = 0; j < m_iteratorCirclesInTolerance.at(i).size(); j++) {
            for (size_t k = 1; k < m_iteratorCirclesInTolerance.at(i).at(j).size(); k++) {
                int iterator = m_iteratorCirclesInTolerance.at(i).at(j).at(k);
                temp.at(counter) = m_innerCirclesPool.at(i).at(iterator);
                counter++;
            }
        }
        m_circlesInTolerance.push_back(temp);
    }
}

void ImageHandler::averageCircles() {
    averageOuterCircle();
    averageDSubScrews();
}

void ImageHandler::averageOuterCircle() {
    int sumX = 0;
    int sumY = 0;
    int radius = 0;
    int counter = 0;
    for (size_t i = 0; i < m_centerOuterCircles.size(); i++) {
        sumX += m_centerOuterCircles.at(i).at(0);
        sumY += m_centerOuterCircles.at(i).at(1);
        radius += m_radiusOuterCircles.at(i);
        counter++;
    }
    m_outerCircle.setCenter((sumX / counter), (sumY / counter));
    m_outerCircle.setRadius((radius / counter));
}

void ImageHandler::averageDSubScrews() {
    std::vector<int> xFirst;
    std::vector<int> xSecond;
    std::vector<int> yFirst;
    std::vector<int> ySecond;
    std::vector<int> radius;
    int counter = 0;
    bool flag = true;
    for (size_t i = 0; i < m_circlesInTolerance.size(); i++) {
        if (m_circlesInTolerance.at(i).size() == 2) {
            if (flag) {
                xFirst.push_back(m_circlesInTolerance.at(i).at(0).getCenter().at(0));
                xSecond.push_back(m_circlesInTolerance.at(i).at(1).getCenter().at(0));
                yFirst.push_back(m_circlesInTolerance.at(i).at(0).getCenter().at(1));
                ySecond.push_back(m_circlesInTolerance.at(i).at(1).getCenter().at(1));
                flag = false;
            }
            else if (std::abs(m_circlesInTolerance.at(i).at(0).getCenter().at(0) - xFirst.back()) < TOLERANCE_DSUB) {
                xFirst.push_back(m_circlesInTolerance.at(i).at(0).getCenter().at(0));
                xSecond.push_back(m_circlesInTolerance.at(i).at(1).getCenter().at(0));
                yFirst.push_back(m_circlesInTolerance.at(i).at(0).getCenter().at(1));
                ySecond.push_back(m_circlesInTolerance.at(i).at(1).getCenter().at(1));
            }
            else {
                xFirst.push_back(m_circlesInTolerance.at(i).at(1).getCenter().at(0));
                xSecond.push_back(m_circlesInTolerance.at(i).at(0).getCenter().at(0));
                yFirst.push_back(m_circlesInTolerance.at(i).at(1).getCenter().at(1));
                ySecond.push_back(m_circlesInTolerance.at(i).at(0).getCenter().at(1));
            }
            radius.push_back(m_circlesInTolerance.at(i).at(0).getRadius());
            radius.push_back(m_circlesInTolerance.at(i).at(1).getRadius());
            counter++;
        }
    }
    int sumXFirst = 0;
    int sumYFirst = 0;
    int sumXSecond = 0;
    int sumYSecond = 0;
    for (size_t i = 0; i < xFirst.size(); i++) {
        sumXFirst += xFirst.at(i);
        sumXSecond += xSecond.at(i);
        sumYFirst += yFirst.at(i);
        sumYSecond += ySecond.at(i);
    }
    int radiusInt = 0;
    for (size_t i = 0; i < radius.size(); i++) {
        radiusInt += radius.at(i);
    }
    radiusInt = radiusInt / 2;
    Circle first;
    Circle second; 
    first.setCenter((sumXFirst / counter), (sumYFirst / counter));
    first.setRadius((radiusInt / counter));
    second.setCenter((sumXSecond / counter), (sumYSecond / counter));
    second.setRadius((radiusInt / counter));
    m_dSubScrews.push_back(first);
    m_dSubScrews.push_back(second);
}
