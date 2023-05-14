#include "image.h"

Image::Image() {
}

Image::~Image() {
}

void Image::editImage(std::string path) {
    if (path == "WEBCAM") {
        this->captureImageFromWebcam(0);
    }
    this->importImage(path);
    this->convertImageToGrayscaleImage();
    this->sharpenImageGray();
    this->cannyFilter();
}

void Image::captureImageFromWebcam(int numberWebcam) {
    // Open connection to the webcam
    //m_captureWebcam.open(numberWebcam, cv::CAP_V4L);
    m_captureWebcam.open(numberWebcam, cv::CAP_ANY);

    // Check if the camera is open
    if (!m_captureWebcam.isOpened()) {
        throw std::runtime_error("Could not open the webcam 0!");
    }

    // Jump the first 25 frames to give the webcam time to focus
    for (int i = 0; i < 25; i++) {
        m_captureWebcam >> m_imageOriginal;
        cv::waitKey(35);
    }

    // Print the size of the image
    m_imageOriginalSize = m_imageOriginal.size();
    std::cout << "Size of the image: Width = " << m_imageOriginalSize.width << ", Height = " << m_imageOriginalSize.height << std::endl;
}

void Image::importImage(std::string path) {
    m_imageOriginal = cv::imread(path, cv::IMREAD_ANYCOLOR);
    cv::imshow("dhf", m_imageOriginal);
    cv::waitKey(0);
    m_imageOriginalSize = m_imageOriginal.size();
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
    //int downWidth = 600;
    //int downHeight = 800;
    //cv::Mat resizedDown;
    //cv::resize(image, resizedDown, cv::Size(downWidth, downHeight), cv::INTER_LINEAR);
    cv::imshow(title, image);
}
