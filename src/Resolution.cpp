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

#include "Resolution.h"
#include "AnalysisManager.h"
#include "WindowsManager.h"
#include "Window.h"
#include "ThreadUtilities.h"
#include "PlanesMapping.h"
#include "MessageTools.h"
#include "XmlParser.h"
#include "XmlAnalysis.h"
#include "XmlPlane.h"
#include "XmlWindow.h"
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
  , theXmlParser_     (analysisManager->getXmlParser())

{
    thePlaneMapping_ = new PlanesMapping();
    fSmearedSquare_ = new TF1("fSmearedSquare",Utilities::smearedSquare,-100,100,4);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Resolution::~Resolution(void)
{
    STDLINE("=======================================================================",ACCyan) ;
    if(thePlaneMapping_)
        delete thePlaneMapping_;

    destroy();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::destroy(void)
{
    STDLINE("=======================================================================",ACCyan) ;
    if(Analysis::fDoNotDelete_) return;

    for(std::vector<TH1F*>::iterator it=hXResiduals_                .begin(); it!=hXResiduals_                .end(); it++) delete *it; hXResiduals_                .clear();
    for(std::vector<TH1F*>::iterator it=hXResidualCalculated_       .begin(); it!=hXResidualCalculated_       .end(); it++) delete *it; hXResidualCalculated_       .clear();
    for(std::vector<TH1F*>::iterator it=hXResidualCalculatedSize2_  .begin(); it!=hXResidualCalculatedSize2_  .end(); it++) delete *it; hXResidualCalculatedSize2_  .clear();
    for(std::vector<TH2F*>::iterator it=hX2DResidualCalculatedSize2_.begin(); it!=hX2DResidualCalculatedSize2_.end(); it++) delete *it; hX2DResidualCalculatedSize2_.clear();
    for(std::vector<TH1F*>::iterator it=hXResidualCalculatedSize3_  .begin(); it!=hXResidualCalculatedSize3_  .end(); it++) delete *it; hXResidualCalculatedSize3_  .clear();
    for(std::vector<TH2F*>::iterator it=hX2DResidualCalculatedSize3_.begin(); it!=hX2DResidualCalculatedSize3_.end(); it++) delete *it; hX2DResidualCalculatedSize3_.clear();
    for(std::vector<TH1F*>::iterator it=hXResidualCalculatedSize4_  .begin(); it!=hXResidualCalculatedSize4_  .end(); it++) delete *it; hXResidualCalculatedSize4_  .clear();
    for(std::vector<TH2F*>::iterator it=hX2DResidualCalculatedSize4_.begin(); it!=hX2DResidualCalculatedSize4_.end(); it++) delete *it; hX2DResidualCalculatedSize4_.clear();
    for(std::vector<TH1F*>::iterator it=hXResidualsClusterSize1_    .begin(); it!=hXResidualsClusterSize1_    .end(); it++) delete *it; hXResidualsClusterSize1_    .clear();
    for(std::vector<TH2F*>::iterator it=hX2DResidualsClusterSize1_  .begin(); it!=hX2DResidualsClusterSize1_  .end(); it++) delete *it; hX2DResidualsClusterSize1_  .clear();
    for(std::vector<TH1F*>::iterator it=hXResidualsClusterSize2_    .begin(); it!=hXResidualsClusterSize2_    .end(); it++) delete *it; hXResidualsClusterSize2_    .clear();
    for(std::vector<TH1F*>::iterator it=hXResidualsClusterSize3_    .begin(); it!=hXResidualsClusterSize3_    .end(); it++) delete *it; hXResidualsClusterSize3_    .clear();
    for(std::vector<TH1F*>::iterator it=hXResidualsClusterSize4_    .begin(); it!=hXResidualsClusterSize4_    .end(); it++) delete *it; hXResidualsClusterSize4_    .clear();
    for(std::vector<TH1F*>::iterator it=hXResidualsDigital_         .begin(); it!=hXResidualsDigital_         .end(); it++) delete *it; hXResidualsDigital_         .clear();
    for(std::vector<TH2F*>::iterator it=hX2DResidualsDigital_       .begin(); it!=hX2DResidualsDigital_       .end(); it++) delete *it; hX2DResidualsDigital_       .clear();\

    for(std::vector<TH1F*>::iterator it=hYResiduals_                         .begin(); it!=hYResiduals_                         .end(); it++) delete *it; hYResiduals_                         .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculated_                .begin(); it!=hYResidualCalculated_                .end(); it++) delete *it; hYResidualCalculated_                .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize2_           .begin(); it!=hYResidualCalculatedSize2_           .end(); it++) delete *it; hYResidualCalculatedSize2_           .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize2Row1of4Rows_.begin(); it!=hYResidualCalculatedSize2Row1of4Rows_.end(); it++) delete *it; hYResidualCalculatedSize2Row1of4Rows_.clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize2Row2of4Rows_.begin(); it!=hYResidualCalculatedSize2Row2of4Rows_.end(); it++) delete *it; hYResidualCalculatedSize2Row2of4Rows_.clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize2Row3of4Rows_.begin(); it!=hYResidualCalculatedSize2Row3of4Rows_.end(); it++) delete *it; hYResidualCalculatedSize2Row3of4Rows_.clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize2Row4of4Rows_.begin(); it!=hYResidualCalculatedSize2Row4of4Rows_.end(); it++) delete *it; hYResidualCalculatedSize2Row4of4Rows_.clear();
    for(std::vector<TH1F*>::iterator it=hLandauChargeDownSize2Row1of4Rows_   .begin(); it!=hLandauChargeDownSize2Row1of4Rows_   .end(); it++) delete *it; hLandauChargeDownSize2Row1of4Rows_   .clear();
    for(std::vector<TH1F*>::iterator it=hLandauChargeUpSize2Row2of4Rows_     .begin(); it!=hLandauChargeUpSize2Row2of4Rows_     .end(); it++) delete *it; hLandauChargeUpSize2Row2of4Rows_     .clear();
    for(std::vector<TH1F*>::iterator it=hLandauChargeDownSize2Row3of4Rows_   .begin(); it!=hLandauChargeDownSize2Row3of4Rows_   .end(); it++) delete *it; hLandauChargeDownSize2Row3of4Rows_   .clear();
    for(std::vector<TH1F*>::iterator it=hLandauChargeUpSize2Row4of4Rows_     .begin(); it!=hLandauChargeUpSize2Row4of4Rows_     .end(); it++) delete *it; hLandauChargeUpSize2Row4of4Rows_     .clear();
    for(std::vector<TH2F*>::iterator it=hY2DResidualCalculatedSize2_         .begin(); it!=hY2DResidualCalculatedSize2_         .end(); it++) delete *it; hY2DResidualCalculatedSize2_         .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize3_           .begin(); it!=hYResidualCalculatedSize3_           .end(); it++) delete *it; hYResidualCalculatedSize3_           .clear();
    for(std::vector<TH2F*>::iterator it=hY2DResidualCalculatedSize3_         .begin(); it!=hY2DResidualCalculatedSize3_         .end(); it++) delete *it; hY2DResidualCalculatedSize3_         .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize4_           .begin(); it!=hYResidualCalculatedSize4_           .end(); it++) delete *it; hYResidualCalculatedSize4_           .clear();
    for(std::vector<TH2F*>::iterator it=hY2DResidualCalculatedSize4_         .begin(); it!=hY2DResidualCalculatedSize4_         .end(); it++) delete *it; hY2DResidualCalculatedSize4_         .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualsClusterSize1_             .begin(); it!=hYResidualsClusterSize1_             .end(); it++) delete *it; hYResidualsClusterSize1_             .clear();
    for(std::vector<TH2F*>::iterator it=hY2DResidualsClusterSize1_           .begin(); it!=hY2DResidualsClusterSize1_           .end(); it++) delete *it; hY2DResidualsClusterSize1_           .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualsClusterSize2_             .begin(); it!=hYResidualsClusterSize2_             .end(); it++) delete *it; hYResidualsClusterSize2_             .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualsClusterSize3_             .begin(); it!=hYResidualsClusterSize3_             .end(); it++) delete *it; hYResidualsClusterSize3_             .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualsClusterSize4_             .begin(); it!=hYResidualsClusterSize4_             .end(); it++) delete *it; hYResidualsClusterSize4_             .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualsDigital_                  .begin(); it!=hYResidualsDigital_                  .end(); it++) delete *it; hYResidualsDigital_                  .clear();
    for(std::vector<TH2F*>::iterator it=hY2DResidualsDigital_                .begin(); it!=hY2DResidualsDigital_                .end(); it++) delete *it; hY2DResidualsDigital_                .clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize2Row1of2Rows_.begin(); it!=hYResidualCalculatedSize2Row1of2Rows_.end(); it++) delete *it; hYResidualCalculatedSize2Row1of2Rows_.clear();
    for(std::vector<TH1F*>::iterator it=hYResidualCalculatedSize2Row2of2Rows_.begin(); it!=hYResidualCalculatedSize2Row2of2Rows_.end(); it++) delete *it; hYResidualCalculatedSize2Row2of2Rows_.clear();

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
void Resolution::fitXresidualsSize1(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hXResidualsClusterSize1_[planeID]->GetEntries()==0)
        return;

    TVirtualFitter::SetMaxIterations(100);

    double center = hXResidualsClusterSize1_[planeID]->GetMean();
    double norm   = hXResidualsClusterSize1_[planeID]->GetBinContent(hXResidualsClusterSize1_[planeID]->GetMaximumBin());
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
    STDLINE("=======================================================================",ACRed) ;

    hXResidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,150.);//Irene changed it, before it was +- 150
    //hXresidualsClusterSize14Rows_[planeID]->Fit(fSmearedSquare_,"Q","",-600.,600.);

    fSmearedSquare_->SetParameter(0,fSmearedSquare_->GetParameter(0)); //norm
    fSmearedSquare_->FixParameter(1,fSmearedSquare_->GetParameter(1)); //center
    fSmearedSquare_->SetParameter(2,fSmearedSquare_->GetParameter(2)); //sigma
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3)); //width

    hXResidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,center);

    fSmearedSquare_->FixParameter(0,fSmearedSquare_->GetParameter(0));
    fSmearedSquare_->ReleaseParameter(1);
    fSmearedSquare_->SetParameter(1,fSmearedSquare_->GetParameter(1));
    fSmearedSquare_->FixParameter(2,fSmearedSquare_->GetParameter(2));
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3));

    hXResidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,150.);

    fSmearedSquare_->ReleaseParameter(0);
    fSmearedSquare_->SetParameter(0,fSmearedSquare_->GetParameter(0));
    fSmearedSquare_->ReleaseParameter(1);
    fSmearedSquare_->SetParameter(1,fSmearedSquare_->GetParameter(1));
    fSmearedSquare_->ReleaseParameter(2);
    fSmearedSquare_->SetParameter(2,fSmearedSquare_->GetParameter(2));
    fSmearedSquare_->ReleaseParameter(3);
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3));

    hXResidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"QM","",-150.,150.);

    TVirtualFitter::SetMaxIterations(5000);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsSize1(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hYResidualsClusterSize1_[planeID]->GetEntries()==0)
        return;

    TVirtualFitter::SetMaxIterations(100);

    double center = hYResidualsClusterSize1_[planeID]->GetMean();
    double norm   = hYResidualsClusterSize1_[planeID]->GetBinContent(hYResidualsClusterSize1_[planeID]->GetMaximumBin());
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

    hYResidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,150.);

    fSmearedSquare_->SetParameter(0,fSmearedSquare_->GetParameter(0)); //norm
    fSmearedSquare_->FixParameter(1,fSmearedSquare_->GetParameter(1)); //center
    fSmearedSquare_->SetParameter(2,fSmearedSquare_->GetParameter(2)); //sigma
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3)); //width

    hYResidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,center);

    fSmearedSquare_->FixParameter(0,fSmearedSquare_->GetParameter(0));
    fSmearedSquare_->ReleaseParameter(1);
    fSmearedSquare_->SetParameter(1,fSmearedSquare_->GetParameter(1));
    fSmearedSquare_->FixParameter(2,fSmearedSquare_->GetParameter(2));
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3));

    hYResidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"Q","",-150.,150.);

    fSmearedSquare_->ReleaseParameter(0);
    fSmearedSquare_->SetParameter(0,fSmearedSquare_->GetParameter(0));
    fSmearedSquare_->ReleaseParameter(1);
    fSmearedSquare_->SetParameter(1,fSmearedSquare_->GetParameter(1));
    fSmearedSquare_->ReleaseParameter(2);
    fSmearedSquare_->SetParameter(2,fSmearedSquare_->GetParameter(2));
    fSmearedSquare_->ReleaseParameter(3);
    fSmearedSquare_->SetParameter(3,fSmearedSquare_->GetParameter(3));

    hYResidualsClusterSize1_[planeID]->Fit(fSmearedSquare_,"QM","",-150.,150.);

    TVirtualFitter::SetMaxIterations(5000);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXResiduals(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hXResiduals_[planeID]->GetEntries()==0)
        return;

    hXResiduals_[planeID]->Fit("gaus","Q","",-150.,150.);


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYResiduals(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hYResiduals_[planeID]->GetEntries()==0)
        return;

    hYResiduals_[planeID]->Fit("gaus","Q","",-40.,40.);


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsCalculated(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hXResidualCalculated_[planeID]->GetEntries()==0)
        return;

    hXResidualCalculated_[planeID]->Fit("gaus","Q","",-150.,150.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsCalculatedSize2(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hXResidualCalculatedSize2_[planeID]->GetEntries()==0)
        return;

    hXResidualCalculatedSize2_[planeID]->Fit("gaus","Q","",-150.,150.);


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsCalculatedSize3(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hXResidualCalculatedSize3_[planeID]->GetEntries()==0)
        return;

    hXResidualCalculatedSize3_[planeID]->Fit("gaus","Q","",-150.,150.);


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsCalculatedSize4(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hXResidualCalculatedSize4_[planeID]->GetEntries()==0)
        return;

    hXResidualCalculatedSize4_[planeID]->Fit("gaus","Q","",-150.,150.);


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsSize2(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;
    if(hXResidualsClusterSize2_[planeID]->GetEntries()==0)
        return;

    hXResidualsClusterSize2_[planeID]->Fit("gaus","Q","",-150.,150.);


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsSize3(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hXResidualsClusterSize3_[planeID]->GetEntries()==0)
        return;

    hXResidualsClusterSize3_[planeID]->Fit("gaus","Q","",-200.,200.);


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsSize4(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hXResidualsClusterSize4_[planeID]->GetEntries()==0)
        return;

    hXResidualsClusterSize4_[planeID]->Fit("gaus","Q","",-200.,200.);


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsSize2(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;
    if(hYResidualsClusterSize2_[planeID]->GetEntries()==0)
        return;

    hYResidualsClusterSize2_[planeID]->Fit("gaus","Q","",-40.,40.);


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsSize3(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hYResidualsClusterSize3_[planeID]->GetEntries()==0)
        return;

    hYResidualsClusterSize3_[planeID]->Fit("gaus","Q","",-200.,200.);


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsSize4(int planeID)
{
    STDLINE("=======================================================================",ACCyan) ;

    if(hYResidualsClusterSize4_[planeID]->GetEntries()==0)
        return;

    hYResidualsClusterSize4_[planeID]->Fit("gaus","Q","",-200.,200.);


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitXresidualsDigital(int planeID)
{

    STDLINE("=======================================================================",ACCyan) ;
    if(hXResidualsDigital_[planeID]->GetEntries()==0)
        return;

    hXResidualsDigital_[planeID]->Fit("gaus","Q","",-150.,150.);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsCalculated(int planeID){

    if(hYResidualCalculated_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculated_[planeID]->Fit("gaus","Q","",-40.,40.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsCalculatedSize2(int planeID){

    if(hYResidualCalculatedSize2_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize2_[planeID]->Fit("gaus","Q","",-40.,40.);

    if(hYResidualCalculatedSize2Row1of2Rows_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize2Row1of2Rows_[planeID]->Fit("gaus","Q","",-40.,40.);

    if(hYResidualCalculatedSize2Row2of2Rows_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize2Row2of2Rows_[planeID]->Fit("gaus","Q","",-40.,40.);

    if(hYResidualCalculatedSize2Row1of4Rows_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize2Row1of4Rows_[planeID]->Fit("gaus","Q","",-40.,40.);

    if(hYResidualCalculatedSize2Row2of4Rows_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize2Row2of4Rows_[planeID]->Fit("gaus","Q","",-40.,40.);

    if(hYResidualCalculatedSize2Row3of4Rows_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize2Row3of4Rows_[planeID]->Fit("gaus","Q","",-40.,40.);

    if(hYResidualCalculatedSize2Row4of4Rows_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize2Row4of4Rows_[planeID]->Fit("gaus","Q","",-40.,40.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsCalculatedSize3(int planeID){

    if(hYResidualCalculatedSize3_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize3_[planeID]->Fit("gaus","Q","",-150.,150.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsCalculatedSize4(int planeID){

    if(hYResidualCalculatedSize4_[planeID]->GetEntries()==0)
        return;

    hYResidualCalculatedSize4_[planeID]->Fit("gaus","Q","",-150.,150.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitYresidualsDigital(int planeID){

    if(hYResidualsDigital_[planeID]->GetEntries()==0)
        return;

    hYResidualsDigital_[planeID]->Fit("gaus","Q","",-150.,150.);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::fitResolution(int planeID)
{

    if(thePlaneMapping_->getPlaneName(planeID).find("Dut")==std::string::npos)
        return;

    std::cout<<__PRETTY_FUNCTION__<<" Fitting residuals for plane "<<thePlaneMapping_->getPlaneName(planeID)<<std::endl;

    fitXResiduals(planeID);

    fitXresidualsSize1(planeID);

    fitXresidualsSize2(planeID);

    fitXresidualsSize3(planeID);

    fitXresidualsSize4(planeID);

    fitXresidualsCalculated(planeID);

    fitXresidualsCalculatedSize2(planeID);

    fitXresidualsCalculatedSize3(planeID);

    fitXresidualsCalculatedSize4(planeID);

    fitXresidualsDigital(planeID);

    fitYResiduals(planeID);

    fitYresidualsSize1(planeID);

    fitYresidualsSize2(planeID);

    fitYresidualsSize3(planeID);

    fitYresidualsSize4(planeID);

    fitYresidualsCalculated(planeID);

    fitYresidualsCalculatedSize2(planeID);

    fitYresidualsCalculatedSize3(planeID);

    fitYresidualsCalculatedSize4(planeID);

    fitYresidualsDigital(planeID);

    hCorrelationsResidualXvsX_[planeID]->Fit("pol1","Q");

    hCorrelationsResidualXvsY_[planeID]->Fit("pol1","Q");

    hCorrelationsResidualYvsY_[planeID]->Fit("pol1","Q");

    hCorrelationsResidualYvsX_[planeID]->Fit("pol1","Q");

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::calculateXresiduals(bool pass, int planeID, const Data &data, int threadNumber)
{
    if( !pass || !data.getIsInDetector( planeID ) || !data.getHasHit( planeID ) || data.getClusterSize( planeID ) > 4  )
        return;

    //track selection
    if(data.getNumberOfTelescopeClustersSizeLE2()!=8)
        return;

    //checking telescope
    int counter = 0;
    for(int p=8; p<16; p++)
    {
        if(data.getClusterSize(p)==2)
            counter++;
    }

    if(counter != 8)
        return;

    std::string planeName;
    std::string toGet;
    //    std::stringstream ss;

    planeName = thePlaneMapping_->getPlaneName(planeID);

    float maxPitchX = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());//Irene changed it. Before it was 150.

    if( data.getXPitchLocal( planeID ) > maxPitchX )
        return;

    float xPixelResidual = data.getXPixelResidualLocal(planeID);
    float xPixelEdgeResidual = 0;//Residual from the edge of the pixel (the divide between 2 pixels)


    if(xPixelResidual  > 0)
        xPixelEdgeResidual = -data.getXPitchLocal( planeID )/2 + xPixelResidual ;
    else if(xPixelResidual  <= 0)
        xPixelEdgeResidual = (xPixelResidual  + data.getXPitchLocal( planeID )/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted   ( planeID)     ;
    int           col       = data.getColPredicted   ( planeID )    ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);

    for(int h=0; h<size; h++)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) //hits are in the window
               || !data.getIsPixelCalibrated(h,planeID)                                                          //pixels are calibrated
               ||  data.getClusterPixelRow  (h,planeID) != row )                                                 //hits are on the same row (sharing is along the row - x direction)
            return;
    }

    //    std::string planeName;
    //    std::string toGet;

    //    planeName = thePlaneMapping_->getPlaneName(planeID);

    int   hitID       =     -1 ;
    int   totalCharge =      0 ;
    int   chargeLeft  =      0 ;
    int   chargeRight =      0 ;
    float asimmetry   =      0 ;

    for(int h=0; h<size; ++h)
    {
        if(data.getClusterPixelCol(h,planeID) == col)//mi assicuro che ci sia la cella predetta in uno dei due hit
        {
            hitID   = h   ;
            break;
        }
    }
    if( hitID == -1 )
        return;

    if(size == 3 || size ==4 )
    {
        int secondHigh = 0;
        int firstHigh  = 0;


        for(int h=0; h<size; ++h)
        {
            if(data.getClusterPixelCharge(h,planeID)>firstHigh)
            {
                secondHigh = firstHigh;
                firstHigh  = data.getClusterPixelCharge(h,planeID);
            }
            else if(data.getClusterPixelCharge(h,planeID)>secondHigh)
            {
                secondHigh = data.getClusterPixelCharge(h,planeID);
            }
            if(   (data.getClusterPixelCharge(h,planeID) != firstHigh || data.getClusterPixelCharge(h,planeID) != secondHigh)
                  && data.getClusterPixelCol(h,planeID)    == col )
                return;
            else if(data.getClusterPixelCharge(h,planeID) == firstHigh || data.getClusterPixelCharge(h,planeID) == secondHigh)
            {
                if(xPixelResidual  > 0 && (col - data.getClusterPixelCol(h,planeID)) == -1)//il secondo hit e' a DX della predetta
                {
                    chargeRight = data.getClusterPixelCharge(h    ,planeID);
                    chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
                    break;
                }
                else if(xPixelResidual  <= 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)//il secondo hit e' a SX della predetta
                {
                    chargeRight = data.getClusterPixelCharge(hitID,planeID);
                    chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
                    break;
                }
                //else
                //    return;
                else if(xPixelResidual  > 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)
                {
                    chargeRight = data.getClusterPixelCharge(hitID,planeID);
                    chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
                    xPixelEdgeResidual =  xPixelResidual  + data.getXPitchLocal(planeID)/2;
                    break;
                }
                else if(xPixelResidual  < 0 && (col - data.getClusterPixelCol(h,planeID)) ==  -1)
                {
                    chargeRight = data.getClusterPixelCharge(h    ,planeID);
                    chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
                    xPixelEdgeResidual = (xPixelResidual  - data.getXPitchLocal(planeID)/2);
                    break;
                }
            }

        }
    }

    if(size == 2)//it is if, not else if because there could be clusters of size 1 and we are not studying them in this way.
    {
        for(int h=0; h<size; ++h)
        {
            if(xPixelResidual  > 0 && (col - data.getClusterPixelCol(h,planeID)) == -1)//il secondo hit e' a DX della predetta
            {
                chargeRight = data.getClusterPixelCharge(h    ,planeID);
                chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
                break;
            }
            else if(xPixelResidual  <= 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)//il secondo hit e' a SX della predetta
            {
                chargeRight = data.getClusterPixelCharge(hitID,planeID);
                chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
                break;
            }
            //else
            //    return;
            else if(xPixelResidual  > 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)
            {
                chargeRight = data.getClusterPixelCharge(hitID,planeID);
                chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
                xPixelEdgeResidual =  xPixelResidual  + data.getXPitchLocal(planeID)/2;
                break;
            }
            else if(xPixelResidual  < 0 && (col - data.getClusterPixelCol(h,planeID)) ==  -1)
            {
                chargeRight = data.getClusterPixelCharge(h    ,planeID);
                chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
                xPixelEdgeResidual = (xPixelResidual  - data.getXPitchLocal(planeID)/2);
                break;
            }
        }
    }

    //    float slope       =  -25  ;
    //    float intercept   = -1.50223 ;

    totalCharge = chargeLeft + chargeRight;
    asimmetry   = (float)(chargeLeft - chargeRight)/totalCharge;


    float xMeasured;
    if(asimmetry >= -1. && asimmetry <= 1.)
    {
        //        xMeasured = slope*asimmetry + intercept;

        toGet = "Charge/" + planeName +  "/XAsimmetry/h1DXcellChargeAsimmetryInv_" + planeName;
        if((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()) ){
            if((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fXAsimmetryFit") != NULL){

                xMeasured = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fXAsimmetryFit"))->Eval(asimmetry);
            }
        }



        THREADED(hXResidualCalculated_   [planeID])->Fill( xMeasured - xPixelEdgeResidual );

        if(size == 2)
        {
            THREADED(hXResidualCalculatedSize2_   [planeID])->Fill( xMeasured - xPixelEdgeResidual );
            THREADED(hX2DResidualCalculatedSize2_ [planeID])->Fill( xPixelEdgeResidual, xMeasured - xPixelEdgeResidual );
            //THREADED(h2DCorrelationsResidualXvsX_[planeID])->Fill(Xall,xMeasured - xPixelEdgeResidual);
            //THREADED(h2DCorrelationsResidualXvsY_[planeID])->Fill(Yall,xMeasured - xPixelEdgeResidual);
        }
        else if(size == 3)
        {
            //            STDLINE("Filling Cluster Size 3 ",ACWhite);
            //            STDLINE("",ACWhite);
            THREADED(hXResidualCalculatedSize3_   [planeID])->Fill( xMeasured - xPixelEdgeResidual );
            THREADED(hX2DResidualCalculatedSize3_ [planeID])->Fill( xPixelEdgeResidual, xMeasured - xPixelEdgeResidual );
        }
        else if(size == 4)
        {
            THREADED(hXResidualCalculatedSize4_   [planeID])->Fill( xMeasured - xPixelEdgeResidual );
            THREADED(hX2DResidualCalculatedSize4_ [planeID])->Fill( xPixelEdgeResidual, xMeasured - xPixelEdgeResidual );
        }
    }

    //    ss.str(""); ss<<"Times excluded hitted pixel in cluster size 3: " << counter3;
    //    STDLINE(ss.str(),ACGreen);
    //    ss.str(""); ss<<"Times excluded hitted pixel in cluster size 4: " << counter4;
    //    STDLINE(ss.str(),ACGreen);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::calculateYresiduals(bool pass, int planeID, const Data &data, int threadNumber)
{
    if( !pass || !data.getIsInDetector( planeID ) || !data.getHasHit( planeID ) || data.getClusterSize( planeID ) > 4 )
        return;

    //track selection
    if(data.getNumberOfTelescopeClustersSizeLE2()!=8)
        return;

    //checking telescope
    int counter = 0;
    for(int p=8; p<16; p++)
    {
        if(data.getClusterSize(p)==2)
            counter++;
    }

    if(counter != 8)
        return;

    std::string planeName;
    std::string toGet;
    std::stringstream ss;

    planeName = thePlaneMapping_->getPlaneName(planeID);


    float yPitch = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());//Irene changed it. Before it was 100.

    if( data.getYPitchLocal( planeID ) > yPitch )
        return;

    float yPixelResidual = data.getYPixelResidualLocal(planeID);
    float yPixelEdgeResidual = 0; //Residual from the edge of the pixel (the divide between 2 pixels)


    if( yPixelResidual > 0 )
        yPixelEdgeResidual = -data.getYPitchLocal(planeID)/2 + yPixelResidual;
    else if( yPixelResidual <= 0 )
        yPixelEdgeResidual = yPixelResidual + data.getYPitchLocal(planeID)/2;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted   ( planeID );
    int           col       = data.getColPredicted   ( planeID );
    int           run       = data.getRunNumber()               ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);

    for(int h=0; h<size; ++h)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // hits are in the window
               || !data.getIsPixelCalibrated(h,planeID)                                                            //pixels are calibrated
               ||  data.getClusterPixelCol    (h,planeID) !=  col )                                            //hits are on the same column (sharing is along the column - y direction)
            return;
    }

    //    std::string planeName;
    //    std::string toGet;

    //    planeName = thePlaneMapping_->getPlaneName(planeID);

    int   hitID       =      -1 ;
    int   totalCharge =       0 ;
    int   chargeDown  =       0 ;
    int   chargeUp    =       0 ;
    float asimmetry   =       0 ;

    for(int h=0; h<size; ++h)
    {
        if(data.getClusterPixelRow(h,planeID) == row)//mi assicuro che ci sia la cella predetta in uno dei due hit
        {
            hitID   = h   ;
            break;
        }
    }
    if( hitID == -1)
        return;

    if(size == 3 || size ==4 )
    {
        int secondHigh = 0;
        int firstHigh  = 0;


        for(int h=0; h<size; ++h)
        {
            if(data.getClusterPixelCharge(h,planeID)>firstHigh)
            {
                secondHigh = firstHigh;
                firstHigh  = data.getClusterPixelCharge(h,planeID);
            }
            else if(data.getClusterPixelCharge(h,planeID)>secondHigh)
            {
                secondHigh = data.getClusterPixelCharge(h,planeID);
            }
            if(   (data.getClusterPixelCharge(h,planeID) != firstHigh || data.getClusterPixelCharge(h,planeID) != secondHigh)
                  && (data.getClusterPixelRow(h,planeID)    == row) )
                return;
            else if(data.getClusterPixelCharge(h,planeID) == firstHigh || data.getClusterPixelCharge(h,planeID) == secondHigh)
            {
                if( yPixelResidual  > 0 && (row - data.getClusterPixelRow(h,planeID)) == -1 )//il secondo hit e' sopra la predetta
                {
                    chargeUp    = data.getClusterPixelCharge(h    ,planeID);
                    chargeDown  = data.getClusterPixelCharge(hitID,planeID);
                    break;
                }
                else if( yPixelResidual  <= 0 && (row - data.getClusterPixelRow(h,planeID)) == 1 )//il secondo hit e' sotto la predetta
                {
                    chargeUp    = data.getClusterPixelCharge(hitID,planeID);
                    chargeDown  = data.getClusterPixelCharge(h    ,planeID);
                    break;
                }
                //else
                //    return;
                else if( yPixelResidual  > 0 && (row - data.getClusterPixelRow(h,planeID)) == 1)
                {
                    chargeUp    = data.getClusterPixelCharge(hitID,planeID);
                    chargeDown  = data.getClusterPixelCharge(h    ,planeID);
                    yPixelEdgeResidual =  yPixelResidual  + data.getYPitchLocal(planeID)/2;
                    break;
                }
                else if( yPixelResidual  < 0 && (row - data.getClusterPixelRow(h,planeID)) ==  -1)
                {
                    chargeUp    = data.getClusterPixelCharge(h    ,planeID);
                    chargeDown  = data.getClusterPixelCharge(hitID,planeID);
                    yPixelEdgeResidual = yPixelResidual  - data.getYPitchLocal(planeID)/2;
                    break;
                }
            }
        }
    }

    if(size == 2)//it is if, not else if because there could be clusters of size 1 and we are not studying them in this way.
    {
        for(int h=0; h<size; ++h)
        {
            if(yPixelResidual  > 0 && (row - data.getClusterPixelRow(h,planeID)) == -1) //il secondo hit e' sopra la predetta
            {
                chargeUp    = data.getClusterPixelCharge(h    ,planeID);
                chargeDown  = data.getClusterPixelCharge(hitID,planeID);
                break;
            }
            else if(yPixelResidual  <= 0 && (row - data.getClusterPixelRow(h,planeID)) == 1)//il secondo hit e' a sotto la predetta
            {
                chargeUp    = data.getClusterPixelCharge(hitID,planeID);
                chargeDown  = data.getClusterPixelCharge(h    ,planeID);
                break;
            }
            //else
            //    return;
            else if(yPixelResidual  > 0 && (row - data.getClusterPixelRow(h,planeID)) == 1)
            {
                chargeUp    = data.getClusterPixelCharge(hitID,planeID);
                chargeDown  = data.getClusterPixelCharge(h    ,planeID);
                yPixelEdgeResidual =  yPixelResidual  + data.getYPitchLocal(planeID)/2;
                break;
            }
            else if(yPixelResidual  < 0 && (row - data.getClusterPixelRow(h,planeID)) ==  -1)
            {
                chargeUp    = data.getClusterPixelCharge(h    ,planeID);
                chargeDown  = data.getClusterPixelCharge(hitID,planeID);
                yPixelEdgeResidual = yPixelResidual  - data.getYPitchLocal(planeID)/2;
                break;
            }
        }
    }

    //        float slope       =  -12  ;
    //        float intercept   = -1.50223 ;

    totalCharge = chargeDown + chargeUp;
    asimmetry  = (float)(chargeDown - chargeUp)/totalCharge;

    float yMeasured;                      ;
    if(asimmetry >= -1. && asimmetry <= 1.)
    {
        //        yMeasured = slope*asimmetry + intercept;

        toGet = "Charge/" + planeName +  "/YAsimmetry/h1DYcellChargeAsimmetryInv_" + planeName;
        if((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()) ){
            if((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fYAsimmetryFit") != NULL){

                yMeasured = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fYAsimmetryFit"))->Eval(asimmetry);
            }
        }

        THREADED(hYResidualCalculated_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );

        if(size == 2)
        {
            THREADED(hYResidualCalculatedSize2_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );
            THREADED(hY2DResidualCalculatedSize2_ [planeID])->Fill( yPixelEdgeResidual, yMeasured - yPixelEdgeResidual );
            //THREADED(h2DCorrelationsResidualYvsY_[planeID])->Fill(Yall,yMeasured - yPixelEdgeResidual);
            //THREADED(h2DCorrelationsResidualYvsX_[planeID])->Fill(Xall,yMeasured - yPixelEdgeResidual);


            //2Rows
            if((row-20)%2==0)
                THREADED(hYResidualCalculatedSize2Row1of2Rows_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );
            if((row-20)%2==1)
                THREADED(hYResidualCalculatedSize2Row2of2Rows_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );

            //4Rows
            //Landau:
            //per Row 1: Charge down, per Row 2: solo charge Up perche' voglio correlazioni tra 1 e 2. Discorso speculare per 3 e 4
            if((row-50)%4==0){
                THREADED(hYResidualCalculatedSize2Row1of4Rows_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );
                if((data.getClusterPixelRow(hitID,planeID)-50)%4 == 0 || (data.getClusterPixelRow(hitID,planeID)-50)%4 == 1)
                    THREADED(hLandauChargeDownSize2Row1of4Rows_           [planeID])->Fill( chargeDown );
            }
            else if((row-50)%4==1){
                THREADED(hYResidualCalculatedSize2Row2of4Rows_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );
                if((data.getClusterPixelRow(hitID,planeID)-50)%4 == 0 || (data.getClusterPixelRow(hitID,planeID)-50)%4 == 1)
                    THREADED(hLandauChargeUpSize2Row2of4Rows_           [planeID])->Fill( chargeUp );
            }
            else if((row-50)%4==2){
                THREADED(hYResidualCalculatedSize2Row3of4Rows_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );
                if((data.getClusterPixelRow(hitID,planeID)-50)%4 == 2 || (data.getClusterPixelRow(hitID,planeID)-50)%4 == 3)
                    THREADED(hLandauChargeDownSize2Row3of4Rows_           [planeID])->Fill( chargeDown );
            }
            else if((row-50)%4==3){
                THREADED(hYResidualCalculatedSize2Row4of4Rows_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );
                if((data.getClusterPixelRow(hitID,planeID)-50)%4 == 2 || (data.getClusterPixelRow(hitID,planeID)-50)%4 == 3)
                    THREADED(hLandauChargeUpSize2Row4of4Rows_           [planeID])->Fill( chargeUp );
            }
        }
        else if(size == 3)
        {
            THREADED(hYResidualCalculatedSize3_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );
            THREADED(hY2DResidualCalculatedSize3_ [planeID])->Fill( yPixelEdgeResidual, yMeasured - yPixelEdgeResidual );
        }
        else if(size == 4)
        {
            THREADED(hYResidualCalculatedSize4_   [planeID])->Fill( yMeasured - yPixelEdgeResidual );
            THREADED(hY2DResidualCalculatedSize4_ [planeID])->Fill( yPixelEdgeResidual, yMeasured - yPixelEdgeResidual );
        }

    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::xResolution(bool pass, int planeID, const Data& data, int threadNumber)
{
    if(!pass || !data.getBelongsToTrack(planeID))
        return;

    //Track selection
    if(data.getNumberOfTelescopeClustersSizeLE2()!=8)
        return;

    //checking telescope
    int counter = 0;
    for(int p=8; p<16; p++)
    {
        if(data.getClusterSize(p)==2)
            counter++;
    }
    if(counter != 8)
        return;

    int   size    = data.getClusterSize(planeID);
    int   maxHits = 4; //Irene modified. Before it was maxHits = 3;
    float Xp;

    if(size > maxHits)
        return;

    const Window* theWindow  = theWindowsManager_->getWindow(planeID);

    for(int h=0; h<size; h++)
    {
        if( !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber()) ) //i due hit devono stare nella finestra
            return;
    }

    THREADED(hXResiduals_[planeID])->Fill(data.getXTrackResidualLocal(planeID));

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

        THREADED(hXResidualsDigital_[planeID])->Fill(xHit-xRes);


        if(data.getXPixelResidualLocal( planeID ) > 0)
            Xp = -data.getXPitchLocal( planeID )/2 + data.getXPixelResidualLocal( planeID );
        else if(data.getXPixelResidualLocal( planeID ) <= 0)
            Xp = (data.getXPixelResidualLocal( planeID ) + data.getXPitchLocal( planeID )/2);

        THREADED(hX2DResidualsDigital_[planeID])->Fill(xHit-xRes,Xp);

    }



    if(size == 1){
        THREADED(hXResidualsClusterSize1_  [planeID])->Fill(data.getXTrackResidualLocal(planeID));
        THREADED(hX2DResidualsClusterSize1_[planeID])->Fill(data.getXTrackResidualLocal(planeID),data.getXPixelResidualLocal(planeID));
    }
    else if(size == 2)
    {
        if(data.getClusterPixelRow(0,planeID) != data.getClusterPixelRow(1,planeID))
            return;

        THREADED(hXResidualsClusterSize2_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
    }
    else if(size == 3)//Irene added
    {
        if((data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID)) ||(data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(2,planeID))|| (data.getClusterPixelCol(1,planeID) != data.getClusterPixelCol(2,planeID)))
            return;

        THREADED(hXResidualsClusterSize3_[planeID])->Fill(data.getXTrackResidualLocal(planeID));

    }
    else if(size == 4)//Irene added
    {
        if((data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID)) || (data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(2,planeID)) || (data.getClusterPixelCol(1,planeID) != data.getClusterPixelCol(2,planeID))
                ||(data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(3,planeID)) || (data.getClusterPixelCol(1,planeID) != data.getClusterPixelCol(3,planeID)) || (data.getClusterPixelCol(2,planeID) != data.getClusterPixelCol(3,planeID))   )
            return;

        THREADED(hXResidualsClusterSize4_[planeID])->Fill(data.getXTrackResidualLocal(planeID));

    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Resolution::yResolution(bool pass, int planeID, const Data& data, int threadNumber)
{
    if(!pass || !data.getBelongsToTrack(planeID))
        return;

    //Track selection
    if(data.getNumberOfTelescopeClustersSizeLE2()!=8)
        return;

    //checking telescope
    int counter = 0;
    for(int p=8; p<16; p++)
    {
        if(data.getClusterSize(p)==2)
            counter++;
    }
    if(counter != 8)
        return;

    int   size    = data.getClusterSize(planeID);
    int   maxHits = 4; //Irene modified. Before it was 2.
    float Yp;

    if(size > maxHits)
        return;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);

    for(int h=0; h<size; h++)
    {
        if( !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber()) ) //i due hit devono stare nella finestra
            return;
    }

    THREADED(hYResiduals_[planeID])->Fill(data.getYTrackResidualLocal(planeID));

    if( theWindow->checkTimeWindowAbout(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber())
            && theWindow->checkWindowAbout    (data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber(),thePlaneMapping_->getPlaneType(planeID)) ){
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

        THREADED(hYResidualsDigital_[planeID])->Fill(yHit-yRes);

        if(data.getYPixelResidualLocal( planeID ) > 0)
            Yp = -data.getYPitchLocal( planeID )/2 + data.getYPixelResidualLocal( planeID );
        else if(data.getYPixelResidualLocal( planeID ) <= 0)
            Yp = (data.getYPixelResidualLocal( planeID ) + data.getYPitchLocal( planeID )/2);

        THREADED(hY2DResidualsDigital_[planeID])->Fill(yHit-yRes,Yp);

    }

    if(size == 1){
        if(data.getRowPredicted(planeID)!=50)
        {
            THREADED(hYResidualsClusterSize1_  [planeID])->Fill(data.getYTrackResidualLocal(planeID));
            THREADED(hY2DResidualsClusterSize1_[planeID])->Fill(data.getYTrackResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
        }
    }
    else if(size == 2 )
    {
        if(data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID))
            return;

        THREADED(hYResidualsClusterSize2_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
    }
    else if(size == 3)//Irene added
    {
        if((data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID)) ||(data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(2,planeID))|| (data.getClusterPixelCol(1,planeID) != data.getClusterPixelCol(2,planeID)))
            return;

        THREADED(hYResidualsClusterSize3_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
    }
    else if(size == 4)//Irene added
    {
        if((data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID)) ||(data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(2,planeID))|| (data.getClusterPixelCol(1,planeID) != data.getClusterPixelCol(2,planeID))
                ||(data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(3,planeID)) || (data.getClusterPixelCol(1,planeID) != data.getClusterPixelCol(3,planeID)) || (data.getClusterPixelCol(2,planeID) != data.getClusterPixelCol(3,planeID)))
            return;

        THREADED(hYResidualsClusterSize4_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
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
//    int excludeMe = 0;
    if(thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos)//Dut case
        minHits = atoi(theParser->getAnalysesFromString("Charge")->getMinHits().c_str());
//    else if(data.getHasHit(planeID) && data.getClusterSize(planeID)<=2)//Telescope case
//        excludeMe = 1;

//    int hitsontelescope = 0;
//    bool HitsClusterLE2 = true;
//    for (unsigned int i = 0; i < 8; i++) 
//    {
//        if (data.getHasHit(i) == true && thePlaneMapping_->getPlaneName(i).find("Dut") == std::string::npos) hitsontelescope++;
//        if (data.getClusterSize(i) > 2) 
//	{
//            HitsClusterLE2 = false;
//        }
//    }

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
	
	
	// ##############################################################
	// # Mauro : require all telescope planes with cluster size = 2 #
	// ##############################################################
	// for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
	//   if ((p > 7) && (p < 16) && (data.getClusterSize(p) != 2)) return;
	
	
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
        ADD_THREADED(hXResiduals_                         [p]);
        ADD_THREADED(hXResidualCalculated_                [p]);
        ADD_THREADED(hXResidualCalculatedSize2_           [p]);
        ADD_THREADED(hX2DResidualCalculatedSize2_         [p]);
        ADD_THREADED(hXResidualCalculatedSize3_           [p]);
        ADD_THREADED(hX2DResidualCalculatedSize3_         [p]);
        ADD_THREADED(hXResidualCalculatedSize4_           [p]);
        ADD_THREADED(hX2DResidualCalculatedSize4_         [p]);
        ADD_THREADED(hXResidualsClusterSize1_             [p]);
        ADD_THREADED(hX2DResidualsClusterSize1_           [p]);
        ADD_THREADED(hXResidualsClusterSize2_             [p]);
        ADD_THREADED(hXResidualsClusterSize3_             [p]);
        ADD_THREADED(hXResidualsClusterSize4_             [p]);
        ADD_THREADED(hXResidualsDigital_                  [p]);
        ADD_THREADED(hX2DResidualsDigital_                [p]);
        ADD_THREADED(hYResiduals_                         [p]);
        ADD_THREADED(hYResidualCalculated_                [p]);
        ADD_THREADED(hYResidualCalculatedSize2_           [p]);
        ADD_THREADED(hYResidualCalculatedSize2Row1of4Rows_[p]);
        ADD_THREADED(hYResidualCalculatedSize2Row2of4Rows_[p]);
        ADD_THREADED(hYResidualCalculatedSize2Row3of4Rows_[p]);
        ADD_THREADED(hYResidualCalculatedSize2Row4of4Rows_[p]);
        ADD_THREADED(hLandauChargeDownSize2Row1of4Rows_   [p]);
        ADD_THREADED(hLandauChargeUpSize2Row2of4Rows_     [p]);
        ADD_THREADED(hLandauChargeDownSize2Row3of4Rows_   [p]);
        ADD_THREADED(hLandauChargeUpSize2Row4of4Rows_     [p]);
        ADD_THREADED(hY2DResidualCalculatedSize2_         [p]);
        ADD_THREADED(hYResidualCalculatedSize3_           [p]);
        ADD_THREADED(hY2DResidualCalculatedSize3_         [p]);
        ADD_THREADED(hYResidualCalculatedSize4_           [p]);
        ADD_THREADED(hY2DResidualCalculatedSize4_         [p]);
        ADD_THREADED(hYResidualsClusterSize1_             [p]);
        ADD_THREADED(hY2DResidualsClusterSize1_           [p]);
        ADD_THREADED(hYResidualsClusterSize2_             [p]);
        ADD_THREADED(hYResidualsClusterSize3_             [p]);
        ADD_THREADED(hYResidualsClusterSize4_             [p]);
        ADD_THREADED(hYResidualsDigital_                  [p]);
        ADD_THREADED(hY2DResidualsDigital_                [p]);
        ADD_THREADED(h2DCorrelationsResidualXvsX_         [p]);
        ADD_THREADED(h2DCorrelationsResidualXvsY_         [p]);
        ADD_THREADED(h2DCorrelationsResidualYvsY_         [p]);
        ADD_THREADED(h2DCorrelationsResidualYvsX_         [p]);
        ADD_THREADED(hCorrelationsResidualXvsX_           [p]);
        ADD_THREADED(hCorrelationsResidualYvsY_           [p]);
        ADD_THREADED(hYResidualCalculatedSize2Row1of2Rows_[p]);
        ADD_THREADED(hYResidualCalculatedSize2Row2of2Rows_[p]);


        hXResiduals_                         [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hXResidualCalculated_                [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hXResidualCalculatedSize2_           [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hX2DResidualCalculatedSize2_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hX2DResidualCalculatedSize2_         [p]->GetYaxis()->SetTitle("x residual (um)"   );
        hXResidualCalculatedSize3_           [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hX2DResidualCalculatedSize3_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hX2DResidualCalculatedSize3_         [p]->GetYaxis()->SetTitle("x residual (um)"   );
        hXResidualCalculatedSize4_           [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hX2DResidualCalculatedSize4_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hX2DResidualCalculatedSize4_         [p]->GetYaxis()->SetTitle("x residual (um)"   );
        hXResidualsClusterSize1_             [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hX2DResidualsClusterSize1_           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hX2DResidualsClusterSize1_           [p]->GetYaxis()->SetTitle("x residual (um)"   );
        hXResidualsClusterSize2_             [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hXResidualsClusterSize3_             [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hXResidualsClusterSize4_             [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hXResidualsDigital_                  [p]->GetXaxis()->SetTitle("x residual (um)"   );
        hX2DResidualsDigital_                [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hX2DResidualsDigital_                [p]->GetYaxis()->SetTitle("x residual (um)"   );
        hYResiduals_                         [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculated_                [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize2_           [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize2Row1of4Rows_[p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize2Row2of4Rows_[p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize2Row3of4Rows_[p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize2Row4of4Rows_[p]->GetXaxis()->SetTitle("y residual (um)"   );
        hLandauChargeDownSize2Row1of4Rows_   [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauChargeUpSize2Row2of4Rows_     [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauChargeDownSize2Row3of4Rows_   [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauChargeUpSize2Row4of4Rows_     [p]->GetXaxis()->SetTitle("charge (electrons)");
        hY2DResidualCalculatedSize2_         [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hY2DResidualCalculatedSize2_         [p]->GetYaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize3_           [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hY2DResidualCalculatedSize3_         [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hY2DResidualCalculatedSize3_         [p]->GetYaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize4_           [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hY2DResidualCalculatedSize4_         [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hY2DResidualCalculatedSize4_         [p]->GetYaxis()->SetTitle("y residual (um)"   );
        hYResidualsClusterSize1_             [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hY2DResidualsClusterSize1_           [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hY2DResidualsClusterSize1_           [p]->GetYaxis()->SetTitle("y residual (um)"   );
        hYResidualsClusterSize2_             [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualsClusterSize3_             [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualsClusterSize4_             [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualsDigital_                  [p]->GetXaxis()->SetTitle("y residual (um)"   );
        hY2DResidualsDigital_                [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hY2DResidualsDigital_                [p]->GetYaxis()->SetTitle("y residual (um)"   );
        h2DCorrelationsResidualXvsX_         [p]->GetXaxis()->SetTitle("x (um)"            );
        h2DCorrelationsResidualXvsX_         [p]->GetYaxis()->SetTitle("x residual (um)"   );
        h2DCorrelationsResidualXvsY_         [p]->GetXaxis()->SetTitle("y (um)"            );
        h2DCorrelationsResidualXvsY_         [p]->GetYaxis()->SetTitle("x residual (um)"   );
        h2DCorrelationsResidualYvsY_         [p]->GetXaxis()->SetTitle("y (um)"            );
        h2DCorrelationsResidualYvsY_         [p]->GetYaxis()->SetTitle("y residual (um)"   );
        h2DCorrelationsResidualYvsX_         [p]->GetXaxis()->SetTitle("x (um)"            );
        h2DCorrelationsResidualYvsX_         [p]->GetYaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize2Row1of2Rows_[p]->GetXaxis()->SetTitle("y residual (um)"   );
        hYResidualCalculatedSize2Row2of2Rows_[p]->GetXaxis()->SetTitle("y residual (um)"   );

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
        hCorrelationsResidualXvsY_.push_back((TH1F*)h2DCorrelationsResidualXvsY_[p]->ProfileX(hName.c_str(),1,-1));
        hCorrelationsResidualXvsY_[p]->SetTitle(hTitle.c_str());

        hName  = "hCorrelationsResidualYvsY_"                          + planeName;
        hTitle = "Correlation Y residuals vs Y "                        + planeName;
        Utilities::customProfileX(h2DCorrelationsResidualYvsY_[p],hCorrelationsResidualYvsY_[p],hName.c_str(),hTitle.c_str(),1,-1,0);
        //hCorrelationsResidualYvsY_.push_back(NEW_THREADED(TH1F("dummy","dummy",1,0,1)));
        //Utilities::customProfileX(h2DCorrelationsResidualYvsY_[p],hCorrelationsResidualYvsY_[p],hName.c_str(),hTitle.c_str(),1,-1,0);
        //        hCorrelationsResidualYvsY_.push_back((TH1F*)h2DCorrelationsResidualYvsY_[p]->ProfileX(hName.c_str(),1,-1));
        //        hCorrelationsResidualYvsY_[p]->SetTitle(hTitle.c_str());

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

        if(p == 22)
        {
            std::stringstream ss;

            ss.str(""); ss << "Detector: " << planeName;
            STDLINE(ss.str(),ACGreen);

            STDLINE("Single hits residuals:", ACRed);

            ss.str(""); ss << "X Residuals: " << hXResidualsClusterSize1_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            ss.str(""); ss << "Y Residuals: " << hYResidualsClusterSize1_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            STDLINE("",ACWhite);

            STDLINE("Double hits residuals:", ACRed);
            ss.str(""); ss << "X Residuals: " << hXResidualsClusterSize2_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            ss.str(""); ss << "Y Residuals: " << hYResidualsClusterSize2_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            STDLINE("",ACWhite);

            STDLINE("Double hits residuals:", ACRed);
            ss.str(""); ss << "X Residuals: " << hXResidualsClusterSize3_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            ss.str(""); ss << "Y Residuals: " << hYResidualsClusterSize3_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            STDLINE("",ACWhite);

            STDLINE("Double hits residuals:", ACRed);
            ss.str(""); ss << "X Residuals: " << hXResidualsClusterSize4_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            ss.str(""); ss << "Y Residuals: " << hYResidualsClusterSize4_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            STDLINE("",ACWhite);

            STDLINE("Single and double hits residuals:", ACRed);
            ss.str(""); ss << "X Residuals: " << hXResiduals_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            ss.str(""); ss << "Y Residuals: " << hYResiduals_[p]->GetMean();
            STDLINE(ss.str(),ACGreen);
            STDLINE("",ACWhite);
        }
        if(theAnalysisManager_->getXmlParser()->getAnalysesFromString("Resolution")->doFits())
        {
            if(p == 22)
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
    float       xPitch;
    float       yPitch;
    int         lowerCol;
    int         higherCol;
    int         lowerRow;
    int         higherRow;

    float       binSize         =     5;
    //int         numberOfRows    =    80;
    //int         numberOfColumns =    52;

    theAnalysisManager_->cd("/");
    theAnalysisManager_->mkdir("Resolution");

    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        planeName = thePlaneMapping_->getPlaneName(p);
        xPitch    = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str()); // Irene changed it. Before it was 150;
        yPitch    = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());// Irene changed it. Before it was 100;
        lowerCol  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerCol()).c_str());
        higherCol = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherCol()).c_str());
        lowerRow  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerRow()).c_str());
        higherRow = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherRow()).c_str());


        int         numberOfRows    =  higherRow - lowerRow + 1  ;
        int         numberOfColumns =  higherCol - lowerCol + 1  ;

        theAnalysisManager_->cd("Resolution");
        theAnalysisManager_->mkdir(planeName);

        theAnalysisManager_->mkdir("XResiduals");

        hName  = "hXResiduals_"                               + planeName;
        hTitle = "X residuals  "                              + planeName;
        hXResiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hXResidualCalculated_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit " + planeName;
        hXResidualCalculated_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));

        hName  = "hXResidualCalculatedSize2_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit (Clusters Size 2) " + planeName;
        hXResidualCalculatedSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));

        hName  = "hX2DResidualCalculatedSize2_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit vs X (Clusters Size 2) " + planeName;
        hX2DResidualCalculatedSize2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -xPitch/2., xPitch/2., 125, -500, 500)));

        hName  = "hXResidualCalculatedSize3_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit (Clusters Size 3) " + planeName;
        hXResidualCalculatedSize3_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));

        hName  = "hX2DResidualCalculatedSize3_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit vs X (Clusters Size 3) " + planeName;
        hX2DResidualCalculatedSize3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -xPitch/2., xPitch/2., 125, -500, 500)));

        hName  = "hXResidualCalculatedSize4_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit (Clusters Size 4) " + planeName;
        hXResidualCalculatedSize4_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));

        hName  = "hX2DResidualCalculatedSize4_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit vs X (Clusters Size 4) " + planeName;
        hX2DResidualCalculatedSize4_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -xPitch/2., xPitch/2., 125, -500, 500)));

        hName  = "hXResidualsClusterSize1_"                   + planeName;
        hTitle = "X residuals cluster size 1 "                + planeName;
        hXResidualsClusterSize1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hX2DresidualClusterSize1_"                      + planeName;
        hTitle = "X residuals cluster size 1 vs X " + planeName;
        hX2DResidualsClusterSize1_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -xPitch/2., xPitch/2., 125, -500, 500)));

        hName  = "hXResidualsClusterSize2_"                   + planeName;
        hTitle = "X residuals cluster size 2 "                + planeName;
        hXResidualsClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hXResidualsClusterSize3_"                   + planeName;
        hTitle = "X residuals cluster size 3 "                + planeName;
        hXResidualsClusterSize3_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hXResidualsClusterSize4_"                   + planeName;
        hTitle = "X residuals cluster size 4 "                + planeName;
        hXResidualsClusterSize4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hXResidualsDigital_"                        + planeName;
        hTitle = "X residuals for digitalized hits "          + planeName;
        hXResidualsDigital_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 125, -500, 500)));

        hName  = "hX2DResidualsDigital_"                        + planeName;
        hTitle = "X residuals for digitalized hits vs X"          + planeName;
        hX2DResidualsDigital_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), (int)xPitch/binSize, -xPitch/2., xPitch/2., 125, -500, 500)));

        theAnalysisManager_->cd("Resolution/" + planeName);

        theAnalysisManager_->mkdir("YResiduals");

        hName  = "hYResiduals_"                               + planeName;
        hTitle = "Y residuals "                               + planeName;
        hYResiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualCalculated_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit " + planeName;
        hYResidualCalculated_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualCalculatedSize2_"                                  + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 2) " + planeName;
        hYResidualCalculatedSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        theAnalysisManager_->cd("Resolution/" + planeName);

        theAnalysisManager_->mkdir("YResiduals2Rows25x600");

        hName  = "hYResidualCalculatedSize2Row1of2Rows_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 2) Row1of2Rows " + planeName;
        hYResidualCalculatedSize2Row1of2Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualCalculatedSize2Row2of2Rows_"                                  + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 2) Row2of2Rows " + planeName;
        hYResidualCalculatedSize2Row2of2Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        theAnalysisManager_->cd("Resolution/" + planeName);

        theAnalysisManager_->mkdir("YResiduals4Rows25x600");

        hName  = "hYResidualCalculatedSize2Row1of4Rows_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 2) Row1of4Rows " + planeName;
        hYResidualCalculatedSize2Row1of4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualCalculatedSize2Row2of4Rows_"                                  + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 2) Row2of4Rows " + planeName;
        hYResidualCalculatedSize2Row2of4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualCalculatedSize2Row3of4Rows_"                                  + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 2) Row3of4Rows " + planeName;
        hYResidualCalculatedSize2Row3of4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualCalculatedSize2Row4of4Rows_"                                  + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 2) Row4of4Rows " + planeName;
        hYResidualCalculatedSize2Row4of4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        theAnalysisManager_->cd("Resolution/" + planeName);

        theAnalysisManager_->mkdir("Landau4Rows25x600");

        hName  = "hLandauChargeDownSize2Row1of4Rows_"                      + planeName;
        hTitle = "Charge distributions (Cluster Size 2) Row1of4Rows " + planeName;
        hLandauChargeDownSize2Row1of4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(),  200, -10000, 100000)));

        hName  = "hLandauChargeUpSize2Row2of4Rows_"                      + planeName;
        hTitle = "Charge distributions (Cluster Size 2) Row2of4Rows " + planeName;
        hLandauChargeUpSize2Row2of4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(),  200, -10000, 100000)));

        hName  = "hLandauChargeDownSize2Row3of4Rows_"                 + planeName;
        hTitle = "Charge distributions (Cluster Size 2) Row3of4Rows " + planeName;
        hLandauChargeDownSize2Row3of4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(),  200, -10000, 100000)));

        hName  = "hLandauChargeUpSize2Row4of4Rows_"                      + planeName;
        hTitle = "Charge distributions (Cluster Size 2) Row4of4Rows " + planeName;
        hLandauChargeUpSize2Row4of4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(),  200, -10000, 100000)));

        theAnalysisManager_->cd("Resolution/" + planeName +"/YResiduals/");

        hName  = "hY2DResidualCalculatedSize2_"                                     + planeName;
        hTitle = "Y residuals calculated from asimmetry fit vs Y (Cluster Size 2) " + planeName;
        hY2DResidualCalculatedSize2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/binSize, -yPitch/2., yPitch/2., 200, -400, 400)));

        hName  = "hYResidualCalculatedSize3_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 3) " + planeName;
        hYResidualCalculatedSize3_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        hName  = "hY2DResidualCalculatedSize3_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit vs Y (Cluster Size 3) " + planeName;
        hY2DResidualCalculatedSize3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/binSize, -yPitch/2., yPitch/2., 200, -400, 400)));

        hName  = "hYResidualCalculatedSize4_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 4) " + planeName;
        hYResidualCalculatedSize4_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -400, 400)));

        hName  = "hY2DResidualCalculatedSize4_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit vs Y (Cluster Size 4) " + planeName;
        hY2DResidualCalculatedSize4_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/binSize, -yPitch/2., yPitch/2., 200, -400, 400)));

        hName  = "hYResidualsClusterSize1_"                   + planeName;
        hTitle = "Y residuals cluster size 1 "                + planeName;
        hYResidualsClusterSize1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 200, -400, 400)));

        hName  = "hY2DresidualClusterSize1_"                      + planeName;
        hTitle = "Y residuals cluster size 1 vs Y " + planeName;
        hY2DResidualsClusterSize1_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/binSize, -yPitch/2., yPitch/2., 200, -400, 400)));

        hName  = "hYResidualsClusterSize2_"                   + planeName;
        hTitle = "Y residuals cluster size 2 "                + planeName;
        hYResidualsClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualsClusterSize3_"                   + planeName;
        hTitle = "Y residuals cluster size 3 "                + planeName;
        hYResidualsClusterSize3_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualsClusterSize4_"                   + planeName;
        hTitle = "Y residuals cluster size 4 "                + planeName;
        hYResidualsClusterSize4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 200, -400, 400)));

        hName  = "hYResidualsDigital_"                        + planeName;
        hTitle = "Y residuals for digitalized hits "          + planeName;
        hYResidualsDigital_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 200, -400, 400)));

        hName  = "hY2DResidualsDigital_"                        + planeName;
        hTitle = "Y residuals for digitalized hits vs Y"          + planeName;
        hY2DResidualsDigital_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), (int)yPitch/binSize, -yPitch/2., yPitch/2., 200, -400, 400)));

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
        hTitle = "Correlation X residuals vs X "                       + planeName;
        h2DCorrelationsResidualXvsX_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfColumns+2, (float)xPitch*lowerCol, (float)xPitch*(lowerCol+numberOfColumns+2), 300, -300., 300.)));//, 150, -75., 75.)));

        hName  = "h2DCorrelationsResidualXvsY_"                        + planeName;
        hTitle = "Correlation X residuals vs Y "                       + planeName;
        h2DCorrelationsResidualXvsY_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfRows   +1, (float)yPitch*lowerRow, (float)yPitch*(lowerRow + numberOfRows + 1), 300, -300., 300.)));//, 100, -50., 50.)));

        hName  = "h2DCorrelationsResidualYvsY_"                        + planeName;
        hTitle = "Correlation Y residuals vs Y "                       + planeName;
        h2DCorrelationsResidualYvsY_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfRows   +1, (float)yPitch*lowerRow, (float)yPitch*(lowerRow + numberOfRows + 1), 300, -300., 300.)));//, 100, -50., 50.)));

        hName  = "h2DCorrelationsResidualYvsX_"                        + planeName;
        hTitle = "Correlation Y residuals vs X "                        + planeName;
        h2DCorrelationsResidualYvsX_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfColumns+2, (float)xPitch*lowerCol, (float)xPitch*(lowerCol+numberOfColumns+2), 300, -300., 300.)));//, 150, -75., 75.)));

        hName  = "hCorrelationsResidualXvsX_"                          + planeName;
        hTitle = "Correlation X residuals vs X "                        + planeName;
        hCorrelationsResidualXvsX_.push_back(NEW_THREADED( TH1F(hName.c_str(),hTitle.c_str(), numberOfColumns+2, (float)xPitch*lowerCol, (float)xPitch*(lowerCol+numberOfColumns+2)) ));

        hName  = "hCorrelationsResidualYvsY_"                        + planeName;
        hTitle = "Correlation Y residuals vs Y "                        + planeName;
        hCorrelationsResidualYvsY_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), numberOfRows   +1, (float)yPitch*lowerRow, (float)yPitch*(lowerRow+numberOfRows   +1)) ));

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
