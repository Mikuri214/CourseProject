/********************************************************************************
** Form generated from reading UI file 'WaitingProgressBar.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITINGPROGRESSBAR_H
#define UI_WAITINGPROGRESSBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaitingProgressBar
{
public:
    QWidget *widget;
    QLabel *label_file;
    QLabel *label;
    QCommandLinkButton *cmdLinkBtn;
    QProgressBar *progressBar;
    QLabel *label_progress;
    QPushButton *pBtn_minimize;
    QPushButton *pBtn_exit;

    void setupUi(QWidget *WaitingProgressBar)
    {
        if (WaitingProgressBar->objectName().isEmpty())
            WaitingProgressBar->setObjectName(QStringLiteral("WaitingProgressBar"));
        WaitingProgressBar->setWindowModality(Qt::ApplicationModal);
        WaitingProgressBar->resize(540, 310);
        WaitingProgressBar->setMinimumSize(QSize(540, 310));
        WaitingProgressBar->setMaximumSize(QSize(540, 310));
        widget = new QWidget(WaitingProgressBar);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 0, 540, 310));
        widget->setMinimumSize(QSize(540, 310));
        widget->setMaximumSize(QSize(540, 310));
        widget->setStyleSheet(QLatin1String("QWidget#widget{border-image: url(:/ImgAlgorithmPro/Resources/bottom.png)}\n"
""));
        label_file = new QLabel(widget);
        label_file->setObjectName(QStringLiteral("label_file"));
        label_file->setGeometry(QRect(185, 60, 211, 41));
        label_file->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(178, 178, 178);"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 60, 171, 41));
        label->setStyleSheet(QString::fromUtf8("font: 75 8pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(178, 178, 178);"));
        cmdLinkBtn = new QCommandLinkButton(widget);
        cmdLinkBtn->setObjectName(QStringLiteral("cmdLinkBtn"));
        cmdLinkBtn->setGeometry(QRect(260, 230, 185, 41));
        cmdLinkBtn->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(170, 255, 255);"));
        progressBar = new QProgressBar(widget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(60, 150, 411, 23));
        progressBar->setStyleSheet(QString::fromUtf8("QProgressBar{\n"
"font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"border-radius:2px;\n"
"color:#fff;\n"
"	background-color:#156079;\n"
"}\n"
"QProgressBar::chunk {\n"
"	background-color: rgb(52, 52, 52);\n"
"    border-bottom:1px solid gray;\n"
"}"));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(true);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setTextDirection(QProgressBar::TopToBottom);
        label_progress = new QLabel(widget);
        label_progress->setObjectName(QStringLiteral("label_progress"));
        label_progress->setGeometry(QRect(350, 90, 161, 41));
        label_progress->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(178, 178, 178);"));
        label_progress->setAlignment(Qt::AlignCenter);
        pBtn_minimize = new QPushButton(widget);
        pBtn_minimize->setObjectName(QStringLiteral("pBtn_minimize"));
        pBtn_minimize->setGeometry(QRect(480, 10, 20, 20));
        pBtn_minimize->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/ImgAlgorithmPro/Resources/minimize.png);}\n"
"QPushButton:hover{border-image: url(:/ImgAlgorithmPro/Resources/minimize-mouse.png);}\n"
"QPushButton:pressed{border-image: url(:/ImgAlgorithmPro/Resources/minimize-mouse.png);}\n"
""));
        pBtn_minimize->setIconSize(QSize(25, 25));
        pBtn_exit = new QPushButton(widget);
        pBtn_exit->setObjectName(QStringLiteral("pBtn_exit"));
        pBtn_exit->setGeometry(QRect(510, 10, 20, 20));
        pBtn_exit->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/ImgAlgorithmPro/Resources/exit.png);}\n"
"QPushButton:hover{border-image: url(:/ImgAlgorithmPro/Resources/exit-mouse.png);}\n"
"QPushButton:pressed{border-image: url(:/ImgAlgorithmPro/Resources/exit-mouse.png);}"));
        pBtn_exit->setIconSize(QSize(25, 25));

        retranslateUi(WaitingProgressBar);

        QMetaObject::connectSlotsByName(WaitingProgressBar);
    } // setupUi

    void retranslateUi(QWidget *WaitingProgressBar)
    {
        WaitingProgressBar->setWindowTitle(QApplication::translate("WaitingProgressBar", "Form", Q_NULLPTR));
        label_file->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p><span style=\" font-size:14pt;\">\346\226\207\344\273\266\345\220\215</span></p></body></html>", Q_NULLPTR));
        label->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p><span style=\" font-size:14pt;\">\345\275\223\345\211\215\345\244\204\347\220\206\346\226\207\344\273\266\357\274\232</span></p></body></html>", Q_NULLPTR));
        cmdLinkBtn->setText(QApplication::translate("WaitingProgressBar", "\346\224\276\345\274\203\347\273\247\347\273\255\350\277\233\350\241\214\345\244\204\347\220\206", Q_NULLPTR));
        label_progress->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p><br/></p></body></html>", Q_NULLPTR));
        pBtn_minimize->setText(QString());
        pBtn_exit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WaitingProgressBar: public Ui_WaitingProgressBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITINGPROGRESSBAR_H
