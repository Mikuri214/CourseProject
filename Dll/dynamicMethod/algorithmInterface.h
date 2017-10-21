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
	AR_DONE = 0,//�㷨�ɹ�ִ�����
	AR_BREAK,//�㷨������ж�
	AR_INTERNAL,//�㷨���ڲ������ж�
	AR_EXTERNAL,//�㷨���ⲿ�����ж�
	AR_END//�㷨ִ��ʧ��
};

#define MAXPATH 256
class AlgorithmInterface{

public:
	char file[MAXPATH];//�����ļ���ȫ·����
	char folder[MAXPATH];//������Ҫ������ݵ��ļ���·����

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

	uint percent;//�㷨����Ľ��ȣ���ΧΪ��0-100����
	bool isRun;//�㷨���ⲿ���Ʊ����������㷨�Ƿ����ִ�С�

	int mutex;//�ź�������ֹͬһ���ݵĶ�д��ͻ��

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
	void lock ( int &_mutex ){//���ź���������
		while ( 0 != _mutex )
			sleep_ms ( 10 );
		if ( 0 == _mutex )
			++_mutex;
	}

	void unlock ( int & _mutex ){//���ź��������� 
		if ( 1 == _mutex )
			--_mutex;
	}

	void createImageData ( ImageData* imgData, int rows,
		int cols, int channels, uchar* pData ){//����ͼ��
		releaseImageData ( imgData );

		imgData->imgCols = cols;
		imgData->imgRows = rows;
		imgData->imgChannels = channels;
		int n = cols*rows*channels;
		imgData->pImgData = new uchar[n];
		memcpy ( imgData->pImgData, pData, n*sizeof ( uchar ) );
	}

	void createImageData ( ImageData* imgData, cv::Mat img ){//����ͼ��
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

	void releaseImageData ( ImageData* imgData ){//�ͷ�ͼ��
		if ( imgData->pImgData != NULL ){
			delete[]imgData->pImgData;
			imgData->pImgData = NULL;
		}
	}

};

#endif