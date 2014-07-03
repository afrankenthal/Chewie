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
#include <QMenuBar>
#include "MessageTools.h"
#include <QMdiSubWindow>
#include <set>
#include <string>
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
#include <vector>
#include <QFileDialog>
#include <QList>
#include <string>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <QStandardItem>
#include "XmlParser.h"
#include "XmlWindow.h"
#include "XmlPlane.h"
#include "XmlAnalysis.h"
#include "XmlGeneral.h"
#include "XmlConverter.h"
#include "XmlScan.h"
#include <limits>
#include <QTextStream>
#include <QFile>

HanSoloFitter::HanSoloFitter(QWidget *parent ) :
    QDialog                     (                       parent)
  , theMainWindow_              (          (MainWindow*)parent)
  , ui_                         (        new Ui::HanSoloFitter)
  , timer_                      (             new QTimer(this))
  , hanSoloTreeBrowser_         (                            0)
  , emptyFileLabel_             ("No files are currently open")
  , displayAllLabel_            (                "Display all")
  , counter_                    (                            0)
  , shiftHSValue_               (                            0)
  , theXmlParser_               (                new XmlParser)


{
    ui_->setupUi(this);
    shiftHSValue_ = ui_->shiftHS->value();

    std::cout<< "ui_->centraltabWidget->setCurrentIndex(0)" << std::endl;


    ui_->centraltabWidget->setCurrentIndex(0);
    std::cout<< "ui_->centraltabWidget->setCurrentIndex(0)" << std::endl;
    on_centraltabWidget_currentChanged(0);


    //hanSoloTreeBrowser_ = new hTreeBrowser(this, theMainWindow_);
    //hanSoloTreeBrowser_->setGeometry(250, 250, 250, 250);
    //this->addItem(emptyFileLabel_.toStdString()) ;

    //QTabWidget * widget_ = new

    //this->collectExistingWidgets();

    /*connect(ui_->HanSoloFitterInputFile,
            SIGNAL(currentIndexChanged(QString)),
            this,
            SLOT(  updateTree(         QString))) ;
    */
    /*connect(timer_,
            SIGNAL(timeout                () ),
            this,
            SLOT  (checkNewObjectsInMemory()));
    */




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

   initialize();


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
//==========================================================================
void HanSoloFitter::on_centraltabWidget_currentChanged(int index)
{
    std::cout << "on_centraltabWidget_currentChanged int index = " << index << std::endl;

    if(index == 0)
    {
        if(hanSoloTreeBrowser_)
        {
            delete hanSoloTreeBrowser_;
            hanSoloTreeBrowser_ = 0;
        }
        getFitters();
        makeFittersSelectable();


    }else if(index == 1)
    {
        connect(ui_->HanSoloFitterInputFile,
                SIGNAL(currentIndexChanged(QString)),
                this,
                SLOT(  updateTree(         QString))) ;

        connect(timer_,
                SIGNAL(timeout                () ),
                this,
                SLOT  (checkNewObjectsInMemory()));

        this->addItem(emptyFileLabel_.toStdString()) ;

        this->collectExistingWidgets();

        hanSoloTreeBrowser_->setGeometry(ui_->HanSoloTreeFrame->pos().x(), ui_->HanSoloTreeFrame->pos().y()+3, ui_->HanSoloTreeFrame->width()+2, ui_->HanSoloTreeFrame->height()+40);

    }
}
//==========================================================================
void HanSoloFitter::initialize(void)
{
    createActions();
    createConnections();
    createMenus();
}
//===========================================================================
void HanSoloFitter::createActions(){

    actionCharge_ = new QAction(tr("Charge"), this);
    actionEfficiency_ = new QAction(tr("Efficiency"), this);
    actionResolution_ = new QAction(tr("Resolution"), this);
    actionRun_Advanced_Fitter_ = new QAction(tr("Run Advanced Fitter"), this);
    actionPlanar_ = new QAction(tr("Planar"), this);
    action3D_ = new QAction(tr("3D"), this);

    detectorPresetsGroup_ = new QActionGroup(this);
    actionPlanar_->setCheckable(true);
    action3D_->setCheckable(true);
    actionPlanar_->setActionGroup(detectorPresetsGroup_);
    action3D_->setActionGroup(detectorPresetsGroup_);

}
//==========================================================================

void HanSoloFitter::createMenus(){

    menu_ = new QMenuBar(this);

    //'Analyze'
    menuAnalyze_ = menu_->addMenu(tr("Analyze"));
    menuRun_Basic_Fit_= menuAnalyze_->addMenu(tr("Run Basic Fit"));
    menuRun_Basic_Fit_->addAction(actionCharge_);
    menuRun_Basic_Fit_->addAction(actionEfficiency_);
    menuRun_Basic_Fit_->addAction(actionResolution_);

    //'Options'
    menuOptions_ = menu_->addMenu(tr("Options"));
    menuDetectorPresets_= menuOptions_->addMenu(tr("Detector Presets"));
    menuDetectorPresets_->addAction(actionPlanar_);
    menuDetectorPresets_->addAction(action3D_);

    //'Advanced'
    menuAdvanced_= menu_->addMenu(tr("Advanced"));
    menuAdvanced_->addAction(actionRun_Advanced_Fitter_);




    menu_->setNativeMenuBar(true);
    menu_->show();
}
//==========================================================================
void HanSoloFitter::createConnections(){



//===========================================================================
//========================= Connect-MenuBar [START] =========================
//===========================================================================





//===========================================================================
//=========================== Connect-MenuBar [END] =========================
//===========================================================================


//===========================================================================
//==================== Connect-Events-to-Repaint [START] ====================
//===========================================================================

    connect(ui_->hanSoloStatCB,
            SIGNAL(stateChanged         (int)),
            this,
            SLOT(checkBoxesHandler      (int)));

    connect(ui_->logxCB,
            SIGNAL(stateChanged         (int)),
            this,
            SLOT(checkBoxesHandler      (int)));

    connect(ui_->logyCB,
            SIGNAL(stateChanged         (int)),
            this,
            SLOT(checkBoxesHandler      (int)));

    connect(ui_->logzCB,
            SIGNAL(stateChanged         (int)),
            this,
            SLOT(checkBoxesHandler      (int)));
//===========================================================================
//===================== Connect-Events-to-Repaint [END] =====================
//===========================================================================



 }
//===========================================================================
void HanSoloFitter::getFitters(void)
{

    std::cout << "getFitters()" << std::endl;

    std::string charge = "harge";
    std::string resolution = "esolution";
    std::string efficiency = "fficiency";
    std::ifstream inFile;
    std::ofstream outFile;
    std::string dir = "ChewieExtended/Fitters";
    std::vector<std::string> fileList;

    chargeVector_.clear();
    efficiencyVector_.clear();
    resolutionVector_.clear();

    DIR *pdir;
    struct dirent *pent;

      pdir=opendir("ChewieExtended/Fitters/");
                   //./../ChewieExtended/Fitters/");
      if (!pdir)
      {
          std::cout << "opendir() failure; terminating" << std::endl;
          return;
      }
      errno=0;
      while ((pent=readdir(pdir))){
          std::cout << "filenames" << pent->d_name << std::endl;
          fileList.push_back(pent->d_name);
      }
      if (errno)
      {
              std::cout << "readdir() failure; terminating" <<std:: endl;
              exit(1);
      }
      closedir(pdir);

      for(unsigned int a = 0; a < fileList.size(); a++)
      {
          fileList.at(a) = fileList.at(a).substr(0, '.');
          std::cout << "fileList[" << a << "] = " << fileList.at(a) << std::endl;

          if(fileList.at(a).find(charge) != std::string::npos)
         {
             chargeVector_.push_back(fileList.at(a));
         }
         if(fileList.at(a).find(efficiency)!= std::string::npos)
         {
             efficiencyVector_.push_back(fileList.at(a));
         }
         if(fileList.at(a).find(resolution)!= std::string::npos)
         {
             resolutionVector_.push_back(fileList.at(a));
         }
     }

      std::cout << "Size: | " << chargeVector_.size() << " | "<< efficiencyVector_.size() << " | "<< resolutionVector_.size() << " | "<< std::endl;

}
//===========================================================================
void HanSoloFitter::makeFittersSelectable(void)
{
    QString qstr;
    int SPACING = 40, XBORDER = 10, YBORDER = 20;



    //===========================================================================
    //============================DUT_0==========================================
    //===========================================================================
    for(unsigned int pos = 0; pos < Dut_0_chargeCheckBoxes_.size(); pos++)
    {
        delete Dut_0_chargeCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < Dut_0_efficiencyCheckBoxes_.size(); pos++)
    {
        delete Dut_0_efficiencyCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < Dut_0_resolutionCheckBoxes_.size(); pos++)
    {
        delete Dut_0_resolutionCheckBoxes_.at(pos);
    }

    Dut_0_chargeCheckBoxes_.resize(chargeVector_.size());
    Dut_0_efficiencyCheckBoxes_.resize(efficiencyVector_.size());
    Dut_0_resolutionCheckBoxes_.resize(resolutionVector_.size());
    //Charge
    for(unsigned int pos = 0; pos < chargeVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = chargeVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * chargeCheckBox = new QCheckBox(ui_->chargeFrame_0);
        Dut_0_chargeCheckBoxes_.at(pos) = chargeCheckBox;
        Dut_0_chargeCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_0_chargeCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_0_chargeCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_0_chargeCheckBoxes_.at(pos)->show();
        connect(Dut_0_chargeCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));


    }
    XBORDER = 10;
    //Efficiency
    for(unsigned int pos = 0; pos < efficiencyVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = efficiencyVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->efficiencyFrame_0);
        Dut_0_efficiencyCheckBoxes_.at(pos) = efficiencyCheckBox;
        Dut_0_efficiencyCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_0_efficiencyCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_0_efficiencyCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_0_efficiencyCheckBoxes_.at(pos)->show();
        connect(Dut_0_efficiencyCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));
    }
    XBORDER = 10;

    //Resolution
    for(unsigned int pos = 0; pos < resolutionVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = resolutionVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->resolutionFrame_0);
        Dut_0_resolutionCheckBoxes_.at(pos) = efficiencyCheckBox;
        Dut_0_resolutionCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_0_resolutionCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_0_resolutionCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_0_resolutionCheckBoxes_.at(pos)->show();
        connect(Dut_0_resolutionCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));

    }
    //===========================================================================
    //===========================DUT_1==========================================
    //===========================================================================
    for(unsigned int pos = 0; pos < Dut_1_chargeCheckBoxes_.size(); pos++)
    {
        delete Dut_1_chargeCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < Dut_1_efficiencyCheckBoxes_.size(); pos++)
    {
        delete Dut_1_efficiencyCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < Dut_1_resolutionCheckBoxes_.size(); pos++)
    {
        delete Dut_1_resolutionCheckBoxes_.at(pos);
    }

    Dut_1_chargeCheckBoxes_.resize(chargeVector_.size());
    Dut_1_efficiencyCheckBoxes_.resize(efficiencyVector_.size());
    Dut_1_resolutionCheckBoxes_.resize(resolutionVector_.size());

    //Charge
    for(unsigned int pos = 0; pos < chargeVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = chargeVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * chargeCheckBox = new QCheckBox(ui_->chargeFrame_1);
        Dut_1_chargeCheckBoxes_.at(pos) = chargeCheckBox;
        Dut_1_chargeCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_1_chargeCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_1_chargeCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_1_chargeCheckBoxes_.at(pos)->show();
        connect(Dut_1_chargeCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));
    }
    XBORDER = 10;


    //Efficiency
    for(unsigned int pos = 0; pos < efficiencyVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = efficiencyVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->efficiencyFrame_1);
        Dut_1_efficiencyCheckBoxes_.at(pos) = efficiencyCheckBox;
        Dut_1_efficiencyCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_1_efficiencyCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_1_efficiencyCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_1_efficiencyCheckBoxes_.at(pos)->show();
        connect(Dut_1_efficiencyCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));
    }
    XBORDER = 10;

    //Resolution
    for(unsigned int pos = 0; pos < resolutionVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = resolutionVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->resolutionFrame_1);
        Dut_1_resolutionCheckBoxes_.at(pos) = efficiencyCheckBox;
        Dut_1_resolutionCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_1_resolutionCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_1_resolutionCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_1_resolutionCheckBoxes_.at(pos)->show();
        connect(Dut_2_resolutionCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));

    }
    //===========================================================================
    //============================DUT_2==========================================
    //===========================================================================
    for(unsigned int pos = 0; pos < Dut_2_chargeCheckBoxes_.size(); pos++)
    {
        delete Dut_2_chargeCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < Dut_2_efficiencyCheckBoxes_.size(); pos++)
    {
        delete Dut_2_efficiencyCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < resolutionCheckBoxes_.size(); pos++)
    {
        delete Dut_2_resolutionCheckBoxes_.at(pos);
    }

    Dut_2_chargeCheckBoxes_.resize(chargeVector_.size());
    Dut_2_efficiencyCheckBoxes_.resize(efficiencyVector_.size());
    Dut_2_resolutionCheckBoxes_.resize(resolutionVector_.size());

    //Charge
    for(unsigned int pos = 0; pos < chargeVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = chargeVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * chargeCheckBox = new QCheckBox(ui_->chargeFrame_2);
        Dut_2_chargeCheckBoxes_.at(pos) = chargeCheckBox;
        Dut_2_chargeCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_2_chargeCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_2_chargeCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_2_chargeCheckBoxes_.at(pos)->show();
        connect(Dut_2_chargeCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));

    }
    XBORDER = 10;


    //Efficiency
    for(unsigned int pos = 0; pos < efficiencyVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = efficiencyVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->efficiencyFrame_2);
        Dut_2_efficiencyCheckBoxes_.at(pos) = efficiencyCheckBox;
        Dut_2_efficiencyCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_2_efficiencyCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_2_efficiencyCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_2_efficiencyCheckBoxes_.at(pos)->show();
        connect(Dut_2_efficiencyCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));
    }
    XBORDER = 10;

    //Resolution
    for(unsigned int pos = 0; pos < resolutionVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = resolutionVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->resolutionFrame_2);
        Dut_2_resolutionCheckBoxes_.at(pos) = efficiencyCheckBox;
        Dut_2_resolutionCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_2_resolutionCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_2_resolutionCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_2_resolutionCheckBoxes_.at(pos)->show();
        connect(Dut_2_resolutionCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));

    }

    //===========================================================================
    //============================DUT_3==========================================
    //===========================================================================
    for(unsigned int pos = 0; pos < Dut_3_chargeCheckBoxes_.size(); pos++)
    {
        delete Dut_3_chargeCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < Dut_2_efficiencyCheckBoxes_.size(); pos++)
    {
        delete Dut_3_efficiencyCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < resolutionCheckBoxes_.size(); pos++)
    {
        delete Dut_3_resolutionCheckBoxes_.at(pos);
    }

    Dut_3_chargeCheckBoxes_.resize(chargeVector_.size());
    Dut_3_efficiencyCheckBoxes_.resize(efficiencyVector_.size());
    Dut_3_resolutionCheckBoxes_.resize(resolutionVector_.size());

    //Charge
    for(unsigned int pos = 0; pos < chargeVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = chargeVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * chargeCheckBox = new QCheckBox(ui_->chargeFrame_3);
        Dut_3_chargeCheckBoxes_.at(pos) = chargeCheckBox;
        Dut_3_chargeCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_3_chargeCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_3_chargeCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_3_chargeCheckBoxes_.at(pos)->show();
        connect(Dut_3_chargeCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));

    }
    XBORDER = 10;


    //Efficiency
    for(unsigned int pos = 0; pos < efficiencyVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = efficiencyVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->efficiencyFrame_3);
        Dut_3_efficiencyCheckBoxes_.at(pos) = efficiencyCheckBox;
        Dut_3_efficiencyCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_3_efficiencyCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_3_efficiencyCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_3_efficiencyCheckBoxes_.at(pos)->show();
        connect(Dut_3_efficiencyCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));
    }
    XBORDER = 10;

    //Resolution
    for(unsigned int pos = 0; pos < resolutionVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;

        std::string name = resolutionVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->resolutionFrame_3);
        Dut_3_resolutionCheckBoxes_.at(pos) = efficiencyCheckBox;
        Dut_3_resolutionCheckBoxes_.at(pos)->setObjectName(qstr.fromStdString(name));
        Dut_3_resolutionCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        Dut_3_resolutionCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        Dut_3_resolutionCheckBoxes_.at(pos)->show();
        connect(Dut_3_resolutionCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(unCheckGeneral(bool)));

    }

    //===========================================================================
    //==========================GENERAL==========================================
    //===========================================================================
    for(unsigned int pos = 0; pos < chargeCheckBoxes_.size(); pos++)
    {
        delete chargeCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < efficiencyCheckBoxes_.size(); pos++)
    {
        delete efficiencyCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < resolutionCheckBoxes_.size(); pos++)
    {
        delete resolutionCheckBoxes_.at(pos);
    }

    chargeCheckBoxes_.resize(chargeVector_.size());
    efficiencyCheckBoxes_.resize(efficiencyVector_.size());
    resolutionCheckBoxes_.resize(resolutionVector_.size());

    //Charge
    for(unsigned int pos = 0; pos < chargeVector_.size(); pos++)
    {
        if(pos > 14)
         XBORDER = 245;
        std::string type = "cha";
        std::string name = chargeVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * chargeCheckBox = new QCheckBox(ui_->chargeFrame);
        chargeCheckBoxes_.at(pos) = chargeCheckBox;
        chargeCheckBoxes_.at(pos)->setObjectName(chargeVector_.at(pos).c_str());
        chargeCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        chargeCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        chargeCheckBoxes_.at(pos)->show();
        connect(chargeCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(checkAllCharge()));

    }
    XBORDER = 10;


    //Efficiency
    for(unsigned int pos = 0; pos < efficiencyVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;
        std::string type = "eff";
        std::string name = efficiencyVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->efficiencyFrame);
        efficiencyCheckBoxes_.at(pos) = efficiencyCheckBox;
        efficiencyCheckBoxes_.at(pos)->setObjectName(efficiencyVector_.at(pos).c_str());
        efficiencyCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        efficiencyCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        efficiencyCheckBoxes_.at(pos)->show();
        connect(efficiencyCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(checkAllEfficiency()));
    }
    XBORDER = 10;

    //Resolution
    for(unsigned int pos = 0; pos < resolutionVector_.size(); pos++)
    {
        if(pos > 14)
           XBORDER = 245;
        std::string type = "res";
        std::string name = resolutionVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->resolutionFrame);
        resolutionCheckBoxes_.at(pos) = efficiencyCheckBox;
        resolutionCheckBoxes_.at(pos)->setObjectName(resolutionVector_.at(pos).c_str());
        resolutionCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        resolutionCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
        resolutionCheckBoxes_.at(pos)->show();
        connect(resolutionCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(checkAllResolution()));

    }

}
//===========================================================================
void HanSoloFitter::checkAllCharge()
{
    int pos = 1;

        std::cout << "Checked: " << pos << std::endl;

        for(unsigned int pos = 0; pos < chargeCheckBoxes_.size(); pos++){



            if(chargeCheckBoxes_.at(pos)->isChecked()){
                Dut_0_chargeCheckBoxes_.at(pos)->setChecked(true);
                Dut_1_chargeCheckBoxes_.at(pos)->setChecked(true);
                Dut_2_chargeCheckBoxes_.at(pos)->setChecked(true);
                Dut_3_chargeCheckBoxes_.at(pos)->setChecked(true);




            }/*else if(chargeCheckBoxes_.at(pos)->isChecked() && position == pos){
                Dut_0_chargeCheckBoxes_.at(pos)->setChecked(false);
                Dut_1_chargeCheckBoxes_.at(pos)->setChecked(false);
                Dut_2_chargeCheckBoxes_.at(pos)->setChecked(false);
                Dut_3_chargeCheckBoxes_.at(pos)->setChecked(false);
            }*/
        }
}
//===========================================================================
void HanSoloFitter::checkAllEfficiency()
{
        for(unsigned int pos = 0; pos < efficiencyCheckBoxes_.size(); pos++){
            if(efficiencyCheckBoxes_.at(pos)->isChecked()){
                Dut_0_efficiencyCheckBoxes_.at(pos)->setChecked(true);
                Dut_1_efficiencyCheckBoxes_.at(pos)->setChecked(true);
                Dut_2_efficiencyCheckBoxes_.at(pos)->setChecked(true);
                Dut_3_efficiencyCheckBoxes_.at(pos)->setChecked(true);

            }/*else{
                Dut_0_efficiencyCheckBoxes_.at(pos)->setChecked(false);
                Dut_1_efficiencyCheckBoxes_.at(pos)->setChecked(false);
                Dut_2_efficiencyCheckBoxes_.at(pos)->setChecked(false);
                Dut_3_efficiencyCheckBoxes_.at(pos)->setChecked(false);
            }*/
        }
}
//===========================================================================
void HanSoloFitter::checkAllResolution()
{
        for(unsigned int pos = 0; pos < resolutionCheckBoxes_.size(); pos++){
            if(resolutionCheckBoxes_.at(pos)->isChecked()){
                Dut_0_resolutionCheckBoxes_.at(pos)->setChecked(true);
                Dut_1_resolutionCheckBoxes_.at(pos)->setChecked(true);
                Dut_2_resolutionCheckBoxes_.at(pos)->setChecked(true);
                Dut_3_resolutionCheckBoxes_.at(pos)->setChecked(true);

            }/*else{
                Dut_0_resolutionCheckBoxes_.at(pos)->setChecked(false);
                Dut_1_resolutionCheckBoxes_.at(pos)->setChecked(false);
                Dut_2_resolutionCheckBoxes_.at(pos)->setChecked(false);
                Dut_3_resolutionCheckBoxes_.at(pos)->setChecked(false);
            }*/
        }

}

