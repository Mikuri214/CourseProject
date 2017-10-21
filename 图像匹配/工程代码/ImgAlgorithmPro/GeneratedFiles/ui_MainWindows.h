/********************************************************************************
** Form generated from reading UI file 'MainWindows.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWS_H
#define UI_MAINWINDOWS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowsClass
{
public:
    QWidget *background;
    QPushButton *pBtn_open_Obj;
    QLineEdit *IEd_InputFolder_Obj;
    QPushButton *pBtn_detect;
    QTreeWidget *treeWidget_Obj;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_pass;
    QLabel *label_all;
    QFrame *line;
    QLabel *label_6;
    QLabel *label_9;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_14;
    QLabel *label_15;
    QFrame *line_2;
    QLabel *label_total;
    QLabel *label_avg;
    QLabel *label_5;
    QLabel *label_10;
    QLabel *label_fail;
    QPushButton *pBtn_selectAlgorithm;
    QComboBox *cBb_algorithmList_feature;
    QLabel *label_11;
    QComboBox *cBb_algorithmList_OutlierResolve;
    QTreeWidget *treeWidget_Scene;
    QLineEdit *IEd_InputFolder_Scene;
    QPushButton *pBtn_open_Scene;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_16;
    QLabel *label_17;
    QComboBox *cBb_algorithmList_match;
    QPushButton *pBtn_exit;
    QPushButton *pBtn_minimize;

    void setupUi(QDialog *MainWindowsClass)
    {
        if (MainWindowsClass->objectName().isEmpty())
            MainWindowsClass->setObjectName(QStringLiteral("MainWindowsClass"));
        MainWindowsClass->resize(1080, 720);
        MainWindowsClass->setMinimumSize(QSize(1080, 720));
        MainWindowsClass->setMaximumSize(QSize(1080, 720));
        background = new QWidget(MainWindowsClass);
        background->setObjectName(QStringLiteral("background"));
        background->setGeometry(QRect(0, 0, 1080, 720));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(13);
        background->setFont(font);
        background->setAutoFillBackground(false);
        background->setStyleSheet(QStringLiteral("QWidget#background{background-image: url(:/ImgAlgorithmPro/Resources/background.png);}"));
        pBtn_open_Obj = new QPushButton(background);
        pBtn_open_Obj->setObjectName(QStringLiteral("pBtn_open_Obj"));
        pBtn_open_Obj->setGeometry(QRect(30, 90, 80, 30));
        pBtn_open_Obj->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/ImgAlgorithmPro/Resources/input-normal-x30y110.png);}\n"
"QPushButton:hover{border-image: url(:/ImgAlgorithmPro/Resources/input-hover.png);}\n"
"QPushButton:pressed{border-image: url(:/ImgAlgorithmPro/Resources/input-pressed.png);}"));
        IEd_InputFolder_Obj = new QLineEdit(background);
        IEd_InputFolder_Obj->setObjectName(QStringLiteral("IEd_InputFolder_Obj"));
        IEd_InputFolder_Obj->setGeometry(QRect(110, 90, 412, 30));
        QPalette palette;
        QBrush brush(QColor(64, 62, 62, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        IEd_InputFolder_Obj->setPalette(palette);
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(11);
        IEd_InputFolder_Obj->setFont(font1);
        IEd_InputFolder_Obj->setStyleSheet(QString::fromUtf8("text-indent:5px;\n"
"font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #403e3e;\n"
"background-color: #ffffff;"));
        pBtn_detect = new QPushButton(background);
        pBtn_detect->setObjectName(QStringLiteral("pBtn_detect"));
        pBtn_detect->setGeometry(QRect(345, 250, 175, 35));
        pBtn_detect->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/ImgAlgorithmPro/Resources/start-normal-x341y200.png);}\n"
"QPushButton:hover{border-image: url(:/ImgAlgorithmPro/Resources/start-hover.png);}\n"
"QPushButton:pressed{border-image: url(:/ImgAlgorithmPro/Resources/start-pressed.png);}"));
        treeWidget_Obj = new QTreeWidget(background);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setTextAlignment(0, Qt::AlignCenter);
        treeWidget_Obj->setHeaderItem(__qtreewidgetitem);
        treeWidget_Obj->setObjectName(QStringLiteral("treeWidget_Obj"));
        treeWidget_Obj->setGeometry(QRect(30, 290, 300, 190));
        treeWidget_Obj->setStyleSheet(QString::fromUtf8("QHeaderView::section{\n"
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
"}\n"
""));
        treeWidget_Obj->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        label = new QLabel(background);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(349, 290, 161, 50));
        QPalette palette1;
        QBrush brush2(QColor(178, 178, 178, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        label->setPalette(palette1);
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(13);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(9);
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 13pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(background);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(360, 360, 40, 40));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        label_2->setPalette(palette2);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_3 = new QLabel(background);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(360, 410, 40, 40));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_4 = new QLabel(background);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(480, 360, 20, 40));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_pass = new QLabel(background);
        label_pass->setObjectName(QStringLiteral("label_pass"));
        label_pass->setGeometry(QRect(410, 410, 70, 40));
        label_pass->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_pass->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_all = new QLabel(background);
        label_all->setObjectName(QStringLiteral("label_all"));
        label_all->setGeometry(QRect(410, 360, 70, 40));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        label_all->setPalette(palette3);
        label_all->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_all->setFrameShape(QFrame::NoFrame);
        label_all->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        line = new QFrame(background);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(350, 320, 160, 40));
        line->setStyleSheet(QStringLiteral("background-color: rgb(5, 51, 66);"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_6 = new QLabel(background);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(480, 410, 20, 40));
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_9 = new QLabel(background);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(360, 630, 81, 41));
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_7 = new QLabel(background);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(360, 590, 61, 41));
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_8 = new QLabel(background);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(350, 530, 161, 51));
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 13pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_8->setAlignment(Qt::AlignCenter);
        label_14 = new QLabel(background);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(480, 590, 20, 41));
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(background);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(480, 630, 20, 41));
        label_15->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        line_2 = new QFrame(background);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(350, 560, 161, 41));
        line_2->setStyleSheet(QStringLiteral("background-color: rgb(5, 51, 66);"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_total = new QLabel(background);
        label_total->setObjectName(QStringLiteral("label_total"));
        label_total->setGeometry(QRect(420, 590, 61, 41));
        label_total->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_total->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_avg = new QLabel(background);
        label_avg->setObjectName(QStringLiteral("label_avg"));
        label_avg->setGeometry(QRect(440, 630, 41, 41));
        label_avg->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_avg->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_5 = new QLabel(background);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(360, 450, 40, 40));
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_10 = new QLabel(background);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(480, 450, 20, 40));
        label_10->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_fail = new QLabel(background);
        label_fail->setObjectName(QStringLiteral("label_fail"));
        label_fail->setGeometry(QRect(410, 450, 70, 40));
        label_fail->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_fail->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pBtn_selectAlgorithm = new QPushButton(background);
        pBtn_selectAlgorithm->setObjectName(QStringLiteral("pBtn_selectAlgorithm"));
        pBtn_selectAlgorithm->setGeometry(QRect(30, 210, 80, 30));
        pBtn_selectAlgorithm->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/ImgAlgorithmPro/Resources/select-normal-x30y160.png);}\n"
"QPushButton:hover{border-image: url(:/ImgAlgorithmPro/Resources/select-hover.png);}\n"
"QPushButton:pressed{border-image: url(:/ImgAlgorithmPro/Resources/select-pressed.png);}"));
        cBb_algorithmList_feature = new QComboBox(background);
        cBb_algorithmList_feature->setObjectName(QStringLiteral("cBb_algorithmList_feature"));
        cBb_algorithmList_feature->setGeometry(QRect(140, 210, 91, 30));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cBb_algorithmList_feature->sizePolicy().hasHeightForWidth());
        cBb_algorithmList_feature->setSizePolicy(sizePolicy);
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush3(QColor(252, 252, 252, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        cBb_algorithmList_feature->setPalette(palette4);
        cBb_algorithmList_feature->setFont(font1);
        cBb_algorithmList_feature->setMouseTracking(true);
        cBb_algorithmList_feature->setAcceptDrops(true);
        cBb_algorithmList_feature->setStyleSheet(QString::fromUtf8("text-indent:5px;\n"
"font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #403e3e;\n"
"background-color: #fcfcfc;"));
        cBb_algorithmList_feature->setIconSize(QSize(16, 16));
        label_11 = new QLabel(background);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(230, 210, 21, 30));
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cBb_algorithmList_OutlierResolve = new QComboBox(background);
        cBb_algorithmList_OutlierResolve->setObjectName(QStringLiteral("cBb_algorithmList_OutlierResolve"));
        cBb_algorithmList_OutlierResolve->setGeometry(QRect(270, 210, 101, 30));
        sizePolicy.setHeightForWidth(cBb_algorithmList_OutlierResolve->sizePolicy().hasHeightForWidth());
        cBb_algorithmList_OutlierResolve->setSizePolicy(sizePolicy);
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        cBb_algorithmList_OutlierResolve->setPalette(palette5);
        cBb_algorithmList_OutlierResolve->setFont(font1);
        cBb_algorithmList_OutlierResolve->setMouseTracking(true);
        cBb_algorithmList_OutlierResolve->setAcceptDrops(true);
        cBb_algorithmList_OutlierResolve->setStyleSheet(QString::fromUtf8("text-indent:5px;\n"
"font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #403e3e;\n"
"background-color: #fcfcfc;"));
        cBb_algorithmList_OutlierResolve->setIconSize(QSize(16, 16));
        treeWidget_Scene = new QTreeWidget(background);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setTextAlignment(0, Qt::AlignCenter);
        treeWidget_Scene->setHeaderItem(__qtreewidgetitem1);
        treeWidget_Scene->setObjectName(QStringLiteral("treeWidget_Scene"));
        treeWidget_Scene->setGeometry(QRect(30, 500, 300, 190));
        treeWidget_Scene->setStyleSheet(QString::fromUtf8("QHeaderView::section{\n"
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
"}\n"
""));
        treeWidget_Scene->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        IEd_InputFolder_Scene = new QLineEdit(background);
        IEd_InputFolder_Scene->setObjectName(QStringLiteral("IEd_InputFolder_Scene"));
        IEd_InputFolder_Scene->setGeometry(QRect(110, 130, 412, 30));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        IEd_InputFolder_Scene->setPalette(palette6);
        IEd_InputFolder_Scene->setFont(font1);
        IEd_InputFolder_Scene->setStyleSheet(QString::fromUtf8("text-indent:5px;\n"
"font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #403e3e;\n"
"background-color: #ffffff;"));
        pBtn_open_Scene = new QPushButton(background);
        pBtn_open_Scene->setObjectName(QStringLiteral("pBtn_open_Scene"));
        pBtn_open_Scene->setGeometry(QRect(30, 130, 80, 30));
        pBtn_open_Scene->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/ImgAlgorithmPro/Resources/input-normal-x30y110.png);}\n"
"QPushButton:hover{border-image: url(:/ImgAlgorithmPro/Resources/input-hover.png);}\n"
"QPushButton:pressed{border-image: url(:/ImgAlgorithmPro/Resources/input-pressed.png);}"));
        label_12 = new QLabel(background);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(170, 170, 30, 30));
        label_12->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_13 = new QLabel(background);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(290, 170, 61, 30));
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_16 = new QLabel(background);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(380, 210, 21, 30));
        label_16->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_17 = new QLabel(background);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(440, 170, 31, 30));
        label_17->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cBb_algorithmList_match = new QComboBox(background);
        cBb_algorithmList_match->setObjectName(QStringLiteral("cBb_algorithmList_match"));
        cBb_algorithmList_match->setGeometry(QRect(420, 210, 101, 30));
        sizePolicy.setHeightForWidth(cBb_algorithmList_match->sizePolicy().hasHeightForWidth());
        cBb_algorithmList_match->setSizePolicy(sizePolicy);
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        cBb_algorithmList_match->setPalette(palette7);
        cBb_algorithmList_match->setFont(font1);
        cBb_algorithmList_match->setMouseTracking(true);
        cBb_algorithmList_match->setAcceptDrops(true);
        cBb_algorithmList_match->setStyleSheet(QString::fromUtf8("text-indent:5px;\n"
"font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #403e3e;\n"
"background-color: #fcfcfc;"));
        cBb_algorithmList_match->setIconSize(QSize(16, 16));
        line_2->raise();
        line->raise();
        pBtn_open_Obj->raise();
        IEd_InputFolder_Obj->raise();
        pBtn_detect->raise();
        treeWidget_Obj->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        label_pass->raise();
        label_all->raise();
        label_6->raise();
        label_9->raise();
        label_7->raise();
        label_8->raise();
        label_14->raise();
        label_15->raise();
        label_total->raise();
        label_avg->raise();
        label_5->raise();
        label_10->raise();
        label_fail->raise();
        pBtn_selectAlgorithm->raise();
        cBb_algorithmList_feature->raise();
        label_11->raise();
        cBb_algorithmList_OutlierResolve->raise();
        treeWidget_Scene->raise();
        IEd_InputFolder_Scene->raise();
        pBtn_open_Scene->raise();
        label_12->raise();
        label_13->raise();
        label_16->raise();
        label_17->raise();
        cBb_algorithmList_match->raise();
        pBtn_exit = new QPushButton(MainWindowsClass);
        pBtn_exit->setObjectName(QStringLiteral("pBtn_exit"));
        pBtn_exit->setGeometry(QRect(1050, 10, 20, 20));
        pBtn_exit->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/ImgAlgorithmPro/Resources/exit.png);}\n"
"QPushButton:hover{border-image: url(:/ImgAlgorithmPro/Resources/exit-mouse.png);}\n"
"QPushButton:pressed{border-image: url(:/ImgAlgorithmPro/Resources/exit-mouse.png);}\n"
""));
        pBtn_minimize = new QPushButton(MainWindowsClass);
        pBtn_minimize->setObjectName(QStringLiteral("pBtn_minimize"));
        pBtn_minimize->setGeometry(QRect(1020, 10, 20, 20));
        pBtn_minimize->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/ImgAlgorithmPro/Resources/minimize.png);}\n"
"QPushButton:hover{border-image: url(:/ImgAlgorithmPro/Resources/minimize-mouse.png);}\n"
"QPushButton:pressed{border-image: url(:/ImgAlgorithmPro/Resources/minimize-mouse.png);}\n"
""));

        retranslateUi(MainWindowsClass);

        QMetaObject::connectSlotsByName(MainWindowsClass);
    } // setupUi

    void retranslateUi(QDialog *MainWindowsClass)
    {
        MainWindowsClass->setWindowTitle(QApplication::translate("MainWindowsClass", "ImgAlgorithmPro", Q_NULLPTR));
        pBtn_open_Obj->setText(QString());
        IEd_InputFolder_Obj->setInputMask(QString());
        IEd_InputFolder_Obj->setText(QApplication::translate("MainWindowsClass", "\347\211\251\344\275\223\345\233\276\347\211\207\351\233\206\347\233\256\345\275\225\350\267\257\345\276\204", Q_NULLPTR));
        pBtn_detect->setText(QString());
        QTreeWidgetItem *___qtreewidgetitem = treeWidget_Obj->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindowsClass", "\346\226\207\344\273\266\345\220\215", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindowsClass", "\346\243\200\346\265\213\347\273\223\346\236\234", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindowsClass", "\345\205\261\346\234\211\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindowsClass", "\345\256\214\346\210\220\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindowsClass", "\345\274\240", Q_NULLPTR));
        label_pass->setText(QApplication::translate("MainWindowsClass", "0", Q_NULLPTR));
        label_all->setText(QApplication::translate("MainWindowsClass", "0", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindowsClass", "\345\274\240", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindowsClass", "\345\271\263\345\235\207\350\200\227\346\227\266\357\274\232", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindowsClass", "\346\200\273\350\200\227\346\227\266\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindowsClass", "\346\227\266\351\227\264\347\273\237\350\256\241", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindowsClass", "\347\247\222", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindowsClass", "\347\247\222", Q_NULLPTR));
        label_total->setText(QApplication::translate("MainWindowsClass", "0", Q_NULLPTR));
        label_avg->setText(QApplication::translate("MainWindowsClass", "0", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindowsClass", "\345\207\272\351\224\231\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindowsClass", "\345\274\240", Q_NULLPTR));
        label_fail->setText(QApplication::translate("MainWindowsClass", "0", Q_NULLPTR));
        pBtn_selectAlgorithm->setText(QString());
        cBb_algorithmList_feature->setCurrentText(QString());
        label_11->setText(QApplication::translate("MainWindowsClass", "+", Q_NULLPTR));
        cBb_algorithmList_OutlierResolve->setCurrentText(QString());
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget_Scene->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindowsClass", "\346\226\207\344\273\266\345\220\215", Q_NULLPTR));
        IEd_InputFolder_Scene->setInputMask(QString());
        IEd_InputFolder_Scene->setText(QApplication::translate("MainWindowsClass", "\345\234\272\346\231\257\345\233\276\347\211\207\351\233\206\347\233\256\345\275\225\350\267\257\345\276\204", Q_NULLPTR));
        pBtn_open_Scene->setText(QString());
        label_12->setText(QApplication::translate("MainWindowsClass", "\347\211\271\345\276\201", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindowsClass", "\351\207\216\347\202\271\346\266\210\351\231\244", Q_NULLPTR));
        label_16->setText(QApplication::translate("MainWindowsClass", "+", Q_NULLPTR));
        label_17->setText(QApplication::translate("MainWindowsClass", "\345\214\271\351\205\215", Q_NULLPTR));
        cBb_algorithmList_match->setCurrentText(QString());
        pBtn_exit->setText(QString());
        pBtn_minimize->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindowsClass: public Ui_MainWindowsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWS_H
