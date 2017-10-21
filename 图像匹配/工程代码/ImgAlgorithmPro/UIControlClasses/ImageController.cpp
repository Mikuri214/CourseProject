/*
 * Image_Controller.cpp
 *  Created on: 2015-3-19
 *      Author: Clq
 *
 *  Modified on: 2015-5-14
 *      Author:  Clq
 */

#include "ImageController.h"

Image_Controller::Image_Controller(QWidget *parent = 0)
	:QGraphicsView(parent){
		setupScene();
}

Image_Controller::~Image_Controller(){

}

void Image_Controller::wheelEvent(QWheelEvent *event){
	QPoint pos = event->pos();
	int numDegrees = event->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
	if( numDegrees > 0)
		ratio += 0.2;
	else
		ratio = 1.0 > ratio- 0.2? 1.0 : ratio -0.2;
	updateQImage(pos);
	//event->accept(); 
}

void Image_Controller::setupScene(){
	setScene(&qScene);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setFrameShape(QFrame::NoFrame);
	setDragMode(QGraphicsView::ScrollHandDrag);
	//setSizePolicy(QSizePolicy);
	ratio = 1.0;
}

void Image_Controller::setQImgFixedSize(QSize qsize){
	qFixedSize = qsize;
}

void Image_Controller::setQImage(QImage& qimg){
	qImage = qimg;
	ratio = 1.0;
	qScene.clear();
	qPixmap = QPixmap::fromImage(qImage.scaled(qFixedSize));
	pPixmapItem = qScene.addPixmap(qPixmap);
	qScene.setSceneRect(0, 0, qPixmap.width(), qPixmap.height());
}

void Image_Controller::updateQImage(QPoint pos){
	qPixmap = QPixmap::fromImage(qImage.scaled(qFixedSize*ratio));
	pPixmapItem->setPixmap(qPixmap);
	qScene.setSceneRect(0, 0, qPixmap.width(), qPixmap.height());

	QPoint position = pos*ratio;
	//viewport()->scroll(position.x(), position.y());
	centerOn(position);
}