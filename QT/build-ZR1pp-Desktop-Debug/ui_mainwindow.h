/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QWebEngineView *webEngineView;
    QVBoxLayout *verticalLayout;
    QLCDNumber *lcdNumber;
    QProgressBar *progressBar;
    QComboBox *comboBox;
    QLabel *label;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(993, 476);
        MainWindow->setTabShape(QTabWidget::Triangular);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        webEngineView = new QWebEngineView(centralwidget);
        webEngineView->setObjectName(QString::fromUtf8("webEngineView"));
        webEngineView->setUrl(QUrl(QString::fromUtf8("about:blank")));

        gridLayout_2->addWidget(webEngineView, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lcdNumber = new QLCDNumber(centralwidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setFrameShape(QFrame::Box);
        lcdNumber->setMidLineWidth(1);

        verticalLayout->addWidget(lcdNumber);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(99);

        verticalLayout->addWidget(progressBar);

        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEditable(false);
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        verticalLayout->addWidget(comboBox);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        gridLayout_2->addLayout(verticalLayout, 1, 1, 1, 1);

        gridLayout_2->setColumnStretch(0, 4);
        gridLayout_2->setColumnStretch(1, 1);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "ZR1pp", nullptr));
        progressBar->setFormat(QCoreApplication::translate("MainWindow", "%p\302\272C", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Auto", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "On", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Off", nullptr));

        comboBox->setCurrentText(QCoreApplication::translate("MainWindow", "Auto", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "N", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
