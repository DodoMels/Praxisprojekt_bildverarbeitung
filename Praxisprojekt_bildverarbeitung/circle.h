#ifndef CIRCLE_H
#define CIRCLE_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>

class Circle
{
public:
    // Constructor
    explicit Circle();

    // Destructor
    virtual ~Circle();

    // Set the Radius
    void setRadius(int radius);
    // Get the Radius
    int getRadius();

    // Set the midpoint
    void setCenter(int xCoordinate, int yCoordinate);
    // Get the midpoint
    std::vector<int> getCenter();

    // Find the two D-Sub Screws
    //void findDSubScrews();

    // Get the D-Sub Screws
    std::vector<Circle> getDSubScrews();

    // Print the vector
    void printCircle();

private:
    int m_radius;
    int m_xCoordinateCenter;
    int m_yCoordinateCenter;


    int m_exactDistanceDSubScrews;
    std::vector<Circle> m_dSubScrews;
};

#endif // CIRCLE_H
