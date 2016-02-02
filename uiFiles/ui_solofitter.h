/********************************************************************************
** Form generated from reading UI file 'hnavigator.ui'
**
** Created: Thu Jun 19 16:23:58 2014
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOLOFITTER_H
#define UI_SOLOFITTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SoloFitter
{
public:
    QWidget *tab;
    QComboBox *hNavigatorInputFileCB;
    QFrame *hNavigatorTreeFrame;
    QGroupBox *hNavigatorCanvasGB;
    QComboBox *hCanvasCB;
    QLabel *currentCanvasLB;
    QComboBox *canvasSizeCB;
    QLabel *canvasSizeLB;
    QTabWidget *displayControlsTB;
    QWidget *actionsTP;
    QPushButton *unZoomPB;
    QPushButton *setRangePB;
    QLabel *xMinL;
    QLabel *xMaxL;
    QLineEdit *xMinLE;
    QLineEdit *xMaxLE;
    QLineEdit *yMaxLE;
    QLabel *yMinL;
    QLabel *yMaxL;
    QLineEdit *yMinLE;
    QLabel *zMinL;
    QLineEdit *zMinLE;
    QLineEdit *zMaxLE;
    QLabel *zMaxL;
    QPushButton *shiftPlusPB;
    QLineEdit *shiftLE;
    QPushButton *shiftMinusPB;
    QCheckBox *fitCB;
    QCheckBox *statCB;
    QCheckBox *logxCB;
    QCheckBox *logyCB;
    QGroupBox *twoDGB;
    QRadioButton *surfaceRB;
    QRadioButton *contourRB;
    QRadioButton *legoRB;
    QRadioButton *lego2RB;
    QRadioButton *colzRB;
    QCheckBox *logzCB;
    QSlider *shiftHS;
    QWidget *fitTP;
    QLabel *label;
    QLabel *label_2;
    QTabWidget *parametersTW;
    QPushButton *fitPB;
    QLabel *label_3;
    QLineEdit *fitMethodLE;
    QDoubleSpinBox *fitRangeMinSB;
    QDoubleSpinBox *fitRangeMaxSB;
    QCheckBox *fitLimitCB;
    QListWidget *fitFuncLW;
    QLineEdit *fitFuncLE;
    QCheckBox *customFunctionCB;
    QGroupBox *splitGB;
    QLabel *currentCanvasLB_2;
    QRadioButton *splitCanvasXYRB;
    QRadioButton *splitCanvasXRB;
    QRadioButton *splitCanvasYRB;
    QPushButton *saveImagePB;
    QPushButton *openFilePB;
    QPushButton *saveComponentsPB;
    QCheckBox *flattenHierarchyCB;

    void setupUi(QTabWidget *HNavigator)
    {
        if (SoloFitter->objectName().isEmpty())
            SoloFitter->setObjectName(QString::fromUtf8("HNavigator"));
        SoloFitter->resize(350, 720);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        soloFitterInputFileCB = new QComboBox(tab);
        soloFitterInputFileCB->setObjectName(QString::fromUtf8("hNavigatorInputFileCB"));
        soloFitterInputFileCB->setGeometry(QRect(0, 25, 346, 20));
        soloFitterTreeFrame = new QFrame(tab);
        soloFitterTreeFrame->setObjectName(QString::fromUtf8("hNavigatorTreeFrame"));
        soloFitterTreeFrame->setGeometry(QRect(0, 45, 346, 421));
        soloFitterTreeFrame->setSizeIncrement(QSize(0, 0));
        soloFitterTreeFrame->setBaseSize(QSize(0, 0));
        QPalette palette;
        QBrush brush(QColor(255, 255, 212, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(212, 208, 200, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        soloFitterTreeFrame->setPalette(palette);
        soloFitterTreeFrame->setFrameShape(QFrame::Box);
        soloFitterTreeFrame->setFrameShadow(QFrame::Raised);
        soloFitterTreeFrame->setLineWidth(2);
        soloFitterTreeFrame->setMidLineWidth(1);
        soloFitterCanvasGB = new QGroupBox(tab);
        soloFitterCanvasGB->setObjectName(QString::fromUtf8("hNavigatorCanvasGB"));
        soloFitterCanvasGB->setGeometry(QRect(0, 465, 346, 226));
        QPalette palette1;
        QBrush brush2(QColor(0, 0, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        QBrush brush3(QColor(213, 253, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush3);
        QBrush brush4(QColor(156, 194, 255, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush4);
        QBrush brush5(QColor(106, 104, 100, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush4);
        soloFitterCanvasGB->setPalette(palette1);
        sCanvasCB = new QComboBox(soloFitterCanvasGB);
        sCanvasCB->setObjectName(QString::fromUtf8("sCanvasCB"));
        sCanvasCB->setGeometry(QRect(55, 205, 39, 16));
        currentCanvasLB = new QLabel(soloFitterCanvasGB);
        currentCanvasLB->setObjectName(QString::fromUtf8("currentCanvasLB"));
        currentCanvasLB->setGeometry(QRect(5, 205, 51, 16));
        canvasSizeCB = new QComboBox(soloFitterCanvasGB);
        canvasSizeCB->setObjectName(QString::fromUtf8("canvasSizeCB"));
        canvasSizeCB->setGeometry(QRect(125, 205, 71, 16));
        canvasSizeLB = new QLabel(hNavigatorCanvasGB);
        canvasSizeLB->setObjectName(QString::fromUtf8("canvasSizeLB"));
        canvasSizeLB->setGeometry(QRect(95, 205, 31, 16));
        displayControlsTB = new QTabWidget(hNavigatorCanvasGB);
        displayControlsTB->setObjectName(QString::fromUtf8("displayControlsTB"));
        displayControlsTB->setGeometry(QRect(5, 15, 341, 181));
        QPalette palette2;
        QBrush brush6(QColor(181, 255, 202, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush6);
        QBrush brush7(QColor(255, 197, 131, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        displayControlsTB->setPalette(palette2);
        actionsTP = new QWidget();
        actionsTP->setObjectName(QString::fromUtf8("actionsTP"));
        unZoomPB = new QPushButton(actionsTP);
        unZoomPB->setObjectName(QString::fromUtf8("unZoomPB"));
        unZoomPB->setGeometry(QRect(70, 5, 61, 21));
        setRangePB = new QPushButton(actionsTP);
        setRangePB->setObjectName(QString::fromUtf8("setRangePB"));
        setRangePB->setGeometry(QRect(5, 5, 61, 21));
        xMinL = new QLabel(actionsTP);
        xMinL->setObjectName(QString::fromUtf8("xMinL"));
        xMinL->setGeometry(QRect(10, 30, 36, 16));
        xMaxL = new QLabel(actionsTP);
        xMaxL->setObjectName(QString::fromUtf8("xMaxL"));
        xMaxL->setGeometry(QRect(60, 30, 41, 16));
        xMinLE = new QLineEdit(actionsTP);
        xMinLE->setObjectName(QString::fromUtf8("xMinLE"));
        xMinLE->setGeometry(QRect(5, 45, 46, 21));
        xMinLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xMaxLE = new QLineEdit(actionsTP);
        xMaxLE->setObjectName(QString::fromUtf8("xMaxLE"));
        xMaxLE->setGeometry(QRect(55, 45, 46, 21));
        xMaxLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yMaxLE = new QLineEdit(actionsTP);
        yMaxLE->setObjectName(QString::fromUtf8("yMaxLE"));
        yMaxLE->setGeometry(QRect(175, 45, 46, 21));
        yMaxLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yMinL = new QLabel(actionsTP);
        yMinL->setObjectName(QString::fromUtf8("yMinL"));
        yMinL->setGeometry(QRect(130, 30, 36, 16));
        yMaxL = new QLabel(actionsTP);
        yMaxL->setObjectName(QString::fromUtf8("yMaxL"));
        yMaxL->setGeometry(QRect(180, 30, 41, 16));
        yMinLE = new QLineEdit(actionsTP);
        yMinLE->setObjectName(QString::fromUtf8("yMinLE"));
        yMinLE->setGeometry(QRect(125, 45, 46, 21));
        yMinLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        zMinL = new QLabel(actionsTP);
        zMinL->setObjectName(QString::fromUtf8("zMinL"));
        zMinL->setGeometry(QRect(245, 30, 36, 16));
        zMinLE = new QLineEdit(actionsTP);
        zMinLE->setObjectName(QString::fromUtf8("zMinLE"));
        zMinLE->setGeometry(QRect(240, 45, 46, 21));
        zMinLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        zMaxLE = new QLineEdit(actionsTP);
        zMaxLE->setObjectName(QString::fromUtf8("zMaxLE"));
        zMaxLE->setGeometry(QRect(290, 45, 46, 21));
        zMaxLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        zMaxL = new QLabel(actionsTP);
        zMaxL->setObjectName(QString::fromUtf8("zMaxL"));
        zMaxL->setGeometry(QRect(295, 30, 41, 16));
        shiftPlusPB = new QPushButton(actionsTP);
        shiftPlusPB->setObjectName(QString::fromUtf8("shiftPlusPB"));
        shiftPlusPB->setGeometry(QRect(305, 70, 16, 16));
        shiftLE = new QLineEdit(actionsTP);
        shiftLE->setObjectName(QString::fromUtf8("shiftLE"));
        shiftLE->setGeometry(QRect(240, 70, 61, 16));
        shiftLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        shiftMinusPB = new QPushButton(actionsTP);
        shiftMinusPB->setObjectName(QString::fromUtf8("shiftMinusPB"));
        shiftMinusPB->setGeometry(QRect(320, 70, 16, 16));
        fitCB = new QCheckBox(actionsTP);
        fitCB->setObjectName(QString::fromUtf8("fitCB"));
        fitCB->setGeometry(QRect(135, 115, 131, 16));
        statCB = new QCheckBox(actionsTP);
        statCB->setObjectName(QString::fromUtf8("statCB"));
        statCB->setGeometry(QRect(5, 115, 126, 16));
        logxCB = new QCheckBox(actionsTP);
        logxCB->setObjectName(QString::fromUtf8("logxCB"));
        logxCB->setGeometry(QRect(5, 95, 61, 16));
        logxCB->setAutoExclusive(false);
        logyCB = new QCheckBox(actionsTP);
        logyCB->setObjectName(QString::fromUtf8("logyCB"));
        logyCB->setGeometry(QRect(65, 95, 61, 16));
        logyCB->setAutoExclusive(false);
        twoDGB = new QGroupBox(actionsTP);
        twoDGB->setObjectName(QString::fromUtf8("twoDGB"));
        twoDGB->setGeometry(QRect(0, 135, 336, 21));
        surfaceRB = new QRadioButton(twoDGB);
        surfaceRB->setObjectName(QString::fromUtf8("surfaceRB"));
        surfaceRB->setGeometry(QRect(5, 0, 71, 21));
        contourRB = new QRadioButton(twoDGB);
        contourRB->setObjectName(QString::fromUtf8("contourRB"));
        contourRB->setGeometry(QRect(80, 0, 71, 21));
        legoRB = new QRadioButton(twoDGB);
        legoRB->setObjectName(QString::fromUtf8("legoRB"));
        legoRB->setGeometry(QRect(155, 0, 51, 21));
        lego2RB = new QRadioButton(twoDGB);
        lego2RB->setObjectName(QString::fromUtf8("lego2RB"));
        lego2RB->setGeometry(QRect(210, 0, 61, 21));
        colzRB = new QRadioButton(twoDGB);
        colzRB->setObjectName(QString::fromUtf8("colzRB"));
        colzRB->setGeometry(QRect(275, 0, 56, 21));
        colzRB->setChecked(true);
        logzCB = new QCheckBox(actionsTP);
        logzCB->setObjectName(QString::fromUtf8("logzCB"));
        logzCB->setGeometry(QRect(125, 95, 61, 16));
        logzCB->setAutoExclusive(false);
        shiftHS = new QSlider(actionsTP);
        shiftHS->setObjectName(QString::fromUtf8("shiftHS"));
        shiftHS->setGeometry(QRect(5, 70, 226, 16));
        shiftHS->setOrientation(Qt::Horizontal);
        displayControlsTB->addTab(actionsTP, QString());
        fitTP = new QWidget();
        fitTP->setObjectName(QString::fromUtf8("fitTP"));
        label = new QLabel(fitTP);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 5, 111, 16));
        label_2 = new QLabel(fitTP);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(200, 0, 81, 16));
        parametersTW = new QTabWidget(fitTP);
        parametersTW->setObjectName(QString::fromUtf8("parametersTW"));
        parametersTW->setGeometry(QRect(130, 15, 201, 116));
        fitPB = new QPushButton(fitTP);
        fitPB->setObjectName(QString::fromUtf8("fitPB"));
        fitPB->setGeometry(QRect(10, 130, 46, 25));
        label_3 = new QLabel(fitTP);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(255, 135, 46, 16));
        fitMethodLE = new QLineEdit(fitTP);
        fitMethodLE->setObjectName(QString::fromUtf8("fitMethodLE"));
        fitMethodLE->setGeometry(QRect(300, 135, 36, 21));
        fitRangeMinSB = new QDoubleSpinBox(fitTP);
        fitRangeMinSB->setObjectName(QString::fromUtf8("fitRangeMinSB"));
        fitRangeMinSB->setGeometry(QRect(135, 135, 56, 16));
        fitRangeMaxSB = new QDoubleSpinBox(fitTP);
        fitRangeMaxSB->setObjectName(QString::fromUtf8("fitRangeMaxSB"));
        fitRangeMaxSB->setGeometry(QRect(195, 135, 56, 16));
        fitLimitCB = new QCheckBox(fitTP);
        fitLimitCB->setObjectName(QString::fromUtf8("fitLimitCB"));
        fitLimitCB->setGeometry(QRect(75, 135, 66, 20));
        fitFuncLW = new QListWidget(fitTP);
        fitFuncLW->setObjectName(QString::fromUtf8("fitFuncLW"));
        fitFuncLW->setGeometry(QRect(5, 20, 121, 66));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        fitFuncLW->setFont(font);
        fitFuncLE = new QLineEdit(fitTP);
        fitFuncLE->setObjectName(QString::fromUtf8("fitFuncLE"));
        fitFuncLE->setGeometry(QRect(7, 105, 116, 24));
        customFunctionCB = new QCheckBox(fitTP);
        customFunctionCB->setObjectName(QString::fromUtf8("customFunctionCB"));
        customFunctionCB->setGeometry(QRect(0, 85, 136, 20));
        displayControlsTB->addTab(fitTP, QString());
        splitGB = new QGroupBox(hNavigatorCanvasGB);
        splitGB->setObjectName(QString::fromUtf8("splitGB"));
        splitGB->setGeometry(QRect(200, 205, 141, 16));
        currentCanvasLB_2 = new QLabel(splitGB);
        currentCanvasLB_2->setObjectName(QString::fromUtf8("currentCanvasLB_2"));
        currentCanvasLB_2->setGeometry(QRect(5, 0, 36, 16));
        splitCanvasXYRB = new QRadioButton(splitGB);
        splitCanvasXYRB->setObjectName(QString::fromUtf8("splitCanvasXYRB"));
        splitCanvasXYRB->setGeometry(QRect(40, 0, 41, 16));
        splitCanvasXYRB->setChecked(true);
        splitCanvasXRB = new QRadioButton(splitGB);
        splitCanvasXRB->setObjectName(QString::fromUtf8("splitCanvasXRB"));
        splitCanvasXRB->setGeometry(QRect(80, 0, 31, 16));
        splitCanvasYRB = new QRadioButton(splitGB);
        splitCanvasYRB->setObjectName(QString::fromUtf8("splitCanvasYRB"));
        splitCanvasYRB->setGeometry(QRect(110, 0, 31, 16));
        saveImagePB = new QPushButton(hNavigatorCanvasGB);
        saveImagePB->setObjectName(QString::fromUtf8("saveImagePB"));
        saveImagePB->setGeometry(QRect(225, 5, 111, 27));
        openFilePB = new QPushButton(tab);
        openFilePB->setObjectName(QString::fromUtf8("openFilePB"));
        openFilePB->setGeometry(QRect(0, 0, 86, 21));
        QPalette palette3;
        QBrush brush8(QColor(0, 170, 0, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        openFilePB->setPalette(palette3);
        saveComponentsPB = new QPushButton(tab);
        saveComponentsPB->setObjectName(QString::fromUtf8("saveComponentsPB"));
        saveComponentsPB->setGeometry(QRect(185, 0, 91, 21));
        QPalette palette4;
        QBrush brush9(QColor(255, 0, 0, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        saveComponentsPB->setPalette(palette4);
        flattenHierarchyCB = new QCheckBox(tab);
        flattenHierarchyCB->setObjectName(QString::fromUtf8("flattenHierarchyCB"));
        flattenHierarchyCB->setGeometry(QRect(280, 0, 66, 21));
        HNavigator->addTab(tab, QString());

        retranslateUi(HNavigator);
        QObject::connect(surfaceRB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(contourRB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(legoRB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(lego2RB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(colzRB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(statCB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(fitCB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(logxCB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(logyCB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(logzCB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(splitCanvasYRB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(splitCanvasXYRB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));
        QObject::connect(splitCanvasXRB, SIGNAL(clicked(bool)), HNavigator, SLOT(checkBoxesHandler(bool)));

        HNavigator->setCurrentIndex(0);
        displayControlsTB->setCurrentIndex(0);
        parametersTW->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(HNavigator);
    } // setupUi

    void retranslateUi(QTabWidget *HNavigator)
    {
        HNavigator->setWindowTitle(QApplication::translate("HNavigator", "Histogram Navigator", 0, QApplication::UnicodeUTF8));
        hNavigatorCanvasGB->setTitle(QApplication::translate("HNavigator", "Controls", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        hCanvasCB->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        hCanvasCB->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        hCanvasCB->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        currentCanvasLB->setText(QApplication::translate("HNavigator", "Canvas:", 0, QApplication::UnicodeUTF8));
        canvasSizeLB->setText(QApplication::translate("HNavigator", "Size:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        unZoomPB->setToolTip(QApplication::translate("HNavigator", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Unzoom</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Res"
                        "ets the X and Y axis to their default for all currently selected histograms</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        unZoomPB->setText(QApplication::translate("HNavigator", "Unzoom", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        setRangePB->setToolTip(QApplication::translate("HNavigator", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Unzoom</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Res"
                        "ets the X and Y axis to their default for all currently selected histograms</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        setRangePB->setText(QApplication::translate("HNavigator", "Range", 0, QApplication::UnicodeUTF8));
        xMinL->setText(QApplication::translate("HNavigator", "X Min", 0, QApplication::UnicodeUTF8));
        xMaxL->setText(QApplication::translate("HNavigator", "X Max", 0, QApplication::UnicodeUTF8));
        xMinLE->setText(QApplication::translate("HNavigator", "0", 0, QApplication::UnicodeUTF8));
        xMaxLE->setText(QApplication::translate("HNavigator", "0", 0, QApplication::UnicodeUTF8));
        yMaxLE->setText(QApplication::translate("HNavigator", "0", 0, QApplication::UnicodeUTF8));
        yMinL->setText(QApplication::translate("HNavigator", "Y Min", 0, QApplication::UnicodeUTF8));
        yMaxL->setText(QApplication::translate("HNavigator", "Y Max", 0, QApplication::UnicodeUTF8));
        yMinLE->setText(QApplication::translate("HNavigator", "0", 0, QApplication::UnicodeUTF8));
        zMinL->setText(QApplication::translate("HNavigator", "Z Min", 0, QApplication::UnicodeUTF8));
        zMinLE->setText(QApplication::translate("HNavigator", "0", 0, QApplication::UnicodeUTF8));
        zMaxLE->setText(QApplication::translate("HNavigator", "0", 0, QApplication::UnicodeUTF8));
        zMaxL->setText(QApplication::translate("HNavigator", "Z Max", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        shiftPlusPB->setToolTip(QApplication::translate("HNavigator", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Unzoom</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Res"
                        "ets the X and Y axis to their default for all currently selected histograms</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        shiftPlusPB->setText(QApplication::translate("HNavigator", "+", 0, QApplication::UnicodeUTF8));
        shiftLE->setText(QApplication::translate("HNavigator", "1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        shiftMinusPB->setToolTip(QApplication::translate("HNavigator", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Unzoom</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Res"
                        "ets the X and Y axis to their default for all currently selected histograms</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        shiftMinusPB->setText(QApplication::translate("HNavigator", "-", 0, QApplication::UnicodeUTF8));
        fitCB->setText(QApplication::translate("HNavigator", "Display fit results", 0, QApplication::UnicodeUTF8));
        statCB->setText(QApplication::translate("HNavigator", "Display statistics", 0, QApplication::UnicodeUTF8));
        logxCB->setText(QApplication::translate("HNavigator", "Log X", 0, QApplication::UnicodeUTF8));
        logyCB->setText(QApplication::translate("HNavigator", "Log Y", 0, QApplication::UnicodeUTF8));
        twoDGB->setTitle(QString());
        surfaceRB->setText(QApplication::translate("HNavigator", "Surface", 0, QApplication::UnicodeUTF8));
        contourRB->setText(QApplication::translate("HNavigator", "Contour", 0, QApplication::UnicodeUTF8));
        legoRB->setText(QApplication::translate("HNavigator", "Lego", 0, QApplication::UnicodeUTF8));
        lego2RB->setText(QApplication::translate("HNavigator", "Lego2", 0, QApplication::UnicodeUTF8));
        colzRB->setText(QApplication::translate("HNavigator", "COLZ", 0, QApplication::UnicodeUTF8));
        logzCB->setText(QApplication::translate("HNavigator", "Log Z", 0, QApplication::UnicodeUTF8));
        displayControlsTB->setTabText(displayControlsTB->indexOf(actionsTP), QApplication::translate("HNavigator", "Actions", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("HNavigator", "Choose a Function", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("HNavigator", "Parameters", 0, QApplication::UnicodeUTF8));
        fitPB->setText(QApplication::translate("HNavigator", "Fit!", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("HNavigator", "Method", 0, QApplication::UnicodeUTF8));
        fitLimitCB->setText(QApplication::translate("HNavigator", "Limit x", 0, QApplication::UnicodeUTF8));
        customFunctionCB->setText(QApplication::translate("HNavigator", "Or write it your own", 0, QApplication::UnicodeUTF8));
        displayControlsTB->setTabText(displayControlsTB->indexOf(fitTP), QApplication::translate("HNavigator", "Fit Manager", 0, QApplication::UnicodeUTF8));
        splitGB->setTitle(QString());
        currentCanvasLB_2->setText(QApplication::translate("HNavigator", "Split:", 0, QApplication::UnicodeUTF8));
        splitCanvasXYRB->setText(QApplication::translate("HNavigator", "XY", 0, QApplication::UnicodeUTF8));
        splitCanvasXRB->setText(QApplication::translate("HNavigator", "X", 0, QApplication::UnicodeUTF8));
        splitCanvasYRB->setText(QApplication::translate("HNavigator", "Y", 0, QApplication::UnicodeUTF8));
        saveImagePB->setText(QApplication::translate("HNavigator", "Save Image", 0, QApplication::UnicodeUTF8));
        openFilePB->setText(QApplication::translate("HNavigator", "Open File", 0, QApplication::UnicodeUTF8));
        saveComponentsPB->setText(QApplication::translate("HNavigator", "Save selected", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        flattenHierarchyCB->setToolTip(QApplication::translate("HNavigator", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Flatten</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">If"
                        " checked, all selected histograms are</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">saved in the file in the top level directory,</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">without preserving the original folder structure.</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        flattenHierarchyCB->setText(QApplication::translate("HNavigator", "Flatten", 0, QApplication::UnicodeUTF8));
        HNavigator->setTabText(HNavigator->indexOf(tab), QApplication::translate("HNavigator", "Input files", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SoloFitter: public Ui_SoloFitter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLOFITTER_H
