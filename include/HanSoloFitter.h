#ifndef HANSOLOFITTER_H
#define HANSOLOFITTER_H
#include <sstream>
#include <QWidget>
#include <QTabWidget>
#include <QTimer>
#include <TBrowser.h>
#include <TFolder.h>
#include <QVBoxLayout>
#include <QActionGroup>
#include <CanvasWidget.h>
#include <TQtWidget.h>
#include "HanSoloTreeBrowser.h"
#include "mainwindow.h"
#include "fitParamManagerWidget.h"
#include <QMdiSubWindow>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/regex.hpp>
#include <QVBoxLayout>
#include <QDialog>
#include <string>
#include <vector>
#include <QGroupBox>

#include <sstream>
#include <string>


class MainWindow         ;
class HanSoloTreeBrowser       ;
class TFile              ;
class QVBoxLayout        ;
class XmlParser          ;


namespace Ui {
class HanSoloFitter;
}

class HanSoloFitter : public QDialog
{
    Q_OBJECT
public:
    explicit     HanSoloFitter          (QWidget *parent = 0       ) ;
                 ~HanSoloFitter         (void                      ) ;

    void         collectExistingWidgets (                          ) ;
    std::string  twoDOption             (void                      ) ;
    bool         plotStatBox            (void                      ) ;
    bool         plotFitBox             (void                      ) ;
    bool         logX                   (void                      ) ;
    bool         logY                   (void                      ) ;
    bool         logZ                   (void                      ) ;
    std::string  canvasSplitType        (void                      ) ;
    CanvasWidget* getCanvas             (void                      ) ;
    void         refresh                (void                      ) ;
    static QString      getEnvPath      (QString environmentName   ) ;

public slots:
    void         addNewFile             (const QString & file      ) ;

    QMdiSubWindow * getDrawingWindow    (void                      ) ;


private:
    void         createActions          (void                      ) ;
    void         createMenus            (void                      ) ;
    void         createConnections      (void                      ) ;
    void         fillWidget             (void                      ) ;
    void         fillWidgetTree         (std::string    fileName   ) ;
    void         addItem                (std::string    item       ) ;
    void         delItem                (std::string    item       ) ;
    void         makeDir                (std::string    dirName    ) ;
    void         addFile                (std::string    fileName, TFile* file) ;
    void         initialize             (void                      ) ;
    void         initXml                (void                      ) ;

private:
    MainWindow*              theMainWindow_      ;
    Ui::HanSoloFitter*       ui_                 ;
    HanSoloTreeBrowser*      hanSoloTreeBrowser_ ;
    QTabWidget*              widget_             ;
    CanvasWidget *           canvas_             ;
    QTimer*                  timer_              ;
    QString                  emptyFileLabel_     ;
    QString                  displayAllLabel_    ;
    QString                  displayMemoryLabel_ ;
    QMenuBar *               menu_               ;
    QMenu *                  menuAnalyze_        ;
    QMenu *                  menuOptions_        ;
    QMenu *                  menuAdvanced_       ;
    QMenu *                  menuRun_Basic_Fit_  ;
    QMenu *                  menuDetectorPresets_;
    QActionGroup *           detectorPresetsGroup_;
    QAction *                actionPlanar_       ;
    QAction *                action3D_           ;
    QAction *                actionCharge_       ;
    QAction *                actionEfficiency_   ;
    QAction *                actionResolution_   ;
    QAction *                actionRun_Advanced_Fitter_;
    int                      counter_            ;    
    int                      shiftHSValue_       ;
    int                      currentIndex_       ;
    std::vector<std::string> chargeVector_       ;
    std::vector<std::string> efficiencyVector_   ;
    std::vector<std::string> resolutionVector_   ;

    std::vector<std::vector<QCheckBox *> > chargeCheckBoxes_   ;
    std::vector<std::vector<QCheckBox *> >efficiencyCheckBoxes_;
    std::vector<std::vector<QCheckBox *> >resolutionCheckBoxes_;

