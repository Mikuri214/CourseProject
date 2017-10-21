// dlltest2_dynamicLoading.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ai_egg_embryo_vessel_detection.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

typedef  uint  (*DetectFunc )( void* p, int size );

int _tmain ( int argc, _TCHAR* argv[] )
{
	HMODULE hdll = LoadLibrary ( L"ai_egg_embryo_vessel_detection.dll" );
	if ( hdll != NULL )
	{
		DetectFunc  func = ( DetectFunc ) GetProcAddress ( hdll, "egg_vessel_detection" );
		if ( func != NULL )
		{
			Mat m = imread ( "C:\\Users\\Administrator\\Desktop\\e-detect\\img\\2.png" );
			if ( !m.empty () )
			{
				ai_egg_embryo_vessel_detection param;
				param.imageSrc = m;
				int i = func ( ( void* ) ( &param ), 0 );
				if ( i == AlgorithmResult::AR_DONE )
				{
					cout<< "finished;\n "<<param.procTime;
					imshow ( "", param.imgDst );
					waitKey ();
				}
				else {
					printf ( "wrong" );
				}
			}
		}
	}
	return 0;
}

