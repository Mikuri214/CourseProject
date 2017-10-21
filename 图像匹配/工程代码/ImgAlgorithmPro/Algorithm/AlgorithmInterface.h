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
	AR_DONE = 0,//算法成功执行完毕
	AR_BREAK,//算法因控制中断
	AR_INTERNAL,//算法因内部错误中断
	AR_EXTERNAL,//算法因外部错误中断
	AR_END//算法执行失败
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


//////////////////所有算法都要提供的全局函数接口/////////////////////
typedef AlgorithmInterface* ( *GetInstance )( );


#endif
