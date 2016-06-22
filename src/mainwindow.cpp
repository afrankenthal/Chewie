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

#include "mainwindow.h"
#include "hnavigator.h"
#include "analyzerdlg.h"
#include "HanSoloFitter.h"

#include <mdisubwindow.h>
#include <QtGui>

//#include "MessageTools.h"

//===========================================================================
MainWindow::MainWindow() :
    theHNavigator_   (0)
  , theAnalyzer_     (0)
  , theHanSoloFitter_(0)
{
    path_ = QString(getenv("CHEWIEDIR")) ;

    mdiArea_ = new QMdiArea;
    mdiArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea_->setVerticalScrollBarPolicy  (Qt::ScrollBarAsNeeded);

    QLinearGradient gradient(0,100,0,100);
    gradient.setColorAt(0.5, QColor::fromRgb( 0,  0,  0, 255));
    gradient.setColorAt(1.0, QColor::fromRgb(60, 60, 60, 255));

    QBrush mdiAreaBrush(gradient);
    mdiArea_->setBackground(mdiAreaBrush);

    setCentralWidget(mdiArea_);

    windowMapper_ = new QSignalMapper(this);

    connect(windowMapper_, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

    createActions  ();
    createMenus    ();
    createToolBars ();
    createStatusBar();
    updateMenus    ();
    readSettings   ();

    setWindowTitle(tr("Chewie"));
    setUnifiedTitleAndToolBarOnMac(true);
}

//===========================================================================
MainWindow::~MainWindow()
{
    destroyActions();
    delete mdiArea_;
}

//===========================================================================
void MainWindow::closeEvent(QCloseEvent *event)
{
    TObjLink *lnk = TApplication::GetApplications()->FirstLink();
    while (lnk)
    {
     ss_.str(""); ss_ << "Terminating " << lnk->GetObject()->GetName();STDLINE(ss_.str(),ACWhite) ;
     ((TApplication*)lnk->GetObject())->Terminate() ;
     lnk = lnk->Next();
    }

    mdiArea_->closeAllSubWindows();
    if (mdiArea_->currentSubWindow())
    {
        event->ignore();
    } else
    {
        writeSettings();
        event->accept();
    }
    STDLINE("Terminating Chewie",ACWhite) ;
}

//===========================================================================
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Chewie"), tr("<b>Chewie</b> is the testbeam analyzer."));
}

//===========================================================================
void MainWindow::updateMenus()
{
    //bool hasChild = (activeChildDlg() != 0);
    //    analyzerAct->setEnabled(hasChild);
}

//===========================================================================
void MainWindow::updateWindowMenu()
{
    windowMenu_->clear();
    //  windowMenu_->addAction(closeAct);

    QList<QMdiSubWindow*> windows = mdiArea_->subWindowList();
    separatorAct_->setVisible(!windows.isEmpty());

    /*
    for (int i = 0; i < windows.size(); ++i)
    {
        QDialog* child = qobject_cast<QDialog*>(windows.at(i)->widget());

        //QString text;
        //if (i < 9) {
        //    text = tr("&%1 %2").arg(i + 1)
        //                       .arg(child->userFriendlyCurrentFile());
        //} else {
        //    text = tr("%1 %2").arg(i + 1)
        //                      .arg(child->userFriendlyCurrentFile());
        //}
        //QAction *action  = windowMenu->addAction(text);
        //action->setCheckable(true);
        //action ->setChecked(child == activeMdiChild());
        //connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        //windowMapper->setMapping(action, windows.at(i));
    }
*/
}

//===========================================================================
void MainWindow::enableAnalyzerButton(bool enable)
{
    analyzerAct_->setEnabled(enable);
    if(enable)
        theAnalyzer_ = 0;
}

//===========================================================================
void MainWindow::enableHNavigatorButton(bool enable)
{
    hNavigatorAct_->setEnabled(enable);
    if(enable)
        theHNavigator_ = 0;
}

//===========================================================================
void MainWindow::enableHanSoloFitterButton(bool enable)
{
    hanSoloFitterAct_->setEnabled(enable);
    if(enable)
        theHanSoloFitter_ = 0;
}

