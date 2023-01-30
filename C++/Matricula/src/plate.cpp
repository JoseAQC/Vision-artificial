#include <cstdlib>
#include <plate.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

int detectPlate(cv::Mat imgIn, vector<cv::Rect> *position) {
    int ret = 0;
    cv::Mat imagenColor;
    

    //Detectar bordes usando Canny 
    cv::Mat imgCanny;

    //60 90 Umbrales para fotos coches pequeños
    //200 230 Umbrales para foto real
    double thresLow = 200; 
    double thresHigh =230; 
    int aperture = 3;

    // llamar funcion canny
    cv::Canny(imgIn, imgCanny, thresLow,thresHigh,aperture);
    cv::rectangle(imgCanny, cv::Rect(0,0,imgIn.cols, imgIn.rows),  CV_RGB(0,0,0),8 ,8,0 );

    // mostrar resultado de la deteccion de bordes 
    cv::imshow("Imagen Canny",imgCanny);

    // encontrar contornos
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
    findContours(imgCanny, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    printf("%d contornos detectados en la imagen \n", contours.size());
   
    // dibujar todos los contornos en verde en imagen de color copia de la original y mostrarlos
    cv::cvtColor(imgCanny,imagenColor, CV_BayerGR2RGB);
    for( int i = 0; i< contours.size(); i++ ) {// parámetros de entrada: imagen de 3 canales, vector de contornos detectados (vector de 	vectores de puntos), índice para cada contorno, color con escalar CV_RGB(), grosor 2, y tipo de línea 8cv::drawContours(...)
    	cv::Scalar color( 0, 255, 0 );
    	drawContours(imagenColor, contours, i, color,2,8,hierarchy);
	}
    cv::imshow("Contornos detectados",imagenColor);
    
    // umbrales para analisis de contornos
    //200 350 1.5
    const double MIN_LENGTH = 200;
    const double MIN_AREA = 350;
    const double MAX_RECTANGULARITY =1.5;

    // analizar todos los contornos, calculando perimetro, area y rectangularidad y aplicando restricciones
    for( int i = 0; i< contours.size(); i++ ){
	double length = cv::arcLength(contours.at(i), true);
	double area = cv::contourArea(contours.at(i));
		
	cv::RotatedRect minRect = cv::minAreaRect(contours.at(i));
	cv::Rect boundRect = cv::boundingRect(contours.at(i));
        
	double area1=minRect.size.height*minRect.size.width;
	double area2=boundRect.height*boundRect.width;
		
	double rectangularity1 = area1/area;
	double rectangularity2 = area2/area;

	
	if ((length>MIN_LENGTH)&&(area>MIN_AREA)&&(rectangularity1<MAX_RECTANGULARITY)&&(rectangularity2<MAX_RECTANGULARITY)){
        position->push_back(boundRect);
	
	printf("Altura del rectángulo igual a %d \n", boundRect.height);
	printf("Ancho del rectángulo igual a %d \n", boundRect.width);
	printf("Posición x igual a %d \n", boundRect.x);
	printf("Posición y igual a %d \n", boundRect.y);


	printf("Rectangularidad 1 es igual a %f \n", rectangularity1);
	printf("Rectangularidad 2 es igual a %f \n", rectangularity2);

     	
    
      
}
 }   
	

	
    return ret;
}

