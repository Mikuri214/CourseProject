/*
 * Image_Controller.h
 *
 *  Created on: 2015-3-19
 *      Author: Clq
 *
 *  Modified on: 2015-5-14
 *      Author:  Clq
 */

#ifndef _H_IMAGECONTROLLER_H_
#define _H_IMAGECONTROLLER_H_
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>

class Image_Controller: public QGraphicsView{
	Q_OBJECT

public:
	QImage qImage;
	QPixmap qPixmap;
	QGraphicsPixmapItem* pPixmapItem;
	QGraphicsScene qScene;
	QSize qFixedSize;
	double ratio;

public:
	Image_Controller(QWidget *parent);
	~Image_Controller();
	void setQImage(QImage& qimg);
	void setQImgFixedSize(QSize qsize);

private:
	void setupScene();
	void updateQImage(QPoint );

protected:
	void wheelEvent(QWheelEvent *event);
};

#endif