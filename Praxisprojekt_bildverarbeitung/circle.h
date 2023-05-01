#ifndef CIRCLE_H
#define CIRCLE_H

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

    // Print the vector
    void printCircle();

private:
    int m_radius;
    int m_xCoordinateCenter;
    int m_yCoordinateCenter;
};

#endif // CIRCLE_H
