#include <iostream>
#include "imageprocessing.h"

using namespace std;

int main()
{
    std::string pathImage = "/home/fhgr/Dokumente/Bildverarbeitung/1/Projekt_Bildverarbeitung_1/build-Projekt_Bildverarbeitung_1-Desktop-Debug/Sensor.jpg";
    ImageProcessing test;
    test.processImage();
    cv::waitKey(0);
    return 0;
}
