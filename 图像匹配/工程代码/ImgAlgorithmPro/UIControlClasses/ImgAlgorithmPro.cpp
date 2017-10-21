#include "ImgAlgorithmPro.h"
#include <QFileDialog>
#include <QMessageBox>

ImgAlgorithmPro::ImgAlgorithmPro(QWidget *parent)
{
	ui.setupUi(this);

	dataModel = DataModel::getInstance ();
	algorithmFactory = ImgMatchAlgorithmFactory::getInstance ();
	m_procThread = new procThread ();

	init ();
}

ImgAlgorithmPro::~ImgAlgorithmPro ()
{
	destroyViewController ();
	destroyTreeView ();

	if ( m_procThread != NULL ){
		m_procThread->terminate ();
		delete m_procThread;
		m_procThread = NULL;
	}

	if ( currAlgorithm!= NULL )
	{
		delete currAlgorithm;
		currAlgorithm = NULL;
	}

	if ( dataModel != NULL ){
		DataModel::deleteInstance ();
		dataModel = NULL;
	}
}

bool ImgAlgorithmPro::initServices ()
{
	initAlgorithmBox ( algorithmBoxFeature,algorithmFactory->getAlgorithmNames("Feature") );
	initAlgorithmBox ( algorithmBoxOutlierResolve, algorithmFactory->getAlgorithmNames ( "OutliersResolve" ) );
	initAlgorithmBox ( algorithmBoxMatch, algorithmFactory->getAlgorithmNames ( "Match" ) );
	return true;
}

bool ImgAlgorithmPro::readImage ( std::string path )
{
	QImage qimg;
	if ( !getImage ( QString::fromStdString ( path ), qimg ) )
		return false;
	showImage ( qimg, imgView );
	return true;
}

void ImgAlgorithmPro::updateTreeView ( QTreeWidget* tree ,QStringList& fileList  )
{
	initDlg.setWindowTitle ( QString::fromLocal8Bit ( "导入文件进度" ) );
	initDlg.setBtnTip ( QString::fromLocal8Bit ( "放弃继续导入文件！" ) );
	initDlg.setInfo ( fileList.count () );
	initDlg.setProcStatus ( false );
	initDlg.show ();
	int i = 1;

	foreach ( QString file, fileList ){
		QString name;
		getFileName ( file, "", name );

		addTreeWidget ( tree, name );

		QCoreApplication::processEvents ();
		initDlg.updateProgress ( i * 100 / fileList.count () );
		initDlg.updateLabel ( i );
		initDlg.setLabelFile ( name );
		i++;
	}
	initDlg.setProcStatus ( true );
	initDlg.close ();
}

//负责ui控件的初始化 、 信号槽的链接、 本地服务设置
bool ImgAlgorithmPro::init ()
{
	imgView = NULL;
	obj_treeWidget = NULL;
	scene_treeWidget = NULL;

	algorithmBoxFeature = ui.cBb_algorithmList_feature;
	algorithmBoxOutlierResolve = ui.cBb_algorithmList_OutlierResolve;
	algorithmBoxMatch = ui.cBb_algorithmList_match;

	if ( !initLocalConnection ( QString::fromLocal8Bit ( "ImgAlgorithmPro" ) ) )
		return false;

	initViewController ();
	initTreeView ();

	//ui组件的信号槽设置
	connect ( ui.treeWidget_Obj, SIGNAL ( itemClicked ( QTreeWidgetItem*, int ) ), this, SLOT ( objTreeItemChanged ( QTreeWidgetItem*, int ) ) );
	connect ( ui.treeWidget_Scene, SIGNAL ( itemClicked ( QTreeWidgetItem*, int ) ), this, SLOT ( sceneTreeItemChanged ( QTreeWidgetItem*, int ) ) );
	connect ( ui.pBtn_exit, SIGNAL ( clicked () ), this, SLOT ( dialog_exit () ) );
	connect ( ui.pBtn_minimize, SIGNAL ( clicked () ), this, SLOT ( dialog_minimize () ) );
	connect ( &waitingDlg, SIGNAL ( procStop () ), this, SLOT ( closeWaitDlg () ) );
	connect ( ui.cBb_algorithmList_feature , SIGNAL ( currentIndexChanged ( const QString & )) , this , SLOT( on_cBb_feature_selected ( const QString & ) ));
	connect ( ui.pBtn_open_Obj , SIGNAL ( click () ) , this , SLOT ( on_pBtn_open_Obj_clicked () ) );
	connect ( ui.pBtn_open_Scene , SIGNAL ( click () ) , this , SLOT ( on_pBtn_open_Scene_clicked () ) );

	//工作组件的信号槽设置
	qRegisterMetaType<Mat> ( "Mat" );
 	connect ( m_procThread, SIGNAL ( threadEnd () ), this, SLOT ( getThreadEnd () ) );
 	connect ( m_procThread, SIGNAL ( threadFail () ), this, SLOT ( getThreadFail () ) );
	connect ( m_procThread, SIGNAL ( update ( QString, int, int ) ), this, SLOT ( updateWaitBar ( QString, int, int ) ) );
	connect ( m_procThread, SIGNAL ( sendDetectRst ( int, int, float, Mat ) ), this, SLOT ( dealProcessResult ( int, int, float, Mat ) ) );


	imgBg.load ( ":/ImgAlgorithmPro/Resources/bottomOfImage.png" );
	imgView->setQImage ( imgBg );
	clearData ();

	return true;
}

