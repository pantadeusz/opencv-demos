// OpenCV Hello-world
// Tadeusz Puźniakowski
//
//    g++ `pkg-config --cflags opencv` cv.cpp -o cv `pkg-config --libs opencv`
#include <cv.hpp>
#include <highgui.h>
#include <iostream>
#include <map>

int main( int argc, char** argv ) {
	using namespace cv;
	using namespace std;
	
	VideoCapture cap(0);
	
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
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			findContours( selected, contours,hierarchy , CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
			for (int i = 0; i < contours.size(); i++) drawContours(frame, contours, i, Scalar(0,255,0) ,2);
			imshow( "cont", frame );
			
			for (auto &contour: contours) {
				Moments m = moments(contour);
				cout << "( " << (m.m10/m.m00) << ", " << (m.m01/m.m00) << ") "  ;
			}
			cout << endl;

			
		} else {
			break;
		}
	} while( (waitKey( 15 )&0x0ff) != 27 );
	return 0;
}
