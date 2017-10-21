/*
 * initProgressbar.cpp
 *
 *  Created on: 2015-4-2
 *      Author: Clq
 *
 *  Modified on: 2015-5-14
 *      Author:  Clq
 */
#include "initProgressbar.h"

InitProgressBar::InitProgressBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//���ô�����������ز�����λ�ڶ���
	this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	//�ɻ�ȡ������Ч��
	this->setMouseTracking(true);
	mouse_press = false;

	fileCnt = 0;
	ui.progressBar->setRange(0, 100);
}

InitProgressBar::~InitProgressBar()
{

}

void InitProgressBar::setInfo(int i){
	fileCnt = i;
}

void InitProgressBar::on_cmdLinkBtn_clicked(){
	on_pBtn_exit_clicked();
}

void InitProgressBar::updateProgress(int i){
	ui.progressBar->setValue(i);
}

void InitProgressBar::updateLabel(int i){
	QString qstr = QString::number(i) + "/" + QString::number(fileCnt);
	ui.label_progress->setText(qstr);
}

void InitProgressBar::setBtnTip(QString qstr){
	ui.cmdLinkBtn->setText(qstr);
}

void InitProgressBar::setLabelFile(QString qstr){
	ui.label_file->setText(qstr);
}

void InitProgressBar::closeEvent(QCloseEvent *e){
	on_pBtn_exit_clicked();
}

void InitProgressBar::mousePressEvent(QMouseEvent *event){  
	if(event->button() == Qt::LeftButton){
		mouse_press = true;
		//�������ڴ����λ�ã�����ʹ��event->globalPos() - this->pos()��
		move_point = event->pos();;
	}
}
void InitProgressBar::mouseMoveEvent(QMouseEvent *event){
	//��������������
	if(mouse_press){
		//����������Ļ��λ��
		QPoint move_pos = event->globalPos();

		//�ƶ�������λ��
		this->move(move_pos - move_point);
	}
}

void InitProgressBar::mouseReleaseEvent(QMouseEvent *event){  
	//�������Ϊδ������
	mouse_press = false;
}

void InitProgressBar::on_pBtn_minimize_clicked(){
	this->showMinimized();
}

void InitProgressBar::on_pBtn_exit_clicked(){
	if( !procEnd)
		emit procStop();
	updateProgress(0);
	this->close();
}

void InitProgressBar::setProcStatus(bool val){
	procEnd = val;
}