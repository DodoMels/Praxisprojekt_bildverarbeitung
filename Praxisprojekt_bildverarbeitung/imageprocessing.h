#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "image.h"
#include "circle.h"
#include "line.h"

class ImageProcessing
{
public:
    // Constructor
    explicit ImageProcessing();

    // Deconstructor
    virtual ~ImageProcessing();

    // Process the Image
    void processImage();

    // Find outer circle
    void findOuterCircle();
private:
    int ITERATIONS_OUTERCIRCLE = 10;
    int MAX_LENGHT_IMAGE = 640;
    int MAX_RADIUS_POSSIBLE = (MAX_LENGHT_IMAGE / 2);
    Image m_image;
    Circle m_outerCircle;
    Circle m_innerCircles;
    Line m_lineInnerCircles;
};

#endif // IMAGEPROCESSING_H