//代码创建自定义图片显示控件
void ImgAlgorithmPro::initViewController ()
{
	imgView = new Image_Controller ( ui.background );
	imgView->setGeometry ( QRect ( 550, 180, 520, 390 ) );
	imgView->setQImgFixedSize ( QSize ( 520, 390 ) );
	imgView->raise ();
	imgView->setStyleSheet ( QString::fromUtf8 ( "\n"
		"QScrollBar:vertical\n"
		"{\n"
		"    width:12px;\n"
		"    background:rgba(0,0,0,0%);\n"
		"    margin:0px,0px,0px,0px;\n"
		"    padding-top:12px;   \n"
		"    padding-bottom:12px;\n"
		"}\n"
		"QScrollBar::handle:vertical\n"
		"{\n"
		"    width:12px;\n"
		"	background-color: #03242e;\n"
		"    border-radius:5px;   \n"
		"    min-height:20;\n"
		"}\n"
		"QScrollBar::sub-line:vertical  \n"
		"{\n"
		"    height:12px;width:12px;\n"
		"	border-image: url(:/ImgAlgorithmPro/Resources/uparrow.png);\n"
		"    subcontrol-position:top;\n"
		"}\n"
		"QScrollBar::add-line:vertical{\n"
		"    height:12px;width:12px;\n"
		"    border-image: url(:/ImgAlgorithmPro/Resources/downarrow.png);\n"
		"    subcontrol-position:bottom;\n"
		"}\n"
		"\n"
		"QScrollBar:horizontal\n"
		"{\n"
		"    height:12px;\n"
		"    background:rgba(0,0,0,0%);\n"
		"    margin:0px,0px,0px,0px;\n"
		"    padding-left:12px;   \n"
		"    padding-right:12px;\n"
		"}\n"
		"QScrollBar::handle:horizontal\n"
		"{\n"
		"    height:12px;\n"
		"	background-color: #03242e;\n"
		"    border-radius:5px;   \n"
		"    min-width:20;\n"
		"}\n"
		"QScrollBar::sub-line:horizontal  \n"
		"{\n"
		"    height:12px;width:12px;\n"
		"	border-image: url(:/ImgAlgorithmPro/Resources/leftarrow.png);\n"
		"    subcontrol-position:left;\n"
		"}\n"
		"QScrollBar::add-line:horizontal{\n"
		"    height:12px;width:12px;\n"
		"    border-image: url(:/ImgAlgorithmPro/Resources/rightarrow.png);\n"
		"    subcontrol-position:right;\n"
		"}\n"
		"\n" ) );
}

void ImgAlgorithmPro::destroyViewController ()
{
	if ( imgView != NULL ){
		delete imgView;
		imgView = NULL;
	}
}

//初始化树形控件 用于以树结构显示要处理的图片文件
void ImgAlgorithmPro::initTreeView ()
{
	treeFont.setPointSize ( 9 );
	treeForeBrush.setColor ( QColor ( 178, 178, 178, 255 ) );
	treeForeBrush.setStyle ( Qt::NoBrush );
	treeBackBrush.setColor ( QColor ( 0, 0, 0, 255 ) );
	treeBackBrush.setStyle ( Qt::NoBrush );

	obj_treeWidget = ui.treeWidget_Obj;
	scene_treeWidget = ui.treeWidget_Scene;

	QRect zoom( 30, 290, 301, 190 );
	initTreeWidget ( obj_treeWidget ,"Object",zoom);
	zoom.setY ( 500 );
	zoom.setHeight ( 190 );
	initTreeWidget ( scene_treeWidget ,"Scene",zoom);
}

