#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <array>
#include <algorithm>
#include <math.h>
#include <stdio.h>
/*
// TODO:
find contours funktion testen 
nach canny filter in x und y achse erstes weisses Pixel suchen um durchmesser bestimmen zukönnen
danach grossen Kreis suchen 
kleine kreise suche in abhängigkeit von grossem Durchmesser
für D-Sub 15 linie durch schraube ziehen und verlängern und mitels Histogramm die beiden äusserenschrauben finden Plus
die Kreise erkennen doppelt gemopelt 


*/
using namespace cv;
using namespace std;
std::vector<cv::Vec3b> values;


// function read image
Mat einlesen(string Dateiname)
{
    Mat img = imread(Dateiname, cv::IMREAD_ANYCOLOR);
    if (!img.data)
    {
        cout << "Unable to read image" << endl;
    }
    //imshow("Original", img);
    //waitKey(0);

    return img;
}

// function detect edges
Mat detect_edges(Mat img)
{
    Mat edges;
    Canny(img, edges, 100, 200);
    //namedWindow("Kanten", WINDOW_NORMAL);
    //imshow("Kanten", edges);
    //waitKey(0);

    return edges;
}

// function highpassfilter
Mat highpassfilter(Mat img)
{
    
    Mat bild = img;
    Mat gray;
    Mat dst;        // Hochpassfilter
    Mat dst1;       // Cannyfilter
    Mat dst2;       // Cannyfilter + Hochpass
    int ddepth = -1;                        //Datentyp Ergebnis-Bild = Datentyp Eingangs-Bild
    Point anchor = Point(-1, -1);   //relative Position des Filters zum berechneten Pixel.
    double delta = 0;                       //Grauwert-Offset (addiert zum Ergebnis-Bild)
    int borderType = BORDER_CONSTANT;   //Wie soll am Rand verfahren werden, CONST = überall konstanter Wert 0
    Mat kernel; 
    Mat kernel1;
    Mat kernel2;
    Mat cdst;
    Mat cdstP;

    cvtColor(img, gray, COLOR_BGR2GRAY);
    //imshow("gray", gray);

    /*******************************************************************************************************************************/
    //filtertypes
    /*******************************************************************************************************************************/

    //Hochpassfilter
    kernel = (Mat_<float>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0); // 1,1,1,1,-8,1,1,1,1 laplace mit diagonalen auch berücksichtigt 
    filter2D(gray, dst, ddepth, kernel, anchor, delta, borderType);
    //imshow("Hochpassfilter", dst);

    //Canny filter black white image
    Canny(img, dst1, 50, 200, 3);
    imshow("just canny", dst1);

    // canny filter + hochpass bad solution
    Canny(dst, dst2, 50, 200, 3);
    //imshow("hochpass+canny", dst2);

    /*******************************************************************************************************************************/
    /*******************************************************************************************************************************/

    // detect cicles
    /*******************************************************************************************************************************/
    vector<Vec3f> circles;
   HoughCircles(dst1, circles, HOUGH_GRADIENT, 1, dst1.rows / static_cast<double>(8), 100, 36, 5, 30);
    //HoughCircles(dst1, circles, HOUGH_GRADIENT, 1, 50, 100, 30, 5, 35);
    //HoughCircles(dst1, circles, HOUGH_GRADIENT, 1, dst1.rows / static_cast<double>(18), 200, 36, 250, 400); // detect the biggest circle

    cout << "No. of circles : " << circles.size() << endl;
    // Draw the circles detected
    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);// circle center     
        circle(img, center, radius, Scalar(0, 0, 255), 3, 8, 0);// circle outline
        for (size_t j = i + 1; j < circles.size(); j++)
        {
            Point center1(cvRound(circles[i][0]), cvRound(circles[i][1]));
            Point center2(cvRound(circles[j][0]), cvRound(circles[j][1]));
            float distance = sqrt(abs(center2.x - center1.x)^2 + abs(center2.y - center1.y)^2);
            if (distance > 20 && distance < 22)
            {
                cout << "true" << center1 << center2 << distance << endl;
                line(img, center1, center2, Scalar(0, 0, 255), 1, LINE_8, 0);
                int intensity = (int)img.at<uchar>(center1.x, center2.y);
                cout << "inten"<<intensity << endl;
                
                LineIterator it(img, center1, center2, 8);
                for (int i = 0; i < it.count; i++, ++it)
                {
                    int intensity[100];
                   Point pt = it.pos();
                   int intens = (int)img.at<uchar>(pt.x, pt.y);
                   intensity[i] = intens;
                   cout << intensity[i] << "int" << endl;
                }
                

                //values.push_back((Vec3b)*it);


                
            }
            else
            {
                cout << "false" << center1 << center2 << distance << endl;
            }
        }
        

        cout << "center : " << center << "\nradius : " << radius << endl;
    }
    imshow("Hough Circle Transform Demo", img);
    waitKey(0);
    /***********************************************************************************************************/
    //Test
    /*
    Point2f srcTri[3];
    Point2f dstTri[3];
    string search_directory = "../Praxisprojekt_bildverarbeitung/Profibus.jpg";
    Mat rot_mat(2, 3, CV_32FC1);
    Mat warp_mat(2, 3, CV_32FC1);
    Mat src, warp_dst, warp_rotate_dst;
    src = einlesen(search_directory);
    warp_dst = Mat::zeros(src.rows, src.cols, src.type());
    srcTri[0] = Point2f(0, 0);
    srcTri[1] = Point2f(src.cols - 1, 0);
    srcTri[2] = Point2f(0, src.rows - 1);

    dstTri[0] = Point2f(src.cols * 0.0, src.rows * 0.33);
    dstTri[1] = Point2f(src.cols * 0.85, src.rows * 0.25);
    dstTri[2] = Point2f(src.cols * 0.15, src.rows * 0.7);
    warp_mat = getAffineTransform(srcTri, dstTri);
    warpAffine(src, warp_dst, warp_mat, warp_dst.size());
    Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
    double angle = -50.0;
    double scale = 0.6;
    rot_mat = getRotationMatrix2D(center, angle, scale);

    /// Rotate the warped image
    warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());

    /// Show what you got
    
    imshow("source_window", src);

 
    imshow("warp_window", warp_dst);

   
    imshow("warp_rotate_window", warp_rotate_dst);

    /// Wait until user exits the program
    waitKey(0);
    */
    /*******************************************************************************************************************************/
       
    /*Point center1(cvRound(circles[0][0]), cvRound(circles[0][1]));
    Point center2(cvRound(circles[1][0]), cvRound(circles[1][1]));
    float distance = sqrt((center2.x - center1.x)^2 + (center2.y - center1.y)^2);
    cout << "line1" << center1 << endl;
    cout << "line2" << center2 << endl;
    cout << "distance betwenn" << center1 << "&"<< center2 << "="<< distance << endl;
    line(img, center1, center2, Scalar(0, 0, 255), 1, LINE_8, 0);
    imshow("Hough Circle Transform Demo", img);
    waitKey(0);
    */

        // kommentar für dodo
        // innere schrauben Distanz gleich 15
        // äussere schraube = 21
        // start und enpixel mit hilfe von Roi +- 2 Pixel ausschneiden und danach hist berechnen 
    
    /*******************************************************************************************************************************/

    // detect lines
    /*******************************************************************************************************************************/
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
    //imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    //imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
    namedWindow("Hough Circle Transform Demo", WINDOW_AUTOSIZE);
    imshow("Hough Circle Transform Demo", img);

    waitKey(0);

    return dst;
}

// function detect geometry

int main(int argc, char* argv[])
{
    Mat img;
    Mat bild;
    Mat edges;
    Mat filter;
    string search_directory = "../Praxisprojekt_bildverarbeitung/Device_net_mit_abdeckung.jpg";

    if (argc > 1)
    {
        search_directory = argv[1];
    }
    cout << "Press ENTER to continue." << endl;
    img = einlesen(search_directory);
    bild = einlesen(search_directory);
    edges = detect_edges(img);
    filter = highpassfilter(img);
    
    return 0;
}