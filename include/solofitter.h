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

#ifndef SOLOFITTER_H
#define SOLOFITTER_H

#include <sstream>

#include <QTabWidget>
#include <QTimer>
#include <TBrowser.h>
#include <TFolder.h>

#include <QVBoxLayout>

//#include "fileEater.h"
//#include "HManager.h"
#include "hTreeBrowser.h"
//#include "maintabs.h"
#include "mainwindow.h"
#include "fitParamManagerWidget.h"

#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/regex.hpp>

class MainWindow ;
//class mainTabs ;
class hTreeBrowser ;
class TFile;

QT_BEGIN_NAMESPACE
class QMdiArea ;
QT_END_NAMESPACE

namespace Ui {
    class HNavigator;
}

class SoloFitter : public QTabWidget
{
    Q_OBJECT

public:
    explicit     SoloFitter            (QWidget       * parent = 0) ;
                ~SoloFitter            (void                      ) ;

    void         collectExistingWidgets(                          ) ;
    void         refresh               (void                      ) ;
    std::string  twoDOption            (void                      ) ;
    bool         plotStatBox           (void                      ) ;
    bool         plotFitBox            (void                      ) ;
    bool         logX                  (void                      ) ;
    bool         logY                  (void                      ) ;
    bool         logZ                  (void                      ) ;
    std::string  canvasSplitType       (void                      ) ;

public slots:
    void         addNewFile            (const QString & file      ) ;

    QMdiArea   * getMdiArea            (void                      ) ;
    MainWindow * getMainWindow         (void                      ) ;

private:
    void         fillWidget            (void                      ) ;
    void         fillWidgetTree        (std::string    fileName   ) ;
    void         resizeEvent           (QResizeEvent * event      ) ;
    void         addItem               (std::string    item       ) ;
    void         delItem               (std::string    item       ) ;
    void         makeDir               (std::string    dirName    ) ;
    void         addFile               (std::string    fileName, TFile* file) ;

    MainWindow*             theMainWindow_      ;
    QTimer*                 timer_              ;
    Ui::HNavigator*         ui_                 ;
    hTreeBrowser*           theHTreeBrowser_    ;

    QString                 emptyFileLabel_     ;
    QString                 displayAllLabel_    ;
    QString                 displayMemoryLabel_ ;

    std::map<std::string, TFile*> openFiles_    ;

    int                     counter_            ;
    std::stringstream       ss_                 ;
    int                     shiftHSValue_       ;

    std::vector<FitParamManagerWidget*> theParamManager_;
    int                                 numberOfFitting_;
    int                                 numberofCanvas_;

    std::map<std::string, int> fitFunctions_;



private slots:
    void checkBoxesHandler          (int                );
    void checkBoxesHandler          (bool               );
    void on_unZoomPB_clicked        (void               );
    void on_canvasSizeCB_activated  (QString            );
    void on_hCanvasCB_activated     (int     index      );
    void on_saveComponentsPB_clicked(                   );
    void updateTree                 (QString currentFile);
    void checkNewObjectsInMemory    (void               );
    void on_openFilePB_clicked      (void               );
    void on_setRangePB_clicked      (void               );
    void on_shiftPlusPB_clicked     (void               );
    void on_shiftMinusPB_clicked    (void               );
    void on_shiftHS_valueChanged    (int     value      );
    void on_fitPB_clicked           (                   );

    std::string getObjectType (TObject *obj);

    void on_fitLimitCB_clicked(bool checked);

    void on_fitFuncLW_itemDoubleClicked (QListWidgetItem *item);
    void on_fitFuncLE_editingFinished   ();
    void on_customFunctionCB_clicked    ();
    void on_saveImagePB_clicked();
};


#endif // SOLOFITTER_H
