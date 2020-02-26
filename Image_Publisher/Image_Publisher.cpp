#include <stdio.h>

#include <iostream>
#include "Image_Publisher.h"
#include "cv.h"
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

void read_test_image()
{

	const char* file_name = "ttt.txt";
	


	cout << "read_test_image";
/*	ifstream image_file;
	//ifstream image_file("after.png", ios::in | ios::binary | ios::ate);
	image_file.open("after.png", ios::in | ios::binary | ios::ate);
	if (image_file.is_open())
	{
		
		cout << "1111 ";
		const auto file_size = image_file.gcount();
		cout << " file_size " + file_size;
		const auto raw_image_data = new char[file_size];
		cout << " 222 file_size " + file_size;
		image_file.seekg(0, ios::beg);
		cout << "3333";
		image_file.read(raw_image_data, file_size);
		image_file.close();

		_image_data.assign(file_size, raw_image_data[0]);
		cout << "_image_data.size() :" + _image_data.size();
		delete[] raw_image_data;
	}
	*/
}

void check()
{
	cout << "Inside12";
}

void SetSubscriber(publish_delegate callback)
{
	cout << "Inside";
	_publish_callback = callback;
	//read_test_image();
/*
  Mat_<Vec3b> img(240, 320, Vec3b(0,255,0));

 for(int i = 0; i < 100; i++)
               img(i,i)=Vec3b(255,255,255);

 for(int i = 0; i < img.rows; i++)
	              for(int j = 0; j < img.cols; j++)
			                      img(i,j)[2] ^= (uchar)(i ^ j);
         imwrite("green.jpg",img);


	cout << "_image_data.size() :" ;
	Size imgSize =  img.size();
	_publish_callback(200, imgSize.area(), &img);
	*/
}
