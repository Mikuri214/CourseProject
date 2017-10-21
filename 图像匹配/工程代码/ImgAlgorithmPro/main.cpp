#include "UIControlClasses/ImgAlgorithmPro.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTextCodec>
#include <QtWidgets/QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QTextCodec *codec = QTextCodec::codecForName ( "UTF-8" );
	QTextCodec::setCodecForLocale ( QTextCodec::codecForLocale () );

    ImgAlgorithmPro w;

	if ( w.isRunning () ){
		a.quit ();
		return -1;
	}

	if ( !w.initServices () ){
		QMessageBox::information ( 0, QString::fromLocal8Bit ( "提示" ),
			QString::fromLocal8Bit ( "服务初始化失败！" ) );
		a.quit ();
		return -1;
	}

    w.show();
    return a.exec();
}
