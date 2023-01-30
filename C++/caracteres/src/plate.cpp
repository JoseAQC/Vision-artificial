#include <cstdlib>
#include <plate.h>

using namespace std;

/** 
 * Detects a car plate in the image
 * @param IN imgIn: Image of type CV_8UC1 where the function should find the plate. 
 * @param OUT position: A vector of rectangles. Each rectangle stores the position of the plate. 
 *
 * @return int value is returned. -1 if an error occurs, 0 otherwise.
 **/
int detectPlate(cv::Mat imgIn, vector<cv::Rect> *position) {
    int ret = 0;
    const bool SHOW_IMAGES = false;
    
    //threshold the original image
    cv::Mat imgThres;
    //80 threshold para las tres primeras imágenes
    //150 threshold para la imagen con dos coches 
    double thres = 150;
    cv::threshold(imgIn, imgThres, thres, 255, CV_THRESH_BINARY);
        
    //detect edges using Canny method
    cv::Mat imgCanny;
    //20 100
    double thresLow = 20; 
    double thresHigh = 100;
    int apertureSize = 5;
    //5
    cv::Canny(imgThres, imgCanny, thresLow, thresHigh, apertureSize);
    
    if (SHOW_IMAGES) {
        cv::imshow("threshold", imgThres);        
        cv::imshow("canny", imgCanny);
        cv::waitKey(0);
    }
    
    //find rectangular contours
    //each contour is a vector of points (vector<cv::Point>) and all the contours are stored in another vector
    vector<vector<cv::Point> > contours;
    //this vector stores the relationship between contours
    vector<cv::Vec4i> hierarchy;
    //constraints to select the contour of the plate
    const double MIN_AREA = 800;
    const double MAX_AREA = 8000;
    const double RATIO_MIN = 1.5;
    const double RATIO_MAX = 4.0;
    const double MIN_OCCUPANCY = 0.70;
    
    cv::Mat imgContours;//image to paint the contours
    cv::Scalar color = CV_RGB(255, 0, 0);//color of the contours
    cv::cvtColor(imgIn, imgContours, cv::COLOR_GRAY2RGB);//the image should be a 3 channel image
    //find only the external contours of the canny image
    cv::findContours(imgCanny, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    for (unsigned int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours.at(i));
        cv::Rect boundRect = cv::boundingRect(contours.at(i));
        double ratioRect = (double)boundRect.width / (double)boundRect.height;        
        double occupancy = area / (double)(boundRect.width * boundRect.height);

        if (area >= MIN_AREA && area <= MAX_AREA &&
            ratioRect >= RATIO_MIN && ratioRect <= RATIO_MAX &&
            occupancy >= MIN_OCCUPANCY){
            
            position->push_back(boundRect);            
            if (SHOW_IMAGES) {         
                printf("contour[%d].ratio(%.3lf).area(%.0lf).occupancy(%.3lf)\n",i,ratioRect,area,occupancy);
                cv::drawContours(imgContours, contours, i, color, 1);
                cv::imshow("selected contours", imgContours);
            }
        }
    }

    return ret;
}

