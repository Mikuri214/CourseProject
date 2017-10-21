#include <QtCore/QCoreApplication>
#include <QtCore/QLibrary>
#include "ai_egg_embryo_vessel_detection.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
using namespace cv;
using namespace std;

typedef uint (  * egg_vessel_detection ) ( void* p, int size);

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QLibrary dll;
	dll.setFileName (  QString("../Debug/ai_egg_embryo_vessel_detection.dll")  );
 	if (!dll.load())
 	{
 		printf ( "qLibrary ����ʧ�ܣ�\n" );
 	}
 	else {
		printf ( "qLibrary ����ɹ���\n" );
 		egg_vessel_detection func = ( egg_vessel_detection ) dll.resolve ( "egg_vessel_detection" );
		if (func == NULL)
		{
			printf ( "func����ʧ�ܣ�\n" );
			return 1;
		}
		printf ( "func����ɹ���\n" );
		Mat m = imread ( "C:\\Users\\Administrator\\Desktop\\e-detect\\img\\2.png" );
 		if (!m.empty())
 		{
			printf ( "img����ɹ���\n" );
			ai_egg_embryo_vessel_detection param;
			param.imageSrc = m;
			int i = func ( ( void* ) ( &param ), sizeof(param) );
 			if (i == AlgorithmResult::AR_DONE)
 			{
				cout << "finished;\n " << param.procTime;
 				imshow ( "",param.imgDst );
				waitKey ();
 			}
 			else {
 				printf ( "wrong" );
 			}
 		}
		else printf ( "img����ʧ�ܣ�\n" );
 	}
	return a.exec();
}
