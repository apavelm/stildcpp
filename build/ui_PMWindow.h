/********************************************************************************
** Form generated from reading ui file 'PMWindow.ui'
**
** Created: Tue Oct 2 23:49:47 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PMWINDOW_H
#define UI_PMWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QToolButton>

class Ui_mdiPMwin
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLineEdit *lineEdit_2;
    QToolButton *SendBtn;
    QListWidget *listWidget_2;

    void setupUi(QDialog *mdiPMwin)
    {
    if (mdiPMwin->objectName().isEmpty())
        mdiPMwin->setObjectName(QString::fromUtf8("mdiPMwin"));
    mdiPMwin->resize(553, 521);
    mdiPMwin->setMinimumSize(QSize(200, 100));
    gridLayout = new QGridLayout(mdiPMwin);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    lineEdit_2 = new QLineEdit(mdiPMwin);
    lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

    hboxLayout->addWidget(lineEdit_2);

    SendBtn = new QToolButton(mdiPMwin);
    SendBtn->setObjectName(QString::fromUtf8("SendBtn"));

    hboxLayout->addWidget(SendBtn);


    gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);

    listWidget_2 = new QListWidget(mdiPMwin);
    listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));

    gridLayout->addWidget(listWidget_2, 0, 0, 1, 1);


    retranslateUi(mdiPMwin);

    QMetaObject::connectSlotsByName(mdiPMwin);
    } // setupUi

    void retranslateUi(QDialog *mdiPMwin)
    {
    mdiPMwin->setWindowTitle(QApplication::translate("mdiPMwin", "Dialog", 0, QApplication::UnicodeUTF8));
    SendBtn->setText(QApplication::translate("mdiPMwin", "Send", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(mdiPMwin);
    } // retranslateUi

};

namespace Ui {
    class mdiPMwin: public Ui_mdiPMwin {};
} // namespace Ui

#endif // UI_PMWINDOW_H