//===========================================================================
void MainWindow::createActions()
{
    QString iconPath ;

    //! [0]
    exitAct_ = new QAction(tr("E&xit"), this);
    exitAct_->setShortcuts(QKeySequence::Quit);
    exitAct_->setStatusTip(tr("Exit the application"));
    connect(exitAct_, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    //! [0]
    separatorAct_ = new QAction(this);
    separatorAct_->setSeparator(true);

    aboutAct_ = new QAction(tr("&About"), this);
    aboutAct_->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct_, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct_ = new QAction(tr("About &Qt"), this);
    aboutQtAct_->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    iconPath  = path_ ;
    iconPath += "/images/chewie.png" ;
    analyzerAct_ = new QAction(QIcon(iconPath), tr("&Open Analyzer"), this);
    analyzerAct_->setShortcuts(QKeySequence::New);
    analyzerAct_->setStatusTip(tr("Open a new analyzer"));
    connect(analyzerAct_, SIGNAL(triggered()), this, SLOT(openAnalyzer()));

    iconPath  = path_ ;
    iconPath += "/images/newCanvas.png" ;
    hNavigatorAct_ = new QAction(QIcon(iconPath), tr("Histogram Navigator"), this);
    hNavigatorAct_->setStatusTip(tr("Create a Histogram Navigator")) ;
    connect(hNavigatorAct_,   SIGNAL(triggered()), this, SLOT(openHNavigator()));

    iconPath  = path_ ;
    iconPath += "/images/HanSolo.png" ;
    hanSoloFitterAct_ = new QAction(QIcon(iconPath), tr("Han Solo Fitter"), this);
    hanSoloFitterAct_->setStatusTip(tr("Create a Han Solo Fitter")) ;
    connect(hanSoloFitterAct_,   SIGNAL(triggered()), this, SLOT(openHanSoloFitter()));
}

//===========================================================================
void MainWindow::destroyActions()
{
    delete exitAct_         ;
    delete separatorAct_    ;
    delete aboutAct_        ;
    delete aboutQtAct_      ;
    delete analyzerAct_     ;
    delete hNavigatorAct_   ;
    delete hanSoloFitterAct_;
}

//===========================================================================
void MainWindow::createMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    //    QAction *action = fileMenu->addAction(tr("Switch layout direction"));
    //    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
    fileMenu_->addAction(exitAct_);

    windowMenu_ = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu_, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(aboutAct_);
    helpMenu_->addAction(aboutQtAct_);
}

//===========================================================================
void MainWindow::createToolBars()
{
    fileToolBar_ = addToolBar(tr("Dialogs"));
    fileToolBar_->addAction(analyzerAct_);
    fileToolBar_->addAction(hNavigatorAct_) ;
    fileToolBar_->addAction(hanSoloFitterAct_) ;
}

//===========================================================================
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

//===========================================================================
void MainWindow::readSettings()
{
    QSettings settings("Trolltech", "Chewie Analyzer");
    QPoint pos = settings.value("pos",  QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(1024, 820)).toSize();
    move  (pos );
    resize(size);
}

//===========================================================================
void MainWindow::writeSettings()
{
    QSettings settings("Trolltech", "Chewie Analyzer");
    settings.setValue("pos",  pos ());
    //cout << __LINE__ << "] " << this->size().width() << " " << this->size().height() << endl ;
    settings.setValue("size", size());
}

//===========================================================================
QDialog* MainWindow::activeChildDlg()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea_->activeSubWindow())
    {
        return qobject_cast<QDialog*>(activeSubWindow->widget());
    }
    return 0;
}

//===========================================================================
void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

//===========================================================================
void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea_->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

//===========================================================================
void MainWindow::openAnalyzer()
{
    if(!theAnalyzer_)
    {
        theAnalyzer_ = new AnalyzerDlg(this) ;
        QMdiSubWindow* subWindow = mdiArea_->addSubWindow(theAnalyzer_);
        enableAnalyzerButton(false) ;
        connect(subWindow, SIGNAL(destroyed()), this, SLOT(enableAnalyzerButton()));
        subWindow->setGeometry(10,10,theAnalyzer_->width(),theAnalyzer_->height()+28);
        subWindow->show();
    }
}
//===========================================================================
void MainWindow::openHNavigator()
{
    if( !theHNavigator_)
    {
        theHNavigator_ = new HNavigator(this) ;
        QMdiSubWindow* subWindow = (mdiSubWindow*)mdiArea_->addSubWindow(theHNavigator_) ;
        enableHNavigatorButton(false) ;
        connect(subWindow, SIGNAL(destroyed()), this, SLOT(enableHNavigatorButton()));
        if( theAnalyzer_ )
        {
            subWindow->setGeometry(theAnalyzer_->width()+17,10,theHNavigator_->width(),theHNavigator_->height()) ;
        }
        else
        {
            subWindow->setGeometry(10,10,theHNavigator_->width(),theHNavigator_->height()) ;
        }
        subWindow->show() ;
    }
}
//===========================================================================
void MainWindow::openHanSoloFitter()
{
    if( !theHanSoloFitter_)
    {
        theHanSoloFitter_ = new HanSoloFitter(this) ;
        QMdiSubWindow* subWindow = (mdiSubWindow*)mdiArea_->addSubWindow(theHanSoloFitter_) ;
        enableHanSoloFitterButton(false) ;
        connect(subWindow, SIGNAL(destroyed()), this, SLOT(enableHanSoloFitterButton()));
        subWindow->setGeometry(100,0,theHanSoloFitter_->width()+8,theHanSoloFitter_->height()+40) ;
        subWindow->show() ;
    }
}