void ImgAlgorithmPro::initTreeWidget ( QTreeWidget* qTree, string treeName, QRect zoom )
{
	QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem ();
	__qtreewidgetitem->setTextAlignment ( 0, Qt::AlignHCenter | Qt::AlignVCenter | Qt::AlignCenter );
	__qtreewidgetitem->setText ( 0, QString::fromStdString ( treeName ) );
	qTree->setHeaderItem ( __qtreewidgetitem );
	qTree->setHorizontalScrollMode ( QAbstractItemView::ScrollPerPixel );
	qTree->setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded );
	qTree->setRootIsDecorated ( false );

	qTree->setGeometry ( zoom );
	qTree->setStyleSheet ( QString::fromUtf8 ( "QHeaderView::section{\n"
		"background-color:rgb(5, 51, 66);\n"
		"color: rgb(178, 178, 178);\n"
		"font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; \n"
		"}\n"
		"\n"
		"QTreeWidget {background-color: rgb(15, 61, 76);\n"
		"color: rgb(178, 178, 178);\n"
		"font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; \n"
		"border-radius:2px;\n"
		"border-bottom:1px solid black;\n"
		"\n"
		"}\n"
		"QTreeWidget::item:selected{\n"
		"        color:rgb(17, 125, 80);\n"
		"}\n"
		"\n"
		"QScrollBar:vertical\n"
		"{\n"
		"    width:18px;\n"
		"    background:rgba(0,0,0,0%);\n"
		"    margin:0px,0px,0px,0px;\n"
		"    padding-top:18px;   \n"
		"    padding-bottom:18px;\n"
		"}\n"
		"QScrollBar::handle:vertical\n"
		"{\n"
		"    width:18px;\n"
		"	background-color: #03242e;\n"
		"    border-radius:18px;   \n"
		"    min-height:24px;\n"
		"}\n"
		"QScrollBar::sub-line:vertical  \n"
		"{\n"
		"    height:18px;width:18px;\n"
		"	border-image: url(:/ImgAlgorithmPro/Resources/uparrow.png);\n"
		"    subcontrol-position:top;\n"
		"}\n"
		"QScrollBar::add-line:vertical{\n"
		""
		"    height:18px;width:18px;\n"
		"    border-image: url(:/ImgAlgorithmPro/Resources/downarrow.png);\n"
		"    subcontrol-position:bottom;\n"
		"}\n"
		"\n"
		"QScrollBar:horizontal\n"
		"{\n"
		"    height:18px;\n"
		"    background:rgba(0,0,0,0%);\n"
		"    margin:0px,0px,0px,0px;\n"
		"    padding-left:18px;   \n"
		"    padding-right:18px;\n"
		"}\n"
		"QScrollBar::handle:horizontal\n"
		"{\n"
		"    width:18px;\n"
		"	background-color: #03242e;\n"
		"    border-radius:18px;   \n"
		"    min-width:18px;\n"
		"}\n"
		"QScrollBar::sub-line:horizontal  \n"
		"{\n"
		"    height:18px;width:18px;\n"
		"	border-image: url(:/ImgAlgorithmPro/Resources/leftarrow.png);\n"
		"    subcontrol-position:left;\n"
		"}\n"
		"QScrollBar::add-line:horizontal{\n"
		"    height:18px;width:18px;\n"
		"    border-image: url(:/ImgAlgorithmPro/Resources/rightarrow.png);\n"
		"    subcontrol-position:right;\n"
		"}" ) );
	qTree->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
	qTree->setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded );
}

void ImgAlgorithmPro::addTreeWidget ( QTreeWidget* item, QString qstr )
{
	QTreeWidgetItem * child = new QTreeWidgetItem ( item );
	child->setText ( 0, qstr );
	child->setForeground ( 0, treeForeBrush );
	child->setBackground ( 0, treeBackBrush );
	QFont font;
	font.setPointSize ( 9 );
	child->setFont ( 0, font );
}

void ImgAlgorithmPro::delAllTreeWidget ( QTreeWidget* item )
{
	item->clear ();
}

void ImgAlgorithmPro::destroyTreeView ()
{
	delAllTreeWidget ( obj_treeWidget );
	delAllTreeWidget ( scene_treeWidget );
}

