#include <plateCharacters.h>
#include <iostream>
using namespace std;

/** 
 * Detects the characters of the plate.
 * @param IN imgIn: Original image of the frame. Image of type CV_8UC1.
 * @param IN platePos: Position of the plate in the image. 
 * @param OUT characters: A vector with the positions of each character. 
 * @param OUT charImages: A vector with the images of the characters.
 *
 * @return int value is returned. -1 if an error occurs, 0 otherwise.
 **/
int detectCharacters(cv::Mat imgIn, cv::Rect platePos, vector<cv::Rect> *characters, vector<cv::Mat> *charImages) {
    int ret = 0;
    cv::Mat imagenROI;
    // Crear una imagen nueva con la region de la matricula
    imagenROI=imgIn(platePos);

    //Mostramos la imagen ROI obtenida
    cv::imshow("Imagen_ROI",imagenROI);
    cv::waitKey(0);

    // Umbralizar la imagen con umbral global y adaptativo (probar los 2)
    const double umbral = 50;
    const int blockSize = 15; // debe ser impar, 3, 5, 7 ...
    const double param1 = 2; // valor de C-media
    cv::Mat imgGlobalThres;
    cv::Mat imgAdaptThres;
    cv::Mat imgGlobalThres_filtro;
    cv::Mat imgAdaptThres_filtro;
    cv::threshold(imagenROI, imgGlobalThres, umbral, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cv::adaptiveThreshold(imagenROI, imgAdaptThres, 255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blockSize, param1);
    
    //Mostramos los resultados de las dos umbralizaciones
    cv::imshow("Umbralizacion_Global",imgGlobalThres);
    cv::imshow("Umbralizacion_Adaptativa",imgAdaptThres);
    cv::waitKey(0);
    
    // Llamar a la funcion removeNoise y mostramos los resultados 
    removeNoise(imgGlobalThres,&imgGlobalThres_filtro);
    cv::imshow("Imagen_filtrada",imgGlobalThres_filtro);
    cv::waitKey(0);
    removeNoise(imgAdaptThres,&imgAdaptThres_filtro);
    cv::imshow("Imagen_filtrada",imgAdaptThres_filtro);
    cv::waitKey(0);

    // Detectar los caracteres llamando a vertHorizProjection
    vertHorizProjection(imgGlobalThres_filtro, platePos, 2, 3, characters, charImages);
    

    return ret;
}

/** 
 * Threshold the plate image and removes noise in the image.
 * @param IN imgIn: Image of the plate with noise. Image of type CV_8UC1.
 * @param OUT imgOut: Image of the plate without noise. Image of type CV_8UC1.
 *
 * @return int value is returned. -1 if an error occurs, 0 otherwise.
 **/
int removeNoise(cv::Mat imgIn, cv::Mat *imgOut) {
    int ret = 0;

    // aplicar filtrados morfologicos tipo erode y/o dilate
    int kernelSize = 1;
    cv::Mat imgFiltered;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kernelSize, kernelSize));    
    // llamar a dilate o erode y visualizar resultados
    
    // eliminar el borde negro alrededor de la imagen
    cv::rectangle(imgIn, cv::Point(1,1), cv::Point(imgIn.cols-1,imgIn.rows-1), cv::Scalar(255), 4);
    

    *imgOut = imgIn;
    return ret;
}

/** 
 * Computes vertical and horizontal projections of the binary image to detect the position and size of the characters. 
 * @param IN imgIn: Image of the plate without noise. Image of type CV_8UC1.
 * @param IN roiPos: Position of the plate in the original image.
 * @param IN vertThres: Threshold of the vertical projection. 
 * @param IN horizThres: Threshold of the horizontal projection. 
 * @param OUT objects: Vector with the detected object positions. 
 * @param OUT vecImages: Vector with the detected object images.
 *
 * @return int value is returned. -1 if an error occurs, 0 otherwise.
 **/
