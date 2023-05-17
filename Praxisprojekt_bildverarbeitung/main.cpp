#include <iostream>
#include "imagehandler.h"

using namespace std;

int main()
{
    std::string pathImage = "/home/fhgr/Dokumente/Bildverarbeitung/1/Projekt_Bildverarbeitung_1/build-Projekt_Bildverarbeitung_1-Desktop-Debug/Sensor.jpg";

    cv::VideoCapture captureWebcam;
    captureWebcam.open(0, cv::CAP_V4L);
    Image temp;
    temp.setCaptureWebcam(captureWebcam);
    for (size_t i = 0; i < 30; i++) {
        temp.captureImageFromWebcam();
    }
    std::cout << "test" << std::endl;
    for (int i = 0; i < 1000; i++) {
        ImageHandler test(captureWebcam);
        test.evaluateSensor("Webcam");
        cv::waitKey(30);
    }
    cv::waitKey(0);
    return 0;
}
