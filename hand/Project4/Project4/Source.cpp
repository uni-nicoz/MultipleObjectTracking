#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <list>
using namespace cv;
using namespace std;
#define HEIGHT 3000
#define WIDTH 3000
#define DEPTH 3

void show();

void avg(Mat& newFrame);

void check(Mat& image, Mat& image0);

int myMax(int a, int b, int c);

int myMin(int a, int b, int c);

void mySkinDetect(Mat& src, Mat& dst);

void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst);

void myMotionEnergy(vector<Mat> mh, Mat& dst);

int main() {
	
	namedWindow("test", WINDOW_AUTOSIZE);
	namedWindow("test0", WINDOW_AUTOSIZE);
	Mat frame = imread("CS585-PianoImages/piano_23.png");
	Mat frame0 = imread("CS585-PianoImages/piano_14.png");
	Mat frameDest;
	Mat frameDest0;
	frameDest = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
	frameDest0 = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);
	mySkinDetect(frame, frameDest);
	mySkinDetect(frame0, frameDest0);
	check(frameDest, frame);
	imshow("test0", frame);
	imshow("test",frameDest);
	//imshow("test", frame);
	//imshow("test0", frame0);
	
	//show();
	waitKey(0);
	return 0;
}


void show() {
	char filename[100];
	for (int i = 14; i <= 32; i++) {
		sprintf(filename, "CS585-PianoImages/piano_%d.png", i);
		Mat frame = imread(filename);
		namedWindow(filename, WINDOW_AUTOSIZE);
		Mat frameDest = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
		check(frameDest, frame);
		imshow(filename, frame);
	}
}


void avg(Mat& newFrame) {
	
	char filename[100];
	Mat frame = imread("CS585-PianoImages/piano_18.png");
	//newFrame = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
	newFrame = imread("CS585-PianoImages/piano_14.png");
	//int a[3][2][4] = { { {1,1,1,1}, {1,1,1,1} },{ {1,1,1,1}, {1,1,1,1} }, { {1,1,1,1}, { 1,1,1,1} } };
	//int Pixel[1000][1000][3];
	vector<vector<vector<int> > > Pixel;
	Pixel.resize(HEIGHT);
	for (int i = 0; i < HEIGHT; ++i) {
		Pixel[i].resize(WIDTH);
		for (int j = 0; j < WIDTH; ++j)
			Pixel[i][j].resize(DEPTH);
	}

	cout << frame.cols;
	cout << frame.rows;

	
	for (int i = 14; i <= 32; i++) {
		int channels = frame.channels();
		sprintf(filename, "CS585-PianoImages/piano_%d.png", i);
		frame = imread(filename);
		for (int row = 0; row < frame.rows; row++) {
			//unsigned char* rowPtr = frame.ptr<unsigned char>(row);
			for (int col = 0; col < frame.cols; col++) {
				Vec3b intensity = frame.at<Vec3b>(row, col);
				Pixel[row][col][0] += intensity[0];
				Pixel[row][col][1] += intensity[1];
				Pixel[row][col][2] += intensity[2];
			}
		}
	}
	
	for (int row = 0; row < newFrame.rows; row++) {
		int channels = newFrame.channels();
		//unsigned char* rowPtr = newFrame.ptr<unsigned char>(row);
		for (int col = 0; col < newFrame.cols; col++) {
			Vec3b intensity = newFrame.at<Vec3b>(row, col);
			intensity[0] = (Pixel[row][col][0]) / 19;
			intensity[1] = (Pixel[row][col][1]) / 19;
			intensity[2] = (Pixel[row][col][2]) / 19;
		}
	}
	//imwrite("avg.png", newFrame);
	return;
}


