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

    // Get radius in pixel
    int getRadiusInPixel();

    // Get distance between the dsub screws in pixel
    int getDistanceScrewsDsubInPixel();

    // Get distance between the "in" and "out" screws in pixel
    int getDistanceScrewsInOutInPixel();

    // Get distance between the "in" and dsub screws in pixel
    int getDistanceScrewsInDsubInPixel();

private:
    int RADIUS_OUTER_CIRCLE_MM = 40;
    int DISTANCE_SCREWS_DSUB_MM = 33;
    int DISTANCE_SCREWS_INOUT_MM = 45;
    int DISTANCE_SCREWS_DSUB_INOUT_MM = 18;

    int RADIUS_OUTER_CIRCLE_PIXEL;
    int DISTANCE_SCREWS_DSUB_PIXEL;
    int DISTANCE_SCREWS_INOUT_PIXEL;
    int DISTANCE_SCREWS_DSUB_INOUT_PIXEL;
};

#endif // SCALEPIXEL_H
