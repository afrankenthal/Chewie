#include "HanSoloFitter.h"
#include "hnavigator.h"
#include "ui_hanSoloFitter.h"
#include "Utilities.h"
#include "analyzerdlg.h"
#include <QDialog>
#include <QString>
#include <QFuture>
#include <QFutureWatcher>
#include <QListWidgetItem>
#include "MessageTools.h"
#include <QMdiSubWindow>
#include <set>

#include <TKey.h>
#include <TFile.h>
#include <TIterator.h>
#include <TDirectory.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TF1.h>
#include <TH2F.h>
#include "TCanvas.h"
#include "CanvasWidget.h"
#include "TGraph.h"
#include <TIterator.h>
#include <TClass.h>

#include <QFileDialog>
#include <QList>

HanSoloFitter::HanSoloFitter(QWidget *parent ) :
    QDialog                     (                       parent)
  , theMainWindow_              (          (MainWindow*)parent)
  , ui_                         (        new Ui::HanSoloFitter)
  , timer_                      (             new QTimer(this))
  , hanSoloTreeBrowser_         (                            0)
  , emptyFileLabel_             ("No files are currently open")
  , displayAllLabel_            (                "Display all")
  , counter_                    (                            0)
{
    ui_->setupUi(this);


    //hanSoloTreeBrowser_ = new hTreeBrowser(this, theMainWindow_);
    //hanSoloTreeBrowser_->setGeometry(250, 250, 250, 250);
    this->addItem(emptyFileLabel_.toStdString()) ;

    //QTabWidget * widget_ = new

    this->collectExistingWidgets();

    connect(ui_->HanSoloFitterInputFile,
            SIGNAL(currentIndexChanged(QString)),
            this,
            SLOT(  updateTree(         QString))) ;

    connect(timer_,
            SIGNAL(timeout                () ),
            this,
            SLOT  (checkNewObjectsInMemory()));


    ui_->canvasSizeCB->setCurrentIndex(2);

    canvas_ = new CanvasWidget();
    canvas_->setParent(ui_->theCanvas);
    canvas_->setTitle("Histogram Display");
    canvas_->setGeometry(0, 30, ui_->theCanvas->width()+20, ui_->theCanvas->height()-12);
    canvas_->show();


   for(int i=0; i<10; ++i)
   {
       ss_.str(""); ss_ << i ;
       ui_->HanSoloCanvasCB->addItem(QString(ss_.str().c_str()));
   }

    //ui_->theCanvas = test;
   ui_->canvasSizeCB->addItem(QString("Small" ));
   ui_->canvasSizeCB->addItem(QString("Medium"));
   ui_->canvasSizeCB->addItem(QString("Large" ));
   ui_->canvasSizeCB->addItem(QString("Huge"  ));
   ui_->canvasSizeCB->setCurrentIndex(2);

   gStyle->SetPalette(1,0) ;




   timer_->start(1000) ;


}
//===========================================================================
HanSoloFitter::~HanSoloFitter()
{

    if(hanSoloTreeBrowser_)
    {
        delete hanSoloTreeBrowser_;
        hanSoloTreeBrowser_ = 0;
    }
    delete ui_;


}
//===========================================================================
//========================File System Sidebar [Start]========================
//===========================================================================
//===========================================================================
void HanSoloFitter::checkNewObjectsInMemory(void)
{
    //    STDLINE("Checking new objects",ACRed);
    if(hanSoloTreeBrowser_)
        hanSoloTreeBrowser_->populate(gROOT->GetRootFolder()) ;
    this->fillWidget();
}

