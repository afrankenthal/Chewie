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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <TApplication.h>
#include <TCollection.h>
#include <sstream>

class HNavigator   ;
class AnalyzerDlg  ;
class HanSoloFitter;

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QSignalMapper;
class QDialog;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    QMdiArea     *  getMdiArea            (void                    ) {return mdiArea_         ;}
    HNavigator   *  getHNavigator         (void                    ) {return theHNavigator_   ;}
    AnalyzerDlg  *  getAnalyzer           (void                    ) {return theAnalyzer_     ;}
    HanSoloFitter*  getHanSoloFitter      (void                    ) {return theHanSoloFitter_;}
protected:
    void            closeEvent            (QCloseEvent  * event    );

private slots:
    void            openAnalyzer             (void                    );
    void            about                    (void                    );
    void            openHNavigator           (void                    );
    void            openHanSoloFitter        (void                    );
    void            updateMenus              (void                    );
    void            updateWindowMenu         (void                    );
    void            enableAnalyzerButton     (bool enable=true        );
    void            enableHNavigatorButton   (bool enable=true        );
    void            enableHanSoloFitterButton(bool enable=true        );
    void            switchLayoutDirection    (void                    );
    void            setActiveSubWindow       (QWidget *window         );


private:
    void            createActions         (void                    );
    void            destroyActions        (void                    );
    void            createMenus           (void                    );
    void            createToolBars        (void                    );
    void            createStatusBar       (void                    );
    void            readSettings          (void                    );
    void            writeSettings         (void                    );

    QDialog*        activeChildDlg        (void                    );

    QMdiArea*       mdiArea_          ;
    QSignalMapper*  windowMapper_     ;

    HNavigator*     theHNavigator_    ;
    AnalyzerDlg*    theAnalyzer_      ;
public:
    HanSoloFitter*  theHanSoloFitter_ ;
private:

    QMenu*          fileMenu_         ;
    QMenu*          editMenu_         ;
    QMenu*          windowMenu_       ;
    QMenu*          helpMenu_         ;

    QToolBar*       fileToolBar_      ;
    QToolBar*       editToolBar_      ;

    QString         path_             ;

    QAction*        exitAct_          ;
    QAction*        separatorAct_     ;
    QAction*        aboutAct_         ;
    QAction*        aboutQtAct_       ;
    QAction*        analyzerAct_      ;
    QAction*        hNavigatorAct_    ;
    QAction*        hanSoloFitterAct_ ;

    std::stringstream ss_ ;
};

#endif