void ImgAlgorithmPro::setImageView ( QString imgPath )
{
	if ( imgPath.isEmpty() )
	{
		imgView->setQImage ( imgBg );
		return;
	}
	imgWatch.load ( imgPath );
	imgView->setQImage ( imgWatch );
}

void ImgAlgorithmPro::showImage ( cv::Mat img, Image_Controller* imgCtl )
{
	QImage qimage = mat_to_qimage_cpy ( img, 0 );
	imgCtl->setQImage ( qimage );
}

void ImgAlgorithmPro::showImage ( QImage& qimage, Image_Controller* imgCtl ){
	imgCtl->setQImage ( qimage );
}

void ImgAlgorithmPro::initAlgorithmBox ( QComboBox* box, vector<string> algorithmNames )
{
	for ( int i = 0; i < algorithmNames.size (); ++i )
		box->addItem ( QString::fromStdString ( algorithmNames[i] ) );
}

void ImgAlgorithmPro::clearData ()
{
	ResultTime = 0.0;
	for ( int i = 0; i < 2; ++i ){
		ResultCnt[i] = 0;
	}

	objFileList.clear ();
	delAllTreeWidget ( obj_treeWidget );
	delAllTreeWidget ( scene_treeWidget );
	sceneFileList.clear ();
}

void ImgAlgorithmPro::outputResult ()
{
	ui.label_fail->setText ( QString::number ( ResultCnt[1] ) );
	ui.label_pass->setText ( QString::number ( ResultCnt[0] ) );
	int total = 0;
	for ( int i = 0; i < 2; ++i ){
		total += ResultCnt[i];
	}
	ui.label_all->setText ( QString::number ( total ) );
	ui.label_total->setText ( QString::number ( ResultTime / 1000, 'g', 2 ) );
	ui.label_avg->setText ( QString::number ( ResultTime / 1000 / ( float ) total, 'g', 2 ) );
}

void getFolderName ( const QString& path, QString& name )
{
	QStringList qstrl = path.split ( "\\" );
	if ( 1 == qstrl.count () ){
		qstrl = path.split ( "/" );
		name = qstrl.at ( qstrl.count () - 2 );
	}
	else{
		name = qstrl.at ( qstrl.count () - 2 );
	}
}

//点击树节点时  将对应文件路径的图片展示到img控件
void ImgAlgorithmPro::objTreeItemChanged ( QTreeWidgetItem* item, int i )
{
	if ( NULL == item )
		return;

	int itemIndex = obj_treeWidget->indexOfTopLevelItem ( item );

	QString qstr = objFileList.at ( itemIndex );

	setImageView ( qstr );
}

void ImgAlgorithmPro::sceneTreeItemChanged ( QTreeWidgetItem* item, int i )
{
	if ( NULL == item )
		return;

	int itemIndex = scene_treeWidget->indexOfTopLevelItem ( item );

	QString qstr = sceneFileList.at ( itemIndex );

	setImageView ( qstr );
}

//停止工作线程时   将运行状态设为false  这样工作线程处理完当前图片后  就不会继续处理
void ImgAlgorithmPro::closeWaitDlg ()
{
	if ( m_procThread->isRunning () )
		m_procThread->status = false;
	waitingDlg.close ();
}

void getImgFilesAbsolutePath ( QString dir , QStringList &fileList )
{
	QStringList fileType;
	fileType << QString::fromLocal8Bit ( "*.jpg" ) << QString::fromLocal8Bit ( "*.png" )
		<< QString::fromLocal8Bit ( "*.ppm" ) << QString::fromLocal8Bit ( "*.gif" );
	QDir fileDir ( dir );
	QStringList fileFiles = fileDir.entryList (
		fileType , QDir::Files );
	for ( int i = 0; i < fileFiles.size (); ++i )
		fileFiles[i] = dir + "/" + fileFiles[i];

	fileList = fileFiles;
}

void ImgAlgorithmPro::on_pBtn_open_Obj_clicked ()
{
	////打开文件夹
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	QString directory = QFileDialog::getExistingDirectory ( this,
		QString::fromLocal8Bit ( "选择需要导入的文件夹" ),
		"./",
		options );
	if ( directory.isEmpty () )
		return;

	ui.IEd_InputFolder_Obj->setText ( directory );

	getImgFilesAbsolutePath ( directory , objFileList );
	delAllTreeWidget ( obj_treeWidget );
	updateTreeView ( obj_treeWidget , objFileList );
}

