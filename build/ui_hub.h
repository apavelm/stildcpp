/********************************************************************************
** Form generated from reading ui file 'hub.ui'
**
** Created: Tue Oct 2 18:56:41 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HUB_H
#define UI_HUB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QSplitter>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_mdiHUBwin
{
public:
    QHBoxLayout *hboxLayout;
    QSplitter *hubsplitter;
    QSplitter *hubsplitter2;
    QWidget *layoutWidget;
    QVBoxLayout *vboxLayout;
    QTextEdit *mainChat;
    QLineEdit *chatEdit;
    QTreeView *userView;

    void setupUi(QWidget *mdiHUBwin)
    {
    if (mdiHUBwin->objectName().isEmpty())
        mdiHUBwin->setObjectName(QString::fromUtf8("mdiHUBwin"));
    mdiHUBwin->resize(603, 455);
    hboxLayout = new QHBoxLayout(mdiHUBwin);
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(9, 9, 9, 9);
    hubsplitter = new QSplitter(mdiHUBwin);
    hubsplitter->setObjectName(QString::fromUtf8("hubsplitter"));
    hubsplitter->setOrientation(Qt::Horizontal);
    hubsplitter2 = new QSplitter(hubsplitter);
    hubsplitter2->setObjectName(QString::fromUtf8("hubsplitter2"));
    hubsplitter2->setOrientation(Qt::Horizontal);
    layoutWidget = new QWidget(hubsplitter2);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    vboxLayout = new QVBoxLayout(layoutWidget);
    vboxLayout->setSpacing(6);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    mainChat = new QTextEdit(layoutWidget);
    mainChat->setObjectName(QString::fromUtf8("mainChat"));

    vboxLayout->addWidget(mainChat);

    chatEdit = new QLineEdit(layoutWidget);
    chatEdit->setObjectName(QString::fromUtf8("chatEdit"));

    vboxLayout->addWidget(chatEdit);

    hubsplitter2->addWidget(layoutWidget);
    hubsplitter->addWidget(hubsplitter2);
    userView = new QTreeView(hubsplitter);
    userView->setObjectName(QString::fromUtf8("userView"));
    userView->setContextMenuPolicy(Qt::CustomContextMenu);
    userView->setIndentation(10);
    userView->setUniformRowHeights(true);
    userView->setItemsExpandable(false);
    hubsplitter->addWidget(userView);

    hboxLayout->addWidget(hubsplitter);


    retranslateUi(mdiHUBwin);

    QMetaObject::connectSlotsByName(mdiHUBwin);
    } // setupUi

    void retranslateUi(QWidget *mdiHUBwin)
    {
    mdiHUBwin->setWindowTitle(QApplication::translate("mdiHUBwin", "Form", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(mdiHUBwin);
    } // retranslateUi

};

namespace Ui {
    class mdiHUBwin: public Ui_mdiHUBwin {};
} // namespace Ui

#endif // UI_HUB_H
