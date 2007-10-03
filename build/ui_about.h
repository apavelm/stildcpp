/********************************************************************************
** Form generated from reading ui file 'about.ui'
**
** Created: Tue Oct 2 23:49:47 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

class Ui_formAbout
{
public:
    QPushButton *Button;
    QLabel *label;
    QLabel *label_2;
    QLabel *VerLabel;

    void setupUi(QDialog *formAbout)
    {
    if (formAbout->objectName().isEmpty())
        formAbout->setObjectName(QString::fromUtf8("formAbout"));
    formAbout->setWindowModality(Qt::ApplicationModal);
    formAbout->resize(400, 300);
    Button = new QPushButton(formAbout);
    Button->setObjectName(QString::fromUtf8("Button"));
    Button->setGeometry(QRect(140, 260, 111, 31));
    Button->setDefault(true);
    label = new QLabel(formAbout);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(140, 10, 107, 30));
    QFont font;
    font.setPointSize(16);
    font.setBold(true);
    font.setWeight(75);
    label->setFont(font);
    label_2 = new QLabel(formAbout);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(170, 110, 48, 50));
    label_2->setAutoFillBackground(false);
    label_2->setAlignment(Qt::AlignCenter);
    VerLabel = new QLabel(formAbout);
    VerLabel->setObjectName(QString::fromUtf8("VerLabel"));
    VerLabel->setGeometry(QRect(30, 40, 331, 20));
    VerLabel->setAlignment(Qt::AlignCenter);

    retranslateUi(formAbout);
    QObject::connect(Button, SIGNAL(clicked()), formAbout, SLOT(close()));

    QMetaObject::connectSlotsByName(formAbout);
    } // setupUi

    void retranslateUi(QDialog *formAbout)
    {
    formAbout->setWindowTitle(QApplication::translate("formAbout", "Dialog", 0, QApplication::UnicodeUTF8));
    Button->setText(QApplication::translate("formAbout", "OK", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("formAbout", "StilDC++", 0, QApplication::UnicodeUTF8));
    label_2->setText(QString());
    VerLabel->setText(QString());
    Q_UNUSED(formAbout);
    } // retranslateUi

};

namespace Ui {
    class formAbout: public Ui_formAbout {};
} // namespace Ui

#endif // UI_ABOUT_H
