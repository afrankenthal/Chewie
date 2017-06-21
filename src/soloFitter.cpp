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

#include "solofitter.h"
//#include "ui_soloFitter.h"
#include "analyzerdlg.h"
#include "Utilities.h"

#include "MessageTools.h"

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

#include <QtWidgets/QFileDialog>
#include <QtCore/QList>
#include <QtWidgets/QVBoxLayout>

//===========================================================================
SoloFitter::SoloFitter(QWidget * parent) :
    QTabWidget          (parent)
  , theMainWindow_      ((MainWindow*)parent)
  , timer_              (new QTimer(this))
  , ui_                 (new Ui::(SoloFitter))
  , theHTreeBrowser_    (0)
  , emptyFileLabel_     ("No files are currently open")
  , displayAllLabel_    ("Display all")
  , displayMemoryLabel_ ("Display memory resident objects")
  , counter_            (0)
  , shiftHSValue_       (0)
{
    ui_->setupUi(this);

    this->addItem(emptyFileLabel_.toStdString()) ;

    this->collectExistingWidgets();

    shiftHSValue_ = ui_->shiftHS->value();

    connect(ui_->soloFitterInputFileCB,
            SIGNAL(currentIndexChanged(QString)),
            this,
            SLOT(  updateTree(         QString))) ;


    connect(timer_,
            SIGNAL(timeout                () ),
            this,
            SLOT  (checkNewObjectsInMemory()));

    for(int i=0; i<10; ++i)
    {
        ss_.str(""); ss_ << i ;
        ui_->hCanvasCB->addItem(QString(ss_.str().c_str()));
    }

    ui_->canvasSizeCB->addItem(QString("Small" ));
    ui_->canvasSizeCB->addItem(QString("Medium"));
    ui_->canvasSizeCB->addItem(QString("Large" ));
    ui_->canvasSizeCB->addItem(QString("Huge"  ));
    ui_->canvasSizeCB->setCurrentIndex(2);

    gStyle->SetPalette(1,0) ;

    timer_->start(1000) ;

    theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    theParamManager_[0]->setParName("Choose a function");

    ui_->parametersTW->insertTab(0, theParamManager_[0], "Par 0");
    ui_->fitMethodLE->setText("L");
    ui_->fitLimitCB->setChecked(false);
    emit (on_fitLimitCB_clicked(false));
    numberOfFitting_ = 0;
    numberofCanvas_  = 0;

    ui_->fitFuncLW->insertItem(0, "Eta");
    fitFunctions_["Eta"] =  6;
    ui_->fitFuncLW->insertItem(1, "Eta (No Depletion)");
    fitFunctions_["Eta (No Depletion)"] =  4;
    ui_->fitFuncLW->insertItem(2, "Eta Derivative");
    fitFunctions_["Eta Derivative"] = 5;//integrandEtaFitFunc2Angle
    ui_->fitFuncLW->insertItem(3, "Eta with Cut");
    fitFunctions_["Eta with Cut"] = 7;
    ui_->fitFuncLW->insertItem(4, "Eta (No Depletion) with Cut");
    fitFunctions_["Eta (No Depletion) with Cut"] = 5;
    ui_->fitFuncLW->insertItem(5, "Eta Inverse");
    fitFunctions_["Eta Inverse"] = 5;
    ui_->fitFuncLW->insertItem(6, "Langaus");
    fitFunctions_["Langaus"] = 4;
    ui_->fitFuncLW->insertItem(7, "Gauss");
//  Directly declared as a string
    ui_->fitFuncLW->insertItem(8, "Gauss Convoluted with Constant");
    fitFunctions_["Gauss Convoluted with Constant"] = 4;
    ui_->fitFuncLW->insertItem(9, "Eta Convoluted with Gauss");
    fitFunctions_["Eta Convoluted with Gauss"] = 9;
    ui_->fitFuncLW->insertItem(10,"Gauss-constant plus Constant");
    fitFunctions_["Gauss-constant plus Constant"] = 5;
    ui_->fitFuncLW->insertItem(11,"Single constant Gauss-conv.");
    fitFunctions_["Single constant Gauss-conv."] = 6;
    ui_->fitFuncLW->insertItem(12,"Double constant Gauss-conv.");
    fitFunctions_["Double constant Gauss-conv."] = 7;
    ui_->fitFuncLW->insertItem(13,"Eta Reproduction");
    fitFunctions_["Eta Reproduction"] = 11;
    ui_->fitFuncLW->insertItem(14,"Eta Distribution");
    fitFunctions_["Eta Distribution"] = 6;
}

//===========================================================================
SoloFitter::~SoloFitter()
{
    timer_->stop();
    delete timer_ ;
    if(theHTreeBrowser_)
    {
        delete theHTreeBrowser_;
        theHTreeBrowser_ = 0;
    }
    delete ui_;
}

//===========================================================================
void SoloFitter::on_openFilePB_clicked()
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
void SoloFitter::checkNewObjectsInMemory(void)
{
    //    STDLINE("Checking new objects",ACRed);
    if(theHTreeBrowser_)
        theHTreeBrowser_->populate(gROOT->GetRootFolder()) ;
    this->fillWidget();
}

//===========================================================================
void SoloFitter::updateTree(QString currentFile)
{
    //STDLINE(currentFile.toStdString(),ACRed);
    theHTreeBrowser_->clear();
    if( currentFile != displayAllLabel_ )
    {
        this->fillWidgetTree(currentFile.toStdString());
    }
    else
    {
        for(int item=0; item < ui_->soloFitterInputFileCB->count(); ++item)
        {
            this->fillWidgetTree(ui_->soloFitterInputFileCB->itemText(item).toStdString());
        }
    }
}

//===========================================================================
void SoloFitter::collectExistingWidgets()
{
    //    STDLINE("collectExistingWidgets",ACRed);

    if( !theHTreeBrowser_ )
    {
        // Create a tree-like folder-navigation tool
        //    this->setGeometry(this->x(),this->y(),this->width(),this->height()+120) ;
        this->show() ;
        this->setCurrentIndex(0);
        QWidget * input  = this->widget(0) ;
        theHTreeBrowser_ = new hTreeBrowser(input, theMainWindow_) ;
        theHTreeBrowser_->setGeometry(ui_->soloFitterTreeFrame->geometry()) ;
        theHTreeBrowser_->show() ;
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
QMdiArea* SoloFitter::getMdiArea(void)
{
    return theMainWindow_->getMdiArea();
}

//===========================================================================
MainWindow* SoloFitter::getMainWindow(void)
{
    return theMainWindow_;
}

//===========================================================================
// Fill the combo-box with the list of currently open root files
void SoloFitter::fillWidget()
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
        this->fillWidgetTree(ui_->soloFitterInputFileCB->currentText().toStdString());
    }
}

