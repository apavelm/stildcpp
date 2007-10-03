/********************************************************************************
** Form generated from reading ui file 'search.ui'
**
** Created: Tue Oct 2 23:49:47 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SEARCH_H
#define UI_SEARCH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>

class Ui_mdiSEARCHwin
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QGroupBox *searchGroupBox;
    QVBoxLayout *vboxLayout1;
    QComboBox *searchCombo;
    QHBoxLayout *hboxLayout1;
    QLabel *minSizeLabel;
    QComboBox *sizeModeCombo;
    QSpinBox *sizeSpin;
    QComboBox *sizeTypeCombo;
    QHBoxLayout *hboxLayout2;
    QLabel *typeLabel;
    QComboBox *typeCombo;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem;
    QPushButton *searchButton;
    QFrame *line;
    QCheckBox *hideZeroSlots;
    QSpacerItem *spacerItem1;
    QTreeView *searchView;

    void setupUi(QDialog *mdiSEARCHwin)
    {
    if (mdiSEARCHwin->objectName().isEmpty())
        mdiSEARCHwin->setObjectName(QString::fromUtf8("mdiSEARCHwin"));
    mdiSEARCHwin->resize(515, 327);
    gridLayout = new QGridLayout(mdiSEARCHwin);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    searchGroupBox = new QGroupBox(mdiSEARCHwin);
    searchGroupBox->setObjectName(QString::fromUtf8("searchGroupBox"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(searchGroupBox->sizePolicy().hasHeightForWidth());
    searchGroupBox->setSizePolicy(sizePolicy);
    vboxLayout1 = new QVBoxLayout(searchGroupBox);
    vboxLayout1->setSpacing(6);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(9, 9, 9, 9);
    searchCombo = new QComboBox(searchGroupBox);
    searchCombo->setObjectName(QString::fromUtf8("searchCombo"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(searchCombo->sizePolicy().hasHeightForWidth());
    searchCombo->setSizePolicy(sizePolicy1);
    searchCombo->setEditable(true);

    vboxLayout1->addWidget(searchCombo);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    minSizeLabel = new QLabel(searchGroupBox);
    minSizeLabel->setObjectName(QString::fromUtf8("minSizeLabel"));

    hboxLayout1->addWidget(minSizeLabel);

    sizeModeCombo = new QComboBox(searchGroupBox);
    sizeModeCombo->setObjectName(QString::fromUtf8("sizeModeCombo"));

    hboxLayout1->addWidget(sizeModeCombo);

    sizeSpin = new QSpinBox(searchGroupBox);
    sizeSpin->setObjectName(QString::fromUtf8("sizeSpin"));
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(1);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(sizeSpin->sizePolicy().hasHeightForWidth());
    sizeSpin->setSizePolicy(sizePolicy2);
    sizeSpin->setMaximum(9999);

    hboxLayout1->addWidget(sizeSpin);

    sizeTypeCombo = new QComboBox(searchGroupBox);
    sizeTypeCombo->setObjectName(QString::fromUtf8("sizeTypeCombo"));

    hboxLayout1->addWidget(sizeTypeCombo);


    vboxLayout1->addLayout(hboxLayout1);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    hboxLayout2->setContentsMargins(0, 0, 0, 0);
    typeLabel = new QLabel(searchGroupBox);
    typeLabel->setObjectName(QString::fromUtf8("typeLabel"));

    hboxLayout2->addWidget(typeLabel);

    typeCombo = new QComboBox(searchGroupBox);
    typeCombo->setObjectName(QString::fromUtf8("typeCombo"));
    sizePolicy2.setHeightForWidth(typeCombo->sizePolicy().hasHeightForWidth());
    typeCombo->setSizePolicy(sizePolicy2);

    hboxLayout2->addWidget(typeCombo);


    vboxLayout1->addLayout(hboxLayout2);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setSpacing(6);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    hboxLayout3->setContentsMargins(0, 0, 0, 0);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem);

    searchButton = new QPushButton(searchGroupBox);
    searchButton->setObjectName(QString::fromUtf8("searchButton"));
    searchButton->setDefault(true);

    hboxLayout3->addWidget(searchButton);


    vboxLayout1->addLayout(hboxLayout3);

    line = new QFrame(searchGroupBox);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    vboxLayout1->addWidget(line);

    hideZeroSlots = new QCheckBox(searchGroupBox);
    hideZeroSlots->setObjectName(QString::fromUtf8("hideZeroSlots"));

    vboxLayout1->addWidget(hideZeroSlots);


    vboxLayout->addWidget(searchGroupBox);

    spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem1);


    hboxLayout->addLayout(vboxLayout);

    searchView = new QTreeView(mdiSEARCHwin);
    searchView->setObjectName(QString::fromUtf8("searchView"));
    QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(1);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(searchView->sizePolicy().hasHeightForWidth());
    searchView->setSizePolicy(sizePolicy3);

    hboxLayout->addWidget(searchView);


    gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);


    retranslateUi(mdiSEARCHwin);

    QMetaObject::connectSlotsByName(mdiSEARCHwin);
    } // setupUi

    void retranslateUi(QDialog *mdiSEARCHwin)
    {
    mdiSEARCHwin->setWindowTitle(QApplication::translate("mdiSEARCHwin", "Dialog", 0, QApplication::UnicodeUTF8));
    searchGroupBox->setTitle(QApplication::translate("mdiSEARCHwin", "Search", 0, QApplication::UnicodeUTF8));
    minSizeLabel->setText(QApplication::translate("mdiSEARCHwin", "Size", 0, QApplication::UnicodeUTF8));
    sizeModeCombo->clear();
    sizeModeCombo->insertItems(0, QStringList()
     << QApplication::translate("mdiSEARCHwin", "at least", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "at most", 0, QApplication::UnicodeUTF8)
    );
    sizeTypeCombo->clear();
    sizeTypeCombo->insertItems(0, QStringList()
     << QApplication::translate("mdiSEARCHwin", "B", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "KiB", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "MiB", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "GiB", 0, QApplication::UnicodeUTF8)
    );
    typeLabel->setText(QApplication::translate("mdiSEARCHwin", "Type:", 0, QApplication::UnicodeUTF8));
    typeCombo->clear();
    typeCombo->insertItems(0, QStringList()
     << QApplication::translate("mdiSEARCHwin", "Any type", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "Audio", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "Compressed", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "Documents", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "Executables", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "Pictures", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "Video", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "Directories", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("mdiSEARCHwin", "TTH", 0, QApplication::UnicodeUTF8)
    );
    searchButton->setText(QApplication::translate("mdiSEARCHwin", "Search", 0, QApplication::UnicodeUTF8));
    hideZeroSlots->setText(QApplication::translate("mdiSEARCHwin", "Hide results with zero slots", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(mdiSEARCHwin);
    } // retranslateUi

};

namespace Ui {
    class mdiSEARCHwin: public Ui_mdiSEARCHwin {};
} // namespace Ui

#endif // UI_SEARCH_H
