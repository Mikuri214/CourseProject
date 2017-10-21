#ifndef ALGORITHM_INTERFACE_H_
#define ALGORITHM_INTERFACE_H_

#include <string>
using namespace std;
#include <opencv2/opencv.hpp>
using namespace cv;

typedef unsigned int uint;

#ifdef WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP(ms) usleep(ms*1000);
#endif


enum AlgorithmResult{
	AR_DONE = 0,//�㷨�ɹ�ִ�����
	AR_BREAK,//�㷨������ж�
	AR_INTERNAL,//�㷨���ڲ������ж�
	AR_EXTERNAL,//�㷨���ⲿ�����ж�
	AR_END//�㷨ִ��ʧ��
};

class AlgorithmInterface
{
public:
	AlgorithmInterface (){};
	virtual ~AlgorithmInterface (){};

	string algorithmName;
	virtual void init ( void* ) = 0;
	virtual void destroy () = 0;
	virtual AlgorithmResult process ( void* param = nullptr ) = 0;

	class AlgorithmParam
	{
	public:
		virtual ~AlgorithmParam () = 0{};
	};
};


//////////////////�����㷨��Ҫ�ṩ��ȫ�ֺ����ӿ�/////////////////////
typedef AlgorithmInterface* ( *GetInstance )( );


#endif