void check(Mat& image, Mat& image0)
{
	int column[2560];
	for (int i = 0; i < 2560; i++)
	{
		column[i] = 0;
	}
	if (image.empty() || image0.empty())
	{
		cout << "Error: image is unallocated" << endl;
		return;
	}

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) == 255) {
				column[i] += 1;
			}
		}
	}
	for (int i = 0; i < 2560; i++) {
		if (column[i] <= 10 && column[i] > 1) {
			cout << "   ";
			cout << i;
			cout << "   ";
		}
	}
	//1072
	//int channels = image.channels();
	//int channels0 = image0.channels();
	/*for (int row = 0; row < image.rows; row++)
	{
		//the .ptr method of the Mat class returns a pointer to the specifed row.
		//the type of the returned pointer is given inside the angle brackets
		//
		// The reason you might prefer to use this notation is because
		// Mat objects can be used to store other types of data too, not just unsigned char's.
		// However, the data member variable is always of unsigned char.
		unsigned char* rowPtr = image.ptr<unsigned char>(row);
		unsigned char* rowPtr0 = image0.ptr<unsigned char>(row);
		for (int col = 0; col < image.cols; col++)
		{
			if (rowPtr[col * channels + 0] == rowPtr0[col * channels0 + 0] && rowPtr[col * channels + 1] == rowPtr0[col * channels0 + 1] && rowPtr[col * channels + 2] == rowPtr0[col * channels0 + 2]) {
				rowPtr0[col * channels0 + 0] = 255;
				rowPtr0[col * channels0 + 1] = 255;
				rowPtr0[col * channels0 + 2] = 255;
			}
			else {
				rowPtr0[col * channels0 + 0] = 0;
				rowPtr0[col * channels0 + 1] = 0;
				rowPtr0[col * channels0 + 2] = 0;
			}
			// Once you have a pointer to the beginning of the row, you still need to calculate
			// the index of the element of the row that you want to access
			//int index = col * channels + 2;	//0 is the blue channel index

			//rowPtr[col * channels + 0] = (rowPtr[col * channels + 0] + rowPtr[col * channels + 1] + rowPtr[col * channels + 2]) / 3;
			//rowPtr[col * channels + 1] = (rowPtr[col * channels + 0] + rowPtr[col * channels + 1] + rowPtr[col * channels + 2]) / 3;
			//rowPtr[col * channels + 2] = (rowPtr[col * channels + 0] + rowPtr[col * channels + 1] + rowPtr[col * channels + 2]) / 3;


			//int mean = (rowPtr[col * channels + 1] + rowPtr[col * channels + 0] + rowPtr[col * channels + 2]) / 3;
			//int blue = col * channels + 0;
			//rowPtr[col * channels + 0] = mean;
			//int green = col * channels + 1;
			//rowPtr[col * channels + 1] = mean;
			//int red = col * channels + 2;
			//rowPtr[col * channels + 2] = mean;
		}
	}*/

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (j < 1072) {
				image.at<uchar>(i, j) = 0;
			}
		}
	}
	cv::Mat  labels, img_color, stats, centroids;
	int i, nccomps = cv::connectedComponentsWithStats(image, labels, stats, centroids);
	vector<cv::Vec3b> colors(nccomps + 1);
	colors[0] = Vec3b(0, 0, 0);
	for (i = 1; i < nccomps; i++) {
		if (stats.at<int>(i, cv::CC_STAT_AREA) > 500) {
			colors[i] = Vec3b(rand() % 256, rand() % 256, rand() % 256);
			int x = stats.at<int>(i, cv::CC_STAT_LEFT);
			int y = stats.at<int>(i, cv::CC_STAT_TOP);
			int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
			int height = stats.at<int>(i, cv::CC_STAT_HEIGHT);
			cv::rectangle(image0, Point(x-50, y-20), Point(x + width + 30, y + height + 30), colors[i], 5);
		}
	}
}




int myMax(int a, int b, int c) {
	int m = a;
	(void)((m < b) && (m = b)); //short-circuit evaluation
	(void)((m < c) && (m = c));
	return m;
}

//Function that returns the minimum of 3 integers
int myMin(int a, int b, int c) {
	int m = a;
	(void)((m > b) && (m = b));
	(void)((m > c) && (m = c));
	return m;
}

//Function that detects whether a pixel belongs to the skin based on RGB values
void mySkinDetect(Mat& src, Mat& dst) {
	//Surveys of skin color modeling and detection techniques:
	//Vezhnevets, Vladimir, Vassili Sazonov, and Alla Andreeva. "A survey on pixel-based skin color detection techniques." Proc. Graphicon. Vol. 3. 2003.
	//Kakumanu, Praveen, Sokratis Makrogiannis, and Nikolaos Bourbakis. "A survey of skin-color modeling and detection methods." Pattern recognition 40.3 (2007): 1106-1122.
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			//For each pixel, compute the average intensity of the 3 color channels
			Vec3b intensity = src.at<Vec3b>(i, j); //Vec3b is a vector of 3 uchar (unsigned character)
			int B = intensity[0]; int G = intensity[1]; int R = intensity[2];
			if ((R > 95 && G > 40 && B > 20) && (myMax(R, G, B) - myMin(R, G, B) > 60) && (abs(R - G) > 60) && (R > G) && (R > B)) {
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}//95 40 20 15 15 

//Function that does frame differencing between the current frame and the previous frame
void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst) {
	//For more information on operation with arrays: http://docs.opencv.org/modules/core/doc/operations_on_arrays.html
	//For more information on how to use background subtraction methods: http://docs.opencv.org/trunk/doc/tutorials/video/background_subtraction/background_subtraction.html
	absdiff(prev, curr, dst);
	Mat gs = dst.clone();
	cvtColor(dst, gs, CV_BGR2GRAY);
	dst = gs > 50;
}

//Function that accumulates the frame differences for a certain number of pairs of frames
void myMotionEnergy(vector<Mat> mh, Mat& dst) {
	Mat mh0 = mh[0];
	Mat mh1 = mh[1];
	Mat mh2 = mh[2];

	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) {
			if (mh0.at<uchar>(i, j) == 255 || mh1.at<uchar>(i, j) == 255 || mh2.at<uchar>(i, j) == 255) {
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}
