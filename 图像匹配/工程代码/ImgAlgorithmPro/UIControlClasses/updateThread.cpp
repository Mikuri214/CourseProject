/*
 * updatethread.cpp
 *
 *  Created on: 2015-1-5
 *      Author: Cai

 *  Modified on: 2015-5-14
 *      Author:  Clq
 */
#include "updateThread.h"

updateThread::updateThread(QObject *parent)
	:QThread(parent){
	pPB = NULL;
	percent = 0;
	isDone = false;
}

updateThread::~updateThread(){
	pPB->close();
};

void updateThread::run(){
	while( !isDone){
		//percent
		msleep(500);
	}
}

void updateThread::init(UpDateInfo* updateInfo){
	pPB = updateInfo->pb;
	pPB->setWindowTitle(QString::fromLocal8Bit("�����ļ�����"));
	pPB->setBtnTip(QString::fromLocal8Bit("�������������ļ���"));
}

void updateThread::setStatus(bool status){
	isDone = status;
}