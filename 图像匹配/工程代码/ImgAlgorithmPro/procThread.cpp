#include "procThread.h"

#include "Algorithm/FernsImgMatchAlgorithm.h"
#include "Algorithm/GenericImgMatchAlgorithm.h"

#include <ctime>
using std::string;



procThread::procThread(QObject *parent)
	: QThread(parent)
{
	algorithm = NULL;
	status = false;
}

procThread::~procThread()
{
}

void procThread::init ( AlgorithmInterface* algorithmPtr , QStringList& param )
{
	algorithm = algorithmPtr;
	fileList = param;
}

QString getFileName ( const QString& file  )
{
	QString fileName;
	QStringList qstrl = file.split ( "\\" );
	if ( 1 == qstrl.count () )
	{
		qstrl = file.split ( "/" );
		fileName = qstrl.at ( qstrl.count () - 1 );
	}
	else
	{
		fileName = qstrl.at ( qstrl.count () - 1 );
	}

	return fileName.split ( "." ).at ( 0 );
}

void procThread::run ()
{
	Mat obj, scene , result;

	obj = imread (  string(fileList.at ( 0 ).toLocal8Bit() ) );
	scene = imread ( string ( fileList.at ( 1 ).toLocal8Bit () ) );
	QString dataFileName = getFileName ( fileList[0] );

	ImgMatchAlgorithm::ImgMatchAlgorithmParam *param;
	if ( ( algorithm->algorithmName ).find ( "FERNS" ) != -1 )
	{
		FernsImgMatchAlgorithm::FernsImgMatchAlgorithmParam *p = new FernsImgMatchAlgorithm::FernsImgMatchAlgorithmParam ();
		p->objFileName = dataFileName.toStdString ();
		param = p;
	}
	else
		param = new ImgMatchAlgorithm::ImgMatchAlgorithmParam ();
	param->obj = obj;
	param->scene = scene;
	param->result = &result;
	param->obj_with_key_point = &obj;
	param->scene_with_key_point = &scene;

	float runtime =  getTickCount ();
	int execResult = algorithm->process ( param );
	runtime = getTickCount () - runtime;

	emit sendDetectRst ( 1 , execResult , runtime , result );
	emit update ( dataFileName , 1,  1 );
	if ( execResult == AlgorithmResult::AR_DONE )
		emit threadEnd ();
	else
		emit threadFail ();
}

