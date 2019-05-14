// OpenCV Hello-world
// Tadeusz Puźniakowski
//
//    g++ `pkg-config --cflags opencv` cv.cpp -o cv `pkg-config --libs opencv`
#include <cv.hpp>
#include <highgui.h>
#include <iostream>

int main( int argc, char** argv ) {
	using namespace cv;
	using namespace std;
	
	VideoCapture cap(2);
	
	vector<int>lower_b = {110,170,109};
	vector<int>upper_b = {132,255,255};
	
	
	namedWindow("regulacja");
	for (int i = 0; i < lower_b.size(); i++) {
		createTrackbar((string("low-") + to_string(i)).c_str(), "regulacja", &lower_b[i], 255, NULL);
	}
	for (int i = 0; i < lower_b.size(); i++) {
		createTrackbar((string("hi-") + to_string(i)).c_str(), "regulacja", &upper_b[i], 255, NULL);
	}
	do {
		Mat frame;

		if ( cap.read( frame ) ) {
			imshow( "A", frame );
			
			
			Mat hsvimg;
			cvtColor(frame, hsvimg, CV_RGB2HSV);
			imshow( "B", hsvimg );
			
			Mat selected;
			
			inRange(hsvimg, Scalar(lower_b[0],lower_b[1],lower_b[2]),Scalar(upper_b[0],upper_b[1],upper_b[2]), selected);
			imshow( "C", selected );
			
			int dilation_size = 3;
			Mat structelem = getStructuringElement( MORPH_ELLIPSE,
				   Size( 2*dilation_size + 1, 2*dilation_size+1 ),
					Point( dilation_size, dilation_size ) );
			Mat thresh2dil;
			dilate(selected, selected, structelem);
			imshow( "dilate", selected );

			erode(selected, selected, structelem);
			imshow( "erode", selected );

			erode(selected, selected, structelem);
			imshow( "erode", selected );
			dilate(selected, selected, structelem);
			imshow( "dilate", selected );
			
			

			
			/*
			Mat bwimage;
			cvtColor(frame, bwimage, CV_RGB2GRAY);
			imshow( "B", bwimage );
			
			
			Mat thresh;
			threshold(bwimage, thresh, 127,255,THRESH_BINARY);
			imshow( "C", thresh );
			
			Mat thresh2;
			adaptiveThreshold(bwimage, thresh2, 255,  ADAPTIVE_THRESH_GAUSSIAN_C, 
			THRESH_BINARY, 3, 2);
			imshow( "CD", thresh2 );
			int dilation_size = 1;
			Mat structelem = getStructuringElement( MORPH_ELLIPSE,
				   Size( 2*dilation_size + 1, 2*dilation_size+1 ),
					Point( dilation_size, dilation_size ) );
			Mat thresh2dil;
			erode(thresh, thresh2dil, structelem);
			imshow( "erode", thresh2dil );
			dilate(thresh2dil, thresh2dil, structelem);
			imshow( "dilate", thresh2dil );
			
			*/
		} else {
			break;
		}
	} while( (waitKey( 15 )&0x0ff) != 27 );
	return 0;
}