int vertHorizProjection(cv::Mat imgIn, cv::Rect roiPos, int vertThres, int horizThres, vector<cv::Rect> *objects, vector<cv::Mat> *vecImages) {
    int ret = 0;
    const bool SHOW_GRAPH = false;

    objects->clear();
    int vTh = 255 * vertThres;
    int hTh = 255 * horizThres;

    //vertical and horizontal projections
    cv::Mat vertSum, horizSum;
    cv::reduce(imgIn, vertSum, 0, CV_REDUCE_SUM, CV_32FC1);
    cv::reduce(imgIn, horizSum, 1, CV_REDUCE_SUM, CV_32FC1);

    //compute maximum values to estimate the scale of the graph
    const int GRAPH_SIZE = 50;
    double minH, maxH, minV, maxV;
    cv::minMaxLoc(horizSum, &minH, &maxH);
    cv::minMaxLoc(vertSum, &minV, &maxV);
    double scaleH = (double) GRAPH_SIZE / maxH;
    double scaleV = (double) GRAPH_SIZE / maxV;

    //estimate position and height of objects
    //margin of pixels around the object
    int marginV = 2;
    int y, h;
    for (int i = 1; i < imgIn.rows; i++) {
        float val1 = horizSum.at<float>(i - 1, 0);
        float val2 = horizSum.at<float>(i, 0);

        if (val1 >= maxH - hTh && val2 < maxH - hTh) {
            y = i - marginV;
        } else {
            if (val1 <= maxH - hTh && val2 > maxH - hTh) {
                h = i - y + marginV;
            }
        }
    }

    //estimate position and width of objects
    //margin of pixels around the object
    int marginH = 2;
    int x = -1, w;
    for (int i = 1; i < imgIn.cols; i++) {
        float val1 = vertSum.at<float>(0, i - 1);
        float val2 = vertSum.at<float>(0, i);

        if (val1 >= maxV - vTh && val2 < maxV - vTh && x == -1) {
            x = i - marginH;
        } else {
            if (val1 <= maxV - vTh && val2 > maxV - vTh && x != -1) {
                w = i - x + marginH;
                objects->push_back(cv::Rect(roiPos.x + x, roiPos.y + y, w, h));
                cv::Mat imgTmp = imgIn(cv::Rect(x,y,w,h));
                vecImages->push_back(imgTmp);
                x = -1;
            }
        }
    }

    if (SHOW_GRAPH) {
        //paint vertical projection and vertical threshold
        cv::Mat imgVertical = cv::Mat::zeros(GRAPH_SIZE, imgIn.cols, CV_8UC3);
        for (int i = 1; i < vertSum.cols; i++) {
            float val1 = vertSum.at<float>(0, i - 1) * scaleV;
            float val2 = vertSum.at<float>(0, i) * scaleV;
            cv::line(imgVertical, cv::Point(i - 1, val1), cv::Point(i, val2), CV_RGB(0, 255, 0), 1);
        }
        cv::line(imgVertical, cv::Point(0, imgVertical.rows - vTh * scaleV), cv::Point(imgVertical.cols, imgVertical.rows - vTh * scaleV), CV_RGB(255, 0, 0), 1, 8, 0);
        cv::namedWindow(string("imgVertical"), CV_WINDOW_NORMAL);
        cv::imshow(string("imgVertical"), imgVertical);

        //paint horizontal projection and horizontal threshold
        cv::Mat imgHorizontal = cv::Mat::zeros(imgIn.rows, GRAPH_SIZE, CV_8UC3);
        for (int i = 1; i < horizSum.rows; i++) {
            float val1 = horizSum.at<float>(i - 1, 0) * scaleH;
            float val2 = horizSum.at<float>(i, 0) * scaleH;
            cv::line(imgHorizontal, cv::Point(val1, i - 1), cv::Point(val2, i), CV_RGB(0, 255, 0), 1);
        }
        cv::line(imgHorizontal, cv::Point(imgHorizontal.cols - hTh * scaleH, 0), cv::Point(imgHorizontal.cols - hTh * scaleH, imgHorizontal.rows), CV_RGB(255, 0, 0), 1, 8, 0);
        cv::namedWindow(string("imgHorizontal"), CV_WINDOW_NORMAL);
        cv::imshow(string("imgHorizontal"), imgHorizontal);
    }


    return ret;
}
