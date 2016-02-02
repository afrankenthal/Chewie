/********************************************************************************
** Form generated from reading UI file 'canvaswidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANVASWIDGET_H
#define UI_CANVASWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CanvasWidget
{
public:
    QWidget *theCanvas;

    void setupUi(QWidget *CanvasWidget)
    {
        if (CanvasWidget->objectName().isEmpty())
            CanvasWidget->setObjectName(QString::fromUtf8("CanvasWidget"));
        CanvasWidget->resize(542, 362);
        theCanvas = new QWidget(CanvasWidget);
        theCanvas->setObjectName(QString::fromUtf8("theCanvas"));
        theCanvas->setGeometry(QRect(0, 0, 543, 363));

        retranslateUi(CanvasWidget);

        QMetaObject::connectSlotsByName(CanvasWidget);
    } // setupUi

    void retranslateUi(QWidget *CanvasWidget)
    {
        CanvasWidget->setWindowTitle(QApplication::translate("CanvasWidget", "Canvas of interactively selected plots", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CanvasWidget: public Ui_CanvasWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASWIDGET_H
