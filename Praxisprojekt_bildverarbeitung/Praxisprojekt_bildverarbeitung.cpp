#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

#include <string>
#include <array>
#include <algorithm>

using namespace cv;
using namespace std;

// function read image
Mat einlesen(string Dateiname)
{
    Mat img = imread(Dateiname, cv::IMREAD_ANYCOLOR);
    if (!img.data)
    {
        cout << "Unable to read image" << endl;
    }
    imshow("Original", img);
    waitKey(0);

    return img;
}

// function detect edges
Mat detect_edges(Mat img)
{
    Mat edges;
    Canny(img, edges, 100, 200);
    namedWindow("Kanten", WINDOW_NORMAL);
    imshow("Kanten", edges);
    waitKey(0);

    return edges;
}

// function highpassfilter
Mat highpassfilter(Mat img)
{
    Mat gray;
    Mat dst;
    Mat dst1; 
    Mat dst2;
    int ddepth = -1;                        //Datentyp Ergebnis-Bild = Datentyp Eingangs-Bild
    Point anchor = Point(-1, -1);   //relative Position des Filters zum berechneten Pixel.
    double delta = 0;                       //Grauwert-Offset (addiert zum Ergebnis-Bild)
    int borderType = BORDER_CONSTANT;   //Wie soll am Rand verfahren werden, CONST = �berall konstanter Wert 0
    Mat kernel; //Kernel-Matrix
    Mat kernel1;
    Mat kernel2;
    Mat cdst;
    Mat cdstP;

    cvtColor(img, gray, COLOR_BGR2GRAY);

    //Hochpassfilter
    kernel = (Mat_<float>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0); // 1,1,1,1,-8,1,1,1,1 laplace mit diagonalen auch ber�cksichtigt 
    filter2D(gray, dst, ddepth, kernel, anchor, delta, borderType);
    imshow("Hochpassfilter", dst);
    Canny(img, dst2, 50, 200, 3);
    // detect cicles
    vector<Vec3f> circles;
    HoughCircles(dst2, circles, HOUGH_GRADIENT, 1, dst2.rows/8, 100, 30, 5, 30);
    cout << "No. of circles : " << circles.size() << endl;
    // Draw the circles detected
    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);// circle center     
        circle(img, center, radius, Scalar(0, 0, 255), 3, 8, 0);// circle outline
        cout << "center : " << center << "\nradius : " << radius << endl;
    }
    /************************************************************************************************/
    // detect lines
    Canny(img, dst1, 50, 200, 3);
    imshow("canny", dst1);
    cvtColor(dst1, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();
    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(dst1, lines, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection
    // Draw the lines
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
    }
    // Probabilistic Line Transform
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(dst1, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection
    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++)
    {
        Vec4i l = linesP[i];
        line(cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
    }
    // Show results
    //imshow("Source", src);
    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
    namedWindow("Hough Circle Transform Demo", WINDOW_AUTOSIZE);
    imshow("Hough Circle Transform Demo", img);

    waitKey(0);

    return dst;
}

// function detect geometry

int main(int argc, char* argv[])
{
    Mat img;
    Mat edges;
    Mat filter;
    string search_directory = "../Praxisprojekt_bildverarbeitung/D_sub15.jpg";

    if (argc > 1)
    {
        search_directory = argv[1];
    }
    cout << "Press ENTER to continue." << endl;
    img = einlesen(search_directory);
    //edges = detect_edges(img);
    filter = highpassfilter(img);
    
    return 0;
}