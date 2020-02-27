#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "Image_Publisher.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>


#include <string.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <unistd.h>

#include "MWFOURCC.h"
#include "LibMWCapture/MWCapture.h"
#include "LibMWCapture/MWEcoCapture.h"
#include "bitmap.h"


using namespace std;
using namespace cv;

void read_test_image()
{

	const char* file_name = "ttt.txt";



	cout << "read_test_image";
	/*      ifstream image_file;
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


HCHANNEL open_channel() {
	HCHANNEL hChannel = NULL;
	int nChannelCount = MWGetChannelCount();

	if (0 == nChannelCount) {
		printf("ERROR: Can't find channels!\n");
		return NULL;
	}
	printf("Find %d channels!\n", nChannelCount);
	int nProDevCount = 0;
	int nProDevChannel[32] = { -1 };
	for (int i = 0; i < nChannelCount; i++) {
		MWCAP_CHANNEL_INFO info;
		MW_RESULT mr = MWGetChannelInfoByIndex(i, &info);
		if ((strcmp(info.szFamilyName, "Pro Capture") == 0) || (strcmp(info.szFamilyName, "Eco Capture") == 0)) {
			printf("find %s\n", info.szFamilyName);
			nProDevChannel[nProDevCount] = i;
			nProDevCount++;
		}
	}
	if (nProDevCount <= 0) {
		printf("\nERROR: Can't find pro or eco channels!\n");
		return NULL;
	}

	printf("Find %d pro or eco channels.\n", nProDevCount);

	// Get <board id > <channel id> or <channel index>
	int byBoardId = 0;
	int byChannelId = 2;
	int nDevIndex = -1;
	BOOL bIndex = FALSE;

	MWCAP_CHANNEL_INFO videoInfo = { 0 };
	//if (argc == 1) {
		if (MW_SUCCEEDED != MWGetChannelInfoByIndex(nProDevChannel[0], &videoInfo)) {
			printf("ERROR: Can't get channel info!\n");
			return NULL;
		}
		else {

			bIndex = TRUE;
			nDevIndex = 0;
		}
	//}
	
	// Open channel
	
		hChannel = MWOpenChannel(byBoardId, byChannelId);
		if (hChannel == NULL) {
			printf("ERROR: Open channel %X:%d error!\n", byBoardId, byChannelId);
			return NULL;
		}
	
	return hChannel;
}


int TakeScreenshot()
{
	
	if (!MWCaptureInitInstance()) {
		printf("have InitilizeFailed");
	}

	HCHANNEL hChannel = NULL;
	HNOTIFY hNotify = 0;
	MWCAP_PTR hNotifyEvent = 0;
	MWCAP_PTR hCaptureEvent = 0;

	HANDLE64 pbImage = 0;

	LONGLONG llTotalTime = 0LL;

	int nRet;


		MWRefreshDevice();
		hChannel = open_channel();
		if (NULL == hChannel) {
			return NULL;
		}
		MWCAP_CHANNEL_INFO videoInfo = { 0 };
		if (MW_SUCCEEDED != MWGetChannelInfo(hChannel, &videoInfo)) {
			printf("ERROR: Can't get channel info!\n");
			return NULL;
		}

		printf("Open channel - BoardIndex = %X, ChannelIndex = %d.\n", videoInfo.byBoardIndex, videoInfo.byChannelIndex);
		printf("Product Name: %s\n", videoInfo.szProductName);
		printf("Board SerialNo: %s\n\n", videoInfo.szBoardSerialNo);
		if (0 == strcmp(videoInfo.szFamilyName, "Eco Capture")) {
			//EcoCaptureTest(hChannel);
			return NULL;
		}
		// Capture frames on input signal frequency
		hCaptureEvent = MWCreateEvent();
		if (hCaptureEvent == 0) {
			printf("Create timer event error\n");
			return NULL;
		}

		hNotifyEvent = MWCreateEvent();
		if (hNotifyEvent == 0) {
			printf("Create notify event error\n");
			return NULL;
		}

		int cx = 1920;
		int cy = 1080;

		DWORD dwFourcc = MWFOURCC_BGR24;
		DWORD dwMinStride = FOURCC_CalcMinStride(dwFourcc, cx, 4);
		DWORD dwImageSize = FOURCC_CalcImageSize(dwFourcc, cx, cy, dwMinStride);
		pbImage = (HANDLE64)(unsigned long)malloc(dwImageSize);

		MW_RESULT xr;
		xr = MWStartVideoCapture(hChannel, hCaptureEvent);
		if (xr != MW_SUCCEEDED) {
			printf("ERROR: Start Video Capture error!\n");
			return NULL;
		}
		MWCAP_VIDEO_BUFFER_INFO videoBufferInfo;
		MWGetVideoBufferInfo(hChannel, &videoBufferInfo);

		MWCAP_VIDEO_FRAME_INFO videoFrameInfo;
		MWGetVideoFrameInfo(hChannel, videoBufferInfo.iNewestBufferedFullFrame, &videoFrameInfo);

		MWCAP_VIDEO_SIGNAL_STATUS videoSignalStatus;
		MWGetVideoSignalStatus(hChannel, &videoSignalStatus);

		switch (videoSignalStatus.state) {
		case MWCAP_VIDEO_SIGNAL_NONE:
			printf("ERRPR: Input signal status: NONE\n");
			return NULL;
		case MWCAP_VIDEO_SIGNAL_UNSUPPORTED:
			printf("ERRPR: Input signal status: Unsupported\n");
			return NULL;
		case MWCAP_VIDEO_SIGNAL_LOCKING:
			printf("ERRPR: Input signal status: Locking\n");
			return NULL;
		case MWCAP_VIDEO_SIGNAL_LOCKED:
			printf("Input signal status: Locked\n");
			return NULL;
		}

		if (videoSignalStatus.state != MWCAP_VIDEO_SIGNAL_LOCKED) {
			MWStopVideoCapture(hChannel);
			return NULL;
		}

		printf("Input signal resolution: %d x %d\n", videoSignalStatus.cx, videoSignalStatus.cy);
		double fps = (videoSignalStatus.bInterlaced == TRUE) ? (double)20000000LL / videoSignalStatus.dwFrameDuration : (double)10000000LL / videoSignalStatus.dwFrameDuration;
		printf("Input signal fps: %.2f\n", fps);
		printf("Input signal interlaced: %d\n", videoSignalStatus.bInterlaced);
		printf("Input signal frame segmented: %d\n", videoSignalStatus.bSegmentedFrame);


		hNotify = MWRegisterNotify(hChannel, hNotifyEvent, MWCAP_NOTIFY_VIDEO_FRAME_BUFFERED);
		if (hNotify == 0) {
			MWStopVideoCapture(hChannel);
			printf("ERROR: Register Notify error.\n");
			return NULL;
		}
		int NUM_CAPTURE = 1;
		printf("Begin to capture %d frames by %.2f fps...\n", NUM_CAPTURE, fps);
		fflush(stdout);
		int i;
		for (i = 0; i < NUM_CAPTURE; i++) {
			nRet = MWWaitEvent(hNotifyEvent, 1000);
			if (nRet <= 0) {
				printf("Error:wait notify error or timeout\n");
				return NULL;
			}

			ULONGLONG ullStatusBits = 0;
			xr = MWGetNotifyStatus(hChannel, hNotify, &ullStatusBits);
			if (xr != MW_SUCCEEDED)
				continue;

			xr = MWGetVideoBufferInfo(hChannel, &videoBufferInfo);
			if (xr != MW_SUCCEEDED)
				continue;

			xr = MWGetVideoFrameInfo(hChannel, videoBufferInfo.iNewestBufferedFullFrame, &videoFrameInfo);
			if (xr != MW_SUCCEEDED)
				continue;

			if (ullStatusBits & MWCAP_NOTIFY_VIDEO_FRAME_BUFFERED == 0) {
				continue;
			}
			xr = MWCaptureVideoFrameToVirtualAddressEx(hChannel,
				videoBufferInfo.iNewestBufferedFullFrame, (unsigned char *)pbImage, dwImageSize, dwMinStride,
				1, 0, dwFourcc, cx, cy, 0, 0, 0, 0, 0, 100, 0, 100, 0, MWCAP_VIDEO_DEINTERLACE_BLEND,
				MWCAP_VIDEO_ASPECT_RATIO_CROPPING, 0, 0, 0, 0, MWCAP_VIDEO_COLOR_FORMAT_UNKNOWN,
				MWCAP_VIDEO_QUANTIZATION_UNKNOWN, MWCAP_VIDEO_SATURATION_UNKNOWN);
			if (xr != MW_SUCCEEDED)
				continue;

			nRet = MWWaitEvent(hCaptureEvent, 1000);
			if (nRet <= 0) {
				printf("Error:wait capture event error or timeout\n");
				return NULL;
			}

			MWCAP_VIDEO_CAPTURE_STATUS captureStatus;
			xr = MWGetVideoCaptureStatus(hChannel, &captureStatus);

			LONGLONG llCurrent = 0LL;
			xr = MWGetDeviceTime(hChannel, &llCurrent);

			llTotalTime += (llCurrent - (videoSignalStatus.bInterlaced ? videoFrameInfo.allFieldBufferedTimes[1] : videoFrameInfo.allFieldBufferedTimes[0]));
			char szBitmapName[64];
			sprintf(szBitmapName, "./video_%d.BMP", NUM_CAPTURE);
			if (i == NUM_CAPTURE - 1)
				create_bitmap(szBitmapName, pbImage, dwImageSize, cx, cy);
		}
		xr = MWUnregisterNotify(hChannel, hNotify);
		hNotify = 0;
		xr = MWStopVideoCapture(hChannel);
		if (i >= NUM_CAPTURE) {
			printf("\nStop capture\n");
			printf("\nAverage frame duration is %lld ms.\n", (LONGLONG)(llTotalTime / (NUM_CAPTURE * 10000)));
			printf("Write 100th frame to file Video_%d.BMP\n", NUM_CAPTURE);

		}

	if (hChannel != NULL) {
		MWCloseChannel(hChannel);
		hChannel = NULL;
	}

	if (hNotifyEvent != 0) {
		MWCloseEvent(hNotifyEvent);
		hNotifyEvent = 0;
	}

	if (hCaptureEvent != 0) {
		MWCloseEvent(hCaptureEvent);
		hCaptureEvent = 0;
	}

	if (pbImage != 0) {
		free((void *)(unsigned long)pbImage);
		pbImage = 0;
	}

	MWCaptureExitInstance();

	printf("\n exit!\n");

}





void SetSubscriber(publish_delegate callback)
{
	cout << "Inside";
	_publish_callback = callback;
	//read_test_image();
	Mat_<Vec3b> img(240, 320, Vec3b(0, 255, 0));

	for (int i = 0; i < 100; i++)
		img(i, i) = Vec3b(255, 255, 255);
	//  int j = 10;
	// for(int i = 0; i < img.rows; i++)
	//    for(int j = 0; j < img.cols; j++)
	//     img(i,j)[2] ^= (uchar)(i ^ j);
	imwrite("green.jpg", img);


	cout << "_image_data.size() :";
	//Size imgSize =  img.size();
	bool is_success;
	Size imgSize = img.size();
	std::vector< uchar >  img_buff;// = new  vector< uchar >[imgSize.width * imgSize.height];
	const std::vector< int > img_buff_int;// = new vector< int >[imgSize.width * imgSize.height];

	is_success = cv::imencode(".jpg", img, img_buff, img_buff_int);
	/*
	for( int i=0; i < img_buff.size(); i++)
	std::cout << img_buff[i] << std::endl;
	*/
	TakeScreenshot();
	_publish_callback(200, img_buff.size(), &img_buff);

	//      _publish_callback(400, imgSize.area(), &img);
}

