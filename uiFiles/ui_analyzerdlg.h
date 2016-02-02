/********************************************************************************
** Form generated from reading UI file 'analyzerdlg.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYZERDLG_H
#define UI_ANALYZERDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTableWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include <customCheckBox.h>
#include <customLineEdit.h>
#include <customSpinBox.h>
#include <customTableWidget.h>
#include "PixelMatrix.h"
#include "PixelMatrixCut.h"

QT_BEGIN_NAMESPACE

class Ui_AnalyzerDlg
{
public:
    QTabWidget *studiesTW;
    QWidget *converterT;
    QGroupBox *runFilesManagementGB_2;
    QTableView *convertFileListTV;
    QPushButton *convertResetListPB;
    QPushButton *openRootFilePB;
    QGroupBox *convertConverterControlsGB;
    customSpinBox *convertMaxEventsSB;
    QLabel *convertTotalEventsLB;
    QLineEdit *convertTotalEventsLE;
    QPushButton *runConverterPB;
    QLabel *convertMaxThreadsLB;
    customSpinBox *convertMaxThreadsSB;
    customCheckBox *convertMaxEventsCB;
    QWidget *infosT;
    customTableWidget *detectorsInfoTW;
    QWidget *calibrationT;
    QPushButton *makeCalibPlotsPB;
    QGroupBox *calibPlotsGB;
    QCheckBox *calibParsCB;
    QCheckBox *calibDispersionCB;
    QListWidget *dutsListLW;
    QGroupBox *groupBox_2;
    QLineEdit *calibPlotsFileLE;
    QLineEdit *loadedGeoFileLE_2;
    QPushButton *openCalibrationPlotsPB;
    QLineEdit *loadedCalibFileLE;
    QPushButton *openCalibrationsFilePB;
    QPushButton *calibrationLoadGeoFilePB;
    QWidget *windowsT;
    QLabel *limitWindowLB;
    customLineEdit *maxWindowEventsLE;
    QTabWidget *dutsPreAnalysisTW;
    QWidget *tab_6;
    QGroupBox *removePixelDUT0GB;
    QLabel *removeRowDUT0LB;
    QLineEdit *removeRowDUT0LE;
    QLabel *removeColDUT0LB;
    QLineEdit *removeColDUT0LE;
    QPushButton *removePxlDUT0PB;
    QPushButton *resetRemovedPxlsDUT0PB;
    QGroupBox *groupBox_3;
    customLineEdit *higherColDUT0LE;
    QLabel *row0LB;
    customLineEdit *lowerColDUT0LE;
    QLabel *col0LB;
    customLineEdit *lowerRowDUT0LE;
    QLabel *higherEdge0LB;
    QLabel *lowerEdge0LB;
    customLineEdit *higherRowDUT0LE;
    QWidget *tab_7;
    QGroupBox *removePixelDUT0GB_2;
    QLabel *removeRowDUT0LB_2;
    QLineEdit *removeRowDUT1LE;
    QLabel *removeColDUT0LB_2;
    QLineEdit *removeColDUT1LE;
    QPushButton *removePxlDUT1PB;
    QPushButton *resetRemovedPxlsDUT1PB;
    QGroupBox *groupBox_4;
    customLineEdit *higherColDUT1LE;
    QLabel *row0LB_2;
    customLineEdit *lowerColDUT1LE;
    QLabel *col0LB_2;
    customLineEdit *lowerRowDUT1LE;
    QLabel *higherEdge0LB_2;
    QLabel *lowerEdge0LB_2;
    customLineEdit *higherRowDUT1LE;
    QWidget *tab_8;
    QGroupBox *removePixelDUT0GB_3;
    QLabel *removeRowDUT0LB_3;
    QLineEdit *removeRowDUT2LE;
    QLabel *removeColDUT0LB_3;
    QLineEdit *removeColDUT2LE;
    QPushButton *removePxlDUT2PB;
    QPushButton *resetRemovedPxlsDUT2PB;
    QGroupBox *groupBox_5;
    customLineEdit *higherColDUT2LE;
    QLabel *row0LB_3;
    customLineEdit *lowerColDUT2LE;
    QLabel *col0LB_3;
    customLineEdit *lowerRowDUT2LE;
    QLabel *higherEdge0LB_3;
    QLabel *lowerEdge0LB_3;
    customLineEdit *higherRowDUT2LE;
    QWidget *tab_9;
    QGroupBox *groupBox_6;
    customLineEdit *higherColDUT3LE;
    QLabel *row0LB_4;
    customLineEdit *lowerColDUT3LE;
    QLabel *col0LB_4;
    customLineEdit *lowerRowDUT3LE;
    QLabel *higherEdge0LB_4;
    QLabel *lowerEdge0LB_4;
    customLineEdit *higherRowDUT3LE;
    QGroupBox *removePixelDUT0GB_4;
    QLabel *removeRowDUT0LB_4;
    QLineEdit *removeRowDUT3LE;
    QLabel *removeColDUT0LB_4;
    QLineEdit *removeColDUT3LE;
    QPushButton *removePxlDUT3PB;
    QPushButton *resetRemovedPxlsDUT3PB;
    QWidget *efficiencyT;
    QGroupBox *efficiencyCutsGB;
    QCheckBox *chi2EfficiencyCutCB;
    QDoubleSpinBox *chi2EfficiencyCutSB;
    QCheckBox *nTrackEfficiencyCutCB;
    QSpinBox *nTrackEfficiencyCutSB;
    QCheckBox *yErrEfficiencyCutCB;
    QDoubleSpinBox *yErrEfficiencyCutSB;
    QLabel *xErrEffLB;
    QCheckBox *xErrEfficiencyCutCB;
    QDoubleSpinBox *xErrEfficiencyCutSB;
    QLabel *xErrEffCutLB;
    QCheckBox *xEfficiencyCutCB;
    QDoubleSpinBox *yEfficiencyCutSB;
    QDoubleSpinBox *xEfficiencyCutSB;
    QCheckBox *yEfficiencyCutCB;
    QCheckBox *hitsEfficiencyCutCB;
    customSpinBox *hitsEfficiencyCutSB;
    QLabel *label_5;
    customCheckBox *efficiencyStandardCutsCB;
    customCheckBox *efficiencyExcludeBadPlanesCB;
    QGroupBox *groupBox_7;
    customLineEdit *efficiencyCuts4LE;
    customLineEdit *efficiencyCuts2LE;
    QCheckBox *efficiencyCuts2CB;
    QCheckBox *efficiencyCuts3CB;
    QCheckBox *efficiencyCuts4CB;
    customLineEdit *efficiencyCuts3LE;
    customLineEdit *efficiencyMainCutsLE;
    QCheckBox *efficiencyMainCutsCB;
    QWidget *chargeT;
    QGroupBox *chargeCutsGB;
    QCheckBox *chi2ChargeCutCB;
    QDoubleSpinBox *chi2ChargeCutSB;
    QCheckBox *nTrackChargeCutCB;
    QSpinBox *nTrackChargeCutSB;
    QCheckBox *xErrChargeCutCB;
    QCheckBox *yErrChargeCutCB;
    QDoubleSpinBox *xErrChargeCutSB;
    QDoubleSpinBox *yErrChargeCutSB;
    QLabel *xErrChargeLB;
    QLabel *yErrChargeLB;
    QCheckBox *xChargeCutCB;
    QCheckBox *yChargeCutCB;
    QDoubleSpinBox *xChargeCutSB;
    QDoubleSpinBox *yChargeCutSB;
    QLabel *xChargeLB;
    QLabel *yChargeLB;
    customSpinBox *hitsChargeCutSB;
    QCheckBox *hitsChargeCutCB;
    customSpinBox *chargePixelMinimumChargeSB;
    customSpinBox *chargeClusterMinimumChargeSB;
    QLabel *chargePixelMinimumChargeElectronLB;
    QLabel *chargeClusterMaximumChargeElectronLB;
    customSpinBox *chargeClusterMaximumChargeSB;
    QLabel *chargeClusterMinimumChargeElectronLB;
    customSpinBox *chargePixelMaximumChargeSB;
    QLabel *chargePixelMaximumChargeElectronLB;
    QLabel *chargePixelMinimumChargeLB;
    QLabel *chargePixelMaximumChargeLB;
    QLabel *chargeClusterMinimumChargeLB;
    QLabel *chargeClusterMaximumChargeLB;
    customCheckBox *chargeExcludeBadPlanesCB;
    QLabel *label_2;
    customSpinBox *chargeBadPlanesHitsSB;
    QLabel *label_4;
    customCheckBox *chargeStandardCutsCB;
    QLabel *parsCutLB;
    QTabWidget *parsTW;
    QWidget *tab_2;
    QCheckBox *par0minCB;
    customLineEdit *par0minLE;
    QCheckBox *par0maxCB;
    customLineEdit *par0maxLE;
    QWidget *tab_3;
    QCheckBox *par1minCB;
    QCheckBox *par1maxCB;
    customLineEdit *par1minLE;
    customLineEdit *par1maxLE;
    QWidget *tab_4;
    QCheckBox *par2minCB;
    customLineEdit *par2minLE;
    QCheckBox *par2maxCB;
    customLineEdit *par2maxLE;
    QWidget *tab_5;
    QCheckBox *par3minCB;
    customLineEdit *par3minLE;
    QCheckBox *par3maxCB;
    customLineEdit *par3maxLE;
    QTextBrowser *textBrowser;
    QGroupBox *groupBox_8;
    customLineEdit *chargeCuts5LE;
    customLineEdit *chargeCuts3LE;
    QCheckBox *chargeMainCutsCB;
    customLineEdit *chargeMainCutsLE;
    customLineEdit *chargeCuts2LE;
    QCheckBox *chargeCuts2CB;
    QCheckBox *chargeCuts6CB;
    customLineEdit *chargeCuts6LE;
    QCheckBox *chargeCuts3CB;
    QCheckBox *chargeCuts4CB;
    QCheckBox *chargeCuts5CB;
    customLineEdit *chargeCuts4LE;
    QWidget *resolutionT;
    QGroupBox *resolutionCutsGB;
    QCheckBox *chi2ResolutionCutCB;
    QDoubleSpinBox *chi2ResolutionCutSB;
    QCheckBox *nTrackResolutionCutCB;
    QSpinBox *nTrackResolutionCutSB;
    QCheckBox *xErrResolutionCutCB;
    QCheckBox *yErrResolutionCutCB;
    QDoubleSpinBox *xErrResolutionCutSB;
    QDoubleSpinBox *yErrResolutionCutSB;
    QLabel *xErrResolutionLB;
    QLabel *yErrResolutionLB;
    customSpinBox *hitsResolutionCutSB;
    QCheckBox *hitsResolutionCutCB;
    customCheckBox *resolutionStandardCutsCB;
    customCheckBox *resolutionExcludeBadPlanesCB;
    QLabel *label_6;
    QWidget *resolutionCanvasW;
    QGroupBox *groupBox_9;
    customLineEdit *resolutionCuts4LE;
    customLineEdit *resolutionMainCutsLE;
    QCheckBox *resolutionMainCutsCB;
    QCheckBox *resolutionCuts2CB;
    QCheckBox *resolutionCuts3CB;
    customLineEdit *resolutionCuts3LE;
    QCheckBox *resolutionCuts4CB;
    customLineEdit *resolutionCuts2LE;
    QWidget *runT;
    QGroupBox *analysesGB;
    customCheckBox *efficiencyCB;
    customCheckBox *chargeCB;
    customCheckBox *resolutionCB;
    customCheckBox *tracksCB;
    customCheckBox *windowsCB;
    QPushButton *runAnalysisPB;
    QLabel *runMaxThreadsLB;
    customSpinBox *runMaxThreadsSB;
    customSpinBox *runMaxEventsSB;
    QLabel *runTotalEventsLB;
    QLineEdit *runTotalEventsLE;
    customCheckBox *runMaxEventsCB;
    customCheckBox *tracksafterCB;
    QLabel *label;
    customSpinBox *priorityWindowsSB;
    customSpinBox *priorityTracksSB;
    customSpinBox *priorityEfficiencySB;
    customSpinBox *priorityChargeSB;
    customSpinBox *priorityResolutionSB;
    customSpinBox *priorityTracksAfterSB;
    QLabel *label_9;
    customCheckBox *resolutionFitCB;
    customCheckBox *chargeFitCB;
    customCheckBox *tracksafterFitCB;
    customCheckBox *efficiencyFitCB;
    customCheckBox *windowsFitCB;
    customCheckBox *tracksFitCB;
    QCheckBox *useCalibrationsCB;
    QListWidget *dutsListLW_2;
    QGroupBox *runFilesManagementGB;
    QLineEdit *outputFileLE;
    QPushButton *openNtupleFilePB;
    QPushButton *saveOutputFilePB;
    QTableView *runFileListTV;
    QPushButton *resetListPB;
    QWidget *tab;
    QPushButton *runScanPB;
    QPushButton *loadFileToScanListPB;
    QTableWidget *scanListTW;
    QPushButton *resetScanListPB;
    QWidget *tab8;
    PixelMatrix *pixelMatrixW;
    QWidget *cellT;
    PixelMatrixCut *pixelCutMatrixW;
    QListWidget *cutVarLW;
    QLineEdit *cutFileLE;
    QPushButton *cutFilePB;
    QLabel *label_7;
    QLabel *label_8;
    QGroupBox *groupBox;
    QRadioButton *cutDut0RB;
    QRadioButton *cutDut1RB;
    QTextEdit *cutDescriptionTE;
    QProgressBar *progressBar;
    QPushButton *abortPB;
    QPushButton *openConfigurationFilePB;
    QLineEdit *loadedConfigurationFileLE;
    QPushButton *saveConfigurationFile;
    QLabel *runningThreadsLB;
    QLineEdit *runningThreadsLE;
    QLabel *processedEventsLB;
    QLineEdit *processedEventsLE;
    QLabel *activityLB;

    void setupUi(QDialog *AnalyzerDlg)
    {
        if (AnalyzerDlg->objectName().isEmpty())
            AnalyzerDlg->setObjectName(QString::fromUtf8("AnalyzerDlg"));
        AnalyzerDlg->setEnabled(true);
        AnalyzerDlg->resize(1028, 720);
        AnalyzerDlg->setSizeIncrement(QSize(0, 0));
        AnalyzerDlg->setBaseSize(QSize(0, 0));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 227, 155, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(227, 227, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(99, 100, 127, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(133, 133, 170, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush6(QColor(228, 236, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush6);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        QBrush brush8(QColor(199, 200, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        AnalyzerDlg->setPalette(palette);
        AnalyzerDlg->setCursor(QCursor(Qt::PointingHandCursor));
        studiesTW = new QTabWidget(AnalyzerDlg);
        studiesTW->setObjectName(QString::fromUtf8("studiesTW"));
        studiesTW->setEnabled(false);
        studiesTW->setGeometry(QRect(0, 72, 1021, 641));
        QPalette palette1;
        QBrush brush9(QColor(170, 85, 0, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        studiesTW->setPalette(palette1);
        converterT = new QWidget();
        converterT->setObjectName(QString::fromUtf8("converterT"));
        runFilesManagementGB_2 = new QGroupBox(converterT);
        runFilesManagementGB_2->setObjectName(QString::fromUtf8("runFilesManagementGB_2"));
        runFilesManagementGB_2->setGeometry(QRect(4, 4, 1009, 529));
        QPalette palette2;
        QBrush brush10(QColor(255, 0, 0, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        runFilesManagementGB_2->setPalette(palette2);
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        runFilesManagementGB_2->setFont(font);
        runFilesManagementGB_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        convertFileListTV = new QTableView(runFilesManagementGB_2);
        convertFileListTV->setObjectName(QString::fromUtf8("convertFileListTV"));
        convertFileListTV->setGeometry(QRect(90, 16, 913, 505));
        convertFileListTV->setEditTriggers(QAbstractItemView::NoEditTriggers);
        convertFileListTV->horizontalHeader()->setMinimumSectionSize(200);
        convertFileListTV->verticalHeader()->setDefaultSectionSize(19);
        convertResetListPB = new QPushButton(runFilesManagementGB_2);
        convertResetListPB->setObjectName(QString::fromUtf8("convertResetListPB"));
        convertResetListPB->setGeometry(QRect(4, 280, 81, 31));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush10);
        QBrush brush11(QColor(255, 127, 127, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush11);
        QBrush brush12(QColor(255, 63, 63, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush12);
        QBrush brush13(QColor(127, 0, 0, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush13);
        QBrush brush14(QColor(170, 0, 0, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush14);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush10);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush12);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush13);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush14);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush13);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush12);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush13);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush14);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush13);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush13);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        convertResetListPB->setPalette(palette3);
        openRootFilePB = new QPushButton(runFilesManagementGB_2);
        openRootFilePB->setObjectName(QString::fromUtf8("openRootFilePB"));
        openRootFilePB->setGeometry(QRect(4, 16, 81, 46));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush15(QColor(0, 255, 0, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush15);
        QBrush brush16(QColor(127, 255, 127, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush16);
        QBrush brush17(QColor(63, 255, 63, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush17);
        QBrush brush18(QColor(0, 127, 0, 255));
        brush18.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush18);
        QBrush brush19(QColor(0, 170, 0, 255));
        brush19.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush19);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush15);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush16);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush17);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush18);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush19);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush15);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush16);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush17);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush19);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        openRootFilePB->setPalette(palette4);
        convertConverterControlsGB = new QGroupBox(converterT);
        convertConverterControlsGB->setObjectName(QString::fromUtf8("convertConverterControlsGB"));
        convertConverterControlsGB->setGeometry(QRect(4, 541, 1009, 69));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        convertConverterControlsGB->setPalette(palette5);
        convertConverterControlsGB->setFont(font);
        convertConverterControlsGB->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        convertMaxEventsSB = new customSpinBox(convertConverterControlsGB);
        convertMaxEventsSB->setObjectName(QString::fromUtf8("convertMaxEventsSB"));
        convertMaxEventsSB->setGeometry(QRect(408, 16, 81, 21));
        convertMaxEventsSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        convertMaxEventsSB->setMinimum(0);
        convertMaxEventsSB->setMaximum(1000000);
        convertMaxEventsSB->setValue(10000);
        convertTotalEventsLB = new QLabel(convertConverterControlsGB);
        convertTotalEventsLB->setObjectName(QString::fromUtf8("convertTotalEventsLB"));
        convertTotalEventsLB->setGeometry(QRect(96, 16, 97, 22));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        convertTotalEventsLB->setFont(font1);
        convertTotalEventsLB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        convertTotalEventsLE = new QLineEdit(convertConverterControlsGB);
        convertTotalEventsLE->setObjectName(QString::fromUtf8("convertTotalEventsLE"));
        convertTotalEventsLE->setGeometry(QRect(196, 16, 96, 21));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush20(QColor(0, 0, 127, 255));
        brush20.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush20);
        palette6.setBrush(QPalette::Active, QPalette::BrightText, brush20);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush20);
        palette6.setBrush(QPalette::Inactive, QPalette::BrightText, brush20);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush20);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush20);
        palette6.setBrush(QPalette::Disabled, QPalette::BrightText, brush20);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush20);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        convertTotalEventsLE->setPalette(palette6);
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(true);
        font2.setWeight(75);
        convertTotalEventsLE->setFont(font2);
        convertTotalEventsLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        convertTotalEventsLE->setReadOnly(true);
        runConverterPB = new QPushButton(convertConverterControlsGB);
        runConverterPB->setObjectName(QString::fromUtf8("runConverterPB"));
        runConverterPB->setEnabled(false);
        runConverterPB->setGeometry(QRect(8, 16, 81, 20));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush15);
        QBrush brush21(QColor(127, 170, 255, 255));
        brush21.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Light, brush21);
        QBrush brush22(QColor(63, 127, 255, 255));
        brush22.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Midlight, brush22);
        QBrush brush23(QColor(0, 42, 127, 255));
        brush23.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Dark, brush23);
        QBrush brush24(QColor(0, 56, 170, 255));
        brush24.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Mid, brush24);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush);
        palette7.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush19);
        palette7.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Active, QPalette::AlternateBase, brush21);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette7.setBrush(QPalette::Inactive, QPalette::Light, brush21);
        palette7.setBrush(QPalette::Inactive, QPalette::Midlight, brush22);
        palette7.setBrush(QPalette::Inactive, QPalette::Dark, brush23);
        palette7.setBrush(QPalette::Inactive, QPalette::Mid, brush24);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush19);
        palette7.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush21);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush23);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette7.setBrush(QPalette::Disabled, QPalette::Light, brush21);
        palette7.setBrush(QPalette::Disabled, QPalette::Midlight, brush22);
        palette7.setBrush(QPalette::Disabled, QPalette::Dark, brush23);
        palette7.setBrush(QPalette::Disabled, QPalette::Mid, brush24);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush23);
        palette7.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush23);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush19);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush19);
        palette7.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        QBrush brush25(QColor(0, 85, 255, 255));
        brush25.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush25);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        runConverterPB->setPalette(palette7);
        QFont font3;
        font3.setFamily(QString::fromUtf8("Sans Serif"));
        font3.setBold(true);
        font3.setWeight(75);
        runConverterPB->setFont(font3);
        convertMaxThreadsLB = new QLabel(convertConverterControlsGB);
        convertMaxThreadsLB->setObjectName(QString::fromUtf8("convertMaxThreadsLB"));
        convertMaxThreadsLB->setGeometry(QRect(511, 16, 96, 22));
        convertMaxThreadsLB->setFont(font1);
        convertMaxThreadsSB = new customSpinBox(convertConverterControlsGB);
        convertMaxThreadsSB->setObjectName(QString::fromUtf8("convertMaxThreadsSB"));
        convertMaxThreadsSB->setGeometry(QRect(609, 16, 81, 21));
        convertMaxThreadsSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        convertMaxThreadsSB->setMinimum(0);
        convertMaxThreadsSB->setMaximum(100);
        convertMaxThreadsSB->setValue(0);
        convertMaxEventsCB = new customCheckBox(convertConverterControlsGB);
        convertMaxEventsCB->setObjectName(QString::fromUtf8("convertMaxEventsCB"));
        convertMaxEventsCB->setGeometry(QRect(303, 16, 106, 21));
        convertMaxEventsCB->setChecked(true);
        studiesTW->addTab(converterT, QString());
        infosT = new QWidget();
        infosT->setObjectName(QString::fromUtf8("infosT"));
        detectorsInfoTW = new customTableWidget(infosT);
        if (detectorsInfoTW->columnCount() < 9)
            detectorsInfoTW->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        detectorsInfoTW->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        detectorsInfoTW->setObjectName(QString::fromUtf8("detectorsInfoTW"));
        detectorsInfoTW->setGeometry(QRect(5, 5, 1009, 605));
        detectorsInfoTW->verticalHeader()->setDefaultSectionSize(20);
        studiesTW->addTab(infosT, QString());
        calibrationT = new QWidget();
        calibrationT->setObjectName(QString::fromUtf8("calibrationT"));
        makeCalibPlotsPB = new QPushButton(calibrationT);
        makeCalibPlotsPB->setObjectName(QString::fromUtf8("makeCalibPlotsPB"));
        makeCalibPlotsPB->setGeometry(QRect(16, 180, 126, 20));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush15);
        QBrush brush26(QColor(130, 190, 255, 255));
        brush26.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Light, brush26);
        QBrush brush27(QColor(66, 157, 255, 255));
        brush27.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Midlight, brush27);
        QBrush brush28(QColor(1, 62, 127, 255));
        brush28.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Dark, brush28);
        QBrush brush29(QColor(1, 82, 170, 255));
        brush29.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Mid, brush29);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush);
        palette8.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush30(QColor(2, 124, 255, 255));
        brush30.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush30);
        palette8.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush31(QColor(128, 189, 255, 255));
        brush31.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::AlternateBase, brush31);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette8.setBrush(QPalette::Inactive, QPalette::Light, brush26);
        palette8.setBrush(QPalette::Inactive, QPalette::Midlight, brush27);
        palette8.setBrush(QPalette::Inactive, QPalette::Dark, brush28);
        palette8.setBrush(QPalette::Inactive, QPalette::Mid, brush29);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush30);
        palette8.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush31);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush28);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette8.setBrush(QPalette::Disabled, QPalette::Light, brush26);
        palette8.setBrush(QPalette::Disabled, QPalette::Midlight, brush27);
        palette8.setBrush(QPalette::Disabled, QPalette::Dark, brush28);
        palette8.setBrush(QPalette::Disabled, QPalette::Mid, brush29);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush28);
        palette8.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush28);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush30);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush30);
        palette8.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush30);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        makeCalibPlotsPB->setPalette(palette8);
        makeCalibPlotsPB->setFont(font);
        calibPlotsGB = new QGroupBox(calibrationT);
        calibPlotsGB->setObjectName(QString::fromUtf8("calibPlotsGB"));
        calibPlotsGB->setGeometry(QRect(4, 108, 151, 65));
        QFont font4;
        font4.setPointSize(9);
        calibPlotsGB->setFont(font4);
        calibParsCB = new QCheckBox(calibPlotsGB);
        calibParsCB->setObjectName(QString::fromUtf8("calibParsCB"));
        calibParsCB->setGeometry(QRect(8, 16, 101, 22));
        calibDispersionCB = new QCheckBox(calibPlotsGB);
        calibDispersionCB->setObjectName(QString::fromUtf8("calibDispersionCB"));
        calibDispersionCB->setGeometry(QRect(8, 36, 101, 22));
        dutsListLW = new QListWidget(calibrationT);
        dutsListLW->setObjectName(QString::fromUtf8("dutsListLW"));
        dutsListLW->setGeometry(QRect(164, 116, 845, 493));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush32(QColor(255, 0, 127, 255));
        brush32.setStyle(Qt::SolidPattern);
        palette9.setBrush(QPalette::Active, QPalette::Highlight, brush32);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::Highlight, brush32);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        QBrush brush33(QColor(123, 123, 123, 255));
        brush33.setStyle(Qt::SolidPattern);
        palette9.setBrush(QPalette::Disabled, QPalette::Highlight, brush33);
        dutsListLW->setPalette(palette9);
        dutsListLW->setFont(font4);
        groupBox_2 = new QGroupBox(calibrationT);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(4, 4, 1005, 97));
        calibPlotsFileLE = new QLineEdit(groupBox_2);
        calibPlotsFileLE->setObjectName(QString::fromUtf8("calibPlotsFileLE"));
        calibPlotsFileLE->setEnabled(false);
        calibPlotsFileLE->setGeometry(QRect(148, 64, 853, 20));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Active, QPalette::Text, brush);
        palette10.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette10.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        QBrush brush34(QColor(170, 0, 127, 255));
        brush34.setStyle(Qt::SolidPattern);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette10.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette10.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        calibPlotsFileLE->setPalette(palette10);
        QFont font5;
        font5.setPointSize(9);
        font5.setBold(false);
        font5.setWeight(50);
        calibPlotsFileLE->setFont(font5);
        calibPlotsFileLE->setReadOnly(true);
        loadedGeoFileLE_2 = new QLineEdit(groupBox_2);
        loadedGeoFileLE_2->setObjectName(QString::fromUtf8("loadedGeoFileLE_2"));
        loadedGeoFileLE_2->setEnabled(false);
        loadedGeoFileLE_2->setGeometry(QRect(148, 16, 853, 20));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Active, QPalette::Text, brush);
        palette11.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette11.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette11.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette11.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        loadedGeoFileLE_2->setPalette(palette11);
        loadedGeoFileLE_2->setFont(font5);
        loadedGeoFileLE_2->setReadOnly(true);
        openCalibrationPlotsPB = new QPushButton(groupBox_2);
        openCalibrationPlotsPB->setObjectName(QString::fromUtf8("openCalibrationPlotsPB"));
        openCalibrationPlotsPB->setGeometry(QRect(4, 64, 142, 20));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush35(QColor(255, 137, 1, 255));
        brush35.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Button, brush35);
        palette12.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush36(QColor(232, 203, 255, 255));
        brush36.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Midlight, brush36);
        QBrush brush37(QColor(104, 75, 127, 255));
        brush37.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Dark, brush37);
        QBrush brush38(QColor(139, 101, 170, 255));
        brush38.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Mid, brush38);
        palette12.setBrush(QPalette::Active, QPalette::Text, brush);
        palette12.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette12.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush39(QColor(209, 151, 255, 255));
        brush39.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Window, brush39);
        palette12.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette12.setBrush(QPalette::Active, QPalette::AlternateBase, brush36);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Button, brush35);
        palette12.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::Midlight, brush36);
        palette12.setBrush(QPalette::Inactive, QPalette::Dark, brush37);
        palette12.setBrush(QPalette::Inactive, QPalette::Mid, brush38);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::Window, brush39);
        palette12.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush36);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush37);
        palette12.setBrush(QPalette::Disabled, QPalette::Button, brush35);
        palette12.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::Midlight, brush36);
        palette12.setBrush(QPalette::Disabled, QPalette::Dark, brush37);
        palette12.setBrush(QPalette::Disabled, QPalette::Mid, brush38);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush37);
        palette12.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::ButtonText, brush37);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush39);
        palette12.setBrush(QPalette::Disabled, QPalette::Window, brush39);
        palette12.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush39);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        openCalibrationPlotsPB->setPalette(palette12);
        openCalibrationPlotsPB->setFont(font4);
        loadedCalibFileLE = new QLineEdit(groupBox_2);
        loadedCalibFileLE->setObjectName(QString::fromUtf8("loadedCalibFileLE"));
        loadedCalibFileLE->setEnabled(false);
        loadedCalibFileLE->setGeometry(QRect(148, 40, 853, 20));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Active, QPalette::Text, brush);
        palette13.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette13.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette13.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette13.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        loadedCalibFileLE->setPalette(palette13);
        loadedCalibFileLE->setFont(font5);
        loadedCalibFileLE->setReadOnly(true);
        openCalibrationsFilePB = new QPushButton(groupBox_2);
        openCalibrationsFilePB->setObjectName(QString::fromUtf8("openCalibrationsFilePB"));
        openCalibrationsFilePB->setGeometry(QRect(4, 40, 142, 20));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette14.setBrush(QPalette::Active, QPalette::Light, brush16);
        palette14.setBrush(QPalette::Active, QPalette::Midlight, brush17);
        palette14.setBrush(QPalette::Active, QPalette::Dark, brush18);
        palette14.setBrush(QPalette::Active, QPalette::Mid, brush19);
        palette14.setBrush(QPalette::Active, QPalette::Text, brush);
        palette14.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette14.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette14.setBrush(QPalette::Active, QPalette::Window, brush15);
        palette14.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette14.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette14.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette14.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette14.setBrush(QPalette::Inactive, QPalette::Light, brush16);
        palette14.setBrush(QPalette::Inactive, QPalette::Midlight, brush17);
        palette14.setBrush(QPalette::Inactive, QPalette::Dark, brush18);
        palette14.setBrush(QPalette::Inactive, QPalette::Mid, brush19);
        palette14.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::Window, brush15);
        palette14.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette14.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette14.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette14.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette14.setBrush(QPalette::Disabled, QPalette::Light, brush16);
        palette14.setBrush(QPalette::Disabled, QPalette::Midlight, brush17);
        palette14.setBrush(QPalette::Disabled, QPalette::Dark, brush18);
        palette14.setBrush(QPalette::Disabled, QPalette::Mid, brush19);
        palette14.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette14.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush15);
        palette14.setBrush(QPalette::Disabled, QPalette::Window, brush15);
        palette14.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush15);
        palette14.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette14.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        openCalibrationsFilePB->setPalette(palette14);
        openCalibrationsFilePB->setFont(font4);
        calibrationLoadGeoFilePB = new QPushButton(groupBox_2);
        calibrationLoadGeoFilePB->setObjectName(QString::fromUtf8("calibrationLoadGeoFilePB"));
        calibrationLoadGeoFilePB->setGeometry(QRect(4, 16, 142, 20));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette15.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette15.setBrush(QPalette::Active, QPalette::Light, brush16);
        palette15.setBrush(QPalette::Active, QPalette::Midlight, brush17);
        palette15.setBrush(QPalette::Active, QPalette::Dark, brush18);
        palette15.setBrush(QPalette::Active, QPalette::Mid, brush19);
        palette15.setBrush(QPalette::Active, QPalette::Text, brush);
        palette15.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette15.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette15.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette15.setBrush(QPalette::Active, QPalette::Window, brush15);
        palette15.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette15.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette15.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette15.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette15.setBrush(QPalette::Inactive, QPalette::Light, brush16);
        palette15.setBrush(QPalette::Inactive, QPalette::Midlight, brush17);
        palette15.setBrush(QPalette::Inactive, QPalette::Dark, brush18);
        palette15.setBrush(QPalette::Inactive, QPalette::Mid, brush19);
        palette15.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::Window, brush15);
        palette15.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette15.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette15.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette15.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette15.setBrush(QPalette::Disabled, QPalette::Light, brush16);
        palette15.setBrush(QPalette::Disabled, QPalette::Midlight, brush17);
        palette15.setBrush(QPalette::Disabled, QPalette::Dark, brush18);
        palette15.setBrush(QPalette::Disabled, QPalette::Mid, brush19);
        palette15.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette15.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette15.setBrush(QPalette::Disabled, QPalette::Base, brush15);
        palette15.setBrush(QPalette::Disabled, QPalette::Window, brush15);
        palette15.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette15.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush15);
        palette15.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette15.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        calibrationLoadGeoFilePB->setPalette(palette15);
        calibrationLoadGeoFilePB->setFont(font4);
        studiesTW->addTab(calibrationT, QString());
        windowsT = new QWidget();
        windowsT->setObjectName(QString::fromUtf8("windowsT"));
        limitWindowLB = new QLabel(windowsT);
        limitWindowLB->setObjectName(QString::fromUtf8("limitWindowLB"));
        limitWindowLB->setGeometry(QRect(4, 4, 53, 22));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Sans Serif"));
        font6.setPointSize(9);
        font6.setBold(true);
        font6.setWeight(75);
        limitWindowLB->setFont(font6);
        maxWindowEventsLE = new customLineEdit(windowsT);
        maxWindowEventsLE->setObjectName(QString::fromUtf8("maxWindowEventsLE"));
        maxWindowEventsLE->setGeometry(QRect(64, 4, 91, 22));
        maxWindowEventsLE->setFont(font4);
        dutsPreAnalysisTW = new QTabWidget(windowsT);
        dutsPreAnalysisTW->setObjectName(QString::fromUtf8("dutsPreAnalysisTW"));
        dutsPreAnalysisTW->setGeometry(QRect(4, 28, 357, 125));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush40(QColor(170, 170, 255, 255));
        brush40.setStyle(Qt::SolidPattern);
        palette16.setBrush(QPalette::Active, QPalette::Button, brush40);
        palette16.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush41(QColor(212, 212, 255, 255));
        brush41.setStyle(Qt::SolidPattern);
        palette16.setBrush(QPalette::Active, QPalette::Midlight, brush41);
        QBrush brush42(QColor(85, 85, 127, 255));
        brush42.setStyle(Qt::SolidPattern);
        palette16.setBrush(QPalette::Active, QPalette::Dark, brush42);
        QBrush brush43(QColor(113, 113, 170, 255));
        brush43.setStyle(Qt::SolidPattern);
        palette16.setBrush(QPalette::Active, QPalette::Mid, brush43);
        palette16.setBrush(QPalette::Active, QPalette::Text, brush);
        palette16.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette16.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette16.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette16.setBrush(QPalette::Active, QPalette::Window, brush40);
        palette16.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette16.setBrush(QPalette::Active, QPalette::AlternateBase, brush41);
        palette16.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette16.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Button, brush40);
        palette16.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette16.setBrush(QPalette::Inactive, QPalette::Midlight, brush41);
        palette16.setBrush(QPalette::Inactive, QPalette::Dark, brush42);
        palette16.setBrush(QPalette::Inactive, QPalette::Mid, brush43);
        palette16.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette16.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette16.setBrush(QPalette::Inactive, QPalette::Window, brush40);
        palette16.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush41);
        palette16.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette16.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush42);
        palette16.setBrush(QPalette::Disabled, QPalette::Button, brush40);
        palette16.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::Midlight, brush41);
        palette16.setBrush(QPalette::Disabled, QPalette::Dark, brush42);
        palette16.setBrush(QPalette::Disabled, QPalette::Mid, brush43);
        palette16.setBrush(QPalette::Disabled, QPalette::Text, brush42);
        palette16.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::ButtonText, brush42);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush40);
        palette16.setBrush(QPalette::Disabled, QPalette::Window, brush40);
        palette16.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush40);
        palette16.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette16.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        dutsPreAnalysisTW->setPalette(palette16);
        dutsPreAnalysisTW->setFont(font4);
        dutsPreAnalysisTW->setCursor(QCursor(Qt::ArrowCursor));
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        removePixelDUT0GB = new QGroupBox(tab_6);
        removePixelDUT0GB->setObjectName(QString::fromUtf8("removePixelDUT0GB"));
        removePixelDUT0GB->setGeometry(QRect(176, 4, 173, 89));
        removeRowDUT0LB = new QLabel(removePixelDUT0GB);
        removeRowDUT0LB->setObjectName(QString::fromUtf8("removeRowDUT0LB"));
        removeRowDUT0LB->setGeometry(QRect(8, 33, 36, 22));
        removeRowDUT0LB->setFont(font3);
        removeRowDUT0LE = new QLineEdit(removePixelDUT0GB);
        removeRowDUT0LE->setObjectName(QString::fromUtf8("removeRowDUT0LE"));
        removeRowDUT0LE->setGeometry(QRect(44, 36, 55, 22));
        removeColDUT0LB = new QLabel(removePixelDUT0GB);
        removeColDUT0LB->setObjectName(QString::fromUtf8("removeColDUT0LB"));
        removeColDUT0LB->setGeometry(QRect(8, 60, 36, 22));
        removeColDUT0LB->setFont(font3);
        removeColDUT0LE = new QLineEdit(removePixelDUT0GB);
        removeColDUT0LE->setObjectName(QString::fromUtf8("removeColDUT0LE"));
        removeColDUT0LE->setGeometry(QRect(44, 60, 55, 22));
        removePxlDUT0PB = new QPushButton(removePixelDUT0GB);
        removePxlDUT0PB->setObjectName(QString::fromUtf8("removePxlDUT0PB"));
        removePxlDUT0PB->setGeometry(QRect(104, 60, 61, 22));
        resetRemovedPxlsDUT0PB = new QPushButton(removePixelDUT0GB);
        resetRemovedPxlsDUT0PB->setObjectName(QString::fromUtf8("resetRemovedPxlsDUT0PB"));
        resetRemovedPxlsDUT0PB->setGeometry(QRect(104, 36, 61, 22));
        groupBox_3 = new QGroupBox(tab_6);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(0, 4, 173, 89));
        higherColDUT0LE = new customLineEdit(groupBox_3);
        higherColDUT0LE->setObjectName(QString::fromUtf8("higherColDUT0LE"));
        higherColDUT0LE->setGeometry(QRect(104, 60, 55, 22));
        row0LB = new QLabel(groupBox_3);
        row0LB->setObjectName(QString::fromUtf8("row0LB"));
        row0LB->setGeometry(QRect(4, 36, 36, 22));
        row0LB->setFont(font3);
        lowerColDUT0LE = new customLineEdit(groupBox_3);
        lowerColDUT0LE->setObjectName(QString::fromUtf8("lowerColDUT0LE"));
        lowerColDUT0LE->setGeometry(QRect(40, 60, 55, 22));
        col0LB = new QLabel(groupBox_3);
        col0LB->setObjectName(QString::fromUtf8("col0LB"));
        col0LB->setGeometry(QRect(4, 60, 36, 22));
        col0LB->setFont(font3);
        lowerRowDUT0LE = new customLineEdit(groupBox_3);
        lowerRowDUT0LE->setObjectName(QString::fromUtf8("lowerRowDUT0LE"));
        lowerRowDUT0LE->setGeometry(QRect(40, 36, 55, 22));
        higherEdge0LB = new QLabel(groupBox_3);
        higherEdge0LB->setObjectName(QString::fromUtf8("higherEdge0LB"));
        higherEdge0LB->setGeometry(QRect(112, 16, 53, 22));
        higherEdge0LB->setFont(font1);
        lowerEdge0LB = new QLabel(groupBox_3);
        lowerEdge0LB->setObjectName(QString::fromUtf8("lowerEdge0LB"));
        lowerEdge0LB->setGeometry(QRect(44, 16, 53, 22));
        lowerEdge0LB->setFont(font1);
        higherRowDUT0LE = new customLineEdit(groupBox_3);
        higherRowDUT0LE->setObjectName(QString::fromUtf8("higherRowDUT0LE"));
        higherRowDUT0LE->setGeometry(QRect(104, 36, 55, 22));
        dutsPreAnalysisTW->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        removePixelDUT0GB_2 = new QGroupBox(tab_7);
        removePixelDUT0GB_2->setObjectName(QString::fromUtf8("removePixelDUT0GB_2"));
        removePixelDUT0GB_2->setGeometry(QRect(176, 4, 173, 89));
        removeRowDUT0LB_2 = new QLabel(removePixelDUT0GB_2);
        removeRowDUT0LB_2->setObjectName(QString::fromUtf8("removeRowDUT0LB_2"));
        removeRowDUT0LB_2->setGeometry(QRect(8, 33, 36, 22));
        removeRowDUT0LB_2->setFont(font3);
        removeRowDUT1LE = new QLineEdit(removePixelDUT0GB_2);
        removeRowDUT1LE->setObjectName(QString::fromUtf8("removeRowDUT1LE"));
        removeRowDUT1LE->setGeometry(QRect(44, 36, 55, 22));
        removeColDUT0LB_2 = new QLabel(removePixelDUT0GB_2);
        removeColDUT0LB_2->setObjectName(QString::fromUtf8("removeColDUT0LB_2"));
        removeColDUT0LB_2->setGeometry(QRect(8, 60, 36, 22));
        removeColDUT0LB_2->setFont(font3);
        removeColDUT1LE = new QLineEdit(removePixelDUT0GB_2);
        removeColDUT1LE->setObjectName(QString::fromUtf8("removeColDUT1LE"));
        removeColDUT1LE->setGeometry(QRect(44, 60, 55, 22));
        removePxlDUT1PB = new QPushButton(removePixelDUT0GB_2);
        removePxlDUT1PB->setObjectName(QString::fromUtf8("removePxlDUT1PB"));
        removePxlDUT1PB->setGeometry(QRect(104, 60, 61, 22));
        resetRemovedPxlsDUT1PB = new QPushButton(removePixelDUT0GB_2);
        resetRemovedPxlsDUT1PB->setObjectName(QString::fromUtf8("resetRemovedPxlsDUT1PB"));
        resetRemovedPxlsDUT1PB->setGeometry(QRect(104, 36, 61, 22));
        groupBox_4 = new QGroupBox(tab_7);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(0, 4, 173, 89));
        higherColDUT1LE = new customLineEdit(groupBox_4);
        higherColDUT1LE->setObjectName(QString::fromUtf8("higherColDUT1LE"));
        higherColDUT1LE->setGeometry(QRect(104, 60, 55, 22));
        row0LB_2 = new QLabel(groupBox_4);
        row0LB_2->setObjectName(QString::fromUtf8("row0LB_2"));
        row0LB_2->setGeometry(QRect(4, 36, 36, 22));
        row0LB_2->setFont(font3);
        lowerColDUT1LE = new customLineEdit(groupBox_4);
        lowerColDUT1LE->setObjectName(QString::fromUtf8("lowerColDUT1LE"));
        lowerColDUT1LE->setGeometry(QRect(40, 60, 55, 22));
        col0LB_2 = new QLabel(groupBox_4);
        col0LB_2->setObjectName(QString::fromUtf8("col0LB_2"));
        col0LB_2->setGeometry(QRect(4, 60, 36, 22));
        col0LB_2->setFont(font3);
        lowerRowDUT1LE = new customLineEdit(groupBox_4);
        lowerRowDUT1LE->setObjectName(QString::fromUtf8("lowerRowDUT1LE"));
        lowerRowDUT1LE->setGeometry(QRect(40, 36, 55, 22));
        higherEdge0LB_2 = new QLabel(groupBox_4);
        higherEdge0LB_2->setObjectName(QString::fromUtf8("higherEdge0LB_2"));
        higherEdge0LB_2->setGeometry(QRect(112, 16, 53, 22));
        higherEdge0LB_2->setFont(font1);
        lowerEdge0LB_2 = new QLabel(groupBox_4);
        lowerEdge0LB_2->setObjectName(QString::fromUtf8("lowerEdge0LB_2"));
        lowerEdge0LB_2->setGeometry(QRect(44, 16, 53, 22));
        lowerEdge0LB_2->setFont(font1);
        higherRowDUT1LE = new customLineEdit(groupBox_4);
        higherRowDUT1LE->setObjectName(QString::fromUtf8("higherRowDUT1LE"));
        higherRowDUT1LE->setGeometry(QRect(104, 36, 55, 22));
        dutsPreAnalysisTW->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        removePixelDUT0GB_3 = new QGroupBox(tab_8);
        removePixelDUT0GB_3->setObjectName(QString::fromUtf8("removePixelDUT0GB_3"));
        removePixelDUT0GB_3->setGeometry(QRect(176, 4, 173, 89));
        removeRowDUT0LB_3 = new QLabel(removePixelDUT0GB_3);
        removeRowDUT0LB_3->setObjectName(QString::fromUtf8("removeRowDUT0LB_3"));
        removeRowDUT0LB_3->setGeometry(QRect(8, 33, 36, 22));
        removeRowDUT0LB_3->setFont(font3);
        removeRowDUT2LE = new QLineEdit(removePixelDUT0GB_3);
        removeRowDUT2LE->setObjectName(QString::fromUtf8("removeRowDUT2LE"));
        removeRowDUT2LE->setGeometry(QRect(44, 36, 55, 22));
        removeColDUT0LB_3 = new QLabel(removePixelDUT0GB_3);
        removeColDUT0LB_3->setObjectName(QString::fromUtf8("removeColDUT0LB_3"));
        removeColDUT0LB_3->setGeometry(QRect(8, 60, 36, 22));
        removeColDUT0LB_3->setFont(font3);
        removeColDUT2LE = new QLineEdit(removePixelDUT0GB_3);
        removeColDUT2LE->setObjectName(QString::fromUtf8("removeColDUT2LE"));
        removeColDUT2LE->setGeometry(QRect(44, 60, 55, 22));
        removePxlDUT2PB = new QPushButton(removePixelDUT0GB_3);
        removePxlDUT2PB->setObjectName(QString::fromUtf8("removePxlDUT2PB"));
        removePxlDUT2PB->setGeometry(QRect(104, 60, 61, 22));
        resetRemovedPxlsDUT2PB = new QPushButton(removePixelDUT0GB_3);
        resetRemovedPxlsDUT2PB->setObjectName(QString::fromUtf8("resetRemovedPxlsDUT2PB"));
        resetRemovedPxlsDUT2PB->setGeometry(QRect(104, 36, 61, 22));
        groupBox_5 = new QGroupBox(tab_8);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(0, 4, 173, 89));
        higherColDUT2LE = new customLineEdit(groupBox_5);
        higherColDUT2LE->setObjectName(QString::fromUtf8("higherColDUT2LE"));
        higherColDUT2LE->setGeometry(QRect(104, 60, 55, 22));
        row0LB_3 = new QLabel(groupBox_5);
        row0LB_3->setObjectName(QString::fromUtf8("row0LB_3"));
        row0LB_3->setGeometry(QRect(4, 36, 36, 22));
        row0LB_3->setFont(font3);
        lowerColDUT2LE = new customLineEdit(groupBox_5);
        lowerColDUT2LE->setObjectName(QString::fromUtf8("lowerColDUT2LE"));
        lowerColDUT2LE->setGeometry(QRect(40, 60, 55, 22));
        col0LB_3 = new QLabel(groupBox_5);
        col0LB_3->setObjectName(QString::fromUtf8("col0LB_3"));
        col0LB_3->setGeometry(QRect(4, 60, 36, 22));
        col0LB_3->setFont(font3);
        lowerRowDUT2LE = new customLineEdit(groupBox_5);
        lowerRowDUT2LE->setObjectName(QString::fromUtf8("lowerRowDUT2LE"));
        lowerRowDUT2LE->setGeometry(QRect(40, 36, 55, 22));
        higherEdge0LB_3 = new QLabel(groupBox_5);
        higherEdge0LB_3->setObjectName(QString::fromUtf8("higherEdge0LB_3"));
        higherEdge0LB_3->setGeometry(QRect(112, 16, 53, 22));
        higherEdge0LB_3->setFont(font1);
        lowerEdge0LB_3 = new QLabel(groupBox_5);
        lowerEdge0LB_3->setObjectName(QString::fromUtf8("lowerEdge0LB_3"));
        lowerEdge0LB_3->setGeometry(QRect(44, 16, 53, 22));
        lowerEdge0LB_3->setFont(font1);
        higherRowDUT2LE = new customLineEdit(groupBox_5);
        higherRowDUT2LE->setObjectName(QString::fromUtf8("higherRowDUT2LE"));
        higherRowDUT2LE->setGeometry(QRect(104, 36, 55, 22));
        dutsPreAnalysisTW->addTab(tab_8, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QString::fromUtf8("tab_9"));
        groupBox_6 = new QGroupBox(tab_9);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(0, 4, 173, 89));
        higherColDUT3LE = new customLineEdit(groupBox_6);
        higherColDUT3LE->setObjectName(QString::fromUtf8("higherColDUT3LE"));
        higherColDUT3LE->setGeometry(QRect(104, 60, 55, 22));
        row0LB_4 = new QLabel(groupBox_6);
        row0LB_4->setObjectName(QString::fromUtf8("row0LB_4"));
        row0LB_4->setGeometry(QRect(4, 36, 36, 22));
        row0LB_4->setFont(font3);
        lowerColDUT3LE = new customLineEdit(groupBox_6);
        lowerColDUT3LE->setObjectName(QString::fromUtf8("lowerColDUT3LE"));
        lowerColDUT3LE->setGeometry(QRect(40, 60, 55, 22));
        col0LB_4 = new QLabel(groupBox_6);
        col0LB_4->setObjectName(QString::fromUtf8("col0LB_4"));
        col0LB_4->setGeometry(QRect(4, 60, 36, 22));
        col0LB_4->setFont(font3);
        lowerRowDUT3LE = new customLineEdit(groupBox_6);
        lowerRowDUT3LE->setObjectName(QString::fromUtf8("lowerRowDUT3LE"));
        lowerRowDUT3LE->setGeometry(QRect(40, 36, 55, 22));
        higherEdge0LB_4 = new QLabel(groupBox_6);
        higherEdge0LB_4->setObjectName(QString::fromUtf8("higherEdge0LB_4"));
        higherEdge0LB_4->setGeometry(QRect(112, 16, 53, 22));
        higherEdge0LB_4->setFont(font1);
        lowerEdge0LB_4 = new QLabel(groupBox_6);
        lowerEdge0LB_4->setObjectName(QString::fromUtf8("lowerEdge0LB_4"));
        lowerEdge0LB_4->setGeometry(QRect(44, 16, 53, 22));
        lowerEdge0LB_4->setFont(font1);
        higherRowDUT3LE = new customLineEdit(groupBox_6);
        higherRowDUT3LE->setObjectName(QString::fromUtf8("higherRowDUT3LE"));
        higherRowDUT3LE->setGeometry(QRect(104, 36, 55, 22));
        removePixelDUT0GB_4 = new QGroupBox(tab_9);
        removePixelDUT0GB_4->setObjectName(QString::fromUtf8("removePixelDUT0GB_4"));
        removePixelDUT0GB_4->setGeometry(QRect(176, 4, 173, 89));
        removeRowDUT0LB_4 = new QLabel(removePixelDUT0GB_4);
        removeRowDUT0LB_4->setObjectName(QString::fromUtf8("removeRowDUT0LB_4"));
        removeRowDUT0LB_4->setGeometry(QRect(8, 33, 36, 22));
        removeRowDUT0LB_4->setFont(font3);
        removeRowDUT3LE = new QLineEdit(removePixelDUT0GB_4);
        removeRowDUT3LE->setObjectName(QString::fromUtf8("removeRowDUT3LE"));
        removeRowDUT3LE->setGeometry(QRect(44, 36, 55, 22));
        removeColDUT0LB_4 = new QLabel(removePixelDUT0GB_4);
        removeColDUT0LB_4->setObjectName(QString::fromUtf8("removeColDUT0LB_4"));
        removeColDUT0LB_4->setGeometry(QRect(8, 60, 36, 22));
        removeColDUT0LB_4->setFont(font3);
        removeColDUT3LE = new QLineEdit(removePixelDUT0GB_4);
        removeColDUT3LE->setObjectName(QString::fromUtf8("removeColDUT3LE"));
        removeColDUT3LE->setGeometry(QRect(44, 60, 55, 22));
        removePxlDUT3PB = new QPushButton(removePixelDUT0GB_4);
        removePxlDUT3PB->setObjectName(QString::fromUtf8("removePxlDUT3PB"));
        removePxlDUT3PB->setGeometry(QRect(104, 60, 61, 22));
        resetRemovedPxlsDUT3PB = new QPushButton(removePixelDUT0GB_4);
        resetRemovedPxlsDUT3PB->setObjectName(QString::fromUtf8("resetRemovedPxlsDUT3PB"));
        resetRemovedPxlsDUT3PB->setGeometry(QRect(104, 36, 61, 22));
        dutsPreAnalysisTW->addTab(tab_9, QString());
        studiesTW->addTab(windowsT, QString());
        efficiencyT = new QWidget();
        efficiencyT->setObjectName(QString::fromUtf8("efficiencyT"));
        efficiencyCutsGB = new QGroupBox(efficiencyT);
        efficiencyCutsGB->setObjectName(QString::fromUtf8("efficiencyCutsGB"));
        efficiencyCutsGB->setGeometry(QRect(776, 4, 237, 221));
        chi2EfficiencyCutCB = new QCheckBox(efficiencyCutsGB);
        chi2EfficiencyCutCB->setObjectName(QString::fromUtf8("chi2EfficiencyCutCB"));
        chi2EfficiencyCutCB->setGeometry(QRect(4, 16, 93, 22));
        chi2EfficiencyCutSB = new QDoubleSpinBox(efficiencyCutsGB);
        chi2EfficiencyCutSB->setObjectName(QString::fromUtf8("chi2EfficiencyCutSB"));
        chi2EfficiencyCutSB->setGeometry(QRect(145, 16, 62, 20));
        chi2EfficiencyCutSB->setDecimals(1);
        chi2EfficiencyCutSB->setMinimum(1);
        chi2EfficiencyCutSB->setMaximum(20);
        chi2EfficiencyCutSB->setSingleStep(0.5);
        chi2EfficiencyCutSB->setValue(2);
        nTrackEfficiencyCutCB = new QCheckBox(efficiencyCutsGB);
        nTrackEfficiencyCutCB->setObjectName(QString::fromUtf8("nTrackEfficiencyCutCB"));
        nTrackEfficiencyCutCB->setGeometry(QRect(4, 36, 131, 22));
        nTrackEfficiencyCutSB = new QSpinBox(efficiencyCutsGB);
        nTrackEfficiencyCutSB->setObjectName(QString::fromUtf8("nTrackEfficiencyCutSB"));
        nTrackEfficiencyCutSB->setGeometry(QRect(145, 36, 62, 20));
        nTrackEfficiencyCutSB->setLayoutDirection(Qt::LeftToRight);
        nTrackEfficiencyCutSB->setMinimum(1);
        nTrackEfficiencyCutSB->setMaximum(10);
        nTrackEfficiencyCutSB->setValue(1);
        yErrEfficiencyCutCB = new QCheckBox(efficiencyCutsGB);
        yErrEfficiencyCutCB->setObjectName(QString::fromUtf8("yErrEfficiencyCutCB"));
        yErrEfficiencyCutCB->setGeometry(QRect(4, 76, 141, 22));
        yErrEfficiencyCutSB = new QDoubleSpinBox(efficiencyCutsGB);
        yErrEfficiencyCutSB->setObjectName(QString::fromUtf8("yErrEfficiencyCutSB"));
        yErrEfficiencyCutSB->setGeometry(QRect(145, 76, 62, 20));
        yErrEfficiencyCutSB->setDecimals(1);
        yErrEfficiencyCutSB->setMinimum(0);
        yErrEfficiencyCutSB->setMaximum(50);
        yErrEfficiencyCutSB->setSingleStep(0.5);
        yErrEfficiencyCutSB->setValue(6);
        xErrEffLB = new QLabel(efficiencyCutsGB);
        xErrEffLB->setObjectName(QString::fromUtf8("xErrEffLB"));
        xErrEffLB->setEnabled(false);
        xErrEffLB->setGeometry(QRect(211, 112, 31, 22));
        QFont font7;
        font7.setFamily(QString::fromUtf8("Sans Serif"));
        font7.setBold(false);
        font7.setWeight(50);
        xErrEffLB->setFont(font7);
        xErrEfficiencyCutCB = new QCheckBox(efficiencyCutsGB);
        xErrEfficiencyCutCB->setObjectName(QString::fromUtf8("xErrEfficiencyCutCB"));
        xErrEfficiencyCutCB->setGeometry(QRect(4, 56, 141, 22));
        xErrEfficiencyCutSB = new QDoubleSpinBox(efficiencyCutsGB);
        xErrEfficiencyCutSB->setObjectName(QString::fromUtf8("xErrEfficiencyCutSB"));
        xErrEfficiencyCutSB->setGeometry(QRect(145, 56, 62, 20));
        xErrEfficiencyCutSB->setDecimals(1);
        xErrEfficiencyCutSB->setMinimum(0);
        xErrEfficiencyCutSB->setMaximum(50);
        xErrEfficiencyCutSB->setSingleStep(0.5);
        xErrEfficiencyCutSB->setValue(6);
        xErrEffCutLB = new QLabel(efficiencyCutsGB);
        xErrEffCutLB->setObjectName(QString::fromUtf8("xErrEffCutLB"));
        xErrEffCutLB->setEnabled(false);
        xErrEffCutLB->setGeometry(QRect(211, 96, 31, 22));
        xErrEffCutLB->setFont(font7);
        xEfficiencyCutCB = new QCheckBox(efficiencyCutsGB);
        xEfficiencyCutCB->setObjectName(QString::fromUtf8("xEfficiencyCutCB"));
        xEfficiencyCutCB->setGeometry(QRect(4, 96, 130, 22));
        yEfficiencyCutSB = new QDoubleSpinBox(efficiencyCutsGB);
        yEfficiencyCutSB->setObjectName(QString::fromUtf8("yEfficiencyCutSB"));
        yEfficiencyCutSB->setGeometry(QRect(145, 116, 62, 20));
        yEfficiencyCutSB->setDecimals(1);
        yEfficiencyCutSB->setMinimum(-50);
        yEfficiencyCutSB->setMaximum(50);
        yEfficiencyCutSB->setSingleStep(0.5);
        yEfficiencyCutSB->setValue(30);
        xEfficiencyCutSB = new QDoubleSpinBox(efficiencyCutsGB);
        xEfficiencyCutSB->setObjectName(QString::fromUtf8("xEfficiencyCutSB"));
        xEfficiencyCutSB->setGeometry(QRect(145, 96, 62, 20));
        xEfficiencyCutSB->setDecimals(1);
        xEfficiencyCutSB->setMinimum(-75);
        xEfficiencyCutSB->setMaximum(75);
        xEfficiencyCutSB->setSingleStep(0.5);
        xEfficiencyCutSB->setValue(55);
        yEfficiencyCutCB = new QCheckBox(efficiencyCutsGB);
        yEfficiencyCutCB->setObjectName(QString::fromUtf8("yEfficiencyCutCB"));
        yEfficiencyCutCB->setGeometry(QRect(4, 116, 131, 22));
        hitsEfficiencyCutCB = new QCheckBox(efficiencyCutsGB);
        hitsEfficiencyCutCB->setObjectName(QString::fromUtf8("hitsEfficiencyCutCB"));
        hitsEfficiencyCutCB->setGeometry(QRect(4, 136, 131, 22));
        hitsEfficiencyCutSB = new customSpinBox(efficiencyCutsGB);
        hitsEfficiencyCutSB->setObjectName(QString::fromUtf8("hitsEfficiencyCutSB"));
        hitsEfficiencyCutSB->setGeometry(QRect(145, 136, 62, 20));
        hitsEfficiencyCutSB->setLayoutDirection(Qt::LeftToRight);
        hitsEfficiencyCutSB->setMinimum(1);
        hitsEfficiencyCutSB->setMaximum(50);
        hitsEfficiencyCutSB->setValue(1);
        label_5 = new QLabel(efficiencyCutsGB);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(140, 160, 81, 16));
        efficiencyStandardCutsCB = new customCheckBox(efficiencyCutsGB);
        efficiencyStandardCutsCB->setObjectName(QString::fromUtf8("efficiencyStandardCutsCB"));
        efficiencyStandardCutsCB->setGeometry(QRect(4, 176, 131, 22));
        efficiencyExcludeBadPlanesCB = new customCheckBox(efficiencyCutsGB);
        efficiencyExcludeBadPlanesCB->setObjectName(QString::fromUtf8("efficiencyExcludeBadPlanesCB"));
        efficiencyExcludeBadPlanesCB->setGeometry(QRect(4, 196, 231, 20));
        groupBox_7 = new QGroupBox(efficiencyT);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(4, 4, 765, 221));
        groupBox_7->setFont(font5);
        efficiencyCuts4LE = new customLineEdit(groupBox_7);
        efficiencyCuts4LE->setObjectName(QString::fromUtf8("efficiencyCuts4LE"));
        efficiencyCuts4LE->setEnabled(false);
        efficiencyCuts4LE->setGeometry(QRect(132, 96, 620, 22));
        QPalette palette17;
        QBrush brush44(QColor(0, 0, 255, 255));
        brush44.setStyle(Qt::SolidPattern);
        palette17.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette17.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        QBrush brush45(QColor(118, 118, 117, 255));
        brush45.setStyle(Qt::SolidPattern);
        palette17.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        efficiencyCuts4LE->setPalette(palette17);
        efficiencyCuts4LE->setFont(font3);
        efficiencyCuts2LE = new customLineEdit(groupBox_7);
        efficiencyCuts2LE->setObjectName(QString::fromUtf8("efficiencyCuts2LE"));
        efficiencyCuts2LE->setEnabled(false);
        efficiencyCuts2LE->setGeometry(QRect(132, 44, 620, 22));
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette18.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette18.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        efficiencyCuts2LE->setPalette(palette18);
        efficiencyCuts2LE->setFont(font3);
        efficiencyCuts2CB = new QCheckBox(groupBox_7);
        efficiencyCuts2CB->setObjectName(QString::fromUtf8("efficiencyCuts2CB"));
        efficiencyCuts2CB->setGeometry(QRect(4, 44, 131, 22));
        efficiencyCuts2CB->setFont(font7);
        efficiencyCuts3CB = new QCheckBox(groupBox_7);
        efficiencyCuts3CB->setObjectName(QString::fromUtf8("efficiencyCuts3CB"));
        efficiencyCuts3CB->setGeometry(QRect(4, 68, 141, 22));
        efficiencyCuts3CB->setFont(font7);
        efficiencyCuts4CB = new QCheckBox(groupBox_7);
        efficiencyCuts4CB->setObjectName(QString::fromUtf8("efficiencyCuts4CB"));
        efficiencyCuts4CB->setGeometry(QRect(4, 96, 141, 22));
        efficiencyCuts4CB->setFont(font7);
        efficiencyCuts3LE = new customLineEdit(groupBox_7);
        efficiencyCuts3LE->setObjectName(QString::fromUtf8("efficiencyCuts3LE"));
        efficiencyCuts3LE->setEnabled(false);
        efficiencyCuts3LE->setGeometry(QRect(132, 70, 620, 22));
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette19.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette19.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        efficiencyCuts3LE->setPalette(palette19);
        efficiencyCuts3LE->setFont(font3);
        efficiencyMainCutsLE = new customLineEdit(groupBox_7);
        efficiencyMainCutsLE->setObjectName(QString::fromUtf8("efficiencyMainCutsLE"));
        efficiencyMainCutsLE->setEnabled(false);
        efficiencyMainCutsLE->setGeometry(QRect(132, 18, 620, 22));
        QPalette palette20;
        palette20.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette20.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette20.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        efficiencyMainCutsLE->setPalette(palette20);
        efficiencyMainCutsLE->setFont(font3);
        efficiencyMainCutsCB = new QCheckBox(groupBox_7);
        efficiencyMainCutsCB->setObjectName(QString::fromUtf8("efficiencyMainCutsCB"));
        efficiencyMainCutsCB->setGeometry(QRect(4, 18, 106, 22));
        efficiencyMainCutsCB->setFont(font7);
        studiesTW->addTab(efficiencyT, QString());
        chargeT = new QWidget();
        chargeT->setObjectName(QString::fromUtf8("chargeT"));
        chargeCutsGB = new QGroupBox(chargeT);
        chargeCutsGB->setObjectName(QString::fromUtf8("chargeCutsGB"));
        chargeCutsGB->setGeometry(QRect(740, 4, 271, 369));
        chi2ChargeCutCB = new QCheckBox(chargeCutsGB);
        chi2ChargeCutCB->setObjectName(QString::fromUtf8("chi2ChargeCutCB"));
        chi2ChargeCutCB->setGeometry(QRect(8, 17, 84, 22));
        chi2ChargeCutSB = new QDoubleSpinBox(chargeCutsGB);
        chi2ChargeCutSB->setObjectName(QString::fromUtf8("chi2ChargeCutSB"));
        chi2ChargeCutSB->setGeometry(QRect(148, 17, 60, 20));
        chi2ChargeCutSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        chi2ChargeCutSB->setDecimals(1);
        chi2ChargeCutSB->setMinimum(1);
        chi2ChargeCutSB->setMaximum(20);
        chi2ChargeCutSB->setSingleStep(0.5);
        chi2ChargeCutSB->setValue(2);
        nTrackChargeCutCB = new QCheckBox(chargeCutsGB);
        nTrackChargeCutCB->setObjectName(QString::fromUtf8("nTrackChargeCutCB"));
        nTrackChargeCutCB->setGeometry(QRect(8, 40, 134, 22));
        nTrackChargeCutSB = new QSpinBox(chargeCutsGB);
        nTrackChargeCutSB->setObjectName(QString::fromUtf8("nTrackChargeCutSB"));
        nTrackChargeCutSB->setGeometry(QRect(148, 40, 60, 20));
        nTrackChargeCutSB->setLayoutDirection(Qt::LeftToRight);
        nTrackChargeCutSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        nTrackChargeCutSB->setMinimum(1);
        nTrackChargeCutSB->setMaximum(10);
        nTrackChargeCutSB->setValue(1);
        xErrChargeCutCB = new QCheckBox(chargeCutsGB);
        xErrChargeCutCB->setObjectName(QString::fromUtf8("xErrChargeCutCB"));
        xErrChargeCutCB->setGeometry(QRect(8, 64, 133, 22));
        yErrChargeCutCB = new QCheckBox(chargeCutsGB);
        yErrChargeCutCB->setObjectName(QString::fromUtf8("yErrChargeCutCB"));
        yErrChargeCutCB->setGeometry(QRect(8, 88, 131, 22));
        xErrChargeCutSB = new QDoubleSpinBox(chargeCutsGB);
        xErrChargeCutSB->setObjectName(QString::fromUtf8("xErrChargeCutSB"));
        xErrChargeCutSB->setGeometry(QRect(148, 64, 60, 20));
        xErrChargeCutSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xErrChargeCutSB->setDecimals(1);
        xErrChargeCutSB->setMinimum(0);
        xErrChargeCutSB->setMaximum(50);
        xErrChargeCutSB->setSingleStep(0.5);
        xErrChargeCutSB->setValue(6);
        yErrChargeCutSB = new QDoubleSpinBox(chargeCutsGB);
        yErrChargeCutSB->setObjectName(QString::fromUtf8("yErrChargeCutSB"));
        yErrChargeCutSB->setGeometry(QRect(148, 88, 60, 20));
        yErrChargeCutSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yErrChargeCutSB->setDecimals(1);
        yErrChargeCutSB->setMinimum(0);
        yErrChargeCutSB->setMaximum(50);
        yErrChargeCutSB->setSingleStep(0.5);
        yErrChargeCutSB->setValue(6);
        xErrChargeLB = new QLabel(chargeCutsGB);
        xErrChargeLB->setObjectName(QString::fromUtf8("xErrChargeLB"));
        xErrChargeLB->setEnabled(false);
        xErrChargeLB->setGeometry(QRect(212, 64, 31, 22));
        xErrChargeLB->setFont(font7);
        yErrChargeLB = new QLabel(chargeCutsGB);
        yErrChargeLB->setObjectName(QString::fromUtf8("yErrChargeLB"));
        yErrChargeLB->setEnabled(false);
        yErrChargeLB->setGeometry(QRect(212, 88, 31, 22));
        yErrChargeLB->setFont(font7);
        xChargeCutCB = new QCheckBox(chargeCutsGB);
        xChargeCutCB->setObjectName(QString::fromUtf8("xChargeCutCB"));
        xChargeCutCB->setGeometry(QRect(8, 112, 87, 22));
        yChargeCutCB = new QCheckBox(chargeCutsGB);
        yChargeCutCB->setObjectName(QString::fromUtf8("yChargeCutCB"));
        yChargeCutCB->setGeometry(QRect(8, 136, 85, 22));
        xChargeCutSB = new QDoubleSpinBox(chargeCutsGB);
        xChargeCutSB->setObjectName(QString::fromUtf8("xChargeCutSB"));
        xChargeCutSB->setGeometry(QRect(148, 112, 60, 20));
        xChargeCutSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xChargeCutSB->setDecimals(1);
        xChargeCutSB->setMinimum(-75);
        xChargeCutSB->setMaximum(75);
        xChargeCutSB->setSingleStep(0.5);
        xChargeCutSB->setValue(55);
        yChargeCutSB = new QDoubleSpinBox(chargeCutsGB);
        yChargeCutSB->setObjectName(QString::fromUtf8("yChargeCutSB"));
        yChargeCutSB->setGeometry(QRect(148, 136, 60, 20));
        yChargeCutSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yChargeCutSB->setDecimals(1);
        yChargeCutSB->setMinimum(-50);
        yChargeCutSB->setMaximum(50);
        yChargeCutSB->setSingleStep(0.5);
        yChargeCutSB->setValue(30);
        xChargeLB = new QLabel(chargeCutsGB);
        xChargeLB->setObjectName(QString::fromUtf8("xChargeLB"));
        xChargeLB->setEnabled(false);
        xChargeLB->setGeometry(QRect(212, 112, 31, 22));
        xChargeLB->setFont(font7);
        yChargeLB = new QLabel(chargeCutsGB);
        yChargeLB->setObjectName(QString::fromUtf8("yChargeLB"));
        yChargeLB->setEnabled(false);
        yChargeLB->setGeometry(QRect(212, 136, 31, 22));
        yChargeLB->setFont(font7);
        hitsChargeCutSB = new customSpinBox(chargeCutsGB);
        hitsChargeCutSB->setObjectName(QString::fromUtf8("hitsChargeCutSB"));
        hitsChargeCutSB->setGeometry(QRect(148, 160, 60, 20));
        hitsChargeCutSB->setLayoutDirection(Qt::LeftToRight);
        hitsChargeCutSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        hitsChargeCutSB->setMinimum(1);
        hitsChargeCutSB->setMaximum(50);
        hitsChargeCutSB->setValue(1);
        hitsChargeCutCB = new QCheckBox(chargeCutsGB);
        hitsChargeCutCB->setObjectName(QString::fromUtf8("hitsChargeCutCB"));
        hitsChargeCutCB->setGeometry(QRect(8, 160, 72, 22));
        chargePixelMinimumChargeSB = new customSpinBox(chargeCutsGB);
        chargePixelMinimumChargeSB->setObjectName(QString::fromUtf8("chargePixelMinimumChargeSB"));
        chargePixelMinimumChargeSB->setGeometry(QRect(148, 268, 62, 20));
        chargePixelMinimumChargeSB->setLayoutDirection(Qt::LeftToRight);
        chargePixelMinimumChargeSB->setMinimum(0);
        chargePixelMinimumChargeSB->setMaximum(10000);
        chargePixelMinimumChargeSB->setSingleStep(100);
        chargePixelMinimumChargeSB->setValue(2000);
        chargeClusterMinimumChargeSB = new customSpinBox(chargeCutsGB);
        chargeClusterMinimumChargeSB->setObjectName(QString::fromUtf8("chargeClusterMinimumChargeSB"));
        chargeClusterMinimumChargeSB->setGeometry(QRect(148, 316, 62, 20));
        chargeClusterMinimumChargeSB->setLayoutDirection(Qt::LeftToRight);
        chargeClusterMinimumChargeSB->setMinimum(0);
        chargeClusterMinimumChargeSB->setMaximum(100000);
        chargeClusterMinimumChargeSB->setSingleStep(100);
        chargeClusterMinimumChargeSB->setValue(22000);
        chargePixelMinimumChargeElectronLB = new QLabel(chargeCutsGB);
        chargePixelMinimumChargeElectronLB->setObjectName(QString::fromUtf8("chargePixelMinimumChargeElectronLB"));
        chargePixelMinimumChargeElectronLB->setGeometry(QRect(220, 268, 31, 22));
        chargeClusterMaximumChargeElectronLB = new QLabel(chargeCutsGB);
        chargeClusterMaximumChargeElectronLB->setObjectName(QString::fromUtf8("chargeClusterMaximumChargeElectronLB"));
        chargeClusterMaximumChargeElectronLB->setGeometry(QRect(220, 340, 31, 22));
        chargeClusterMaximumChargeSB = new customSpinBox(chargeCutsGB);
        chargeClusterMaximumChargeSB->setObjectName(QString::fromUtf8("chargeClusterMaximumChargeSB"));
        chargeClusterMaximumChargeSB->setGeometry(QRect(148, 340, 62, 20));
        chargeClusterMaximumChargeSB->setLayoutDirection(Qt::LeftToRight);
        chargeClusterMaximumChargeSB->setMinimum(0);
        chargeClusterMaximumChargeSB->setMaximum(100000);
        chargeClusterMaximumChargeSB->setSingleStep(100);
        chargeClusterMaximumChargeSB->setValue(24000);
        chargeClusterMinimumChargeElectronLB = new QLabel(chargeCutsGB);
        chargeClusterMinimumChargeElectronLB->setObjectName(QString::fromUtf8("chargeClusterMinimumChargeElectronLB"));
        chargeClusterMinimumChargeElectronLB->setGeometry(QRect(220, 316, 31, 22));
        chargePixelMaximumChargeSB = new customSpinBox(chargeCutsGB);
        chargePixelMaximumChargeSB->setObjectName(QString::fromUtf8("chargePixelMaximumChargeSB"));
        chargePixelMaximumChargeSB->setGeometry(QRect(148, 292, 62, 20));
        chargePixelMaximumChargeSB->setLayoutDirection(Qt::LeftToRight);
        chargePixelMaximumChargeSB->setMinimum(0);
        chargePixelMaximumChargeSB->setMaximum(100000);
        chargePixelMaximumChargeSB->setSingleStep(100);
        chargePixelMaximumChargeSB->setValue(1);
        chargePixelMaximumChargeElectronLB = new QLabel(chargeCutsGB);
        chargePixelMaximumChargeElectronLB->setObjectName(QString::fromUtf8("chargePixelMaximumChargeElectronLB"));
        chargePixelMaximumChargeElectronLB->setGeometry(QRect(220, 292, 31, 22));
        chargePixelMinimumChargeLB = new QLabel(chargeCutsGB);
        chargePixelMinimumChargeLB->setObjectName(QString::fromUtf8("chargePixelMinimumChargeLB"));
        chargePixelMinimumChargeLB->setGeometry(QRect(8, 272, 111, 17));
        chargePixelMaximumChargeLB = new QLabel(chargeCutsGB);
        chargePixelMaximumChargeLB->setObjectName(QString::fromUtf8("chargePixelMaximumChargeLB"));
        chargePixelMaximumChargeLB->setGeometry(QRect(8, 296, 113, 17));
        chargeClusterMinimumChargeLB = new QLabel(chargeCutsGB);
        chargeClusterMinimumChargeLB->setObjectName(QString::fromUtf8("chargeClusterMinimumChargeLB"));
        chargeClusterMinimumChargeLB->setGeometry(QRect(8, 320, 121, 17));
        chargeClusterMaximumChargeLB = new QLabel(chargeCutsGB);
        chargeClusterMaximumChargeLB->setObjectName(QString::fromUtf8("chargeClusterMaximumChargeLB"));
        chargeClusterMaximumChargeLB->setGeometry(QRect(8, 344, 129, 17));
        chargeExcludeBadPlanesCB = new customCheckBox(chargeCutsGB);
        chargeExcludeBadPlanesCB->setObjectName(QString::fromUtf8("chargeExcludeBadPlanesCB"));
        chargeExcludeBadPlanesCB->setGeometry(QRect(8, 216, 211, 20));
        label_2 = new QLabel(chargeCutsGB);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(8, 244, 101, 20));
        chargeBadPlanesHitsSB = new customSpinBox(chargeCutsGB);
        chargeBadPlanesHitsSB->setObjectName(QString::fromUtf8("chargeBadPlanesHitsSB"));
        chargeBadPlanesHitsSB->setGeometry(QRect(148, 244, 61, 20));
        chargeBadPlanesHitsSB->setValue(50);
        label_4 = new QLabel(chargeCutsGB);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(212, 248, 57, 16));
        chargeStandardCutsCB = new customCheckBox(chargeCutsGB);
        chargeStandardCutsCB->setObjectName(QString::fromUtf8("chargeStandardCutsCB"));
        chargeStandardCutsCB->setGeometry(QRect(8, 188, 131, 22));
        parsCutLB = new QLabel(chargeT);
        parsCutLB->setObjectName(QString::fromUtf8("parsCutLB"));
        parsCutLB->setGeometry(QRect(8, 192, 225, 16));
        parsCutLB->setFont(font3);
        parsTW = new QTabWidget(chargeT);
        parsTW->setObjectName(QString::fromUtf8("parsTW"));
        parsTW->setGeometry(QRect(8, 212, 221, 77));
        QPalette palette21;
        palette21.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette21.setBrush(QPalette::Active, QPalette::Button, brush40);
        palette21.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette21.setBrush(QPalette::Active, QPalette::Midlight, brush41);
        palette21.setBrush(QPalette::Active, QPalette::Dark, brush42);
        palette21.setBrush(QPalette::Active, QPalette::Mid, brush43);
        palette21.setBrush(QPalette::Active, QPalette::Text, brush);
        palette21.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette21.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette21.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette21.setBrush(QPalette::Active, QPalette::Window, brush40);
        palette21.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette21.setBrush(QPalette::Active, QPalette::AlternateBase, brush41);
        palette21.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette21.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::Button, brush40);
        palette21.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::Midlight, brush41);
        palette21.setBrush(QPalette::Inactive, QPalette::Dark, brush42);
        palette21.setBrush(QPalette::Inactive, QPalette::Mid, brush43);
        palette21.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::Window, brush40);
        palette21.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush41);
        palette21.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette21.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette21.setBrush(QPalette::Disabled, QPalette::WindowText, brush42);
        palette21.setBrush(QPalette::Disabled, QPalette::Button, brush40);
        palette21.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::Midlight, brush41);
        palette21.setBrush(QPalette::Disabled, QPalette::Dark, brush42);
        palette21.setBrush(QPalette::Disabled, QPalette::Mid, brush43);
        palette21.setBrush(QPalette::Disabled, QPalette::Text, brush42);
        palette21.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::ButtonText, brush42);
        palette21.setBrush(QPalette::Disabled, QPalette::Base, brush40);
        palette21.setBrush(QPalette::Disabled, QPalette::Window, brush40);
        palette21.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette21.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush40);
        palette21.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette21.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        parsTW->setPalette(palette21);
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        par0minCB = new QCheckBox(tab_2);
        par0minCB->setObjectName(QString::fromUtf8("par0minCB"));
        par0minCB->setGeometry(QRect(2, 4, 61, 22));
        par0minLE = new customLineEdit(tab_2);
        par0minLE->setObjectName(QString::fromUtf8("par0minLE"));
        par0minLE->setEnabled(false);
        par0minLE->setGeometry(QRect(63, 4, 81, 20));
        par0maxCB = new QCheckBox(tab_2);
        par0maxCB->setObjectName(QString::fromUtf8("par0maxCB"));
        par0maxCB->setGeometry(QRect(2, 28, 61, 22));
        par0maxLE = new customLineEdit(tab_2);
        par0maxLE->setObjectName(QString::fromUtf8("par0maxLE"));
        par0maxLE->setEnabled(false);
        par0maxLE->setGeometry(QRect(63, 28, 81, 20));
        parsTW->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        par1minCB = new QCheckBox(tab_3);
        par1minCB->setObjectName(QString::fromUtf8("par1minCB"));
        par1minCB->setGeometry(QRect(2, 4, 61, 22));
        par1maxCB = new QCheckBox(tab_3);
        par1maxCB->setObjectName(QString::fromUtf8("par1maxCB"));
        par1maxCB->setGeometry(QRect(2, 28, 61, 22));
        par1minLE = new customLineEdit(tab_3);
        par1minLE->setObjectName(QString::fromUtf8("par1minLE"));
        par1minLE->setEnabled(false);
        par1minLE->setGeometry(QRect(63, 4, 81, 22));
        par1maxLE = new customLineEdit(tab_3);
        par1maxLE->setObjectName(QString::fromUtf8("par1maxLE"));
        par1maxLE->setEnabled(false);
        par1maxLE->setGeometry(QRect(63, 28, 81, 22));
        parsTW->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        par2minCB = new QCheckBox(tab_4);
        par2minCB->setObjectName(QString::fromUtf8("par2minCB"));
        par2minCB->setGeometry(QRect(2, 4, 61, 22));
        par2minLE = new customLineEdit(tab_4);
        par2minLE->setObjectName(QString::fromUtf8("par2minLE"));
        par2minLE->setEnabled(false);
        par2minLE->setGeometry(QRect(63, 4, 81, 22));
        par2maxCB = new QCheckBox(tab_4);
        par2maxCB->setObjectName(QString::fromUtf8("par2maxCB"));
        par2maxCB->setGeometry(QRect(2, 28, 61, 22));
        par2maxLE = new customLineEdit(tab_4);
        par2maxLE->setObjectName(QString::fromUtf8("par2maxLE"));
        par2maxLE->setEnabled(false);
        par2maxLE->setGeometry(QRect(63, 28, 81, 22));
        parsTW->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        par3minCB = new QCheckBox(tab_5);
        par3minCB->setObjectName(QString::fromUtf8("par3minCB"));
        par3minCB->setGeometry(QRect(2, 4, 61, 22));
        par3minLE = new customLineEdit(tab_5);
        par3minLE->setObjectName(QString::fromUtf8("par3minLE"));
        par3minLE->setEnabled(false);
        par3minLE->setGeometry(QRect(63, 4, 81, 22));
        par3maxCB = new QCheckBox(tab_5);
        par3maxCB->setObjectName(QString::fromUtf8("par3maxCB"));
        par3maxCB->setGeometry(QRect(2, 28, 61, 22));
        par3maxLE = new customLineEdit(tab_5);
        par3maxLE->setObjectName(QString::fromUtf8("par3maxLE"));
        par3maxLE->setEnabled(false);
        par3maxLE->setGeometry(QRect(63, 28, 81, 22));
        parsTW->addTab(tab_5, QString());
        textBrowser = new QTextBrowser(chargeT);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(236, 240, 351, 51));
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette22.setBrush(QPalette::Active, QPalette::Button, brush);
        QBrush brush46(QColor(255, 197, 129, 255));
        brush46.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Light, brush46);
        QBrush brush47(QColor(255, 167, 65, 255));
        brush47.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Midlight, brush47);
        QBrush brush48(QColor(127, 68, 0, 255));
        brush48.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Dark, brush48);
        QBrush brush49(QColor(170, 91, 0, 255));
        brush49.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Mid, brush49);
        palette22.setBrush(QPalette::Active, QPalette::Text, brush);
        palette22.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette22.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush50(QColor(255, 255, 127, 255));
        brush50.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Base, brush50);
        palette22.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette22.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush51(QColor(255, 196, 128, 255));
        brush51.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::AlternateBase, brush51);
        palette22.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette22.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::Light, brush46);
        palette22.setBrush(QPalette::Inactive, QPalette::Midlight, brush47);
        palette22.setBrush(QPalette::Inactive, QPalette::Dark, brush48);
        palette22.setBrush(QPalette::Inactive, QPalette::Mid, brush49);
        palette22.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::Base, brush50);
        palette22.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette22.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush51);
        palette22.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette22.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette22.setBrush(QPalette::Disabled, QPalette::WindowText, brush48);
        palette22.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette22.setBrush(QPalette::Disabled, QPalette::Light, brush46);
        palette22.setBrush(QPalette::Disabled, QPalette::Midlight, brush47);
        palette22.setBrush(QPalette::Disabled, QPalette::Dark, brush48);
        palette22.setBrush(QPalette::Disabled, QPalette::Mid, brush49);
        palette22.setBrush(QPalette::Disabled, QPalette::Text, brush48);
        palette22.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette22.setBrush(QPalette::Disabled, QPalette::ButtonText, brush48);
        palette22.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette22.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        palette22.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette22.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush35);
        palette22.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette22.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        textBrowser->setPalette(palette22);
        groupBox_8 = new QGroupBox(chargeT);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setGeometry(QRect(4, 4, 729, 181));
        chargeCuts5LE = new customLineEdit(groupBox_8);
        chargeCuts5LE->setObjectName(QString::fromUtf8("chargeCuts5LE"));
        chargeCuts5LE->setEnabled(false);
        chargeCuts5LE->setGeometry(QRect(120, 44, 600, 22));
        QPalette palette23;
        palette23.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette23.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette23.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        chargeCuts5LE->setPalette(palette23);
        chargeCuts5LE->setFont(font1);
        chargeCuts5LE->setReadOnly(false);
        chargeCuts3LE = new customLineEdit(groupBox_8);
        chargeCuts3LE->setObjectName(QString::fromUtf8("chargeCuts3LE"));
        chargeCuts3LE->setEnabled(false);
        chargeCuts3LE->setGeometry(QRect(120, 124, 600, 22));
        QPalette palette24;
        palette24.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette24.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette24.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        chargeCuts3LE->setPalette(palette24);
        chargeCuts3LE->setFont(font1);
        chargeCuts3LE->setReadOnly(false);
        chargeMainCutsCB = new QCheckBox(groupBox_8);
        chargeMainCutsCB->setObjectName(QString::fromUtf8("chargeMainCutsCB"));
        chargeMainCutsCB->setGeometry(QRect(5, 16, 106, 22));
        QFont font8;
        font8.setFamily(QString::fromUtf8("Sans Serif"));
        font8.setPointSize(9);
        font8.setBold(false);
        font8.setWeight(50);
        chargeMainCutsCB->setFont(font8);
        chargeMainCutsLE = new customLineEdit(groupBox_8);
        chargeMainCutsLE->setObjectName(QString::fromUtf8("chargeMainCutsLE"));
        chargeMainCutsLE->setEnabled(false);
        chargeMainCutsLE->setGeometry(QRect(120, 16, 600, 22));
        QPalette palette25;
        palette25.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette25.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette25.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        chargeMainCutsLE->setPalette(palette25);
        chargeMainCutsLE->setFont(font1);
        chargeMainCutsLE->setReadOnly(false);
        chargeCuts2LE = new customLineEdit(groupBox_8);
        chargeCuts2LE->setObjectName(QString::fromUtf8("chargeCuts2LE"));
        chargeCuts2LE->setEnabled(false);
        chargeCuts2LE->setGeometry(QRect(120, 98, 600, 22));
        QPalette palette26;
        palette26.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette26.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette26.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        chargeCuts2LE->setPalette(palette26);
        chargeCuts2LE->setFont(font1);
        chargeCuts2LE->setReadOnly(false);
        chargeCuts2CB = new QCheckBox(groupBox_8);
        chargeCuts2CB->setObjectName(QString::fromUtf8("chargeCuts2CB"));
        chargeCuts2CB->setGeometry(QRect(5, 98, 105, 22));
        chargeCuts2CB->setFont(font8);
        chargeCuts6CB = new QCheckBox(groupBox_8);
        chargeCuts6CB->setObjectName(QString::fromUtf8("chargeCuts6CB"));
        chargeCuts6CB->setGeometry(QRect(5, 72, 113, 22));
        chargeCuts6CB->setFont(font8);
        chargeCuts6LE = new customLineEdit(groupBox_8);
        chargeCuts6LE->setObjectName(QString::fromUtf8("chargeCuts6LE"));
        chargeCuts6LE->setEnabled(false);
        chargeCuts6LE->setGeometry(QRect(120, 72, 600, 22));
        QPalette palette27;
        palette27.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette27.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette27.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        chargeCuts6LE->setPalette(palette27);
        chargeCuts6LE->setFont(font1);
        chargeCuts6LE->setReadOnly(false);
        chargeCuts3CB = new QCheckBox(groupBox_8);
        chargeCuts3CB->setObjectName(QString::fromUtf8("chargeCuts3CB"));
        chargeCuts3CB->setGeometry(QRect(5, 124, 105, 22));
        chargeCuts3CB->setFont(font8);
        chargeCuts4CB = new QCheckBox(groupBox_8);
        chargeCuts4CB->setObjectName(QString::fromUtf8("chargeCuts4CB"));
        chargeCuts4CB->setGeometry(QRect(5, 152, 105, 22));
        chargeCuts4CB->setFont(font8);
        chargeCuts5CB = new QCheckBox(groupBox_8);
        chargeCuts5CB->setObjectName(QString::fromUtf8("chargeCuts5CB"));
        chargeCuts5CB->setGeometry(QRect(5, 44, 113, 22));
        chargeCuts5CB->setFont(font8);
        chargeCuts4LE = new customLineEdit(groupBox_8);
        chargeCuts4LE->setObjectName(QString::fromUtf8("chargeCuts4LE"));
        chargeCuts4LE->setEnabled(false);
        chargeCuts4LE->setGeometry(QRect(120, 152, 600, 22));
        QPalette palette28;
        palette28.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette28.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette28.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        chargeCuts4LE->setPalette(palette28);
        chargeCuts4LE->setFont(font1);
        chargeCuts4LE->setReadOnly(false);
        studiesTW->addTab(chargeT, QString());
        resolutionT = new QWidget();
        resolutionT->setObjectName(QString::fromUtf8("resolutionT"));
        resolutionCutsGB = new QGroupBox(resolutionT);
        resolutionCutsGB->setObjectName(QString::fromUtf8("resolutionCutsGB"));
        resolutionCutsGB->setGeometry(QRect(772, 0, 241, 229));
        chi2ResolutionCutCB = new QCheckBox(resolutionCutsGB);
        chi2ResolutionCutCB->setObjectName(QString::fromUtf8("chi2ResolutionCutCB"));
        chi2ResolutionCutCB->setGeometry(QRect(4, 16, 93, 22));
        chi2ResolutionCutSB = new QDoubleSpinBox(resolutionCutsGB);
        chi2ResolutionCutSB->setObjectName(QString::fromUtf8("chi2ResolutionCutSB"));
        chi2ResolutionCutSB->setGeometry(QRect(146, 16, 62, 20));
        chi2ResolutionCutSB->setDecimals(1);
        chi2ResolutionCutSB->setMinimum(1);
        chi2ResolutionCutSB->setMaximum(20);
        chi2ResolutionCutSB->setSingleStep(0.5);
        chi2ResolutionCutSB->setValue(2);
        nTrackResolutionCutCB = new QCheckBox(resolutionCutsGB);
        nTrackResolutionCutCB->setObjectName(QString::fromUtf8("nTrackResolutionCutCB"));
        nTrackResolutionCutCB->setGeometry(QRect(4, 40, 141, 22));
        nTrackResolutionCutSB = new QSpinBox(resolutionCutsGB);
        nTrackResolutionCutSB->setObjectName(QString::fromUtf8("nTrackResolutionCutSB"));
        nTrackResolutionCutSB->setGeometry(QRect(146, 40, 62, 20));
        nTrackResolutionCutSB->setLayoutDirection(Qt::LeftToRight);
        nTrackResolutionCutSB->setMinimum(1);
        nTrackResolutionCutSB->setMaximum(10);
        nTrackResolutionCutSB->setValue(1);
        xErrResolutionCutCB = new QCheckBox(resolutionCutsGB);
        xErrResolutionCutCB->setObjectName(QString::fromUtf8("xErrResolutionCutCB"));
        xErrResolutionCutCB->setGeometry(QRect(4, 64, 141, 22));
        yErrResolutionCutCB = new QCheckBox(resolutionCutsGB);
        yErrResolutionCutCB->setObjectName(QString::fromUtf8("yErrResolutionCutCB"));
        yErrResolutionCutCB->setGeometry(QRect(4, 88, 141, 22));
        xErrResolutionCutSB = new QDoubleSpinBox(resolutionCutsGB);
        xErrResolutionCutSB->setObjectName(QString::fromUtf8("xErrResolutionCutSB"));
        xErrResolutionCutSB->setGeometry(QRect(146, 64, 62, 20));
        xErrResolutionCutSB->setDecimals(1);
        xErrResolutionCutSB->setMinimum(0);
        xErrResolutionCutSB->setMaximum(50);
        xErrResolutionCutSB->setSingleStep(0.5);
        xErrResolutionCutSB->setValue(6);
        yErrResolutionCutSB = new QDoubleSpinBox(resolutionCutsGB);
        yErrResolutionCutSB->setObjectName(QString::fromUtf8("yErrResolutionCutSB"));
        yErrResolutionCutSB->setGeometry(QRect(146, 88, 62, 20));
        yErrResolutionCutSB->setDecimals(1);
        yErrResolutionCutSB->setMinimum(0);
        yErrResolutionCutSB->setMaximum(50);
        yErrResolutionCutSB->setSingleStep(0.5);
        yErrResolutionCutSB->setValue(6);
        xErrResolutionLB = new QLabel(resolutionCutsGB);
        xErrResolutionLB->setObjectName(QString::fromUtf8("xErrResolutionLB"));
        xErrResolutionLB->setEnabled(false);
        xErrResolutionLB->setGeometry(QRect(216, 64, 31, 22));
        xErrResolutionLB->setFont(font7);
        yErrResolutionLB = new QLabel(resolutionCutsGB);
        yErrResolutionLB->setObjectName(QString::fromUtf8("yErrResolutionLB"));
        yErrResolutionLB->setEnabled(false);
        yErrResolutionLB->setGeometry(QRect(216, 88, 31, 22));
        yErrResolutionLB->setFont(font7);
        hitsResolutionCutSB = new customSpinBox(resolutionCutsGB);
        hitsResolutionCutSB->setObjectName(QString::fromUtf8("hitsResolutionCutSB"));
        hitsResolutionCutSB->setGeometry(QRect(146, 112, 62, 20));
        hitsResolutionCutSB->setLayoutDirection(Qt::LeftToRight);
        hitsResolutionCutSB->setMinimum(1);
        hitsResolutionCutSB->setMaximum(50);
        hitsResolutionCutSB->setValue(1);
        hitsResolutionCutCB = new QCheckBox(resolutionCutsGB);
        hitsResolutionCutCB->setObjectName(QString::fromUtf8("hitsResolutionCutCB"));
        hitsResolutionCutCB->setGeometry(QRect(4, 112, 131, 22));
        resolutionStandardCutsCB = new customCheckBox(resolutionCutsGB);
        resolutionStandardCutsCB->setObjectName(QString::fromUtf8("resolutionStandardCutsCB"));
        resolutionStandardCutsCB->setGeometry(QRect(4, 152, 131, 22));
        resolutionExcludeBadPlanesCB = new customCheckBox(resolutionCutsGB);
        resolutionExcludeBadPlanesCB->setObjectName(QString::fromUtf8("resolutionExcludeBadPlanesCB"));
        resolutionExcludeBadPlanesCB->setGeometry(QRect(4, 172, 231, 20));
        label_6 = new QLabel(resolutionCutsGB);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(140, 196, 85, 16));
        resolutionCanvasW = new QWidget(resolutionT);
        resolutionCanvasW->setObjectName(QString::fromUtf8("resolutionCanvasW"));
        resolutionCanvasW->setGeometry(QRect(4, 132, 761, 477));
        groupBox_9 = new QGroupBox(resolutionT);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        groupBox_9->setGeometry(QRect(0, 0, 765, 129));
        resolutionCuts4LE = new customLineEdit(groupBox_9);
        resolutionCuts4LE->setObjectName(QString::fromUtf8("resolutionCuts4LE"));
        resolutionCuts4LE->setEnabled(false);
        resolutionCuts4LE->setGeometry(QRect(128, 100, 630, 22));
        QPalette palette29;
        palette29.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette29.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette29.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        resolutionCuts4LE->setPalette(palette29);
        resolutionCuts4LE->setFont(font1);
        resolutionCuts4LE->setReadOnly(false);
        resolutionMainCutsLE = new customLineEdit(groupBox_9);
        resolutionMainCutsLE->setObjectName(QString::fromUtf8("resolutionMainCutsLE"));
        resolutionMainCutsLE->setEnabled(false);
        resolutionMainCutsLE->setGeometry(QRect(128, 18, 630, 22));
        QPalette palette30;
        palette30.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette30.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette30.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        resolutionMainCutsLE->setPalette(palette30);
        resolutionMainCutsLE->setFont(font1);
        resolutionMainCutsLE->setReadOnly(false);
        resolutionMainCutsCB = new QCheckBox(groupBox_9);
        resolutionMainCutsCB->setObjectName(QString::fromUtf8("resolutionMainCutsCB"));
        resolutionMainCutsCB->setGeometry(QRect(5, 18, 106, 22));
        resolutionMainCutsCB->setFont(font3);
        resolutionCuts2CB = new QCheckBox(groupBox_9);
        resolutionCuts2CB->setObjectName(QString::fromUtf8("resolutionCuts2CB"));
        resolutionCuts2CB->setGeometry(QRect(5, 44, 121, 22));
        resolutionCuts2CB->setFont(font3);
        resolutionCuts3CB = new QCheckBox(groupBox_9);
        resolutionCuts3CB->setObjectName(QString::fromUtf8("resolutionCuts3CB"));
        resolutionCuts3CB->setGeometry(QRect(5, 72, 121, 22));
        resolutionCuts3CB->setFont(font3);
        resolutionCuts3LE = new customLineEdit(groupBox_9);
        resolutionCuts3LE->setObjectName(QString::fromUtf8("resolutionCuts3LE"));
        resolutionCuts3LE->setEnabled(false);
        resolutionCuts3LE->setGeometry(QRect(128, 72, 630, 22));
        QPalette palette31;
        palette31.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette31.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette31.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        resolutionCuts3LE->setPalette(palette31);
        resolutionCuts3LE->setFont(font1);
        resolutionCuts3LE->setReadOnly(false);
        resolutionCuts4CB = new QCheckBox(groupBox_9);
        resolutionCuts4CB->setObjectName(QString::fromUtf8("resolutionCuts4CB"));
        resolutionCuts4CB->setGeometry(QRect(5, 100, 121, 22));
        resolutionCuts4CB->setFont(font3);
        resolutionCuts2LE = new customLineEdit(groupBox_9);
        resolutionCuts2LE->setObjectName(QString::fromUtf8("resolutionCuts2LE"));
        resolutionCuts2LE->setEnabled(false);
        resolutionCuts2LE->setGeometry(QRect(128, 44, 630, 22));
        QPalette palette32;
        palette32.setBrush(QPalette::Active, QPalette::Text, brush44);
        palette32.setBrush(QPalette::Inactive, QPalette::Text, brush44);
        palette32.setBrush(QPalette::Disabled, QPalette::Text, brush45);
        resolutionCuts2LE->setPalette(palette32);
        resolutionCuts2LE->setFont(font1);
        resolutionCuts2LE->setReadOnly(false);
        studiesTW->addTab(resolutionT, QString());
        runT = new QWidget();
        runT->setObjectName(QString::fromUtf8("runT"));
        analysesGB = new QGroupBox(runT);
        analysesGB->setObjectName(QString::fromUtf8("analysesGB"));
        analysesGB->setGeometry(QRect(4, 444, 521, 169));
        QPalette palette33;
        palette33.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette33.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette33.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        analysesGB->setPalette(palette33);
        analysesGB->setFont(font5);
        analysesGB->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        efficiencyCB = new customCheckBox(analysesGB);
        efficiencyCB->setObjectName(QString::fromUtf8("efficiencyCB"));
        efficiencyCB->setGeometry(QRect(6, 76, 91, 21));
        efficiencyCB->setChecked(false);
        chargeCB = new customCheckBox(analysesGB);
        chargeCB->setObjectName(QString::fromUtf8("chargeCB"));
        chargeCB->setGeometry(QRect(6, 98, 83, 21));
        resolutionCB = new customCheckBox(analysesGB);
        resolutionCB->setObjectName(QString::fromUtf8("resolutionCB"));
        resolutionCB->setGeometry(QRect(6, 120, 93, 22));
        tracksCB = new customCheckBox(analysesGB);
        tracksCB->setObjectName(QString::fromUtf8("tracksCB"));
        tracksCB->setGeometry(QRect(6, 52, 83, 21));
        windowsCB = new customCheckBox(analysesGB);
        windowsCB->setObjectName(QString::fromUtf8("windowsCB"));
        windowsCB->setEnabled(false);
        windowsCB->setGeometry(QRect(6, 30, 83, 21));
        windowsCB->setChecked(true);
        runAnalysisPB = new QPushButton(analysesGB);
        runAnalysisPB->setObjectName(QString::fromUtf8("runAnalysisPB"));
        runAnalysisPB->setEnabled(false);
        runAnalysisPB->setGeometry(QRect(405, 140, 101, 20));
        QPalette palette34;
        palette34.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette34.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette34.setBrush(QPalette::Active, QPalette::Light, brush26);
        palette34.setBrush(QPalette::Active, QPalette::Midlight, brush27);
        palette34.setBrush(QPalette::Active, QPalette::Dark, brush28);
        palette34.setBrush(QPalette::Active, QPalette::Mid, brush29);
        palette34.setBrush(QPalette::Active, QPalette::Text, brush);
        palette34.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette34.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette34.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette34.setBrush(QPalette::Active, QPalette::Window, brush30);
        palette34.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette34.setBrush(QPalette::Active, QPalette::AlternateBase, brush31);
        palette34.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette34.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette34.setBrush(QPalette::Inactive, QPalette::Light, brush26);
        palette34.setBrush(QPalette::Inactive, QPalette::Midlight, brush27);
        palette34.setBrush(QPalette::Inactive, QPalette::Dark, brush28);
        palette34.setBrush(QPalette::Inactive, QPalette::Mid, brush29);
        palette34.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette34.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette34.setBrush(QPalette::Inactive, QPalette::Window, brush30);
        palette34.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush31);
        palette34.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette34.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette34.setBrush(QPalette::Disabled, QPalette::WindowText, brush28);
        palette34.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette34.setBrush(QPalette::Disabled, QPalette::Light, brush26);
        palette34.setBrush(QPalette::Disabled, QPalette::Midlight, brush27);
        palette34.setBrush(QPalette::Disabled, QPalette::Dark, brush28);
        palette34.setBrush(QPalette::Disabled, QPalette::Mid, brush29);
        palette34.setBrush(QPalette::Disabled, QPalette::Text, brush28);
        palette34.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette34.setBrush(QPalette::Disabled, QPalette::ButtonText, brush28);
        palette34.setBrush(QPalette::Disabled, QPalette::Base, brush30);
        palette34.setBrush(QPalette::Disabled, QPalette::Window, brush30);
        palette34.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette34.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush30);
        palette34.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette34.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        runAnalysisPB->setPalette(palette34);
        runAnalysisPB->setFont(font3);
        runMaxThreadsLB = new QLabel(analysesGB);
        runMaxThreadsLB->setObjectName(QString::fromUtf8("runMaxThreadsLB"));
        runMaxThreadsLB->setGeometry(QRect(330, 60, 96, 21));
        QFont font9;
        font9.setBold(false);
        font9.setWeight(50);
        runMaxThreadsLB->setFont(font9);
        runMaxThreadsSB = new customSpinBox(analysesGB);
        runMaxThreadsSB->setObjectName(QString::fromUtf8("runMaxThreadsSB"));
        runMaxThreadsSB->setGeometry(QRect(420, 60, 97, 21));
        runMaxThreadsSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        runMaxThreadsSB->setMinimum(0);
        runMaxThreadsSB->setMaximum(100);
        runMaxThreadsSB->setValue(0);
        runMaxEventsSB = new customSpinBox(analysesGB);
        runMaxEventsSB->setObjectName(QString::fromUtf8("runMaxEventsSB"));
        runMaxEventsSB->setGeometry(QRect(420, 36, 97, 21));
        runMaxEventsSB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        runMaxEventsSB->setMinimum(0);
        runMaxEventsSB->setMaximum(1000000);
        runMaxEventsSB->setValue(10000);
        runTotalEventsLB = new QLabel(analysesGB);
        runTotalEventsLB->setObjectName(QString::fromUtf8("runTotalEventsLB"));
        runTotalEventsLB->setGeometry(QRect(320, 12, 91, 22));
        runTotalEventsLB->setFont(font9);
        runTotalEventsLB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        runTotalEventsLE = new QLineEdit(analysesGB);
        runTotalEventsLE->setObjectName(QString::fromUtf8("runTotalEventsLE"));
        runTotalEventsLE->setGeometry(QRect(420, 12, 96, 21));
        QPalette palette35;
        palette35.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette35.setBrush(QPalette::Active, QPalette::Text, brush20);
        palette35.setBrush(QPalette::Active, QPalette::BrightText, brush20);
        palette35.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette35.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette35.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette35.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette35.setBrush(QPalette::Inactive, QPalette::Text, brush20);
        palette35.setBrush(QPalette::Inactive, QPalette::BrightText, brush20);
        palette35.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette35.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette35.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette35.setBrush(QPalette::Disabled, QPalette::WindowText, brush20);
        palette35.setBrush(QPalette::Disabled, QPalette::Text, brush20);
        palette35.setBrush(QPalette::Disabled, QPalette::BrightText, brush20);
        palette35.setBrush(QPalette::Disabled, QPalette::ButtonText, brush20);
        palette35.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette35.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        runTotalEventsLE->setPalette(palette35);
        runTotalEventsLE->setFont(font2);
        runTotalEventsLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        runTotalEventsLE->setReadOnly(true);
        runMaxEventsCB = new customCheckBox(analysesGB);
        runMaxEventsCB->setObjectName(QString::fromUtf8("runMaxEventsCB"));
        runMaxEventsCB->setGeometry(QRect(320, 36, 106, 21));
        runMaxEventsCB->setChecked(true);
        tracksafterCB = new customCheckBox(analysesGB);
        tracksafterCB->setObjectName(QString::fromUtf8("tracksafterCB"));
        tracksafterCB->setGeometry(QRect(6, 144, 101, 21));
        label = new QLabel(analysesGB);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 12, 101, 16));
        priorityWindowsSB = new customSpinBox(analysesGB);
        priorityWindowsSB->setObjectName(QString::fromUtf8("priorityWindowsSB"));
        priorityWindowsSB->setGeometry(QRect(126, 30, 51, 20));
        priorityWindowsSB->setMaximum(0);
        priorityTracksSB = new customSpinBox(analysesGB);
        priorityTracksSB->setObjectName(QString::fromUtf8("priorityTracksSB"));
        priorityTracksSB->setGeometry(QRect(126, 52, 51, 20));
        priorityTracksSB->setMinimum(1);
        priorityTracksSB->setMaximum(10);
        priorityEfficiencySB = new customSpinBox(analysesGB);
        priorityEfficiencySB->setObjectName(QString::fromUtf8("priorityEfficiencySB"));
        priorityEfficiencySB->setGeometry(QRect(126, 76, 51, 20));
        priorityEfficiencySB->setMinimum(1);
        priorityEfficiencySB->setMaximum(10);
        priorityChargeSB = new customSpinBox(analysesGB);
        priorityChargeSB->setObjectName(QString::fromUtf8("priorityChargeSB"));
        priorityChargeSB->setGeometry(QRect(126, 98, 51, 20));
        priorityChargeSB->setMinimum(1);
        priorityChargeSB->setMaximum(10);
        priorityResolutionSB = new customSpinBox(analysesGB);
        priorityResolutionSB->setObjectName(QString::fromUtf8("priorityResolutionSB"));
        priorityResolutionSB->setGeometry(QRect(126, 120, 51, 20));
        priorityResolutionSB->setMinimum(1);
        priorityResolutionSB->setMaximum(10);
        priorityTracksAfterSB = new customSpinBox(analysesGB);
        priorityTracksAfterSB->setObjectName(QString::fromUtf8("priorityTracksAfterSB"));
        priorityTracksAfterSB->setGeometry(QRect(126, 144, 51, 20));
        priorityTracksAfterSB->setMinimum(1);
        priorityTracksAfterSB->setMaximum(10);
        label_9 = new QLabel(analysesGB);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(208, 12, 101, 16));
        resolutionFitCB = new customCheckBox(analysesGB);
        resolutionFitCB->setObjectName(QString::fromUtf8("resolutionFitCB"));
        resolutionFitCB->setGeometry(QRect(198, 120, 93, 22));
        chargeFitCB = new customCheckBox(analysesGB);
        chargeFitCB->setObjectName(QString::fromUtf8("chargeFitCB"));
        chargeFitCB->setGeometry(QRect(198, 98, 83, 21));
        tracksafterFitCB = new customCheckBox(analysesGB);
        tracksafterFitCB->setObjectName(QString::fromUtf8("tracksafterFitCB"));
        tracksafterFitCB->setGeometry(QRect(198, 144, 111, 21));
        efficiencyFitCB = new customCheckBox(analysesGB);
        efficiencyFitCB->setObjectName(QString::fromUtf8("efficiencyFitCB"));
        efficiencyFitCB->setGeometry(QRect(198, 76, 91, 21));
        efficiencyFitCB->setChecked(false);
        windowsFitCB = new customCheckBox(analysesGB);
        windowsFitCB->setObjectName(QString::fromUtf8("windowsFitCB"));
        windowsFitCB->setEnabled(false);
        windowsFitCB->setGeometry(QRect(198, 30, 83, 21));
        windowsFitCB->setChecked(true);
        tracksFitCB = new customCheckBox(analysesGB);
        tracksFitCB->setObjectName(QString::fromUtf8("tracksFitCB"));
        tracksFitCB->setGeometry(QRect(198, 52, 83, 21));
        useCalibrationsCB = new QCheckBox(runT);
        useCalibrationsCB->setObjectName(QString::fromUtf8("useCalibrationsCB"));
        useCalibrationsCB->setGeometry(QRect(688, 452, 141, 22));
        dutsListLW_2 = new QListWidget(runT);
        dutsListLW_2->setObjectName(QString::fromUtf8("dutsListLW_2"));
        dutsListLW_2->setGeometry(QRect(528, 452, 157, 157));
        QPalette palette36;
        palette36.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette36.setBrush(QPalette::Active, QPalette::Highlight, brush32);
        palette36.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette36.setBrush(QPalette::Inactive, QPalette::Highlight, brush32);
        palette36.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette36.setBrush(QPalette::Disabled, QPalette::Highlight, brush33);
        dutsListLW_2->setPalette(palette36);
        runFilesManagementGB = new QGroupBox(runT);
        runFilesManagementGB->setObjectName(QString::fromUtf8("runFilesManagementGB"));
        runFilesManagementGB->setGeometry(QRect(0, 4, 1013, 433));
        QPalette palette37;
        palette37.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette37.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        palette37.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        runFilesManagementGB->setPalette(palette37);
        runFilesManagementGB->setFont(font1);
        runFilesManagementGB->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        outputFileLE = new QLineEdit(runFilesManagementGB);
        outputFileLE->setObjectName(QString::fromUtf8("outputFileLE"));
        outputFileLE->setEnabled(false);
        outputFileLE->setGeometry(QRect(79, 404, 925, 22));
        QPalette palette38;
        palette38.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette38.setBrush(QPalette::Active, QPalette::Text, brush);
        palette38.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette38.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette38.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette38.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette38.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette38.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette38.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette38.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette38.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette38.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette38.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette38.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette38.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        outputFileLE->setPalette(palette38);
        outputFileLE->setFont(font1);
        outputFileLE->setReadOnly(true);
        openNtupleFilePB = new QPushButton(runFilesManagementGB);
        openNtupleFilePB->setObjectName(QString::fromUtf8("openNtupleFilePB"));
        openNtupleFilePB->setGeometry(QRect(5, 34, 71, 53));
        QPalette palette39;
        palette39.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette39.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette39.setBrush(QPalette::Active, QPalette::Light, brush16);
        palette39.setBrush(QPalette::Active, QPalette::Midlight, brush17);
        palette39.setBrush(QPalette::Active, QPalette::Dark, brush18);
        palette39.setBrush(QPalette::Active, QPalette::Mid, brush19);
        palette39.setBrush(QPalette::Active, QPalette::Text, brush);
        palette39.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette39.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette39.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette39.setBrush(QPalette::Active, QPalette::Window, brush15);
        palette39.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette39.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette39.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette39.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette39.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette39.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette39.setBrush(QPalette::Inactive, QPalette::Light, brush16);
        palette39.setBrush(QPalette::Inactive, QPalette::Midlight, brush17);
        palette39.setBrush(QPalette::Inactive, QPalette::Dark, brush18);
        palette39.setBrush(QPalette::Inactive, QPalette::Mid, brush19);
        palette39.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette39.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette39.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette39.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette39.setBrush(QPalette::Inactive, QPalette::Window, brush15);
        palette39.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette39.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette39.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette39.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette39.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette39.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette39.setBrush(QPalette::Disabled, QPalette::Light, brush16);
        palette39.setBrush(QPalette::Disabled, QPalette::Midlight, brush17);
        palette39.setBrush(QPalette::Disabled, QPalette::Dark, brush18);
        palette39.setBrush(QPalette::Disabled, QPalette::Mid, brush19);
        palette39.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette39.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette39.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette39.setBrush(QPalette::Disabled, QPalette::Base, brush15);
        palette39.setBrush(QPalette::Disabled, QPalette::Window, brush15);
        palette39.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette39.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush15);
        palette39.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette39.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        openNtupleFilePB->setPalette(palette39);
        saveOutputFilePB = new QPushButton(runFilesManagementGB);
        saveOutputFilePB->setObjectName(QString::fromUtf8("saveOutputFilePB"));
        saveOutputFilePB->setGeometry(QRect(8, 404, 65, 21));
        QPalette palette40;
        palette40.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette40.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette40.setBrush(QPalette::Active, QPalette::Light, brush16);
        palette40.setBrush(QPalette::Active, QPalette::Midlight, brush17);
        palette40.setBrush(QPalette::Active, QPalette::Dark, brush18);
        palette40.setBrush(QPalette::Active, QPalette::Mid, brush19);
        palette40.setBrush(QPalette::Active, QPalette::Text, brush);
        palette40.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette40.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette40.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette40.setBrush(QPalette::Active, QPalette::Window, brush15);
        palette40.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette40.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette40.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette40.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette40.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette40.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette40.setBrush(QPalette::Inactive, QPalette::Light, brush16);
        palette40.setBrush(QPalette::Inactive, QPalette::Midlight, brush17);
        palette40.setBrush(QPalette::Inactive, QPalette::Dark, brush18);
        palette40.setBrush(QPalette::Inactive, QPalette::Mid, brush19);
        palette40.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette40.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette40.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette40.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette40.setBrush(QPalette::Inactive, QPalette::Window, brush15);
        palette40.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette40.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette40.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette40.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette40.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette40.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette40.setBrush(QPalette::Disabled, QPalette::Light, brush16);
        palette40.setBrush(QPalette::Disabled, QPalette::Midlight, brush17);
        palette40.setBrush(QPalette::Disabled, QPalette::Dark, brush18);
        palette40.setBrush(QPalette::Disabled, QPalette::Mid, brush19);
        palette40.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette40.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette40.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette40.setBrush(QPalette::Disabled, QPalette::Base, brush15);
        palette40.setBrush(QPalette::Disabled, QPalette::Window, brush15);
        palette40.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette40.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush15);
        palette40.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette40.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        saveOutputFilePB->setPalette(palette40);
        runFileListTV = new QTableView(runFilesManagementGB);
        runFileListTV->setObjectName(QString::fromUtf8("runFileListTV"));
        runFileListTV->setGeometry(QRect(80, 19, 925, 381));
        runFileListTV->setEditTriggers(QAbstractItemView::NoEditTriggers);
        runFileListTV->horizontalHeader()->setMinimumSectionSize(991);
        runFileListTV->verticalHeader()->setDefaultSectionSize(19);
        resetListPB = new QPushButton(runFilesManagementGB);
        resetListPB->setObjectName(QString::fromUtf8("resetListPB"));
        resetListPB->setGeometry(QRect(5, 225, 71, 31));
        QPalette palette41;
        palette41.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette41.setBrush(QPalette::Active, QPalette::Button, brush10);
        palette41.setBrush(QPalette::Active, QPalette::Light, brush11);
        palette41.setBrush(QPalette::Active, QPalette::Midlight, brush12);
        palette41.setBrush(QPalette::Active, QPalette::Dark, brush13);
        palette41.setBrush(QPalette::Active, QPalette::Mid, brush14);
        palette41.setBrush(QPalette::Active, QPalette::Text, brush);
        palette41.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette41.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette41.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette41.setBrush(QPalette::Active, QPalette::Window, brush10);
        palette41.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette41.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette41.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette41.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::Button, brush10);
        palette41.setBrush(QPalette::Inactive, QPalette::Light, brush11);
        palette41.setBrush(QPalette::Inactive, QPalette::Midlight, brush12);
        palette41.setBrush(QPalette::Inactive, QPalette::Dark, brush13);
        palette41.setBrush(QPalette::Inactive, QPalette::Mid, brush14);
        palette41.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette41.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette41.setBrush(QPalette::Inactive, QPalette::Window, brush10);
        palette41.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        palette41.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette41.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette41.setBrush(QPalette::Disabled, QPalette::WindowText, brush13);
        palette41.setBrush(QPalette::Disabled, QPalette::Button, brush10);
        palette41.setBrush(QPalette::Disabled, QPalette::Light, brush11);
        palette41.setBrush(QPalette::Disabled, QPalette::Midlight, brush12);
        palette41.setBrush(QPalette::Disabled, QPalette::Dark, brush13);
        palette41.setBrush(QPalette::Disabled, QPalette::Mid, brush14);
        palette41.setBrush(QPalette::Disabled, QPalette::Text, brush13);
        palette41.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette41.setBrush(QPalette::Disabled, QPalette::ButtonText, brush13);
        palette41.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        palette41.setBrush(QPalette::Disabled, QPalette::Window, brush10);
        palette41.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette41.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        palette41.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette41.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        resetListPB->setPalette(palette41);
        studiesTW->addTab(runT, QString());
        runFilesManagementGB->raise();
        analysesGB->raise();
        useCalibrationsCB->raise();
        dutsListLW_2->raise();
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        runScanPB = new QPushButton(tab);
        runScanPB->setObjectName(QString::fromUtf8("runScanPB"));
        runScanPB->setGeometry(QRect(924, 588, 89, 20));
        QPalette palette42;
        palette42.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette42.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette42.setBrush(QPalette::Active, QPalette::Light, brush26);
        palette42.setBrush(QPalette::Active, QPalette::Midlight, brush27);
        palette42.setBrush(QPalette::Active, QPalette::Dark, brush28);
        palette42.setBrush(QPalette::Active, QPalette::Mid, brush29);
        palette42.setBrush(QPalette::Active, QPalette::Text, brush);
        palette42.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette42.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette42.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette42.setBrush(QPalette::Active, QPalette::Window, brush30);
        palette42.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette42.setBrush(QPalette::Active, QPalette::AlternateBase, brush31);
        palette42.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette42.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette42.setBrush(QPalette::Inactive, QPalette::Light, brush26);
        palette42.setBrush(QPalette::Inactive, QPalette::Midlight, brush27);
        palette42.setBrush(QPalette::Inactive, QPalette::Dark, brush28);
        palette42.setBrush(QPalette::Inactive, QPalette::Mid, brush29);
        palette42.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette42.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette42.setBrush(QPalette::Inactive, QPalette::Window, brush30);
        palette42.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush31);
        palette42.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette42.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette42.setBrush(QPalette::Disabled, QPalette::WindowText, brush28);
        palette42.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette42.setBrush(QPalette::Disabled, QPalette::Light, brush26);
        palette42.setBrush(QPalette::Disabled, QPalette::Midlight, brush27);
        palette42.setBrush(QPalette::Disabled, QPalette::Dark, brush28);
        palette42.setBrush(QPalette::Disabled, QPalette::Mid, brush29);
        palette42.setBrush(QPalette::Disabled, QPalette::Text, brush28);
        palette42.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette42.setBrush(QPalette::Disabled, QPalette::ButtonText, brush28);
        palette42.setBrush(QPalette::Disabled, QPalette::Base, brush30);
        palette42.setBrush(QPalette::Disabled, QPalette::Window, brush30);
        palette42.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette42.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush30);
        palette42.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette42.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        runScanPB->setPalette(palette42);
        runScanPB->setFont(font1);
        loadFileToScanListPB = new QPushButton(tab);
        loadFileToScanListPB->setObjectName(QString::fromUtf8("loadFileToScanListPB"));
        loadFileToScanListPB->setGeometry(QRect(112, 4, 141, 26));
        QPalette palette43;
        palette43.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette43.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette43.setBrush(QPalette::Active, QPalette::Light, brush46);
        palette43.setBrush(QPalette::Active, QPalette::Midlight, brush47);
        palette43.setBrush(QPalette::Active, QPalette::Dark, brush48);
        palette43.setBrush(QPalette::Active, QPalette::Mid, brush49);
        palette43.setBrush(QPalette::Active, QPalette::Text, brush);
        palette43.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette43.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette43.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette43.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette43.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette43.setBrush(QPalette::Active, QPalette::AlternateBase, brush51);
        palette43.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette43.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette43.setBrush(QPalette::Inactive, QPalette::Light, brush46);
        palette43.setBrush(QPalette::Inactive, QPalette::Midlight, brush47);
        palette43.setBrush(QPalette::Inactive, QPalette::Dark, brush48);
        palette43.setBrush(QPalette::Inactive, QPalette::Mid, brush49);
        palette43.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette43.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette43.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette43.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush51);
        palette43.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette43.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette43.setBrush(QPalette::Disabled, QPalette::WindowText, brush48);
        palette43.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette43.setBrush(QPalette::Disabled, QPalette::Light, brush46);
        palette43.setBrush(QPalette::Disabled, QPalette::Midlight, brush47);
        palette43.setBrush(QPalette::Disabled, QPalette::Dark, brush48);
        palette43.setBrush(QPalette::Disabled, QPalette::Mid, brush49);
        palette43.setBrush(QPalette::Disabled, QPalette::Text, brush48);
        palette43.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette43.setBrush(QPalette::Disabled, QPalette::ButtonText, brush48);
        palette43.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette43.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        palette43.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette43.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush35);
        palette43.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette43.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        loadFileToScanListPB->setPalette(palette43);
        scanListTW = new QTableWidget(tab);
        if (scanListTW->columnCount() < 2)
            scanListTW->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        scanListTW->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        scanListTW->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        scanListTW->setObjectName(QString::fromUtf8("scanListTW"));
        scanListTW->setGeometry(QRect(4, 32, 1009, 549));
        resetScanListPB = new QPushButton(tab);
        resetScanListPB->setObjectName(QString::fromUtf8("resetScanListPB"));
        resetScanListPB->setGeometry(QRect(4, 4, 105, 26));
        QPalette palette44;
        palette44.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette44.setBrush(QPalette::Active, QPalette::Button, brush10);
        palette44.setBrush(QPalette::Active, QPalette::Light, brush46);
        palette44.setBrush(QPalette::Active, QPalette::Midlight, brush47);
        palette44.setBrush(QPalette::Active, QPalette::Dark, brush48);
        palette44.setBrush(QPalette::Active, QPalette::Mid, brush49);
        palette44.setBrush(QPalette::Active, QPalette::Text, brush);
        palette44.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette44.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette44.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette44.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette44.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette44.setBrush(QPalette::Active, QPalette::AlternateBase, brush51);
        palette44.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette44.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::Button, brush10);
        palette44.setBrush(QPalette::Inactive, QPalette::Light, brush46);
        palette44.setBrush(QPalette::Inactive, QPalette::Midlight, brush47);
        palette44.setBrush(QPalette::Inactive, QPalette::Dark, brush48);
        palette44.setBrush(QPalette::Inactive, QPalette::Mid, brush49);
        palette44.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette44.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette44.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette44.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush51);
        palette44.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette44.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette44.setBrush(QPalette::Disabled, QPalette::WindowText, brush48);
        palette44.setBrush(QPalette::Disabled, QPalette::Button, brush10);
        palette44.setBrush(QPalette::Disabled, QPalette::Light, brush46);
        palette44.setBrush(QPalette::Disabled, QPalette::Midlight, brush47);
        palette44.setBrush(QPalette::Disabled, QPalette::Dark, brush48);
        palette44.setBrush(QPalette::Disabled, QPalette::Mid, brush49);
        palette44.setBrush(QPalette::Disabled, QPalette::Text, brush48);
        palette44.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette44.setBrush(QPalette::Disabled, QPalette::ButtonText, brush48);
        palette44.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette44.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        palette44.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette44.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush35);
        palette44.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette44.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        resetScanListPB->setPalette(palette44);
        studiesTW->addTab(tab, QString());
        tab8 = new QWidget();
        tab8->setObjectName(QString::fromUtf8("tab8"));
        pixelMatrixW = new PixelMatrix(tab8);
        pixelMatrixW->setObjectName(QString::fromUtf8("pixelMatrixW"));
        pixelMatrixW->setGeometry(QRect(4, 12, 1001, 589));
        studiesTW->addTab(tab8, QString());
        cellT = new QWidget();
        cellT->setObjectName(QString::fromUtf8("cellT"));
        pixelCutMatrixW = new PixelMatrixCut(cellT);
        pixelCutMatrixW->setObjectName(QString::fromUtf8("pixelCutMatrixW"));
        pixelCutMatrixW->setGeometry(QRect(4, 4, 757, 605));
        cutVarLW = new QListWidget(cellT);
        cutVarLW->setObjectName(QString::fromUtf8("cutVarLW"));
        cutVarLW->setGeometry(QRect(764, 92, 245, 81));
        cutVarLW->setResizeMode(QListView::Adjust);
        cutFileLE = new QLineEdit(cellT);
        cutFileLE->setObjectName(QString::fromUtf8("cutFileLE"));
        cutFileLE->setGeometry(QRect(764, 36, 245, 27));
        cutFileLE->setReadOnly(true);
        cutFilePB = new QPushButton(cellT);
        cutFilePB->setObjectName(QString::fromUtf8("cutFilePB"));
        cutFilePB->setGeometry(QRect(922, 4, 89, 20));
        QPalette palette45;
        palette45.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette45.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette45.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        cutFilePB->setPalette(palette45);
        label_7 = new QLabel(cellT);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(828, 8, 85, 17));
        label_8 = new QLabel(cellT);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(764, 72, 191, 17));
        groupBox = new QGroupBox(cellT);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(764, 180, 245, 33));
        cutDut0RB = new QRadioButton(groupBox);
        cutDut0RB->setObjectName(QString::fromUtf8("cutDut0RB"));
        cutDut0RB->setGeometry(QRect(10, 4, 71, 22));
        cutDut0RB->setChecked(true);
        cutDut1RB = new QRadioButton(groupBox);
        cutDut1RB->setObjectName(QString::fromUtf8("cutDut1RB"));
        cutDut1RB->setGeometry(QRect(80, 4, 71, 22));
        cutDescriptionTE = new QTextEdit(cellT);
        cutDescriptionTE->setObjectName(QString::fromUtf8("cutDescriptionTE"));
        cutDescriptionTE->setGeometry(QRect(764, 220, 245, 385));
        cutDescriptionTE->setReadOnly(true);
        studiesTW->addTab(cellT, QString());
        progressBar = new QProgressBar(AnalyzerDlg);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(140, 30, 629, 21));
        progressBar->setValue(0);
        abortPB = new QPushButton(AnalyzerDlg);
        abortPB->setObjectName(QString::fromUtf8("abortPB"));
        abortPB->setGeometry(QRect(10, 30, 121, 21));
        QPalette palette46;
        palette46.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette46.setBrush(QPalette::Active, QPalette::Button, brush10);
        palette46.setBrush(QPalette::Active, QPalette::Light, brush11);
        palette46.setBrush(QPalette::Active, QPalette::Midlight, brush12);
        palette46.setBrush(QPalette::Active, QPalette::Dark, brush13);
        palette46.setBrush(QPalette::Active, QPalette::Mid, brush14);
        palette46.setBrush(QPalette::Active, QPalette::Text, brush);
        palette46.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette46.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette46.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette46.setBrush(QPalette::Active, QPalette::Window, brush10);
        palette46.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette46.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette46.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette46.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette46.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette46.setBrush(QPalette::Inactive, QPalette::Button, brush10);
        palette46.setBrush(QPalette::Inactive, QPalette::Light, brush11);
        palette46.setBrush(QPalette::Inactive, QPalette::Midlight, brush12);
        palette46.setBrush(QPalette::Inactive, QPalette::Dark, brush13);
        palette46.setBrush(QPalette::Inactive, QPalette::Mid, brush14);
        palette46.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette46.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette46.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette46.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette46.setBrush(QPalette::Inactive, QPalette::Window, brush10);
        palette46.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette46.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        palette46.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette46.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette46.setBrush(QPalette::Disabled, QPalette::WindowText, brush13);
        palette46.setBrush(QPalette::Disabled, QPalette::Button, brush10);
        palette46.setBrush(QPalette::Disabled, QPalette::Light, brush11);
        palette46.setBrush(QPalette::Disabled, QPalette::Midlight, brush12);
        palette46.setBrush(QPalette::Disabled, QPalette::Dark, brush13);
        palette46.setBrush(QPalette::Disabled, QPalette::Mid, brush14);
        palette46.setBrush(QPalette::Disabled, QPalette::Text, brush13);
        palette46.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette46.setBrush(QPalette::Disabled, QPalette::ButtonText, brush13);
        palette46.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        palette46.setBrush(QPalette::Disabled, QPalette::Window, brush10);
        palette46.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette46.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        palette46.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette46.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        abortPB->setPalette(palette46);
        abortPB->setFont(font3);
        openConfigurationFilePB = new QPushButton(AnalyzerDlg);
        openConfigurationFilePB->setObjectName(QString::fromUtf8("openConfigurationFilePB"));
        openConfigurationFilePB->setGeometry(QRect(10, 5, 121, 22));
        QPalette palette47;
        palette47.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette47.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette47.setBrush(QPalette::Active, QPalette::Light, brush16);
        palette47.setBrush(QPalette::Active, QPalette::Midlight, brush17);
        palette47.setBrush(QPalette::Active, QPalette::Dark, brush18);
        palette47.setBrush(QPalette::Active, QPalette::Mid, brush19);
        palette47.setBrush(QPalette::Active, QPalette::Text, brush);
        palette47.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette47.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette47.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette47.setBrush(QPalette::Active, QPalette::Window, brush15);
        palette47.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette47.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette47.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette47.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette47.setBrush(QPalette::Inactive, QPalette::Light, brush16);
        palette47.setBrush(QPalette::Inactive, QPalette::Midlight, brush17);
        palette47.setBrush(QPalette::Inactive, QPalette::Dark, brush18);
        palette47.setBrush(QPalette::Inactive, QPalette::Mid, brush19);
        palette47.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette47.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette47.setBrush(QPalette::Inactive, QPalette::Window, brush15);
        palette47.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette47.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette47.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette47.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette47.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        palette47.setBrush(QPalette::Disabled, QPalette::Light, brush16);
        palette47.setBrush(QPalette::Disabled, QPalette::Midlight, brush17);
        palette47.setBrush(QPalette::Disabled, QPalette::Dark, brush18);
        palette47.setBrush(QPalette::Disabled, QPalette::Mid, brush19);
        palette47.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette47.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette47.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette47.setBrush(QPalette::Disabled, QPalette::Base, brush15);
        palette47.setBrush(QPalette::Disabled, QPalette::Window, brush15);
        palette47.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette47.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush15);
        palette47.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette47.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        openConfigurationFilePB->setPalette(palette47);
        loadedConfigurationFileLE = new QLineEdit(AnalyzerDlg);
        loadedConfigurationFileLE->setObjectName(QString::fromUtf8("loadedConfigurationFileLE"));
        loadedConfigurationFileLE->setEnabled(false);
        loadedConfigurationFileLE->setGeometry(QRect(140, 5, 765, 22));
        QPalette palette48;
        palette48.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette48.setBrush(QPalette::Active, QPalette::Text, brush);
        palette48.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette48.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette48.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette48.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette48.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette48.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette48.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette48.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette48.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette48.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette48.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette48.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette48.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        loadedConfigurationFileLE->setPalette(palette48);
        loadedConfigurationFileLE->setFont(font1);
        loadedConfigurationFileLE->setReadOnly(true);
        saveConfigurationFile = new QPushButton(AnalyzerDlg);
        saveConfigurationFile->setObjectName(QString::fromUtf8("saveConfigurationFile"));
        saveConfigurationFile->setGeometry(QRect(915, 4, 105, 22));
        QPalette palette49;
        palette49.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette49.setBrush(QPalette::Active, QPalette::Button, brush35);
        palette49.setBrush(QPalette::Active, QPalette::Light, brush46);
        palette49.setBrush(QPalette::Active, QPalette::Midlight, brush47);
        palette49.setBrush(QPalette::Active, QPalette::Dark, brush48);
        palette49.setBrush(QPalette::Active, QPalette::Mid, brush49);
        palette49.setBrush(QPalette::Active, QPalette::Text, brush);
        palette49.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette49.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette49.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette49.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette49.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette49.setBrush(QPalette::Active, QPalette::AlternateBase, brush51);
        palette49.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette49.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette49.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette49.setBrush(QPalette::Inactive, QPalette::Button, brush35);
        palette49.setBrush(QPalette::Inactive, QPalette::Light, brush46);
        palette49.setBrush(QPalette::Inactive, QPalette::Midlight, brush47);
        palette49.setBrush(QPalette::Inactive, QPalette::Dark, brush48);
        palette49.setBrush(QPalette::Inactive, QPalette::Mid, brush49);
        palette49.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette49.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette49.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette49.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette49.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette49.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette49.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush51);
        palette49.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette49.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette49.setBrush(QPalette::Disabled, QPalette::WindowText, brush48);
        palette49.setBrush(QPalette::Disabled, QPalette::Button, brush35);
        palette49.setBrush(QPalette::Disabled, QPalette::Light, brush46);
        palette49.setBrush(QPalette::Disabled, QPalette::Midlight, brush47);
        palette49.setBrush(QPalette::Disabled, QPalette::Dark, brush48);
        palette49.setBrush(QPalette::Disabled, QPalette::Mid, brush49);
        palette49.setBrush(QPalette::Disabled, QPalette::Text, brush48);
        palette49.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette49.setBrush(QPalette::Disabled, QPalette::ButtonText, brush48);
        palette49.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette49.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        palette49.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette49.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush35);
        palette49.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette49.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        saveConfigurationFile->setPalette(palette49);
        runningThreadsLB = new QLabel(AnalyzerDlg);
        runningThreadsLB->setObjectName(QString::fromUtf8("runningThreadsLB"));
        runningThreadsLB->setEnabled(true);
        runningThreadsLB->setGeometry(QRect(848, 52, 133, 21));
        runningThreadsLB->setFont(font1);
        runningThreadsLE = new QLineEdit(AnalyzerDlg);
        runningThreadsLE->setObjectName(QString::fromUtf8("runningThreadsLE"));
        runningThreadsLE->setGeometry(QRect(988, 52, 31, 21));
        runningThreadsLE->setFont(font1);
        runningThreadsLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        runningThreadsLE->setReadOnly(true);
        processedEventsLB = new QLabel(AnalyzerDlg);
        processedEventsLB->setObjectName(QString::fromUtf8("processedEventsLB"));
        processedEventsLB->setEnabled(true);
        processedEventsLB->setGeometry(QRect(776, 32, 137, 21));
        processedEventsLB->setFont(font1);
        processedEventsLB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        processedEventsLE = new QLineEdit(AnalyzerDlg);
        processedEventsLE->setObjectName(QString::fromUtf8("processedEventsLE"));
        processedEventsLE->setGeometry(QRect(924, 30, 96, 21));
        QPalette palette50;
        palette50.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette50.setBrush(QPalette::Active, QPalette::Text, brush20);
        palette50.setBrush(QPalette::Active, QPalette::BrightText, brush20);
        palette50.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette50.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette50.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette50.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette50.setBrush(QPalette::Inactive, QPalette::Text, brush20);
        palette50.setBrush(QPalette::Inactive, QPalette::BrightText, brush20);
        palette50.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette50.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette50.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette50.setBrush(QPalette::Disabled, QPalette::WindowText, brush20);
        palette50.setBrush(QPalette::Disabled, QPalette::Text, brush20);
        palette50.setBrush(QPalette::Disabled, QPalette::BrightText, brush20);
        palette50.setBrush(QPalette::Disabled, QPalette::ButtonText, brush20);
        palette50.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette50.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        processedEventsLE->setPalette(palette50);
        processedEventsLE->setFont(font2);
        processedEventsLE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        processedEventsLE->setReadOnly(true);
        activityLB = new QLabel(AnalyzerDlg);
        activityLB->setObjectName(QString::fromUtf8("activityLB"));
        activityLB->setGeometry(QRect(140, 52, 689, 21));
        QPalette palette51;
        palette51.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette51.setBrush(QPalette::Active, QPalette::Text, brush10);
        palette51.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette51.setBrush(QPalette::Inactive, QPalette::Text, brush10);
        QBrush brush52(QColor(106, 104, 100, 255));
        brush52.setStyle(Qt::SolidPattern);
        palette51.setBrush(QPalette::Disabled, QPalette::WindowText, brush52);
        palette51.setBrush(QPalette::Disabled, QPalette::Text, brush52);
        activityLB->setPalette(palette51);
        activityLB->setFont(font1);

        retranslateUi(AnalyzerDlg);
        QObject::connect(efficiencyMainCutsCB, SIGNAL(clicked(bool)), efficiencyMainCutsLE, SLOT(setEnabled(bool)));
        QObject::connect(efficiencyCuts2CB, SIGNAL(clicked(bool)), efficiencyCuts2LE, SLOT(setEnabled(bool)));
        QObject::connect(efficiencyCuts3CB, SIGNAL(clicked(bool)), efficiencyCuts3LE, SLOT(setEnabled(bool)));
        QObject::connect(efficiencyCuts4CB, SIGNAL(clicked(bool)), efficiencyCuts4LE, SLOT(setEnabled(bool)));
        QObject::connect(chargeMainCutsCB, SIGNAL(clicked(bool)), chargeMainCutsLE, SLOT(setEnabled(bool)));
        QObject::connect(chargeCuts5CB, SIGNAL(clicked(bool)), chargeCuts5LE, SLOT(setEnabled(bool)));
        QObject::connect(chargeCuts6CB, SIGNAL(clicked(bool)), chargeCuts6LE, SLOT(setEnabled(bool)));
        QObject::connect(chargeCuts2CB, SIGNAL(clicked(bool)), chargeCuts2LE, SLOT(setEnabled(bool)));
        QObject::connect(chargeCuts3CB, SIGNAL(clicked(bool)), chargeCuts3LE, SLOT(setEnabled(bool)));
        QObject::connect(chargeCuts4CB, SIGNAL(clicked(bool)), chargeCuts4LE, SLOT(setEnabled(bool)));
        QObject::connect(resolutionMainCutsCB, SIGNAL(clicked(bool)), resolutionMainCutsLE, SLOT(setEnabled(bool)));
        QObject::connect(resolutionCuts2CB, SIGNAL(clicked(bool)), resolutionCuts2LE, SLOT(setEnabled(bool)));
        QObject::connect(resolutionCuts3CB, SIGNAL(clicked(bool)), resolutionCuts3LE, SLOT(setEnabled(bool)));
        QObject::connect(resolutionCuts4CB, SIGNAL(clicked(bool)), resolutionCuts4LE, SLOT(setEnabled(bool)));
        QObject::connect(par1minCB, SIGNAL(clicked(bool)), par1minLE, SLOT(setEnabled(bool)));
        QObject::connect(par1maxCB, SIGNAL(clicked(bool)), par1maxLE, SLOT(setEnabled(bool)));
        QObject::connect(par0minCB, SIGNAL(clicked(bool)), par0minLE, SLOT(setEnabled(bool)));
        QObject::connect(par0maxCB, SIGNAL(clicked(bool)), par0maxLE, SLOT(setEnabled(bool)));
        QObject::connect(par2minCB, SIGNAL(clicked(bool)), par2minLE, SLOT(setEnabled(bool)));
        QObject::connect(par2maxCB, SIGNAL(clicked(bool)), par2maxLE, SLOT(setEnabled(bool)));
        QObject::connect(par3minCB, SIGNAL(clicked(bool)), par3minLE, SLOT(setEnabled(bool)));
        QObject::connect(par3maxCB, SIGNAL(clicked(bool)), par3maxLE, SLOT(setEnabled(bool)));

        studiesTW->setCurrentIndex(7);
        dutsPreAnalysisTW->setCurrentIndex(1);
        parsTW->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AnalyzerDlg);
    } // setupUi

    void retranslateUi(QDialog *AnalyzerDlg)
    {
        AnalyzerDlg->setWindowTitle(QApplication::translate("AnalyzerDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        runFilesManagementGB_2->setTitle(QApplication::translate("AnalyzerDlg", "File(s) management", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        convertResetListPB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        convertResetListPB->setText(QApplication::translate("AnalyzerDlg", "Reset\n"
"list", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openRootFilePB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        openRootFilePB->setText(QApplication::translate("AnalyzerDlg", "Load\n"
"Monicelli\n"
"file(s)", 0, QApplication::UnicodeUTF8));
        convertConverterControlsGB->setTitle(QApplication::translate("AnalyzerDlg", "Converter Controls", 0, QApplication::UnicodeUTF8));
        convertTotalEventsLB->setText(QApplication::translate("AnalyzerDlg", "Total Events:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        convertTotalEventsLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Events</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Sho"
                        "w the number of event </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">objects found in input file.</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        convertTotalEventsLE->setText(QString());
        runConverterPB->setText(QApplication::translate("AnalyzerDlg", "Convert", 0, QApplication::UnicodeUTF8));
        convertMaxThreadsLB->setText(QApplication::translate("AnalyzerDlg", "Max Threads:", 0, QApplication::UnicodeUTF8));
        convertMaxEventsCB->setText(QApplication::translate("AnalyzerDlg", "Max Events:", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(converterT), QApplication::translate("AnalyzerDlg", "Convert", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = detectorsInfoTW->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AnalyzerDlg", "Id", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = detectorsInfoTW->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AnalyzerDlg", "Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = detectorsInfoTW->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("AnalyzerDlg", "Calibration", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = detectorsInfoTW->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("AnalyzerDlg", "XRocs", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = detectorsInfoTW->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("AnalyzerDlg", "YRocs", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = detectorsInfoTW->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("AnalyzerDlg", "XPitch", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = detectorsInfoTW->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("AnalyzerDlg", "YPitch", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = detectorsInfoTW->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("AnalyzerDlg", "ZPosition", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = detectorsInfoTW->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("AnalyzerDlg", "ZRotation", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(infosT), QApplication::translate("AnalyzerDlg", "Infos", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        makeCalibPlotsPB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        makeCalibPlotsPB->setText(QApplication::translate("AnalyzerDlg", "Make Plots", 0, QApplication::UnicodeUTF8));
        calibPlotsGB->setTitle(QApplication::translate("AnalyzerDlg", "Calibration Plots", 0, QApplication::UnicodeUTF8));
        calibParsCB->setText(QApplication::translate("AnalyzerDlg", "Parameters", 0, QApplication::UnicodeUTF8));
        calibDispersionCB->setText(QApplication::translate("AnalyzerDlg", "Dispersion", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("AnalyzerDlg", "I/O files", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        calibPlotsFileLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        calibPlotsFileLE->setText(QApplication::translate("AnalyzerDlg", "Output file with calibration plots (.root)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        loadedGeoFileLE_2->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        loadedGeoFileLE_2->setText(QApplication::translate("AnalyzerDlg", "Load geometry file (.geo)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openCalibrationPlotsPB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        openCalibrationPlotsPB->setText(QApplication::translate("AnalyzerDlg", "Load calibration plots", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        loadedCalibFileLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        loadedCalibFileLE->setText(QApplication::translate("AnalyzerDlg", "Load calibration file (.txt)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openCalibrationsFilePB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        openCalibrationsFilePB->setText(QApplication::translate("AnalyzerDlg", "Load calibrations", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        calibrationLoadGeoFilePB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        calibrationLoadGeoFilePB->setText(QApplication::translate("AnalyzerDlg", "Load geo file", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(calibrationT), QApplication::translate("AnalyzerDlg", "Calibrations", 0, QApplication::UnicodeUTF8));
        limitWindowLB->setText(QApplication::translate("AnalyzerDlg", "Limit:", 0, QApplication::UnicodeUTF8));
        removePixelDUT0GB->setTitle(QApplication::translate("AnalyzerDlg", "Remove Pixels", 0, QApplication::UnicodeUTF8));
        removeRowDUT0LB->setText(QApplication::translate("AnalyzerDlg", "Row:", 0, QApplication::UnicodeUTF8));
        removeColDUT0LB->setText(QApplication::translate("AnalyzerDlg", "Col:", 0, QApplication::UnicodeUTF8));
        removePxlDUT0PB->setText(QApplication::translate("AnalyzerDlg", "Remove", 0, QApplication::UnicodeUTF8));
        resetRemovedPxlsDUT0PB->setText(QApplication::translate("AnalyzerDlg", "Reset", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("AnalyzerDlg", "Windows", 0, QApplication::UnicodeUTF8));
        row0LB->setText(QApplication::translate("AnalyzerDlg", "Row:", 0, QApplication::UnicodeUTF8));
        col0LB->setText(QApplication::translate("AnalyzerDlg", "Col:", 0, QApplication::UnicodeUTF8));
        higherEdge0LB->setText(QApplication::translate("AnalyzerDlg", "Higher:", 0, QApplication::UnicodeUTF8));
        lowerEdge0LB->setText(QApplication::translate("AnalyzerDlg", "Lower:", 0, QApplication::UnicodeUTF8));
        dutsPreAnalysisTW->setTabText(dutsPreAnalysisTW->indexOf(tab_6), QApplication::translate("AnalyzerDlg", "DUT0", 0, QApplication::UnicodeUTF8));
        removePixelDUT0GB_2->setTitle(QApplication::translate("AnalyzerDlg", "Remove Pixels", 0, QApplication::UnicodeUTF8));
        removeRowDUT0LB_2->setText(QApplication::translate("AnalyzerDlg", "Row:", 0, QApplication::UnicodeUTF8));
        removeColDUT0LB_2->setText(QApplication::translate("AnalyzerDlg", "Col:", 0, QApplication::UnicodeUTF8));
        removePxlDUT1PB->setText(QApplication::translate("AnalyzerDlg", "Remove", 0, QApplication::UnicodeUTF8));
        resetRemovedPxlsDUT1PB->setText(QApplication::translate("AnalyzerDlg", "Reset", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("AnalyzerDlg", "Windows", 0, QApplication::UnicodeUTF8));
        row0LB_2->setText(QApplication::translate("AnalyzerDlg", "Row:", 0, QApplication::UnicodeUTF8));
        col0LB_2->setText(QApplication::translate("AnalyzerDlg", "Col:", 0, QApplication::UnicodeUTF8));
        higherEdge0LB_2->setText(QApplication::translate("AnalyzerDlg", "Higher:", 0, QApplication::UnicodeUTF8));
        lowerEdge0LB_2->setText(QApplication::translate("AnalyzerDlg", "Lower:", 0, QApplication::UnicodeUTF8));
        dutsPreAnalysisTW->setTabText(dutsPreAnalysisTW->indexOf(tab_7), QApplication::translate("AnalyzerDlg", "DUT1", 0, QApplication::UnicodeUTF8));
        removePixelDUT0GB_3->setTitle(QApplication::translate("AnalyzerDlg", "Remove Pixels", 0, QApplication::UnicodeUTF8));
        removeRowDUT0LB_3->setText(QApplication::translate("AnalyzerDlg", "Row:", 0, QApplication::UnicodeUTF8));
        removeColDUT0LB_3->setText(QApplication::translate("AnalyzerDlg", "Col:", 0, QApplication::UnicodeUTF8));
        removePxlDUT2PB->setText(QApplication::translate("AnalyzerDlg", "Remove", 0, QApplication::UnicodeUTF8));
        resetRemovedPxlsDUT2PB->setText(QApplication::translate("AnalyzerDlg", "Reset", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("AnalyzerDlg", "Windows", 0, QApplication::UnicodeUTF8));
        row0LB_3->setText(QApplication::translate("AnalyzerDlg", "Row:", 0, QApplication::UnicodeUTF8));
        col0LB_3->setText(QApplication::translate("AnalyzerDlg", "Col:", 0, QApplication::UnicodeUTF8));
        higherEdge0LB_3->setText(QApplication::translate("AnalyzerDlg", "Higher:", 0, QApplication::UnicodeUTF8));
        lowerEdge0LB_3->setText(QApplication::translate("AnalyzerDlg", "Lower:", 0, QApplication::UnicodeUTF8));
        dutsPreAnalysisTW->setTabText(dutsPreAnalysisTW->indexOf(tab_8), QApplication::translate("AnalyzerDlg", "DUT2", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("AnalyzerDlg", "Windows", 0, QApplication::UnicodeUTF8));
        row0LB_4->setText(QApplication::translate("AnalyzerDlg", "Row:", 0, QApplication::UnicodeUTF8));
        col0LB_4->setText(QApplication::translate("AnalyzerDlg", "Col:", 0, QApplication::UnicodeUTF8));
        higherEdge0LB_4->setText(QApplication::translate("AnalyzerDlg", "Higher:", 0, QApplication::UnicodeUTF8));
        lowerEdge0LB_4->setText(QApplication::translate("AnalyzerDlg", "Lower:", 0, QApplication::UnicodeUTF8));
        removePixelDUT0GB_4->setTitle(QApplication::translate("AnalyzerDlg", "Remove Pixels", 0, QApplication::UnicodeUTF8));
        removeRowDUT0LB_4->setText(QApplication::translate("AnalyzerDlg", "Row:", 0, QApplication::UnicodeUTF8));
        removeColDUT0LB_4->setText(QApplication::translate("AnalyzerDlg", "Col:", 0, QApplication::UnicodeUTF8));
        removePxlDUT3PB->setText(QApplication::translate("AnalyzerDlg", "Remove", 0, QApplication::UnicodeUTF8));
        resetRemovedPxlsDUT3PB->setText(QApplication::translate("AnalyzerDlg", "Reset", 0, QApplication::UnicodeUTF8));
        dutsPreAnalysisTW->setTabText(dutsPreAnalysisTW->indexOf(tab_9), QApplication::translate("AnalyzerDlg", "DUT3", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(windowsT), QApplication::translate("AnalyzerDlg", "Windows", 0, QApplication::UnicodeUTF8));
        efficiencyCutsGB->setTitle(QApplication::translate("AnalyzerDlg", "Cuts", 0, QApplication::UnicodeUTF8));
        chi2EfficiencyCutCB->setText(QApplication::translate("AnalyzerDlg", "chi2/DOF", 0, QApplication::UnicodeUTF8));
        nTrackEfficiencyCutCB->setText(QApplication::translate("AnalyzerDlg", "Number of tracks", 0, QApplication::UnicodeUTF8));
        yErrEfficiencyCutCB->setText(QApplication::translate("AnalyzerDlg", "Y predicted error", 0, QApplication::UnicodeUTF8));
        xErrEffLB->setText(QApplication::translate("AnalyzerDlg", "um", 0, QApplication::UnicodeUTF8));
        xErrEfficiencyCutCB->setText(QApplication::translate("AnalyzerDlg", "X predicted error", 0, QApplication::UnicodeUTF8));
        xErrEffCutLB->setText(QApplication::translate("AnalyzerDlg", "um", 0, QApplication::UnicodeUTF8));
        xEfficiencyCutCB->setText(QApplication::translate("AnalyzerDlg", "X residual", 0, QApplication::UnicodeUTF8));
        yEfficiencyCutCB->setText(QApplication::translate("AnalyzerDlg", "Y residual", 0, QApplication::UnicodeUTF8));
        hitsEfficiencyCutCB->setText(QApplication::translate("AnalyzerDlg", "Min hits", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("AnalyzerDlg", "see Charge", 0, QApplication::UnicodeUTF8));
        efficiencyStandardCutsCB->setText(QApplication::translate("AnalyzerDlg", "Standard cuts", 0, QApplication::UnicodeUTF8));
        efficiencyExcludeBadPlanesCB->setText(QApplication::translate("AnalyzerDlg", "Exclude bad working planes:", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("AnalyzerDlg", "Expressions", 0, QApplication::UnicodeUTF8));
        efficiencyCuts2CB->setText(QApplication::translate("AnalyzerDlg", "Cell efficiency", 0, QApplication::UnicodeUTF8));
        efficiencyCuts3CB->setText(QApplication::translate("AnalyzerDlg", "Cell efficiency X", 0, QApplication::UnicodeUTF8));
        efficiencyCuts4CB->setText(QApplication::translate("AnalyzerDlg", "Cell efficiency Y", 0, QApplication::UnicodeUTF8));
        efficiencyMainCutsCB->setText(QApplication::translate("AnalyzerDlg", "Main cuts", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(efficiencyT), QApplication::translate("AnalyzerDlg", "Efficiency", 0, QApplication::UnicodeUTF8));
        chargeCutsGB->setTitle(QApplication::translate("AnalyzerDlg", "Cuts", 0, QApplication::UnicodeUTF8));
        chi2ChargeCutCB->setText(QApplication::translate("AnalyzerDlg", "chi2/ndof", 0, QApplication::UnicodeUTF8));
        nTrackChargeCutCB->setText(QApplication::translate("AnalyzerDlg", "Number of tracks", 0, QApplication::UnicodeUTF8));
        xErrChargeCutCB->setText(QApplication::translate("AnalyzerDlg", "X predicted error", 0, QApplication::UnicodeUTF8));
        yErrChargeCutCB->setText(QApplication::translate("AnalyzerDlg", "Y predicted error", 0, QApplication::UnicodeUTF8));
        xErrChargeLB->setText(QApplication::translate("AnalyzerDlg", "um", 0, QApplication::UnicodeUTF8));
        yErrChargeLB->setText(QApplication::translate("AnalyzerDlg", "um", 0, QApplication::UnicodeUTF8));
        xChargeCutCB->setText(QApplication::translate("AnalyzerDlg", "X residual", 0, QApplication::UnicodeUTF8));
        yChargeCutCB->setText(QApplication::translate("AnalyzerDlg", "Y residual", 0, QApplication::UnicodeUTF8));
        xChargeLB->setText(QApplication::translate("AnalyzerDlg", "um", 0, QApplication::UnicodeUTF8));
        yChargeLB->setText(QApplication::translate("AnalyzerDlg", "um", 0, QApplication::UnicodeUTF8));
        hitsChargeCutCB->setText(QApplication::translate("AnalyzerDlg", "Min hits", 0, QApplication::UnicodeUTF8));
        chargePixelMinimumChargeElectronLB->setText(QApplication::translate("AnalyzerDlg", "e-", 0, QApplication::UnicodeUTF8));
        chargeClusterMaximumChargeElectronLB->setText(QApplication::translate("AnalyzerDlg", "e-", 0, QApplication::UnicodeUTF8));
        chargeClusterMinimumChargeElectronLB->setText(QApplication::translate("AnalyzerDlg", "e-", 0, QApplication::UnicodeUTF8));
        chargePixelMaximumChargeElectronLB->setText(QApplication::translate("AnalyzerDlg", "e-", 0, QApplication::UnicodeUTF8));
        chargePixelMinimumChargeLB->setText(QApplication::translate("AnalyzerDlg", "Pixel Min Charge", 0, QApplication::UnicodeUTF8));
        chargePixelMaximumChargeLB->setText(QApplication::translate("AnalyzerDlg", "Pixel Max Charge", 0, QApplication::UnicodeUTF8));
        chargeClusterMinimumChargeLB->setText(QApplication::translate("AnalyzerDlg", "Cluster Min Charge", 0, QApplication::UnicodeUTF8));
        chargeClusterMaximumChargeLB->setText(QApplication::translate("AnalyzerDlg", "Cluster Max Charge", 0, QApplication::UnicodeUTF8));
        chargeExcludeBadPlanesCB->setText(QApplication::translate("AnalyzerDlg", "Exclude bad working planes:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AnalyzerDlg", "With less than", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AnalyzerDlg", "% of hits", 0, QApplication::UnicodeUTF8));
        chargeStandardCutsCB->setText(QApplication::translate("AnalyzerDlg", "Standard cuts", 0, QApplication::UnicodeUTF8));
        parsCutLB->setText(QApplication::translate("AnalyzerDlg", "Calibrations parameters cut:", 0, QApplication::UnicodeUTF8));
        par0minCB->setText(QApplication::translate("AnalyzerDlg", "min", 0, QApplication::UnicodeUTF8));
        par0maxCB->setText(QApplication::translate("AnalyzerDlg", "max", 0, QApplication::UnicodeUTF8));
        parsTW->setTabText(parsTW->indexOf(tab_2), QApplication::translate("AnalyzerDlg", "Par 0", 0, QApplication::UnicodeUTF8));
        par1minCB->setText(QApplication::translate("AnalyzerDlg", "min", 0, QApplication::UnicodeUTF8));
        par1maxCB->setText(QApplication::translate("AnalyzerDlg", "max", 0, QApplication::UnicodeUTF8));
        parsTW->setTabText(parsTW->indexOf(tab_3), QApplication::translate("AnalyzerDlg", "Par 1", 0, QApplication::UnicodeUTF8));
        par2minCB->setText(QApplication::translate("AnalyzerDlg", "min", 0, QApplication::UnicodeUTF8));
        par2maxCB->setText(QApplication::translate("AnalyzerDlg", "max", 0, QApplication::UnicodeUTF8));
        parsTW->setTabText(parsTW->indexOf(tab_4), QApplication::translate("AnalyzerDlg", "Par 2", 0, QApplication::UnicodeUTF8));
        par3minCB->setText(QApplication::translate("AnalyzerDlg", "min", 0, QApplication::UnicodeUTF8));
        par3maxCB->setText(QApplication::translate("AnalyzerDlg", "max", 0, QApplication::UnicodeUTF8));
        parsTW->setTabText(parsTW->indexOf(tab_5), QApplication::translate("AnalyzerDlg", "Par 3", 0, QApplication::UnicodeUTF8));
        textBrowser->setHtml(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">par[0] + par[1]*tanh(par[2]*Vcal + par[3])</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">charge(e</span><span style=\" font-style:italic; vertical-align:super;\">-</span><span style=\" font-style:itali"
                        "c;\">) = Vcal*421</span></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("AnalyzerDlg", "Expressions", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chargeCuts5LE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        chargeCuts5LE->setText(QString());
#ifndef QT_NO_TOOLTIP
        chargeCuts3LE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        chargeCuts3LE->setText(QString());
        chargeMainCutsCB->setText(QApplication::translate("AnalyzerDlg", "Main cuts", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chargeMainCutsLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        chargeMainCutsLE->setText(QString());
#ifndef QT_NO_TOOLTIP
        chargeCuts2LE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        chargeCuts2LE->setText(QString());
        chargeCuts2CB->setText(QApplication::translate("AnalyzerDlg", "Cell charge", 0, QApplication::UnicodeUTF8));
        chargeCuts6CB->setText(QApplication::translate("AnalyzerDlg", "Cell Landau", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chargeCuts6LE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        chargeCuts6LE->setText(QString());
        chargeCuts3CB->setText(QApplication::translate("AnalyzerDlg", "Cell charge X", 0, QApplication::UnicodeUTF8));
        chargeCuts4CB->setText(QApplication::translate("AnalyzerDlg", "Cell charge Y", 0, QApplication::UnicodeUTF8));
        chargeCuts5CB->setText(QApplication::translate("AnalyzerDlg", "Cluster Landau", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chargeCuts4LE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        chargeCuts4LE->setText(QString());
        studiesTW->setTabText(studiesTW->indexOf(chargeT), QApplication::translate("AnalyzerDlg", "Charge", 0, QApplication::UnicodeUTF8));
        resolutionCutsGB->setTitle(QApplication::translate("AnalyzerDlg", "Cuts", 0, QApplication::UnicodeUTF8));
        chi2ResolutionCutCB->setText(QApplication::translate("AnalyzerDlg", "chi2/ndof", 0, QApplication::UnicodeUTF8));
        nTrackResolutionCutCB->setText(QApplication::translate("AnalyzerDlg", "Number of tracks", 0, QApplication::UnicodeUTF8));
        xErrResolutionCutCB->setText(QApplication::translate("AnalyzerDlg", "X predicted error", 0, QApplication::UnicodeUTF8));
        yErrResolutionCutCB->setText(QApplication::translate("AnalyzerDlg", "Y predicted error", 0, QApplication::UnicodeUTF8));
        xErrResolutionLB->setText(QApplication::translate("AnalyzerDlg", "um", 0, QApplication::UnicodeUTF8));
        yErrResolutionLB->setText(QApplication::translate("AnalyzerDlg", "um", 0, QApplication::UnicodeUTF8));
        hitsResolutionCutCB->setText(QApplication::translate("AnalyzerDlg", "Min hits", 0, QApplication::UnicodeUTF8));
        resolutionStandardCutsCB->setText(QApplication::translate("AnalyzerDlg", "Standard cuts", 0, QApplication::UnicodeUTF8));
        resolutionExcludeBadPlanesCB->setText(QApplication::translate("AnalyzerDlg", "Exclude bad working planes:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("AnalyzerDlg", "see Charge", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("AnalyzerDlg", "Expressions", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        resolutionCuts4LE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        resolutionCuts4LE->setText(QString());
#ifndef QT_NO_TOOLTIP
        resolutionMainCutsLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        resolutionMainCutsLE->setText(QString());
        resolutionMainCutsCB->setText(QApplication::translate("AnalyzerDlg", "main cuts", 0, QApplication::UnicodeUTF8));
        resolutionCuts2CB->setText(QApplication::translate("AnalyzerDlg", "X resolution", 0, QApplication::UnicodeUTF8));
        resolutionCuts3CB->setText(QApplication::translate("AnalyzerDlg", "Y resolution", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        resolutionCuts3LE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        resolutionCuts3LE->setText(QString());
        resolutionCuts4CB->setText(QApplication::translate("AnalyzerDlg", "Errors", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        resolutionCuts2LE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        resolutionCuts2LE->setText(QString());
        studiesTW->setTabText(studiesTW->indexOf(resolutionT), QApplication::translate("AnalyzerDlg", "Resolution", 0, QApplication::UnicodeUTF8));
        analysesGB->setTitle(QApplication::translate("AnalyzerDlg", "Analyses Controls", 0, QApplication::UnicodeUTF8));
        efficiencyCB->setText(QApplication::translate("AnalyzerDlg", "Efficiency", 0, QApplication::UnicodeUTF8));
        chargeCB->setText(QApplication::translate("AnalyzerDlg", "Charge", 0, QApplication::UnicodeUTF8));
        resolutionCB->setText(QApplication::translate("AnalyzerDlg", "Resolution", 0, QApplication::UnicodeUTF8));
        tracksCB->setText(QApplication::translate("AnalyzerDlg", "Tracks", 0, QApplication::UnicodeUTF8));
        windowsCB->setText(QApplication::translate("AnalyzerDlg", "Windows", 0, QApplication::UnicodeUTF8));
        runAnalysisPB->setText(QApplication::translate("AnalyzerDlg", "Run Analysis", 0, QApplication::UnicodeUTF8));
        runMaxThreadsLB->setText(QApplication::translate("AnalyzerDlg", "Max Threads:", 0, QApplication::UnicodeUTF8));
        runTotalEventsLB->setText(QApplication::translate("AnalyzerDlg", "Total Events:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        runTotalEventsLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Events</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Sho"
                        "w the number of event </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">objects found in input file.</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        runTotalEventsLE->setText(QString());
        runMaxEventsCB->setText(QApplication::translate("AnalyzerDlg", "Max Events:", 0, QApplication::UnicodeUTF8));
        tracksafterCB->setText(QApplication::translate("AnalyzerDlg", "TracksAfter", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AnalyzerDlg", "Priority List", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("AnalyzerDlg", "Do Fits:", 0, QApplication::UnicodeUTF8));
        resolutionFitCB->setText(QApplication::translate("AnalyzerDlg", "Resolution", 0, QApplication::UnicodeUTF8));
        chargeFitCB->setText(QApplication::translate("AnalyzerDlg", "Charge", 0, QApplication::UnicodeUTF8));
        tracksafterFitCB->setText(QApplication::translate("AnalyzerDlg", "TracksAfter", 0, QApplication::UnicodeUTF8));
        efficiencyFitCB->setText(QApplication::translate("AnalyzerDlg", "Efficiency", 0, QApplication::UnicodeUTF8));
        windowsFitCB->setText(QApplication::translate("AnalyzerDlg", "Windows", 0, QApplication::UnicodeUTF8));
        tracksFitCB->setText(QApplication::translate("AnalyzerDlg", "Tracks", 0, QApplication::UnicodeUTF8));
        useCalibrationsCB->setText(QApplication::translate("AnalyzerDlg", "Use calibrations:", 0, QApplication::UnicodeUTF8));
        runFilesManagementGB->setTitle(QApplication::translate("AnalyzerDlg", "File(s) management", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        outputFileLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        outputFileLE->setText(QApplication::translate("AnalyzerDlg", "Output file with analysis plots (.root)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openNtupleFilePB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        openNtupleFilePB->setText(QApplication::translate("AnalyzerDlg", "Load\n"
"file(s) to\n"
"analyze", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        saveOutputFilePB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        saveOutputFilePB->setText(QApplication::translate("AnalyzerDlg", "Save as", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        resetListPB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        resetListPB->setText(QApplication::translate("AnalyzerDlg", "Reset\n"
"list", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(runT), QApplication::translate("AnalyzerDlg", "Run", 0, QApplication::UnicodeUTF8));
        runScanPB->setText(QApplication::translate("AnalyzerDlg", "Run Scan", 0, QApplication::UnicodeUTF8));
        loadFileToScanListPB->setText(QApplication::translate("AnalyzerDlg", "Load file to scan list", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = scanListTW->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("AnalyzerDlg", "File Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = scanListTW->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("AnalyzerDlg", "Scan Value", 0, QApplication::UnicodeUTF8));
        resetScanListPB->setText(QApplication::translate("AnalyzerDlg", "Reset scan list", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(tab), QApplication::translate("AnalyzerDlg", "Run Scan", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(tab8), QApplication::translate("AnalyzerDlg", "Page", 0, QApplication::UnicodeUTF8));
        cutFilePB->setText(QApplication::translate("AnalyzerDlg", "Browse File", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("AnalyzerDlg", "Cut with File:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("AnalyzerDlg", "Choose Histogram:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        cutDut0RB->setText(QApplication::translate("AnalyzerDlg", "Dut0", 0, QApplication::UnicodeUTF8));
        cutDut1RB->setText(QApplication::translate("AnalyzerDlg", "Dut1", 0, QApplication::UnicodeUTF8));
        studiesTW->setTabText(studiesTW->indexOf(cellT), QApplication::translate("AnalyzerDlg", "Manual Cell Selection", 0, QApplication::UnicodeUTF8));
        abortPB->setText(QApplication::translate("AnalyzerDlg", "Abort", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openConfigurationFilePB->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        openConfigurationFilePB->setText(QApplication::translate("AnalyzerDlg", "Load config file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        loadedConfigurationFileLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        loadedConfigurationFileLE->setText(QApplication::translate("AnalyzerDlg", "Load configuration file (.xml)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        saveConfigurationFile->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        saveConfigurationFile->setText(QApplication::translate("AnalyzerDlg", "Save config as", 0, QApplication::UnicodeUTF8));
        runningThreadsLB->setText(QApplication::translate("AnalyzerDlg", "Running Threads:", 0, QApplication::UnicodeUTF8));
        runningThreadsLE->setText(QApplication::translate("AnalyzerDlg", "0", 0, QApplication::UnicodeUTF8));
        processedEventsLB->setText(QApplication::translate("AnalyzerDlg", "Processed Events:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        processedEventsLE->setToolTip(QApplication::translate("AnalyzerDlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; text-decoration: underline; color:#0000ff;\">Events</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Sho"
                        "w the number of event </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">objects found in input file.</p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        processedEventsLE->setText(QString());
        activityLB->setText(QApplication::translate("AnalyzerDlg", "Idle", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AnalyzerDlg: public Ui_AnalyzerDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYZERDLG_H
