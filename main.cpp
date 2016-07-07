#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "libgfwx/gfwx.hpp"

using namespace cv;

int main( int argc, char** argv ){ 
    char* imageName = argv[1];

    Mat image;
    image = imread( imageName, 1 );

    if( argc != 2 || !image.data ){
        printf( " No image data \n " );
        return -1;
    }

    Mat gray_image;
    cvtColor( image, gray_image, CV_BGR2GRAY );

    imwrite( "img/Gray_Image.jpg", gray_image );

    GFWX::Image<uint8_t> img(image.data, 5472, 3080);

    return 0;
    
}