//===========================================================================
void SoloFitter::addFile(std::string fileName, TFile* file)
{
    if( openFiles_.find(fileName) == openFiles_.end() )
    {
        if(file->IsOpen())
        {
            openFiles_[fileName] = file;
            this->addItem(fileName) ;
        }
    }
    else if(!file->IsOpen())
    {
        openFiles_.erase(fileName);
        this->delItem(fileName);
    }

}

//===========================================================================
// Fill the tree
void SoloFitter::fillWidgetTree(std::string currentFile)
{
    //STDLINE(std::string("fillWidgetTree. File: ") + currentFile,ACRed);
    //STDLINE(openFiles_.size(),ACPurple);
    if(openFiles_.size() == 0)
        return;
    TFile* file  = openFiles_[currentFile] ;
    theHTreeBrowser_->setCurrentFile(file);
    theHTreeBrowser_->populate((TDirectory *)file) ;
}

//===========================================================================
void SoloFitter::addNewFile(const QString &)
{
    //    STDLINE("addNewFile",ACRed);
    this->refresh() ;
    this->collectExistingWidgets();
}

//===========================================================================
void SoloFitter::refresh()
{
    STDLINE("Refreshing the content of the file tree-structure navigator",ACCyan) ;

    // Populate the folders navigation tool with the existing open files
    // and their content and point to the current one
    //  if( !theTabWidget_ )
    //  {
    //    return ;
    //  }
    //  else
    //  {
    //    theFileEater_ = theTabWidget_->getFileEater() ;
    //  }
}

//===========================================================================
void SoloFitter::addItem(std::string item)
{
    //    STDLINE("addItem",ACRed);
    bool alreadyIncluded = false ;

    for(int i=0; i<ui_->soloFitterInputFileCB->count(); i++)
    {
        if( ui_->soloFitterInputFileCB->itemText(i).toStdString() == emptyFileLabel_.toStdString()          ) ui_->soloFitterInputFileCB->setItemText(i,displayAllLabel_) ;
        if( ui_->soloFitterInputFileCB->itemText(i).toStdString() == displayAllLabel_.toStdString() && i > 0) ui_->soloFitterInputFileCB->removeItem(i) ;
        if( ui_->soloFitterInputFileCB->itemText(i).toStdString() == item                                   ) alreadyIncluded = true ;
    }

    if( !alreadyIncluded )
    {
        ui_->soloFitterInputFileCB->addItem(QString::fromStdString(item)) ;
    }

    ui_->soloFitterInputFileCB->setCurrentIndex(ui_->soloFitterInputFileCB->count()-1);
}

//===========================================================================
void SoloFitter::delItem(std::string item)
{
    //    STDLINE("delItem",ACRed);
    for(int i=0; i<ui_->soloFitterInputFileCB->count(); i++)
    {
        if( ui_->soloFitterInputFileCB->itemText(i).toStdString() == item )
        {
            ui_->soloFitterInputFileCB->removeItem(i);
            break;
        }
    }

    //    ui_->soloFitterInputFileCB->setCurrentIndex(ui_->soloFitterInputFileCB->count()-1);
}

//===========================================================================
void SoloFitter::resizeEvent(QResizeEvent * )
{
    //STDLINE("resizeEvent",ACRed);
    /* +-O-------------------------------+
       |                                 |
       | A-----------------------------a |
       | |            file             | |
       | B-----------------------------b |
       |                                 |
       | A-----------------------------a |
       | |            comb             | |
       | B-----------------------------b |
       |                                 |
       | C-----------------------------c |
       | |                             | |
       | |            fram             | |
       | |                             | |
       | |                             | |
       | D-----------------------------d |
       |                                 |
       | E-----------------------------e |
       | |            grbx             | |
       | F-----------------------------f |
       |                                 |
       +---------------------------------+

       A: topComb
       B: botComb
       C: topFram
       D: botFram
       E: topGrbx
       F: botGrbx

       AB: combRect.height()
       CD: framRect.height()
       EF: grbxRect.height()

       OA: topComb
       BC: topFram - botComb
       DE: topGrbx - botFram (dYGrbx)

   */
    QRect thisRect = this->geometry() ;

    QRect fileRect = ui_->openFilePB           ->geometry() ;
    QRect combRect = ui_->soloFitterInputFileCB->geometry() ;
    QRect framRect = ui_->soloFitterTreeFrame  ->geometry() ;
    QRect grbxRect = ui_->soloFitterCanvasGB   ->geometry() ;

    int topComb = combRect.y() ; int botComb = topComb + combRect.height() ;
    int topFram = framRect.y() ; int botFram = topFram + framRect.height() ;
    int topGrbx = grbxRect.y() ; int botGrbx = topGrbx + grbxRect.height() ;

    int dw = fileRect.height() + combRect.height() + grbxRect.height() + 30 ; // 40 is the total amount of vertical space between components

    // Adjust for width stretching first
    ui_->soloFitterInputFileCB->setGeometry(combRect.x(), combRect.y(), thisRect.width()-5, combRect.height()) ;
    ui_->soloFitterTreeFrame  ->setGeometry(framRect.x(), framRect.y(), thisRect.width()-5, framRect.height()) ;

    // Adjust for height stretching the only strechable part (the frame)
    framRect = ui_->soloFitterTreeFrame  ->geometry() ;
    ui_->soloFitterTreeFrame  ->setGeometry(framRect.x(), framRect.y(), framRect.width()   , thisRect.height()-dw) ; // Consider room for components without vertical stretch

    // Recompute current corners for vertical stretch
    combRect = ui_->soloFitterInputFileCB->geometry() ;
    framRect = ui_->soloFitterTreeFrame  ->geometry() ;

    topComb = combRect.y()    ; botComb = topComb + combRect.height() ;
    topFram = framRect.y()    ; botFram = topFram + framRect.height() ;
    topGrbx = botFram         ; botGrbx = topGrbx + grbxRect.height() ;

    // Adjust for vertical stretching
    //  if( theHTreeBrowser_ ) // At first call this component is not yet existing
    //    theHTreeBrowser_       ->setGeometry(framRect.x(), framRect.y(), thisRect.width()-5, framRect.height() ) ;

    ui_->soloFitterCanvasGB   ->setGeometry(grbxRect.x(), topGrbx     , thisRect.width()-5, grbxRect.height() ) ;

    int currentIndex = this->currentIndex() ;
    if( currentIndex == 0 )
    {
        this->setCurrentIndex(1);
        this->setCurrentIndex(0);
    }
    else
    {
        this->setCurrentIndex(0);
        this->setCurrentIndex(currentIndex);
    }

}

