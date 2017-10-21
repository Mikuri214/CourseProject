/*
 * initProgressbar.h
 *
 *  Created on: 2015-4-2
 *      Author: Clq
 *
 *  Modified on: 2015-5-14
 *      Author:  Clq
 */

#ifndef INITPROGRESSBAR_H
#define INITPROGRESSBAR_H

#include <QWidget>
#include <QMouseEvent>
#include "ui_waitingprogressbar.h"

class InitProgressBar : public QWidget
{
	Q_OBJECT

public:
	InitProgressBar(QWidget *parent = 0);
	~InitProgressBar();
	void setInfo(int );
	void updateProgress(int i);
	void updateLabel(int i);
	void setBtnTip(QString);
	void setLabelFile(QString);
	void setProcStatus(bool);
	//void 
protected:
	void closeEvent(QCloseEvent *e);
	//��갴���¼�
	void mousePressEvent(QMouseEvent *event);
	//����ͷ��¼�
	void mouseReleaseEvent(QMouseEvent *event);
	//����ƶ��¼�
	void mouseMoveEvent(QMouseEvent *event);

private:
	Ui::WaitingProgressBar ui;
	int fileCnt;
	bool procEnd;

	QPoint move_point; //�ƶ��ľ���
	bool mouse_press; //��갴��

signals:
	void procStop();
	void procDone();

private slots:
	void on_cmdLinkBtn_clicked();
	void on_pBtn_exit_clicked();
	void on_pBtn_minimize_clicked();
};

#endif // WAITINGPROGRESSBAR_H
