/*******************************************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************************************
*                                                                                                                                                                                           *
*                                                                                July 31, 2014                                                                                              *
*                                                                                                                                                                                           *
*                                                                                DANIEL PARILLA                                                                                             *
*                                                                         FERMI RESEARCH ALLIANCE, LLC                                                                                      *
*                                                                               parilla\@fnal.gov                                                                                           *
*                                                                                                                                                                                           *
*                                                                                                                                                                                           *
*                                                                                                                                                                                           *
********************************************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************************************/
/********************************************************************************************************************************************************************************************
*. Initialize                                                   II. HanSoloFitter [Fitter]                                      III. HanSoloFitter [Viewer]                                 *
*      createActions()                                                 Initialize HanSoloFitter                                    File System Sidebar                                      *
*      createMenus()                                                       getFitters()                                                updateTree(QString currentFile)                      *
*  Connect-Events-to-Repaint                                               makeFittersSelectable()                                     collectExistingWidgets()                             *
*      createConnections()                                             Handle Fit Checkboxes                                           fillWidgetTree(std::string currentFile)              *
*  Switching between Fitter/Viewer                                         checkAllCharge()                                            fillWidget()                                         *
*      on_centraltabWidget_currentChanged(int index)                       check_for_checkall()                                        addNewFile(const QString &)                          *
*                                                                          checkAllEfficiency()                                        on_openFilePB_clicked()                              *
*                                                                  Root File                                                           addItem(std::string item)                            *
*                                                                      on_openRootFile_clicked()                                       delItem(std::string item)                            *
*                                                                      getEnvPath(QString environmentName)                             refresh()                                            *
*                                                                  Configuration                                                       addFile(std::string fileName, TFile* file)           *
*                                                                       on_saveConfigurationFile_clicked()                          Canvas Display Histogram Display                        *
*                                                                       on_openConfigurationFilePB_clicked()                            getDrawingWindow()                                  *
*                                                                   Dynamic Tabs                                                    On-Screen Control                                       *
*                                                                       on_fitterOptions_selected(const QString &arg1)                  checkBoxesHandler(bool)                             *
*                                                                       on_fitterOptions_currentChanged(int index)                      checkBoxesHandler(int)                              *
*                                                                   Reset Buttons                                                       twoDOption()                                        *
*                                                                       on_reset_clicked()                                              plotFitBox()                                        *
*                                                                       on_resetDut_clicked()                                           canvasSplitType()                                   *
*                                                                   Run Button                                                          getCanvas()                                         *
*                                                                       on_runButton_clicked()                                          Range Buttons                                       *
*                                                                                                                                       on_unZoomPB_clicked()                               *
*                                                                                                                                       on_setRangePB_clicked()                             *
*                                                                                                                                       on_shiftPlusPB_clicked()                            *
*                                                                                                                                       on_shiftMinusPB_clicked()                           *
*                                                                                                                                       on_shiftHS_valueChanged(int value)                  *
*                                                                                                                                   Log Buttons                                             *
*                                                                                                                                       logX()                                              *
*                                                                                                                                       logY()                                              *
*                                                                                                                                       logZ()                                              *
*                                                                                                                                       Stats Box                                           *
*                                                                                                                                       plotStatBox()                                       *
*                                                                                                                                     Fit Manager                                           *
*                                                                                                                                      on_fitPB_clicked()                                   *
*                                                                                                                                       getObjectType(TObject * obj)                        *
*                                                                                                                                       on_fitLimitCB_clicked(bool checked)                 *
********************************************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************************************/



#include "HanSoloFitter.h"
#include "hnavigator.h"
#include "ui_hanSoloFitter.h"
#include "Utilities.h"
#include "analyzerdlg.h"
#include <QtWidgets/QDialog>
#include <QtCore/QString>
#include <QtCore/QFuture>
#include <QtCore/QFutureWatcher>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QMenuBar>
#include "MessageTools.h"
#include <QtWidgets/QMdiSubWindow>
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
#include <QtWidgets/QFileDialog>
#include <QtCore/QList>
#include <string>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <QtGui/QStandardItem>
#include "XmlParser.h"
#include "XmlWindow.h"
#include "XmlPlane.h"
#include "XmlAnalysis.h"
#include "XmlGeneral.h"
#include "XmlConverter.h"
#include "XmlScan.h"
#include <limits>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtXml/QtXml>

