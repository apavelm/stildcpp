/********************************************************************************
** Form generated from reading ui file 'quickconnect.ui'
**
** Created: Wed Oct 3 14:49:35 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QUICKCONNECT_H
#define UI_QUICKCONNECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

class Ui_quickConnectwin
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *IconLabel;
    QVBoxLayout *vboxLayout1;
    QLabel *AdrLabel;
    QHBoxLayout *hboxLayout1;
    QLineEdit *adrline;
    QSpinBox *spinBox;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem;
    QPushButton *OKButton;
    QPushButton *CancelButton;

    void setupUi(QDialog *quickConnectwin)
    {
    if (quickConnectwin->objectName().isEmpty())
        quickConnectwin->setObjectName(QString::fromUtf8("quickConnectwin"));
    quickConnectwin->setWindowModality(Qt::ApplicationModal);
    quickConnectwin->resize(400, 120);
    quickConnectwin->setModal(true);
    gridLayout = new QGridLayout(quickConnectwin);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    IconLabel = new QLabel(quickConnectwin);
    IconLabel->setObjectName(QString::fromUtf8("IconLabel"));

    hboxLayout->addWidget(IconLabel);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    AdrLabel = new QLabel(quickConnectwin);
    AdrLabel->setObjectName(QString::fromUtf8("AdrLabel"));

    vboxLayout1->addWidget(AdrLabel);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    adrline = new QLineEdit(quickConnectwin);
    adrline->setObjectName(QString::fromUtf8("adrline"));

    hboxLayout1->addWidget(adrline);

    spinBox = new QSpinBox(quickConnectwin);
    spinBox->setObjectName(QString::fromUtf8("spinBox"));
    spinBox->setFocusPolicy(Qt::NoFocus);
    spinBox->setMaximum(65536);
    spinBox->setValue(411);

    hboxLayout1->addWidget(spinBox);


    vboxLayout1->addLayout(hboxLayout1);


    hboxLayout->addLayout(vboxLayout1);


    vboxLayout->addLayout(hboxLayout);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem);

    OKButton = new QPushButton(quickConnectwin);
    OKButton->setObjectName(QString::fromUtf8("OKButton"));

    hboxLayout2->addWidget(OKButton);

    CancelButton = new QPushButton(quickConnectwin);
    CancelButton->setObjectName(QString::fromUtf8("CancelButton"));

    hboxLayout2->addWidget(CancelButton);


    vboxLayout->addLayout(hboxLayout2);


    gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);


    retranslateUi(quickConnectwin);

    QMetaObject::connectSlotsByName(quickConnectwin);
    } // setupUi

    void retranslateUi(QDialog *quickConnectwin)
    {
    quickConnectwin->setWindowTitle(QApplication::translate("quickConnectwin", "Quick Connect", 0, QApplication::UnicodeUTF8));
    IconLabel->setText(QApplication::translate("quickConnectwin", "TextLabel", 0, QApplication::UnicodeUTF8));
    AdrLabel->setText(QApplication::translate("quickConnectwin", "Addres:", 0, QApplication::UnicodeUTF8));
    OKButton->setText(QApplication::translate("quickConnectwin", "OK", 0, QApplication::UnicodeUTF8));
    CancelButton->setText(QApplication::translate("quickConnectwin", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(quickConnectwin);
    } // retranslateUi

};

namespace Ui {
    class quickConnectwin: public Ui_quickConnectwin {};
} // namespace Ui

#endif // UI_QUICKCONNECT_H