//===========================================================================
std::string SoloFitter::twoDOption(void)
{
    if( ui_->surfaceRB->isChecked() ) return "SURF4";
    if( ui_->contourRB->isChecked() ) return "CONT" ;
    if( ui_->legoRB   ->isChecked() ) return "LEGO4";
    if( ui_->lego2RB  ->isChecked() ) return "LEGO2";
    if( ui_->colzRB   ->isChecked() ) return "COLZ" ;
    return "" ;
}

//===========================================================================
void SoloFitter::on_saveComponentsPB_clicked()
{
//    if ((*theHTreeBrowser_->getSelectedItems().begin()->second.begin())->GetName())
//    {
//        STDLINE("No histogram to save", ACRed);
        //return;
//    }

     //Ask for output file name
    std::stringstream ss;
    ss << "$CHEWIEFITOUTPUTDIR/" << theHTreeBrowser_->getFileName() << "|" << (*theHTreeBrowser_->getSelectedItems().begin()->second.begin())->GetName() << "_fitted.root";
      QString fileName = QFileDialog::getSaveFileName(this,
                                                      tr("Save File"),
                                                      (QString)ss.str().c_str(),
                                                      tr("Root files (*.root)"));
      if(fileName.isEmpty()) return ;

      TFile * tmpFile = new TFile(fileName.toStdString().c_str(),"recreate") ;

      // Get list of selected items to dump into new file
      hTreeBrowser::selectedObjectsDef selectedObjects = theHTreeBrowser_->getSelectedItems()  ;

      // Build the folder structure (if requested, otherwise dump a flat list)
      if( !ui_->flattenHierarchyCB->isChecked())
      {
        for(hTreeBrowser::selectedObjectsDef::iterator it=selectedObjects.begin(); it!=selectedObjects.end(); ++it)
        {
          tmpFile->cd() ; // Restart always from top directory
          std::string slashTerminatedFullPath = it->first + std::string("/") ;
          ss_.str("") ;
          ss_ << "makeDir(" << slashTerminatedFullPath << ")" ;
          STDLINE(ss_.str(),ACWhite) ;
          this->makeDir(slashTerminatedFullPath) ;
        }
      }

      // Save each selected item (if requested, into the appropriate folder)
      for(hTreeBrowser::selectedObjectsDef::iterator it=selectedObjects.begin(); it!=selectedObjects.end(); ++it)
      {

        for(hTreeBrowser::tFileVDef::iterator jt=it->second.begin(); jt!=it->second.end(); ++jt)
        {
          tmpFile->cd() ; // Restart always from top directory
          if( !ui_->flattenHierarchyCB->isChecked())
          {
            tmpFile->cd(it->first.c_str()) ;
          }

          if( (*jt)->IsA() == TFolder::Class() ) continue ;

                (*jt)->Write() ;
        }
      }

      tmpFile->Close() ;
      delete tmpFile ;
}

//===========================================================================
void SoloFitter::makeDir(std::string dirName)
{
    // Recursive method: builds a directory structure into the currently open file
    //                   by scanning the elements names in dirName
    boost::cmatch what;
    static const boost::regex exp("(.*?)/(.*)", boost::regex::perl);

    std::string match = "" ;
    std::string rest  = "" ;

    // Split directory name according to the '/' character
    if( boost::regex_match(dirName.c_str(), what, exp, boost::match_extra) )
    {
        match = what[1] ;
        rest  = what[2] ;
        TKey * key = gDirectory->FindKey(match.c_str()) ;
        if( key )
        {
            gDirectory->cd(match.c_str()) ;
        }
        else
        {
            gDirectory->mkdir(match.c_str())->cd() ;
        }
        if( !rest.empty()) this->makeDir(rest) ; // If there are still additional components in the directory name, continue splitting
    }
}

//===========================================================================
void SoloFitter::on_hCanvasCB_activated(int index)
{
    //    STDLINE("on_hCanvasCB_activated",ACRed);
    theHTreeBrowser_->setCurrentCanvas(index) ;
}

//===========================================================================
void SoloFitter::on_canvasSizeCB_activated(QString size)
{
    theHTreeBrowser_->setCanvasSize(size.toStdString()) ;
}

//===========================================================================
bool SoloFitter::plotStatBox(void)
{
    return ui_->statCB->isChecked();
}

//===========================================================================
bool SoloFitter::plotFitBox(void)
{
    return ui_->fitCB->isChecked();
}

//===========================================================================
bool SoloFitter::logX(void)
{
    return ui_->logxCB->isChecked();
}

//===========================================================================
bool SoloFitter::logY(void)
{
    return ui_->logyCB->isChecked();
}

//===========================================================================
bool SoloFitter::logZ(void)
{
    return ui_->logzCB->isChecked();
}

//===========================================================================
std::string SoloFitter::canvasSplitType(void)
{
    if     (ui_->splitCanvasXRB->isChecked())
        return "X";
    else if(ui_->splitCanvasYRB->isChecked())
        return "Y";
    else
        return "XY";
}

//===========================================================================
void SoloFitter::checkBoxesHandler(bool)
{
    //STDLINE("Bool",ACRed);
    theHTreeBrowser_->showContextMenu(QPoint());
}

//===========================================================================
void SoloFitter::checkBoxesHandler(int)
{
    //STDLINE("Int",ACGreen);
    theHTreeBrowser_->showContextMenu(QPoint());
}

//===========================================================================
void SoloFitter::on_unZoomPB_clicked()
{
    theHTreeBrowser_->unZoom() ;
}

//===========================================================================
void SoloFitter::on_setRangePB_clicked()
{
    theHTreeBrowser_->setRange( ui_->xMinLE->text().toFloat(),ui_->xMaxLE->text().toFloat()
                              , ui_->yMinLE->text().toFloat(),ui_->yMaxLE->text().toFloat()
                              , ui_->zMinLE->text().toFloat(),ui_->zMaxLE->text().toFloat());
}

//===========================================================================
void SoloFitter::on_shiftPlusPB_clicked()
{
    float shiftMin = ui_->xMinLE->text().toFloat()+ui_->shiftLE->text().toFloat();
    float shiftMax = ui_->xMaxLE->text().toFloat()+ui_->shiftLE->text().toFloat();
    ui_->xMinLE->setText(QString("%1").arg(shiftMin));
    ui_->xMaxLE->setText(QString("%1").arg(shiftMax));
    emit on_setRangePB_clicked();
}

