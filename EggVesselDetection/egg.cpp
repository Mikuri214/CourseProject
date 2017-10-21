// egg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "helper.h"
#include <sys/timeb.h>
#include <iostream>
#include <fstream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string dir_path = "C:\\Users\\Administrator\\Desktop\\e-detect\\img";
	string store_path = "E:\\";

	Directory dir;
	vector<string>  fileList = dir.GetListFiles ( dir_path, "*.png",false );

	Mat origin, hsv, h, v, tmp[3], bw, bw2;
	struct timeb startTime, endTime;
	int times = 0;
	fstream out ;
	out.open ( "C:\\Users\\Administrator\\Desktop\\out1.txt", ios::out);
	if ( !out ) return 0;
	for ( int i = 0; i < 10; ++i )
	{
		origin = imread ( dir_path + "\\" + fileList[i] );
		if ( origin.empty () )
		{
			cerr << "error : can't read " << dir_path << "\\" << fileList[i];
			continue;
		}
		times++;

		ftime ( &startTime );
		resize ( origin, origin, Size( 800, 600 ) );
		cvtColor ( origin, hsv, COLOR_BGR2HSV );
		split ( hsv, tmp );
		h = tmp[0];
		v = tmp[2];
		ftime ( &endTime );
		static double pretime = 0;
		pretime += ( ( endTime.time - startTime.time ) * 1000 + ( endTime.millitm - startTime.millitm ) );

		ftime ( &startTime );
		enhance (out, h, bw );
		ftime ( &endTime );
		static double enhanceTime = 0;
		enhanceTime += ( ( endTime.time - startTime.time ) * 1000 + ( endTime.millitm - startTime.millitm ) );

		ftime ( &startTime );
		trimap ( out, bw, v, bw2 );
		ftime ( &endTime );
		static double trimaptime = 0;
		trimaptime += ( ( endTime.time - startTime.time ) * 1000 + ( endTime.millitm - startTime.millitm ) );

		ftime ( &startTime );
		postProcess ( out, bw2, bw );
		ftime ( &endTime );
		static double posttime = 0;
		posttime += ( ( endTime.time - startTime.time ) * 1000 + ( endTime.millitm - startTime.millitm ) );
		
		ftime ( &startTime );
		markOut ( origin, bw );
		ftime ( &endTime );
		static double markouttime = 0;
		markouttime += ( ( endTime.time - startTime.time ) * 1000 + ( endTime.millitm - startTime.millitm ) );

		if (i ==9 )
		{
			
			out << "\n\n\n\npretime:" << pretime / times
				<< "\nenhanceTime:" << enhanceTime / times
				<< "\ntrimaptime:" << trimaptime / times
				<< "\nposttime:" << posttime / times
				<< "\nmarkouttime:" << markouttime / times
				<< "\navg/ : " << ( pretime + enhanceTime + trimaptime + posttime + markouttime ) / times << "\n";
		}
	}
	return 0;
}

