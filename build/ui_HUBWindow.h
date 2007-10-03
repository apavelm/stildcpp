/********************************************************************************
** Form generated from reading ui file 'HUBWindow.ui'
**
** Created: Tue Oct 2 23:49:47 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HUBWINDOW_H
#define UI_HUBWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

class Ui_mdiHUBwin
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QListWidget *Userlist;

    void setupUi(QDialog *mdiHUBwin)
    {
    if (mdiHUBwin->objectName().isEmpty())
        mdiHUBwin->setObjectName(QString::fromUtf8("mdiHUBwin"));
    mdiHUBwin->resize(400, 300);
    mdiHUBwin->setMinimumSize(QSize(300, 150));
    mdiHUBwin->setAutoFillBackground(true);
    gridLayout = new QGridLayout(mdiHUBwin);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    textEdit = new QTextEdit(mdiHUBwin);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));

    vboxLayout->addWidget(textEdit);

    lineEdit = new QLineEdit(mdiHUBwin);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    vboxLayout->addWidget(lineEdit);


    hboxLayout->addLayout(vboxLayout);

    Userlist = new QListWidget(mdiHUBwin);
    Userlist->setObjectName(QString::fromUtf8("Userlist"));

    hboxLayout->addWidget(Userlist);


    gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);


    retranslateUi(mdiHUBwin);

    QMetaObject::connectSlotsByName(mdiHUBwin);
    } // setupUi

    void retranslateUi(QDialog *mdiHUBwin)
    {
    mdiHUBwin->setWindowTitle(QApplication::translate("mdiHUBwin", "Dialog", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(mdiHUBwin);
    } // retranslateUi

};

namespace Ui {
    class mdiHUBwin: public Ui_mdiHUBwin {};
} // namespace Ui

#endif // UI_HUBWINDOW_H
