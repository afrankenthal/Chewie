#ifndef HANSOLOFITTER_H
#define HANSOLOFITTER_H
#include <sstream>
#include <QWidget>
#include <QTabWidget>
#include <QTimer>
#include <TBrowser.h>
#include <TFolder.h>
#include <QVBoxLayout>
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

class MainWindow         ;
class HanSoloTreeBrowser       ;
class TFile              ;
class QVBoxLayout        ;


namespace Ui {
class HanSoloFitter;
}

class HanSoloFitter : public QDialog
{
    Q_OBJECT
public:
    explicit     HanSoloFitter         (QWidget *parent = 0       ) ;
                 ~HanSoloFitter        (void                      ) ;

    void         collectExistingWidgets(                          ) ;
    std::string  twoDOption            (void                      ) ;
    bool         plotStatBox           (void                      ) ;
    bool         plotFitBox            (void                      ) ;
    bool         logX                  (void                      ) ;
    bool         logY                  (void                      ) ;
    bool         logZ                  (void                      ) ;
    std::string  canvasSplitType       (void                      ) ;
    CanvasWidget* getCanvas             (void                     ) ;
    void         refresh               (void                      ) ;

public slots:
    void         addNewFile            (const QString & file      ) ;

    QMdiSubWindow * getDrawingWindow         (void                      ) ;
private:
    void         fillWidget            (void                      ) ;
    void         fillWidgetTree        (std::string    fileName   ) ;
    void         addItem               (std::string    item       ) ;
    void         delItem               (std::string    item       ) ;
    void         makeDir               (std::string    dirName    ) ;
    void         addFile               (std::string    fileName, TFile* file) ;


private:
    MainWindow*              theMainWindow_      ;
    Ui::HanSoloFitter*       ui_                 ;
    HanSoloTreeBrowser*            hanSoloTreeBrowser_ ;
    QTabWidget*              widget_             ;
    CanvasWidget *           canvas_             ;
    QTimer*                  timer_              ;
    QString                  emptyFileLabel_     ;
    QString                  displayAllLabel_    ;
    QString                  displayMemoryLabel_ ;
    int                      counter_            ;    
    int                      shiftHSValue_       ;

    std::stringstream        ss_                 ;
    std::map<std::string, TFile*>  openFiles_    ;




signals:
    
public slots:
    
private slots:
    void on_openFilePB_clicked                         ();
    void updateTree                 (QString currentFile);
    void checkNewObjectsInMemory    (void               );
};

#endif // HANSOLOFITTER_H