//===========================================================================
void HanSoloFitter::updateTree(QString currentFile)
{
    STDLINE(currentFile.toStdString(),ACRed);
    //hanSoloTreeBrowser_->clear();
    if( currentFile != displayAllLabel_ )
    {
        this->fillWidgetTree(currentFile.toStdString());
    }
    else
    {
        for(int item=0; item < ui_->HanSoloFitterInputFile->count(); ++item)
        {
            this->fillWidgetTree(ui_->HanSoloFitterInputFile->itemText(item).toStdString());
        }
    }
}//===========================================================================
void HanSoloFitter::collectExistingWidgets()
{
    //    STDLINE("collectExistingWidgets",ACRed);
    //QTabWidget * WidgetTabObject = new QTabWidget((QWidget*)parent());


    if( !hanSoloTreeBrowser_ )
    {
        // Create a tree-like folder-navigation tool
        //    this->setGeometry(this->x(),this->y(),this->width(),this->height()+120) ;
        this->show() ;
        //this->setCurrentIndex(0);
        //QWidget * input  = (QTabWidget)this->widget(0);
        hanSoloTreeBrowser_ = new HanSoloTreeBrowser(this, theMainWindow_) ;
        hanSoloTreeBrowser_->setGeometry(ui_->HanSoloTreeFrame->geometry()) ;
        hanSoloTreeBrowser_->show() ;
    }

    // Recover pointers to essential objects
    //  theHManager_  = theMainWindow_->getHManager()  ;
    //  theTabWidget_ = theMainWindow_->getTabWidget() ;

    // Populate the folders navigation tool with the existing open files
    // and their content and point to the current one
    //  if( !theTabWidget_ ) // No mainTabs widget has been opened yet.
    //  {
    //    return ;
    //  }
    //  else
    //  {
    //    theFileEater_ = theTabWidget_->getFileEater() ;
    //  }

    this->fillWidget()     ; // Fill the combo-box with the list of open files
    //    this->fillWidgetTree() ; // Populate the tree widget with file structure content

}


//===========================================================================
// Fill the tree
void HanSoloFitter::fillWidgetTree(std::string currentFile)
{
    STDLINE(std::string("fillWidgetTree. File: ") + currentFile,ACRed);
    STDLINE(openFiles_.size(),ACPurple);
    if(openFiles_.size() == 0)
        return;
    TFile* file  = openFiles_[currentFile] ;
    std::cout << __PRETTY_FUNCTION__ << hanSoloTreeBrowser_ << std::endl;
    hanSoloTreeBrowser_->setCurrentFile(file);
    hanSoloTreeBrowser_->populate((TDirectory *)file) ;
}
//============================================================================
// Fill the combo-box with the list of currently open root files
void HanSoloFitter::fillWidget()
{
    //STDLINE("fillWidget",ACRed);
    AnalyzerDlg* tmpAnalyzer = theMainWindow_->getAnalyzer();
    if(tmpAnalyzer != 0)
    {
        const std::map<std::string,TFile*>& fileList = tmpAnalyzer->getOpenFiles() ;

        for(std::map<std::string,TFile*>::const_iterator it=fileList.begin(); it!=fileList.end(); ++it)
        {
            this->addFile(it->first,it->second);
        }
        this->fillWidgetTree(ui_->HanSoloFitterInputFile->currentText().toStdString());
    }
}
//===========================================================================

void HanSoloFitter::addNewFile(const QString &)
{
    //    STDLINE("addNewFile",ACRed);
    this->refresh() ;
    //this->collectExistingWidgets();
}

