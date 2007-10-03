/********************************************************************************
** Form generated from reading ui file 'favhublist.ui'
**
** Created: Tue Oct 2 23:49:47 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FAVHUBLIST_H
#define UI_FAVHUBLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

class Ui_mdiFAVHUBwin
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QListWidget *listWidget;
    QHBoxLayout *hboxLayout;
    QPushButton *ConnectBtn;
    QSpacerItem *spacerItem;
    QPushButton *OkBtn;
    QPushButton *closeBtn;

    void setupUi(QDialog *mdiFAVHUBwin)
    {
    if (mdiFAVHUBwin->objectName().isEmpty())
        mdiFAVHUBwin->setObjectName(QString::fromUtf8("mdiFAVHUBwin"));
    mdiFAVHUBwin->resize(400, 300);
    gridLayout = new QGridLayout(mdiFAVHUBwin);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    listWidget = new QListWidget(mdiFAVHUBwin);
    listWidget->setObjectName(QString::fromUtf8("listWidget"));

    vboxLayout->addWidget(listWidget);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    ConnectBtn = new QPushButton(mdiFAVHUBwin);
    ConnectBtn->setObjectName(QString::fromUtf8("ConnectBtn"));

    hboxLayout->addWidget(ConnectBtn);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    OkBtn = new QPushButton(mdiFAVHUBwin);
    OkBtn->setObjectName(QString::fromUtf8("OkBtn"));

    hboxLayout->addWidget(OkBtn);

    closeBtn = new QPushButton(mdiFAVHUBwin);
    closeBtn->setObjectName(QString::fromUtf8("closeBtn"));

    hboxLayout->addWidget(closeBtn);


    vboxLayout->addLayout(hboxLayout);


    gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);


    retranslateUi(mdiFAVHUBwin);

    QMetaObject::connectSlotsByName(mdiFAVHUBwin);
    } // setupUi

    void retranslateUi(QDialog *mdiFAVHUBwin)
    {
    mdiFAVHUBwin->setWindowTitle(QApplication::translate("mdiFAVHUBwin", "Favorite Hubs", 0, QApplication::UnicodeUTF8));
    ConnectBtn->setText(QApplication::translate("mdiFAVHUBwin", "Connect", 0, QApplication::UnicodeUTF8));
    OkBtn->setText(QApplication::translate("mdiFAVHUBwin", "OK", 0, QApplication::UnicodeUTF8));
    closeBtn->setText(QApplication::translate("mdiFAVHUBwin", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(mdiFAVHUBwin);
    } // retranslateUi

};

namespace Ui {
    class mdiFAVHUBwin: public Ui_mdiFAVHUBwin {};
} // namespace Ui

#endif // UI_FAVHUBLIST_H
