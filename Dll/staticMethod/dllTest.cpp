// dllTest.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "egg_vessel_detect.h"
#include "ai_egg_embryo_vessel_detection.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#pragma comment( lib,"../Release/ai_egg_embryo_vessel_detection.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	Mat m = imread ( "C:\\Users\\Administrator\\Desktop\\e-detect\\img\\2.png" );
		if ( !m.empty () )
		{
		ai_egg_embryo_vessel_detection param;
		param.imageSrc = m;
		int i = egg_vessel_detection ( ( void* ) ( &param ), 0 );
		if ( i == AlgorithmResult::AR_DONE )
		{
			cout << "finished;\n " << param.procTime;
			imshow ( "", param.imgDst );
			waitKey ();
		}
		else {
			printf ( "wrong" );
		}
		}
	return 0;
}

