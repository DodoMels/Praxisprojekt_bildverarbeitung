#include "line.h"

Line::Line() {
    // Set de default values
    m_xCoordinateStartpoint = 0;
    m_yCoordinateStartpoint = 0;

    m_xCoordinateEndpoint = 0;
    m_yCoordinateEndpoint = 0;
}

Line::~Line() {
}

void Line::setStartpoint(int xCoordinate, int yCoordinate) {
    m_xCoordinateStartpoint = xCoordinate;
    m_yCoordinateStartpoint = yCoordinate;
}

std::vector<int> Line::getStartpoint() {
    std::vector<int> startpoint{m_xCoordinateStartpoint, m_yCoordinateStartpoint};
    return startpoint;
}

void Line::setEndpoint(int xCooridinate, int yCoordinate) {
    m_xCoordinateEndpoint = xCooridinate;
    m_yCoordinateEndpoint = yCoordinate;
}

std::vector<int> Line::getEndpoint() {
    std::vector<int> endpoint{m_xCoordinateEndpoint, m_yCoordinateEndpoint};
    return endpoint;
}

void Line::printLine() {
    std::cout << "Startpoint: x = " << m_xCoordinateStartpoint << ", y = " << m_yCoordinateStartpoint << std::endl;
    std::cout << "Endpoint: x = " << m_xCoordinateEndpoint << ", y = " << m_yCoordinateEndpoint << std::endl;
}