//=============================================================================
//Start Button:: called  if(Clicked() == true)
void HanSoloFitter::on_openFilePB_clicked()
{
    QString localPath = getenv("CHEWIEOUTPUTDIR");
    QStringList fileNames = QFileDialog::getOpenFileNames(this,"Event root tree files",localPath,"Root Tree Files(*.root)");
    if(fileNames.isEmpty())
        return;
    for(int f=0; f<fileNames.size(); f++)
    {
        TFile* file=TFile::Open(fileNames[f].toAscii(),"READ");
        this->addFile(fileNames[f].toStdString(),file);
    }

}
//===========================================================================
void HanSoloFitter::addItem(std::string item)
{
       STDLINE("addItem",ACRed);
    bool alreadyIncluded = false ;
    STDLINE(ui_->HanSoloFitterInputFile->count(), ACRed);

    for(int i=0; i<ui_->HanSoloFitterInputFile->count(); i++)
    {
        if( ui_->HanSoloFitterInputFile->itemText(i).toStdString() == emptyFileLabel_.toStdString()          ) STDLINE("New File: " + item, ACCyan) ;
        if( ui_->HanSoloFitterInputFile->itemText(i).toStdString() == displayAllLabel_.toStdString() && i > 0) STDLINE("Remove File: "+ item, ACPurple);
        if( ui_->HanSoloFitterInputFile->itemText(i).toStdString() == emptyFileLabel_.toStdString()          ) ui_->HanSoloFitterInputFile->setItemText(i,displayAllLabel_) ;
        if( ui_->HanSoloFitterInputFile->itemText(i).toStdString() == displayAllLabel_.toStdString() && i > 0) ui_->HanSoloFitterInputFile->removeItem(i) ;
        if( ui_->HanSoloFitterInputFile->itemText(i).toStdString() == item                                   ) alreadyIncluded = true ;
    }

    if( !alreadyIncluded )
    {
        STDLINE("Not Included:" + item, ACGreen);
        ui_->HanSoloFitterInputFile->addItem(QString::fromStdString(item)) ;
    }

    ui_->HanSoloFitterInputFile->setCurrentIndex(ui_->HanSoloFitterInputFile->count()-1);

    STDLINE("ui_->HanSoloFitterInputFile->count(): " + ui_->HanSoloFitterInputFile->count(), ACRed);
}

//===========================================================================
void HanSoloFitter::delItem(std::string item)
{
        STDLINE("delItem",ACRed);
    for(int i=0; i<ui_->HanSoloFitterInputFile->count(); i++)
    {
        if( ui_->HanSoloFitterInputFile->itemText(i).toStdString() == item )
        {
            ui_->HanSoloFitterInputFile->removeItem(i);
            break;
        }
    }

}
// Populate the folders navigation tool with the existing open files
// and their content and point to the current one
//===========================================================================
void HanSoloFitter::refresh()
{
    STDLINE("Refreshing the content of the file tree-structure navigator",ACCyan) ;

}
//===========================================================================
//========================File System Sidebar [End]==========================
//===========================================================================
//===========================================================================
void HanSoloFitter::addFile(std::string fileName, TFile* file)
{
    if( openFiles_.find(fileName) == openFiles_.end() )
    {
        if(file->IsOpen())
        {
            openFiles_[fileName] = file;
            this->addItem(fileName) ;
            STDLINE("file->IsOpen()", ACCyan)
            STDLINE(openFiles_[fileName], ACCyan)
        }
    }
    else if(!file->IsOpen())
    {
        openFiles_.erase(fileName);
        this->delItem(fileName);
        STDLINE("!file->IsOpen()", ACCyan);
    }

}

//=================================================================================
//===================Canvas Display Histogram Display [Start]======================
//=================================================================================
//=================================================================================
std::string HanSoloFitter::twoDOption(void)
{
    if( ui_->surfaceRB->isChecked() ) return "SURF4";
    if( ui_->contourRB->isChecked() ) return "CONT" ;
    if( ui_->legoRB   ->isChecked() ) return "LEGO4";
    if( ui_->lego2RB  ->isChecked() ) return "LEGO2";
    if( ui_->colzRB   ->isChecked() ) return "COLZ" ;
    return "" ;
}
//===========================================================================
bool HanSoloFitter::plotStatBox(void)
{
    //return false;
    return ui_->hanSoloStatCB->isChecked();
}
//===========================================================================
bool HanSoloFitter::plotFitBox(void)
{
    return false;
    //return ui_->fitCB->isChecked();
}
//===========================================================================
bool HanSoloFitter::logX(void)
{
    //return false;
    return ui_->logxCB->isChecked();
}

//===========================================================================
bool HanSoloFitter::logY(void)
{
    //return false;
    return ui_->logyCB->isChecked();
}

//===========================================================================
bool HanSoloFitter::logZ(void)
{
    //return false;
    return ui_->logzCB->isChecked();
}

