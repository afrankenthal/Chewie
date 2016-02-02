/********************************************************************************
** Form generated from reading UI file 'hanSoloFitter.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HANSOLOFITTER_H
#define UI_HANSOLOFITTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HanSoloFitter
{
public:
    QAction *actionAdvanced_Fitter;
    QAction *actionCharge;
    QAction *actionEfficiency;
    QAction *actionResolution;
    QAction *actionAll;
    QAction *actionPlanar;
    QAction *action3_D;
    QTabWidget *centraltabWidget;
    QWidget *viewerWindow;
    QGroupBox *HanSoloCanvas;
    QComboBox *HanSoloCanvasCB;
    QLabel *currentCanvasLB;
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
    QCheckBox *hanSoloStatCB;
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
    QComboBox *HanSoloFitterInputFile;
    QFrame *HanSoloTreeFrame;
    QMdiSubWindow *theCanvas;
    QWidget *fitterWindow;
    QPushButton *openRootFile;
    QTableView *convertFileListTV;
    QPushButton *convertResetListPB;
    QTabWidget *fitterOptions;
    QWidget *generalCategories;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QFrame *chargeFrame;
    QFrame *efficiencyFrame;
    QFrame *resolutionFrame;
    QWidget *removeDut;
    QWidget *addDut;
    QLabel *label_7;
    QFrame *resolutionFrame_0;
    QFrame *efficiencyFrame_0;
    QLabel *label_8;
    QLabel *label_9;
    QFrame *chargeFrame_0;
    QPushButton *runButton;
    QPushButton *openConfigurationFilePB;
    QLineEdit *configurationFileFrame;
    QPushButton *saveConfigurationFile;
    QPushButton *reset;
    QPushButton *resetDut;
    QStatusBar *statusbar;

    void setupUi(QDialog *HanSoloFitter)
    {
        if (HanSoloFitter->objectName().isEmpty())
            HanSoloFitter->setObjectName(QString::fromUtf8("HanSoloFitter"));
        HanSoloFitter->resize(1030, 740);
        actionAdvanced_Fitter = new QAction(HanSoloFitter);
        actionAdvanced_Fitter->setObjectName(QString::fromUtf8("actionAdvanced_Fitter"));
        actionCharge = new QAction(HanSoloFitter);
        actionCharge->setObjectName(QString::fromUtf8("actionCharge"));
        actionEfficiency = new QAction(HanSoloFitter);
        actionEfficiency->setObjectName(QString::fromUtf8("actionEfficiency"));
        actionResolution = new QAction(HanSoloFitter);
        actionResolution->setObjectName(QString::fromUtf8("actionResolution"));
        actionAll = new QAction(HanSoloFitter);
        actionAll->setObjectName(QString::fromUtf8("actionAll"));
        actionPlanar = new QAction(HanSoloFitter);
        actionPlanar->setObjectName(QString::fromUtf8("actionPlanar"));
        action3_D = new QAction(HanSoloFitter);
        action3_D->setObjectName(QString::fromUtf8("action3_D"));
        centraltabWidget = new QTabWidget(HanSoloFitter);
        centraltabWidget->setObjectName(QString::fromUtf8("centraltabWidget"));
        centraltabWidget->setGeometry(QRect(4, 4, 1021, 729));
        QFont font;
        font.setPointSize(9);
        centraltabWidget->setFont(font);
        viewerWindow = new QWidget();
        viewerWindow->setObjectName(QString::fromUtf8("viewerWindow"));
        HanSoloCanvas = new QGroupBox(viewerWindow);
        HanSoloCanvas->setObjectName(QString::fromUtf8("HanSoloCanvas"));
        HanSoloCanvas->setGeometry(QRect(352, 468, 661, 226));
        QPalette palette;
        QBrush brush(QColor(0, 0, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(213, 253, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush2(QColor(156, 194, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        QBrush brush3(QColor(106, 104, 100, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        HanSoloCanvas->setPalette(palette);
        HanSoloCanvasCB = new QComboBox(HanSoloCanvas);
        HanSoloCanvasCB->setObjectName(QString::fromUtf8("HanSoloCanvasCB"));
        HanSoloCanvasCB->setGeometry(QRect(64, 200, 57, 20));
        currentCanvasLB = new QLabel(HanSoloCanvas);
        currentCanvasLB->setObjectName(QString::fromUtf8("currentCanvasLB"));
        currentCanvasLB->setGeometry(QRect(12, 201, 51, 16));
        displayControlsTB = new QTabWidget(HanSoloCanvas);
        displayControlsTB->setObjectName(QString::fromUtf8("displayControlsTB"));
        displayControlsTB->setGeometry(QRect(8, 16, 645, 181));
        QPalette palette1;
        QBrush brush4(QColor(181, 255, 202, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush4);
        QBrush brush5(QColor(255, 197, 131, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        QBrush brush6(QColor(212, 208, 200, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        displayControlsTB->setPalette(palette1);
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
        xMinLE->setGeometry(QRect(0, 45, 46, 21));
        xMinLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xMaxLE = new QLineEdit(actionsTP);
        xMaxLE->setObjectName(QString::fromUtf8("xMaxLE"));
        xMaxLE->setGeometry(QRect(60, 45, 46, 21));
        xMaxLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yMaxLE = new QLineEdit(actionsTP);
        yMaxLE->setObjectName(QString::fromUtf8("yMaxLE"));
        yMaxLE->setGeometry(QRect(60, 90, 46, 21));
        yMaxLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yMinL = new QLabel(actionsTP);
        yMinL->setObjectName(QString::fromUtf8("yMinL"));
        yMinL->setGeometry(QRect(10, 70, 36, 16));
        yMaxL = new QLabel(actionsTP);
        yMaxL->setObjectName(QString::fromUtf8("yMaxL"));
        yMaxL->setGeometry(QRect(60, 70, 41, 16));
        yMinLE = new QLineEdit(actionsTP);
        yMinLE->setObjectName(QString::fromUtf8("yMinLE"));
        yMinLE->setGeometry(QRect(0, 90, 46, 21));
        yMinLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        zMinL = new QLabel(actionsTP);
        zMinL->setObjectName(QString::fromUtf8("zMinL"));
        zMinL->setGeometry(QRect(10, 110, 36, 16));
        zMinLE = new QLineEdit(actionsTP);
        zMinLE->setObjectName(QString::fromUtf8("zMinLE"));
        zMinLE->setGeometry(QRect(0, 130, 46, 21));
        zMinLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        zMaxLE = new QLineEdit(actionsTP);
        zMaxLE->setObjectName(QString::fromUtf8("zMaxLE"));
        zMaxLE->setGeometry(QRect(60, 130, 46, 21));
        zMaxLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        zMaxL = new QLabel(actionsTP);
        zMaxL->setObjectName(QString::fromUtf8("zMaxL"));
        zMaxL->setGeometry(QRect(60, 110, 41, 16));
        shiftPlusPB = new QPushButton(actionsTP);
        shiftPlusPB->setObjectName(QString::fromUtf8("shiftPlusPB"));
        shiftPlusPB->setGeometry(QRect(270, 50, 16, 16));
        shiftLE = new QLineEdit(actionsTP);
        shiftLE->setObjectName(QString::fromUtf8("shiftLE"));
        shiftLE->setGeometry(QRect(210, 50, 61, 16));
        shiftLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        shiftMinusPB = new QPushButton(actionsTP);
        shiftMinusPB->setObjectName(QString::fromUtf8("shiftMinusPB"));
        shiftMinusPB->setGeometry(QRect(290, 50, 16, 16));
        fitCB = new QCheckBox(actionsTP);
        fitCB->setObjectName(QString::fromUtf8("fitCB"));
        fitCB->setGeometry(QRect(480, 40, 131, 16));
        hanSoloStatCB = new QCheckBox(actionsTP);
        hanSoloStatCB->setObjectName(QString::fromUtf8("hanSoloStatCB"));
        hanSoloStatCB->setGeometry(QRect(480, 20, 126, 16));
        logxCB = new QCheckBox(actionsTP);
        logxCB->setObjectName(QString::fromUtf8("logxCB"));
        logxCB->setGeometry(QRect(130, 50, 61, 16));
        logxCB->setAutoExclusive(false);
        logyCB = new QCheckBox(actionsTP);
        logyCB->setObjectName(QString::fromUtf8("logyCB"));
        logyCB->setGeometry(QRect(130, 90, 61, 16));
        logyCB->setAutoExclusive(false);
        twoDGB = new QGroupBox(actionsTP);
        twoDGB->setObjectName(QString::fromUtf8("twoDGB"));
        twoDGB->setGeometry(QRect(480, 70, 281, 71));
        surfaceRB = new QRadioButton(twoDGB);
        surfaceRB->setObjectName(QString::fromUtf8("surfaceRB"));
        surfaceRB->setGeometry(QRect(10, 0, 71, 21));
        contourRB = new QRadioButton(twoDGB);
        contourRB->setObjectName(QString::fromUtf8("contourRB"));
        contourRB->setGeometry(QRect(140, 0, 71, 21));
        legoRB = new QRadioButton(twoDGB);
        legoRB->setObjectName(QString::fromUtf8("legoRB"));
        legoRB->setGeometry(QRect(10, 20, 51, 21));
        lego2RB = new QRadioButton(twoDGB);
        lego2RB->setObjectName(QString::fromUtf8("lego2RB"));
        lego2RB->setGeometry(QRect(140, 20, 61, 21));
        colzRB = new QRadioButton(twoDGB);
        colzRB->setObjectName(QString::fromUtf8("colzRB"));
        colzRB->setGeometry(QRect(10, 40, 56, 21));
        colzRB->setChecked(true);
        logzCB = new QCheckBox(actionsTP);
        logzCB->setObjectName(QString::fromUtf8("logzCB"));
        logzCB->setGeometry(QRect(130, 130, 61, 16));
        logzCB->setAutoExclusive(false);
        shiftHS = new QSlider(actionsTP);
        shiftHS->setObjectName(QString::fromUtf8("shiftHS"));
        shiftHS->setGeometry(QRect(210, 100, 226, 16));
        shiftHS->setOrientation(Qt::Horizontal);
        displayControlsTB->addTab(actionsTP, QString());
        fitTP = new QWidget();
        fitTP->setObjectName(QString::fromUtf8("fitTP"));
        label = new QLabel(fitTP);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(8, 4, 111, 16));
        label_2 = new QLabel(fitTP);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(528, 0, 81, 16));
        parametersTW = new QTabWidget(fitTP);
        parametersTW->setObjectName(QString::fromUtf8("parametersTW"));
        parametersTW->setGeometry(QRect(512, 20, 117, 133));
        fitPB = new QPushButton(fitTP);
        fitPB->setObjectName(QString::fromUtf8("fitPB"));
        fitPB->setGeometry(QRect(264, 112, 40, 20));
        fitPB->setMaximumSize(QSize(600, 100));
        QPalette palette2;
        QBrush brush7(QColor(0, 255, 0, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        fitPB->setPalette(palette2);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Sans Serif"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        fitPB->setFont(font1);
        fitPB->setIconSize(QSize(15, 24));
        label_3 = new QLabel(fitTP);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(264, 88, 46, 16));
        fitMethodLE = new QLineEdit(fitTP);
        fitMethodLE->setObjectName(QString::fromUtf8("fitMethodLE"));
        fitMethodLE->setGeometry(QRect(316, 84, 191, 21));
        fitRangeMinSB = new QDoubleSpinBox(fitTP);
        fitRangeMinSB->setObjectName(QString::fromUtf8("fitRangeMinSB"));
        fitRangeMinSB->setGeometry(QRect(328, 60, 71, 20));
        fitRangeMaxSB = new QDoubleSpinBox(fitTP);
        fitRangeMaxSB->setObjectName(QString::fromUtf8("fitRangeMaxSB"));
        fitRangeMaxSB->setGeometry(QRect(420, 60, 81, 21));
        fitLimitCB = new QCheckBox(fitTP);
        fitLimitCB->setObjectName(QString::fromUtf8("fitLimitCB"));
        fitLimitCB->setGeometry(QRect(260, 60, 66, 20));
        fitFuncLW = new QListWidget(fitTP);
        fitFuncLW->setObjectName(QString::fromUtf8("fitFuncLW"));
        fitFuncLW->setGeometry(QRect(5, 20, 249, 133));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        fitFuncLW->setFont(font2);
        fitFuncLE = new QLineEdit(fitTP);
        fitFuncLE->setObjectName(QString::fromUtf8("fitFuncLE"));
        fitFuncLE->setGeometry(QRect(264, 36, 241, 21));
        customFunctionCB = new QCheckBox(fitTP);
        customFunctionCB->setObjectName(QString::fromUtf8("customFunctionCB"));
        customFunctionCB->setGeometry(QRect(260, 17, 151, 20));
        displayControlsTB->addTab(fitTP, QString());
        splitGB = new QGroupBox(HanSoloCanvas);
        splitGB->setObjectName(QString::fromUtf8("splitGB"));
        splitGB->setGeometry(QRect(130, 201, 161, 20));
        currentCanvasLB_2 = new QLabel(splitGB);
        currentCanvasLB_2->setObjectName(QString::fromUtf8("currentCanvasLB_2"));
        currentCanvasLB_2->setGeometry(QRect(5, 2, 36, 16));
        splitCanvasXYRB = new QRadioButton(splitGB);
        splitCanvasXYRB->setObjectName(QString::fromUtf8("splitCanvasXYRB"));
        splitCanvasXYRB->setGeometry(QRect(40, 2, 41, 16));
        splitCanvasXYRB->setChecked(true);
        splitCanvasXRB = new QRadioButton(splitGB);
        splitCanvasXRB->setObjectName(QString::fromUtf8("splitCanvasXRB"));
        splitCanvasXRB->setGeometry(QRect(88, 2, 31, 16));
        splitCanvasYRB = new QRadioButton(splitGB);
        splitCanvasYRB->setObjectName(QString::fromUtf8("splitCanvasYRB"));
        splitCanvasYRB->setGeometry(QRect(128, 2, 31, 16));
        saveImagePB = new QPushButton(HanSoloCanvas);
        saveImagePB->setObjectName(QString::fromUtf8("saveImagePB"));
        saveImagePB->setGeometry(QRect(300, 200, 111, 20));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        saveImagePB->setPalette(palette3);
        openFilePB = new QPushButton(viewerWindow);
        openFilePB->setObjectName(QString::fromUtf8("openFilePB"));
        openFilePB->setGeometry(QRect(6, 4, 86, 20));
        QPalette palette4;
        QBrush brush8(QColor(0, 170, 0, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        openFilePB->setPalette(palette4);
        saveComponentsPB = new QPushButton(viewerWindow);
        saveComponentsPB->setObjectName(QString::fromUtf8("saveComponentsPB"));
        saveComponentsPB->setGeometry(QRect(253, 4, 91, 20));
        QPalette palette5;
        QBrush brush9(QColor(255, 0, 0, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        saveComponentsPB->setPalette(palette5);
        HanSoloFitterInputFile = new QComboBox(viewerWindow);
        HanSoloFitterInputFile->setObjectName(QString::fromUtf8("HanSoloFitterInputFile"));
        HanSoloFitterInputFile->setGeometry(QRect(4, 28, 341, 20));
        HanSoloTreeFrame = new QFrame(viewerWindow);
        HanSoloTreeFrame->setObjectName(QString::fromUtf8("HanSoloTreeFrame"));
        HanSoloTreeFrame->setGeometry(QRect(4, 52, 341, 645));
        HanSoloTreeFrame->setSizeIncrement(QSize(0, 0));
        HanSoloTreeFrame->setBaseSize(QSize(0, 0));
        QPalette palette6;
        QBrush brush10(QColor(255, 255, 212, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush10);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        HanSoloTreeFrame->setPalette(palette6);
        HanSoloTreeFrame->setFrameShape(QFrame::Box);
        HanSoloTreeFrame->setFrameShadow(QFrame::Raised);
        HanSoloTreeFrame->setLineWidth(2);
        HanSoloTreeFrame->setMidLineWidth(1);
        theCanvas = new QMdiSubWindow(viewerWindow);
        theCanvas->setObjectName(QString::fromUtf8("theCanvas"));
        theCanvas->setGeometry(QRect(356, 4, 653, 461));
        centraltabWidget->addTab(viewerWindow, QString());
        fitterWindow = new QWidget();
        fitterWindow->setObjectName(QString::fromUtf8("fitterWindow"));
        openRootFile = new QPushButton(fitterWindow);
        openRootFile->setObjectName(QString::fromUtf8("openRootFile"));
        openRootFile->setGeometry(QRect(4, 8, 69, 57));
        QPalette palette7;
        QBrush brush11(QColor(0, 0, 0, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush7);
        QBrush brush12(QColor(127, 255, 127, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Light, brush12);
        QBrush brush13(QColor(63, 255, 63, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        QBrush brush14(QColor(0, 127, 0, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette7.setBrush(QPalette::Active, QPalette::Mid, brush8);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush11);
        QBrush brush15(QColor(255, 255, 255, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::BrightText, brush15);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush11);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette7.setBrush(QPalette::Active, QPalette::Shadow, brush11);
        palette7.setBrush(QPalette::Active, QPalette::AlternateBase, brush12);
        QBrush brush16(QColor(255, 255, 220, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipBase, brush16);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipText, brush11);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette7.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette7.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette7.setBrush(QPalette::Inactive, QPalette::Mid, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush11);
        palette7.setBrush(QPalette::Inactive, QPalette::BrightText, brush15);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush11);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::Shadow, brush11);
        palette7.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush12);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush16);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush11);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette7.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette7.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette7.setBrush(QPalette::Disabled, QPalette::Mid, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette7.setBrush(QPalette::Disabled, QPalette::BrightText, brush15);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::Shadow, brush11);
        palette7.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush16);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush11);
        openRootFile->setPalette(palette7);
        openRootFile->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
        convertFileListTV = new QTableView(fitterWindow);
        convertFileListTV->setObjectName(QString::fromUtf8("convertFileListTV"));
        convertFileListTV->setGeometry(QRect(76, 10, 481, 81));
        convertFileListTV->setEditTriggers(QAbstractItemView::NoEditTriggers);
        convertFileListTV->horizontalHeader()->setMinimumSectionSize(200);
        convertFileListTV->verticalHeader()->setDefaultSectionSize(19);
        convertResetListPB = new QPushButton(fitterWindow);
        convertResetListPB->setObjectName(QString::fromUtf8("convertResetListPB"));
        convertResetListPB->setGeometry(QRect(4, 70, 70, 20));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush9);
        QBrush brush17(QColor(255, 127, 127, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Light, brush17);
        QBrush brush18(QColor(255, 63, 63, 255));
        brush18.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Midlight, brush18);
        QBrush brush19(QColor(127, 0, 0, 255));
        brush19.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Dark, brush19);
        QBrush brush20(QColor(170, 0, 0, 255));
        brush20.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Mid, brush20);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush11);
        palette8.setBrush(QPalette::Active, QPalette::BrightText, brush15);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush11);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush15);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush9);
        palette8.setBrush(QPalette::Active, QPalette::Shadow, brush11);
        palette8.setBrush(QPalette::Active, QPalette::AlternateBase, brush17);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipBase, brush16);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipText, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette8.setBrush(QPalette::Inactive, QPalette::Light, brush17);
        palette8.setBrush(QPalette::Inactive, QPalette::Midlight, brush18);
        palette8.setBrush(QPalette::Inactive, QPalette::Dark, brush19);
        palette8.setBrush(QPalette::Inactive, QPalette::Mid, brush20);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::BrightText, brush15);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush15);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush9);
        palette8.setBrush(QPalette::Inactive, QPalette::Shadow, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush17);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush16);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush11);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush19);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::Light, brush17);
        palette8.setBrush(QPalette::Disabled, QPalette::Midlight, brush18);
        palette8.setBrush(QPalette::Disabled, QPalette::Dark, brush19);
        palette8.setBrush(QPalette::Disabled, QPalette::Mid, brush20);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush19);
        palette8.setBrush(QPalette::Disabled, QPalette::BrightText, brush15);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush19);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::Shadow, brush11);
        palette8.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush16);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush11);
        convertResetListPB->setPalette(palette8);
        fitterOptions = new QTabWidget(fitterWindow);
        fitterOptions->setObjectName(QString::fromUtf8("fitterOptions"));
        fitterOptions->setGeometry(QRect(6, 100, 1009, 577));
        fitterOptions->setTabPosition(QTabWidget::North);
        generalCategories = new QWidget();
        generalCategories->setObjectName(QString::fromUtf8("generalCategories"));
        label_4 = new QLabel(generalCategories);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 0, 131, 51));
        label_5 = new QLabel(generalCategories);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(530, 0, 131, 51));
        label_6 = new QLabel(generalCategories);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(756, -12, 131, 51));
        chargeFrame = new QFrame(generalCategories);
        chargeFrame->setObjectName(QString::fromUtf8("chargeFrame"));
        chargeFrame->setGeometry(QRect(0, 40, 330, 509));
        chargeFrame->setFrameShape(QFrame::NoFrame);
        chargeFrame->setFrameShadow(QFrame::Plain);
        chargeFrame->setLineWidth(0);
        efficiencyFrame = new QFrame(generalCategories);
        efficiencyFrame->setObjectName(QString::fromUtf8("efficiencyFrame"));
        efficiencyFrame->setGeometry(QRect(336, 40, 330, 509));
        efficiencyFrame->setFrameShape(QFrame::NoFrame);
        efficiencyFrame->setFrameShadow(QFrame::Plain);
        efficiencyFrame->setLineWidth(0);
        resolutionFrame = new QFrame(generalCategories);
        resolutionFrame->setObjectName(QString::fromUtf8("resolutionFrame"));
        resolutionFrame->setGeometry(QRect(672, 40, 330, 509));
        resolutionFrame->setFrameShape(QFrame::NoFrame);
        resolutionFrame->setFrameShadow(QFrame::Plain);
        resolutionFrame->setLineWidth(0);
        fitterOptions->addTab(generalCategories, QString());
        removeDut = new QWidget();
        removeDut->setObjectName(QString::fromUtf8("removeDut"));
        fitterOptions->addTab(removeDut, QString());
        addDut = new QWidget();
        addDut->setObjectName(QString::fromUtf8("addDut"));
        label_7 = new QLabel(addDut);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(1050, 0, 131, 51));
        resolutionFrame_0 = new QFrame(addDut);
        resolutionFrame_0->setObjectName(QString::fromUtf8("resolutionFrame_0"));
        resolutionFrame_0->setGeometry(QRect(1050, 40, 471, 561));
        resolutionFrame_0->setFrameShape(QFrame::StyledPanel);
        resolutionFrame_0->setFrameShadow(QFrame::Raised);
        efficiencyFrame_0 = new QFrame(addDut);
        efficiencyFrame_0->setObjectName(QString::fromUtf8("efficiencyFrame_0"));
        efficiencyFrame_0->setGeometry(QRect(504, 40, 500, 505));
        efficiencyFrame_0->setFrameShape(QFrame::StyledPanel);
        efficiencyFrame_0->setFrameShadow(QFrame::Raised);
        label_8 = new QLabel(addDut);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(530, 0, 131, 51));
        label_9 = new QLabel(addDut);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 0, 131, 51));
        chargeFrame_0 = new QFrame(addDut);
        chargeFrame_0->setObjectName(QString::fromUtf8("chargeFrame_0"));
        chargeFrame_0->setGeometry(QRect(0, 40, 500, 505));
        chargeFrame_0->setFrameShape(QFrame::StyledPanel);
        chargeFrame_0->setFrameShadow(QFrame::Raised);
        fitterOptions->addTab(addDut, QString());
        runButton = new QPushButton(fitterWindow);
        runButton->setObjectName(QString::fromUtf8("runButton"));
        runButton->setGeometry(QRect(8, 680, 45, 22));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette9.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette9.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        runButton->setPalette(palette9);
        openConfigurationFilePB = new QPushButton(fitterWindow);
        openConfigurationFilePB->setObjectName(QString::fromUtf8("openConfigurationFilePB"));
        openConfigurationFilePB->setGeometry(QRect(564, 8, 121, 20));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        palette10.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette10.setBrush(QPalette::Active, QPalette::Light, brush12);
        palette10.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        palette10.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette10.setBrush(QPalette::Active, QPalette::Mid, brush8);
        palette10.setBrush(QPalette::Active, QPalette::Text, brush11);
        palette10.setBrush(QPalette::Active, QPalette::BrightText, brush15);
        palette10.setBrush(QPalette::Active, QPalette::ButtonText, brush11);
        palette10.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette10.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette10.setBrush(QPalette::Active, QPalette::Shadow, brush11);
        palette10.setBrush(QPalette::Active, QPalette::AlternateBase, brush12);
        palette10.setBrush(QPalette::Active, QPalette::ToolTipBase, brush16);
        palette10.setBrush(QPalette::Active, QPalette::ToolTipText, brush11);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette10.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette10.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette10.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette10.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette10.setBrush(QPalette::Inactive, QPalette::Mid, brush8);
        palette10.setBrush(QPalette::Inactive, QPalette::Text, brush11);
        palette10.setBrush(QPalette::Inactive, QPalette::BrightText, brush15);
        palette10.setBrush(QPalette::Inactive, QPalette::ButtonText, brush11);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette10.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette10.setBrush(QPalette::Inactive, QPalette::Shadow, brush11);
        palette10.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush12);
        palette10.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush16);
        palette10.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush11);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette10.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette10.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette10.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette10.setBrush(QPalette::Disabled, QPalette::Mid, brush8);
        palette10.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette10.setBrush(QPalette::Disabled, QPalette::BrightText, brush15);
        palette10.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::Shadow, brush11);
        palette10.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush16);
        palette10.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush11);
        openConfigurationFilePB->setPalette(palette10);
        openConfigurationFilePB->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
        configurationFileFrame = new QLineEdit(fitterWindow);
        configurationFileFrame->setObjectName(QString::fromUtf8("configurationFileFrame"));
        configurationFileFrame->setEnabled(false);
        configurationFileFrame->setGeometry(QRect(688, 8, 285, 81));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        palette11.setBrush(QPalette::Active, QPalette::Text, brush11);
        palette11.setBrush(QPalette::Active, QPalette::ButtonText, brush11);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush15);
        palette11.setBrush(QPalette::Active, QPalette::Window, brush15);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette11.setBrush(QPalette::Inactive, QPalette::Text, brush11);
        palette11.setBrush(QPalette::Inactive, QPalette::ButtonText, brush11);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush15);
        palette11.setBrush(QPalette::Inactive, QPalette::Window, brush15);
        QBrush brush21(QColor(170, 0, 127, 255));
        brush21.setStyle(Qt::SolidPattern);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush21);
        palette11.setBrush(QPalette::Disabled, QPalette::Text, brush21);
        palette11.setBrush(QPalette::Disabled, QPalette::ButtonText, brush21);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush15);
        palette11.setBrush(QPalette::Disabled, QPalette::Window, brush15);
        configurationFileFrame->setPalette(palette11);
        configurationFileFrame->setFont(font2);
        configurationFileFrame->setReadOnly(true);
        saveConfigurationFile = new QPushButton(fitterWindow);
        saveConfigurationFile->setObjectName(QString::fromUtf8("saveConfigurationFile"));
        saveConfigurationFile->setGeometry(QRect(564, 28, 121, 20));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        QBrush brush22(QColor(255, 170, 0, 255));
        brush22.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Button, brush22);
        QBrush brush23(QColor(255, 213, 127, 255));
        brush23.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Light, brush23);
        QBrush brush24(QColor(255, 191, 63, 255));
        brush24.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Midlight, brush24);
        QBrush brush25(QColor(127, 85, 0, 255));
        brush25.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Dark, brush25);
        QBrush brush26(QColor(170, 113, 0, 255));
        brush26.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Mid, brush26);
        palette12.setBrush(QPalette::Active, QPalette::Text, brush11);
        palette12.setBrush(QPalette::Active, QPalette::BrightText, brush15);
        palette12.setBrush(QPalette::Active, QPalette::ButtonText, brush11);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush15);
        palette12.setBrush(QPalette::Active, QPalette::Window, brush22);
        palette12.setBrush(QPalette::Active, QPalette::Shadow, brush11);
        QBrush brush27(QColor(255, 212, 127, 255));
        brush27.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::AlternateBase, brush27);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipBase, brush16);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipText, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::Button, brush22);
        palette12.setBrush(QPalette::Inactive, QPalette::Light, brush23);
        palette12.setBrush(QPalette::Inactive, QPalette::Midlight, brush24);
        palette12.setBrush(QPalette::Inactive, QPalette::Dark, brush25);
        palette12.setBrush(QPalette::Inactive, QPalette::Mid, brush26);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::BrightText, brush15);
        palette12.setBrush(QPalette::Inactive, QPalette::ButtonText, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush15);
        palette12.setBrush(QPalette::Inactive, QPalette::Window, brush22);
        palette12.setBrush(QPalette::Inactive, QPalette::Shadow, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush27);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush16);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush11);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush25);
        palette12.setBrush(QPalette::Disabled, QPalette::Button, brush22);
        palette12.setBrush(QPalette::Disabled, QPalette::Light, brush23);
        palette12.setBrush(QPalette::Disabled, QPalette::Midlight, brush24);
        palette12.setBrush(QPalette::Disabled, QPalette::Dark, brush25);
        palette12.setBrush(QPalette::Disabled, QPalette::Mid, brush26);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush25);
        palette12.setBrush(QPalette::Disabled, QPalette::BrightText, brush15);
        palette12.setBrush(QPalette::Disabled, QPalette::ButtonText, brush25);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush22);
        palette12.setBrush(QPalette::Disabled, QPalette::Window, brush22);
        palette12.setBrush(QPalette::Disabled, QPalette::Shadow, brush11);
        palette12.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush22);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush16);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush11);
        saveConfigurationFile->setPalette(palette12);
        reset = new QPushButton(fitterWindow);
        reset->setObjectName(QString::fromUtf8("reset"));
        reset->setGeometry(QRect(564, 72, 121, 20));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        palette13.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette13.setBrush(QPalette::Active, QPalette::Light, brush17);
        palette13.setBrush(QPalette::Active, QPalette::Midlight, brush18);
        palette13.setBrush(QPalette::Active, QPalette::Dark, brush19);
        palette13.setBrush(QPalette::Active, QPalette::Mid, brush20);
        palette13.setBrush(QPalette::Active, QPalette::Text, brush11);
        palette13.setBrush(QPalette::Active, QPalette::BrightText, brush15);
        palette13.setBrush(QPalette::Active, QPalette::ButtonText, brush11);
        palette13.setBrush(QPalette::Active, QPalette::Base, brush15);
        palette13.setBrush(QPalette::Active, QPalette::Window, brush9);
        palette13.setBrush(QPalette::Active, QPalette::Shadow, brush11);
        palette13.setBrush(QPalette::Active, QPalette::AlternateBase, brush17);
        palette13.setBrush(QPalette::Active, QPalette::ToolTipBase, brush16);
        palette13.setBrush(QPalette::Active, QPalette::ToolTipText, brush11);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette13.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette13.setBrush(QPalette::Inactive, QPalette::Light, brush17);
        palette13.setBrush(QPalette::Inactive, QPalette::Midlight, brush18);
        palette13.setBrush(QPalette::Inactive, QPalette::Dark, brush19);
        palette13.setBrush(QPalette::Inactive, QPalette::Mid, brush20);
        palette13.setBrush(QPalette::Inactive, QPalette::Text, brush11);
        palette13.setBrush(QPalette::Inactive, QPalette::BrightText, brush15);
        palette13.setBrush(QPalette::Inactive, QPalette::ButtonText, brush11);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush15);
        palette13.setBrush(QPalette::Inactive, QPalette::Window, brush9);
        palette13.setBrush(QPalette::Inactive, QPalette::Shadow, brush11);
        palette13.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush17);
        palette13.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush16);
        palette13.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush11);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush19);
        palette13.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::Light, brush17);
        palette13.setBrush(QPalette::Disabled, QPalette::Midlight, brush18);
        palette13.setBrush(QPalette::Disabled, QPalette::Dark, brush19);
        palette13.setBrush(QPalette::Disabled, QPalette::Mid, brush20);
        palette13.setBrush(QPalette::Disabled, QPalette::Text, brush19);
        palette13.setBrush(QPalette::Disabled, QPalette::BrightText, brush15);
        palette13.setBrush(QPalette::Disabled, QPalette::ButtonText, brush19);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::Window, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::Shadow, brush11);
        palette13.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush16);
        palette13.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush11);
        reset->setPalette(palette13);
        reset->setAutoFillBackground(false);
        resetDut = new QPushButton(fitterWindow);
        resetDut->setObjectName(QString::fromUtf8("resetDut"));
        resetDut->setGeometry(QRect(564, 48, 121, 20));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        QBrush brush28(QColor(0, 145, 255, 255));
        brush28.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Button, brush28);
        QBrush brush29(QColor(127, 200, 255, 255));
        brush29.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Light, brush29);
        QBrush brush30(QColor(63, 172, 255, 255));
        brush30.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Midlight, brush30);
        QBrush brush31(QColor(0, 72, 127, 255));
        brush31.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Dark, brush31);
        QBrush brush32(QColor(0, 97, 170, 255));
        brush32.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Mid, brush32);
        palette14.setBrush(QPalette::Active, QPalette::Text, brush11);
        palette14.setBrush(QPalette::Active, QPalette::BrightText, brush15);
        palette14.setBrush(QPalette::Active, QPalette::ButtonText, brush11);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush15);
        palette14.setBrush(QPalette::Active, QPalette::Window, brush28);
        palette14.setBrush(QPalette::Active, QPalette::Shadow, brush11);
        palette14.setBrush(QPalette::Active, QPalette::AlternateBase, brush29);
        palette14.setBrush(QPalette::Active, QPalette::ToolTipBase, brush16);
        palette14.setBrush(QPalette::Active, QPalette::ToolTipText, brush11);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette14.setBrush(QPalette::Inactive, QPalette::Button, brush28);
        palette14.setBrush(QPalette::Inactive, QPalette::Light, brush29);
        palette14.setBrush(QPalette::Inactive, QPalette::Midlight, brush30);
        palette14.setBrush(QPalette::Inactive, QPalette::Dark, brush31);
        palette14.setBrush(QPalette::Inactive, QPalette::Mid, brush32);
        palette14.setBrush(QPalette::Inactive, QPalette::Text, brush11);
        palette14.setBrush(QPalette::Inactive, QPalette::BrightText, brush15);
        palette14.setBrush(QPalette::Inactive, QPalette::ButtonText, brush11);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush15);
        palette14.setBrush(QPalette::Inactive, QPalette::Window, brush28);
        palette14.setBrush(QPalette::Inactive, QPalette::Shadow, brush11);
        palette14.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush29);
        palette14.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush16);
        palette14.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush11);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush31);
        palette14.setBrush(QPalette::Disabled, QPalette::Button, brush28);
        palette14.setBrush(QPalette::Disabled, QPalette::Light, brush29);
        palette14.setBrush(QPalette::Disabled, QPalette::Midlight, brush30);
        palette14.setBrush(QPalette::Disabled, QPalette::Dark, brush31);
        palette14.setBrush(QPalette::Disabled, QPalette::Mid, brush32);
        palette14.setBrush(QPalette::Disabled, QPalette::Text, brush31);
        palette14.setBrush(QPalette::Disabled, QPalette::BrightText, brush15);
        palette14.setBrush(QPalette::Disabled, QPalette::ButtonText, brush31);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush28);
        palette14.setBrush(QPalette::Disabled, QPalette::Window, brush28);
        palette14.setBrush(QPalette::Disabled, QPalette::Shadow, brush11);
        palette14.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush28);
        palette14.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush16);
        palette14.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush11);
        resetDut->setPalette(palette14);
        resetDut->setAutoFillBackground(false);
        centraltabWidget->addTab(fitterWindow, QString());
        statusbar = new QStatusBar(HanSoloFitter);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setGeometry(QRect(0, 0, 3, 22));

        retranslateUi(HanSoloFitter);

        centraltabWidget->setCurrentIndex(1);
        displayControlsTB->setCurrentIndex(1);
        parametersTW->setCurrentIndex(-1);
        fitterOptions->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(HanSoloFitter);
    } // setupUi

    void retranslateUi(QDialog *HanSoloFitter)
    {
        HanSoloFitter->setWindowTitle(QApplication::translate("HanSoloFitter", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionAdvanced_Fitter->setText(QApplication::translate("HanSoloFitter", "Advanced Fitter", 0, QApplication::UnicodeUTF8));
        actionCharge->setText(QApplication::translate("HanSoloFitter", "Charge", 0, QApplication::UnicodeUTF8));
        actionEfficiency->setText(QApplication::translate("HanSoloFitter", "Efficiency", 0, QApplication::UnicodeUTF8));
        actionResolution->setText(QApplication::translate("HanSoloFitter", "Resolution", 0, QApplication::UnicodeUTF8));
        actionAll->setText(QApplication::translate("HanSoloFitter", "All", 0, QApplication::UnicodeUTF8));
        actionPlanar->setText(QApplication::translate("HanSoloFitter", "Planar", 0, QApplication::UnicodeUTF8));
        action3_D->setText(QApplication::translate("HanSoloFitter", "3D", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        centraltabWidget->setToolTip(QApplication::translate("HanSoloFitter", "<html><head/><body><p>Fitter</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        HanSoloCanvas->setTitle(QApplication::translate("HanSoloFitter", "Controls", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        HanSoloCanvasCB->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        HanSoloCanvasCB->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        HanSoloCanvasCB->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        currentCanvasLB->setText(QApplication::translate("HanSoloFitter", "Canvas:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        unZoomPB->setToolTip(QApplication::translate("HanSoloFitter", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        unZoomPB->setText(QApplication::translate("HanSoloFitter", "Unzoom", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        setRangePB->setToolTip(QApplication::translate("HanSoloFitter", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        setRangePB->setText(QApplication::translate("HanSoloFitter", "Range", 0, QApplication::UnicodeUTF8));
        xMinL->setText(QApplication::translate("HanSoloFitter", "X Min", 0, QApplication::UnicodeUTF8));
        xMaxL->setText(QApplication::translate("HanSoloFitter", "X Max", 0, QApplication::UnicodeUTF8));
        xMinLE->setText(QApplication::translate("HanSoloFitter", "0", 0, QApplication::UnicodeUTF8));
        xMaxLE->setText(QApplication::translate("HanSoloFitter", "0", 0, QApplication::UnicodeUTF8));
        yMaxLE->setText(QApplication::translate("HanSoloFitter", "0", 0, QApplication::UnicodeUTF8));
        yMinL->setText(QApplication::translate("HanSoloFitter", "Y Min", 0, QApplication::UnicodeUTF8));
        yMaxL->setText(QApplication::translate("HanSoloFitter", "Y Max", 0, QApplication::UnicodeUTF8));
        yMinLE->setText(QApplication::translate("HanSoloFitter", "0", 0, QApplication::UnicodeUTF8));
        zMinL->setText(QApplication::translate("HanSoloFitter", "Z Min", 0, QApplication::UnicodeUTF8));
        zMinLE->setText(QApplication::translate("HanSoloFitter", "0", 0, QApplication::UnicodeUTF8));
        zMaxLE->setText(QApplication::translate("HanSoloFitter", "0", 0, QApplication::UnicodeUTF8));
        zMaxL->setText(QApplication::translate("HanSoloFitter", "Z Max", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        shiftPlusPB->setToolTip(QApplication::translate("HanSoloFitter", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        shiftPlusPB->setText(QApplication::translate("HanSoloFitter", "+", 0, QApplication::UnicodeUTF8));
        shiftLE->setText(QApplication::translate("HanSoloFitter", "1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        shiftMinusPB->setToolTip(QApplication::translate("HanSoloFitter", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        shiftMinusPB->setText(QApplication::translate("HanSoloFitter", "-", 0, QApplication::UnicodeUTF8));
        fitCB->setText(QApplication::translate("HanSoloFitter", "Display fit results", 0, QApplication::UnicodeUTF8));
        hanSoloStatCB->setText(QApplication::translate("HanSoloFitter", "Display statistics", 0, QApplication::UnicodeUTF8));
        logxCB->setText(QApplication::translate("HanSoloFitter", "Log X", 0, QApplication::UnicodeUTF8));
        logyCB->setText(QApplication::translate("HanSoloFitter", "Log Y", 0, QApplication::UnicodeUTF8));
        twoDGB->setTitle(QString());
        surfaceRB->setText(QApplication::translate("HanSoloFitter", "Surface", 0, QApplication::UnicodeUTF8));
        contourRB->setText(QApplication::translate("HanSoloFitter", "Contour", 0, QApplication::UnicodeUTF8));
        legoRB->setText(QApplication::translate("HanSoloFitter", "Lego", 0, QApplication::UnicodeUTF8));
        lego2RB->setText(QApplication::translate("HanSoloFitter", "Lego2", 0, QApplication::UnicodeUTF8));
        colzRB->setText(QApplication::translate("HanSoloFitter", "COLZ", 0, QApplication::UnicodeUTF8));
        logzCB->setText(QApplication::translate("HanSoloFitter", "Log Z", 0, QApplication::UnicodeUTF8));
        displayControlsTB->setTabText(displayControlsTB->indexOf(actionsTP), QApplication::translate("HanSoloFitter", "Actions", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("HanSoloFitter", "Choose a Function", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("HanSoloFitter", "Parameters", 0, QApplication::UnicodeUTF8));
        fitPB->setText(QApplication::translate("HanSoloFitter", "Fit!", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("HanSoloFitter", "Method", 0, QApplication::UnicodeUTF8));
        fitLimitCB->setText(QApplication::translate("HanSoloFitter", "Limit x", 0, QApplication::UnicodeUTF8));
        customFunctionCB->setText(QApplication::translate("HanSoloFitter", "Or write it your own", 0, QApplication::UnicodeUTF8));
        displayControlsTB->setTabText(displayControlsTB->indexOf(fitTP), QApplication::translate("HanSoloFitter", "Fit Manager", 0, QApplication::UnicodeUTF8));
        splitGB->setTitle(QString());
        currentCanvasLB_2->setText(QApplication::translate("HanSoloFitter", "Split:", 0, QApplication::UnicodeUTF8));
        splitCanvasXYRB->setText(QApplication::translate("HanSoloFitter", "XY", 0, QApplication::UnicodeUTF8));
        splitCanvasXRB->setText(QApplication::translate("HanSoloFitter", "X", 0, QApplication::UnicodeUTF8));
        splitCanvasYRB->setText(QApplication::translate("HanSoloFitter", "Y", 0, QApplication::UnicodeUTF8));
        saveImagePB->setText(QApplication::translate("HanSoloFitter", "Save Image", 0, QApplication::UnicodeUTF8));
        openFilePB->setText(QApplication::translate("HanSoloFitter", "Open File", 0, QApplication::UnicodeUTF8));
        saveComponentsPB->setText(QApplication::translate("HanSoloFitter", "Save selected", 0, QApplication::UnicodeUTF8));
        centraltabWidget->setTabText(centraltabWidget->indexOf(viewerWindow), QApplication::translate("HanSoloFitter", "Viewer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openRootFile->setToolTip(QApplication::translate("HanSoloFitter", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Open ROOT file</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; t"
                        "ext-indent:0px;\">This button is used to open a <span style=\" font-weight:600; font-style:italic;\">ROOT</span> file </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">containing already (at least partially) </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">reconstructed tracks. At a bare minimum</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">such a file might contain events with only </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">hits, but these could be complemented </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">by resolved clusters or even reconstructed </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px"
                        "; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">tracks.</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        openRootFile->setText(QApplication::translate("HanSoloFitter", "SELECT\n"
".root\n"
"file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        convertFileListTV->setToolTip(QApplication::translate("HanSoloFitter", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">ROOT filename</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px"
                        ";\">The full path of the selected ROOT</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">input data file (these files contain</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">event objects).</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        convertResetListPB->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        convertResetListPB->setText(QApplication::translate("HanSoloFitter", "Reset", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fitterOptions->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("HanSoloFitter", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Charge</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("HanSoloFitter", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Efficiency</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("HanSoloFitter", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Resolution</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        fitterOptions->setTabText(fitterOptions->indexOf(generalCategories), QApplication::translate("HanSoloFitter", "General", 0, QApplication::UnicodeUTF8));
        fitterOptions->setTabText(fitterOptions->indexOf(removeDut), QApplication::translate("HanSoloFitter", "-", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("HanSoloFitter", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Resolution</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("HanSoloFitter", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Efficiency</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("HanSoloFitter", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Charge</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        fitterOptions->setTabText(fitterOptions->indexOf(addDut), QApplication::translate("HanSoloFitter", "+", 0, QApplication::UnicodeUTF8));
        runButton->setText(QApplication::translate("HanSoloFitter", "RUN", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openConfigurationFilePB->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        openConfigurationFilePB->setText(QApplication::translate("HanSoloFitter", "Load config file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        configurationFileFrame->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        configurationFileFrame->setText(QApplication::translate("HanSoloFitter", "                                       Load configuration file (.xml)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        saveConfigurationFile->setToolTip(QApplication::translate("HanSoloFitter", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Open ROOT file</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; t"
                        "ext-indent:0px;\">This button is used to open a <span style=\" font-weight:600; font-style:italic;\">ROOT</span> file </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">containing already (at least partially) </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">reconstructed tracks. At a bare minimum</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">such a file might contain events with only </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">hits, but these could be complemented </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">by resolved clusters or even reconstructed </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px"
                        "; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">tracks.</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        saveConfigurationFile->setText(QApplication::translate("HanSoloFitter", "Save config", 0, QApplication::UnicodeUTF8));
        reset->setText(QApplication::translate("HanSoloFitter", "Reset All", 0, QApplication::UnicodeUTF8));
        resetDut->setText(QApplication::translate("HanSoloFitter", "Reset Dut", 0, QApplication::UnicodeUTF8));
        centraltabWidget->setTabText(centraltabWidget->indexOf(fitterWindow), QApplication::translate("HanSoloFitter", "Fitter", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HanSoloFitter: public Ui_HanSoloFitter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANSOLOFITTER_H
