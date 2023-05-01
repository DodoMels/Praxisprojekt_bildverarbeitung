#include "circle.h"

Circle::Circle() {
    // Set the default values
    m_radius = 0;
    m_xCoordinateCenter = 0;
    m_yCoordinateCenter = 0;
}

Circle::~Circle() {
}

void Circle::setRadius(int radius) {
    m_radius = radius;
}

int Circle::getRadius() {
    return m_radius;
}

void Circle::setCenter(int xCoordinate, int yCoordinate) {
    m_xCoordinateCenter = xCoordinate;
    m_yCoordinateCenter = yCoordinate;
}

std::vector<int> Circle::getCenter() {
    std::vector<int> midpoint{m_xCoordinateCenter, m_yCoordinateCenter};
    return midpoint;
}

void Circle::printCircle() {
    std::cout << "Radius: " << m_radius << "\nCenter: x = " << m_xCoordinateCenter << ", y = " << m_yCoordinateCenter << std::endl;
}
