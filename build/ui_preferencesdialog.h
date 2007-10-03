/********************************************************************************
** Form generated from reading ui file 'preferencesdialog.ui'
**
** Created: Wed Oct 3 00:12:29 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_PreferencesDialog
{
public:
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QTreeWidget *categoryList;
    QVBoxLayout *vboxLayout;
    QLabel *currentLabel;
    QFrame *line_2;
    QStackedWidget *widgetStack;
    QWidget *LookNFeelPage;
    QCheckBox *chkHiddenStart;
    QWidget *page;
    QWidget *page_2;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *closeButton;

    void setupUi(QWidget *PreferencesDialog)
    {
    if (PreferencesDialog->objectName().isEmpty())
        PreferencesDialog->setObjectName(QString::fromUtf8("PreferencesDialog"));
    PreferencesDialog->resize(467, 420);
    gridLayout = new QGridLayout(PreferencesDialog);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    splitter = new QSplitter(PreferencesDialog);
    splitter->setObjectName(QString::fromUtf8("splitter"));
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);
    layoutWidget = new QWidget(splitter);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    hboxLayout = new QHBoxLayout(layoutWidget);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    categoryList = new QTreeWidget(layoutWidget);
    categoryList->setObjectName(QString::fromUtf8("categoryList"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(categoryList->sizePolicy().hasHeightForWidth());
    categoryList->setSizePolicy(sizePolicy);
    categoryList->setMinimumSize(QSize(150, 0));
    categoryList->setMaximumSize(QSize(250, 16777215));
    categoryList->setIndentation(15);
    categoryList->setRootIsDecorated(false);
    categoryList->setUniformRowHeights(true);
    categoryList->setItemsExpandable(false);

    hboxLayout->addWidget(categoryList);

    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    currentLabel = new QLabel(layoutWidget);
    currentLabel->setObjectName(QString::fromUtf8("currentLabel"));
    currentLabel->setIndent(5);

    vboxLayout->addWidget(currentLabel);

    line_2 = new QFrame(layoutWidget);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line_2);

    widgetStack = new QStackedWidget(layoutWidget);
    widgetStack->setObjectName(QString::fromUtf8("widgetStack"));
    widgetStack->setFrameShape(QFrame::NoFrame);
    widgetStack->setFrameShadow(QFrame::Plain);
    LookNFeelPage = new QWidget();
    LookNFeelPage->setObjectName(QString::fromUtf8("LookNFeelPage"));
    chkHiddenStart = new QCheckBox(LookNFeelPage);
    chkHiddenStart->setObjectName(QString::fromUtf8("chkHiddenStart"));
    chkHiddenStart->setGeometry(QRect(10, 30, 251, 24));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(chkHiddenStart->sizePolicy().hasHeightForWidth());
    chkHiddenStart->setSizePolicy(sizePolicy1);
    widgetStack->addWidget(LookNFeelPage);
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    widgetStack->addWidget(page);
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    widgetStack->addWidget(page_2);

    vboxLayout->addWidget(widgetStack);


    hboxLayout->addLayout(vboxLayout);

    splitter->addWidget(layoutWidget);

    gridLayout->addWidget(splitter, 0, 0, 1, 1);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    closeButton = new QPushButton(PreferencesDialog);
    closeButton->setObjectName(QString::fromUtf8("closeButton"));

    hboxLayout1->addWidget(closeButton);


    gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);

    QWidget::setTabOrder(categoryList, closeButton);

    retranslateUi(PreferencesDialog);

    widgetStack->setCurrentIndex(2);


    QMetaObject::connectSlotsByName(PreferencesDialog);
    } // setupUi

    void retranslateUi(QWidget *PreferencesDialog)
    {
    PreferencesDialog->setWindowTitle(QApplication::translate("PreferencesDialog", "Preferences", 0, QApplication::UnicodeUTF8));
    categoryList->headerItem()->setText(0, QString());
    chkHiddenStart->setText(QApplication::translate("PreferencesDialog", "Hidden Start", 0, QApplication::UnicodeUTF8));
    closeButton->setText(QApplication::translate("PreferencesDialog", "&Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(PreferencesDialog);
    } // retranslateUi

};

namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui

#endif // UI_PREFERENCESDIALOG_H
