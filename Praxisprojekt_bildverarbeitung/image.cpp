#include "image.h"

Image::Image() {
    m_flagScaleImageShow = false;
}

Image::~Image() {
}

void Image::setCaptureWebcam(cv::VideoCapture& captureWebcam) {
    m_captureWebcam = &captureWebcam;
}

void Image::editImage(std::string path) {
    if (path == "Webcam") {
        this->captureImageFromWebcam();
    }
    else {
        importImage(path);
        m_flagScaleImageShow = true;
    }
    this->convertImageToGrayscaleImage();
    this->sharpenImageGray();
    this->cannyFilter();
}

void Image::captureImageFromWebcam() {

    // Check if the camera is open
    if (!m_captureWebcam->isOpened()) {
        throw std::runtime_error("Could not open the webcam 0!");
    }
    *m_captureWebcam >> m_imageOriginal;

    // Get the size of the image
    m_imageOriginalSize = m_imageOriginal.size();
    //std::cout << "Size of the image: Width = " << m_imageOriginalSize.width << ", Height = " << m_imageOriginalSize.height << std::endl;
}

void Image::importImage(std::string path) {
    m_imageOriginal = cv::imread(path, cv::IMREAD_ANYCOLOR);
    m_imageOriginalSize = m_imageOriginal.size();

    // Print the size of the image
    m_imageOriginalSize = m_imageOriginal.size();
    std::cout << "Size of the image: Width = " << m_imageOriginalSize.width << ", Height = " << m_imageOriginalSize.height << std::endl;
}

void Image::convertImageToGrayscaleImage() {
    cv::cvtColor(m_imageOriginal, m_imageGrayscale, cv::COLOR_BGR2GRAY);
}

void Image::sharpenImageGray() {
    cv::GaussianBlur(m_imageGrayscale, m_imageSharpendGrayGaussian, cv::Size(0, 0), 3);
    cv::addWeighted(m_imageGrayscale, 1.5, m_imageSharpendGrayGaussian, -0.5, 0, m_imageSharpendGrayGaussian);

    // or, if the Mat_<float> is too weird for you, like this:
    //float kData[] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
    //cv::Mat kernel(3,3,CV_32F, kData);
    //cv::filter2D(m_imageSharpendGrayGaussian, m_imageSharpendGrayKernel, CV_32F, kernel);
}

void Image::cannyFilter() {
    cv::Canny(m_imageSharpendGrayGaussian, m_imageCannyFilter, 150, 500);
}

cv::Mat Image::getOriginalImage() {
    return m_imageOriginal;
}

cv::Mat Image::getGrayscaleImage() {
    return m_imageGrayscale;
}

cv::Mat Image::getSharpendGrayscaleImageGaussian() {
    return m_imageSharpendGrayGaussian;
}

cv::Mat Image::getSharpendGrayscaleImageKernel() {
    return m_imageSharpendGrayKernel;
}

cv::Mat Image::getCannyFilterImage() {
    return m_imageCannyFilter;
}

void Image::showImage(std::string title, cv::Mat image) {
    if (m_flagScaleImageShow) {
        //int downWidth = 600;
        //int downHeight = 800;
        //cv::Mat resizedDown;
        //cv::resize(image, resizedDown, cv::Size(downWidth, downHeight), cv::INTER_LINEAR);
        cv::imshow(title, image);
    }
    else {
        cv::imshow(title, image);
    }
}

void Image::showCircles(std::vector<Circle> circles) {
    for(size_t i = 0; i < circles.size(); i++ )
    {
        std::vector<int> circleExtracted = circles.at(i).getCenter();
        cv::Point center = cv::Point(circleExtracted[0], circleExtracted[1]);

        // circle center
        circle( this->getOriginalImage(), center, 1, cv::Scalar(0,100,100), 2, cv::LINE_AA);
        // circle outline
        int radius = circles.at(i).getRadius();
        circle( this->getOriginalImage(), center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);
    }

    //this->showImage("Picture", this->getOriginalImage());
}

void Image::showCircles(Circle circles) {
    std::vector<int> circleExtracted = circles.getCenter();
    cv::Point center = cv::Point(circleExtracted[0], circleExtracted[1]);
    // circle center
    circle( this->getOriginalImage(), center, 1, cv::Scalar(0,100,100), 2, cv::LINE_AA);
    // circle outline
    int radius = circles.getRadius();
    circle( this->getOriginalImage(), center, radius, cv::Scalar(255,0,255), 1, cv::LINE_AA);
    //this->showImage("Picture", this->getOriginalImage());
}
