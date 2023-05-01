#include "imageprocessing.h"

ImageProcessing::ImageProcessing() {
}

ImageProcessing::~ImageProcessing() {
}

void ImageProcessing::processImage() {
    m_image.editImage("Webcam");
    this->findOuterCircle();
}

void ImageProcessing::findOuterCircle() {
    // a
    std::vector<cv::Vec3f> circles;
    int maxRadius = 250;
    int minRadius = 245;

    do {
    cv::HoughCircles(m_image.getSharpendGrayscaleImageGaussian(), circles, cv::HOUGH_GRADIENT, 1, m_image.getSharpendGrayscaleImageGaussian().rows/1, 100, 30, minRadius, maxRadius);
    maxRadius--;
    minRadius--;
    }
    while (circles.size() != 1);

    cv::Vec3i c = circles[0];
    std::cout << c[2] << std::endl;
    cv::Point center = cv::Point(c[0], c[1]);
    // circle center
    circle(m_image.getOriginalImage(), center, 1, cv::Scalar(0,100,100), 2, cv::LINE_AA);
    // circle outline
    int radius = c[2];
    circle(m_image.getOriginalImage(), center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);



    //for( size_t i = 0; i < circles.size(); i++ )
    //{
        //cv::Vec3i c = circles[i];
        //cv::Point center = cv::Point(c[0], c[1]);
        //// circle center
        //circle( m_imageOriginal, center, 1, cv::Scalar(0,100,100), 2, cv::LINE_AA);
        //// circle outline
        //int radius = c[2];
        //circle( m_imageOriginal, center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);
    //}
    imshow("detected circles", m_image.getOriginalImage());
    //cv::Mat thresh;
    //threshold(m_imageSharpendGrayGaussian, thresh, 150, 255, cv::THRESH_BINARY);
    //cv::imshow("Binary mage", thresh);
    //cv::waitKey(0);
}