void ImgAlgorithmPro::on_pBtn_open_Scene_clicked ()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	QString directory = QFileDialog::getExistingDirectory ( this ,
															QString::fromLocal8Bit ( "选择需要导入的文件夹" ) ,
															"./" ,
															options );
	if ( directory.isEmpty () )
		return;

	ui.IEd_InputFolder_Scene->setText ( directory );
	
	getImgFilesAbsolutePath ( directory , sceneFileList );
	delAllTreeWidget ( scene_treeWidget );
	updateTreeView ( scene_treeWidget , sceneFileList );
}

void ImgAlgorithmPro::on_pBtn_detect_clicked ()
{
	//算法有效性检查
	QString feature , outliersResolve , match;
	feature = ui.cBb_algorithmList_feature->currentText ();
	outliersResolve = ui.cBb_algorithmList_OutlierResolve->currentText ();
	match = ui.cBb_algorithmList_match->currentText ();
	if ( feature.isEmpty() || (( feature != "FERNS" )&&( outliersResolve.isEmpty() || match.isEmpty()) ))
	{
		QMessageBox::information ( this, QString::fromLocal8Bit ( "提示" ), QString::fromLocal8Bit ( "请先选择使用的算法" ) );
			return;
	}

	//数据有效性检查
	int objFileIndex = obj_treeWidget->indexOfTopLevelItem ( obj_treeWidget->currentItem () );
	int sceneFileIndex = scene_treeWidget->indexOfTopLevelItem ( scene_treeWidget->currentItem () );
	if ( objFileIndex < 0 || sceneFileIndex < 0 )
	{
		QMessageBox::information ( this , QString::fromLocal8Bit ( "提示" ) , QString::fromLocal8Bit ( "请先选择使用的目标图片和场景图片" ) );
		return;
	}


	if ( currAlgorithm ) delete currAlgorithm;
	currAlgorithm = algorithmFactory->getImgMatchAlgorithm ( feature.toStdString () , outliersResolve.toStdString () , match.toStdString () );

	//进度框设置显示
	waitingDlg.setWindowTitle ( QString::fromLocal8Bit ( "当前文件处理进度" ) );
	waitingDlg.setBtnTip ( QString::fromLocal8Bit ( "放弃继续处理文件！" ) );
	waitingDlg.setInfo ( 1 );
	waitingDlg.setProcStatus ( false );
	waitingDlg.show ();


	QStringList fileList;
	fileList << objFileList.at ( objFileIndex )
		<< sceneFileList.at ( sceneFileIndex );

	m_procThread->init ( currAlgorithm, fileList);
	m_procThread->start ();
}

void ImgAlgorithmPro::on_cBb_feature_selected( const QString& )
{
	if ( ui.cBb_algorithmList_feature->currentText() == "FERNS")
	{
		ui.cBb_algorithmList_match->setDisabled ( true );
		ui.cBb_algorithmList_OutlierResolve->setDisabled ( true );
	}
	else
	{
		ui.cBb_algorithmList_match->setDisabled ( false );
		ui.cBb_algorithmList_OutlierResolve->setDisabled ( false );
	}
}

void ImgAlgorithmPro::getThreadEnd ()
{
	//outputResult ();

	waitingDlg.setBtnTip ( QString::fromLocal8Bit ( "文件处理完毕！" ) );
	waitingDlg.setProcStatus ( true );
	closeWaitDlg ();
}

void ImgAlgorithmPro::getThreadFail ()
{
	//outputResult ();

	QMessageBox::information ( 0, QString::fromLocal8Bit ( "提示" ),
		QString::fromLocal8Bit ( "0匹配，处理已取消！" ) );
	closeWaitDlg ();
}

void ImgAlgorithmPro::updateWaitBar ( QString file, int n, int i )
{
	waitingDlg.setLabelFile ( file );
	waitingDlg.updateLabel ( i );
	waitingDlg.updateProgress ( i * 100 / n );
}

//成功 展示匹配图片  失败 展示特征图片     更新ui控件 
void ImgAlgorithmPro::dealProcessResult ( int i, int rst, float time, Mat dst )
{
	QImage curr;
	curr = mat_to_qimage_cpy ( dst, 0);

	
	showImage ( curr, imgView );

	//0 pass 1 failed
	ResultCnt[ 1 - ( rst==AlgorithmResult::AR_DONE )]++;
	ResultTime += time;
	//结果展示
	outputResult ();
}
