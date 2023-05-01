#ifndef LINE_H
#define LINE_H

#include <vector>
#include <iostream>

class Line
{
public:
    // Constructor
    explicit Line();

    // Deconstructor
    virtual ~Line();

    // Set the startpoint
    void setStartpoint(int xCoordinate, int yCoordinate);
    // Get the startpoint
    std::vector<int> getStartpoint();

    // Set the endpoint
    void setEndpoint(int xCooridinate, int yCoordinate);
    // Get the endpoint
    std::vector<int> getEndpoint();

    // Print
    void printLine();

private:
    int m_xCoordinateStartpoint;
    int m_yCoordinateStartpoint;

    int m_xCoordinateEndpoint;
    int m_yCoordinateEndpoint;
};

#endif // LINE_H