//===========================================================================
void SoloFitter::on_shiftMinusPB_clicked()
{
    float shiftMin = ui_->xMinLE->text().toFloat()-ui_->shiftLE->text().toFloat();
    if(shiftMin < 0)
        shiftMin = 0;
    float shiftMax = ui_->xMaxLE->text().toFloat()-ui_->shiftLE->text().toFloat();
    if(shiftMax < 0)
        shiftMax = 0;
    ui_->xMinLE->setText(QString("%1").arg(shiftMin));
    ui_->xMaxLE->setText(QString("%1").arg(shiftMax));
    emit on_setRangePB_clicked();
}

//===========================================================================
void SoloFitter::on_shiftHS_valueChanged(int value)
{
//    STDLINE(shiftHSValue_,ACRed);
//    STDLINE(value,ACPurple);
    int delta = value - shiftHSValue_;

    float shiftMin = ui_->xMinLE->text().toFloat()+delta*ui_->shiftLE->text().toFloat();
    //if(shiftMin < 0)
    //    shiftMin = 0;
    float shiftMax = ui_->xMaxLE->text().toFloat()+delta*ui_->shiftLE->text().toFloat();
    //if(shiftMax < 0)
    //    shiftMax = 0;
    ui_->xMinLE->setText(QString("%1").arg(shiftMin));
    ui_->xMaxLE->setText(QString("%1").arg(shiftMax));
//    STDLINE(shiftMin,ACRed);
//    STDLINE(shiftMax,ACGreen);

    shiftHSValue_ = value;
    emit on_setRangePB_clicked();
}

