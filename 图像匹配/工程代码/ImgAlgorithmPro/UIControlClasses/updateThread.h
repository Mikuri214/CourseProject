/*
 * updatethread.h
 *
 *  Created on: 2015-1-5
 *      Author: Cai

 *  Modified on: 2015-5-14
 *      Author:  Clq
 */
#ifndef _H_UPDATETHREAD_CAI_
#define _H_UPDATETHREAD_CAI_
#include <QThread.h>
#include "waitingprogressbar.h"

struct UpDateInfo{
	WaitingProgressBar* pb;
};

class updateThread :public QThread{
	Q_OBJECT

public:
	void init(UpDateInfo*);
	updateThread(QObject *parent);
	~updateThread();
	void setStatus(bool);

protected:
	void run();

private:
	WaitingProgressBar* pPB;
	int percent;
	bool isDone;
};

#endif