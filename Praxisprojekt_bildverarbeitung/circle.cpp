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
/*
void Circle::findDSubScrews() {
    int startSecondForLoop = 0;
    int distanceCircleCenters = 0;
    int bestIndexFirstCircle = 0;
    int bestIndexSecondCircle = 0;
    int smallestDiffrence = 10000;
    for (size_t i = 0; i < m_extractedCircles.size(); i++) {
        startSecondForLoop++;
        std::vector<int> centerFirstCircle = m_extractedCircles.at(i).getCenter();
        for (size_t j = startSecondForLoop; j < m_extractedCircles.size(); j++) {
            std::vector<int> centerSecondCircle = m_extractedCircles.at(j).getCenter();
            distanceCircleCenters = sqrt(((centerFirstCircle.at(0) - centerSecondCircle.at(0)) * (centerFirstCircle.at(0) - centerSecondCircle.at(0))) + ((centerSecondCircle.at(1) - centerSecondCircle.at(1)) * (centerSecondCircle.at(1) - centerSecondCircle.at(1))));
            int difference = std::abs(m_exactDistanceDSubScrews - distanceCircleCenters);
            if (difference < smallestDiffrence) {
                //if (m_extractedCircles.at(j).getRadius() < (m_extractedCircles.at(i).getRadius() + 1) && (m_extractedCircles.at(i).getRadius() - 1) < m_extractedCircles.at(j).getRadius()) {
                if (m_extractedCircles.at(i).getRadius() == m_extractedCircles.at(j).getRadius()) {
                    smallestDiffrence = difference;
                    bestIndexFirstCircle = i;
                    bestIndexSecondCircle = j;
                }
            }
        }
    }
    m_dSubScrews.push_back(m_extractedCircles.at(bestIndexFirstCircle));
    m_dSubScrews.push_back(m_extractedCircles.at(bestIndexSecondCircle));
}*/

std::vector<Circle> Circle::getDSubScrews() {
    return m_dSubScrews;
}

void Circle::printCircle() {
    std::cout << "Radius: " << m_radius << "\nCenter: x = " << m_xCoordinateCenter << ", y = " << m_yCoordinateCenter << std::endl;
}