void SoloFitter::on_fitFuncLW_itemDoubleClicked(QListWidgetItem *item)
{
    if (ui_->customFunctionCB->isChecked()) return;

    std::stringstream ss;

    for (unsigned int j = 0; j < theParamManager_.size(); ++j)
    {
        delete theParamManager_[j];
        ui_->parametersTW->removeTab(j);
    }
    theParamManager_.clear();

    if (item->text() == "Eta")
    {
        for (int i = 0; i < 6; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Thickness                        ");
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("2*mu*DepletionVoltage            ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(9.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParName  ("DepletionVoltage                 ");
                theParamManager_[i]->setParValue (25.);
                theParamManager_[i]->setParLimInf(10.);
                theParamManager_[i]->setParLimSup(100.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1.);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("AppliedVoltage + DepletionVoltage");
                theParamManager_[i]->setParValue (525.);
                theParamManager_[i]->setParLimInf(510.);
                theParamManager_[i]->setParLimSup(600.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1.);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParPrecision(5);
                theParamManager_[i]->setParStep(0.00001);
                theParamManager_[i]->setParName  ("4*DiffusionConstant              ");
                theParamManager_[i]->setParValue (0.0007);
                theParamManager_[i]->setParLimInf(0.00001);
                theParamManager_[i]->setParLimSup(0.3);
                theParamManager_[i]->setParFixed (false);
            }
            else if (i == 5)
            {
                theParamManager_[i]->setParName  ("ScaleFactor                      ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Eta (No Depletion)")
    {
        for (int i = 0; i < 4; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Thickness                     ");
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                theParamManager_[i]->setParValue (6.5);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(30.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParPrecision(5);
                theParamManager_[i]->setParStep(0.00001);
                theParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                theParamManager_[i]->setParValue (0.0007);
                theParamManager_[i]->setParLimInf(0.00001);
                theParamManager_[i]->setParLimSup(0.3);
                theParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("ScaleFactor                   ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );
        }
    }

    else if (item->text() == "Eta Derivative")
    {
        for (int i = 0; i < 5; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Thickness                     ");
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                theParamManager_[i]->setParValue (6.5);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(30.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParPrecision(5);
                theParamManager_[i]->setParStep(0.00001);
                theParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                theParamManager_[i]->setParValue (0.0007);
                theParamManager_[i]->setParLimInf(0.00001);
                theParamManager_[i]->setParLimSup(0.3);
                theParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("ScaleFactor                   ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParName  ("Tan(angle)                    ");
                theParamManager_[i]->setParValue (0.);
                theParamManager_[i]->setParLimits(-100, 100);
                theParamManager_[i]->setParLimInf(-1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(4);
                theParamManager_[i]->setParStep(0.0001);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Eta with Cut")
    {
        for (int i = 0; i < 7; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Thickness                        ");
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("2*mu*DepletionVoltage            ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(9.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParName  ("DepletionVoltage                 ");
                theParamManager_[i]->setParValue (25.);
                theParamManager_[i]->setParLimInf(10.);
                theParamManager_[i]->setParLimSup(100.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1.);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("AppliedVoltage + DepletionVoltage");
                theParamManager_[i]->setParValue (525.);
                theParamManager_[i]->setParLimInf(510.);
                theParamManager_[i]->setParLimSup(600.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1.);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParPrecision(5);
                theParamManager_[i]->setParStep(0.00001);
                theParamManager_[i]->setParName  ("4*DiffusionConstant              ");
                theParamManager_[i]->setParValue (0.0007);
                theParamManager_[i]->setParLimInf(0.00001);
                theParamManager_[i]->setParLimSup(0.3);
                theParamManager_[i]->setParFixed (false);
            }
            else if (i == 5)
            {
                theParamManager_[i]->setParName  ("ScaleFactor                      ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 6)
            {
                theParamManager_[i]->setParName  ("CutThreashold                    ");
                theParamManager_[i]->setParValue (0.0005);
                theParamManager_[i]->setParLimInf(0.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(4);
                theParamManager_[i]->setParStep(0.0001);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Eta (No Depletion) with Cut")
    {
        for (int i = 0; i < 5; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Thickness                     ");
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                theParamManager_[i]->setParValue (6.5);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(30.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParPrecision(5);
                theParamManager_[i]->setParStep(0.00001);
                theParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                theParamManager_[i]->setParValue (0.0007);
                theParamManager_[i]->setParLimInf(0.00001);
                theParamManager_[i]->setParLimSup(0.3);
                theParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("ScaleFactor                   ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParName  ("CutThreashold                 ");
                theParamManager_[i]->setParValue (0.0005);
                theParamManager_[i]->setParLimInf(0.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(4);
                theParamManager_[i]->setParStep(0.0001);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );
        }
    }

    else if (item->text() == "Eta Inverse")
    {
        for (int i = 0; i < 5; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Thickness                     ");
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                theParamManager_[i]->setParValue (6.5);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(30.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParPrecision(5);
                theParamManager_[i]->setParStep(0.00001);
                theParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                theParamManager_[i]->setParValue (0.0007);
                theParamManager_[i]->setParLimInf(0.00001);
                theParamManager_[i]->setParLimSup(0.3);
                theParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("ScaleFactor                   ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParName  ("Tan(theta)                    ");
                theParamManager_[i]->setParLimits(-100, 100);
                theParamManager_[i]->setParValue (0);
                theParamManager_[i]->setParLimInf(-1);
                theParamManager_[i]->setParLimSup(1);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(4);
                theParamManager_[i]->setParStep(0.0001);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Langaus")
    {
        for (int i = 0; i < 4; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Width");
                theParamManager_[i]->setParLimits(10, 10000);
                theParamManager_[i]->setParValue (3000);
                theParamManager_[i]->setParLimInf(1000);
                theParamManager_[i]->setParLimSup(10000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(10);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("MPV");
                theParamManager_[i]->setParLimits(10, 100000);
                theParamManager_[i]->setParValue (20000);
                theParamManager_[i]->setParLimInf(1000);
                theParamManager_[i]->setParLimSup(30000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(10);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParName  ("Area");
                theParamManager_[i]->setParLimits(10000, 1000000000);
                theParamManager_[i]->setParValue (10000000);
                theParamManager_[i]->setParLimInf(100000);
                theParamManager_[i]->setParLimSup(1000000000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1000);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("GSigma");
                theParamManager_[i]->setParLimits(10, 100000);
                theParamManager_[i]->setParValue (3000);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(10000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(10);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Gauss")
    {
        for (int i = 0; i < 4; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Constant");
                theParamManager_[i]->setParLimits(0, 100000);
                theParamManager_[i]->setParValue (1);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(1000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(1);
                theParamManager_[i]->setParStep(0.1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Mean");
                theParamManager_[i]->setParLimits(-100000, 100000);
                theParamManager_[i]->setParValue (1);
                theParamManager_[i]->setParLimInf(-100);
                theParamManager_[i]->setParLimSup(100);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(1);
                theParamManager_[i]->setParStep(0.1);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParName  ("Sigma");
                theParamManager_[i]->setParValue (1);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(100.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("Vertical Translation");
                theParamManager_[i]->setParValue (0);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(1000.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Gauss Convoluted with Constant")
    {
        for (int i = 0; i < 4; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Pixel Limit");
                theParamManager_[i]->setParLimits(-100, 100);
                theParamManager_[i]->setParValue (50);
                theParamManager_[i]->setParLimInf(50);
                theParamManager_[i]->setParLimSup(75);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            if (i == 1)
            {
                theParamManager_[i]->setParName  ("Cluster 1 extension");
                theParamManager_[i]->setParValue (40);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(50);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            if (i == 2)
            {
                theParamManager_[i]->setParName  ("Sigma Gauss");
                theParamManager_[i]->setParValue (5);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(75);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            if (i == 3)
            {
                theParamManager_[i]->setParName  ("Scale Factor");
                theParamManager_[i]->setParLimits(0, 100000);
                theParamManager_[i]->setParValue (100);
                theParamManager_[i]->setParLimInf(10);
                theParamManager_[i]->setParLimSup(10000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );
        }
    }

        else if (item->text() == "Eta Convoluted with Gauss")
        {
            for (int i = 0; i < 9; ++i)
            {
                theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
        //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

                if (i == 0)
                {
                    theParamManager_[i]->setParName  ("Thickness                     ");
                    theParamManager_[i]->setParValue (500);
                    theParamManager_[i]->setParLimInf(500);
                    theParamManager_[i]->setParLimSup(500);
                    theParamManager_[i]->setParFixed (true);
                    theParamManager_[i]->setParPrecision(0);
                    theParamManager_[i]->setParStep(1);
                }
                else if (i == 1)
                {
                    theParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage) ");
                    theParamManager_[i]->setParValue (6.5);
                    theParamManager_[i]->setParLimInf(0.1);
                    theParamManager_[i]->setParLimSup(30.);
                    theParamManager_[i]->setParFixed (false);
                    theParamManager_[i]->setParPrecision(2);
                    theParamManager_[i]->setParStep(0.01);
                }
                else if (i == 2)
                {
                    theParamManager_[i]->setParPrecision(5);
                    theParamManager_[i]->setParStep(0.00001);
                    theParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                    theParamManager_[i]->setParValue (0.0700);
                    theParamManager_[i]->setParLimInf(0.00001);
                    theParamManager_[i]->setParLimSup(0.3);
                    theParamManager_[i]->setParFixed (false);
                }
                else if (i == 3)
                {
                    theParamManager_[i]->setParName  ("ScaleFactor                   ");
                    theParamManager_[i]->setParValue (1.);
                    theParamManager_[i]->setParLimInf(1.);
                    theParamManager_[i]->setParLimSup(1.);
                    theParamManager_[i]->setParFixed (true);
                    theParamManager_[i]->setParPrecision(2);
                    theParamManager_[i]->setParStep(0.01);
                }
                else if (i == 4)
                {
                    theParamManager_[i]->setParName  ("Extension of constant function");
                    theParamManager_[i]->setParLimits(0, 1000);
                    theParamManager_[i]->setParValue (4.);
                    theParamManager_[i]->setParLimInf(0.);
                    theParamManager_[i]->setParLimSup(50);
                    theParamManager_[i]->setParFixed (false);
                    theParamManager_[i]->setParPrecision(3);
                    theParamManager_[i]->setParStep(0.001);
                }
                else if (i == 5)
                {
                    theParamManager_[i]->setParName  ("Sigma                         ");
                    theParamManager_[i]->setParLimits(0,100);
                    theParamManager_[i]->setParValue (10);
                    theParamManager_[i]->setParLimInf(1);
                    theParamManager_[i]->setParLimSup(100);
                    theParamManager_[i]->setParFixed (false);
                    theParamManager_[i]->setParPrecision(1);
                    theParamManager_[i]->setParStep(0.1);
                }
                else if (i == 6)
                {
                    theParamManager_[i]->setParName  ("Scale Factor                  ");
                    theParamManager_[i]->setParLimits(0, 10000);
                    theParamManager_[i]->setParValue (400);
                    theParamManager_[i]->setParLimInf(100);
                    theParamManager_[i]->setParLimSup(10000);
                    theParamManager_[i]->setParFixed (false);
                    theParamManager_[i]->setParPrecision(0);
                    theParamManager_[i]->setParStep(1);
                }
                else if (i == 7)
                {
                    theParamManager_[i]->setParName  ("Center of distribution        ");
                    theParamManager_[i]->setParLimits(-100, 100);
                    theParamManager_[i]->setParValue (0);
                    theParamManager_[i]->setParLimInf(-100);
                    theParamManager_[i]->setParLimSup(100);
                    theParamManager_[i]->setParFixed (false);
                    theParamManager_[i]->setParPrecision(1);
                    theParamManager_[i]->setParStep(0.1);
                }
                else if (i == 8)
                {
                    theParamManager_[i]->setParName  ("Vertical translation Constant ");
                    theParamManager_[i]->setParValue (100);
                    theParamManager_[i]->setParLimInf(0);
                    theParamManager_[i]->setParLimSup(1000);
                    theParamManager_[i]->setParFixed (false);
                    theParamManager_[i]->setParPrecision(0);
                    theParamManager_[i]->setParStep(1);
                }

                connect(theParamManager_[i]->getParFixed(),
                        SIGNAL(clicked() ),
                        theParamManager_[i],
                        SLOT(on_fixPar_clicked())
                        );

        }
    }

    else if (item->text() == "Single constant Gauss-conv.")
    {
        for (int i = 0; i < 6; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Pixel Extension");
                theParamManager_[i]->setParValue (75);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(100);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Center");
                theParamManager_[i]->setParLimits(-100, 100);
                theParamManager_[i]->setParValue (0);
                theParamManager_[i]->setParLimInf(-10);
                theParamManager_[i]->setParLimSup(10);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParName  ("Well Width");
                theParamManager_[i]->setParValue (14);
                theParamManager_[i]->setParLimInf(1);
                theParamManager_[i]->setParLimSup(30);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("Max Value");
                theParamManager_[i]->setParValue (1);
                theParamManager_[i]->setParLimInf(0.);
                theParamManager_[i]->setParLimSup(10);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(3);
                theParamManager_[i]->setParStep(0.001);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParName  ("Min Value");
                theParamManager_[i]->setParValue (0.6);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(10);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(3);
                theParamManager_[i]->setParStep(0.001);
            }
            else if (i == 5)
            {
                theParamManager_[i]->setParName  ("Gauss Sigma");
                theParamManager_[i]->setParValue (10);
                theParamManager_[i]->setParLimInf(1);
                theParamManager_[i]->setParLimSup(100);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Double constant Gauss-conv.")
    {
        for (int i = 0; i < 7; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Pixel Extension");
                theParamManager_[i]->setParValue (75);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(100);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Center");
                theParamManager_[i]->setParLimits(-100, 100);
                theParamManager_[i]->setParValue (0);
                theParamManager_[i]->setParLimInf(-10);
                theParamManager_[i]->setParLimSup(10);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParName  ("Well Center");
                theParamManager_[i]->setParValue (40);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(75);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("Well Width");
                theParamManager_[i]->setParValue (14);
                theParamManager_[i]->setParLimInf(1);
                theParamManager_[i]->setParLimSup(30);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParName  ("Max Value");
                theParamManager_[i]->setParValue (1);
                theParamManager_[i]->setParLimInf(0.);
                theParamManager_[i]->setParLimSup(10);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(3);
                theParamManager_[i]->setParStep(0.001);
            }
            else if (i == 5)
            {
                theParamManager_[i]->setParName  ("Min Value");
                theParamManager_[i]->setParValue (0.6);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(10);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(3);
                theParamManager_[i]->setParStep(0.001);
            }
            else if (i == 6)
            {
                theParamManager_[i]->setParName  ("Gauss Sigma");
                theParamManager_[i]->setParValue (10);
                theParamManager_[i]->setParLimInf(1);
                theParamManager_[i]->setParLimSup(100);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Gauss-constant plus Constant")
    {
        for (int i = 0; i < 5; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Extension of constant function");
                theParamManager_[i]->setParLimits(0, 100000);
                theParamManager_[i]->setParValue (4.);
                theParamManager_[i]->setParLimInf(0.);
                theParamManager_[i]->setParLimSup(50);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(3);
                theParamManager_[i]->setParStep(0.001);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Sigma                         ");
                theParamManager_[i]->setParLimits(0,100);
                theParamManager_[i]->setParValue (10);
                theParamManager_[i]->setParLimInf(1);
                theParamManager_[i]->setParLimSup(100);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(1);
                theParamManager_[i]->setParStep(0.1);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParName  ("Scale Factor                  ");
                theParamManager_[i]->setParLimits(0, 1000000);
                theParamManager_[i]->setParValue (400);
                theParamManager_[i]->setParLimInf(100);
                theParamManager_[i]->setParLimSup(10000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("Center of distribution        ");
                theParamManager_[i]->setParLimits(-100, 100);
                theParamManager_[i]->setParValue (0);
                theParamManager_[i]->setParLimInf(-100);
                theParamManager_[i]->setParLimSup(100);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(1);
                theParamManager_[i]->setParStep(0.1);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParName  ("Vertical translation Constant ");
                theParamManager_[i]->setParLimits(0, 1000000);
                theParamManager_[i]->setParValue (100);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(1000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Eta Reproduction")
    {
        for (int i = 0; i < 11; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Thickness                     ");
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                theParamManager_[i]->setParValue (6.5);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(30.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParPrecision(5);
                theParamManager_[i]->setParStep(0.00001);
                theParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                theParamManager_[i]->setParValue (0.0007);
                theParamManager_[i]->setParLimInf(0.00001);
                theParamManager_[i]->setParLimSup(0.3);
                theParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("ScaleFactor                   ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            if (i == 4)
            {
                theParamManager_[i]->setParName  ("Cluster 1 extension           ");
                theParamManager_[i]->setParValue (40);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(50);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            if (i == 5)
            {
                theParamManager_[i]->setParName  ("Sigma Gauss (side)            ");
                theParamManager_[i]->setParLimits(-1000, 1000);
                theParamManager_[i]->setParValue (0);
                theParamManager_[i]->setParLimInf(-10);
                theParamManager_[i]->setParLimSup(10);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            if (i == 6)
            {
                theParamManager_[i]->setParName  ("Constant factor (side)        ");
                theParamManager_[i]->setParLimits(0, 10000);
                theParamManager_[i]->setParValue (1000);
                theParamManager_[i]->setParLimInf(100);
                theParamManager_[i]->setParLimSup(10000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(10);
            }
            if (i == 7)
            {
                theParamManager_[i]->setParName  ("Sigma Gauss (center)          ");
                theParamManager_[i]->setParValue (10);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(75);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            if (i == 8)
            {
                theParamManager_[i]->setParName  ("Scale planeID (center)          ");
                theParamManager_[i]->setParLimits(0, 10000);
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(100);
                theParamManager_[i]->setParLimSup(1000);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            if (i == 9)
            {
                theParamManager_[i]->setParName  ("Horizontal Translation        ");
                theParamManager_[i]->setParLimits(-100, 100);
                theParamManager_[i]->setParValue (0);
                theParamManager_[i]->setParLimInf(-100);
                theParamManager_[i]->setParLimSup(100);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(1);
                theParamManager_[i]->setParStep(0.1);
            }
            if (i == 10)
            {
                theParamManager_[i]->setParName  ("Vertical Translation          ");
                theParamManager_[i]->setParValue (100);
                theParamManager_[i]->setParLimInf(0);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );
        }
    }
    else if (item->text() == "Eta Distribution")
    {
        for (int i = 0; i < 6; ++i)
        {
            theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    //        if (i > 0) theParamManager_[i]->stackUnder(theParamManager_[i-1]);

            if (i == 0)
            {
                theParamManager_[i]->setParName  ("Thickness                     ");
                theParamManager_[i]->setParValue (500);
                theParamManager_[i]->setParLimInf(500);
                theParamManager_[i]->setParLimSup(500);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(0);
                theParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                theParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                theParamManager_[i]->setParValue (6.5);
                theParamManager_[i]->setParLimInf(0.1);
                theParamManager_[i]->setParLimSup(30.);
                theParamManager_[i]->setParFixed (false);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                theParamManager_[i]->setParPrecision(5);
                theParamManager_[i]->setParStep(0.00001);
                theParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                theParamManager_[i]->setParValue (0.0007);
                theParamManager_[i]->setParLimInf(0.00001);
                theParamManager_[i]->setParLimSup(0.3);
                theParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                theParamManager_[i]->setParName  ("ScaleFactor for Charge profile");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                theParamManager_[i]->setParName  ("Tan(theta)                    ");
                theParamManager_[i]->setParLimits(-100, 100);
                theParamManager_[i]->setParValue (0);
                theParamManager_[i]->setParLimInf(-1);
                theParamManager_[i]->setParLimSup(1);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(4);
                theParamManager_[i]->setParStep(0.0001);
            }
            else if (i == 5)
            {
                theParamManager_[i]->setParName  ("Global ScaleFactor            ");
                theParamManager_[i]->setParValue (1.);
                theParamManager_[i]->setParLimInf(1.);
                theParamManager_[i]->setParLimSup(1.);
                theParamManager_[i]->setParFixed (true);
                theParamManager_[i]->setParPrecision(2);
                theParamManager_[i]->setParStep(0.01);
            }

            connect(theParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    theParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }


    for (unsigned int j = 0; j < theParamManager_.size(); ++j)
    {
        ss.str("");
        ss << "Par " << j;
        ui_->parametersTW->insertTab(j, theParamManager_[j], (QString)ss.str().c_str());
    }
}

void SoloFitter::on_fitFuncLE_editingFinished()
{
    if (!ui_->customFunctionCB->isChecked()) return;

    std::stringstream ss;

    for (unsigned int j = 0; j < theParamManager_.size(); ++j)
    {
        delete theParamManager_[j];
        ui_->parametersTW->removeTab(j);
    }
    theParamManager_.clear();

    QRegExp parFind ("\\[(\\w)\\]");
    std::set<int> params;

    int pos = 0;
    int nPars = 0;
    while ((pos = parFind.indexIn(ui_->fitFuncLE->text(), pos)) != -1) {
        params.insert(parFind.cap(1).toInt());
        pos += parFind.matchedLength();
    }

    for (std::set<int>::iterator it = params.begin(); it != params.end(); ++it)
    {
        theParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

        ss.str("");
        ss << "Par " << *it;
        theParamManager_[nPars]->setParName  (ss.str());
        theParamManager_[nPars]->setParLimits(-100000, 100000);
        theParamManager_[nPars]->setParValue (0);
        theParamManager_[nPars]->setParLimInf(-100);
        theParamManager_[nPars]->setParLimSup(100);
        theParamManager_[nPars]->setParFixed (false);
        theParamManager_[nPars]->setParPrecision(1);
        theParamManager_[nPars]->setParStep(10);

        ++nPars;
    }

    for (unsigned int j = 0; j < theParamManager_.size(); ++j)
    {
        ss.str("");
        ss << "Par " << j;
        ui_->parametersTW->insertTab(j, theParamManager_[j], (QString)ss.str().c_str());
    }

}
//===========================================================================================================

void SoloFitter::on_fitPB_clicked()
{
    ui_->fitPB->setEnabled(false);

    std::string fitMethod = ui_->fitMethodLE->text().toStdString();

    if (!theHTreeBrowser_->getCurrentObject())
    {
        STDLINE("Select an histogram to fit", ACYellow);
        ui_->fitPB->setEnabled(true);
        return;
    }

    TObject * toFit = theHTreeBrowser_->getCurrentObject();
    TH1F * toFit1D = 0;
    TH2F * toFit2D = 0;
    int hDimension = 0;
    if (this->getObjectType(toFit).find("TH1") != std::string::npos){
        toFit1D = (TH1F*)toFit;
        hDimension = 1;
    }
    if (this->getObjectType(toFit).find("TH2") != std::string::npos){
        toFit2D = (TH2F*)toFit;
        hDimension = 2;
    }

    std::stringstream ss;

    ss.str("");
    ss << toFit->GetName() << "_fitter_" << numberOfFitting_;
    std::string hName = ss.str();
    if (hDimension == 1) {
        double xMin = toFit1D->GetXaxis()->GetXmin();
        double xMax = toFit1D->GetXaxis()->GetXmax();  //check eventually for resize event
        ss.str("");
        ss << toFit1D->GetName();

        if (!ui_->customFunctionCB->isChecked())
        {
            TF1 * fitter = 0;
            if (ui_->fitFuncLW->currentItem()->text() == "Gauss")
                fitter = new TF1(hName.c_str(), "gaus + [3]", xMin, xMax);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta")
                fitter = new TF1(hName.c_str(), Utilities::etaDistribution, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta (No Depletion)")
                fitter = new TF1(hName.c_str(), Utilities::etaFitFunc2, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta with Cut")
                fitter = new TF1(hName.c_str(), Utilities::etaFitFunc_cut, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta (No Depletion) with Cut")
                fitter = new TF1(hName.c_str(), Utilities::etaFitFunc_cut2, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta Inverse")
                fitter = new TF1(hName.c_str(), Utilities::etaInverseFitFunc2Angle, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Langaus")
                fitter = new TF1(hName.c_str(), Utilities::langaus, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Gauss Convoluted with Constant")
                fitter = new TF1(hName.c_str(), Utilities::uniformSideSmeared, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta Convoluted with Gauss")
                fitter = new TF1(hName.c_str(), Utilities::etaSmeared, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Gauss-constant plus Constant")
                fitter = new TF1(hName.c_str(), Utilities::uniformCenterSmearedPlusConstant, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Single constant Gauss-conv.")
                fitter = new TF1(hName.c_str(), Utilities::singleUniformSmeared, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Double constant Gauss-conv.")
                fitter = new TF1(hName.c_str(), Utilities::doubleUniformSmeared, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta Reproduction")
                fitter = new TF1(hName.c_str(), Utilities::etaReproduction, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta Distribution")
                fitter = new TF1(hName.c_str(), Utilities::etaDistributionAngle, xMin, xMax, fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else
                STDLINE("Select a function!", ACYellow);

            //fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()].first TOFIX
            fitter->SetLineColor(kBlue);
            for (int i = 0; i < fitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]; ++i)
            {
                fitter->SetParName(i, theParamManager_[i]->getParName().c_str());
                if (theParamManager_[i]->isParFixed()) fitter->FixParameter(i, theParamManager_[i]->getParValue());
                else {
                    fitter->SetParameter(i, theParamManager_[i]->getParValue());
                    fitter->SetParLimits(i, theParamManager_[i]->getParLimInf(), theParamManager_[i]->getParLimSup());
                }
            }
            STDLINE("Fitting " + ss.str() + " with " << ui_->fitFuncLW->currentItem()->text().toStdString() << " function", ACGreen);
            if (!ui_->fitLimitCB->isChecked()) toFit1D->Fit(fitter, fitMethod.c_str());
            else toFit1D->Fit(fitter, fitMethod.c_str(), "", ui_->fitRangeMinSB->value(), ui_->fitRangeMaxSB->value());
        }
        else
        {
            TF1 * fitter = new TF1(hName.c_str(), ui_->fitFuncLE->text().toStdString().c_str());
            fitter->SetLineColor(kGreen);
            for (unsigned int i = 0; i < theParamManager_.size(); ++i)
            {
                fitter->SetParName(i, theParamManager_[i]->getParName().c_str());
                if (theParamManager_[i]->isParFixed()) fitter->FixParameter(i, theParamManager_[i]->getParValue());
                else {
                    fitter->SetParameter(i, theParamManager_[i]->getParValue());
                    fitter->SetParLimits(i, theParamManager_[i]->getParLimInf(), theParamManager_[i]->getParLimSup());
                }
            }
            STDLINE("Fitting " + ss.str() + " with cut Custom function", ACGreen);
            if (!ui_->fitLimitCB->isChecked()) toFit1D->Fit(fitter, fitMethod.c_str());
            else toFit1D->Fit(fitter, fitMethod.c_str(), "", ui_->fitRangeMinSB->value(), ui_->fitRangeMaxSB->value());
        }
    }
    else STDLINE("Method for 2D fitting not implemented!", ACRed);
    ++numberOfFitting_;

    ui_->fitPB->setEnabled(true);
}

std::string SoloFitter::getObjectType (TObject * obj)
{
    std::string objectType = "Unknown" ;
    TKey* keyH = 0 ;

    TIter bases(obj->IsA()->GetListOfBases());
    int count = 0 ;
    while((keyH = (TKey*)bases()))
    {
        if( count++ == 0 )
            objectType = keyH->GetName() ;
    }
    return objectType ;
}

void SoloFitter::on_fitLimitCB_clicked(bool checked)
{
    if (checked == false)
    {
        ui_->fitRangeMinSB->setEnabled(false);
        ui_->fitRangeMaxSB->setEnabled(false);
    }
    else
    {
        ui_->fitRangeMinSB->setEnabled(true);
        ui_->fitRangeMaxSB->setEnabled(true);
        if(theHTreeBrowser_->getCurrentObject())
        {
            ui_->fitRangeMinSB->setRange(((TH1F*)(theHTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmin(), ((TH1F*)(theHTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmax());
            ui_->fitRangeMinSB->setValue(((TH1F*)(theHTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmin());
            ui_->fitRangeMaxSB->setRange(((TH1F*)(theHTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmin(), ((TH1F*)(theHTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmax());
            ui_->fitRangeMaxSB->setValue(((TH1F*)(theHTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmax());
        }
    }
}

void SoloFitter::on_customFunctionCB_clicked()
{
        for (unsigned int j = 0; j < theParamManager_.size(); ++j)
        {
            delete theParamManager_[j];
            ui_->parametersTW->removeTab(j);
        }
        theParamManager_.clear();
}

void SoloFitter::on_saveImagePB_clicked()
{
    QString localPath = getenv("CHEWIEIMAGEDIR");
    if (!theHTreeBrowser_->getCurrentObject())
    {
        STDLINE("Choose an object!", ACYellow);
        return;
    }
    std::stringstream ss;
//    ss << "$CHEWIEIMAGEDIR/" << theHTreeBrowser_->getCurrentObject()->GetName() << ".png";
    ss << localPath.toStdString() << "/" << theHTreeBrowser_->getCurrentObject()->GetName() << ".png";
    QString fileName = QFileDialog::getSaveFileName(this,"Images",(QString)ss.str().c_str(),"Images(*.png *.jpeg *.ps *.eps)");
    if(fileName.isEmpty())
        return;

    hTreeBrowser::selectedObjectsDef selectedObjects = theHTreeBrowser_->getSelectedItems();
    int nObjects = 1;
    std::stringstream drawOpt;
    std::stringstream cName;
    cName << "UtilityCanvas_" << numberofCanvas_;
    TCanvas * c1 = new TCanvas(cName.str().c_str());
    ++numberofCanvas_;
    for(hTreeBrowser::selectedObjectsDef::iterator it=selectedObjects.begin(); it!=selectedObjects.end(); ++it)
    {
      for(hTreeBrowser::tFileVDef::iterator jt=it->second.begin(); jt!=it->second.end(); ++jt)
      {
          drawOpt.str("");
          gStyle->SetPalette(1);
        if( (*jt)->IsA() == TFolder::Class() ) continue ;
        if( (*jt)->IsA() == TGraph::Class())
            drawOpt << "AP";
        if( (*jt)->IsA() == TH2F::Class() || (*jt)->IsA() == TH2D::Class() || (*jt)->IsA() == TH2I::Class())
        {
            STDLINE("here", ACWhite);
            if (ui_->surfaceRB->isChecked())
                drawOpt << "surf";
            else if (ui_->colzRB->isChecked())
                drawOpt << "COLZ";
            else if (ui_->legoRB->isChecked())
                drawOpt << "lego";
            else if (ui_->lego2RB->isChecked())
                drawOpt << "lego2";
            else if (ui_->contourRB->isChecked())
                drawOpt << "contour";
        }
        if (nObjects > 1)
            drawOpt << ", same";
       (*jt)->Draw(drawOpt.str().c_str());
        ++nObjects;
      }
    }
//    theHTreeBrowser_->getCurrentObject()->Draw(drawOpt.str().c_str());
    c1->Print(fileName.toStdString().c_str(), fileName.toStdString().substr(fileName.toStdString().find_last_of("."), fileName.toStdString().size()).c_str());
    delete c1;
}