    std::vector<std::vector<std::string> > checkedChargeCheckBoxes_   ;
    std::vector<std::vector<std::string> >checkedEfficiencyCheckBoxes_;
    std::vector<std::vector<std::string> >checkedResolutionCheckBoxes_;

    std::vector<std::vector<std::string> > checkedBoxes_       ;
    std::vector<std::vector<QFrame*> >     qFrames_;

    std::vector<QWidget*>   pointerToDutTabs_;

    std::vector<QCheckBox *> generalChargeCheckBoxes_   ;
    std::vector<QCheckBox *>generalEfficiencyCheckBoxes_;
    std::vector<QCheckBox *>generalResolutionCheckBoxes_;
    std::vector<std::string> generalCheckedBoxes_;

 /*   std::vector<QCheckBox *> Dut_0_chargeCheckBoxes_   ;
    std::vector<QCheckBox *>Dut_0_efficiencyCheckBoxes_;
    std::vector<QCheckBox *>Dut_0_resolutionCheckBoxes_;
    std::vector<std::string> Dut_0_checkedBoxes_       ;

    std::vector<QCheckBox *> Dut_1_chargeCheckBoxes_   ;
    std::vector<QCheckBox *>Dut_1_efficiencyCheckBoxes_;
    std::vector<QCheckBox *>Dut_1_resolutionCheckBoxes_;
    std::vector<std::string> Dut_1_checkedBoxes_       ;

    std::vector<QCheckBox *> Dut_2_chargeCheckBoxes_   ;
    std::vector<QCheckBox *>Dut_2_efficiencyCheckBoxes_;
    std::vector<QCheckBox *>Dut_2_resolutionCheckBoxes_;
    std::vector<std::string> Dut_2_checkedBoxes_       ;

    std::vector<QCheckBox *> Dut_3_chargeCheckBoxes_   ;
    std::vector<QCheckBox *>Dut_3_efficiencyCheckBoxes_;
    std::vector<QCheckBox *>Dut_3_resolutionCheckBoxes_;
    std::vector<std::string> Dut_3_checkedBoxes_       ;
*/
    std::string                rootFile_         ;
    XmlParser*                   theXmlParser_   ;



    std::stringstream        ss_                 ;
    std::map<std::string, TFile*>  openFiles_    ;

private slots:
    void       checkBoxesHandler      (bool                      ) ;
    void       checkBoxesHandler      (int                       ) ;
    void       on_openFilePB_clicked  (                          ) ;
    void       updateTree             (QString currentFile       ) ;
    void       checkNewObjectsInMemory(void                      ) ;
    void       on_setRangePB_clicked  (void                      ) ;
    void       on_unZoomPB_clicked    (                          ) ;
    void       on_hCanvasCB_activated (int index                 ) ;
    void       on_shiftHS_valueChanged(int value                 ) ;
    void       on_shiftPlusPB_clicked (void                      ) ;
    void       on_shiftMinusPB_clicked(void                      ) ;
    void on_centraltabWidget_currentChanged(int index            ) ;
    void       getFitters             (void                      ) ;
    void        makeFittersSelectable (void                      ) ;
    void        on_runButton_clicked  (                          ) ;
    void        on_openRootFile_clicked(                         ) ;
    void        checkAllCharge         (                         ) ;
    void        checkAllEfficiency     (                         ) ;
    void        checkAllResolution     (                         ) ;
    void        unCheckGeneral         (bool check               ) ;
    void        on_reset_clicked       (                         ) ;
    void        on_openConfigurationFilePB_clicked(              ) ;
    void        on_fitterOptions_selected(const QString &arg1    ) ;
    void        on_saveConfigurationFile_clicked(                ) ;
    void        on_resetDut_clicked    (                         ) ;
    void        on_fitterOptions_currentChanged(int index        ) ;
    void        check_for_checkall     (void                     ) ;

signals:
    


};

#endif // HANSOLOFITTER_H
















