#include "scalepixel.h"

ScalePixel::ScalePixel() {
    RADIUS_OUTER_CIRCLE_PIXEL = 0;
    DISTANCE_SCREWS_DSUB_PIXEL = 0;
    DISTANCE_SCREWS_INOUT_PIXEL = 0;
    DISTANCE_SCREWS_DSUB_INOUT_PIXEL = 0;
}

ScalePixel::~ScalePixel() {
}

void ScalePixel::calculateMmInPixels(int radius) {
    RADIUS_OUTER_CIRCLE_PIXEL = radius;
    DISTANCE_SCREWS_DSUB_PIXEL = DISTANCE_SCREWS_DSUB_MM * (RADIUS_OUTER_CIRCLE_PIXEL / RADIUS_OUTER_CIRCLE_MM);
    DISTANCE_SCREWS_INOUT_PIXEL = DISTANCE_SCREWS_INOUT_MM * (RADIUS_OUTER_CIRCLE_PIXEL / RADIUS_OUTER_CIRCLE_MM);
    DISTANCE_SCREWS_DSUB_INOUT_PIXEL = DISTANCE_SCREWS_DSUB_INOUT_MM * (RADIUS_OUTER_CIRCLE_PIXEL / RADIUS_OUTER_CIRCLE_MM);
}

// Get radius in pixel
int ScalePixel::getRadiusInPixel() {
    return RADIUS_OUTER_CIRCLE_PIXEL;
}

// Get distance between the dsub screws in pixel
int ScalePixel::getDistanceScrewsDsubInPixel() {
    return DISTANCE_SCREWS_DSUB_PIXEL;
}

// Get distance between the "in" and "out" screws in pixel
int ScalePixel::getDistanceScrewsInOutInPixel() {
    return DISTANCE_SCREWS_INOUT_PIXEL;
}

// Get distance between the "in" and dsub screws in pixel
int ScalePixel::getDistanceScrewsInDsubInPixel() {
    return DISTANCE_SCREWS_DSUB_INOUT_PIXEL;
}
