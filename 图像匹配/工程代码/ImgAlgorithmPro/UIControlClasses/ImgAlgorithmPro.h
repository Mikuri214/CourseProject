#ifndef IMG_ALGORITHM_PRO_H_
#define IMG_ALGORITHM_PRO_H_

//ui ������ͷ�ļ�
#include <QtGui>
#include <QTreeWidget>
#include <QtWidgets/QDialog>

#include "ui_MainWindows.h"
#include "waitingprogressbar.h"
#include "ImageController.h"
#include "procThread.h"
#include  "initProgressbar.h"
#include "QtServiceDialog.h"

//ʹ�õ����ݽṹ��ͷ�ļ�
#include <string>
#include <vector>
#include <map>
#include <QVector>
using namespace std;


//�㷨��ͷ�ļ�
#include "Algorithm/AlgorithmInterface.h"
#include "Algorithm/ImgMatchAlgorithmFactory.h"

//���ݿ�����ͷ�ļ�
#include "DataModel/DataModel.h"

//�����ĵ��������ͷ�ļ�
#include <opencv2/opencv.hpp>
using namespace cv;

class ImgAlgorithmPro : public QtServiceDialog
{
    Q_OBJECT

public:
    ImgAlgorithmPro(QWidget *parent = Q_NULLPTR);
	~ImgAlgorithmPro ();

	bool initServices ();

	bool readImage ( std::string path );
	void updateTreeView ( QTreeWidget* tree, QStringList& );

protected:
	virtual bool init ();
	void initViewController ();
	void destroyViewController ();

	/////���οؼ����
	void initTreeView ();
	void initTreeWidget ( QTreeWidget*, string treeName, QRect zoom );
	void addTreeWidget ( QTreeWidget*, QString );
	void delAllTreeWidget ( QTreeWidget* );
	void destroyTreeView ();

	////ͼƬ��ʾ�ؼ����
	void setImageView ( QString imgPath );
	void showImage ( cv::Mat, Image_Controller* );
	void showImage ( QImage&, Image_Controller* );

	////�㷨ѡ��������ؼ����
	void initAlgorithmBox ( QComboBox* box, vector<string> algorithmNames );

	void clearData ();
	void outputResult ();//���� �����ʾ�� ��ؿؼ�



	private slots:
	void objTreeItemChanged ( QTreeWidgetItem*, int );
	void sceneTreeItemChanged ( QTreeWidgetItem*, int );
	void closeWaitDlg ();
	void on_pBtn_open_Obj_clicked ();
	void on_pBtn_open_Scene_clicked ();
	void on_pBtn_detect_clicked ();
	void on_cBb_feature_selected ( const QString& );

	void getThreadEnd ();
	void getThreadFail ();
	void updateWaitBar ( QString, int, int );
	void dealProcessResult ( int, int, float, Mat );

private:
    Ui::MainWindowsClass  ui;

	//ui ���

	////��������
	WaitingProgressBar waitingDlg;
	QComboBox *algorithmBoxFeature;
	QComboBox *algorithmBoxOutlierResolve;
	QComboBox *algorithmBoxMatch;

	InitProgressBar initDlg;
	Image_Controller* imgView;
	QTreeWidget* obj_treeWidget;
	QTreeWidget* scene_treeWidget;

	QFont treeFont;
	QBrush treeForeBrush;
	QBrush treeBackBrush;

	QImage imgBg;
	QImage imgWatch;

	//�������
	QStringList objFileList;
	QStringList sceneFileList;

	////ͳ�ƽ��
	int ResultCnt[2];
	float ResultTime;


	//�������
	ImgMatchAlgorithmFactory* algorithmFactory = nullptr;
	AlgorithmInterface* currAlgorithm = nullptr;
	procThread *m_procThread;

	DataModel *dataModel;
};

#endif // !IMG_ALGORITHM_PRO_H_