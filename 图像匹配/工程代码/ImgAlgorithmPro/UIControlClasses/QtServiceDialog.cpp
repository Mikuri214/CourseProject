#include "QtServiceDialog.h"

QtServiceDialog::QtServiceDialog(QWidget *parent , Qt::WindowFlags flags)
:QDialog(parent, flags){
	init();
}

QtServiceDialog::~QtServiceDialog(){
	destroy();
}

bool QtServiceDialog::init(){
	//is_running = false;

	//设置窗体标题栏隐藏并设置位于顶层
	this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	//可获取鼠标跟踪效果
	this->setMouseTracking(true);
	mouse_press = false;

	return true;
}

void QtServiceDialog::destroy(){

}

/////文件路径读取图片
bool QtServiceDialog::getImage(QString qstr, QImage& img){
	///读取图片
	if( !img.load(qstr)){
		cv::VideoCapture capture(qstr.toStdString());
		cv::Mat image;
		int ret = capture.read(image);
		if( !ret){
			printf("File \" %s\" open failed!\n", qstr.toStdString().c_str());
			return false;
		}
		img= mat_to_qimage_cpy(image, 0);
	}
	return true;
}

//////图像转换部分
QImage const QtServiceDialog::copy_mat_to_qimage(cv::Mat const &mat, QImage::Format format){
	QImage image(mat.cols, mat.rows, format);
	for (int i = 0; i != mat.rows; ++i){
		memcpy(image.scanLine(i), mat.ptr(i), image.bytesPerLine() );
	}
	return image;
}  

QImage const QtServiceDialog::mat_to_qimage_cpy(cv::Mat &mat, int qimage_format){
	cv::Mat img;
	if(mat.type() == CV_8UC3){        
		cv::cvtColor(mat, img, cv::COLOR_BGR2RGB);
		return copy_mat_to_qimage(img, QImage::Format_RGB888);
	}
	if(mat.type() == CV_8U){
		return copy_mat_to_qimage(mat, QImage::Format_Indexed8);
	}
	if(mat.type() == CV_8UC4){        
		if(qimage_format == -1){
			return copy_mat_to_qimage(mat, QImage::Format_ARGB32);
		}else{
			return copy_mat_to_qimage(mat, QImage::Format(qimage_format));
		}
	}
	return QImage();
}

//////初始化本地连接
bool QtServiceDialog::initLocalConnection(QString strName){
	is_running = false;
	QCoreApplication::setApplicationName(strName);
	QString serverName = QCoreApplication::applicationName();
	QLocalSocket socket;
	socket.connectToServer(serverName);
	if( socket.waitForConnected(500)){
		is_running = true;
		return false;
	}
	/////连接不上服务器
	server = new QLocalServer(this);
	connect(server, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));
	if( server->listen(serverName)){
		///防止程序奔溃，进程残留
		if( server->serverError() == QAbstractSocket::AddressInUseError &&
			QFile::exists(server->serverName())){
				QFile::remove(server->serverName());
				server->listen(serverName);
		}
	}
	return true;
}

////建立新的本地连接
void QtServiceDialog::newLocalConnection(){
	QLocalSocket *socket = server->nextPendingConnection();
	if( !socket)
		return;
	socket->waitForReadyRead(1000);
	delete socket;
}

///是否已存在程序实例
bool QtServiceDialog::isRunning() const{
	return is_running;
}

/////初始化共享内存
void QtServiceDialog::initShareMem(QString strMem){
	sharedMem.setKey(strMem);
}

bool QtServiceDialog::shareMem(QSharedMemory* qsm, ProceedData& pd){
	if( !qsm->attach()){
		printf("shareMem attach failed!\n");
		return false;
	}
	QBuffer buffer;
	QDataStream in(&buffer);

	qsm->lock();
	buffer.setData((char*) qsm->constData(), qsm->size());
	buffer.open(QBuffer::ReadOnly);
	/////存储数据
	in>>pd.qvecFolder;
	in>>pd.qvecFiles;
	in>>pd.folder;

	qsm->unlock();
	qsm->detach();
	return true;
}

void QtServiceDialog::mousePressEvent(QMouseEvent *event){  
	if(event->button() == Qt::LeftButton){
		mouse_press = true;
		//鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
		move_point = event->pos();;
	}
}

void QtServiceDialog::mouseMoveEvent(QMouseEvent *event){
	//若鼠标左键被按下
	if(mouse_press){
		//鼠标相对于屏幕的位置
		QPoint move_pos = event->globalPos();
		//移动主窗体位置
		this->move(move_pos - move_point);
	}
}

void QtServiceDialog::mouseReleaseEvent(QMouseEvent *event){  
	//设置鼠标为未被按下
	mouse_press = false;
}

void QtServiceDialog::dialog_minimize(){
	this->showMinimized();
}

void QtServiceDialog::dialog_exit(){
	this->close();
}

std::string QtServiceDialog::getFileName(const QString& file, QString path, QString& name){
	QString fileName;
	QStringList qstrl = file.split("\\");
	if( 1 == qstrl.count()){
		qstrl = file.split("/");
		name = qstrl.at(qstrl.count()-1);
		path.replace("\\", "/");
		fileName = path + "/" + name;
	}else{
		name = qstrl.at(qstrl.count()-1);
		path.replace("\\","/" );
		fileName = path + "/" + name;
	}
	return fileName.toStdString();
}