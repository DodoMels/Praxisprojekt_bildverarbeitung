#ifndef SCALEPIXEL_H
#define SCALEPIXEL_H


class ScalePixel
{
public:

    // Constructor
    ScalePixel();

    // Deconstructor
    ~ScalePixel();

    // Calculate mm in pixels
    void calculateMmInPixels(int radius);

    // Get outer radius in pixel
    int getOuterRadiusInPixel();

    // Get inner radius in pixel
    int getInnerRadiusInPixel();

    // Get distance between the dsub screws in pixel
    int getDistanceScrewsDsubInPixel();

    // Get distance between the "in" and "out" screws in pixel
    int getDistanceScrewsInOutInPixel();

    // Get distance between the "in" and dsub screws in pixel
    int getDistanceScrewsInDsubInPixel();

    int getDistanceLidInPixel();

    int getDistanceConectorInPixel();

private:
    int RADIUS_OUTER_CIRCLE_MM = 40;
    int DISTANCE_SCREWS_DSUB_MM = 33;
    int DISTANCE_SCREWS_INOUT_MM = 45;
    int DISTANCE_SCREWS_DSUB_INOUT_MM = 18;
    int DISTANCE_LID_MM = 15;
    int DISTANCE_CONECTOR_MM = 11;

    int RADIUS_OUTER_CIRCLE_PIXEL;
    int RADIUS_INNER_CIRCLE_PIXEL;
    int DISTANCE_SCREWS_DSUB_PIXEL;
    int DISTANCE_SCREWS_INOUT_PIXEL;
    int DISTANCE_SCREWS_DSUB_INOUT_PIXEL;
    int DISTANCE_LID_PIXEL = 15;
    int DISTANCE_CONECTOR = 11;
};

#endif // SCALEPIXEL_H