HanSoloFitter::HanSoloFitter(QWidget *parent ) :
    QDialog                     (                       parent)
  , theMainWindow_              (          (MainWindow*)parent)
  , ui_                         (        new Ui::HanSoloFitter)
  , hanSoloTreeBrowser_         (                            0)
  , timer_                      (             new QTimer(this))
  , emptyFileLabel_             ("No files are currently open")
  , displayAllLabel_            (                "Display all")
  , counter_                    (                            0)
  , shiftHSValue_               (                            0)
  , theXmlParser_               (                new XmlParser)
{
    //===========================================================================
    //=========================== Initialize [START] ============================
    //===========================================================================

    ui_->setupUi(this);
    shiftHSValue_ = ui_->shiftHS->value();

    ui_->centraltabWidget->setCurrentIndex(0);
    on_centraltabWidget_currentChanged(0);
    canvas_ = new CanvasWidget();
    canvas_->setParent(ui_->theCanvas);
    canvas_->setTitle("Histogram Display");
    canvas_->setGeometry(0, 0, ui_->theCanvas->width(), ui_->theCanvas->height());
    canvas_->show();

    for(int i=0; i<10; ++i)
    {
        ss_.str(""); ss_ << i ;
        ui_->HanSoloCanvasCB->addItem(QString(ss_.str().c_str()));
    }

    gStyle->SetPalette(1,0) ;

    timer_->start(1000) ;
    hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));
    hanSoloParamManager_[0]->setParName("Choose a function");

    ui_->parametersTW->insertTab(0, hanSoloParamManager_[0], "Par 0");
    ui_->fitMethodLE->setText("L");
    ui_->fitLimitCB->setChecked(false);
    emit (on_fitLimitCB_clicked(false));
    numberOfFitting_ = 0;
    numberofCanvas_  = 0;

    ui_->fitFuncLW->insertItem(0, "Eta");
    hanSoloFitFunctions_["Eta"] =  6;
    ui_->fitFuncLW->insertItem(1, "Eta (No Depletion)");
    hanSoloFitFunctions_["Eta (No Depletion)"] =  4;
    ui_->fitFuncLW->insertItem(2, "Eta Derivative");
    hanSoloFitFunctions_["Eta Derivative"] = 5;//integrandEtaFitFunc2Angle
    ui_->fitFuncLW->insertItem(3, "Eta with Cut");
    hanSoloFitFunctions_["Eta with Cut"] = 7;
    ui_->fitFuncLW->insertItem(4, "Eta (No Depletion) with Cut");
    hanSoloFitFunctions_["Eta (No Depletion) with Cut"] = 5;
    ui_->fitFuncLW->insertItem(5, "Eta Inverse");
    hanSoloFitFunctions_["Eta Inverse"] = 5;
    ui_->fitFuncLW->insertItem(6, "Langaus");
    hanSoloFitFunctions_["Langaus"] = 4;
    ui_->fitFuncLW->insertItem(7, "Gauss");
    hanSoloFitFunctions_["Gauss"] = 4;
    ui_->fitFuncLW->insertItem(8, "Gauss Convoluted with Constant");
    hanSoloFitFunctions_["Gauss Convoluted with Constant"] = 4;
    ui_->fitFuncLW->insertItem(9, "Eta Convoluted with Gauss");
    hanSoloFitFunctions_["Eta Convoluted with Gauss"] = 9;
    ui_->fitFuncLW->insertItem(10,"Gauss-constant plus Constant");
    hanSoloFitFunctions_["Gauss-constant plus Constant"] = 5;
    ui_->fitFuncLW->insertItem(11,"Single constant Gauss-conv.");
    hanSoloFitFunctions_["Single constant Gauss-conv."] = 6;
    ui_->fitFuncLW->insertItem(12,"Double constant Gauss-conv.");
    hanSoloFitFunctions_["Double constant Gauss-conv."] = 7;
    ui_->fitFuncLW->insertItem(13,"Eta Reproduction");
    hanSoloFitFunctions_["Eta Reproduction"] = 11;
    ui_->fitFuncLW->insertItem(14,"Eta Distribution");
    hanSoloFitFunctions_["Eta Distribution"] = 6;

    ui_->fitFuncLW->insertItem(15,"Linear"); //Irene Added
    hanSoloFitFunctions_["Linear"] = 2; //Irene Added
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
void HanSoloFitter::initialize(void)
{
    createActions();
    createConnections();
    createMenus();
}
//===========================================================================
void HanSoloFitter::createActions()
{

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
void HanSoloFitter::createMenus()
{

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





    menu_->setNativeMenuBar(true);
    menu_->show();
}
//===========================================================================
//==================== Connect-Events-to-Repaint [START] ====================
//===========================================================================
//==========================================================================
void HanSoloFitter::createConnections(){



    connect(ui_->hanSoloStatCB,
            SIGNAL(stateChanged         (int)),
            this,
            SLOT(checkBoxesHandler      (int)));

    connect(ui_->fitCB,
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


}
//===========================================================================
//===================== Connect-Events-to-Repaint [END] =====================
//===========================================================================

//===========================================================================
//============================= Initialize [END] ============================
//===========================================================================

//===========================================================================
//================= Switching between Fitter/Viewer [START] =================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_centraltabWidget_currentChanged(int index)
{
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
//===========================================================================
//================= Switching between Fitter/Viewer [END] ===================
//===========================================================================
//===========================================================================

//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//============================================ HanSoloFitter [Fitter] ==============================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================


//===========================================================================
//==================== Initialize HanSoloFitter [START] =====================
//===========================================================================
//===========================================================================
void HanSoloFitter::getFitters(void)
{
    std::string charge = "Charge::fitCharge";
    std::string resolution = "::fit";
    std::string efficiency = "::fit";
    std::ifstream inFile;
    std::ofstream outFile;
    std::string dir = "src";
    std::string line;

    chargeVector_.clear();
    efficiencyVector_.clear();
    resolutionVector_.clear();


    //Charge
    size_t pos;
    std::ifstream file("src/Charge.cpp", std::ios::out);
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file,line);
            pos=line.find(charge);
            if(pos != std::string::npos)
            {
                int len = line.find_first_of('(') - 1 - line.find_last_of(':');
                std::string methodName = line.substr(line.find_last_of(':') +1, len);
                chargeVector_.push_back(methodName);
            }
        }
    }
    file.close();

    //Efficiency
    file.open("src/Efficiency.cpp", std::ios::out);
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file,line);
            pos=line.find(efficiency);
            if(pos != std::string::npos)
            {
                int len = line.find_first_of('(') - 1 - line.find_last_of(':');
                std::string methodName = line.substr(line.find_last_of(':') +1, len);
                efficiencyVector_.push_back(methodName);
            }
        }
    }
    file.close();

    //Resolution
    file.open("src/Resolution.cpp", std::ios::out);
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file,line);
            pos=line.find(resolution);
            if(pos != std::string::npos)
            {
                int len = line.find_first_of('(') - 1 - line.find_last_of(':');
                std::string methodName = line.substr(line.find_last_of(':') +1, len);
                resolutionVector_.push_back(methodName);
            }
        }
    }
    file.close();

}
//===========================================================================
void HanSoloFitter::makeFittersSelectable(void)
{
    QString qstr;
    int SPACING = 40, XBORDER = 10, YBORDER = 20, YSPACING = 0;


    //===========================================================================
    //==========================GENERAL==========================================
    //===========================================================================
    for(unsigned int pos = 0; pos < generalChargeCheckBoxes_.size(); pos++)
    {
        delete generalChargeCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < generalEfficiencyCheckBoxes_.size(); pos++)
    {
        delete generalEfficiencyCheckBoxes_.at(pos);
    }
    for(unsigned int pos = 0; pos < generalResolutionCheckBoxes_.size(); pos++)
    {
        delete generalResolutionCheckBoxes_.at(pos);
    }

    generalChargeCheckBoxes_.resize(chargeVector_.size());
    generalEfficiencyCheckBoxes_.resize(efficiencyVector_.size());
    generalResolutionCheckBoxes_.resize(resolutionVector_.size());

    //Charge
    for(unsigned int pos = 0; pos < chargeVector_.size(); pos++)
    {
        if(pos > 12)
        {
            XBORDER = 245;
            YSPACING = (pos-13) * SPACING;

        }else{

            YSPACING = pos * SPACING;
        }
        std::string type = "cha";
        std::string name = chargeVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * chargeCheckBox = new QCheckBox(ui_->chargeFrame);
        generalChargeCheckBoxes_.at(pos) = chargeCheckBox;
        generalChargeCheckBoxes_.at(pos)->setObjectName(chargeVector_.at(pos).c_str());
        generalChargeCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        generalChargeCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (YSPACING)), 200, 20);
        generalChargeCheckBoxes_.at(pos)->show();
        connect(generalChargeCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(checkAllCharge()));

    }
    XBORDER = 10;


    //Efficiency
    for(unsigned int pos = 0; pos < efficiencyVector_.size(); pos++)
    {
        if(pos > 12)
        {
            XBORDER = 245;
            YSPACING = (pos-13) * SPACING;

        }else{

            YSPACING = pos * SPACING;
        }
        std::string type = "eff";
        std::string name = efficiencyVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->efficiencyFrame);
        generalEfficiencyCheckBoxes_.at(pos) = efficiencyCheckBox;
        generalEfficiencyCheckBoxes_.at(pos)->setObjectName(efficiencyVector_.at(pos).c_str());
        generalEfficiencyCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        generalEfficiencyCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (YSPACING)), 200, 20);
        generalEfficiencyCheckBoxes_.at(pos)->show();
        connect(generalEfficiencyCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(checkAllEfficiency()));
    }
    XBORDER = 10;

    //Resolution
    for(unsigned int pos = 0; pos < resolutionVector_.size(); pos++)
    {
        if(pos > 12)
        {
            XBORDER = 245;
            YSPACING = (pos-13) * SPACING;

        }else{

            YSPACING = pos * SPACING;
        }
        std::string type = "res";
        std::string name = resolutionVector_.at(pos).c_str();
        name = name.substr (0, name.find('.'));
        QCheckBox * efficiencyCheckBox = new QCheckBox(ui_->resolutionFrame);
        generalResolutionCheckBoxes_.at(pos) = efficiencyCheckBox;
        generalResolutionCheckBoxes_.at(pos)->setObjectName(resolutionVector_.at(pos).c_str());
        generalResolutionCheckBoxes_.at(pos)->setText(qstr.fromStdString(name));
        generalResolutionCheckBoxes_.at(pos)->setGeometry(XBORDER, (YBORDER + (YSPACING)), 200, 20);
        generalResolutionCheckBoxes_.at(pos)->show();
        connect(generalResolutionCheckBoxes_.at(pos),
                SIGNAL(clicked(bool)),
                this,
                SLOT(checkAllResolution()));

    }

}
//===========================================================================
//===================== Initialize HanSoloFitter [END] ======================
//===========================================================================
//===========================================================================

//===========================================================================
//===================== Handle Fit Checkboxes [START] =======================
//===========================================================================
//===========================================================================
//===========================================================================
void HanSoloFitter::checkAllCharge()
{
    for(unsigned int pos = 0; pos < generalChargeCheckBoxes_.size(); pos++){



        if(generalChargeCheckBoxes_.at(pos)->isChecked()){
            for(unsigned int posy = 0; posy < chargeCheckBoxes_.size(); posy++){
                chargeCheckBoxes_[posy][pos]->setChecked(true);
            }
        }

    }
}
//===========================================================================
void HanSoloFitter::check_for_checkall(void)
{
    bool checkIt = true;

    for(unsigned int posx = 0; posx < chargeVector_.size(); posx++)
    {
        for(int posy = 0; posy < ui_->fitterOptions->count()-3; posy++)
        {
            if(chargeCheckBoxes_[posy][posx]->isChecked() == false)
                checkIt = false;
        }
        generalChargeCheckBoxes_.at(posx)->setChecked(checkIt);

        checkIt = true;

    }

    checkIt = true;


    for(unsigned int posx = 0; posx < efficiencyVector_.size(); posx++)
    {
        for(int posy = 0; posy < ui_->fitterOptions->count()-3; posy++)
        {
            if(efficiencyCheckBoxes_[posy][posx]->isChecked() == false)
                checkIt = false;
        }
        generalEfficiencyCheckBoxes_.at(posx)->setChecked(checkIt);

        checkIt = true;

    }

    checkIt = true;


    for(unsigned int posx = 0; posx < resolutionVector_.size(); posx++)
    {
        for(int posy = 0; posy < ui_->fitterOptions->count()-3; posy++)
        {
            if(resolutionCheckBoxes_[posy][posx]->isChecked() == false)
                checkIt = false;
        }
        generalResolutionCheckBoxes_.at(posx)->setChecked(checkIt);

        checkIt = true;

    }
}
//===========================================================================
void HanSoloFitter::checkAllEfficiency(void)
{
    for(unsigned int pos = 0; pos < generalEfficiencyCheckBoxes_.size(); pos++){



        if(generalEfficiencyCheckBoxes_.at(pos)->isChecked()){
            for(unsigned int posy = 0; posy < efficiencyCheckBoxes_.size(); posy++){
                efficiencyCheckBoxes_[posy][pos]->setChecked(true);
            }
        }
    }

}
//===========================================================================
void HanSoloFitter::checkAllResolution()
{
    for(unsigned int pos = 0; pos < generalResolutionCheckBoxes_.size(); pos++){



        if(generalResolutionCheckBoxes_.at(pos)->isChecked()){
            for(unsigned int posy = 0; posy < resolutionCheckBoxes_.size(); posy++){
                resolutionCheckBoxes_[posy][pos]->setChecked(true);
            }
        }

    }

}
//===========================================================================
//====================== Handle Fit Checkboxes [END] ========================
//===========================================================================

//===========================================================================
//============================ Root File [START] ============================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_openRootFile_clicked(void)
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
//============================== Root File [END] ============================
//===========================================================================

