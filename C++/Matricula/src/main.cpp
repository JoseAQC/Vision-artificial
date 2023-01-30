#include <cstdlib>
#include <plate.h>



using namespace std;
int numero_contorno;
char ArchivoImagen[100];
int main(int argc, char** argv) {
    int ret = 0;
    cv::Mat imgColor;

    if (argc != 2) {
        printf("Example: ./matricula ./dat/coche1.png");
        ret = -1;}
    else {
        cv::Mat imgGrey = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	cv::cvtColor(imgGrey,imgColor, CV_BayerGR2RGB);
        if (imgGrey.empty()) {
            printf("Error reading image %s\n", argv[1]);
            ret = -1;
	    }
        else {           
            vector<cv::Rect> platePos;
	    // Detectar matricula. La función detectPlate debe declararse en ./include/plate.h y definirse en ./src/plate.cpp	
	    detectPlate(imgGrey, &platePos);
	 
            printf("%lu matricula/s detectada/s en la imagen %s\n", platePos.size(), argv[1]);
	    
	    // Dibujar rectángulos en color (en imagen imgColor que debe ser copia en color de imgGrey)
	
	
            for( int i = 0; i<platePos.size(); i++ )
     		{
      		cv::Scalar color2(255,255,255);
		cv::rectangle( imgColor, platePos.at(i), color2, 2, 8, 0 );
     		}

        }
      cv::imshow("Plates", imgColor);
      cv::waitKey(0);
      sprintf(ArchivoImagen, "./dat/Final.png");
      cv::imwrite(ArchivoImagen,imgColor);
    }

    return ret;
}
