/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *svgView;
    QLabel *labelStatus;
    QPushButton *btnParse;
    QPushButton *btnLoad;
    QTextEdit *textTokens;
    QPushButton *btnPNG;
    QPushButton *btnPDF;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1503, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        svgView = new QGraphicsView(centralwidget);
        svgView->setObjectName("svgView");
        svgView->setGeometry(QRect(280, 0, 1181, 621));
        labelStatus = new QLabel(centralwidget);
        labelStatus->setObjectName("labelStatus");
        labelStatus->setGeometry(QRect(30, 10, 111, 20));
        btnParse = new QPushButton(centralwidget);
        btnParse->setObjectName("btnParse");
        btnParse->setGeometry(QRect(280, 630, 90, 29));
        btnLoad = new QPushButton(centralwidget);
        btnLoad->setObjectName("btnLoad");
        btnLoad->setGeometry(QRect(30, 630, 90, 29));
        textTokens = new QTextEdit(centralwidget);
        textTokens->setObjectName("textTokens");
        textTokens->setGeometry(QRect(30, 40, 241, 581));
        btnPNG = new QPushButton(centralwidget);
        btnPNG->setObjectName("btnPNG");
        btnPNG->setGeometry(QRect(370, 630, 90, 29));
        btnPDF = new QPushButton(centralwidget);
        btnPDF->setObjectName("btnPDF");
        btnPDF->setGeometry(QRect(460, 630, 90, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1503, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelStatus->setText(QCoreApplication::translate("MainWindow", "the input file", nullptr));
        btnParse->setText(QCoreApplication::translate("MainWindow", "parse", nullptr));
        btnLoad->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
        btnPNG->setText(QCoreApplication::translate("MainWindow", "PNG", nullptr));
        btnPDF->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