//===========================================================================
void HanSoloFitter::unCheckGeneral(bool check)
{
    for(unsigned int pos = 0; pos < Dut_0_chargeCheckBoxes_.size(); pos++){
        if(Dut_0_chargeCheckBoxes_.at(pos)->isChecked()==false)
            chargeCheckBoxes_.at(pos)->setChecked(false);
    }
    for(unsigned int pos = 0; pos < Dut_1_chargeCheckBoxes_.size(); pos++){
        if(Dut_1_chargeCheckBoxes_.at(pos)->isChecked()==false)
            chargeCheckBoxes_.at(pos)->setChecked(false);
    }
    for(unsigned int pos = 0; pos < Dut_2_chargeCheckBoxes_.size(); pos++){
        if(Dut_2_chargeCheckBoxes_.at(pos)->isChecked()==false)
            chargeCheckBoxes_.at(pos)->setChecked(false);

    }
    for(unsigned int pos = 0; pos < Dut_3_chargeCheckBoxes_.size(); pos++){
        if(Dut_3_chargeCheckBoxes_.at(pos)->isChecked()==false)
            chargeCheckBoxes_.at(pos)->setChecked(false);
    }

    for(unsigned int pos = 0; pos < Dut_0_efficiencyCheckBoxes_.size(); pos++){
        if(Dut_0_efficiencyCheckBoxes_.at(pos)->isChecked()==false)
            efficiencyCheckBoxes_.at(pos)->setChecked(false);
    }
    for(unsigned int pos = 0; pos < Dut_1_efficiencyCheckBoxes_.size(); pos++){
        if(Dut_1_efficiencyCheckBoxes_.at(pos)->isChecked()==false)
            efficiencyCheckBoxes_.at(pos)->setChecked(false);
    }
    for(unsigned int pos = 0; pos < Dut_2_efficiencyCheckBoxes_.size(); pos++){
        if(Dut_2_efficiencyCheckBoxes_.at(pos)->isChecked()==false)
            efficiencyCheckBoxes_.at(pos)->setChecked(false);

    }
    for(unsigned int pos = 0; pos < Dut_3_efficiencyCheckBoxes_.size(); pos++){
        if(Dut_3_efficiencyCheckBoxes_.at(pos)->isChecked()==false)
            efficiencyCheckBoxes_.at(pos)->setChecked(false);
    }

    for(unsigned int pos = 0; pos < Dut_0_resolutionCheckBoxes_.size(); pos++){
        if(Dut_0_resolutionCheckBoxes_.at(pos)->isChecked()==false)
            resolutionCheckBoxes_.at(pos)->setChecked(false);
    }
    for(unsigned int pos = 0; pos < Dut_1_resolutionCheckBoxes_.size(); pos++){
        if(Dut_1_resolutionCheckBoxes_.at(pos)->isChecked()==false)
            resolutionCheckBoxes_.at(pos)->setChecked(false);
    }
    for(unsigned int pos = 0; pos < Dut_2_resolutionCheckBoxes_.size(); pos++){
        if(Dut_2_resolutionCheckBoxes_.at(pos)->isChecked()==false)
            resolutionCheckBoxes_.at(pos)->setChecked(false);

    }
    for(unsigned int pos = 0; pos < Dut_3_resolutionCheckBoxes_.size(); pos++){
        if(Dut_3_resolutionCheckBoxes_.at(pos)->isChecked()==false)
            resolutionCheckBoxes_.at(pos)->setChecked(false);
    }

}
//===========================================================================
void HanSoloFitter::on_openConfigurationFilePB_clicked()
{
    QString localPath = this->getEnvPath("CHEWIEXMLDIR");
    QString fileName = QFileDialog::getOpenFileName(this,"Xml configuration file",localPath,"Xml Files(*.xml)");
    if(fileName.isEmpty())
    {
        ui_->configurationFileFrame->setText("Load configuration file (.xml)");
        theXmlParser_->destroy();
        return ;
    }
    ui_->configurationFileFrame->setText(fileName);

    QDomDocument * document = new QDomDocument( "ConfigurationFile" );
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text ))
    {
        STDLINE(std::string("Could not open ")+file.fileName().toStdString(),ACRed);
        return;
    }
    QString errMsg = "";
    int line;
    int col;
    if (!document->setContent( &file, true , &errMsg, &line, &col))
    {
        STDLINE(std::string("Could not access ")+file.fileName().toStdString(),ACRed);
        ss_ << "Error: " << errMsg.toStdString() << " line: " << line << " col: " << col;
        STDLINE(ss_.str(),ACGreen);
        file.close();
        return;
    }

    QDomNode ConfigurationFile = document->elementsByTagName("ConfigurationFile").at(0);
    QDomNodeList Duts = document->elementsByTagName("Dut");

    for(int pos = 0; pos<Duts.size(); pos++)
    {
        QDomNode DutNode = Duts.at(pos);

    }

}
//===========================================================================
void HanSoloFitter::on_reset_clicked()
{
    for(unsigned int pos = 0; pos < chargeCheckBoxes_.size(); pos++){
        chargeCheckBoxes_.at(pos)->setChecked(false);
        Dut_0_chargeCheckBoxes_.at(pos)->setChecked(false);
        Dut_1_chargeCheckBoxes_.at(pos)->setChecked(false);
        Dut_2_chargeCheckBoxes_.at(pos)->setChecked(false);
        Dut_3_chargeCheckBoxes_.at(pos)->setChecked(false);
    }
    for(unsigned int pos = 0; pos < efficiencyCheckBoxes_.size(); pos++){
        efficiencyCheckBoxes_.at(pos)->setChecked(false);
        Dut_0_efficiencyCheckBoxes_.at(pos)->setChecked(false);
        Dut_1_efficiencyCheckBoxes_.at(pos)->setChecked(false);
        Dut_2_efficiencyCheckBoxes_.at(pos)->setChecked(false);
        Dut_3_efficiencyCheckBoxes_.at(pos)->setChecked(false);

   }
    for(unsigned int pos = 0; pos < resolutionCheckBoxes_.size(); pos++){
        resolutionCheckBoxes_.at(pos)->setChecked(false);
        Dut_0_resolutionCheckBoxes_.at(pos)->setChecked(false);
        Dut_1_resolutionCheckBoxes_.at(pos)->setChecked(false);
        Dut_2_resolutionCheckBoxes_.at(pos)->setChecked(false);
        Dut_3_resolutionCheckBoxes_.at(pos)->setChecked(false);

    }



}
//===========================================================================
void HanSoloFitter::on_runButton_clicked()
{


    std::cout << "Running!" << std::endl;
    //GENERAL

    checkedBoxes_.clear();

    for(unsigned int pos = 0; pos < chargeCheckBoxes_.size(); pos++)
    {
       if(chargeCheckBoxes_.at(pos)->isChecked())
        {
           checkedBoxes_.push_back(chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos < efficiencyCheckBoxes_.size(); pos++)
    {
       if(efficiencyCheckBoxes_.at(pos)->isChecked())
        {
           checkedBoxes_.push_back(efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos < resolutionCheckBoxes_.size(); pos++)
    {
       if(resolutionCheckBoxes_.at(pos)->isChecked())
        {
           checkedBoxes_.push_back(resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }


    //DUT_0

    Dut_0_checkedBoxes_.clear();

    for(unsigned int pos = 0; pos <  Dut_0_chargeCheckBoxes_.size(); pos++)
    {
       if( Dut_0_chargeCheckBoxes_.at(pos)->isChecked())
        {
            Dut_0_checkedBoxes_.push_back( Dut_0_chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " <<  Dut_0_chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos <  Dut_0_efficiencyCheckBoxes_.size(); pos++)
    {
       if( Dut_0_efficiencyCheckBoxes_.at(pos)->isChecked())
        {
            Dut_0_checkedBoxes_.push_back( Dut_0_efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " <<  Dut_0_efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos <  Dut_0_resolutionCheckBoxes_.size(); pos++)
    {
       if( Dut_0_resolutionCheckBoxes_.at(pos)->isChecked())
        {
            Dut_0_checkedBoxes_.push_back( Dut_0_resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " <<  Dut_0_resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }


    //Dut_1

    Dut_1_checkedBoxes_.clear();

    for(unsigned int pos = 0; pos < Dut_1_chargeCheckBoxes_.size(); pos++)
    {
       if(Dut_1_chargeCheckBoxes_.at(pos)->isChecked())
        {
           Dut_1_checkedBoxes_.push_back(Dut_1_chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_1_chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos < Dut_1_efficiencyCheckBoxes_.size(); pos++)
    {
       if(Dut_1_efficiencyCheckBoxes_.at(pos)->isChecked())
        {
           Dut_1_checkedBoxes_.push_back(Dut_1_efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_1_efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos < Dut_1_resolutionCheckBoxes_.size(); pos++)
    {
       if(Dut_1_resolutionCheckBoxes_.at(pos)->isChecked())
        {
           Dut_1_checkedBoxes_.push_back(Dut_1_resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_1_resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }


    //Dut_2

    Dut_2_checkedBoxes_.clear();

    for(unsigned int pos = 0; pos < Dut_2_chargeCheckBoxes_.size(); pos++)
    {
       if(Dut_2_chargeCheckBoxes_.at(pos)->isChecked())
        {
           Dut_2_checkedBoxes_.push_back(Dut_2_chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_2_chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos < Dut_2_efficiencyCheckBoxes_.size(); pos++)
    {
       if(Dut_2_efficiencyCheckBoxes_.at(pos)->isChecked())
        {
           Dut_2_checkedBoxes_.push_back(Dut_2_efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_2_efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos < Dut_2_resolutionCheckBoxes_.size(); pos++)
    {
       if(Dut_2_resolutionCheckBoxes_.at(pos)->isChecked())
        {
           Dut_2_checkedBoxes_.push_back(Dut_2_resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_2_resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }

    //Dut_3

    Dut_3_checkedBoxes_.clear();

    for(unsigned int pos = 0; pos < Dut_3_chargeCheckBoxes_.size(); pos++)
    {
       if(Dut_3_chargeCheckBoxes_.at(pos)->isChecked())
        {
           Dut_3_checkedBoxes_.push_back(Dut_3_chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_3_chargeCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos < Dut_3_efficiencyCheckBoxes_.size(); pos++)
    {
       if(Dut_3_efficiencyCheckBoxes_.at(pos)->isChecked())
        {
           Dut_3_checkedBoxes_.push_back(Dut_3_efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_3_efficiencyCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }
    for(unsigned int pos = 0; pos < Dut_3_resolutionCheckBoxes_.size(); pos++)
    {
       if(Dut_3_resolutionCheckBoxes_.at(pos)->isChecked())
        {
           Dut_3_checkedBoxes_.push_back(Dut_3_resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData());
            std::cout << "Checked! : " << Dut_3_resolutionCheckBoxes_.at(pos)->objectName().toUtf8().constData() << std::endl;
        }
    }


    std::cout << "Root File: " << rootFile_ << std::endl;

    std::string generalFits = "", Dut_0_Fits = "", Dut_1_Fits = "", Dut_2_Fits = "", Dut_3_Fits = "";
    std::string include = "";
    std::string fittedFileName = rootFile_.substr(0, rootFile_.find('.')) + "_fitted.root";
    std::cout << "copyFileName: " << fittedFileName << std::endl;

    for(unsigned int pos = 0; pos < chargeVector_.size(); pos++)
    {
       include = include + "#include \"Fitters/" + chargeVector_.at(pos) + "\" \n";
    }
    for(unsigned int pos = 0; pos < efficiencyVector_.size(); pos++)
    {
       include = include + "#include \"Fitters/" + efficiencyVector_.at(pos) + "\" \n";
    }
    for(unsigned int pos = 0; pos < resolutionVector_.size(); pos++)
    {
       include = include + "#include \"Fitters/" + resolutionVector_.at(pos) + "\" \n";
    }

    for(unsigned int pos = 0; pos < Dut_0_checkedBoxes_.size(); pos++)
    {
        Dut_0_Fits = Dut_0_Fits + Dut_0_checkedBoxes_.at(pos) + "(inFile, \"Dut0\"); \n";
    }
    for(unsigned int pos = 0; pos < Dut_1_checkedBoxes_.size(); pos++)
    {
        Dut_1_Fits = Dut_1_Fits + Dut_1_checkedBoxes_.at(pos) + "(inFile, \"Dut1\"); \n";
    }
    for(unsigned int pos = 0; pos < Dut_2_checkedBoxes_.size(); pos++)
    {
        Dut_2_Fits = Dut_2_Fits + Dut_2_checkedBoxes_.at(pos) + "(inFile, \"Dut2\"); \n";
    }
    for(unsigned int pos = 0; pos < Dut_3_checkedBoxes_.size(); pos++)
    {
        Dut_2_Fits = Dut_2_Fits + Dut_3_checkedBoxes_.at(pos) + "(inFile, \"Dut2\"); \n";
    }
        //boxes = boxes + checkedBoxes_.at(pos) + "(inFile, \"\"); \n";

    //std::cout << "include: \n" << include << "\nboxes: \n" << boxes << std::endl;



    fstream scriptFile("ChewieExtended/ScriptToFit.cpp");
    std::string filePath = "ChewieExtended/ScriptToFit.cpp";
    //scriptFile.open("/../../ChewieExtended/ScriptToFit.cpp");

    if(scriptFile.is_open())
    {
    scriptFile << "#include <TFile.h> \n#include <TObject.h> \n" << include <<"\n void ScriptToFit() \n { \n    TFile* inFile = TFile::Open(\"" << fittedFileName
               << "\",\"UPDATE\"); \n \n \n//Place all the fitters here \n  inFile->cd(); \n    " << Dut_0_Fits << Dut_1_Fits << Dut_2_Fits << " \n \ninFile->Write("",TObject::kOverwrite); \n "
               << "inFile->Close(); \n \n }";

    scriptFile.close();
    
    std::string cp = "cp " + rootFile_ + " " + fittedFileName ;
    system(cp.c_str());
    system("pwd");
    system("cd ChewieExtended; root.exe -l -b -q -x ScriptToFit.cpp");
    
    }else
        std::cout << "Error: Couldn't open " << filePath << std::endl;
}
//===========================================================================
void HanSoloFitter::on_openRootFile_clicked()
{

    QString dataDir = this->getEnvPath("CHEWIEDATADIR");
    QStringList fileNames = QFileDialog::getOpenFileNames(this,"Root ntuple files",dataDir,"Root Ntuple Files(*.root)");
    if(fileNames.isEmpty())
        return;

    fileNames.sort();

    QStandardItemModel* model;
    if(!(model = (QStandardItemModel*)ui_->convertFileListTV->model()))
    {
        model = new QStandardItemModel(fileNames.size(),4,this);
        ui_->convertFileListTV->setModel(model);
    }
    else
        model->clear();

    std::vector<std::string> stdFileList;
    for(int f=0; f<fileNames.size(); f++)
    {
        QString fileName = fileNames.at(f);
        QStandardItem* directoryItem         = new QStandardItem(fileName.section('/',0,-2));
        fileName = fileName.section('/',0,-2) + "/" + fileName.section('/',-1);
        rootFile_ = fileName.toUtf8().constData();
        QStandardItem* fileNameItem          = new QStandardItem(fileName);
        QStandardItem* geoFileNameItem       = new QStandardItem(fileName.replace(fileName.lastIndexOf(".root"),5,".geo"));
        QStandardItem* convertedFileNameItem = new QStandardItem(fileName.replace(fileName.lastIndexOf(".geo"),4,"_Converted.root"));
        model->setItem(f,0,directoryItem);
        model->setItem(f,1,fileNameItem);
        model->setItem(f,2,geoFileNameItem);
        model->setItem(f,3,convertedFileNameItem);
        stdFileList.push_back(fileNames.at(f).toStdString());
    }
    model->setHeaderData(0, Qt::Horizontal, "Directory");
    model->setHeaderData(1, Qt::Horizontal, "Files");
    model->setHeaderData(2, Qt::Horizontal, "Geo");
    model->setHeaderData(3, Qt::Horizontal, "Output");
    ui_->convertFileListTV->resizeColumnsToContents();




    //theEventManager_->setInFilesList(stdFileList);
}
//============================================================================================================================================
QString HanSoloFitter::getEnvPath(QString environmentName)
{
    QString environmentValue = getenv(environmentName.toStdString().c_str()) ;

    if(environmentValue.isEmpty())
    {
        environmentValue = "./" ;
        std::stringstream ss;
        ss << ACRed << ACBold
           << "WARNING: "
           << ACPlain
           << "environment variable "
           << environmentName.toStdString()
           << " is undefined. Assuming "
           << environmentValue.toStdString()
           << " as its value." ;
        STDLINE(ss.str(),ACYellow) ;
    }
    if(environmentValue.at(environmentValue.size()-1) != '/') environmentValue += '/';
    return environmentValue ;
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
//===========================================================================
//========================File System Sidebar [End]==========================
//===========================================================================
//===========================================================================


//=================================================================================
//===================Canvas Display Histogram Display [Start]======================
//=================================================================================
//=================================================================================

//===========================================================================
QMdiSubWindow* HanSoloFitter::getDrawingWindow(void)
{
    return ui_->theCanvas;
}

//=================================================================================
//===================Canvas Display Histogram Display [End]========================
//=================================================================================
//=================================================================================


//=================================================================================
//========================On-Screen Control [Start]================================
//=================================================================================
//=================================================================================
//=================================================================================
void HanSoloFitter::checkBoxesHandler(bool)
{
    STDLINE("Check box read.", ACWhite);
    //STDLINE("Bool",ACRed);
    hanSoloTreeBrowser_->showContextMenu(QPoint());
}

//===========================================================================
void HanSoloFitter::checkBoxesHandler(int)
{
    //STDLINE("Int",ACGreen);
    hanSoloTreeBrowser_->showContextMenu(QPoint());
}
std::string HanSoloFitter::twoDOption(void)
{
    

    if( ui_->surfaceRB->isChecked() ) return "SURF4";
    if( ui_->contourRB->isChecked() ) return "CONT" ;
    if( ui_->legoRB   ->isChecked() ) return "LEGO4";
    if( ui_->lego2RB  ->isChecked() ) return "LEGO2";
    if( ui_->colzRB   ->isChecked() ) return "COLZ" ;
    return "SURF4" ;
    //return "" ;
}

//===========================================================================
bool HanSoloFitter::plotFitBox(void)
{
    return false;
    std::cout << "plotFitBox: " << ui_->fitCB->isChecked() << std::endl;
    //return ui_->fitCB->isChecked();
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
//========= Canvas Selection ===================================================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_hCanvasCB_activated(int index)
{
    //    STDLINE("on_hCanvasCB_activated",ACRed);
    hanSoloTreeBrowser_->setCurrentCanvas(index) ;
}

//===========================================================================
//========= Range Buttons ===================================================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_unZoomPB_clicked()
{
    hanSoloTreeBrowser_->unZoom() ;
    std::string str = "Set Range: \nX-Range: [0, 0] \nY-Range: [0, 0] \nZ-Range: [0, 0]";

    STDLINE(str, ACGreen);
}
//===========================================================================
void HanSoloFitter::on_setRangePB_clicked()
{
    hanSoloTreeBrowser_->setRange( ui_->xMinLE->text().toFloat(),ui_->xMaxLE->text().toFloat()
                              , ui_->yMinLE->text().toFloat(),ui_->yMaxLE->text().toFloat()
                              , ui_->zMinLE->text().toFloat(),ui_->zMaxLE->text().toFloat());

    std::string str = "Set Range: \nX-Range: [" + ui_->xMinLE->text().toStdString() + ", " + ui_->xMaxLE->text().toStdString() + "]"
              + "\nY-Range: [" + ui_->yMinLE->text().toStdString() + ", " + ui_->yMaxLE->text().toStdString() + "]"
              + "\nZ-Range: [" + ui_->zMinLE->text().toStdString() + ", " + ui_->zMaxLE->text().toStdString() + "]";

    STDLINE(str, ACGreen);

}
//===========================================================================
void HanSoloFitter::on_shiftPlusPB_clicked()
{
    float shiftMin = ui_->xMinLE->text().toFloat()+ui_->shiftLE->text().toFloat();
    float shiftMax = ui_->xMaxLE->text().toFloat()+ui_->shiftLE->text().toFloat();
    ui_->xMinLE->setText(QString("%1").arg(shiftMin));
    ui_->xMaxLE->setText(QString("%1").arg(shiftMax));
    emit on_setRangePB_clicked();
}

//===========================================================================
void HanSoloFitter::on_shiftMinusPB_clicked()
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
void HanSoloFitter::on_shiftHS_valueChanged(int value)
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
//===========================================================================
//=========== Log Buttons ===================================================
//===========================================================================
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
//=========== Statistics Box ================================================
//===========================================================================
//===========================================================================
bool HanSoloFitter::plotStatBox(void)
{
    //return false;
    return ui_->hanSoloStatCB->isChecked();
}

//=================================================================================
//======================== On-Screen Control [End] ================================
//=================================================================================
//=================================================================================
//=================================================================================
