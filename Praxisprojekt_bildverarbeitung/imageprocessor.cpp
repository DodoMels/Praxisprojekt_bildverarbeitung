#include "imageprocessor.h"

ImageProcessor::ImageProcessor(cv::VideoCapture& captureWebcam) {
    m_captureWebcam = &captureWebcam;
    m_image.setCaptureWebcam(*m_captureWebcam);
}

ImageProcessor::~ImageProcessor() {
}

std::vector<Circle> ImageProcessor::processImageToFindInnerCircles(std::string path) {
    m_image.editImage(path);
    this->findOuterCircle();
    this->findInnerCircles();
    return m_innerCircles;
}

void ImageProcessor::findOuterCircle() {
    // a
    std::vector<cv::Vec3f> circles;
    int maxRadius = MAX_RADIUS_POSSIBLE;
    int minRadius = maxRadius - 3;

    do {
    cv::HoughCircles(m_image.getGrayscaleImage(), circles, cv::HOUGH_GRADIENT, 1, m_image.getSharpendGrayscaleImageGaussian().rows/1, 20, 30, minRadius, maxRadius);
    maxRadius--;
    minRadius--;
    }
    while (circles.size() == 0);
    m_radiusOuterCirclePixel = circles[0][2];
    m_scalePixel.calculateMmInPixels(m_radiusOuterCirclePixel);
    m_exactDistanceDSubScrews = m_scalePixel.getDistanceScrewsDsubInPixel();

    cv::Vec3i c = circles[0];
    m_radiusOuterCirclePixel = c[2];
    cv::Point center = cv::Point(c[0], c[1]);
    m_centerOuterCircle.push_back(c[0]);
    m_centerOuterCircle.push_back(c[1]);
    // circle center
    circle(m_image.getOriginalImage(), center, 1, cv::Scalar(0,100,100), 2, cv::LINE_AA);
    // circle outline
    circle(m_image.getOriginalImage(), center, m_radiusOuterCirclePixel, cv::Scalar(255,0,255), 1, cv::LINE_AA);
}

void ImageProcessor::findInnerCircles() {
    int radiusInnerCirclePixel = m_scalePixel.getInnerRadiusInPixel();
    int radiusMax = radiusInnerCirclePixel + 2;
    int radiusMin = radiusInnerCirclePixel - 2;
    std::vector<cv::Vec3f> circles;
    //std::cout << "vor dem kreis suchen" << std::endl;
    cv::HoughCircles(m_image.getGrayscaleImage(), circles, cv::HOUGH_GRADIENT, 1, 15, 90, 15, radiusMin,radiusMax);
    //std::cout << "nach dem kreis suchen" << std::endl;
    m_innerCircles = extractCirclesFromCvVec3i(circles);
    //m_innerCircles.findDSubScrews();
    //std::vector<Circle> dSubCircles = m_innerCircles.getDSubScrews();
    //std::cout << "Effective distance :" << m_scalePixel.getDistanceScrewsDsubInPixel() << std::endl;

    /*for (size_t i = 0; i < dSubCircles.size(); i++) {
        std::vector<int> centerPoint = dSubCircles.at(i).getCenter();
        cv::Point center = cv::Point(centerPoint.at(0), centerPoint.at(1));
        // circle center
        circle( m_image.getOriginalImage(), center, 1, cv::Scalar(0,100,100), 2, cv::LINE_AA);
        // circle outline
        int radius = dSubCircles.at(i).getRadius();
        circle( m_image.getOriginalImage(), center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);
    }*/

    // Show all found inner circles
    /*for(size_t i = 0; i < circles.size(); i++ )
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);

        // circle center
        circle( m_image.getOriginalImage(), center, 1, cv::Scalar(0,100,100), 2, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        circle( m_image.getOriginalImage(), center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);
    }

    m_image.showImage("detected circles", m_image.getOriginalImage());*/
}

std::vector<Circle> ImageProcessor::extractCirclesFromCvVec3i(std::vector<cv::Vec3f> circles) {
    std::vector<Circle> extractedCircles;
    for (size_t i = 0; i < circles.size(); i++) {
        Circle tempCircle;
        tempCircle.setCenter(circles[i][0], circles[i][1]);
        tempCircle.setRadius(circles[i][2]);
        extractedCircles.push_back(tempCircle);
    }
    return extractedCircles;
    //std::cout << m_extractedCircles.size() << std::endl;
}

void ImageProcessor::showTheFoundCircles(std::vector<Circle> circles) {
    for(size_t i = 0; i < circles.size(); i++ )
    {
        std::vector<int> circleExtracted = circles.at(i).getCenter();
        cv::Point center = cv::Point(circleExtracted[0], circleExtracted[1]);

        // circle center
        circle( m_image.getOriginalImage(), center, 1, cv::Scalar(0,100,100), 2, cv::LINE_AA);
        // circle outline
        int radius = circles.at(i).getRadius();
        circle( m_image.getOriginalImage(), center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);
    }

    m_image.showImage("detected circles", m_image.getOriginalImage());
}

int ImageProcessor::getRadiusOuterCircle() {
    return m_radiusOuterCirclePixel;
}

int ImageProcessor::getDistanceDSub() {
    return m_exactDistanceDSubScrews;
}

std::vector<int> ImageProcessor::getCenterOuterCircle() {
    return m_centerOuterCircle;
}
