#ifndef _H_ALGORITHMINTERFACE_H_
#define _H_ALGORITHMINTERFACE_H_
#include <string>
#include <opencv2/opencv.hpp>

typedef unsigned int uint;

#ifdef WIN32
#include <windows.h>
#define SLEEP_FUNC(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_FUNC(ms) usleep(ms*1000);
#endif

enum AlgorithmResult{
	AR_DONE = 0,//算法成功执行完毕
	AR_BREAK,//算法因控制中断
	AR_INTERNAL,//算法因内部错误中断
	AR_EXTERNAL,//算法因外部错误中断
	AR_END//算法执行失败
};

#define MAXPATH 256
class AlgorithmInterface{

public:
	char file[MAXPATH];//保存文件的全路径。
	char folder[MAXPATH];//保存需要输出数据的文件夹路径。

	struct ImageData{
		int imgRows;
		int imgCols;
		int imgChannels;
		uchar *pImgData;
		ImageData (){
			imgRows = imgCols = imgChannels = 0;
			pImgData = NULL;
		}
	};
	ImageData imageData;

	uint percent;//算法处理的进度，范围为（0-100）。
	bool isRun;//算法的外部控制变量，决定算法是否继续执行。

	int mutex;//信号量，防止同一数据的读写冲突。

public:
	AlgorithmInterface (){
		init ();
	}

	virtual ~AlgorithmInterface (){
		destroy ();
	}

	virtual void init (){
		mutex = 0;
		percent = 0;
		isRun = false;
	}
	virtual void destroy (){
	}
	void sleep_ms ( int ms ) {
		SLEEP_FUNC ( ms );
	}
	void lock ( int &_mutex ){//对信号量加锁。
		while ( 0 != _mutex )
			sleep_ms ( 10 );
		if ( 0 == _mutex )
			++_mutex;
	}

	void unlock ( int & _mutex ){//对信号量解锁。 
		if ( 1 == _mutex )
			--_mutex;
	}

	void createImageData ( ImageData* imgData, int rows,
		int cols, int channels, uchar* pData ){//创建图像
		releaseImageData ( imgData );

		imgData->imgCols = cols;
		imgData->imgRows = rows;
		imgData->imgChannels = channels;
		int n = cols*rows*channels;
		imgData->pImgData = new uchar[n];
		memcpy ( imgData->pImgData, pData, n*sizeof ( uchar ) );
	}

	void createImageData ( ImageData* imgData, cv::Mat img ){//创建图像
		releaseImageData ( imgData );

		imgData->imgCols = img.cols;
		imgData->imgRows = img.rows;
		imgData->imgChannels = img.channels ();
		int n = img.cols*img.rows*img.channels ();
		imgData->pImgData = new uchar[n];
		memcpy ( imgData->pImgData, img.data, n*sizeof ( uchar ) );
	}

	cv::Mat createMat ( ImageData* imgData ){
		uchar* data = imgData->pImgData;
		int rows = imgData->imgRows;
		int cols = imgData->imgCols;
		int channals = imgData->imgChannels;
		int type;
		if ( channals == 1 )
			type = CV_8U;
		else
			type = CV_8UC3;
		cv::Mat temp ( rows, cols, type, data );
		return temp.clone ();
	}

	void releaseImageData ( ImageData* imgData ){//释放图像
		if ( imgData->pImgData != NULL ){
			delete[]imgData->pImgData;
			imgData->pImgData = NULL;
		}
	}

};

#endif