//===========================================================================
//======================== Configuration [START] ============================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_saveConfigurationFile_clicked(void)
{
    std::string filePath = "/home/parilla/Programming/Chewie/ChewieExtended/Configuration/test.xml";

    QFile file(ui_->configurationFileFrame->text());

    QDomDocument doc;

    doc.setContent(&file);

    QDomElement documentElement = doc.documentElement();

    QDomNode root;

    if(documentElement.hasChildNodes())
    {
        root = documentElement.elementsByTagName("HanSoloFitter").at(0);
    }else
    {
        root = doc.createElement("HanSoloFitter");
        root.toElement().setAttribute("id", "configuration");
        doc.appendChild(root);
    }
    QDomNode dut;


    for(unsigned int posx = 0; posx < chargeCheckBoxes_.size(); posx++)
    {


        if(root.hasChildNodes())
        {
            if(posx == 0){dut = root.firstChild();}
            else
            {
                if(!dut.nextSibling().isNull())
                {
                    dut = dut.nextSibling();
                }else
                {
                    dut = doc.createElement("Dut");
                    root.appendChild(dut);
                }


            }
        }else
        {
            dut = doc.createElement("Dut");
            root.appendChild(dut);
        }

        for(int pos = 0; pos < root.childNodes().size(); pos++)
        {
            if(pos >= (int)chargeCheckBoxes_.size())
            {
                root.removeChild(root.childNodes().at(pos));
                pos--;
            }

        }


        dut.toElement().setAttribute("id", posx);
        QDomNode charge;
        QDomNode efficiency;
        QDomNode resolution;

        if(dut.hasChildNodes())
        {
            charge = dut.firstChild();
            efficiency = charge.nextSibling();
            resolution = efficiency.nextSibling();
        }else
        {
            charge = doc.createElement("charge");
            dut.appendChild(charge);
            efficiency = doc.createElement("efficiency");
            dut.appendChild(efficiency);
            resolution = doc.createElement("resolution");
            dut.appendChild(resolution);
        }



        do{
            charge.removeChild(charge.lastChild());

        }while(charge.childNodes().length() > 0);


        for(unsigned int posy = 0; posy < chargeCheckBoxes_[posx].size(); posy++)
        {
            QDomNode checkbox = doc.createElement("checkbox");
            charge.appendChild(checkbox);
            checkbox.toElement().setAttribute("fitterID", chargeCheckBoxes_[posx][posy]->text());

            if(chargeCheckBoxes_[posx][posy]->isChecked())
                checkbox.toElement().setAttribute("checked", true);
            else
                checkbox.toElement().setAttribute("checked", false);
        }





        do{
            efficiency.removeChild(efficiency.lastChild());

        }while(efficiency.childNodes().length() > 0);


        for(unsigned int posy = 0; posy < efficiencyCheckBoxes_[posx].size(); posy++)
        {
            QDomNode checkbox = doc.createElement("checkbox");
            efficiency.appendChild(checkbox);
            checkbox.toElement().setAttribute("fitterID", efficiencyCheckBoxes_[posx][posy]->text());

            if(efficiencyCheckBoxes_[posx][posy]->isChecked())
                checkbox.toElement().setAttribute("checked", true);
            else
                checkbox.toElement().setAttribute("checked", false);
        }

        do{
            resolution.removeChild(resolution.lastChild());

        }while(resolution.childNodes().length() > 0);


        for(unsigned int posy = 0; posy < resolutionCheckBoxes_[posx].size(); posy++)
        {
            QDomNode checkbox = doc.createElement("checkbox");
            resolution.appendChild(checkbox);
            checkbox.toElement().setAttribute("fitterID", resolutionCheckBoxes_[posx][posy]->text());

            if(resolutionCheckBoxes_[posx][posy]->isChecked())
                checkbox.toElement().setAttribute("checked", true);
            else
                checkbox.toElement().setAttribute("checked", false);
        }

    }


    file.close();
    std::string beginning;
    std::string ending;
    std::string beforeSoloConfiguration;
    std::string afterSoloConfiguration;
    std::string newConfiguration;

    beginning = "<HanSoloFitter id=\"configuration\">";
    ending = "</HanSoloFitter>";

    std::ifstream myFile(filePath.c_str());
    std::string fileString((std::istreambuf_iterator<char>(myFile)), std::istreambuf_iterator<char>());


    beginning = "<HanSoloFitter id=\"configuration\">";
    ending = "</HanSoloFitter>";

    std::size_t firstIndex = fileString.find(beginning);
    std::size_t secondIndex = fileString.find(ending);



    if(firstIndex!=std::string::npos && firstIndex!=std::string::npos)
    {
        beforeSoloConfiguration = fileString.substr(0, firstIndex);
        afterSoloConfiguration = fileString.substr(secondIndex+ending.size(), (fileString.size() - (secondIndex+ending.size())));


        newConfiguration = doc.toString().toUtf8().constData();

        myFile.close();

	std::ofstream overWrite;
        overWrite.open(filePath.c_str());
        overWrite << newConfiguration;
        overWrite.close();

    }else{


        beginning = "</ChewieConfiguration>";
        ending = "</ChewieConfiguration>";

        std::size_t firstIndex = fileString.find(beginning);
        std::size_t secondIndex = fileString.find(ending);

        beforeSoloConfiguration = fileString.substr(0, firstIndex);
        afterSoloConfiguration = fileString.substr(secondIndex+ending.size());

        newConfiguration = doc.toString().toUtf8().constData();
        std::string completeFile = beforeSoloConfiguration + newConfiguration + afterSoloConfiguration;
	std::ofstream overWrite;
        overWrite.open(filePath.c_str());
        overWrite << completeFile;
        overWrite.close();

        myFile.close();
        return;

    }

    myFile.close();

    STDLINE("Saved!", ACGreen);
}

//===========================================================================
void HanSoloFitter::on_openConfigurationFilePB_clicked(void)
{
    QString localPath = this->getEnvPath("CHEWIEXMLDIR");
    QString fileName = QFileDialog::getOpenFileName(this,"Xml configuration file",localPath,"Xml Files(*.xml)");
    if(fileName.isEmpty())
    {
        ui_->configurationFileFrame->setText("Load configuration file (.xml)");
        theXmlParser_->destroy();
        return ;
    }

    for(int pos = 1;pos < ui_->fitterOptions->count()-3; pos++){

        ui_->fitterOptions->removeTab(pos);
        --pos;
    }

    ui_->configurationFileFrame->setText(fileName);

    QDomDocument xmlDoc;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text ))
    {
        STDLINE(std::string("Could not open ")+file.fileName().toStdString(),ACRed);
        return;
    }
    xmlDoc.setContent(&file);
    file.close();

    QDomElement root = xmlDoc.documentElement();

    QDomNodeList HanSoloFitterNode = root.childNodes();
    QDomElement HanSoloXML;

    for(int pos = 0; pos < HanSoloFitterNode.size(); pos++){

        if(HanSoloFitterNode.at(pos).toElement().attribute("id") == "configuration")
        {
            HanSoloXML = HanSoloFitterNode.at(pos).toElement();
            break;
        }
    }

    QDomNodeList dut = HanSoloXML.elementsByTagName("Dut");

    for(int posx = 0; posx < dut.size(); posx++)
    {
        on_fitterOptions_selected("+");


        QDomNode typeOfFit = dut.at(posx);

        QDomElement charge = typeOfFit.firstChildElement();

        QDomNodeList checkbox = charge.elementsByTagName("checkbox");


        for(unsigned int posy = 0; posy < chargeCheckBoxes_[posx].size(); posy++){

            for(int counter = 0; counter < checkbox.size(); counter++){

                QString name = chargeCheckBoxes_[posx][posy]->text();
                QString checked = checkbox.at(counter).toElement().attribute("checked");
                QString fitter = checkbox.at(counter).toElement().attribute("fitterID");

                if(name.compare(fitter) == 0)
                {
                    bool check;

                    int yes = checkbox.at(counter).toElement().attribute("checked").toInt();

                    if(yes == 1)
                    {
                        check = true;
                    }else
                        check = false;

                    chargeCheckBoxes_[posx][posy]->setChecked(check);
                    break;
                }
            }

            QDomElement efficiency = charge.nextSiblingElement();

            checkbox = efficiency.elementsByTagName("checkbox");

            for(unsigned int posy = 0; posy < efficiencyCheckBoxes_[posx].size(); posy++){

                for(int counter = 0; counter < checkbox.size(); counter++){

                    QString name = efficiencyCheckBoxes_[posx][posy]->text();
                    QString checked = checkbox.at(counter).toElement().attribute("checked");
                    QString fitter = checkbox.at(counter).toElement().attribute("fitterID");

                    if(name.compare(fitter) == 0)
                    {
                        bool check;

                        int yes = checkbox.at(counter).toElement().attribute("checked").toInt();

                        if(yes == 1)
                        {
                            check = true;
                        }else
                            check = false;

                        efficiencyCheckBoxes_[posx][posy]->setChecked(check);
                        break;
                    }
                }
            }


            QDomElement resolution = efficiency.nextSiblingElement();

            checkbox = resolution.elementsByTagName("checkbox");

            for(unsigned int posy = 0; posy < resolutionCheckBoxes_[posx].size(); posy++){

                for(int counter = 0; counter < checkbox.size(); counter++){

                    QString name = resolutionCheckBoxes_[posx][posy]->text();
                    QString checked = checkbox.at(counter).toElement().attribute("checked");
                    QString fitter = checkbox.at(counter).toElement().attribute("fitterID");

                    if(name.compare(fitter) == 0)
                    {
                        bool check;

                        int yes = checkbox.at(counter).toElement().attribute("checked").toInt();

                        if(yes == 1)
                        {
                            check = true;
                        }else
                            check = false;


                        resolutionCheckBoxes_[posx][posy]->setChecked(check);
                        break;
                    }
                }
            }

        }


    }

    check_for_checkall();
}
//===========================================================================
//========================== Configuration [END] ============================
//===========================================================================

