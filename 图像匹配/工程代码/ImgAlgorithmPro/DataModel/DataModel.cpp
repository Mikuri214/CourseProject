#include "DataModel.h"
#include <QDir>
#include <QFile>

DataModel* DataModel::m_instance = nullptr;

DataModel::~DataModel ()
{

}

DataModel* DataModel::getInstance ()
{
	if ( !m_instance )
	{
		m_instance = new DataModel ();
	}
	return m_instance;
}

bool DataModel::deleteInstance ()
{
	if ( m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
	return true;
}

bool DataModel::loadData ( string dirPath )
{
	QDir dir ( QString::fromStdString(dirPath) );
	if ( !dir.exists () )
		return false;

	QStringList fileType;
	fileType << QString::fromLocal8Bit ( "*.jpg" ) <<QString::fromLocal8Bit ( "*.png" ) << QString::fromLocal8Bit ( "*.tif" );

	QStringList fileFiles = dir.entryList (fileType, QDir::Files );
	for ( int i = 0; i < fileFiles.size (); ++i )
		fileLists.push_back( dirPath + "/" + fileFiles[i].toStdString() );

	return true;
}

cv::Mat DataModel::getData ( int index )
{
	return imread ( fileLists.at(index) );
}
