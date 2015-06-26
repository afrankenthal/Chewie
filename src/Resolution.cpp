#include "Resolution.h"
#include "AnalysisManager.h"
#include "WindowsManager.h"
#include "Window.h"
#include "ThreadUtilities.h"
#include "PlanesMapping.h"
#include "MessageTools.h"
#include "XmlParser.h"
#include "XmlAnalysis.h"
#include "HistogramWindow.h"
#include "Utilities.h"

#include <TH1F.h>
#include <TF1.h>
#include <TThread.h>
#include <TFile.h>
#include <TTreeFormula.h>
#include <TMath.h>
#include <TVirtualFitter.h>

#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Resolution::Resolution(AnalysisManager* analysisManager, int nOfThreads) :
    Analysis          (analysisManager,
                       nOfThreads      )
  , thePlaneMapping_  (0               )
  , theWindowsManager_(0               )
{
    thePlaneMapping_ = new PlanesMapping();
    fSmearedSquare_ = new TF1("fSmearedSquare",Utilities::smearedSquare,-100,100,4);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Resolution::~Resolution(void)
{
    if(thePlaneMapping_)
        delete thePlaneMapping_;

    destroy();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::destroy(void)
{
    if(Analysis::fDoNotDelete_) return;

    for(std::vector<TH1F*>::iterator it=hXresiduals_              .begin(); it!=hXresiduals_              .end(); it++) delete *it; hXresiduals_              .clear();
    for(std::vector<TH1F*>::iterator it=hXresidualCalculated_     .begin(); it!=hXresidualCalculated_     .end(); it++) delete *it; hXresidualCalculated_     .clear();
    for(std::vector<TH2F*>::iterator it=hX2DresidualCalculated_   .begin(); it!=hX2DresidualCalculated_   .end(); it++) delete *it; hX2DresidualCalculated_   .clear();
    for(std::vector<TH1F*>::iterator it=hXresidualsClusterSize1_  .begin(); it!=hXresidualsClusterSize1_  .end(); it++) delete *it; hXresidualsClusterSize1_  .clear();
    for(std::vector<TH2F*>::iterator it=hX2DresidualsClusterSize1_.begin(); it!=hX2DresidualsClusterSize1_.end(); it++) delete *it; hX2DresidualsClusterSize1_.clear();
    for(std::vector<TH1F*>::iterator it=hXresidualsClusterSize2_  .begin(); it!=hXresidualsClusterSize2_  .end(); it++) delete *it; hXresidualsClusterSize2_  .clear();
    for(std::vector<TH1F*>::iterator it=hXresidualsDigital_       .begin(); it!=hXresidualsDigital_       .end(); it++) delete *it; hXresidualsDigital_       .clear();
    for(std::vector<TH2F*>::iterator it=hX2DresidualsDigital_     .begin(); it!=hX2DresidualsDigital_     .end(); it++) delete *it; hX2DresidualsDigital_     .clear();

    for(std::vector<TH1F*>::iterator it=hYresiduals_              .begin(); it!=hYresiduals_              .end(); it++) delete *it; hYresiduals_              .clear();
    for(std::vector<TH1F*>::iterator it=hYresidualCalculated_     .begin(); it!=hYresidualCalculated_     .end(); it++) delete *it; hYresidualCalculated_     .clear();
    for(std::vector<TH2F*>::iterator it=hY2DresidualCalculated_   .begin(); it!=hY2DresidualCalculated_   .end(); it++) delete *it; hY2DresidualCalculated_   .clear();
    for(std::vector<TH1F*>::iterator it=hYresidualsClusterSize1_  .begin(); it!=hYresidualsClusterSize1_  .end(); it++) delete *it; hYresidualsClusterSize1_  .clear();
    for(std::vector<TH2F*>::iterator it=hY2DresidualsClusterSize1_.begin(); it!=hY2DresidualsClusterSize1_.end(); it++) delete *it; hY2DresidualsClusterSize1_.clear();
    for(std::vector<TH1F*>::iterator it=hYresidualsClusterSize2_  .begin(); it!=hYresidualsClusterSize2_  .end(); it++) delete *it; hYresidualsClusterSize2_  .clear();
    for(std::vector<TH1F*>::iterator it=hYresidualsDigital_       .begin(); it!=hYresidualsDigital_       .end(); it++) delete *it; hYresidualsDigital_       .clear();
    for(std::vector<TH2F*>::iterator it=hY2DresidualsDigital_     .begin(); it!=hY2DresidualsDigital_     .end(); it++) delete *it; hY2DresidualsDigital_     .clear();

    for(std::map<std::string,TH1F*>::iterator it=hPredictedXErrors_      .begin(); it!=hPredictedXErrors_      .end(); it++) delete it->second; hPredictedXErrors_      .clear();
    for(std::map<std::string,TH1F*>::iterator it=hPredictedYErrors_      .begin(); it!=hPredictedYErrors_      .end(); it++) delete it->second; hPredictedYErrors_      .clear();

    for(std::vector<TH2F*>::iterator it=h2DCorrelationsResidualXvsX_.begin(); it!=h2DCorrelationsResidualXvsX_.end(); it++) delete *it; h2DCorrelationsResidualXvsX_.clear();
    for(std::vector<TH2F*>::iterator it=h2DCorrelationsResidualXvsY_.begin(); it!=h2DCorrelationsResidualXvsY_.end(); it++) delete *it; h2DCorrelationsResidualXvsY_.clear();
    for(std::vector<TH2F*>::iterator it=h2DCorrelationsResidualYvsY_.begin(); it!=h2DCorrelationsResidualYvsY_.end(); it++) delete *it; h2DCorrelationsResidualYvsY_.clear();
    for(std::vector<TH2F*>::iterator it=h2DCorrelationsResidualYvsX_.begin(); it!=h2DCorrelationsResidualYvsX_.end(); it++) delete *it; h2DCorrelationsResidualYvsX_.clear();
    for(std::vector<TH1F*>::iterator it=hCorrelationsResidualXvsX_  .begin(); it!=hCorrelationsResidualXvsX_  .end(); it++) delete *it; hCorrelationsResidualXvsX_  .clear();
    for(std::vector<TH1F*>::iterator it=hCorrelationsResidualXvsY_  .begin(); it!=hCorrelationsResidualXvsY_  .end(); it++) delete *it; hCorrelationsResidualXvsY_  .clear();
    for(std::vector<TH1F*>::iterator it=hCorrelationsResidualYvsY_  .begin(); it!=hCorrelationsResidualYvsY_  .end(); it++) delete *it; hCorrelationsResidualYvsY_  .clear();
    for(std::vector<TH1F*>::iterator it=hCorrelationsResidualYvsX_  .begin(); it!=hCorrelationsResidualYvsX_  .end(); it++) delete *it; hCorrelationsResidualYvsX_  .clear();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::predictedErrors(bool pass, int planeID, const Data& data, int threadNumber)
{
    if(!pass)
        return;
    std::string planeName = thePlaneMapping_->getPlaneName(planeID);
    THREADED(hPredictedXErrors_[planeName])->Fill(data.getXErrorPredictedGlobal(planeID));
    THREADED(hPredictedYErrors_[planeName])->Fill(data.getYErrorPredictedGlobal(planeID));
}
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Resolution::smearedSquare(double *xx, double *par){
    //smearedSquare: square function convoluted with a gaussian
    //par[0] = normalization
    //par[1] = center of the function, look for misalignment
    //par[2] = sigma of the gaussian, contains telescope and DUT resolutions
    //par[3] = width of the square function, shuold be the width within no charge sharing occurs

    double step=0.03125;//1/2^5
    double func=0;

    for(double y=par[1]-par[3]/2.; y<=par[1]+par[3]/2.; y+=step){
      func += par[0]*1/TMath::Sqrt(2*TMath::Pi()*par[2])*TMath::Exp(-(xx[0]-y)*(xx[0]-y)/(par[2]*par[2]))*step;
    }

    return func;

}*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsSize1(int planeID){

    if(hXresidualsClusterSize1_[planeID]->GetEntries()==0)
        return;

    TVirtualFitter::SetMaxIterations(100);

    double center = hXresidualsClusterSize1_[planeID]->GetMean();
    double norm   = hXresidualsClusterSize1_[planeID]->GetBinContent(hXresidualsClusterSize1_[planeID]->GetMaximumBin());
    double sigma  = 16.;  //gaussian sigma
    double width  = 140.; //square width

    fSmearedSquare_->SetRange(-10*sigma+center-width,10*sigma+center+width);

    fSmearedSquare_->SetParameters(norm,center,sigma,width);

    fSmearedSquare_->SetParName(0,"Constant");
    fSmearedSquare_->SetParName(1,"Mean");
    fSmearedSquare_->SetParName(2,"Sigma");
    fSmearedSquare_->SetParName(3,"Width");

    fSmearedSquare_->SetParLimits(2,10.,20.);
    fSmearedSquare_->SetParLimits(3,50.,160.);

    hXresidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,150.);

    fSmearedSquare_->SetParameter(0,fSmearedSquare_->GetParameter(0)); //norm
    fSmearedSquare_->FixParameter(1,fSmearedSquare_->GetParameter(1)); //center
    fSmearedSquare_->SetParameter(2,fSmearedSquare_->GetParameter(2)); //sigma
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3)); //width

    hXresidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,center);

    fSmearedSquare_->FixParameter(0,fSmearedSquare_->GetParameter(0));
    fSmearedSquare_->ReleaseParameter(1);
    fSmearedSquare_->SetParameter(1,fSmearedSquare_->GetParameter(1));
    fSmearedSquare_->FixParameter(2,fSmearedSquare_->GetParameter(2));
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3));

    hXresidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,150.);

    fSmearedSquare_->ReleaseParameter(0);
    fSmearedSquare_->SetParameter(0,fSmearedSquare_->GetParameter(0));
    fSmearedSquare_->ReleaseParameter(1);
    fSmearedSquare_->SetParameter(1,fSmearedSquare_->GetParameter(1));
    fSmearedSquare_->ReleaseParameter(2);
    fSmearedSquare_->SetParameter(2,fSmearedSquare_->GetParameter(2));
    fSmearedSquare_->ReleaseParameter(3);
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3));

    hXresidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"QM","",-150.,150.);

    TVirtualFitter::SetMaxIterations(5000);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsSize1(int planeID){

    if(hYresidualsClusterSize1_[planeID]->GetEntries()==0)
        return;

    TVirtualFitter::SetMaxIterations(100);

    double center = hYresidualsClusterSize1_[planeID]->GetMean();
    double norm   = hYresidualsClusterSize1_[planeID]->GetBinContent(hYresidualsClusterSize1_[planeID]->GetMaximumBin());
    double sigma  = 16.;  //gaussian sigma
    double width  = 140.; //square width

    fSmearedSquare_->SetRange(-10*sigma+center-width,10*sigma+center+width);

    fSmearedSquare_->SetParameters(norm,center,sigma,width);

    fSmearedSquare_->SetParName(0,"Constant");
    fSmearedSquare_->SetParName(1,"Mean");
    fSmearedSquare_->SetParName(2,"Sigma");
    fSmearedSquare_->SetParName(3,"Width");

    fSmearedSquare_->SetParLimits(2,10.,20.);
    fSmearedSquare_->SetParLimits(3,50.,160.);

    hYresidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,150.);

    fSmearedSquare_->SetParameter(0,fSmearedSquare_->GetParameter(0)); //norm
    fSmearedSquare_->FixParameter(1,fSmearedSquare_->GetParameter(1)); //center
    fSmearedSquare_->SetParameter(2,fSmearedSquare_->GetParameter(2)); //sigma
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3)); //width

    hYresidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,center);

    fSmearedSquare_->FixParameter(0,fSmearedSquare_->GetParameter(0));
    fSmearedSquare_->ReleaseParameter(1);
    fSmearedSquare_->SetParameter(1,fSmearedSquare_->GetParameter(1));
    fSmearedSquare_->FixParameter(2,fSmearedSquare_->GetParameter(2));
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3));

    hYresidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,150.);

    fSmearedSquare_->ReleaseParameter(0);
    fSmearedSquare_->SetParameter(0,fSmearedSquare_->GetParameter(0));
    fSmearedSquare_->ReleaseParameter(1);
    fSmearedSquare_->SetParameter(1,fSmearedSquare_->GetParameter(1));
    fSmearedSquare_->ReleaseParameter(2);
    fSmearedSquare_->SetParameter(2,fSmearedSquare_->GetParameter(2));
    fSmearedSquare_->ReleaseParameter(3);
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3));

    hYresidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"QM","",-150.,150.);

    TVirtualFitter::SetMaxIterations(5000);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsCalculated(int planeID){

    if(hXresidualCalculated_[planeID]->GetEntries()==0)
        return;

    hXresidualCalculated_[planeID]->Fit("gaus","Q","",-150.,150.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsDigital(int planeID){

    if(hXresidualsDigital_[planeID]->GetEntries()==0)
        return;

    hXresidualsDigital_[planeID]->Fit("gaus","Q","",-150.,150.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsCalculated(int planeID){

    if(hYresidualCalculated_[planeID]->GetEntries()==0)
        return;

    hYresidualCalculated_[planeID]->Fit("gaus","Q","",-150.,150.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsDigital(int planeID){

    if(hYresidualsDigital_[planeID]->GetEntries()==0)
        return;

    hYresidualsDigital_[planeID]->Fit("gaus","Q","",-150.,150.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitResolution(int planeID)
{

    if(thePlaneMapping_->getPlaneName(planeID).find("Dut")==std::string::npos)
        return;

    std::cout<<__PRETTY_FUNCTION__<<" Fitting residuals for plane "<<thePlaneMapping_->getPlaneName(planeID)<<std::endl;

    fitXresidualsSize1(planeID);

    fitXresidualsCalculated(planeID);

    fitXresidualsDigital(planeID);

    fitYresidualsSize1(planeID);

    fitYresidualsCalculated(planeID);

    fitYresidualsDigital(planeID);

    hCorrelationsResidualXvsX_[planeID]->Fit("pol1","Q");

    hCorrelationsResidualXvsY_[planeID]->Fit("pol1","Q");

    hCorrelationsResidualYvsY_[planeID]->Fit("pol1","Q");

    hCorrelationsResidualYvsX_[planeID]->Fit("pol1","Q");

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::calculateXresiduals(bool pass, int planeID, const Data &data, int threadNumber)
{
    if( !pass || !data.getIsInDetector( planeID ) || !data.getHasHit( planeID ) || data.getClusterSize( planeID ) != 2 )
        return;

    float maxPitchX = 150;

    if( data.getXPitchLocal( planeID ) > maxPitchX )
        return;

    float Xp = 0;

    if(data.getXPixelResidualLocal( planeID ) > 0)
        Xp = -data.getXPitchLocal( planeID )/2 + data.getXPixelResidualLocal( planeID );
    else if(data.getXPixelResidualLocal( planeID ) <= 0)
        Xp = (data.getXPixelResidualLocal( planeID ) + data.getXPitchLocal( planeID )/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted   ( planeID)     ;
    int           col       = data.getColPredicted   ( planeID )    ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    for(int h=0; h<2; ++h)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) //hits are in the window
            || !data.getIsPixelCalibrated(h,planeID)                                                          //pixels are calibrated
            ||  data.getClusterPixelRow  (h,planeID) != row )                                                 //hits are on the same row (sharing is along the row - x direction)
            return;
    }

    std::string planeName;
    std::string toGet;

    planeName = thePlaneMapping_->getPlaneName(planeID);

    int   hitID       =     -1 ;
    int   totalCharge =      0 ;
    int   chargeLeft  =      0 ;
    int   chargeRight =      0 ;
    float Asimmetry   =      0 ;
    float slope       =      0 ;
    float intercept   =      0 ;

    toGet = "Charge/" + planeName +  "/Xasimmetry/h1DXcellChargeAsimmetryInv_" + planeName;
    if((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()) ){
        if((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("lineX")!=NULL){
            slope     = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("lineX"))->GetParameter(1);
            intercept = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("lineX"))->GetParameter(0);
        }
    }

    //float slope       = -13.65 ;
    //float intercept   =  1.385 ;
    //float slope       = -5.181 ;
    //float intercept   =  1.228 ;
    float Xm                     ;
    //float Xall = data.getXPredictedLocal(planeID);
    //float Yall = data.getYPredictedLocal(planeID);

       for(int h=0; h<2; ++h)
       {
        if(data.getClusterPixelCol(h,planeID) == col)//mi assicuro che ci sia la cella predetta in uno dei due hit
        {
            hitID   = h   ;
            break;
        }
    }
    if( hitID == -1 )
        return;

    for(int h=0; h<2; ++h)
    {
        if(data.getXPixelResidualLocal(planeID) > 0 && (col - data.getClusterPixelCol(h,planeID)) == -1)//il secondo hit e' a DX della predetta
        {
            chargeRight = data.getClusterPixelCharge(h    ,planeID);
            chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
            break;
        }
        else if(data.getXPixelResidualLocal(planeID) <= 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)//il secondo hit e' a SX della predetta
        {
            chargeRight = data.getClusterPixelCharge(hitID,planeID);
            chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
            break;
        }
        //else
        //    return;
        else if(data.getXPixelResidualLocal(planeID) > 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)
        {
            chargeRight = data.getClusterPixelCharge(hitID,planeID);
            chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
            Xp =  data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
            break;
        }
        else if(data.getXPixelResidualLocal(planeID) < 0 && (col - data.getClusterPixelCol(h,planeID)) ==  -1)
        {
            chargeRight = data.getClusterPixelCharge(h    ,planeID);
            chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
            Xp = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);
            break;
        }
    }

    totalCharge = chargeLeft + chargeRight;
    Asimmetry   = (float)(chargeLeft - chargeRight)/totalCharge;

    if(Asimmetry >= -1. && Asimmetry <= 1.)// && totalCharge <= 30000)
    {
        Xm = slope*Asimmetry + intercept;
        THREADED(hXresidualCalculated_ [planeID])->Fill( Xm - Xp );
        THREADED(hX2DresidualCalculated_ [planeID])->Fill( Xp, Xm - Xp );
        //THREADED(h2DCorrelationsResidualXvsX_[planeID])->Fill(Xall,Xm - Xp);
        //THREADED(h2DCorrelationsResidualXvsY_[planeID])->Fill(Yall,Xm - Xp);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::calculateYresiduals(bool pass, int planeID, const Data &data, int threadNumber)
{
    if( !pass || !data.getIsInDetector( planeID ) || !data.getHasHit( planeID ) || data.getClusterSize( planeID ) != 2 )
        return;

    float maxPitchY = 100;

    if( data.getYPitchLocal( planeID ) > maxPitchY )
        return;

    float Yp = 0;

    if(data.getYPixelResidualLocal( planeID ) > 0)
        Yp = -data.getYPitchLocal( planeID )/2 + data.getYPixelResidualLocal( planeID );
    else if(data.getYPixelResidualLocal( planeID ) <= 0)
        Yp = (data.getYPixelResidualLocal( planeID ) + data.getYPitchLocal( planeID )/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted   ( planeID );
    int           col       = data.getColPredicted   ( planeID );
    int           run       = data.getRunNumber()               ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    for(int h=0; h<2; ++h)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // hits are in the window
            || !data.getIsPixelCalibrated(h,planeID)                                                          //pixels are calibrated
            ||  data.getClusterPixelCol    (h,planeID) !=  col )                                              //hits are on the same column (sharing is along the column - y direction)
            return;
    }

    std::string planeName;
    std::string toGet;

    planeName = thePlaneMapping_->getPlaneName(planeID);

    int   hitID       =      -1 ;
    int   totalCharge =       0 ;
    int   chargeLeft  =       0 ;
    int   chargeRight =       0 ;
    float Asimmetry   =       0 ;
    float slope       =       0 ;
    float intercept   =       0 ;

    toGet = "Charge/" + planeName +  "/Yasimmetry/h1DYcellChargeAsimmetryInv_" + planeName;

    if((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()) ){
        if((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("lineY")!=NULL){
           slope     = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("lineY"))->GetParameter(1);
            intercept = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("lineY"))->GetParameter(0);
        }
    }

    //float slope       =  -12.13 ;
    //float intercept   = -0.5596 ;
    //float slope       =  -5.14  ;
    //float intercept   = -0.8479 ;
    float Ym                      ;
    //float Xall = data.getXPredictedLocal(planeID);
    //float Yall = data.getYPredictedLocal(planeID);

    for(int h=0; h<2; ++h)
    {
        if(data.getClusterPixelRow(h,planeID) == row)//mi assicuro che ci sia la cella predetta in uno dei due hit
        {
            hitID   = h   ;
            break;
        }
    }
    if( hitID == -1)
        return;

    for(int h=0; h<2; ++h)
    {
        if(data.getYPixelResidualLocal(planeID) > 0 && (row - data.getClusterPixelRow(h,planeID)) == -1)//il secondo hit e' a DX della predetta
        {
            chargeRight = data.getClusterPixelCharge(h    ,planeID);
            chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
            break;
        }
        else if(data.getYPixelResidualLocal(planeID) <= 0 && (row - data.getClusterPixelRow(h,planeID)) == 1)//il secondo hit e' a SX della predetta
        {
            chargeRight = data.getClusterPixelCharge(hitID,planeID);
            chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
            break;
        }
        //else
        //    return;
        else if(data.getYPixelResidualLocal(planeID) > 0 && (row - data.getClusterPixelRow(h,planeID)) == 1)
        {
            chargeRight = data.getClusterPixelCharge(hitID,planeID);
            chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
            Yp =  data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
            break;
        }
        else if(data.getYPixelResidualLocal(planeID) < 0 && (row - data.getClusterPixelRow(h,planeID)) ==  -1)
        {
            chargeRight = data.getClusterPixelCharge(h    ,planeID);
            chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
            Yp = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);
            break;
        }
    }

    totalCharge = chargeLeft + chargeRight;
    Asimmetry  = (float)(chargeLeft - chargeRight)/totalCharge;

    if(Asimmetry >= -1. && Asimmetry <= 1.)// && totalCharge <= 30000)
    {
        Ym = slope*Asimmetry + intercept;
        THREADED(hYresidualCalculated_ [planeID])->Fill( Ym - Yp );
        THREADED(hY2DresidualCalculated_ [planeID])->Fill( Yp , Ym - Yp );
        //THREADED(h2DCorrelationsResidualYvsX_[planeID])->Fill(Xall,Ym - Yp);
        //THREADED(h2DCorrelationsResidualYvsY_[planeID])->Fill(Yall,Ym - Yp);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::xResolution(bool pass, int planeID, const Data& data, int threadNumber)
{
    if(!pass || !data.getBelongsToTrack(planeID))
        return;

    int   size    = data.getClusterSize(planeID);
    int   maxHits = 2;
    float Xp;

    if(size > maxHits)
        return;

    const Window* theWindow  = theWindowsManager_->getWindow(planeID);

    for(int h=0; h<size; h++)
    {
        if( !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber()) ) //i due hit devono stare nella finestra
            return;
    }

    THREADED(hXresiduals_[planeID])->Fill(data.getXTrackResidualLocal(planeID));

    if( theWindow->checkTimeWindowAbout(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber()) ){
        // &&theWindow->checkWindowAbout    (data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber()) ){
        THREADED(h2DCorrelationsResidualXvsX_[planeID])->Fill(data.getXPredictedLocal(planeID),data.getXTrackResidualLocal(planeID));
        THREADED(h2DCorrelationsResidualXvsY_[planeID])->Fill(data.getYPredictedLocal(planeID),data.getXTrackResidualLocal(planeID));
    }

    if (size == 2 && data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID) && data.getClusterCharge(planeID) < 20000)
    {
        double xRes = data.getXPredictedLocal(planeID);
        /*if(data.getXPixelResidualLocal(planeID) > 0)
            xRes = data.getXPitchLocal(planeID)/2 - data.getXPixelResidualLocal(planeID);
        else if(data.getXPixelResidualLocal(planeID) <= 0)
            xRes = -(data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);
*/
        double xHit;
        if (data.getClusterPixelCol(0,planeID) > data.getClusterPixelCol(1,planeID))
        {
            //if (data.getClusterPixelCharge(0, planeID) > data.getClusterPixelCharge(1, planeID))
                xHit = data.getXPitchLocal(planeID)*(data.getClusterPixelCol(0,planeID)+1);
           // else
             //   yHit = -data.getYPitchLocal(planeID)/2;
        }
        else
        {
            //if (data.getClusterPixelCharge(0, planeID) > data.getClusterPixelCharge(1, planeID))
                xHit = data.getXPitchLocal(planeID)*(data.getClusterPixelCol(1,planeID)+1);
           // else
             //   yHit = data.getYPitchLocal(planeID)/2;
        }

        THREADED(hXresidualsDigital_[planeID])->Fill(xHit-xRes);

        if(data.getXPixelResidualLocal( planeID ) > 0)
            Xp = -data.getXPitchLocal( planeID )/2 + data.getXPixelResidualLocal( planeID );
        else if(data.getXPixelResidualLocal( planeID ) <= 0)
            Xp = (data.getXPixelResidualLocal( planeID ) + data.getXPitchLocal( planeID )/2);

        THREADED(hX2DresidualsDigital_[planeID])->Fill(xHit-xRes,Xp);

    }



    if(size == 1){
        THREADED(hXresidualsClusterSize1_  [planeID])->Fill(data.getXTrackResidualLocal(planeID));
        THREADED(hX2DresidualsClusterSize1_[planeID])->Fill(data.getXTrackResidualLocal(planeID),data.getXPixelResidualLocal(planeID));
    }
    if(size == 2)
    {
        if(data.getClusterPixelRow(0,planeID) != data.getClusterPixelRow(1,planeID))
            return;

        THREADED(hXresidualsClusterSize2_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::yResolution(bool pass, int planeID, const Data& data, int threadNumber)
{
     if(!pass || !data.getBelongsToTrack(planeID))
        return;

    int   size    = data.getClusterSize(planeID);
    int   maxHits = 2;
    float Yp;

    if(size > maxHits)
        return;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);

    for(int h=0; h<size; h++)
    {
        if( !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber()) ) //i due hit devono stare nella finestra
            return;
    }

    THREADED(hYresiduals_[planeID])->Fill(data.getYTrackResidualLocal(planeID));

    if( theWindow->checkTimeWindowAbout(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber())
        && theWindow->checkWindowAbout    (data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber(),data.getDataType(planeID)) ){
        THREADED(h2DCorrelationsResidualYvsX_[planeID])->Fill(data.getXPredictedLocal(planeID),data.getYTrackResidualLocal(planeID));
        THREADED(h2DCorrelationsResidualYvsY_[planeID])->Fill(data.getYPredictedLocal(planeID),data.getYTrackResidualLocal(planeID));
    }

    if (size == 2 && data.getClusterPixelRow(0,planeID) != data.getClusterPixelRow(1,planeID) && data.getClusterCharge(planeID) < 20000)
    {
        double yRes = data.getYPredictedLocal(planeID);
        /*if(data.getYPixelResidualLocal(planeID) > 0)
            yRes = data.getYPitchLocal(planeID)/2 - data.getYPixelResidualLocal(planeID);
        else if(data.getYPixelResidualLocal(planeID) <= 0)
            yRes = -(data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);
*/
        double yHit;
        if (data.getClusterPixelRow(0,planeID) > data.getClusterPixelRow(1,planeID))
        {
            //if (data.getClusterPixelCharge(0, planeID) > data.getClusterPixelCharge(1, planeID))
                yHit = data.getYPitchLocal(planeID)*data.getClusterPixelRow(0,planeID);
           // else
             //   yHit = -data.getYPitchLocal(planeID)/2;
        }
        else
        {
            //if (data.getClusterPixelCharge(0, planeID) > data.getClusterPixelCharge(1, planeID))
                yHit = data.getYPitchLocal(planeID)*data.getClusterPixelRow(1,planeID);
           // else
             //   yHit = data.getYPitchLocal(planeID)/2;
        }

        THREADED(hYresidualsDigital_[planeID])->Fill(yHit-yRes);

        if(data.getYPixelResidualLocal( planeID ) > 0)
            Yp = -data.getYPitchLocal( planeID )/2 + data.getYPixelResidualLocal( planeID );
        else if(data.getYPixelResidualLocal( planeID ) <= 0)
            Yp = (data.getYPixelResidualLocal( planeID ) + data.getYPitchLocal( planeID )/2);

        THREADED(hY2DresidualsDigital_[planeID])->Fill(yHit-yRes,Yp);

    }

    if(size == 1){
        THREADED(hYresidualsClusterSize1_  [planeID])->Fill(data.getYTrackResidualLocal(planeID));
        THREADED(hY2DresidualsClusterSize1_[planeID])->Fill(data.getYTrackResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
   }
    if(size == 2 && data.getClusterCharge(planeID) < 20000)
    {
        if(data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID))
            return;

        THREADED(hYresidualsClusterSize2_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::setCutsFormula(std::map<std::string,std::string> cutsList, std::vector<TTree*> tree)
{
    std::vector<TTreeFormula*> formulasVector;

    for(std::map<std::string,std::string>::iterator it=cutsList.begin(); it!=cutsList.end(); it++)
    {
        if((it->first) == "main cut" && (it->second).size()==0)
            STDLINE("WARNING: no main cut set in resolution analysis!! Default value = true!", ACRed);

        formulasVector.clear();
        if((it->second).size()!=0)
        {
            for(unsigned int t=0; t<tree.size(); t++)
            {
                formulasVector.push_back(new TTreeFormula((it->second).c_str(),(it->second).c_str(),tree[t]));
            }
            cutsFormulas_[it->first] = formulasVector;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Resolution::passStandardCuts(int planeID, const Data &data)
{
    XmlParser* theParser = theAnalysisManager_->getXmlParser();
    if(!theParser->getAnalysesFromString("Charge")->applyStandardCuts()) {
        return true;
    }

    if (theParser->getAnalysesFromString("Charge")->excludeBadPlanes())
        return passBadPlanesCut(planeID, data);

    int minHits = 7;//To calculate efficiency on the telescope
    int excludeMe = 0;
    if(thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos)//Dut case
        minHits = atoi(theParser->getAnalysesFromString("Charge")->getMinHits().c_str());
    else if(data.getHasHit(planeID) && data.getClusterSize(planeID)<=2)//Telescope case
        excludeMe = 1;

    int hitsontelescope = 0;
    bool HitsClusterLE2 = true;
    for (unsigned int i = 0; i < 8; i++) {
        if (data.getHasHit(i) == true && thePlaneMapping_->getPlaneName(i).find("Dut") == std::string::npos) hitsontelescope++;
        if (data.getClusterSize(i) > 2) {
            HitsClusterLE2 = false;
        }
    }

    if(data.getNumberOfTelescopeHits() >= minHits)
        return true;
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Resolution::passBadPlanesCut (int planeID, const Data &data)
{
    XmlParser* theParser = theAnalysisManager_->getXmlParser();
//    bool badPlanes = theParser->getAnalysesFromString("Charge")->excludeBadPlanes();
    int badPlanesCut = theParser->getAnalysesFromString("Charge")->getBadPlanesCut();


    int maxNumberOfEvents = 0;
    for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes() -2; ++p)// -2 is to exclude DUTs
    {
        HistogramWindow * aWindow = (HistogramWindow*)theAnalysisManager_->getWindowsManager()->getWindow(p);
        if (aWindow->getNumberOfEvents() > maxNumberOfEvents)
        {
            maxNumberOfEvents = aWindow->getNumberOfEvents();
        }
    }

    int minHits = 7;//To calculate efficiency on the telescope
    int excludeMe = 0;
    if(thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) //Dut case
        minHits = atoi(theParser->getAnalysesFromString("Charge")->getMinHits().c_str());
    else if(data.getHasHit(planeID))//Telescope case
    {
        if(data.getClusterSize(planeID) == 1)
            excludeMe = 1;
        else if(data.getClusterSize(planeID) == 2
                && (data.getClusterPixelRow(0,planeID) == data.getClusterPixelRow(1,planeID)
                    || data.getClusterPixelCol(0,planeID) == data.getClusterPixelCol(1,planeID)))
            excludeMe = 1;
    }

    if (badPlanesCut == 50) badPlanesCut = 60;

    for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes() -2; ++p) // -2 is to exclude DUTs
    {
        HistogramWindow * aWindow = (HistogramWindow*)theAnalysisManager_->getWindowsManager()->getWindow(p);
        if (!data.getHasHit(p) && (float)aWindow->getNumberOfEvents() < (float)maxNumberOfEvents*badPlanesCut/100)
            excludeMe += 1;
    }

    if(data.getNumberOfTelescopeHits()-excludeMe >= minHits) {
        return true;
    }
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::beginJob(void)
{
    theWindowsManager_ = theAnalysisManager_->getWindowsManager();
    book();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::analyze(const Data& data, int threadNumber)//WARNING: You can't change this name (threadNumber) or the MACRO THREAD won't compile
{   
    bool mainCut;
    if(cutsFormulas_.find("main cut")!=cutsFormulas_.end())
        mainCut = cutsFormulas_["main cut"][threadNumber]->EvalInstance();
    else
        mainCut = true;

    if(!mainCut)
        return;

    bool errorsCut;
    if(cutsFormulas_.find("errors")!=cutsFormulas_.end())
        errorsCut = cutsFormulas_["errors"][threadNumber]->EvalInstance();
    else
        errorsCut = mainCut;

    bool xResolutionCut;
    if(cutsFormulas_.find("X resolution")!=cutsFormulas_.end())
        xResolutionCut = cutsFormulas_["X resolution"][threadNumber]->EvalInstance();
    else
        xResolutionCut = mainCut;

    bool yResolutionCut;
    if(cutsFormulas_.find("Y resolution")!=cutsFormulas_.end())
        yResolutionCut = cutsFormulas_["Y resolution"][threadNumber]->EvalInstance();
    else
        yResolutionCut = mainCut;

    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        if(!passStandardCuts(p,data))
            continue;
        \
        calculateXresiduals(xResolutionCut,p,data,threadNumber);
        calculateYresiduals(yResolutionCut,p,data,threadNumber);
        xResolution(xResolutionCut,p,data,threadNumber);
        yResolution(yResolutionCut,p,data,threadNumber);

       if(thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos)
           predictedErrors(errorsCut,p,data,threadNumber);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::endJob(void)
{
    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        ADD_THREADED(hXresiduals_                [p]);
        ADD_THREADED(hXresidualCalculated_       [p]);
        ADD_THREADED(hX2DresidualCalculated_     [p]);
        ADD_THREADED(hXresidualsClusterSize1_    [p]);
        ADD_THREADED(hX2DresidualsClusterSize1_  [p]);
        ADD_THREADED(hXresidualsClusterSize2_    [p]);
        ADD_THREADED(hXresidualsDigital_         [p]);
        ADD_THREADED(hX2DresidualsDigital_       [p]);
        ADD_THREADED(hYresiduals_                [p]);
        ADD_THREADED(hYresidualCalculated_       [p]);
        ADD_THREADED(hY2DresidualCalculated_     [p]);
        ADD_THREADED(hYresidualsClusterSize1_    [p]);
        ADD_THREADED(hY2DresidualsClusterSize1_  [p]);
        ADD_THREADED(hYresidualsClusterSize2_    [p]);
        ADD_THREADED(hYresidualsDigital_         [p]);
        ADD_THREADED(hY2DresidualsDigital_       [p]);
        ADD_THREADED(h2DCorrelationsResidualXvsX_[p]);
        ADD_THREADED(h2DCorrelationsResidualXvsY_[p]);
        ADD_THREADED(h2DCorrelationsResidualYvsY_[p]);
        ADD_THREADED(h2DCorrelationsResidualYvsX_[p]);
        ADD_THREADED(hCorrelationsResidualXvsX_  [p]);


        hXresiduals_                [p]->GetXaxis()->SetTitle("x residual (um)" );
        hXresidualCalculated_       [p]->GetXaxis()->SetTitle("x residual (um)" );
        hX2DresidualCalculated_     [p]->GetXaxis()->SetTitle("long pitch (um)" );
        hX2DresidualCalculated_     [p]->GetYaxis()->SetTitle("x residual (um)" );
        hXresidualsClusterSize1_    [p]->GetXaxis()->SetTitle("x residual (um)" );
        hX2DresidualsClusterSize1_  [p]->GetXaxis()->SetTitle("long pitch (um)" );
        hX2DresidualsClusterSize1_  [p]->GetYaxis()->SetTitle("x residual (um)" );
        hXresidualsClusterSize2_    [p]->GetXaxis()->SetTitle("x residual (um)" );
        hXresidualsDigital_         [p]->GetXaxis()->SetTitle("x residual (um)" );
        hX2DresidualsDigital_       [p]->GetXaxis()->SetTitle("long pitch (um)" );
        hX2DresidualsDigital_       [p]->GetYaxis()->SetTitle("x residual (um)" );
        hYresiduals_                [p]->GetXaxis()->SetTitle("y residual (um)" );
        hYresidualCalculated_       [p]->GetXaxis()->SetTitle("y residual (um)" );
        hY2DresidualCalculated_     [p]->GetXaxis()->SetTitle("short pitch (um)");
        hY2DresidualCalculated_     [p]->GetYaxis()->SetTitle("y residual (um)" );
        hYresidualsClusterSize1_    [p]->GetXaxis()->SetTitle("y residual (um)" );
        hY2DresidualsClusterSize1_  [p]->GetXaxis()->SetTitle("short pitch (um)");
        hY2DresidualsClusterSize1_  [p]->GetYaxis()->SetTitle("y residual (um)" );
        hYresidualsClusterSize2_    [p]->GetXaxis()->SetTitle("y residual (um)" );
        hYresidualsDigital_         [p]->GetXaxis()->SetTitle("y residual (um)" );
        hY2DresidualsDigital_       [p]->GetXaxis()->SetTitle("short pitch (um)");
        hY2DresidualsDigital_       [p]->GetYaxis()->SetTitle("y residual (um)" );
        h2DCorrelationsResidualXvsX_[p]->GetXaxis()->SetTitle("x (um)"          );
        h2DCorrelationsResidualXvsX_[p]->GetYaxis()->SetTitle("x residual (um)" );
        h2DCorrelationsResidualXvsY_[p]->GetXaxis()->SetTitle("y (um)"          );
        h2DCorrelationsResidualXvsY_[p]->GetYaxis()->SetTitle("x residual (um)" );
        h2DCorrelationsResidualYvsY_[p]->GetXaxis()->SetTitle("y (um)"          );
        h2DCorrelationsResidualYvsY_[p]->GetYaxis()->SetTitle("y residual (um)" );
        h2DCorrelationsResidualYvsX_[p]->GetXaxis()->SetTitle("x (um)"          );
        h2DCorrelationsResidualYvsX_[p]->GetYaxis()->SetTitle("y residual (um)" );

        if(thePlaneMapping_->getPlaneName(p).find("Dut")!=std::string::npos)
        {
            ADD_THREADED(hPredictedXErrors_[thePlaneMapping_->getPlaneName(p)]);
            ADD_THREADED(hPredictedYErrors_[thePlaneMapping_->getPlaneName(p)]);

            hPredictedXErrors_[thePlaneMapping_->getPlaneName(p)]->GetXaxis()->SetTitle("x error (um)");
            hPredictedYErrors_[thePlaneMapping_->getPlaneName(p)]->GetXaxis()->SetTitle("y error (um)");
        }

        std::string hName;
        std::string hTitle;
        std::string planeName = thePlaneMapping_->getPlaneName(p);

        theAnalysisManager_->cd("Resolution/" + planeName + "/Correlations");

        hName  = "hCorrelationsResidualXvsX_"                          + planeName;
        hTitle = "Correlation X residuals vs X "                        + planeName;
        //hCorrelationsResidualXvsX_.push_back(0x0);

        //customProfileX(h2DCorrelationsResidualXvsX_,hCorrelationsResidualXvsX_,p,hName.c_str(),hTitle.c_str(),1,-1,0);
        Utilities::customProfileX(h2DCorrelationsResidualXvsX_[p],hCorrelationsResidualXvsX_[p],hName.c_str(),hTitle.c_str(),1,-1,0);
        //Utilities::customProfileX(h2DCorrelationsResidualXvsX_[p],hCorrelationsResidualXvsX_,hName.c_str(),hTitle.c_str(),1,-1,0);
        //hCorrelationsResidualXvsX_.push_back((TH1F*)h2DCorrelationsResidualXvsX_[p]->ProfileX(hName.c_str(),1,-1));
        //hCorrelationsResidualXvsX_[p]->SetTitle(hTitle.c_str());

        hName  = "hCorrelationsResidualXvsY_"                          + planeName;
        hTitle = "Correlation X residuals vs Y "                        + planeName;
        //Utilities::customProfileX(h2DCorrelationsResidualXvsY_[p],hCorrelationsResidualXvsY_,hName.c_str(),hTitle.c_str(),1,-1,0);
        //hCorrelationsResidualXvsY_.push_back(NEW_THREADED(TH1F("dummy","dummy",1,0,1)));
        //Utilities::customProfileX(h2DCorrelationsResidualXvsY_[p],hCorrelationsResidualXvsY_[p],hName.c_str(),hTitle.c_str(),1,-1,0);
        hCorrelationsResidualXvsY_.push_back((TH1F*)h2DCorrelationsResidualXvsX_[p]->ProfileX(hName.c_str(),1,-1));
        hCorrelationsResidualXvsY_[p]->SetTitle(hTitle.c_str());

        hName  = "hCorrelationsResidualYvsY_"                          + planeName;
        hTitle = "Correlation Y residuals vs Y "                        + planeName;
        //Utilities::customProfileX(h2DCorrelationsResidualYvsY_[p],hCorrelationsResidualYvsY_,hName.c_str(),hTitle.c_str(),1,-1,0);
        //hCorrelationsResidualYvsY_.push_back(NEW_THREADED(TH1F("dummy","dummy",1,0,1)));
        //Utilities::customProfileX(h2DCorrelationsResidualYvsY_[p],hCorrelationsResidualYvsY_[p],hName.c_str(),hTitle.c_str(),1,-1,0);
        hCorrelationsResidualYvsY_.push_back((TH1F*)h2DCorrelationsResidualYvsY_[p]->ProfileX(hName.c_str(),1,-1));
        hCorrelationsResidualYvsY_[p]->SetTitle(hTitle.c_str());

        hName  = "hCorrelationsResidualYvsX_"                          + planeName;
        hTitle = "Correlation Y residuals vs X "                        + planeName;
        //Utilities::customProfileX(h2DCorrelationsResidualYvsX_[p],hCorrelationsResidualYvsX_,hName.c_str(),hTitle.c_str(),1,-1,0);
        //hCorrelationsResidualYvsX_.push_back(NEW_THREADED(TH1F("dummy","dummy",1,0,1)));
        //Utilities::customProfileX(h2DCorrelationsResidualYvsX_[p],hCorrelationsResidualYvsX_[p],hName.c_str(),hTitle.c_str(),1,-1,0);
        hCorrelationsResidualYvsX_.push_back((TH1F*)h2DCorrelationsResidualYvsX_[p]->ProfileX(hName.c_str(),1,-1));
        hCorrelationsResidualYvsX_[p]->SetTitle(hTitle.c_str());

        hCorrelationsResidualXvsX_  [p]->GetXaxis()->SetTitle("x (um)"          );
        hCorrelationsResidualXvsX_  [p]->GetYaxis()->SetTitle("x residual (um)" );
        hCorrelationsResidualXvsX_  [p]->GetYaxis()->SetRangeUser(-20.,20.);
        hCorrelationsResidualXvsY_  [p]->GetXaxis()->SetTitle("y (um)"          );
        hCorrelationsResidualXvsY_  [p]->GetYaxis()->SetTitle("x residual (um)" );
        hCorrelationsResidualXvsY_  [p]->GetYaxis()->SetRangeUser(-20.,20.);
        hCorrelationsResidualYvsY_  [p]->GetXaxis()->SetTitle("y (um)"          );
        hCorrelationsResidualYvsY_  [p]->GetYaxis()->SetTitle("y residual (um)" );
        hCorrelationsResidualYvsY_  [p]->GetYaxis()->SetRangeUser(-20.,20.);
        hCorrelationsResidualYvsX_  [p]->GetXaxis()->SetTitle("x (um)"          );
        hCorrelationsResidualYvsX_  [p]->GetYaxis()->SetTitle("y residual (um)" );
        hCorrelationsResidualYvsX_  [p]->GetYaxis()->SetRangeUser(-20.,20.);

        if(p == 9)
        {
            std::stringstream ss;
            STDLINE("Single hits residuals:", ACRed);
            ss.str(""); ss << "X Residuals: " << hXresidualsClusterSize1_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            ss.str(""); ss << "Y Residuals: " << hYresidualsClusterSize1_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            STDLINE("",ACWhite);

            STDLINE("Double hits residuals:", ACRed);
            ss.str(""); ss << "X Residuals: " << hXresidualsClusterSize2_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            ss.str(""); ss << "Y Residuals: " << hYresidualsClusterSize2_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            STDLINE("",ACWhite);

            STDLINE("Single and double hits residuals:", ACRed);
            ss.str(""); ss << "X Residuals: " << hXresiduals_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            ss.str(""); ss << "Y Residuals: " << hYresiduals_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            STDLINE("",ACWhite);
        }
        if(theAnalysisManager_->getXmlParser()->getAnalysesFromString("Resolution")->doFits())
        {
            fitResolution(p);
        }

    }


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::book(void)
{
    destroy();

    std::string hName;
    std::string hTitle;
    std::string planeName;
    float             resXRange       =   150;
    float             resYRange       =   100;
    float             binSize         =     5;
    int               numberOfRows    =    80;
    int               numberOfColumns =    52;

    theAnalysisManager_->cd("/");
    theAnalysisManager_->mkdir("Resolution");

    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        planeName = thePlaneMapping_->getPlaneName(p);
        theAnalysisManager_->cd("Resolution");
        theAnalysisManager_->mkdir(planeName);

        theAnalysisManager_->mkdir("X Residuals");

        hName  = "hXresiduals_"                               + planeName;
        hTitle = "X residuals  "                              + planeName;
        hXresiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hXresidualCalculated_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit " + planeName;
        hXresidualCalculated_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));

        hName  = "hX2DresidualCalculated_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit vs X " + planeName;
        hX2DresidualCalculated_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize, -resXRange/2., resXRange/2., 125, -500, 500)));

        hName  = "hXresidualsClusterSize1_"                   + planeName;
        hTitle = "X residuals cluster size 1 "                + planeName;
        hXresidualsClusterSize1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hX2DresidualClusterSize1_"                      + planeName;
        hTitle = "X residuals cluster size 1 vs X " + planeName;
        hX2DresidualsClusterSize1_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize, -resXRange/2., resXRange/2., 125, -500, 500)));

        hName  = "hXresidualsClusterSize2_"                   + planeName;
        hTitle = "X residuals cluster size 2 "                + planeName;
        hXresidualsClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hXresidualsDigital_"                        + planeName;
        hTitle = "X residuals for digitalized hits "          + planeName;
        hXresidualsDigital_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hX2DresidualsDigital_"                        + planeName;
        hTitle = "X residuals for digitalized hits vs X"          + planeName;
        hX2DresidualsDigital_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), (int)resXRange/binSize, -resXRange/2., resXRange/2., 125, -500, 500)));

        theAnalysisManager_->cd("Resolution/" + planeName);

        theAnalysisManager_->mkdir("Y Residuals");

        hName  = "hYresiduals_"                               + planeName;
        hTitle = "Y residuals "                               + planeName;
        hYresiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hYresidualCalculated_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit " + planeName;
        hYresidualCalculated_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));

        hName  = "hY2DresidualCalculated_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit vs Y " + planeName;
        hY2DresidualCalculated_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/binSize, -resYRange/2., resYRange/2., 125, -500, 500)));

        hName  = "hYresidualsClusterSize1_"                   + planeName;
        hTitle = "Y residuals cluster size 1 "                + planeName;
        hYresidualsClusterSize1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hY2DresidualClusterSize1_"                      + planeName;
        hTitle = "Y residuals cluster size 1 vs Y " + planeName;
        hY2DresidualsClusterSize1_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/binSize, -resYRange/2., resYRange/2., 125, -500, 500)));

        hName  = "hYresidualsClusterSize2_"                   + planeName;
        hTitle = "Y residuals cluster size 2 "                + planeName;
        hYresidualsClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hYresidualsDigital_"                        + planeName;
        hTitle = "Y residuals for digitalized hits "          + planeName;
        hYresidualsDigital_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hY2DresidualsDigital_"                        + planeName;
        hTitle = "Y residuals for digitalized hits vs Y"          + planeName;
        hY2DresidualsDigital_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), (int)resYRange/binSize, -resYRange/2., resYRange/2., 125, -500, 500)));

        if(planeName.find("Dut")!=std::string::npos)
        {
            theAnalysisManager_->cd("Resolution/" + planeName);

            theAnalysisManager_->mkdir("Errors");

            hName  = "hPredictedXErrors_"                     + planeName;
            hTitle = "predicted X errors "                    + planeName;
            hPredictedXErrors_[planeName] = NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 10000, 0, 10));

            hName  = "hPredictedYErrors_"                     + planeName;
            hTitle = "predicted Y errors "                    + planeName;
            hPredictedYErrors_[planeName] = NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 10000, 0, 10));
        }

        theAnalysisManager_->cd("Resolution/" + planeName);
        theAnalysisManager_->mkdir("Correlations");

        hName  = "h2DCorrelationsResidualXvsX_"                        + planeName;
        hTitle = "Correlation X residuals vs X "                        + planeName;
        h2DCorrelationsResidualXvsX_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfColumns+2, 0., (float)resXRange*(numberOfColumns+2), 300, -300., 300.)));//, 150, -75., 75.)));

        hName  = "h2DCorrelationsResidualXvsY_"                        + planeName;
        hTitle = "Correlation X residuals vs Y "                        + planeName;
        h2DCorrelationsResidualXvsY_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfRows   +1, 0., (float)resYRange*(numberOfRows   +1), 300, -300., 300.)));//, 100, -50., 50.)));

        hName  = "h2DCorrelationsResidualYvsY_"                        + planeName;
        hTitle = "Correlation Y residuals vs Y "                        + planeName;
        h2DCorrelationsResidualYvsY_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfRows   +1, 0., (float)resYRange*(numberOfRows   +1), 300, -300., 300.)));//, 100, -50., 50.)));

        hName  = "h2DCorrelationsResidualYvsX_"                        + planeName;
        hTitle = "Correlation Y residuals vs X "                        + planeName;
        h2DCorrelationsResidualYvsX_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfColumns+2, 0., (float)resXRange*(numberOfColumns+2), 300, -300., 300.)));//, 150, -75., 75.)));

        hName  = "hCorrelationsResidualXvsX_"                          + planeName;
        hTitle = "Correlation X residuals vs X "                        + planeName;
        hCorrelationsResidualXvsX_.push_back(NEW_THREADED( TH1F(hName.c_str(),hTitle.c_str(), numberOfColumns+2, 0., (float)resXRange*(numberOfColumns+2)) ));

    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void Resolution::customProfileX(std::vector<TH2F *>h2D, std::vector<TH1F *>prof, int planeID, const char* outputName, const char* outputTitle, int yBinMin, int yBinMax, int minNumberOfEntries){

    TH2F * h2DHist = h2D[planeID];
    if(yBinMax<=0) yBinMax = h2DHist->GetNbinsY();
    if(yBinMin<=0) yBinMin = 1;

    int nXBin = h2DHist->GetNbinsX();

    TH1F * profile = prof.back();
    profile->SetNameTitle(outputName,outputTitle);
    profile->SetBins(nXBin,h2DHist->GetBinLowEdge(1),h2DHist->GetBinWidth(1)*nXBin+h2DHist->GetBinLowEdge(1));
    //prof.push_back(NEW_THREADED( TH1F(outputName,outputTitle,nXBin,h2DHist->GetBinLowEdge(1),h2DHist->GetBinWidth(1)*nXBin+h2DHist->GetBinLowEdge(1)) ));


    for(int i=1; i<=nXBin; i++){

        float mean=0.;
        float mean_std;
        float error=0.;
        float sigma=0.;
        float weight=0.;
        int   entries=0.;

        for(int j=yBinMin; j<=yBinMax ; j++){

            float z      = h2DHist->GetBinContent(i,j);
            float center = h2DHist->GetYaxis()->GetBinCenter(j);
            if(z==0) continue;
            weight = 1./z;
            mean+=center*weight;
            mean_std += center*z;
            error+=weight;
            sigma+=center*center*z;

            entries+= z;

        }

        if(entries<=minNumberOfEntries) continue;

        mean=mean/error;
        mean_std=mean_std/entries;
        error=TMath::Sqrt(1./error);
        sigma = TMath::Sqrt(sigma - mean_std*mean_std)/entries;

        profile->SetBinContent(i,mean_std);
        profile->SetBinError(i,sigma);

    }

    return;

}
*/