//===========================================================================
//========================= Dynamic Tabs [START] ============================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_fitterOptions_selected(const QString &arg1)
{
    if(arg1.compare("-") == 0){

        if(ui_->fitterOptions->count() > 3){
            ui_->fitterOptions->setCurrentIndex(ui_->fitterOptions->count()-4);
            ui_->fitterOptions->widget(ui_->fitterOptions->count()-3)->deleteLater();
            chargeCheckBoxes_.resize(ui_->fitterOptions->count()-4);
            efficiencyCheckBoxes_.resize(ui_->fitterOptions->count()-4);
            resolutionCheckBoxes_.resize(ui_->fitterOptions->count()-4);
        }else
            ui_->fitterOptions->setCurrentIndex(ui_->fitterOptions->count()-3);

    }else if(arg1.compare("+") == 0)
    {
        QWidget * newDutTab = new QWidget(ui_->fitterOptions);
        pointerToDutTabs_.push_back(newDutTab);

        QLabel * chargeLabel = new QLabel();
        chargeLabel->setStyleSheet("font-size:14pt; font-weight:600;");
        chargeLabel->setText("Charge");
        chargeLabel->setParent(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        chargeLabel->setWhatsThis("Charge");
        chargeLabel->setGeometry(ui_->label_4->geometry());
        chargeLabel->show();

        QLabel * efficiencyLabel = new QLabel();
        efficiencyLabel->setStyleSheet("font-size:14pt; font-weight:600;");
        efficiencyLabel->setText("Efficiency");
        efficiencyLabel->setParent(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        efficiencyLabel->setWhatsThis("Efficiency");
        efficiencyLabel->setGeometry(ui_->label_5->geometry());
        efficiencyLabel->show();

        QLabel * resolutionLabel = new QLabel();
        resolutionLabel->setStyleSheet("font-size:14pt; font-weight:600;");
        resolutionLabel->setText("Resolution");
        resolutionLabel->setParent(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        resolutionLabel->setWhatsThis("Resolution");
        resolutionLabel->setGeometry(ui_->label_6->geometry());
        resolutionLabel->show();

        QFrame * chargeFrame = new QFrame(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        chargeFrame->setParent(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        chargeFrame->setGeometry(ui_->chargeFrame->geometry());
        chargeFrame->setWhatsThis("Charge Frame");
        chargeFrame->show();

        QFrame * efficiencyFrame = new QFrame(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        efficiencyFrame->setParent(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        efficiencyFrame->setGeometry(ui_->efficiencyFrame->geometry());
        efficiencyFrame->setWhatsThis("Efficiency Frame");
        efficiencyFrame->show();

        QFrame * resolutionFrame = new QFrame(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        resolutionFrame->setParent(pointerToDutTabs_.at(pointerToDutTabs_.size()-1));
        resolutionFrame->setGeometry(ui_->resolutionFrame->geometry());
        resolutionFrame->setWhatsThis("Resolution Frame");
        resolutionFrame->show();

        QString title = QString("Dut_");
        QString number = QString::number(ui_->fitterOptions->count()-3);
        title = title + number;

        int SPACING = 40, XBORDER = 10, YBORDER = 20;
        QString qstr;

        chargeCheckBoxes_.resize(ui_->fitterOptions->count()-2);
        efficiencyCheckBoxes_.resize(ui_->fitterOptions->count()-2);
        resolutionCheckBoxes_.resize(ui_->fitterOptions->count()-2);

        for(unsigned int pos = 0; pos < chargeCheckBoxes_[ui_->fitterOptions->count()-3].size(); pos++)
        {
            delete chargeCheckBoxes_[ui_->fitterOptions->count()-3].at(pos);
        }
        for(unsigned int pos = 0; pos < efficiencyCheckBoxes_[ui_->fitterOptions->count()-3].size(); pos++)
        {
            delete efficiencyCheckBoxes_[ui_->fitterOptions->count()-3].at(pos);
        }
        for(unsigned int pos = 0; pos < resolutionCheckBoxes_[ui_->fitterOptions->count()-3].size(); pos++)
        {
            delete resolutionCheckBoxes_[ui_->fitterOptions->count()-3].at(pos);
        }



        chargeCheckBoxes_[ui_->fitterOptions->count()-3].resize(chargeVector_.size());
        efficiencyCheckBoxes_[ui_->fitterOptions->count()-3].resize(efficiencyVector_.size());
        resolutionCheckBoxes_[ui_->fitterOptions->count()-3].resize(resolutionVector_.size());

        //Charge
        for(unsigned int pos = 0; pos < chargeVector_.size(); pos++)
        {
            if(pos > 14)
                XBORDER = 245;

            std::string type = "cha";
            std::string name = chargeVector_[pos].c_str();
            name = name.substr (0, name.find('.'));
            QCheckBox * chargeCheckBox = new QCheckBox(chargeFrame);
            chargeCheckBoxes_[ui_->fitterOptions->count()-3][pos] = chargeCheckBox;
            chargeCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setObjectName(chargeVector_[pos].c_str());
            chargeCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setText(qstr.fromStdString(name));
            chargeCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
            chargeCheckBoxes_[ui_->fitterOptions->count()-3][pos]->show();
            if(generalChargeCheckBoxes_[pos]->isChecked()){
                chargeCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setChecked(true);
            }
            connect(chargeCheckBoxes_[ui_->fitterOptions->count()-3][pos],
                    SIGNAL(clicked(bool)),
                    this,
                    SLOT(check_for_checkall()));

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
            QCheckBox * efficiencyCheckBox = new QCheckBox(efficiencyFrame);
            efficiencyCheckBoxes_[ui_->fitterOptions->count()-3][pos] = efficiencyCheckBox;
            efficiencyCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setObjectName(efficiencyVector_.at(pos).c_str());
            efficiencyCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setText(qstr.fromStdString(name));
            efficiencyCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
            efficiencyCheckBoxes_[ui_->fitterOptions->count()-3][pos]->show();
            if(generalEfficiencyCheckBoxes_[pos]->isChecked()){
                efficiencyCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setChecked(true);
            }
            connect(efficiencyCheckBoxes_[ui_->fitterOptions->count()-3][pos],
                    SIGNAL(clicked(bool)),
                    this,
                    SLOT(check_for_checkall()));
        }
        XBORDER = 10;

        //Resolution
        for(unsigned int pos = 0; pos < resolutionVector_.size(); pos++)
        {
            if(pos > 14)
                XBORDER = 245;
            std::string type = "res";
            std::string name = resolutionVector_[pos].c_str();
            name = name.substr (0, name.find('.'));
            QCheckBox * efficiencyCheckBox = new QCheckBox(resolutionFrame);
            resolutionCheckBoxes_[ui_->fitterOptions->count()-3][pos] = efficiencyCheckBox;
            resolutionCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setObjectName(resolutionVector_[pos].c_str());
            resolutionCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setText(qstr.fromStdString(name));
            resolutionCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setGeometry(XBORDER, (YBORDER + (pos * SPACING)), 200, 20);
            resolutionCheckBoxes_[ui_->fitterOptions->count()-3][pos]->show();
            if(generalResolutionCheckBoxes_[pos]->isChecked()){
                resolutionCheckBoxes_[ui_->fitterOptions->count()-3][pos]->setChecked(true);
            }
            connect(resolutionCheckBoxes_[ui_->fitterOptions->count()-3][pos],
                    SIGNAL(clicked(bool)),
                    this,
                    SLOT(check_for_checkall()));
        }

        ui_->fitterOptions->insertTab(ui_->fitterOptions->count()-2, newDutTab, title);
        ui_->fitterOptions->setTabText(ui_->fitterOptions->count()-3, title);
        ui_->fitterOptions->setCurrentIndex(ui_->fitterOptions->count()-3);
    }
}

//===========================================================================
void HanSoloFitter::on_fitterOptions_currentChanged(int index)
{
    currentIndex_ = index;
}
//===========================================================================
//========================== Dynamic Tabs [END] =============================
//===========================================================================


//===========================================================================
//======================== Reset Buttons [START] ============================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_reset_clicked(void)
{
    for(unsigned int posx = 0; posx < chargeCheckBoxes_.size(); posx++)
    {
        for(unsigned posy = 0; posy < chargeCheckBoxes_[posx].size(); posy++){
            chargeCheckBoxes_[posx][posy]->setChecked(false);
        }
    }
    for(unsigned int posx = 0; posx < efficiencyCheckBoxes_.size(); posx++)
    {
        for(unsigned posy = 0; posy < efficiencyCheckBoxes_[posx].size(); posy++){
            efficiencyCheckBoxes_[posx][posy]->setChecked(false);
        }
    }
    for(unsigned int posx = 0; posx < resolutionCheckBoxes_.size(); posx++)
    {
        for(unsigned posy = 0; posy < resolutionCheckBoxes_[posx].size(); posy++){
            resolutionCheckBoxes_[posx][posy]->setChecked(false);
        }
    }
    for(unsigned int posx = 0; posx < generalChargeCheckBoxes_.size(); posx++)
    {
        generalChargeCheckBoxes_[posx]->setChecked(false);
    }
    for(unsigned int posx = 0; posx < generalEfficiencyCheckBoxes_.size(); posx++)
    {
        generalEfficiencyCheckBoxes_[posx]->setChecked(false);
    }
    for(unsigned int posx = 0; posx < generalResolutionCheckBoxes_.size(); posx++)
    {
        generalResolutionCheckBoxes_[posx]->setChecked(false);
    }

}
//===========================================================================
void HanSoloFitter::on_resetDut_clicked(void)
{
    if(currentIndex_ != 0)
    {
        for(unsigned int pos = 0; pos < chargeCheckBoxes_[currentIndex_-1].size(); pos++)
        {
            chargeCheckBoxes_[currentIndex_-1][pos]->setChecked(false);
        }
        for(unsigned int pos = 0; pos < efficiencyCheckBoxes_[currentIndex_-1].size(); pos++)
        {
            efficiencyCheckBoxes_[currentIndex_-1][pos]->setChecked(false);
        }
        for(unsigned int pos = 0; pos < resolutionCheckBoxes_[currentIndex_-1].size(); pos++)
        {
            resolutionCheckBoxes_[currentIndex_-1][pos]->setChecked(false);
        }
    }else
    {
        for(unsigned int posx = 0; posx < chargeCheckBoxes_.size(); posx++)
        {
            for(unsigned posy = 0; posy < chargeCheckBoxes_[posx].size(); posy++){
                chargeCheckBoxes_[posx][posy]->setChecked(false);
            }
        }
        for(unsigned int posx = 0; posx < efficiencyCheckBoxes_.size(); posx++)
        {
            for(unsigned posy = 0; posy < efficiencyCheckBoxes_[posx].size(); posy++){
                efficiencyCheckBoxes_[posx][posy]->setChecked(false);
            }
        }
        for(unsigned int posx = 0; posx < resolutionCheckBoxes_.size(); posx++)
        {
            for(unsigned posy = 0; posy < resolutionCheckBoxes_[posx].size(); posy++){
                resolutionCheckBoxes_[posx][posy]->setChecked(false);
            }
        }
        for(unsigned int posx = 0; posx < generalChargeCheckBoxes_.size(); posx++)
        {
            generalChargeCheckBoxes_[posx]->setChecked(false);
        }
        for(unsigned int posx = 0; posx < generalEfficiencyCheckBoxes_.size(); posx++)
        {
            generalEfficiencyCheckBoxes_[posx]->setChecked(false);
        }
        for(unsigned int posx = 0; posx < generalResolutionCheckBoxes_.size(); posx++)
        {
            generalResolutionCheckBoxes_[posx]->setChecked(false);
        }

    }
}
//===========================================================================
//========================= Reset Buttons [END] =============================
//===========================================================================

//===========================================================================
//========================== Run Button [START] =============================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_runButton_clicked(void)
{
    //GENERAL
    for(unsigned int pos = 0; pos< checkedBoxes_.size(); pos++)
        checkedBoxes_[pos].clear();
    for(unsigned int pos = 0; pos< checkedChargeCheckBoxes_.size(); pos++)
        checkedChargeCheckBoxes_[pos].clear();
    for(unsigned int pos = 0; pos< checkedEfficiencyCheckBoxes_.size(); pos++)
        checkedEfficiencyCheckBoxes_[pos].clear();
    for(unsigned int pos = 0; pos< checkedResolutionCheckBoxes_.size(); pos++)
        checkedResolutionCheckBoxes_[pos].clear();

    generalCheckedBoxes_.clear();

    checkedChargeCheckBoxes_.resize(chargeCheckBoxes_.size());

    for(unsigned int posx = 0; posx < chargeCheckBoxes_.size(); posx++)
    {
        for(unsigned int posy = 0; posy < chargeCheckBoxes_[posx].size(); posy++)
        {
            if(chargeCheckBoxes_[posx].at(posy)->isChecked())
            {

                checkedChargeCheckBoxes_[posx].push_back(chargeCheckBoxes_[posx][posy]->text().toUtf8().constData());
            }
        }
    }
    checkedEfficiencyCheckBoxes_.resize(efficiencyCheckBoxes_.size());

    for(unsigned int posx = 0; posx < efficiencyCheckBoxes_.size(); posx++)
    {

        for(unsigned int posy = 0; posy < efficiencyCheckBoxes_[posx].size(); posy++)
        {
            if(efficiencyCheckBoxes_[posx].at(posy)->isChecked())
            {
                checkedEfficiencyCheckBoxes_.resize(posx);
                checkedEfficiencyCheckBoxes_[posx].push_back(efficiencyCheckBoxes_[posx][posy]->text().toUtf8().constData());
            }
        }
    }
    checkedResolutionCheckBoxes_.resize(resolutionCheckBoxes_.size());

    for(unsigned int posx = 0; posx < resolutionCheckBoxes_.size(); posx++)
    {
        for(unsigned int posy = 0; posy < resolutionCheckBoxes_[posx].size(); posy++)
        {
            if(resolutionCheckBoxes_[posx].at(posy)->isChecked())
            {
                checkedResolutionCheckBoxes_.resize(posx);
                checkedResolutionCheckBoxes_[posx].push_back(resolutionCheckBoxes_[posx][posy]->text().toUtf8().constData());
            }
        }
    }
    for(int pos = checkedChargeCheckBoxes_.size()-1; pos >= 0; pos--)
    {
        if(checkedChargeCheckBoxes_[pos].size() == 0)
        {
            checkedChargeCheckBoxes_.erase(checkedChargeCheckBoxes_.end()-1, checkedChargeCheckBoxes_.end());
        }else
            break;
    }
    for(int pos = checkedEfficiencyCheckBoxes_.size()-1; pos >= 0; pos--)
    {
        if(checkedEfficiencyCheckBoxes_[0].size() == 0)
        {
            checkedEfficiencyCheckBoxes_.erase(checkedEfficiencyCheckBoxes_.end()-1, checkedEfficiencyCheckBoxes_.end());
        }else
            break;
    }
    for(int pos = checkedResolutionCheckBoxes_.size()-1; pos >= 0; pos--)
    {
        if(checkedResolutionCheckBoxes_[0].size() == 0)
        {
            checkedResolutionCheckBoxes_.erase(checkedResolutionCheckBoxes_.end()-1, checkedResolutionCheckBoxes_.end());
        }else
            break;
    }
    std::stringstream ss;
    std::string fits = "";
    std::string include = "";
    std::string fittedFileName = rootFile_.substr(0, rootFile_.find('.')) + "_fitted.root";

    include = "#include \"../include/Charge.h\"  \n#include \"../include/Efficiency.h\" \n#include \"../include/Resolution.h\" \n";


    //fits = fits + "\nCharge charge; \n charge.load(inFile); \nResolution resolution; \n resolution.load(inFile); \nEfficiency efficiency; \n efficiency.load(inFile); \n";
    if(checkedChargeCheckBoxes_.size() != 0)
    {
        fits = fits + "\n//Charge \nCharge charge; \n charge.load(inFile);\n";
        for(unsigned int posx = 0; posx < checkedChargeCheckBoxes_.size(); posx ++)
        {
            std::stringstream ss;
            ss.str("");
            ss << posx;
            fits = fits + "\n//Dut_" + ss.str() + "\n";

            for(unsigned int posy = 0; posy < checkedChargeCheckBoxes_[posx].size(); posy++)
            {
                fits = fits + "\ninFile->cd();\ncharge." +  checkedChargeCheckBoxes_[posx][posy] + "(" + ss.str()  + "); \n";
            }
        }
    }
    if(checkedEfficiencyCheckBoxes_.size() > 0)
    {
        fits = fits + "\n//Efficiency \nEfficiency efficiency; \n efficiency.load(inFile);\n";
        for(unsigned int posx = 0; posx < checkedEfficiencyCheckBoxes_.size(); posx ++)
        {


            std::stringstream ss;
            ss.str("");
            ss << posx;
            fits = fits + "\n//Dut_" + ss.str() + "\n";

            for(unsigned int posy = 0; posy < checkedEfficiencyCheckBoxes_[posx].size(); posy++)
            {
                fits = fits + "\ninFile->cd();\nefficiency." +  checkedEfficiencyCheckBoxes_[posx][posy] + "(" + ss.str()  + "); \n";
            }
        }
    }
    if(checkedResolutionCheckBoxes_.size() > 0)
    {

        fits = fits + "\n//Resolution \nResolution resolution; \n resolution.load(inFile);\n";

        for(unsigned int posx = 0; posx < checkedResolutionCheckBoxes_.size(); posx ++)
        {
            std::stringstream ss;
            ss.str("");
            ss << posx;
            fits = fits + "\n//Dut_" + ss.str() + "\n";

            for(unsigned int posy = 0; posy < checkedResolutionCheckBoxes_[posx].size(); posy++)
            {
                fits = fits + "\ninFile->cd();\nresolution." +  checkedResolutionCheckBoxes_[posx][posy] + "(" + ss.str()  + "); \n";
            }
        }
    }

    std::fstream file("ChewieExtended/ScriptToFit.cpp", std::ios::out);
    file.close();

    std::fstream scriptFile("ChewieExtended/ScriptToFit.cpp");
    std::string filePath = "ChewieExtended/ScriptToFit.cpp";

    if(scriptFile.is_open())
    {
        scriptFile << "#include <TFile.h>";
        scriptFile << "\n#include <TObject.h>\n";
        scriptFile << include;
        scriptFile << "int main()";
        scriptFile << "\n { \n    TFile* inFile = TFile::Open(\"";
        scriptFile << fittedFileName;
        scriptFile << "\",\"UPDATE\"); \n \n \n//Place all the fitters here \n  inFile->cd(); \n    ";
        scriptFile << fits;
        scriptFile << " \n \ninFile->Write(\"\",TObject::kOverwrite); \n ";
        scriptFile << "inFile->Close(); \n \n return 0; \n}";

        scriptFile.close();

        std::string cp = "cp " + rootFile_ + " " + fittedFileName ;
        system(cp.c_str());
        system("pwd");
        system("cd ChewieExtended; ./compile.py && ./ScriptToFit");
        system("cd ChewieExtended; ./ScriptToFit");
    }
}
//===========================================================================
//========================== Run Buttons [END] ==============================
//===========================================================================


//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//============================================ HanSoloFitter [Viewer] ==============================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================


//===========================================================================
//======================== File System Sidebar [Start] ======================
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


    this->fillWidget()     ; // Fill the combo-box with the list of open files

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
    this->refresh() ;
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
        TFile* file=TFile::Open(fileNames[f].toStdString().c_str(),"READ");
        this->addFile(fileNames[f].toStdString().c_str(),file);
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
//=================== Canvas Display Histogram Display [Start]=====================
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
//===========================================================================
//====================== Canvas Selection [START] ===========================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_hCanvasCB_activated(int index)
{
    //    STDLINE("on_hCanvasCB_activated",ACRed);
    hanSoloTreeBrowser_->setCurrentCanvas(index) ;
}
//===========================================================================
//====================== Canvas Selection [END]==============================
//===========================================================================


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
//===========================================================================
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

    return ui_->fitCB->isChecked();
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
CanvasWidget* HanSoloFitter::getCanvas(void)
{
    return (CanvasWidget*) canvas_;

}

//===========================================================================
//======================= Range Buttons [START] =============================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_unZoomPB_clicked(void)
{
    hanSoloTreeBrowser_->unZoom() ;
    std::string str = "Set Range: \nX-Range: [0, 0] \nY-Range: [0, 0] \nZ-Range: [0, 0]";

    STDLINE(str, ACGreen);
}
//===========================================================================
void HanSoloFitter::on_setRangePB_clicked(void)
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
void HanSoloFitter::on_shiftPlusPB_clicked(void)
{
    float shiftMin = ui_->xMinLE->text().toFloat()+ui_->shiftLE->text().toFloat();
    float shiftMax = ui_->xMaxLE->text().toFloat()+ui_->shiftLE->text().toFloat();
    ui_->xMinLE->setText(QString("%1").arg(shiftMin));
    ui_->xMaxLE->setText(QString("%1").arg(shiftMax));
    emit on_setRangePB_clicked();
}

//===========================================================================
void HanSoloFitter::on_shiftMinusPB_clicked(void)
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
    int delta = value - shiftHSValue_;

    float shiftMin = ui_->xMinLE->text().toFloat()+delta*ui_->shiftLE->text().toFloat();
    float shiftMax = ui_->xMaxLE->text().toFloat()+delta*ui_->shiftLE->text().toFloat();

    ui_->xMinLE->setText(QString("%1").arg(shiftMin));
    ui_->xMaxLE->setText(QString("%1").arg(shiftMax));

    shiftHSValue_ = value;
    emit on_setRangePB_clicked();
}
//===========================================================================
//======================== Range Buttons [END] ==============================
//===========================================================================
//===========================================================================

//===========================================================================
//======================== Log Buttons [START] ==============================
//===========================================================================
//===========================================================================
bool HanSoloFitter::logX(void)
{
    return ui_->logxCB->isChecked();
}

//===========================================================================
bool HanSoloFitter::logY(void)
{
    return ui_->logyCB->isChecked();
}

//===========================================================================
bool HanSoloFitter::logZ(void)
{
    return ui_->logzCB->isChecked();
}
//===========================================================================
//========================= Log Buttons [END] ===============================
//===========================================================================
//===========================================================================


//===========================================================================
//========================= Stats Box [START] ===============================
//===========================================================================
//===========================================================================
bool HanSoloFitter::plotStatBox(void)
{
    return ui_->hanSoloStatCB->isChecked();
}
//===========================================================================
//========================== Stats Box [END] ================================
//===========================================================================


//===========================================================================
//======================= Fit Manager [START] ===============================
//===========================================================================
//===========================================================================
void HanSoloFitter::on_fitPB_clicked(void)
{

    STDLINE("Starting Fit!", ACRed);

    ui_->fitPB->setEnabled(false);

    std::string fitMethod = ui_->fitMethodLE->text().toStdString();

    if (!hanSoloTreeBrowser_->getCurrentObject())
    {
        STDLINE("Select a histogram to fit", ACYellow);
        ui_->fitPB->setEnabled(true);
        return;
    }

    TObject * toFit = hanSoloTreeBrowser_->getCurrentObject();
    TH1F * toFit1D = 0;
//    TH2F * toFit2D = 0;
    int hDimension = 0;
    if (this->getObjectType(toFit).find("TH1") != std::string::npos){
        toFit1D = (TH1F*)toFit;
        hDimension = 1;
    }
    if (this->getObjectType(toFit).find("TH2") != std::string::npos){
//        toFit2D = (TH2F*)toFit;
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

        std::string function = ui_->fitFuncLW->currentItem()->text().toUtf8().constData();

        if (!ui_->customFunctionCB->isChecked())
        {

            TF1 * fitter = 0;
            if (ui_->fitFuncLW->currentItem()->text() == "Gauss")
                fitter = new TF1(hName.c_str(), "gaus + [3]", xMin, xMax);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta")
                fitter = new TF1(hName.c_str(), Utilities::etaDistribution, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta (No Depletion)")
                fitter = new TF1(hName.c_str(), Utilities::etaFitFunc2, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta with Cut")
                fitter = new TF1(hName.c_str(), Utilities::etaFitFunc_cut, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta (No Depletion) with Cut")
                fitter = new TF1(hName.c_str(), Utilities::etaFitFunc_cut2, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta Inverse")
                fitter = new TF1(hName.c_str(), Utilities::etaInverseFitFunc2Angle, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Langaus")
                fitter = new TF1(hName.c_str(), Utilities::langaus, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Gauss Convoluted with Constant")
                fitter = new TF1(hName.c_str(), Utilities::uniformSideSmeared, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta Convoluted with Gauss")
                fitter = new TF1(hName.c_str(), Utilities::etaSmeared, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Gauss-constant plus Constant")
                fitter = new TF1(hName.c_str(), Utilities::uniformCenterSmearedPlusConstant, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Single constant Gauss-conv.")
                fitter = new TF1(hName.c_str(), Utilities::singleUniformSmeared, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Double constant Gauss-conv.")
                fitter = new TF1(hName.c_str(), Utilities::doubleUniformSmeared, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta Reproduction")
                fitter = new TF1(hName.c_str(), Utilities::etaReproduction, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Eta Distribution")
                fitter = new TF1(hName.c_str(), Utilities::etaDistributionAngle, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else if (ui_->fitFuncLW->currentItem()->text() == "Linear")
                fitter = new TF1(hName.c_str(), Utilities::linear, xMin, xMax, hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]);
            else
                STDLINE("Select a function!", ACYellow);

            //hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()].first TOFIX
            fitter->SetLineColor(kBlue);
            for (int i = 0; i < hanSoloFitFunctions_[ui_->fitFuncLW->currentItem()->text().toStdString()]; ++i)
            {
                fitter->SetParName(i, hanSoloParamManager_[i]->getParName().c_str());
                if (hanSoloParamManager_[i]->isParFixed()) fitter->FixParameter(i, hanSoloParamManager_[i]->getParValue());
                else {
                    fitter->SetParameter(i, hanSoloParamManager_[i]->getParValue());
                    fitter->SetParLimits(i, hanSoloParamManager_[i]->getParLimInf(), hanSoloParamManager_[i]->getParLimSup());
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
            for (unsigned int i = 0; i < hanSoloParamManager_.size(); ++i)
            {
                fitter->SetParName(i, hanSoloParamManager_[i]->getParName().c_str());
                if (hanSoloParamManager_[i]->isParFixed()) fitter->FixParameter(i, hanSoloParamManager_[i]->getParValue());
                else {
                    fitter->SetParameter(i, hanSoloParamManager_[i]->getParValue());
                    fitter->SetParLimits(i, hanSoloParamManager_[i]->getParLimInf(), hanSoloParamManager_[i]->getParLimSup());
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
    if(ui_->fitCB->isChecked()) {checkBoxesHandler(true);}
    STDLINE("Finished!", ACGreen);
    return;
}
void HanSoloFitter::on_fitFuncLW_itemClicked(QListWidgetItem *item)
{
    if (ui_->customFunctionCB->isChecked()) return;

    std::stringstream ss;

    for (unsigned int j = 0; j < hanSoloParamManager_.size(); ++j)
    {
        delete hanSoloParamManager_[j];
        ui_->parametersTW->removeTab(j);
    }
    hanSoloParamManager_.clear();

    if (item->text() == "Eta")
    {
        for (int i = 0; i < 6; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                        ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("2*mu*DepletionVoltage            ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(9.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParName  ("DepletionVoltage                 ");
                hanSoloParamManager_[i]->setParValue (25.);
                hanSoloParamManager_[i]->setParLimInf(10.);
                hanSoloParamManager_[i]->setParLimSup(100.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1.);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("AppliedVoltage + DepletionVoltage");
                hanSoloParamManager_[i]->setParValue (525.);
                hanSoloParamManager_[i]->setParLimInf(510.);
                hanSoloParamManager_[i]->setParLimSup(600.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1.);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant              ");
                hanSoloParamManager_[i]->setParValue (0.0007);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 5)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor                      ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Eta (No Depletion)")
    {
        for (int i = 0; i < 4; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                     ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                hanSoloParamManager_[i]->setParValue (6.5);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(30.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                hanSoloParamManager_[i]->setParValue (0.0007);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor                   ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );
        }
    }

    else if (item->text() == "Eta Derivative")
    {
        for (int i = 0; i < 5; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                     ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                hanSoloParamManager_[i]->setParValue (6.5);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(30.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                hanSoloParamManager_[i]->setParValue (0.0007);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor                   ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("Tan(angle)                    ");
                hanSoloParamManager_[i]->setParValue (0.);
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParLimInf(-1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(4);
                hanSoloParamManager_[i]->setParStep(0.0001);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Eta with Cut")
    {
        for (int i = 0; i < 7; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                        ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("2*mu*DepletionVoltage            ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(9.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParName  ("DepletionVoltage                 ");
                hanSoloParamManager_[i]->setParValue (25.);
                hanSoloParamManager_[i]->setParLimInf(10.);
                hanSoloParamManager_[i]->setParLimSup(100.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1.);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("AppliedVoltage + DepletionVoltage");
                hanSoloParamManager_[i]->setParValue (525.);
                hanSoloParamManager_[i]->setParLimInf(510.);
                hanSoloParamManager_[i]->setParLimSup(600.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1.);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant              ");
                hanSoloParamManager_[i]->setParValue (0.0007);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 5)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor                      ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 6)
            {
                hanSoloParamManager_[i]->setParName  ("CutThreshold                    ");
                hanSoloParamManager_[i]->setParValue (0.0005);
                hanSoloParamManager_[i]->setParLimInf(0.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(4);
                hanSoloParamManager_[i]->setParStep(0.0001);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Eta (No Depletion) with Cut")
    {
        for (int i = 0; i < 5; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                     ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                hanSoloParamManager_[i]->setParValue (6.5);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(30.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                hanSoloParamManager_[i]->setParValue (0.0007);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor                   ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("CutThreshold                 ");
                hanSoloParamManager_[i]->setParValue (0.0005);
                hanSoloParamManager_[i]->setParLimInf(0.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(4);
                hanSoloParamManager_[i]->setParStep(0.0001);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );
        }
    }

    else if (item->text() == "Eta Inverse")
    {
        for (int i = 0; i < 5; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                     ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                hanSoloParamManager_[i]->setParValue (6.5);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(30.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                hanSoloParamManager_[i]->setParValue (0.0007);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor                   ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("Tan(theta)                    ");
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(-1);
                hanSoloParamManager_[i]->setParLimSup(1);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(4);
                hanSoloParamManager_[i]->setParStep(0.0001);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Langaus")
    {
        for (int i = 0; i < 4; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Width");
                hanSoloParamManager_[i]->setParLimits(10, 10000);
                hanSoloParamManager_[i]->setParValue (3000);
                hanSoloParamManager_[i]->setParLimInf(1000);
                hanSoloParamManager_[i]->setParLimSup(10000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(10);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("MPV");
                hanSoloParamManager_[i]->setParLimits(10, 100000);
                hanSoloParamManager_[i]->setParValue (20000);
                hanSoloParamManager_[i]->setParLimInf(1000);
                hanSoloParamManager_[i]->setParLimSup(30000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(10);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParName  ("Area");
                hanSoloParamManager_[i]->setParLimits(10000, 1000000000);
                hanSoloParamManager_[i]->setParValue (10000000);
                hanSoloParamManager_[i]->setParLimInf(100000);
                hanSoloParamManager_[i]->setParLimSup(1000000000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1000);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("GSigma");
                hanSoloParamManager_[i]->setParLimits(10, 100000);
                hanSoloParamManager_[i]->setParValue (3000);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(10000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(10);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Gauss")
    {
        for (int i = 0; i < 4; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Constant");
                hanSoloParamManager_[i]->setParLimits(0, 100000);
                hanSoloParamManager_[i]->setParValue (1);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(1000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(1);
                hanSoloParamManager_[i]->setParStep(0.1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Mean");
                hanSoloParamManager_[i]->setParLimits(-100000, 100000);
                hanSoloParamManager_[i]->setParValue (1);
                hanSoloParamManager_[i]->setParLimInf(-100);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(1);
                hanSoloParamManager_[i]->setParStep(0.1);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParName  ("Sigma");
                hanSoloParamManager_[i]->setParValue (1);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(100.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("Vertical Translation");
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(1000.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Gauss Convoluted with Constant")
    {
        for (int i = 0; i < 4; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Pixel Limit");
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParValue (50);
                hanSoloParamManager_[i]->setParLimInf(50);
                hanSoloParamManager_[i]->setParLimSup(75);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Cluster 1 extension");
                hanSoloParamManager_[i]->setParValue (40);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(50);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            if (i == 2)
            {
                hanSoloParamManager_[i]->setParName  ("Sigma Gauss");
                hanSoloParamManager_[i]->setParValue (5);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(75);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("Scale Factor");
                hanSoloParamManager_[i]->setParLimits(0, 100000);
                hanSoloParamManager_[i]->setParValue (100);
                hanSoloParamManager_[i]->setParLimInf(10);
                hanSoloParamManager_[i]->setParLimSup(10000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );
        }
    }

    else if (item->text() == "Eta Convoluted with Gauss")
    {
        for (int i = 0; i < 9; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                     ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage) ");
                hanSoloParamManager_[i]->setParValue (6.5);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(30.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                hanSoloParamManager_[i]->setParValue (0.0700);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor                   ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("Extension of constant function");
                hanSoloParamManager_[i]->setParLimits(0, 1000);
                hanSoloParamManager_[i]->setParValue (4.);
                hanSoloParamManager_[i]->setParLimInf(0.);
                hanSoloParamManager_[i]->setParLimSup(50);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(3);
                hanSoloParamManager_[i]->setParStep(0.001);
            }
            else if (i == 5)
            {
                hanSoloParamManager_[i]->setParName  ("Sigma                         ");
                hanSoloParamManager_[i]->setParLimits(0,100);
                hanSoloParamManager_[i]->setParValue (10);
                hanSoloParamManager_[i]->setParLimInf(1);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(1);
                hanSoloParamManager_[i]->setParStep(0.1);
            }
            else if (i == 6)
            {
                hanSoloParamManager_[i]->setParName  ("Scale Factor                  ");
                hanSoloParamManager_[i]->setParLimits(0, 10000);
                hanSoloParamManager_[i]->setParValue (400);
                hanSoloParamManager_[i]->setParLimInf(100);
                hanSoloParamManager_[i]->setParLimSup(10000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 7)
            {
                hanSoloParamManager_[i]->setParName  ("Center of distribution        ");
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(-100);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(1);
                hanSoloParamManager_[i]->setParStep(0.1);
            }
            else if (i == 8)
            {
                hanSoloParamManager_[i]->setParName  ("Vertical translation Constant ");
                hanSoloParamManager_[i]->setParValue (100);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(1000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Single constant Gauss-conv.")
    {
        for (int i = 0; i < 6; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Pixel Extension");
                hanSoloParamManager_[i]->setParValue (75);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Center");
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(-10);
                hanSoloParamManager_[i]->setParLimSup(10);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParName  ("Well Width");
                hanSoloParamManager_[i]->setParValue (14);
                hanSoloParamManager_[i]->setParLimInf(1);
                hanSoloParamManager_[i]->setParLimSup(30);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("Max Value");
                hanSoloParamManager_[i]->setParValue (1);
                hanSoloParamManager_[i]->setParLimInf(0.);
                hanSoloParamManager_[i]->setParLimSup(10);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(3);
                hanSoloParamManager_[i]->setParStep(0.001);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("Min Value");
                hanSoloParamManager_[i]->setParValue (0.6);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(10);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(3);
                hanSoloParamManager_[i]->setParStep(0.001);
            }
            else if (i == 5)
            {
                hanSoloParamManager_[i]->setParName  ("Gauss Sigma");
                hanSoloParamManager_[i]->setParValue (10);
                hanSoloParamManager_[i]->setParLimInf(1);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Double constant Gauss-conv.")
    {
        for (int i = 0; i < 7; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Pixel Extension");
                hanSoloParamManager_[i]->setParValue (75);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Center");
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(-10);
                hanSoloParamManager_[i]->setParLimSup(10);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParName  ("Well Center");
                hanSoloParamManager_[i]->setParValue (40);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(75);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("Well Width");
                hanSoloParamManager_[i]->setParValue (14);
                hanSoloParamManager_[i]->setParLimInf(1);
                hanSoloParamManager_[i]->setParLimSup(30);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("Max Value");
                hanSoloParamManager_[i]->setParValue (1);
                hanSoloParamManager_[i]->setParLimInf(0.);
                hanSoloParamManager_[i]->setParLimSup(10);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(3);
                hanSoloParamManager_[i]->setParStep(0.001);
            }
            else if (i == 5)
            {
                hanSoloParamManager_[i]->setParName  ("Min Value");
                hanSoloParamManager_[i]->setParValue (0.6);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(10);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(3);
                hanSoloParamManager_[i]->setParStep(0.001);
            }
            else if (i == 6)
            {
                hanSoloParamManager_[i]->setParName  ("Gauss Sigma");
                hanSoloParamManager_[i]->setParValue (10);
                hanSoloParamManager_[i]->setParLimInf(1);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Gauss-constant plus Constant")
    {
        for (int i = 0; i < 5; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Extension of constant function");
                hanSoloParamManager_[i]->setParLimits(0, 100000);
                hanSoloParamManager_[i]->setParValue (4.);
                hanSoloParamManager_[i]->setParLimInf(0.);
                hanSoloParamManager_[i]->setParLimSup(50);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(3);
                hanSoloParamManager_[i]->setParStep(0.001);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Sigma                         ");
                hanSoloParamManager_[i]->setParLimits(0,100);
                hanSoloParamManager_[i]->setParValue (10);
                hanSoloParamManager_[i]->setParLimInf(1);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(1);
                hanSoloParamManager_[i]->setParStep(0.1);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParName  ("Scale Factor                  ");
                hanSoloParamManager_[i]->setParLimits(0, 1000000);
                hanSoloParamManager_[i]->setParValue (400);
                hanSoloParamManager_[i]->setParLimInf(100);
                hanSoloParamManager_[i]->setParLimSup(10000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("Center of distribution        ");
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(-100);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(1);
                hanSoloParamManager_[i]->setParStep(0.1);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("Vertical translation Constant ");
                hanSoloParamManager_[i]->setParLimits(0, 1000000);
                hanSoloParamManager_[i]->setParValue (100);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(1000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Eta Reproduction")
    {
        for (int i = 0; i < 11; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                     ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                hanSoloParamManager_[i]->setParValue (6.5);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(30.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                hanSoloParamManager_[i]->setParValue (0.0007);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor                   ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("Cluster 1 extension           ");
                hanSoloParamManager_[i]->setParValue (40);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(50);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            if (i == 5)
            {
                hanSoloParamManager_[i]->setParName  ("Sigma Gauss (side)            ");
                hanSoloParamManager_[i]->setParLimits(-1000, 1000);
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(-10);
                hanSoloParamManager_[i]->setParLimSup(10);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            if (i == 6)
            {
                hanSoloParamManager_[i]->setParName  ("Constant factor (side)        ");
                hanSoloParamManager_[i]->setParLimits(0, 10000);
                hanSoloParamManager_[i]->setParValue (1000);
                hanSoloParamManager_[i]->setParLimInf(100);
                hanSoloParamManager_[i]->setParLimSup(10000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(10);
            }
            if (i == 7)
            {
                hanSoloParamManager_[i]->setParName  ("Sigma Gauss (center)          ");
                hanSoloParamManager_[i]->setParValue (10);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(75);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            if (i == 8)
            {
                hanSoloParamManager_[i]->setParName  ("Scale planeID (center)          ");
                hanSoloParamManager_[i]->setParLimits(0, 10000);
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(100);
                hanSoloParamManager_[i]->setParLimSup(1000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            if (i == 9)
            {
                hanSoloParamManager_[i]->setParName  ("Horizontal Translation        ");
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(-100);
                hanSoloParamManager_[i]->setParLimSup(100);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(1);
                hanSoloParamManager_[i]->setParStep(0.1);
            }
            if (i == 10)
            {
                hanSoloParamManager_[i]->setParName  ("Vertical Translation          ");
                hanSoloParamManager_[i]->setParValue (100);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );
        }
    }
    else if (item->text() == "Eta Distribution")
    {
        for (int i = 0; i < 6; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness                     ");
                hanSoloParamManager_[i]->setParValue (500);
                hanSoloParamManager_[i]->setParLimInf(500);
                hanSoloParamManager_[i]->setParLimSup(500);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(1);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Thickness/(mu*AppliedVoltage)");
                hanSoloParamManager_[i]->setParValue (6.5);
                hanSoloParamManager_[i]->setParLimInf(0.1);
                hanSoloParamManager_[i]->setParLimSup(30.);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 2)
            {
                hanSoloParamManager_[i]->setParPrecision(5);
                hanSoloParamManager_[i]->setParStep(0.00001);
                hanSoloParamManager_[i]->setParName  ("4*DiffusionConstant           ");
                hanSoloParamManager_[i]->setParValue (0.0007);
                hanSoloParamManager_[i]->setParLimInf(0.00001);
                hanSoloParamManager_[i]->setParLimSup(0.3);
                hanSoloParamManager_[i]->setParFixed (false);
            }
            else if (i == 3)
            {
                hanSoloParamManager_[i]->setParName  ("ScaleFactor for Charge profile");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }
            else if (i == 4)
            {
                hanSoloParamManager_[i]->setParName  ("Tan(theta)                    ");
                hanSoloParamManager_[i]->setParLimits(-100, 100);
                hanSoloParamManager_[i]->setParValue (0);
                hanSoloParamManager_[i]->setParLimInf(-1);
                hanSoloParamManager_[i]->setParLimSup(1);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(4);
                hanSoloParamManager_[i]->setParStep(0.0001);
            }
            else if (i == 5)
            {
                hanSoloParamManager_[i]->setParName  ("Global ScaleFactor            ");
                hanSoloParamManager_[i]->setParValue (1.);
                hanSoloParamManager_[i]->setParLimInf(1.);
                hanSoloParamManager_[i]->setParLimSup(1.);
                hanSoloParamManager_[i]->setParFixed (true);
                hanSoloParamManager_[i]->setParPrecision(2);
                hanSoloParamManager_[i]->setParStep(0.01);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    else if (item->text() == "Linear")
    {
        for (int i = 0; i < 2; ++i)
        {
            hanSoloParamManager_.push_back(new FitParamManagerWidget(ui_->parametersTW));

            if (i == 0)
            {
                hanSoloParamManager_[i]->setParName  ("Intercept");
                hanSoloParamManager_[i]->setParLimits(-100000, 100000);
                hanSoloParamManager_[i]->setParValue (-3000);
                hanSoloParamManager_[i]->setParLimInf(-100000);
                hanSoloParamManager_[i]->setParLimSup(100000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(10);
            }
            else if (i == 1)
            {
                hanSoloParamManager_[i]->setParName  ("Slope");
                hanSoloParamManager_[i]->setParLimits(-10, 100000);
                hanSoloParamManager_[i]->setParValue (5);
                hanSoloParamManager_[i]->setParLimInf(0);
                hanSoloParamManager_[i]->setParLimSup(30000);
                hanSoloParamManager_[i]->setParFixed (false);
                hanSoloParamManager_[i]->setParPrecision(0);
                hanSoloParamManager_[i]->setParStep(10);
            }

            connect(hanSoloParamManager_[i]->getParFixed(),
                    SIGNAL(clicked() ),
                    hanSoloParamManager_[i],
                    SLOT(on_fixPar_clicked())
                    );

        }
    }

    for (unsigned int j = 0; j < hanSoloParamManager_.size(); ++j)
    {
        ss.str("");
        ss << "Par " << j;
        ui_->parametersTW->insertTab(j, hanSoloParamManager_[j], (QString)ss.str().c_str());
    }
}
//=================================================================================
std::string HanSoloFitter::getObjectType (TObject * obj)
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

//=================================================================================
void HanSoloFitter::on_fitLimitCB_clicked(bool checked)
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
        if(hanSoloTreeBrowser_->getCurrentObject())
        {
            ui_->fitRangeMinSB->setRange(((TH1F*)(hanSoloTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmin(), ((TH1F*)(hanSoloTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmax());
            ui_->fitRangeMinSB->setValue(((TH1F*)(hanSoloTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmin());
            ui_->fitRangeMaxSB->setRange(((TH1F*)(hanSoloTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmin(), ((TH1F*)(hanSoloTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmax());
            ui_->fitRangeMaxSB->setValue(((TH1F*)(hanSoloTreeBrowser_->getCurrentObject()))->GetXaxis()->GetXmax());
        }
    }
}
//===========================================================================
//========================= Fit Manager [END] ===============================
//===========================================================================

//=================================================================================
//======================== On-Screen Control [End] ================================
//=================================================================================
//=================================================================================
//=================================================================================
