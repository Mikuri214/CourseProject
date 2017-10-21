#ifndef IMG_ALGORITHM_PRO_H_
#define IMG_ALGORITHM_PRO_H_

//ui 控制类头文件
#include <QtGui>
#include <QTreeWidget>
#include <QtWidgets/QDialog>

#include "ui_MainWindows.h"
#include "waitingprogressbar.h"
#include "ImageController.h"
#include "procThread.h"
#include  "initProgressbar.h"
#include "QtServiceDialog.h"

//使用的数据结构的头文件
#include <string>
#include <vector>
#include <map>
#include <QVector>
using namespace std;


//算法类头文件
#include "Algorithm/AlgorithmInterface.h"
#include "Algorithm/ImgMatchAlgorithmFactory.h"

//数据控制类头文件
#include "DataModel/DataModel.h"

//依赖的第三方库的头文件
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

	/////树形控件相关
	void initTreeView ();
	void initTreeWidget ( QTreeWidget*, string treeName, QRect zoom );
	void addTreeWidget ( QTreeWidget*, QString );
	void delAllTreeWidget ( QTreeWidget* );
	void destroyTreeView ();

	////图片显示控件相关
	void setImageView ( QString imgPath );
	void showImage ( cv::Mat, Image_Controller* );
	void showImage ( QImage&, Image_Controller* );

	////算法选择下拉框控件相关
	void initAlgorithmBox ( QComboBox* box, vector<string> algorithmNames );

	void clearData ();
	void outputResult ();//设置 结果显示栏 相关控件



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

	//ui 组件

	////批量处理
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

	//数据组件
	QStringList objFileList;
	QStringList sceneFileList;

	////统计结果
	int ResultCnt[2];
	float ResultTime;


	//工作组件
	ImgMatchAlgorithmFactory* algorithmFactory = nullptr;
	AlgorithmInterface* currAlgorithm = nullptr;
	procThread *m_procThread;

	DataModel *dataModel;
};

#endif // !IMG_ALGORITHM_PRO_H_