#include <cstdlib>
#include <plate.h>
#include <plateCharacters.h>

using namespace std;

int main(int argc, char** argv) {
    int ret = 0;

    //check input parameters
    if (argc != 2) {
        printf("\n Error. Type the name of the image\n");
        printf("\n      Example: ./caracteres ./dat/coche.png\n");
        ret = -1;
    } else {

        cv::Scalar colorPlate = CV_RGB(0, 255, 0); //color for the plate
        cv::Scalar colorChar = CV_RGB(255, 0, 0); //color for the numbers

        cv::Mat imgCoche = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
        cv::Mat imagenROI;

        if (imgCoche.empty()) {
            printf("Error reading image %s\n", argv[1]);
            ret = -1;
        } else {

            cv::Mat imgColor;
            cv::cvtColor(imgCoche, imgColor, cv::COLOR_GRAY2RGB);

            //detect the car plate
            vector<cv::Rect> plates;
            detectPlate(imgCoche, &plates);

            //draw each plate found
            for (unsigned int i = 0; i < plates.size(); i++) {
                cv::rectangle(imgColor, plates.at(i), colorPlate, 1);

                vector<cv::Rect> charPos;
                vector<cv::Mat> charImages;
                
                detectCharacters(imgCoche, plates.at(i), &charPos, &charImages);
                printf("plate %d/%d. %d characters detected\n", i + 1, plates.size(), charPos.size());

		// Dibujamos cada car�cter detectado en la imagen original
                for(unsigned int j=0; j<charPos.size();j++){
                cv::rectangle(imgColor, charPos.at(j), colorChar, 1);
                }
            }
            //save the results            
            cv::imwrite("./dat/results.png", imgColor);
            cv::imshow("Resultado final", imgColor);
            cv::waitKey(0);
        }
    }

    return ret;
}
