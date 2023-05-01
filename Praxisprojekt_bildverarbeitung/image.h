#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>

class Image
{
public:
    // Constructor
    explicit Image();

    // Destructor
    virtual ~Image();

    // Process image
    void editImage(std::string path);

    // Get image from a webcam
    void captureImageFromWebcam(int numberWebcam);

    // Import an image
    void importImage(std::string path);

    // Convert the image to a grayscale image
    void convertImageToGrayscaleImage();

    // Sharpen the grayscale image
    void sharpenImageGray();

    // Cannyfilter
    void cannyFilter();

    // Get the original image
    cv::Mat getOriginalImage();

    // Get the grayscale image
    cv::Mat getGrayscaleImage();

    // Get the grayscale image
    cv::Mat getSharpendGrayscaleImageGaussian();

    // Get the grayscale image
    cv::Mat getSharpendGrayscaleImageKernel();

    // Get the canny filter image
    cv::Mat getCannyFilterImage();

    // Show the image
    void showImage(std::string title, cv::Mat image);

private:
    cv::VideoCapture m_captureWebcam;

    cv::Mat m_imageOriginal;
    cv::Size m_imageOriginalSize;

    cv::Mat m_imageGrayscale;

    cv::Mat m_imageSharpendGrayGaussian;
    cv::Mat m_imageSharpendGrayKernel;

    cv::Mat m_imageCannyFilter;
};

#endif // IMAGE_H