//===========================================================================
std::string HanSoloFitter::canvasSplitType(void)
{
    if     (ui_->splitCanvasXRB->isChecked())
        return "X";
    else if(ui_->splitCanvasYRB->isChecked())
        return "Y";
    else
        return "XY";
}
CanvasWidget* HanSoloFitter::getCanvas()
{
    return (CanvasWidget*) canvas_;

}
//===========================================================================
QMdiSubWindow* HanSoloFitter::getDrawingWindow(void)
{
    return ui_->theCanvas;
}
//===========================================================================
/*void HanSoloFitter::showContextMenu(const QPoint &)
{
 // Create suitable canvas (a singleton)
    if()
    {
        CanvasWidget* canvas =new CanvasWidget(ui_->theCanvas) ;
        ss_.str(""); ss_ << "Current canvas: " << currentCanvas_ ;
        canvas->setTitle(ss_.str()) ;
        connect(canvas, SIGNAL(destroyed()    ),
                 this                          , SLOT  (cleanDestroy())) ;
    }
    else
    {

    }

    //cSw_[currentCanvas_] = (QMdiSubWindow*)  &pointerToHanSolo;
    // Add the canvas to the main window
    //if( cSw_.find(currentCanvas_) == cSw_.end())
    //{
        //QMdiSubWindow* pointerToHanSolo = theHanSoloFitter_->getCanvas();
        //cSw_[currentCanvas_] = theHanSoloFitter_->getCanvas();



        //cSw_[currentCanvas_] =  (QMdiSubWindow*) theMainWindow_->theHanSoloFitter_->ui_->theCanvas;
        //std::cout<< "Memory Location: " << cSw_[currentCanvas_] << " = csW_[currentCanvas] | theCanvas = " << theMainWindow_->theHanSoloFitter_->ui_->theCanvas << std::endl;
        //theMainWindow_->theHanSoloFitter_->ui_->theCanvas->mdiArea()->addSubWindow(serviceCanvas_[currentCanvas_], 0) ;
        //QMdiSubWindow * renderee =*cSw_[currentCanvas_];
        //renderee->render(this, theHanSoloFitter_->getCanvas());
       // cSw_[currentCanvas_]->setParent(theHanSoloFitter_);
        //cSw_[currentCanvas_]->setGeometry(canvasPosX_,canvasPosY_,canvasWitdh_,canvasHeight_) ;
        //canvasPosX_   += 5;
        //canvasPosY_   += 5;
    //}

    // Determine name -> full-path association for the selected items
    selectedObjectsDef selectedObjects = this->getSelectedItems() ;

    int numberOfPlots = 0 ;
    for (selectedObjectsDef::iterator it=selectedObjects.begin(); it!=selectedObjects.end(); ++it)
    {
        for(tFileVDef::iterator jt=it->second.begin(); jt!=it->second.end(); ++jt)
        {
            if( (*jt)->IsA() == TFolder::Class()        ) continue ;
            if( (*jt)->IsA() == TDirectory::Class()     ) continue ;
            if( (*jt)->IsA() == TDirectoryFile::Class() ) continue ;
            if( (*jt)->IsA() == TCanvas::Class()        ) continue ;
            numberOfPlots++ ;
        }
    }
    if(numberOfPlots == 0) return;



    // if( theHNavigator_->plotStatBox() ){

        gStyle->SetOptStat(111111) ;
        STDLINE("plotStatBox(): " +theHNavigator_->plotStatBox(), ACWhite);
    }else
        gStyle->SetOptStat(0) ;

    if( theHNavigator_->plotFitBox() )
    {
        gStyle->SetOptFit(111111) ;
        STDLINE("OptFit",ACRed);
    }
    else
        gStyle->SetOptFit(0) ;

    int nx = 1;
    int ny = 1;
    if(theHNavigator_->canvasSplitType() == "X")
        nx=numberOfPlots;
    else if(theHNavigator_->canvasSplitType() == "Y")
        ny=numberOfPlots;
    else if(theHNavigator_->canvasSplitType() == "XY")
    {
        if     ( numberOfPlots ==  1 ) {nx=1;ny=1;}
        else if( numberOfPlots ==  2 ) {nx=2;ny=1;}
        else if( numberOfPlots ==  3 ) {nx=3;ny=1;}
        else if( numberOfPlots ==  4 ) {nx=2;ny=2;}
        else if( numberOfPlots ==  5 ) {nx=3;ny=2;}
        else if( numberOfPlots ==  6 ) {nx=3;ny=2;}
        else if( numberOfPlots ==  7 ) {nx=3;ny=3;}
        else if( numberOfPlots ==  8 ) {nx=3;ny=3;}
        else if( numberOfPlots ==  9 ) {nx=3;ny=3;}
        else if( numberOfPlots == 10 ) {nx=4;ny=3;}
        else if( numberOfPlots == 11 ) {nx=4;ny=3;}
        else if( numberOfPlots == 12 ) {nx=4;ny=3;}
        else if( numberOfPlots == 13 ) {nx=4;ny=4;}
        else if( numberOfPlots == 14 ) {nx=4;ny=4;}
        else if( numberOfPlots == 15 ) {nx=4;ny=4;}
        else if( numberOfPlots == 16 ) {nx=4;ny=4;}
        else
        {
            ny = ceil(sqrt(numberOfPlots));
            nx = ny-1;
            if(nx*ny<numberOfPlots)
                ++nx;
        }
    }
    serviceCanvas_[currentCanvas_]->divide(nx, ny) ;
    serviceCanvas_[currentCanvas_]->setLogX(theHNavigator_->logX());
    serviceCanvas_[currentCanvas_]->setLogY(theHNavigator_->logY());
    serviceCanvas_[currentCanvas_]->setLogZ(theHNavigator_->logZ());


    std::string options = "" ;

    int pad = 1 ;
    for (selectedObjectsDef::iterator it=selectedObjects.begin(); it!=selectedObjects.end(); ++it)
    {
        for(tFileVDef::iterator jt=it->second.begin(); jt!=it->second.end(); ++jt)
        {
            if( (*jt)->IsA() == TDirectory::Class()     ) continue ;
            if( (*jt)->IsA() == TFolder::Class()        ) continue ;
            if( (*jt)->IsA() == TDirectoryFile::Class() ) continue ;
            if( (*jt)->IsA() == TCanvas::Class() )
            {
                (*jt)->Draw() ;
                continue ;
            }
            serviceCanvas_[currentCanvas_]->cd(pad++) ;
            if(this->getObjectType(*jt).find("TH1") != std::string::npos)
            {
                (*jt)->Draw() ;
 //               if (currentObject_) delete currentObject_;
                currentObject_ = (*jt);
            }
            else if(this->getObjectType(*jt).find("TH2") != std::string::npos)
            {
                options += theHNavigator_->twoDOption() ;
                (*jt)->Draw(options.c_str()) ;
//                if (currentObject_) delete currentObject_;
                currentObject_ = (*jt);
            }
            else if(this->getObjectType(*jt).find("TGraph") != std::string::npos)
            {
                options += "ACL" ;
                (*jt)->Draw(options.c_str()) ;
//                if (currentObject_) delete currentObject_;
                currentObject_ = (*jt);
            }
            else
            {
                STDLINE(std::string("Don't know how to plot object of type: ") + this->getObjectType(*jt),ACRed);
            }
        }
    }
    serviceCanvas_[currentCanvas_]->flush() ;

    //theMainWindow_->theHanSoloFitter_->duplicate(cSw_[currentCanvas_]);
    //QMdiSubWindow* pointerToHanSolo = (QMdiSubWindow*) theHanSoloFitter_->getCanvas();
    // *pointerToHanSolo = cSw_[currentCanvas_].copy();
    //cSw_[currentCanvas_] = (QMdiSubWindow*)  &pointerToHanSolo;
    // *pointerToHanSolo->show() ;
    // *pointerToHanSolo ->raise() ;
    // * pointerToHanSolo->setWidget(serviceCanvas_[currentCanvas_]);
    //theMainWindow_->theHanSoloFitter_->ui_->theCanvas->setWidget(cSw_[currentCanvas_]);
    // *cSw_[currentCanvas_]->setWindowTitle("Updated");
    //cSw_[currentCanvas_]->setVisible(true);
    //cSw_[currentCanvas_]->raise() ;
    STDLINE("Ryan doesn't believe me: if this prints, I'll buy you a coke!", ACWhite);
}
*/
//=================================================================================
//===================Canvas Display Histogram Display [End]========================
//=================================================================================
//=================================================================================
