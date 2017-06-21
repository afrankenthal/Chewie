/*===============================================================================
 * Chewie: the FERMILAB MTEST telescope and DUT anaysis tool
 * 
 * Copyright (C) 2014 
 *
 * Authors:
 *
 * Mauro Dinardo      (Universita' Bicocca) 
 * Dario Menasce      (INFN) 
 * Jennifer Ngadiuba  (INFN)
 * Lorenzo Uplegger   (FNAL)
 * Luigi Vigani       (INFN)
 *
 * INFN: Piazza della Scienza 3, Edificio U2, Milano, Italy 20126
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ================================================================================*/

#ifndef ANALYZERDLG_H
#define ANALYZERDLG_H

#include <QtWidgets/QDialog>
#include <QtCore/QString>
#include <QtCore/QFuture>
#include <QtCore/QFutureWatcher>
#include <QtWidgets/QListWidgetItem>

#include <sstream>
#include <string>

class EventReader        ;
class EventManager       ;
class AnalysisManager    ;
class HistogramWindow    ;
class XmlParser          ;
class TFile              ;

namespace Ui {
class AnalyzerDlg;
}

class AnalyzerDlg : public QDialog
{
    Q_OBJECT

public:
    typedef std::map<std::string,TFile*> openFilesDef_ ;

    explicit          AnalyzerDlg        (QWidget * parent = 0     );
                     ~AnalyzerDlg        (void                     );

    openFilesDef_     getOpenFiles       (void                     );

    AnalysisManager * getAnalysisManager (void                     ) {return theAnalysisManager_;}

    static QString    getEnvPath         (QString   environmentName);

private:
    void              setInnerGeometry   (void                    );
    void              openGeoFile        (QString   fileName      );
    void              initXml            (void                    );
    static void     * threaderMethod     (void    * myClass,
                                          void    * whatToRun,
                                          int       arg0=0        );
    void              checkBoxHandler    (void                    );
    void              setPrioritiesFromUi(void                    );

    Ui::AnalyzerDlg             *ui                     ;
    QTimer                      *theTimer_              ;
    QFuture<void*>              *theFuture_             ;
    QFutureWatcher<void>        *theWatcher_            ;
    XmlParser                   *theXmlParser_          ;
    EventManager                *theEventManager_       ;
    AnalysisManager             *theAnalysisManager_    ;
    QString                      whatToRun_             ;
    std::map<std::string,TFile*> openFileList_          ;

    std::string cutFile_;

private slots:
    void on_calibrationLoadGeoFilePB_clicked   (void                 );
    void on_openCalibrationsFilePB_clicked     (void                 );
    void on_openRootFilePB_clicked             (void                 );
    void on_openNtupleFilePB_clicked           (void                 );
    void on_runAnalysisPB_clicked              (void                 );
    void on_saveOutputFilePB_clicked           (void                 );
    void displayFinishedBox                    (void                 );
    void advanceProgressBar                    (void                 );
    void updateFileName                        (void                 );
    void on_abortPB_clicked                    (void                 );
    void on_calibParsCB_stateChanged           (int   arg1           );
    void on_calibDispersionCB_stateChanged     (int   arg1           );
    void on_dutsListLW_2_itemSelectionChanged  (void                 );
    void on_makeCalibPlotsPB_clicked           (void                 );
    void on_openCalibrationPlotsPB_clicked     (void                 );
    void on_runConverterPB_clicked             (void                 );
    void on_openConfigurationFilePB_clicked    (void                 );
    void on_saveConfigurationFile_clicked      (void                 );
    void on_useCalibrationsCB_stateChanged     (int   arg1           );
    void on_resetRemovedPxlsDUT1PB_clicked     (void                 );
    void on_resetRemovedPxlsDUT0PB_clicked     (void                 );
    void on_removePxlDUT1PB_clicked            (void                 );
    void on_removePxlDUT0PB_clicked            (void                 );
    void on_loadFileToScanListPB_clicked       (void                 );
    void on_scanListTW_cellChanged             (int  row,
                                                int  column          );
    void on_resetScanListPB_clicked            (void                 );
    void on_runScanPB_clicked                  (void                 );

    //////////cuts//////////////////////////////////////////////////
    void on_chi2EfficiencyCutCB_stateChanged   (int  arg1            );
    void on_nTrackEfficiencyCutCB_stateChanged (int  arg1            );
    void on_xErrEfficiencyCutCB_stateChanged   (int  arg1            );
    void on_yErrEfficiencyCutCB_stateChanged   (int  arg1            );
    void on_xEfficiencyCutCB_stateChanged      (int  arg1            );
    void on_yEfficiencyCutCB_stateChanged      (int  arg1            );
    void on_chi2ChargeCutCB_stateChanged       (int  arg1            );
    void on_nTrackChargeCutCB_stateChanged     (int  arg1            );
    void on_xErrChargeCutCB_stateChanged       (int  arg1            );
    void on_yErrChargeCutCB_stateChanged       (int  arg1            );
    void on_xChargeCutCB_stateChanged          (int  arg1            );
    void on_yChargeCutCB_stateChanged          (int  arg1            );
    void on_chi2ResolutionCutCB_stateChanged   (int  arg1            );
    void on_nTrackResolutionCutCB_stateChanged (int  arg1            );
    void on_xErrResolutionCutCB_stateChanged   (int  arg1            );
    void on_yErrResolutionCutCB_stateChanged   (int  arg1            );
    void on_resetListPB_clicked                (void                 );
    void on_convertResetListPB_clicked         (void                 );
//    void on_excludeBadPlanesCB_clicked         (bool checked         );
    void on_cutFilePB_clicked                  (void                 );
    void on_windowsCB_isToggled                (void                 ) {checkBoxHandler();}
    void on_tracksCB_isToggled                 (void                 ) {checkBoxHandler();}
    void on_efficiencyCB_isToggled             (void                 ) {checkBoxHandler();}
    void on_chargeCB_isToggled                 (void                 ) {checkBoxHandler();}
    void on_resolutionCB_isToggled             (void                 ) {checkBoxHandler();}
    void on_tracksafterCB_isToggled            (void                 ) {checkBoxHandler();}

    void setPixelMatrixReferenceHistogram      (void                 );
    void on_cutVarLW_itemDoubleClicked         (QListWidgetItem *item);
};

#endif // ANALYZERDLG_H
