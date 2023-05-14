#include <iostream>
#include "imageprocessing.h"
#include "histogramm.h"


using namespace std;

int main()
{
    //std::string pathImage = "/home/fhgr/Dokumente/Bildverarbeitung/1/Projekt_Bildverarbeitung_1/build-Projekt_Bildverarbeitung_1-Desktop-Debug/Sensor.jpg";
    std::string pathImage = "C:/Users/domi002/Documents/6_Semester/Bildverarbeitung/Praxisprojekt/Praxisprojekt_bildverarbeitung/Praxisprojekt_bildverarbeitung/Profibus.jpg";
    //ImageProcessing test;
    //test.processImage();
    Histogramm test;
    test.calchist();
    cv::waitKey(0);
    return 0;
}
