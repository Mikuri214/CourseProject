#ifndef _H_QTSERVICE_DIALOG_H_
#define _H_QTSERVICE_DIALOG_H_

#include <QtGui>
#include <QDialog>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QSharedMemory>
#include <opencv2/opencv.hpp>
#include <string>
#include <QVector>

struct ProceedData{
	QVector<QString> qvecFolder;
	QVector<QStringList> qvecFiles;
	QString folder;
};

class QtServiceDialog: public QDialog{
	Q_OBJECT

public:
	QtServiceDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	virtual ~QtServiceDialog();

	bool isRunning() const;

protected:
	virtual bool init();
	virtual void destroy();

	bool getImage(QString, QImage&);
	bool initLocalConnection(QString);
	void initShareMem(QString);
	bool shareMem(QSharedMemory*, ProceedData&);

	std::string getFileName(const QString& , QString , QString& );
	QImage const copy_mat_to_qimage(cv::Mat const &, QImage::Format);
	QImage const mat_to_qimage_cpy(cv::Mat &, int);
protected:
	void mousePressEvent(QMouseEvent *event);//��갴���¼�
	void mouseReleaseEvent(QMouseEvent *event);//����ͷ��¼�
	void mouseMoveEvent(QMouseEvent *event);//����ƶ��¼�

protected slots:
	void newLocalConnection();///������������
	void dialog_minimize();///������С��
	void dialog_exit();////�����˳�

protected:
	QPoint move_point; //�ƶ��ľ���
	bool mouse_press; //��갴��

	QLocalServer* server;
	bool is_running;
	QSharedMemory sharedMem;

};

#endif