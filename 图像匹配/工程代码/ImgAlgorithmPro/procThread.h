#ifndef _H_PROCTHREAD_
#define _H_PROCTHREAD_

#include <QThread>
#include <QStringList>
#include "Algorithm/AlgorithmInterface.h"
#include <opencv2/opencv.hpp>
using namespace cv;


class procThread : public QThread
{
	Q_OBJECT

public:
	procThread(QObject *parent = 0);//��һ���߳�������  ��������ui
	~procThread();

	void init ( AlgorithmInterface* , QStringList& );

	bool status;
	AlgorithmInterface* algorithm;//��ǰʹ�õĴ����㷨
signals:
	void update ( QString, int, int );
	void threadEnd ();
	void threadFail ();
	void sendDetectRst ( int, int, float,Mat  );

protected:
	void run ();

private:
	QStringList fileList;
};

#endif