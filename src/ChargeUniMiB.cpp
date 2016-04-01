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

#include "Charge.h"
#include "AnalysisManager.h"
#include "ThreadUtilities.h"
#include "PlanesMapping.h"
#include "WindowsManager.h"
#include "CalibrationsManager.h"
#include "Window.h"
#include "MessageTools.h"
#include "Utilities.h"
#include "XmlParser.h"
#include "XmlPlane.h"
#include "XmlAnalysis.h"
#include "HistogramWindow.h"

#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TThread.h>
#include <TFile.h>
#include <TTreeFormula.h>
#include <TMath.h>
#include <TF1.h>
#include <Rtypes.h>
#include <TROOT.h>
#include <TVectorT.h>
#include <TMap.h>

#include <iostream>
#include <map>


// @@@ Hard coded parameters @@@
#define maxChargeDeltaRay 13600. // = 8000 (MPV Landau for 100 um bulk thickness) * 1.7
#define ONLYdoubleHITS false     // Process only clusters of size 2
// ============================


//=======================================================================
Charge::Charge(AnalysisManager* analysisManager, int nOfThreads) :
  Analysis(analysisManager, nOfThreads),
  thePlaneMapping_(0),
  langaus_(0),
  linear_(0),
  theWindowsManager_(0),
  theXmlParser_(analysisManager->getXmlParser()),
  standardCutsPixelMinimumCharge_(0),
  standardCutsPixelMaximumCharge_(0),
  standardCutsClusterMinimumCharge_(0),
  standardCutsClusterMaximumCharge_(0)
{
  thePlaneMapping_ = new PlanesMapping();

  for (int p = 0; p < 4; p++)
    {
      parMin_      [p] = 0;
      parMax_      [p] = 0;
      isMinToLimit_[p] = 0;
      isMaxToLimit_[p] = 0;
    }
  
  langaus_ = new TF1("langaus",Utilities::langaus,0,60000,4);
  langaus_->SetParNames("Width","MPV","Area","GSigma");
  langaus_->SetLineColor(kBlue);
  
  linear_ = new TF1("linear",Utilities::linear,0,60000,2);
  linear_->SetParNames("Intercept","Slope");
  linear_->SetLineColor(kBlue);
}

//=======================================================================
Charge::~Charge(void)
{
  if (thePlaneMapping_)
    {
      delete thePlaneMapping_;
      thePlaneMapping_ = 0;
    }
  
  if (langaus_)
    {
      delete langaus_;
      langaus_ = 0;
    }

  if (linear_)
    {
      delete linear_;
      linear_ = 0;
    }

  destroy();
}

//=======================================================================
// @TMP@
void Charge::setInvincible(bool cannotBeKilled)
{
  cannotBeDestroyed_ = cannotBeKilled;
}

//=======================================================================
void Charge::destroy()
{
  if (Analysis::fDoNotDelete_ || cannotBeDestroyed_) return;

  std::vector<TH1F*>::iterator it1;
  std::vector<TH2F*>::iterator it2;

  for(it1=hCellLandau_                   .begin(); it1!=hCellLandau_                   .end(); it1++) delete *it1; hCellLandau_                  .clear();
  for(it1=hClusterSize_                  .begin(); it1!=hClusterSize_                  .end(); it1++) delete *it1; hClusterSize_                 .clear();
  
  for(it1=h1DXcellCharge_                .begin(); it1!=h1DXcellCharge_                .end(); it1++) delete *it1; h1DXcellCharge_               .clear();
  for(it1=h1DXcellChargeNorm_            .begin(); it1!=h1DXcellChargeNorm_            .end(); it1++) delete *it1; h1DXcellChargeNorm_           .clear();
  
  for(it1=h1DXcellChargeSecondHit_       .begin(); it1!=h1DXcellChargeSecondHit_       .end(); it1++) delete *it1; h1DXcellChargeSecondHit_      .clear();
  for(it1=h1DXcellChargeSecondHitNorm_   .begin(); it1!=h1DXcellChargeSecondHitNorm_   .end(); it1++) delete *it1; h1DXcellChargeSecondHitNorm_  .clear();
  
  for(it1=h1DYcellCharge_                .begin(); it1!=h1DYcellCharge_                .end(); it1++) delete *it1; h1DYcellCharge_               .clear();
  for(it1=h1DYcellChargeNorm_            .begin(); it1!=h1DXcellChargeNorm_            .end(); it1++) delete *it1; h1DXcellChargeNorm_           .clear();
  
  for(it1=h1DYcellChargeSecondHit_       .begin(); it1!=h1DYcellChargeSecondHit_       .end(); it1++) delete *it1; h1DYcellChargeSecondHit_      .clear();
  for(it1=h1DYcellChargeSecondHitNorm_   .begin(); it1!=h1DYcellChargeSecondHitNorm_   .end(); it1++) delete *it1; h1DYcellChargeSecondHitNorm_  .clear();
  
  
  for(it2=h2DClusterSize_                .begin(); it2!=h2DClusterSize_                .end(); it2++) delete *it2; h2DClusterSize_               .clear();
  for(it2=h2DCellCharge_                 .begin(); it2!=h2DCellCharge_                 .end(); it2++) delete *it2; h2DCellCharge_                .clear();
  for(it2=h2DCellChargeNorm_             .begin(); it2!=h2DCellChargeNorm_             .end(); it2++) delete *it2; h2DCellChargeNorm_            .clear();
  
  for(it2=h4CellsCharge_                 .begin(); it2!=h4CellsCharge_                 .end(); it2++) delete *it2; h4CellsCharge_                .clear();
  for(it2=h4CellsChargeNorm_             .begin(); it2!=h4CellsChargeNorm_             .end(); it2++) delete *it2; h4CellsChargeNorm_            .clear();
  
  for(it2=h2DXcellCharge_                .begin(); it2!=h2DXcellCharge_                .end(); it2++) delete *it2; h2DXcellCharge_               .clear();
  for(it2=h2DXcellChargeSecondHit_       .begin(); it2!=h2DXcellChargeSecondHit_       .end(); it2++) delete *it2; h2DXcellChargeSecondHit_      .clear();
  
  for(it2=h2DYcellCharge_                .begin(); it2!=h2DYcellCharge_                .end(); it2++) delete *it2; h2DYcellCharge_               .clear();
  for(it2=h2DYcellChargeSecondHit_       .begin(); it2!=h2DYcellChargeSecondHit_       .end(); it2++) delete *it2; h2DYcellChargeSecondHit_      .clear();

  
  for(it2=h2DXcellChargeAsimmetry_       .begin(); it2!=h2DXcellChargeAsimmetry_       .end(); it2++) delete *it2; h2DXcellChargeAsimmetry_      .clear();
  for(it1=h1DXcellChargeAsimmetry_       .begin(); it1!=h1DXcellChargeAsimmetry_       .end(); it1++) delete *it1; h1DXcellChargeAsimmetry_      .clear();
  for(it2=h2DXcellChargeAsimmetryInv_    .begin(); it2!=h2DXcellChargeAsimmetryInv_    .end(); it2++) delete *it2; h2DXcellChargeAsimmetryInv_   .clear();
  for(it1=h1DXcellChargeAsimmetryInv_    .begin(); it1!=h1DXcellChargeAsimmetryInv_    .end(); it1++) delete *it1; h1DXcellChargeAsimmetryInv_   .clear();
  
  for(it2=h2DYcellChargeAsimmetry_       .begin(); it2!=h2DYcellChargeAsimmetry_       .end(); it2++) delete *it2; h2DYcellChargeAsimmetry_      .clear();
  for(it1=h1DYcellChargeAsimmetry_       .begin(); it1!=h1DYcellChargeAsimmetry_       .end(); it1++) delete *it1; h1DYcellChargeAsimmetry_      .clear();
  for(it2=h2DYcellChargeAsimmetryInv_    .begin(); it2!=h2DYcellChargeAsimmetryInv_    .end(); it2++) delete *it2; h2DYcellChargeAsimmetryInv_   .clear();
  for(it1=h1DYcellChargeAsimmetryInv_    .begin(); it1!=h1DYcellChargeAsimmetryInv_    .end(); it1++) delete *it1; h1DYcellChargeAsimmetryInv_   .clear();
}

//=======================================================================
int Charge::linearFit(TH1* histo, double *fitParameters)
{
  double fitRange    [2];
  double startValues [2];

  fitRange[0]    = 4000;
  fitRange[1]    = 10000;

  startValues[0] = 1000;
  startValues[1] = 3;

  linear_->SetRange(fitRange[0],fitRange[1]);
  linear_->SetParameters(startValues);

  TFitResultPtr r = histo->Fit(linear_,"QRBL");
  int fitStatus   = r;

  linear_->GetParameters(fitParameters);

  return fitStatus;
}

//=======================================================================
void Charge::setErrorsBar(int planeID)
{
  std::string       planeName = thePlaneMapping_->getPlaneName(planeID);
  std::stringstream hName;
  double            binError;
  int               nBinsX;




  theAnalysisManager_->cd("/Charge/" + planeName + "/XAsimmetry");

  hName.str(""); hName << "h1DXcellChargeAsimmetry_" << planeName;
  h1DXcellChargeAsimmetry_.push_back((TH1F*)h2DXcellChargeAsimmetry_[planeID]->ProfileX(hName.str().c_str(),1,-1));

  hName.str(""); hName << "h1DXcellChargeAsimmetryInv_" << planeName;
  h1DXcellChargeAsimmetryInv_.push_back((TH1F*)h2DXcellChargeAsimmetryInv_[planeID]->ProfileX(hName.str().c_str(),1,-1));


  theAnalysisManager_->cd("/Charge/" + planeName + "/YAsimmetry");

  hName.str(""); hName << "h1DYcellChargeAsimmetry_" << planeName;
  h1DYcellChargeAsimmetry_.push_back((TH1F*)h2DYcellChargeAsimmetry_[planeID]->ProfileX(hName.str().c_str(),1,-1));

  hName.str(""); hName << "h1DYcellChargeAsimmetryInv_" << planeName;
  h1DYcellChargeAsimmetryInv_.push_back((TH1F*)h2DYcellChargeAsimmetryInv_[planeID]->ProfileX(hName.str().c_str(),1,-1));




  theAnalysisManager_->cd("/Charge/" + planeName + "/XcellCharge1D");

  nBins = h2DXcellCharge_[planeID]->GetNbinsX();
  TH1D* hXchargeTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "Xcharge_Proj_bin_" << bX;
      hXchargeTmp[bX-1] = h2DXcellCharge_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeTmp[bX-1]->GetEntries() != 0)
	{
	  binError = hXchargeTmp[bX-1]->GetRMS() / sqrt(hXchargeTmp[bX-1]->GetEntries());
	  h1DXcellCharge_[planeID]->SetBinError(bX,binError);
	}
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeTmp[p]->Delete("0");

  nBins = h2DXcellChargeSecondHit_[planeID]->GetNbinsX();
  TH1D* hXchargeSecondHitTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "XchargeSecondHit_Proj_bin_" << bX;
      hXchargeSecondHitTmp[bX-1] = h2DXcellChargeSecondHit_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeSecondHitTmp[bX-1]->GetEntries() != 0)
	{
	  binError = hXchargeSecondHitTmp[bX-1]->GetRMS() / sqrt(hXchargeSecondHitTmp[bX-1]->GetEntries());
	  h1DXcellChargeSecondHit_[planeID]->SetBinError(bX,binError);
	}
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeSecondHitTmp[p]->Delete("0");




  theAnalysisManager_->cd("/Charge/" + planeName + "/YcellCharge1D");

  nBins = h2DYcellCharge_[planeID]->GetNbinsX();
  TH1D* hYchargeTmp[nBins];
  for (int bY = 1; bY <= nBins; bY++)
    {
      hName.str(""); hName << "Ycharge_Proj_bin_" << bY;
      hYchargeTmp[bY-1] = h2DYcellCharge_[planeID]->ProjectionY(hName.str().c_str(),bY,bY);

      if (hYchargeTmp[bY-1]->GetEntries() != 0)
	{
	  binError = hYchargeTmp[bY-1]->GetRMS() / sqrt(hYchargeTmp[bY-1]->GetEntries());
	  h1DYcellCharge_[planeID]->SetBinError(bY,binError);
	}
      else continue;
    }
  for (int p = 0; p < nBins; p++) hYchargeTmp[p]->Delete("0");

  nBins = h2DYcellChargeSecondHit_[planeID]->GetNbinsX();
  TH1D* hYchargeSecondHitTmp[nBins];
  for(int bY = 1; bY <= nBins; bY++)
    {
      hName.str(""); hName << "YchargeSecondHit_Proj_bin_" << bY;
      hYchargeSecondHitTmp[bY-1] = h2DYcellChargeSecondHit_[planeID]->ProjectionY(hName.str().c_str(),bY,bY);

      if (hYchargeSecondHitTmp[bY-1]->GetEntries() != 0)
	{
	  binError = hYchargeSecondHitTmp[bY-1]->GetRMS() / sqrt(hYchargeSecondHitTmp[bY-1]->GetEntries());
	  h1DYcellChargeSecondHit_[planeID]->SetBinError(bY,binError);
	}
      else continue;
    }
  for (int p = 0; p < nBins; p++) hYchargeSecondHitTmp[p]->Delete("0");
}

//=======================================================================
void Charge::clusterSize(int planeID, const Data& data, int threadNumber)
{
  if (data.getHasHit(planeID)) THREADED(hClusterSize_[planeID])->Fill(data.getClusterSize(planeID));
}

//=======================================================================
void Charge::cellLandau(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int clusterSize = 1;


  if (!pass || !data.getHasHit(planeID) || data.getClusterSize(planeID) != clusterSize) return;

  const Window* theWindow = theWindowsManager_->getWindow(planeID);
  if (theWindow->checkWindow(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber()) &&
      data.getIsPixelCalibrated(0,planeID) &&
      data.getClusterCharge(planeID) > standardCutsPixelMinimumCharge_)
    THREADED(hCellLandau_[planeID])->Fill(data.getClusterCharge(planeID));
}

//=======================================================================
void Charge::cellCharge(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getIsInDetector(planeID) )
        return;

    float maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())                                   ;
    float maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str())                                  ;

    if( data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY )
        return;

    float xPixelResidual = data.getXPixelResidualLocal(planeID);
    float yPixelResidual = data.getYPixelResidualLocal(planeID);
    float xPixelEdgeResidual = 0; //Residual from the edge of the pixel (the divide between 2 pixels)
    float yPixelEdgeResidual = 0; //Residual from the edge of the pixel (the divide between 2 pixels)

    //25x600
    float Q1 = 0;
    float Q2 = 0;
    float Q3 = 0;
    float Q4 = 0;

    //50x300
    float q1 = 0;
    float q2 = 0;


    if (xPixelResidual > 0)
      xPixelEdgeResidual = -data.getXPitchLocal(planeID)/2 + xPixelResidual;
    else if (xPixelResidual <= 0)
      xPixelEdgeResidual = xPixelResidual + data.getXPitchLocal(planeID)/2;

    if (yPixelResidual > 0)
      yPixelEdgeResidual = -data.getYPitchLocal(planeID)/2 + yPixelResidual;
    else if (yPixelResidual <= 0)
      yPixelEdgeResidual = yPixelResidual + data.getYPitchLocal(planeID)/2;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    THREADED(h2DAllTracks_      [planeID])->Fill(xPixelResidual,yPixelResidual);
    THREADED(h2DAllTracks4Rows_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID)); //((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID)) allows to have 4 rows of the 25 microns pitch, in order to compare with the standard pitch of 100 microns
    THREADED(h2DAllTracks2Rows_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID)); //((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID)) allows to have 2 rows of the 50 microns pitch, in order to compare with the standard pitch of 100 microns

    if (data.getClusterSize(planeID) == 1)
        THREADED(h2DCellChargeSize1AllTracksNorm_[planeID])->Fill(xPixelResidual, yPixelResidual);
    else if (data.getClusterSize(planeID) == 2)
        THREADED(h2DCellChargeSize2AllTracksNorm_[planeID])->Fill(xPixelResidual, yPixelResidual);

    THREADED(h4CellsAllTracks_[planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual);

    int size = data.getClusterSize(planeID); //ClusterSize

    //////////////////////////////////////////////////////////////////
    //FROM NOW ON ONLY TRACKS WITH A HIT ON THE PLANE ARE CONSIDERED//
    //////////////////////////////////////////////////////////////////
    if( !data.getHasHit(planeID) || size > 4 )
        return;

    for(int h=0; h<size; h++) //h<4
    {
        THREADED(hCutsControl_[planeID])->Fill(0);
        if (data.getIsPixelCalibrated (h,planeID))
        {
            THREADED(hCutsControl_[planeID])->Fill(1);
            if (data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col)
            {
                THREADED(hCutsControl_[planeID])->Fill(2);
                if (data.getClusterSize(planeID) < 2)
                {
                    THREADED(hCutsControl_[planeID])->Fill(3);
                }
            }
            else
            {
                THREADED(hCutsControl_[planeID])->Fill(4);
                if (data.getClusterSize(planeID) < 2)
                    THREADED(hCutsControl_[planeID])->Fill(5);
                if (size == 1)
                {
                    THREADED(hHitsNotOnRowCol_[planeID])->Fill(xPixelEdgeResidual, yPixelEdgeResidual);
                    if (yPixelEdgeResidual > 20 || yPixelEdgeResidual < -20)

                        THREADED(hHitsNotOnRowColProjX_[planeID])->Fill(xPixelEdgeResidual);
                    if (xPixelEdgeResidual > 30 || xPixelEdgeResidual < -30)

                        THREADED(hHitsNotOnRowColProjY_[planeID])->Fill(yPixelEdgeResidual);
                }
                THREADED(hHitsNotONRowColVsXSlope_[planeID])->Fill(xPixelEdgeResidual, data.getXSlopeUnconstrained(planeID));
                THREADED(hHitsNotONRowColVsYSlope_[planeID])->Fill(yPixelEdgeResidual, data.getYSlopeUnconstrained(planeID));
                THREADED(hChargeNotOnRowCol_[planeID])->Fill(data.getClusterPixelCharge(h, planeID));
            }
        }
    }


    int hitID = -1;
    //int l = 0;
    for (int h=0; h<size; h++)
      {
        if (data.getClusterPixelRow   (h,planeID) == row &&
	    data.getClusterPixelCol   (h,planeID) == col &&
	    data.getIsPixelCalibrated (h,planeID)        &&
	    data.getClusterPixelCharge(h,planeID) > standardCutsPixelMinimumCharge_ &&
	    data.getClusterPixelCharge(h,planeID) < standardCutsPixelMaximumCharge_)
	  {
	    // @@@ Please do not modify this code @@@
            THREADED(h4CellsCharge_           [planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual,data.getClusterPixelCharge(h,planeID));
            THREADED(h4CellsChargeNorm_       [planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual);
            THREADED(h2DCellCharge_           [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(h,planeID));
            THREADED(h2DCellChargeNorm_       [planeID])->Fill(xPixelResidual,yPixelResidual);
            THREADED(h2DClusterSize_          [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterSize(planeID));
	    // ======================================



            THREADED(h2DCellChargeNum_        [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(h,planeID));
            THREADED(h2DCellClusterCharge_    [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterCharge(planeID));
            THREADED(h1DPixelYTrackResiduals_     [planeID])->Fill(yPixelResidual,fabs(data.getYTrackResidualLocal(planeID)));
            THREADED(h1DPixelYTrackResidualsNorm_     [planeID])->Fill(yPixelResidual);

            /*-------------------------------------------------/////   2 &  4 ROWS HISTOGRAMS     ////------------------------------------------------------------------------------------------------------------------------------------------*/

            THREADED(h2DCellCharge4RowsNorm_        [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
            THREADED(h2DCellPixelCharge4Rows_       [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
            THREADED(h2DCellClusterCharge4Rows_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterCharge(planeID));
            THREADED(h2DClusterSize4Rows_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterSize(planeID));

            THREADED(h2DCellCharge2RowsNorm_        [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));
            THREADED(h2DCellPixelCharge2Rows_       [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
            THREADED(h2DCellClusterCharge2Rows_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterCharge(planeID));
            THREADED(h2DClusterSize2Rows_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterSize(planeID));

            THREADED(h1DPixelYTrackResiduals4Rows_[planeID])->Fill(yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),fabs(data.getYTrackResidualLocal(planeID)));
            THREADED(h1DPixelYTrackResiduals2Rows_[planeID])->Fill(yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),fabs(data.getYTrackResidualLocal(planeID)));

            THREADED(h1DPixelYTrackResiduals4RowsNorm_[planeID])->Fill(yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
            THREADED(h1DPixelYTRackResiduals2RowsNorm_[planeID])->Fill(yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));

            if(size == 1)
            {
                THREADED(h2DCellPixelCharge4RowsClusterSize1_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DCellPixelCharge4RowsClusterSize1Norm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                THREADED(h2DCellPixelCharge2RowsClusterSize1_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DCellPixelCharge2RowsClusterSize1Norm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));

                if(row != 50) // 4Rows - cluster of size one mostly come from this row that is over the last 50x300 row
                {
                    THREADED(h2DCellPixelCharge4RowsExcept50ClusterSize1_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                    THREADED(h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                }

            }



            else if(size == 2)
            {
                THREADED(h2DCellPixelCharge4RowsClusterSize2_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DClusterCharge4RowsClusterSize2_       [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterCharge(planeID));
                THREADED(h2DCellPixelCharge4RowsClusterSize2Norm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                THREADED(h2DCellPixelCharge2RowsClusterSize2_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DCellPixelCharge2RowsClusterSize2Norm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));

                for (int g=0; g<size; g++)
                {

                    if ( data.getClusterPixelCol(g,planeID) == col)
                    {

                        //25x600
                        if((data.getClusterPixelRow(g,planeID)-50)%4 == 0)
                        {
                            Q1 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 1)
                        {
                            Q2 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 2)
                        {
                            Q3 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 3)
                        {
                            Q4 = data.getClusterPixelCharge(g,planeID);
                        }

                        //50x300
                        if((data.getClusterPixelRow(g,planeID)-20)%2 == 0)
                        {
                            q1 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-20)%2 == 1)
                        {
                            q2 = data.getClusterPixelCharge(g,planeID);
                        }
                    }
                }

                //plots of the correlations among the charges in different rows
                THREADED(h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_ [planeID])->Fill(Q2,Q1);
                THREADED(h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_ [planeID])->Fill(Q2,Q3);
                THREADED(h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_ [planeID])->Fill(Q3,Q4);

                THREADED(h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_ [planeID])->Fill(q2,q1);

                if(    data.getClusterPixelRow      (h,planeID) == row
                       && data.getClusterPixelCol   (h,planeID) == col
                       //&& data.getIsPixelCalibrated (h,planeID)
                       //&& data.getClusterPixelCharge(h,planeID) > standardCutsPixelMinimumCharge_
                       //&& data.getClusterPixelCharge(h,planeID) < standardCutsPixelMaximumCharge_
                       )
                {
                    //selection of cluster size 2 that have a hit ina range of 10 microns around the edge between the first and second row of 4 rows (row 50 starts at 25x50=1250 microns using this configuration
                    if(((data.getClusterPixelRow(h,planeID)-50)%4 == 0 && data.getYPredictedLocal(planeID)>1265+(row-50)*data.getYPitchLocal(planeID) && data.getYPredictedLocal(planeID)<1285+(row-50)*data.getYPitchLocal(planeID)) ||
                            ((data.getClusterPixelRow(h,planeID)-50)%4 == 1 && data.getYPredictedLocal(planeID)>1265+((row-50)-1)*data.getYPitchLocal(planeID) && data.getYPredictedLocal(planeID)<1285+((row-50)+3)*data.getYPitchLocal(planeID)))
                    {
                        THREADED(h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_     [planeID])->Fill(Q1,Q2);
                        THREADED(h2DPixelCharge4RowsClusterSize2BetweenRow1vsRows1And2_[planeID])->Fill(Q1+Q2,Q1);
                        THREADED(h2DPixelCharge4RowsClusterSize2BetweenRow2vsRows1And2_[planeID])->Fill(Q1+Q2,Q2);
                    }
                    //only row one
                    if(((data.getClusterPixelRow(h,planeID)-50)%4 == 0 && data.getYPredictedLocal(planeID)>1265+(row-50)*data.getYPitchLocal(planeID) && data.getYPredictedLocal(planeID)<1285+(row-50)*data.getYPitchLocal(planeID)))
                        THREADED(h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_ [planeID])->Fill(Q1,Q2);
                    //only row two
                    if(((data.getClusterPixelRow(h,planeID)-50)%4 == 1 && data.getYPredictedLocal(planeID)>1265+((row-50)-1)*data.getYPitchLocal(planeID) && data.getYPredictedLocal(planeID)<1285+((row-50)+3)*data.getYPitchLocal(planeID)))
                        THREADED(h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_ [planeID])->Fill(Q1,Q2);
                    //selection of cluster size 2 that have a hit ina range of 10 microns around the edge between the third and fourth row of 4 rows (row 50 starts at 25x50=1250 microns using this configuration
                    if(((data.getClusterPixelRow(h,planeID)-50)%4 == 2 && data.getYPredictedLocal(planeID)>1315+((row-50)-2)*data.getYPitchLocal(planeID) && data.getYPredictedLocal(planeID)<1335+((row-50)-1)*data.getYPitchLocal(planeID)) ||
                            ((data.getClusterPixelRow(h,planeID)-50)%4 == 3 && data.getYPredictedLocal(planeID)>1315+((row-50)-3)*data.getYPitchLocal(planeID) && data.getYPredictedLocal(planeID)<1335+((row-50)+1)*data.getYPitchLocal(planeID)))
                    {
                        THREADED(h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_     [planeID])->Fill(Q4,Q3);
                        THREADED(h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_[planeID])->Fill(Q3+Q4,Q4);
                        THREADED(h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_[planeID])->Fill(Q3+Q4,Q3);

                    }
                    //only row 4
                    if(((data.getClusterPixelRow(h,planeID)-50)%4 == 2 && data.getYPredictedLocal(planeID)>1315+((row-50)-2)*data.getYPitchLocal(planeID) && data.getYPredictedLocal(planeID)<1335+((row-50)-1)*data.getYPitchLocal(planeID)))
                        THREADED(h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_ [planeID])->Fill(Q4,Q3);
                    //only row 3
                    if(((data.getClusterPixelRow(h,planeID)-50)%4 == 3 && data.getYPredictedLocal(planeID)>1315+((row-50)-3)*data.getYPitchLocal(planeID) && data.getYPredictedLocal(planeID)<1335+((row-50)+1)*data.getYPitchLocal(planeID)))
                        THREADED(h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_ [planeID])->Fill(Q4,Q3);

                    //hits in row 2 or 3 (internal rows)
                    if((data.getClusterPixelRow(h,planeID)-50)%4 == 2 || (data.getClusterPixelRow(h,planeID)-50)%4 == 1 )
                    {
                        THREADED(h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_ [planeID])->Fill(Q1+Q4,Q2+Q3);
                    }
                    //hits in row 1 or 4 (external rows)
                    else if((data.getClusterPixelRow(h,planeID)-50)%4 == 0 || (data.getClusterPixelRow(h,planeID)-50)%4 == 3 )
                    {
                        THREADED(h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_ [planeID])->Fill(Q2+Q3,Q1+Q4);
                    }

                }

                for (int g=0; g<size; g++)
                {
                    if (g != h
                            //&& data.getIsPixelCalibrated (g,planeID)
                            //&& data.getClusterPixelCharge(g,planeID) > standardCutsPixelMinimumCharge_
                            //&& data.getClusterPixelCharge(g,planeID) < standardCutsPixelMaximumCharge_
                            )
                    {
                        if  (data.getClusterPixelCol(g,planeID) == col)
                        {
                            THREADED(h2DCellChargeSecondHit4RowsSameCol_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                            THREADED(h2DCellChargeSecondHit4RowsSameColNorm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                            THREADED(h2DCellChargeSecondHit2RowsSameCol_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                            THREADED(h2DCellChargeSecondHit2RowsSameColNorm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));

                        }

                    }

                }
            }


            else if(size == 3)
            {
                THREADED(h2DCellPixelCharge4RowsClusterSize3_      [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DClusterCharge4RowsClusterSize3_        [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterCharge(planeID));
                THREADED(h2DCellPixelCharge4RowsClusterSize3Norm_  [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                THREADED(h2DCellPixelCharge2RowsClusterSize3_      [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DCellPixelCharge2RowsClusterSize3Norm_  [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));
                for (int g=0; g<size; g++)
                {

                    if ( data.getClusterPixelCol(g,planeID) == col)
                    {
                        if((data.getClusterPixelRow(g,planeID)-50)%4 == 0)
                        {
                            Q1 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 1)
                        {
                            Q2 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 2)
                        {
                            Q3 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 3)
                        {
                            Q4 = data.getClusterPixelCharge(g,planeID);
                        }
                    }
                }

                THREADED(h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_ [planeID])->Fill(Q2,Q1);
                THREADED(h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_ [planeID])->Fill(Q2,Q3);
                THREADED(h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_ [planeID])->Fill(Q3,Q4);

                if(    data.getClusterPixelRow      (h,planeID) == row
                       && data.getClusterPixelCol   (h,planeID) == col
                       //&& data.getIsPixelCalibrated (h,planeID)
                       //&& data.getClusterPixelCharge(h,planeID) > standardCutsPixelMinimumCharge_
                       //&& data.getClusterPixelCharge(h,planeID) < standardCutsPixelMaximumCharge_
                       )
                {
                    if((data.getClusterPixelRow(h,planeID)-50)%4 == 2 || (data.getClusterPixelRow(h,planeID)-50)%4 == 1 )
                    {
                        THREADED(h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_ [planeID])->Fill(Q1+Q4,Q2+Q3);
                    }
                    else if((data.getClusterPixelRow(h,planeID)-50)%4 == 0 || (data.getClusterPixelRow(h,planeID)-50)%4 == 3 )
                    {
                        THREADED(h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_ [planeID])->Fill(Q2+Q3,Q1+Q4);
                    }
                }
                for (int g=0; g<size; g++)
                {

                    if (g != h) //&& data.getIsPixelCalibrated (g,planeID)//&& data.getClusterPixelCharge(g,planeID) > standardCutsPixelMinimumCharge_//&& data.getClusterPixelCharge(g,planeID) < standardCutsPixelMaximumCharge_


                        if ( data.getClusterPixelCol(g,planeID) == col)
                        {
                            THREADED(h2DCellChargeSecondHit4RowsClusterSize3_    [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                            THREADED(h2DCellChargeSecondHit4RowsClusterSize3Norm_[planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                            THREADED(h2DCellChargeSecondHit2RowsClusterSize3_    [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                            THREADED(h2DCellChargeSecondHit2RowsClusterSize3Norm_[planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));


                            if(data.getClusterPixelCharge(g,planeID)<data.getClusterPixelCharge(h,planeID))

                            {

                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowCharge_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighCharge_    [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_[planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                                THREADED(h2DCellChargeSecondHit2RowsClusterSize3LowCharge_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                                THREADED(h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));
                                THREADED(h2DCellChargeSecondHit2RowsClusterSize3HighCharge_    [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                                THREADED(h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_[planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));

                            }


                            else if (data.getClusterPixelCharge(h,planeID)<data.getClusterPixelCharge(g,planeID))
                            {

                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowCharge_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighCharge_    [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_[planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                                THREADED(h2DCellChargeSecondHit2RowsClusterSize3LowCharge_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                                THREADED(h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));
                                THREADED(h2DCellChargeSecondHit2RowsClusterSize3HighCharge_    [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                                THREADED(h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_[planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));

                            }
                        }
                }
            }

            else if(size == 4)
            {
                THREADED(h2DCellPixelCharge4RowsClusterSize4_      [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DClusterCharge4RowsClusterSize4_        [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterCharge(planeID));
                THREADED(h2DCellPixelCharge4RowsClusterSize4Norm_  [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                THREADED(h2DCellPixelCharge2RowsClusterSize4_      [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DCellPixelCharge2RowsClusterSize4Norm_  [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-20)%2)-0.5)*data.getYPitchLocal(planeID));

                for (int g=0; g<size; g++)
                {

                    if ( data.getClusterPixelCol(g,planeID) == col)
                    {
                        if((data.getClusterPixelRow(g,planeID)-50)%4 == 0)
                        {
                            Q1 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 1)
                        {
                            Q2 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 2)
                        {
                            Q3 = data.getClusterPixelCharge(g,planeID);
                        }
                        else if((data.getClusterPixelRow(g,planeID)-50)%4 == 3)
                        {
                            Q4 = data.getClusterPixelCharge(g,planeID);
                        }
                    }
                }

                THREADED(h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_ [planeID])->Fill(Q2,Q1);
                THREADED(h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_ [planeID])->Fill(Q2,Q3);
                THREADED(h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_ [planeID])->Fill(Q3,Q4);

                if(    data.getClusterPixelRow      (h,planeID) == row
                       && data.getClusterPixelCol   (h,planeID) == col
                       //&& data.getIsPixelCalibrated (h,planeID)
                       //&& data.getClusterPixelCharge(h,planeID) > standardCutsPixelMinimumCharge_
                       //&& data.getClusterPixelCharge(h,planeID) < standardCutsPixelMaximumCharge_
                       )
                {
                    if((data.getClusterPixelRow(h,planeID)-50)%4 == 2 || (data.getClusterPixelRow(h,planeID)-50)%4 == 1 )
                    {
                        THREADED(h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_ [planeID])->Fill(Q1+Q4,Q2+Q3);
                    }
                    else if((data.getClusterPixelRow(h,planeID)-50)%4 == 0 || (data.getClusterPixelRow(h,planeID)-50)%4 == 3 )
                    {
                        THREADED(h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_ [planeID])->Fill(Q2+Q3,Q1+Q4);
                    }
                }

            }



            /*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
            THREADED(hCellChargeCoarse_      [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(h,planeID));
            THREADED(hCellChargeCoarseNorm_  [planeID])->Fill(xPixelResidual,yPixelResidual);
            TH2F* histo = THREADED(hCellChargeCoarse_[planeID]);
            int histoN = (histo->GetXaxis()->FindBin(xPixelResidual)-1)*histo->GetNbinsY() + histo->GetYaxis()->FindBin(yPixelResidual)-1;

            if ((unsigned int)histoN < hCellChargeCoarseLandau_[planeID].size())
                THREADED(hCellChargeCoarseLandau_[planeID][histoN])->Fill(data.getClusterPixelCharge(h,planeID)); //DOESN'T ALWAYS WORK...

            hitID = h;

            break;
        }

    }


    if (hitID == -1 && size <= 2)
    {
        for (int l =0; l<size; ++l)
        {
            if (data.getClusterPixelCharge(l,planeID) < standardCutsPixelMinimumCharge_ || data.getClusterPixelCharge(l,planeID) > standardCutsPixelMaximumCharge_ || !data.getIsPixelCalibrated(l,planeID))
                continue;

            if (xPixelEdgeResidual <= 0 && data.getClusterPixelCol(l,planeID) - col == 1 && data.getClusterPixelRow(l,planeID) == row )
            {
                THREADED(h2DCellChargeSecondHit_[planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(l,planeID));
            }
            else if (xPixelEdgeResidual > 0 && data.getClusterPixelCol(l,planeID) - col == -1 && data.getClusterPixelRow(l,planeID) == row )
            {
                THREADED(h2DCellChargeSecondHit_[planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(l,planeID));
            }
            if (yPixelEdgeResidual <= 0 && data.getClusterPixelRow(l,planeID) - row == 1 && data.getClusterPixelCol(l,planeID) == col )
            {
                THREADED(h2DCellChargeSecondHit_[planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(l,planeID));
            }
            else if (yPixelEdgeResidual > 0 && data.getClusterPixelRow(l,planeID) - row == -1 && data.getClusterPixelCol(l,planeID) == col )
            {
                THREADED(h2DCellChargeSecondHit_[planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(l,planeID));
            }
        }
    }

    if( size == 4 )
    {

        for(int h=0; h<size; ++h)
        {
            if( !theWindow->checkWindow( data.getClusterPixelCol(h,planeID) , data.getClusterPixelRow(h,planeID),run ) ||
                    !data.getIsPixelCalibrated(h,planeID) ||
                    data.getClusterPixelCharge(h,planeID) < standardCutsPixelMinimumCharge_ ||
                    data.getClusterPixelCharge(h,planeID) > standardCutsPixelMaximumCharge_ ) return;
        }
        bool isPredictedIn = false;
        int  firstHit;
        for(int h=0; h<size; ++h)
        {
            if(data.getClusterPixelCol(h,planeID) == col && data.getClusterPixelRow(h,planeID) == row)
            {
                isPredictedIn = true;
                firstHit = h;
                break;
            }
        }

        if(!isPredictedIn)
            return;

        bool isXmin = false;
        bool isXmax = false;
        bool isYmin = false;
        bool isYmax = false;
        for(int h=0; h<size; ++h)
        {
            if(h == firstHit)
                continue;

            if( data.getXClusterPixelCenterLocal(firstHit,planeID) > data.getXClusterPixelCenterLocal(h,planeID) )
            {
                isXmin = false;
                isXmax = true;
            }
            if( data.getXClusterPixelCenterLocal(firstHit,planeID) < data.getXClusterPixelCenterLocal(h,planeID) )
            {
                isXmin = true;
                isXmax = false;
            }
            if( data.getYClusterPixelCenterLocal(firstHit,planeID) > data.getYClusterPixelCenterLocal(h,planeID) )
            {
                isYmin = false;
                isYmax = true;
            }
            if( data.getYClusterPixelCenterLocal(firstHit,planeID) < data.getYClusterPixelCenterLocal(h,planeID) )
            {
                isYmin = true;
                isYmax = false;
            }
        }

        int secondHit = 0;
        int thirdHit  = 0;
        for(int h=0; h<size; ++h)
        {
            if( h != firstHit && row == data.getClusterPixelRow(h,planeID) &&
                    (col - data.getClusterPixelCol(h,planeID)) != 0 &&
                    abs(col - data.getClusterPixelCol(h,planeID)) <= 1 )
            {
                secondHit = h;
                break;
            }
        }
        for(int h=0; h<size; ++h)
        {
            if( h != firstHit && h!= secondHit && col == data.getClusterPixelCol(h,planeID) &&
                    (row - data.getClusterPixelRow(h,planeID)) != 0 &&
                    abs(row - data.getClusterPixelRow(h,planeID)) <= 1 )
            {
                thirdHit = h;
                break;
            }
        }

        for(int h=0; h<size; ++h)
        {
            if( h != firstHit && h != secondHit && h != thirdHit &&
                    data.getClusterPixelRow(h,planeID) == data.getClusterPixelRow(thirdHit,planeID) &&
                    data.getClusterPixelCol(h,planeID) == data.getClusterPixelCol(secondHit,planeID))
            {
                if(isYmax && isXmin)
                {
                    xPixelEdgeResidual = +xPixelResidual + data.getXPitchLocal(planeID)/2;
                    yPixelEdgeResidual = +yPixelResidual - data.getYPitchLocal(planeID)/2;
                }
                else if(isYmin && isXmin)
                {
                    xPixelEdgeResidual = +xPixelResidual + data.getXPitchLocal(planeID)/2;
                    yPixelEdgeResidual = +yPixelResidual + data.getYPitchLocal(planeID)/2;
                }
                else if(isYmax && isXmax)
                {
                    xPixelEdgeResidual = +xPixelResidual - data.getXPitchLocal(planeID)/2;
                    yPixelEdgeResidual = +yPixelResidual - data.getYPitchLocal(planeID)/2;
                }
                else if(isYmin && isXmax)
                {
                    xPixelEdgeResidual = +xPixelResidual - data.getXPitchLocal(planeID)/2;
                    yPixelEdgeResidual = +yPixelResidual + data.getYPitchLocal(planeID)/2;
                }

                THREADED(h4Hits_      [planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual);
                THREADED(h4HitsCharge_[planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual,data.getClusterPixelCharge(firstHit,planeID));
                break;
            }
        }

    }
}

//=======================================================================
void Charge::xChargeDivision(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float xRes = 0;
  float yRes = 0;
  float maxPitchX;
  float maxPitchY;


  if (!pass || !data.getIsInDetector(planeID)) return;

  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY) return;

  if (data.getXPixelResidualLocal(planeID) > 0)       xRes = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
  else if (data.getXPixelResidualLocal(planeID) <= 0) xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);

  if (data.getYPixelResidualLocal(planeID) > 0)       yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
  else if (data.getYPixelResidualLocal(planeID) <= 0) yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);


  const Window* theWindow = theWindowsManager_->getWindow(planeID);
  int           row       = data.getRowPredicted(planeID);
  int           col       = data.getColPredicted(planeID);
  int           run       = data.getRunNumber();

  if (!theWindow->checkWindow(col,row,run)) return;


  bool myReturn = false;
  if ((theWindow->checkWindowAbout(col,row,run,thePlaneMapping_->getPlaneType(planeID)) == true) && (data.getClusterSize(planeID) <= maxClusterSize))
    {
      for (int h = 0; h < data.getClusterSize(planeID); h++)
	{
	  if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	      || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	      ||  data.getClusterPixelRow    (h,planeID) != row                                                  // Hits are on the same row (sharing is along the row - x direction)
	      ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
	      ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
	    {
	      myReturn = true;
	      break;
	    }
	}

      THREADED(h1DXcellChargeNorm_         [planeID])->Fill(xRes);
      THREADED(h1DXcellChargeSecondHitNorm_[planeID])->Fill(xRes);

      if ((data.getHasHit(planeID) == true) && (myReturn == false))
	{
	  for (int h = 0; h < data.getClusterSize(planeID); h++)
	    {
	      if (data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col)
		{
		  THREADED(h2DXcellCharge_          [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h2DXcellChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DXcellCharge_          [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DXcellChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
		  break;
		}		
	    }
	  
	  for (int h = 0; h < data.getClusterSize(planeID); h++)
	    {
	      if (xRes <= 0 && data.getClusterPixelCol(h,planeID) - col == 1 && data.getClusterPixelRow(h,planeID) == row)
		{
		  THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h, planeID));
		  break;
		}
	      else if (xRes > 0 && data.getClusterPixelCol(h,planeID) - col == -1 && data.getClusterPixelRow(h,planeID) == row)
		{
		  THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h, planeID));
		  break;
		}
	    }
	}
    }
}

//=======================================================================
void Charge::xAsimmetry(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  int size;
  float maxPitchX;
  float xPixelResidual;
  float yPixelResidual;


  if (!pass || !data.getIsInDetector(planeID)|| !data.getHasHit(planeID)) return;
  
  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX) return;
  
  size = data.getClusterSize(planeID);
  if (size > maxClusterSize) return;
  
  
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;
  
  
  for (int h = 0; h < size; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	  || !data.getIsPixelCalibrated(h,planeID)                                                           // Pixels are calibrated
	  ||  data.getClusterPixelRow    (h,planeID) != rowPredicted                                         // Hits are on the same row (sharing is along the row - x direction)
	  ||  data.getClusterPixelCharge (h,planeID) <  standardCutsPixelMinimumCharge_                      // Charge is over threshold
	  ||  data.getClusterPixelCharge (h,planeID) >  standardCutsPixelMaximumCharge_)                     // Maximum allowed charge for this physics
	return;
    }
  
  
  xPixelResidual = data.getXPixelResidualLocal(planeID);
  yPixelResidual = data.getYPixelResidualLocal(planeID);
  
  
  if (size == 2)
    {
      float asimmetry   = 0;
      int   totalCharge = 0;
      int   chargeLeft  = 0;
      int   chargeRight = 0;
      float xPredicted  = data.getXPredictedLocal(planeID);
      float xMeasured   = (data.getXClusterPixelCenterLocal(0, planeID) + data.getXClusterPixelCenterLocal(1, planeID))/2;
      float xResidual   = xPredicted - xMeasured;
      
      if (data.getXClusterPixelCenterLocal(0, planeID)>data.getXClusterPixelCenterLocal(1, planeID))
	{
	  chargeRight = data.getClusterPixelCharge(0, planeID);
	  chargeLeft  = data.getClusterPixelCharge(1, planeID);
	}
      else if (data.getXClusterPixelCenterLocal(0, planeID)<data.getXClusterPixelCenterLocal(1, planeID))
	{
	  chargeRight = data.getClusterPixelCharge(1, planeID);
	  chargeLeft  = data.getClusterPixelCharge(0, planeID);
	}
      
      totalCharge = chargeLeft + chargeRight;
      
      if (totalCharge > 0 && totalCharge < maxChargeDeltaRay)
	{
	  asimmetry = (float)(chargeLeft - chargeRight) / (float)totalCharge;
	  
	  if (totalCharge >= standardCutsClusterMinimumCharge_ && totalCharge <= standardCutsClusterMaximumCharge_)
	    {
	      THREADED(h2DXcellChargeAsimmetry_   [planeID])->Fill(xResidual, asimmetry);
	      THREADED(h2DXcellChargeAsimmetryInv_[planeID])->Fill(asimmetry, xResidual);
	    }
	}
    }
}

//=======================================================================
  void Charge::yChargeDivision(bool pass, int planeID, const Data& data, int threadNumber)
  {
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float xRes = 0;
  float yRes = 0;
  float maxPitchX;
  float maxPitchY;


  if (!pass || !data.getIsInDetector(planeID)) return;
  
  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY) return;
    
  if (data.getXPixelResidualLocal(planeID) > 0)      xRes = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
  else if(data.getXPixelResidualLocal(planeID) <= 0) xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);
  
  if (data.getYPixelResidualLocal(planeID) > 0)       yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
  else if (data.getYPixelResidualLocal(planeID) <= 0) yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);


  const Window* theWindow = theWindowsManager_->getWindow(planeID);
  int           row       = data.getRowPredicted(planeID);
  int           col       = data.getColPredicted(planeID);
  int           run       = data.getRunNumber();
  
  if (!theWindow->checkWindow(col,row,run)) return;


  bool myReturn = false;
  if ((theWindow->checkWindowAbout(col,row,run,thePlaneMapping_->getPlaneType(planeID)) == true) && (data.getClusterSize(planeID) <= maxClusterSize))
    {
      for (int h = 0; h < data.getClusterSize(planeID); h++)
	{
	  if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	      || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	      ||  data.getClusterPixelCol    (h,planeID) != col                                                  // Hits are on the same column (sharing is along the column - y direction)
	      ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
	      ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
	    {
	      myReturn = true;
	      break;
	    }
	}
      
      THREADED(h1DYcellChargeNorm_         [planeID])->Fill(yRes);
      THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(yRes);
	
      if ((data.getHasHit(planeID) == true) && (myReturn == false))
	{
	  for (int h = 0; h < data.getClusterSize(planeID); h++)
	    {
	      if (data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col)
		{
		  THREADED(h2DYcellCharge_          [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h2DYcellChargeSecondHit_ [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DYcellCharge_          [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DYcellChargeSecondHit_ [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
		  break;
		}
	    }
	  
	  for (int h = 0; h < data.getClusterSize(planeID); h++)
	    {
	      if (yRes <= 0 && data.getClusterPixelRow(h,planeID) - row == 1 && data.getClusterPixelCol(h,planeID) == col)
		{
		  THREADED(h2DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h, planeID));
		  break;
		}
	      else if (yRes > 0 && data.getClusterPixelRow(h,planeID) - row == -1 && data.getClusterPixelCol(h,planeID) == col)
		{
		  THREADED(h2DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h, planeID));
		  break;
		}
	    }
	}
    }
}

//=======================================================================
void Charge::yAsimmetry(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  int size;
  float maxPitchY;
  float xPixelResidual;
  float yPixelResidual;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID)) return;

  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getYPitchLocal(planeID) > maxPitchY) return;
  
  size = data.getClusterSize(planeID);
  if (size > maxClusterSize) return;


  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;



  for (int h = 0; h < size; h++)
    {
      if (!data.getIsPixelCalibrated(h,planeID)                                          // Pixels are calibrated
	  ||  data.getClusterPixelCol    (h,planeID) != colPredicted                     // Hits are on the same column (sharing is along the column - y direction)
	  ||  data.getClusterPixelCharge (h,planeID) <  standardCutsPixelMinimumCharge_  // Charge is over threshold
	  ||  data.getClusterPixelCharge (h,planeID) >  standardCutsPixelMaximumCharge_) // Maximum allowed charge for this physics
	return;
    }


  xPixelResidual = data.getXPixelResidualLocal(planeID);
  yPixelResidual = data.getYPixelResidualLocal(planeID);


  if (size == 2)
    {
      float asimmetry   = 0;
      int   totalCharge = 0;
      int   chargeDown  = 0;
      int   chargeUp    = 0;
      float yPredicted  = data.getYPredictedLocal(planeID);
      float yMeasured   = (data.getYClusterPixelCenterLocal(0, planeID) + data.getYClusterPixelCenterLocal(1, planeID))/2;
      float yResidual   = yPredicted - yMeasured;
      
      if (data.getYClusterPixelCenterLocal(0, planeID)>data.getYClusterPixelCenterLocal(1, planeID))
	{
	  chargeUp   = data.getClusterPixelCharge(0, planeID);
	  chargeDown = data.getClusterPixelCharge(1, planeID);
	}
      else if (data.getYClusterPixelCenterLocal(0, planeID)<data.getYClusterPixelCenterLocal(1, planeID))
	{
	  chargeUp   = data.getClusterPixelCharge(1, planeID);
	  chargeDown = data.getClusterPixelCharge(0, planeID);
	}

      totalCharge = chargeDown + chargeUp;

      if (totalCharge > 0 && totalCharge < maxChargeDeltaRay)
	{
	  asimmetry = (float)(chargeDown - chargeUp) / (float)totalCharge;
	  
	  if (totalCharge >= standardCutsClusterMinimumCharge_ && totalCharge <= standardCutsClusterMaximumCharge_)
	  {
            THREADED(h2DYcellChargeAsimmetry_   [planeID])->Fill(yResidual, asimmetry);
            THREADED(h2DYcellChargeAsimmetryInv_[planeID])->Fill(asimmetry, yResidual);
	  }
	}
    }
}

//=======================================================================
void Charge::setCutsFormula(std::map<std::string,std::string> cutsList,std::vector<TTree*> tree)
{
  std::vector<TTreeFormula*> formulasVector;
  
  for (std::map<std::string,std::string>::iterator it = cutsList.begin(); it != cutsList.end(); it++)
    {
      if ((it->first) == "main cut" && (it->second).size() == 0)
	STDLINE("WARNING: no main cut set in charge analysis ! Default value = true !", ACRed);
      
      formulasVector.clear();
      if ((it->second).size() != 0)
        {
	  for (unsigned int t = 0; t < tree.size(); t++)
	    formulasVector.push_back(new TTreeFormula((it->second).c_str(),(it->second).c_str(),tree[t]));
	  cutsFormulas_[it->first] = formulasVector;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::setParsLimits(void)
{
  std::stringstream ss;

  for (unsigned int pl = 0; pl < thePlaneMapping_->getNumberOfPlanes(); pl++)
    {
      if (thePlaneMapping_->getPlaneName(pl).find("Dut") != std::string::npos
	  && (theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(pl)]->useCalibrations())
        {
	  for (int p = 0; p < 4; p++)
            {
	      parMin_      [p] = theXmlParser_->getAnalysesFromString("Charge")->getParLimits(p,true).first;
	      parMax_      [p] = theXmlParser_->getAnalysesFromString("Charge")->getParLimits(p,true).second;
	      isMinToLimit_[p] = theXmlParser_->getAnalysesFromString("Charge")->isParToLimit(p).first;
	      isMaxToLimit_[p] = theXmlParser_->getAnalysesFromString("Charge")->isParToLimit(p).second;
	      h2DparsPlots_[p] = theCalibrationsManager_->getParHisto(p);

	      ss.str("");
	      ss << "Par" << p << "\t min: " << parMin_[p] << " - limit: " << (int)isMinToLimit_[p];
	      ss << " - max: " << parMax_[p] << " - limit: " << (int)isMaxToLimit_[p];
	      STDLINE(ss.str(),ACWhite);
            }
	  break;
        }
      else continue;
    }
}

//=======================================================================
bool Charge::passCalibrationsCut(int planeID, const Data &data)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  int    row;
  int    col;
  double parValue;
  bool   pass = false;


  if (!(theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->useCalibrations()) return true;
  if (data.getClusterSize(planeID) > maxClusterSize)                                             return false;


  for (int h = 0; h < data.getClusterSize(planeID); h++)
    {
      row = data.getClusterPixelRow(h,planeID);
      col = data.getClusterPixelCol(h,planeID);
      
      for (int p = 0; p < 4; p++)
        {
	  parValue = h2DparsPlots_[p]->GetBinContent(h2DparsPlots_[p]->GetXaxis()->FindBin(col),h2DparsPlots_[p]->GetYaxis()->FindBin(row));

	  if (isMinToLimit_[p] == false && isMaxToLimit_[p] == false) continue;
	  else if(isMinToLimit_[p] == true && isMaxToLimit_[p] == true)
            {
	      if (parValue > parMin_[p] && parValue<parMax_[p]) pass = true;
            }
	  else if (isMinToLimit_[p] == true && isMaxToLimit_[p] == false)
	    {
	      if (parValue > parMin_[p]) pass = true;
            }
	  else if(isMinToLimit_[p] == false && isMaxToLimit_[p] == true)
            {
	      if(parValue < parMax_[p]) pass = true;
            }
        }
    }

  return pass;
}

//=======================================================================
bool Charge::passBadPlanesCut(int planeID, const Data &data)
{
  int badPlanesCut = theXmlParser_->getAnalysesFromString("Charge")->getBadPlanesCut();

  int maxNumberOfEvents = 0;
  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes() - 2; p++) // -2 is to exclude DUTs
    {
      HistogramWindow* aWindow = (HistogramWindow*)theAnalysisManager_->getWindowsManager()->getWindow(p);
      if (aWindow->getNumberOfEvents() > maxNumberOfEvents) maxNumberOfEvents = aWindow->getNumberOfEvents();
    }

  int minHits   = 7;
  int excludeMe = 0;
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos)
    minHits = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str());
  else if (data.getHasHit(planeID))
    {
      if (data.getClusterSize(planeID) == 1) excludeMe = 1;
      else if (data.getClusterSize(planeID) == 2 && (data.getClusterPixelRow(0,planeID) == data.getClusterPixelRow(1,planeID)
						     || data.getClusterPixelCol(0,planeID) == data.getClusterPixelCol(1,planeID)))
	excludeMe = 1;
    }

  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes() - 2; p++) // -2 is to exclude DUTs
    {
      HistogramWindow* aWindow = (HistogramWindow*)theAnalysisManager_->getWindowsManager()->getWindow(p);
      if (!data.getHasHit(p) && (float)aWindow->getNumberOfEvents() < (float)maxNumberOfEvents*badPlanesCut / 100) excludeMe += 1;
    }
  
  if (data.getNumberOfTelescopeHits() - excludeMe >= minHits) return true;
  else                                                        return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::normalizeEtaDistributionSize2 (int p)
{
    std::stringstream hName;
    std::string planeName = thePlaneMapping_->getPlaneName(p);

    float xCellPitch = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
    float yCellPitch = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

    theAnalysisManager_->cd("/Charge/" + planeName + "/XcellCharge1D");

    hName.str(""); hName << "projXCellChargeSize1AllTracksNorm_" << planeName;
    projXCellChargeSize1AllTracksNorm_.push_back((TH1F*)h2DCellChargeSize1AllTracksNorm_[p]->ProjectionX(hName.str().c_str(), -xCellPitch/2, xCellPitch/2));

    hName.str(""); hName << "projXCellChargeSize2AllTracksNorm_" << planeName;
    projXCellChargeSize2AllTracksNorm_.push_back((TH1F*)h2DCellChargeSize2AllTracksNorm_[p]->ProjectionX(hName.str().c_str(), -xCellPitch/2, xCellPitch/2));

    theAnalysisManager_->cd("/Charge/" + planeName + "/XAsimmetry");

    hName.str(""); hName << "h1DXEtaDistribution_" << planeName;
    h1DXEtaDistribution_.push_back((TH1F*)h2DXCellChargeAsimmetrySizeLE2_[p]->ProjectionY(hName.str().c_str(), -xCellPitch, xCellPitch));

    theAnalysisManager_->cd("/Charge/" + planeName + "/YcellCharge1D");

    hName.str(""); hName << "projYCellChargeSize1AllTracksNorm_" << planeName;
    projYCellChargeSize1AllTracksNorm_.push_back((TH1F*)h2DCellChargeSize1AllTracksNorm_[p]->ProjectionY(hName.str().c_str(), -yCellPitch/2, yCellPitch/2));

    hName.str(""); hName << "projYCellChargeSize2AllTracksNorm_" << planeName;
    projYCellChargeSize2AllTracksNorm_.push_back((TH1F*)h2DCellChargeSize2AllTracksNorm_[p]->ProjectionY(hName.str().c_str(), -yCellPitch/2-0.5, yCellPitch/2+0.5));

    theAnalysisManager_->cd("/Charge/" + planeName + "/YAsimmetry");

    hName.str(""); hName << "h1DYEtaDistribution_" << planeName;
    h1DYEtaDistribution_.push_back((TH1F*)h2DYCellChargeAsimmetrySizeLE2_[p]->ProjectionY(hName.str().c_str(), -xCellPitch/2, xCellPitch/2));

    for (int j = 1; j < h1DXEtaDistribution_[p]->GetXaxis()->GetNbins()+1; ++j)
    {
        h1DXEtaDistribution_[p]->SetBinContent(j, (double)h1DXEtaDistribution_[p]->GetBinContent(j)/((double)projXCellChargeSize2AllTracksNorm_[p]->GetBinContent(j)));//projX_[p]->FindBin(h2DXCellChargeAsimmetrySizeLE2_[p]->GetXaxis()->GetBinCenter(i)))));
    }
    h1DXEtaDistribution_[p]->Scale(projXCellChargeSize2AllTracksNorm_[p]->GetEntries()/(double)projXCellChargeSize2AllTracksNorm_[p]->GetXaxis()->GetNbins());

    for (int j = 1; j < h1DYEtaDistribution_[p]->GetXaxis()->GetNbins()+1; ++j)
    {
        h1DYEtaDistribution_[p]->SetBinContent(j, (double)h1DYEtaDistribution_[p]->GetBinContent(j)/((double)projYCellChargeSize2AllTracksNorm_[p]->GetBinContent(j)));//projX_[p]->FindBin(h2DXCellChargeAsimmetrySizeLE2_[p]->GetXaxis()->GetBinCenter(i)))));
    }
    h1DYEtaDistribution_[p]->Scale(projYCellChargeSize2AllTracksNorm_[p]->GetEntries()/(double)projYCellChargeSize2AllTracksNorm_[p]->GetXaxis()->GetNbins());
}

//=======================================================================
bool Charge::passStandardCuts(int planeID, const Data &data)
{
  if (!theXmlParser_->getAnalysesFromString("Charge")->applyStandardCuts()) return true;
  if (theXmlParser_->getAnalysesFromString("Charge")->excludeBadPlanes())   return passBadPlanesCut(planeID, data);

  int minHits   = 7;
  int excludeMe = 0;
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos)
    minHits = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str());
  else if(data.getHasHit(planeID))
    {
      if (data.getClusterSize(planeID) == 1) excludeMe = 1;
      else if (data.getClusterSize(planeID) == 2 && (data.getClusterPixelRow(0,planeID) == data.getClusterPixelRow(1,planeID)
						     || data.getClusterPixelCol(0,planeID) == data.getClusterPixelCol(1,planeID)))
	excludeMe = 1;
    }
  
  if (data.getNumberOfTelescopeHits()-excludeMe >= minHits) return true;
  else                                                      return false;
}

//=======================================================================
void Charge::beginJob(void)
{
  standardCutsPixelMinimumCharge_   = theXmlParser_->getAnalysesFromString("Charge")->getPixelMinimumCharge();
  standardCutsPixelMaximumCharge_   = theXmlParser_->getAnalysesFromString("Charge")->getPixelMaximumCharge();
  standardCutsClusterMinimumCharge_ = theXmlParser_->getAnalysesFromString("Charge")->getClusterMinimumCharge();
  standardCutsClusterMaximumCharge_ = theXmlParser_->getAnalysesFromString("Charge")->getClusterMaximumCharge();
  
  theWindowsManager_      = theAnalysisManager_->getWindowsManager();
  theCalibrationsManager_ = theAnalysisManager_->getCalibrationsManager();
  
  book();
  
  setParsLimits();
}

//=======================================================================
void Charge::analyze(const Data& data, int threadNumber)
{
  if (cutsFormulas_.find("main cut") != cutsFormulas_.end() && !cutsFormulas_["main cut"][threadNumber]->EvalInstance()) return;

  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++) clusterSize(p,data,threadNumber);

  bool clusterLandauCut = true;
  if(cutsFormulas_.find("cluster Landau") != cutsFormulas_.end())
    clusterLandauCut = cutsFormulas_["cluster Landau"][threadNumber]->EvalInstance();

  bool cellLandauCut = true;
  if(cutsFormulas_.find("cell Landau") != cutsFormulas_.end())
    cellLandauCut = cutsFormulas_["cell Landau"][threadNumber]->EvalInstance();
  
  bool cellChargeCut = true;
  if(cutsFormulas_.find("cell charge") != cutsFormulas_.end())
    cellChargeCut = cutsFormulas_["cell charge"][threadNumber]->EvalInstance();
  
  bool cellChargeXCut = true;
  if(cutsFormulas_.find("cell charge X") != cutsFormulas_.end())
    cellChargeXCut = cutsFormulas_["cell charge X"][threadNumber]->EvalInstance();
  
  bool cellChargeYCut = true;
  if(cutsFormulas_.find("cell charge Y") != cutsFormulas_.end())
    cellChargeYCut = cutsFormulas_["cell charge Y"][threadNumber]->EvalInstance();
    
  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      if (!passStandardCuts(p,data)) continue;
      
      if ((thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos) && (!passCalibrationsCut(p,data)))
	{
	  std::cout << __PRETTY_FUNCTION__ << "Calibration check not passed" << std::endl;
	  return;
	}


      // ######################################################
      // # Require all telescope planes with cluster size = 2 #
      // ######################################################
      if (ONLYdoubleHITS == true)
	for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
	  if ((p > 7) && (p < 16) && (data.getClusterSize(p) != 2)) return;


      cellLandau           (cellLandauCut ,p,data,threadNumber);
      cellCharge           (cellChargeCut ,p,data,threadNumber);
      meanChargePositionRN (cellChargeCut ,p,data,threadNumber);
      
      xChargeDivision      (cellChargeXCut,p,data,threadNumber);
      xAsimmetry           (cellChargeXCut,p,data,threadNumber);
      
      yChargeDivision      (cellChargeYCut,p,data,threadNumber);
      yAsimmetry           (cellChargeYCut,p,data,threadNumber);
    }
}

//=======================================================================
void Charge::endJob(void)
{
    std::stringstream ss;

    STDLINE("",ACWhite);

    for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
      {
        std::string planeName = thePlaneMapping_->getPlaneName(p);


        ss.str("") ; ss << "Adding threads for plane " << p ;
        STDLINE(ss.str().c_str(),ACYellow) ;

        ADD_THREADED(hCellLandau_                             [p]);
	ADD_THREADED(hClusterSize_                            [p]);

        ADD_THREADED(h1DXcellCharge_                          [p]);
        ADD_THREADED(h1DXcellChargeNorm_                      [p]);

        ADD_THREADED(h1DXcellChargeSecondHit_                 [p]);
        ADD_THREADED(h1DXcellChargeSecondHitNorm_             [p]);

        ADD_THREADED(h1DYcellCharge_                          [p]);
        ADD_THREADED(h1DYcellChargeNorm_                      [p]);

        ADD_THREADED(h1DYcellChargeSecondHit_                 [p]);
        ADD_THREADED(h1DYcellChargeSecondHitNorm_             [p]);


        ADD_THREADED(h2DClusterSize_                          [p]);
        ADD_THREADED(h2DCellCharge_                           [p]);
        ADD_THREADED(h2DCellChargeNorm_                       [p]);

        ADD_THREADED(h4CellsCharge_                           [p]);
        ADD_THREADED(h4CellsChargeNorm_                       [p]);

        ADD_THREADED(h2DXcellCharge_                          [p]);
        ADD_THREADED(h2DXcellChargeSecondHit_                 [p]);

        ADD_THREADED(h2DYcellCharge_                          [p]);
        ADD_THREADED(h2DYcellChargeSecondHit_                 [p]);


        ADD_THREADED(h2DXcellChargeAsimmetry_                 [p]);
        ADD_THREADED(h2DXcellChargeAsimmetryInv_              [p]);

        ADD_THREADED(h2DYcellChargeAsimmetry_                 [p]);
        ADD_THREADED(h2DYcellChargeAsimmetryInv_              [p]);

        STDLINE("Threading phase completed",ACGreen);


        STDLINE("Filling phase...",ACWhite);

        float xPitch   =   atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
        float yPitch   =   atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

        h2DCellCharge_                                         [p]->Divide(h2DCellChargeNorm_       [p]);
        h2DCellClusterCharge_                                  [p]->Divide(h2DCellChargeNorm_       [p]);
        h1DPixelYTrackResiduals_                               [p]->Divide(h1DPixelYTrackResidualsNorm_                                       [p]);
        h2DClusterSize_                                        [p]->Divide(h2DCellChargeNorm_                                                 [p]);

        h2DCellPixelCharge4Rows_                               [p]->Divide(h2DAllTracks4Rows_                                                 [p]);
        h2DCellClusterCharge4Rows_                             [p]->Divide(h2DCellCharge4RowsNorm_                                            [p]);
        h2DClusterCharge4RowsClusterSize2_                     [p]->Divide(h2DCellPixelCharge4RowsClusterSize2Norm_                           [p]);
        h2DClusterCharge4RowsClusterSize3_                     [p]->Divide(h2DCellPixelCharge4RowsClusterSize2Norm_                           [p]);
        h2DClusterCharge4RowsClusterSize4_                     [p]->Divide(h2DCellPixelCharge4RowsClusterSize2Norm_                           [p]);
        h2DCellCharge4RowsOnly50_                              [p]->Divide(h2DCellCharge4RowsNorm_                                            [p]);
        h2DClusterSize4Rows_                                   [p]->Divide(h2DCellCharge4RowsNorm_                                            [p]);
        h2DCellChargeSecondHit4RowsSameRow_                    [p]->Divide(h2DCellChargeSecondHit4RowsSameRowNorm_                            [p]);
        h2DCellChargeSecondHit4RowsSameCol_                    [p]->Divide(h2DCellChargeSecondHit4RowsSameColNorm_                            [p]);
        h2DCellChargeSecondHit4RowsClusterSize3_               [p]->Divide(h2DCellChargeSecondHit4RowsClusterSize3Norm_                       [p]);
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_      [p]->Divide(h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_              [p]);
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_     [p]->Divide(h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_             [p]);
        h2DCellPixelCharge4RowsClusterSize1_                   [p]->Divide(h2DCellPixelCharge4RowsClusterSize1Norm_                           [p]);
        h2DCellPixelCharge4RowsExcept50ClusterSize1_           [p]->Divide(h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_                   [p]);
        h2DCellPixelCharge4RowsClusterSize2_                   [p]->Divide(h2DCellPixelCharge4RowsClusterSize2Norm_                           [p]);
        h2DCellPixelCharge4RowsClusterSize3_                   [p]->Divide(h2DCellPixelCharge4RowsClusterSize3Norm_                           [p]);
        h2DCellPixelCharge4RowsClusterSize4_                   [p]->Divide(h2DCellPixelCharge4RowsClusterSize4Norm_                           [p]);
        h1DPixelYTrackResiduals4Rows_                          [p]->Divide(h1DPixelYTrackResiduals4RowsNorm_                                  [p]);

        h2DCellPixelCharge2Rows_                               [p]->Divide(h2DAllTracks2Rows_                                                 [p]);
        h2DCellClusterCharge2Rows_                             [p]->Divide(h2DCellCharge2RowsNorm_                                            [p]);
        h2DCellCharge2RowsOnly50_                              [p]->Divide(h2DCellCharge2RowsNorm_                                            [p]);
        h2DClusterSize2Rows_                                   [p]->Divide(h2DCellCharge2RowsNorm_                                            [p]);
        h2DCellChargeSecondHit2RowsSameRow_                    [p]->Divide(h2DCellChargeSecondHit2RowsSameRowNorm_                            [p]);
        h2DCellChargeSecondHit2RowsSameCol_                    [p]->Divide(h2DCellChargeSecondHit2RowsSameColNorm_                            [p]);
        h2DCellChargeSecondHit2RowsClusterSize3_               [p]->Divide(h2DCellChargeSecondHit2RowsClusterSize3Norm_                       [p]);
        h2DCellChargeSecondHit2RowsClusterSize3LowCharge_      [p]->Divide(h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_              [p]);
        h2DCellChargeSecondHit2RowsClusterSize3HighCharge_     [p]->Divide(h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_             [p]);
        h2DCellPixelCharge2RowsClusterSize1_                   [p]->Divide(h2DCellPixelCharge2RowsClusterSize1Norm_                           [p]);
        h2DCellPixelCharge2RowsClusterSize2_                   [p]->Divide(h2DCellPixelCharge2RowsClusterSize2Norm_                           [p]);
        h2DCellPixelCharge2RowsClusterSize3_                   [p]->Divide(h2DCellPixelCharge2RowsClusterSize3Norm_                           [p]);
        h2DCellPixelCharge2RowsClusterSize4_                   [p]->Divide(h2DCellPixelCharge2RowsClusterSize4Norm_                           [p]);
        h1DPixelYTrackResiduals2Rows_                          [p]->Divide(h1DPixelYTRackResiduals2RowsNorm_                                  [p]);


        h2DCellChargeSecondHit_       [p]->Divide(h2DAllTracks_             [p]); //to normalize with respect to all the events
        h4CellsCharge_                [p]->Divide(h4CellsChargeNorm_        [p]);
        h4HitsCharge_                 [p]->Divide(h4Hits_                   [p]);
        hClusterSizeDistribution1s_   [p]->Divide(hClusterSizeNormalization_[p]);
        hClusterSizeDistribution2s_   [p]->Divide(hClusterSizeNormalization_[p]);
        h1DXcellCharge_               [p]->Divide(h1DXcellChargeNorm_       [p]);
        h1DXcellChargeNormToAll_      [p]->Divide(h1DXallTracks_            [p]);
        h1DXcellChargeSumLE2_         [p]->Divide(h1DXcellChargeSumLE2Norm_ [p]);
        h1DXcellChargeSumLE2NormToAll_[p]->Divide(h1DXallTracks_            [p]);
        h1DXcellChargeSumLE3_         [p]->Divide(h1DXcellChargeSumLE3Norm_ [p]);
        h1DXcellChargeSumLE3NormToAll_[p]->Divide(h1DXallTracks_            [p]);
        h1DXcellChargeSecondHit_      [p]->Divide(h1DXcellChargeSecondHitNorm_[p]);
        hCellChargeCoarse_            [p]->Divide(hCellChargeCoarseNorm_    [p]);

        h1DYcellCharge_               [p]->Divide(h1DYcellChargeNorm_       [p]);
        h1DYcellChargeNormToAll_      [p]->Divide(h1DYallTracks_            [p]);
        h1DYcellChargeSumLE2_         [p]->Divide(h1DYcellChargeSumLE2Norm_ [p]);
        h1DYcellChargeSecondHit_      [p]->Divide(h1DYcellChargeSecondHitNorm_[p]);
        h1DYcellChargeSumLE2NormToAll_[p]->Divide(h1DYallTracks_            [p]);
        h1DYcellChargeSumLE3_         [p]->Divide(h1DYcellChargeSumLE3Norm_ [p]);
        h1DYcellChargeSumLE3NormToAll_[p]->Divide(h1DYallTracks_            [p]);

        h2DXCellChargeAsimmetryCell_  [p]->Divide(h2DXCellChargeAsimmetryCellNorm_[p]);
        h2DYCellChargeAsimmetryCell_  [p]->Divide(h2DYCellChargeAsimmetryCellNorm_[p]);


        setErrorsBar(p);


        STDLINE("Setting styles...",ACWhite) ;

        h1DXcellChargeAsimmetry_   [p]->SetMinimum(-1);
        h1DXcellChargeAsimmetry_   [p]->SetMaximum( 1);
        h1DXcellChargeAsimmetry_   [p]->SetMarkerStyle(20);
        h1DXcellChargeAsimmetry_   [p]->SetMarkerSize(0.6);

        h1DXcellChargeAsimmetryInv_[p]->SetMinimum(-xPitch/2);
        h1DXcellChargeAsimmetryInv_[p]->SetMaximum(xPitch/2);
        h1DXcellChargeAsimmetryInv_[p]->SetMarkerStyle(20);
        h1DXcellChargeAsimmetryInv_[p]->SetMarkerSize(0.6);

        h1DYcellChargeAsimmetry_   [p]->SetMinimum(-1);
        h1DYcellChargeAsimmetry_   [p]->SetMaximum( 1);
        h1DYcellChargeAsimmetry_   [p]->SetMarkerStyle(20);
        h1DYcellChargeAsimmetry_   [p]->SetMarkerSize(0.6);

        h1DYcellChargeAsimmetryInv_[p]->SetMinimum(-yPitch/2);
        h1DYcellChargeAsimmetryInv_[p]->SetMaximum(yPitch/2);
        h1DYcellChargeAsimmetryInv_[p]->SetMarkerStyle(20);
        h1DYcellChargeAsimmetryInv_[p]->SetMarkerSize(0.6);

        h1DXcellCharge_                          [p]->SetMarkerStyle(20);
        h1DXcellCharge_                          [p]->SetMarkerSize(0.6);
        h1DXcellCharge_                          [p]->SetMarkerColor(kBlack);
        h1DXcellCharge_                          [p]->SetLineColor(kBlack);

        h1DXcellChargeSumLE2_                    [p]->SetMarkerStyle(20);
        h1DXcellChargeSumLE2_                    [p]->SetMarkerSize(0.6);
        h1DXcellChargeSumLE2_                    [p]->SetMarkerColor(kRed);
        h1DXcellChargeSumLE2_                    [p]->SetLineColor(kRed);

        h1DXcellChargeSumLE3_                    [p]->SetMarkerStyle(20);
        h1DXcellChargeSumLE3_                    [p]->SetMarkerSize(0.6);
        h1DXcellChargeSumLE3_                    [p]->SetMarkerColor(kBlue);
        h1DXcellChargeSumLE3_                    [p]->SetLineColor(kBlue);

        h1DYcellCharge_                          [p]->SetMarkerStyle(20);
        h1DYcellCharge_                          [p]->SetMarkerSize(0.6);
        h1DYcellCharge_                          [p]->SetMarkerColor(kBlack);
        h1DYcellCharge_                          [p]->SetLineColor(kBlack);

        h1DYcellChargeSumLE2_                    [p]->SetMarkerStyle(20);
        h1DYcellChargeSumLE2_                    [p]->SetMarkerSize(0.6);
        h1DYcellChargeSumLE2_                    [p]->SetMarkerColor(kRed);
        h1DYcellChargeSumLE2_                    [p]->SetLineColor(kRed);

        h1DYcellChargeSumLE3_                    [p]->SetMarkerStyle(20);
        h1DYcellChargeSumLE3_                    [p]->SetMarkerSize(0.6);
        h1DYcellChargeSumLE3_                    [p]->SetMarkerColor(kBlue);
        h1DYcellChargeSumLE3_                    [p]->SetLineColor(kBlue);

        h1DXcellChargeSecondHit_                 [p]->SetMarkerStyle(20);
        h1DXcellChargeSecondHit_                 [p]->SetMarkerSize(0.6);
        h1DXcellChargeSecondHit_                 [p]->SetMarkerColor(kBlack);
        h1DXcellChargeSecondHit_                 [p]->SetLineColor(kBlack);






	h2DClusterSize_                                [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DClusterSize_                                [p]->GetYaxis()->SetTitle("short pitch (um)"  );


        hClusterSize_                                  [p]->GetXaxis()->SetTitle("cluster size"      );
        hClusterSizeStandardCutsThreshold_             [p]->GetXaxis()->SetTitle("cluster size"      );
        hClusterSizeStandardCutsThresholdAndCellLandau_[p]->GetXaxis()->SetTitle("cluster size"      );
        hNumberOfCols_                                 [p]->GetXaxis()->SetTitle("number of columns" );
        hNumberOfRows_                                 [p]->GetXaxis()->SetTitle("number of rows"    );
        hClusterSizeDistribution1s_                    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hClusterSizeDistribution1s_                    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hClusterSizeDistribution2s_                    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hClusterSizeDistribution2s_                    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hClusterSizeNormalization_                     [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hClusterSizeNormalization_                     [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DClusterSizeYProjection_                     [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DClusterSizeYProjection_                     [p]->GetYaxis()->SetTitle("Cluster Size"      );

        hLandauClusterSize1_                           [p]->GetXaxis()->SetTitle("charge (electrons)");
        hCellLandau_                                   [p]->GetXaxis()->SetTitle("charge (electrons)");
        hCellLandau3D_                                 [p]->GetXaxis()->SetTitle("charge (electrons)");
        hCellLandau3DElectrodes_                       [p]->GetXaxis()->SetTitle("charge (electrons)");
        hCellLandauSinglePixel_                        [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize2_                           [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize2ChargeOver5000_             [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize2sameRow_                    [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize2sameCol_                    [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize3_                           [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize3ChargeOver5000_             [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize3sameRow_                    [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize3sameCol_                    [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize4_                           [p]->GetXaxis()->SetTitle("charge (electrons)");


        h1DPixelYTrackResiduals_                 [p]->GetXaxis()->SetTitle("short pitch (um)"        );
        h1DPixelYTrackResiduals_                 [p]->GetYaxis()->SetTitle("Y Track Residuals (um)"  );
        h2DCellCharge_                           [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellCharge_                           [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellClusterCharge_                    [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellClusterCharge_                    [p]->GetYaxis()->SetTitle("short pitch (um)"        );

        /*----------------------------------- 4 Rows ------------------------------------------------------------*/
        h2DCellPixelCharge4Rows_                                  [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4Rows_                                  [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellClusterCharge4Rows_                                [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellClusterCharge4Rows_                                [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DClusterCharge4RowsClusterSize2_                        [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DClusterCharge4RowsClusterSize2_                        [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DClusterCharge4RowsClusterSize3_                        [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DClusterCharge4RowsClusterSize3_                        [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DClusterCharge4RowsClusterSize4_                        [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DClusterCharge4RowsClusterSize4_                        [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DAllTracks4Rows_                                        [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DAllTracks4Rows_                                        [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellCharge4RowsOnly50_                                 [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellCharge4RowsOnly50_                                 [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DClusterSize4Rows_                                      [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DClusterSize4Rows_                                      [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h1DClusterSize4RowsYProjection_                           [p]->GetXaxis()->SetTitle("short pitch (um)"      );
        h1DClusterSize4RowsYProjection_                           [p]->GetYaxis()->SetTitle("Cluster Size"          );
        h1DClusterCharge4RowsYProjection_                         [p]->GetXaxis()->SetTitle("short pitch (um)"      );
        h1DClusterCharge4RowsYProjection_                         [p]->GetYaxis()->SetTitle("Cell Charge"           );
        h2DCellCharge4RowsNorm_                                   [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellCharge4RowsNorm_                                   [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsSameRow_                       [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsSameRow_                       [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsSameRowNorm_                   [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsSameRowNorm_                   [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsSameCol_                       [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsSameCol_                       [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsSameColNorm_                   [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsSameColNorm_                   [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsClusterSize3_                  [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsClusterSize3_                  [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsClusterSize3Norm_              [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsClusterSize3Norm_              [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_         [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_         [p]->GetXaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_     [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_     [p]->GetXaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_        [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_        [p]->GetXaxis()->SetTitle("short pitch (um)"      );
        h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_    [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_    [p]->GetXaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsClusterSize1_                      [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsClusterSize1_                      [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsClusterSize1Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsClusterSize1Norm_                  [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsExcept50ClusterSize1_              [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsExcept50ClusterSize1_              [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_          [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_          [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsClusterSize2_                      [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsClusterSize2_                      [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsClusterSize2Norm_                  [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsClusterSize2Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsClusterSize3_                      [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsClusterSize3_                      [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsClusterSize3Norm_                  [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsClusterSize3Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsClusterSize4_                      [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsClusterSize4_                      [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DCellPixelCharge4RowsClusterSize4Norm_                  [p]->GetXaxis()->SetTitle("long pitch (um)"       );
        h2DCellPixelCharge4RowsClusterSize4Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"      );
        h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 1"       );
        h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 4"       );
        h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_  [p]->GetXaxis()->SetTitle("Charge in Rows 2+3"    );
        h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_  [p]->GetYaxis()->SetTitle("Charge in Rows 1+4"    );
        h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_  [p]->GetXaxis()->SetTitle("Charge in Rows 1+4"    );
        h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_  [p]->GetYaxis()->SetTitle("Charge in Rows 2+3"    );
        h1DPixelCharge4RowsClusterSize4InternalProfile_           [p]->GetXaxis()->SetTitle("Charge in Rows 1+4"    );
        h1DPixelCharge4RowsClusterSize4InternalProfile_           [p]->GetYaxis()->SetTitle("Charge in Rows 2+3"    );
        h1DPixelCharge4RowsClusterSize4ExternalProfile_           [p]->GetXaxis()->SetTitle("Charge in Rows 2+3"    );
        h1DPixelCharge4RowsClusterSize4ExternalProfile_           [p]->GetYaxis()->SetTitle("Charge in Rows 1+4"    );
        h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 1"       );
        h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 4"       );
        h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_  [p]->GetXaxis()->SetTitle("Charge in Rows 2+3"    );
        h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_  [p]->GetYaxis()->SetTitle("Charge in Rows 1+4"    );
        h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_  [p]->GetXaxis()->SetTitle("Charge in Rows 1+4"    );
        h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_  [p]->GetYaxis()->SetTitle("Charge in Rows 2+3"    );
        h1DPixelCharge4RowsClusterSize3InternalProfile_           [p]->GetXaxis()->SetTitle("Charge in Rows 1+4"    );
        h1DPixelCharge4RowsClusterSize3InternalProfile_           [p]->GetYaxis()->SetTitle("Charge in Rows 2+3"    );
        h1DPixelCharge4RowsClusterSize3ExternalProfile_           [p]->GetXaxis()->SetTitle("Charge in Rows 2+3"    );
        h1DPixelCharge4RowsClusterSize3ExternalProfile_           [p]->GetYaxis()->SetTitle("Charge in Rows 1+4"    );
        h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 1"       );
        h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 4"       );
        h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_  [p]->GetXaxis()->SetTitle("Charge in Rows 2+3"    );
        h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_  [p]->GetYaxis()->SetTitle("Charge in Rows 1+4"    );
        h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_  [p]->GetXaxis()->SetTitle("Charge in Rows 1+4"    );
        h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_  [p]->GetYaxis()->SetTitle("Charge in Rows 2+3"    );
        h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_         [p]->GetXaxis()->SetTitle("Charge in Row 1"       );
        h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_         [p]->GetYaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelCharge4RowsClusterSize2BetweenRow1vsRows1And2_    [p]->GetXaxis()->SetTitle("Charge in Rows 1 And 2");
        h2DPixelCharge4RowsClusterSize2BetweenRow1vsRows1And2_    [p]->GetYaxis()->SetTitle("Charge in Row 1"       );
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_    [p]->GetXaxis()->SetTitle("Charge in Rows 1 And 2");
        h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_    [p]->GetYaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_    [p]->GetXaxis()->SetTitle("Charge in Row 1"       );
        h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_    [p]->GetYaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_    [p]->GetXaxis()->SetTitle("Charge in Row 1"       );
        h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_    [p]->GetYaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_         [p]->GetXaxis()->SetTitle("Charge in Row 4"       );
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_         [p]->GetYaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_    [p]->GetXaxis()->SetTitle("Charge in Rows 3 And 4");
        h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_    [p]->GetYaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_    [p]->GetXaxis()->SetTitle("Charge in Rows 3 And 4");
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_    [p]->GetYaxis()->SetTitle("Charge in Row 4"       );
        h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_    [p]->GetXaxis()->SetTitle("Charge in Row 4"       );
        h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_    [p]->GetYaxis()->SetTitle("Charge in Row 3"       );
        h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_    [p]->GetXaxis()->SetTitle("Charge in Row 4"       );
        h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_    [p]->GetYaxis()->SetTitle("Charge in Row 3"       );
        h1DPixelCharge4RowsClusterSize2InternalProfile_           [p]->GetXaxis()->SetTitle("Charge in Rows 1+4"    );
        h1DPixelCharge4RowsClusterSize2InternalProfile_           [p]->GetYaxis()->SetTitle("Charge in Rows 2+3"    );
        h1DPixelCharge4RowsClusterSize2ExternalProfile_           [p]->GetXaxis()->SetTitle("Charge in Rows 2+3"    );
        h1DPixelCharge4RowsClusterSize2ExternalProfile_           [p]->GetYaxis()->SetTitle("Charge in Rows 1+4"    );
        h1DPixelYTrackResiduals4Rows_                             [p]->GetXaxis()->SetTitle("short pitch (um)"      );
        h1DPixelYTrackResiduals4Rows_                             [p]->GetYaxis()->SetTitle("Y Track Residuals (um)");


        /*----------------------------------- 2 Rows ------------------------------------------------------------*/
        h2DCellPixelCharge2Rows_                                  [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellPixelCharge2Rows_                                  [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellClusterCharge2Rows_                                [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellClusterCharge2Rows_                                [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DAllTracks2Rows_                                        [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DAllTracks2Rows_                                        [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellCharge2RowsOnly50_                                 [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellCharge2RowsOnly50_                                 [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DClusterSize2Rows_                                      [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DClusterSize2Rows_                                      [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h1DClusterSize2RowsYProjection_                           [p]->GetXaxis()->SetTitle("short pitch (um)"        );
        h1DClusterSize2RowsYProjection_                           [p]->GetYaxis()->SetTitle("Cluster Size"            );
        h1DClusterCharge2RowsYProfile_                            [p]->GetXaxis()->SetTitle("short pitch (um)"        );
        h1DClusterCharge2RowsYProfile_                            [p]->GetYaxis()->SetTitle("Cell Charge"             );
        h2DCellCharge2RowsNorm_                                   [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellCharge2RowsNorm_                                   [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsSameRow_                       [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsSameRow_                       [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsSameRowNorm_                   [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsSameRowNorm_                   [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsSameCol_                       [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsSameCol_                       [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsSameColNorm_                   [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsSameColNorm_                   [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsClusterSize3_                  [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsClusterSize3_                  [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsClusterSize3Norm_              [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsClusterSize3Norm_              [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsClusterSize3LowCharge_         [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsClusterSize3LowCharge_         [p]->GetXaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_     [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_     [p]->GetXaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsClusterSize3HighCharge_        [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsClusterSize3HighCharge_        [p]->GetXaxis()->SetTitle("short pitch (um)"        );
        h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_    [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_    [p]->GetXaxis()->SetTitle("short pitch (um)"        );
        h2DCellPixelCharge2RowsClusterSize1_                      [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellPixelCharge2RowsClusterSize1Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellPixelCharge2RowsClusterSize2_                      [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellPixelCharge2RowsClusterSize2Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellPixelCharge2RowsClusterSize3_                      [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellPixelCharge2RowsClusterSize3Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h2DCellPixelCharge2RowsClusterSize4_                      [p]->GetXaxis()->SetTitle("long pitch (um)"         );
        h2DCellPixelCharge2RowsClusterSize4Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"        );
        h1DPixelYTrackResiduals2Rows_                             [p]->GetXaxis()->SetTitle("short pitch (um)"        );
        h1DPixelYTrackResiduals2Rows_                             [p]->GetYaxis()->SetTitle("Y Track Residuals (um)"  );
        h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_              [p]->GetXaxis()->SetTitle("Charge in Row 2"       );
        h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_              [p]->GetYaxis()->SetTitle("Charge in Row 1"       );

        /*----------------------------------- 4 Rows ------------------------------------------------------------*/
        h2DCellPixelCharge4Rows_                                  [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellPixelCharge4Rows_                                  [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellClusterCharge4Rows_                                [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellClusterCharge4Rows_                                [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DAllTracks4Rows_                                        [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DAllTracks4Rows_                                        [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellCharge4RowsOnly50_                                 [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellCharge4RowsOnly50_                                 [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellCharge4RowsNorm_                                   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellCharge4RowsNorm_                                   [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsSameRow_                       [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsSameRow_                       [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsSameRowNorm_                   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsSameRowNorm_                   [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsSameCol_                       [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsSameCol_                       [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsSameColNorm_                   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsSameColNorm_                   [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsClusterSize3_                  [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsClusterSize3_                  [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsClusterSize3Norm_              [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsClusterSize3Norm_              [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_         [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_     [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_     [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_        [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_        [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DCellPixelCharge4RowsClusterSize1_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellPixelCharge4RowsClusterSize1Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellPixelCharge4RowsClusterSize2_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellPixelCharge4RowsClusterSize2Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellPixelCharge4RowsClusterSize3_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellPixelCharge4RowsClusterSize3Norm_                  [p]->GetYaxis()->SetTitle("short pitch (um)"  );

        /*-----------------------------------------------------------------------------------------------------------*/

        h2DCellChargeSecondHit_                  [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSecondHit_                  [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeNum_                        [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeNum_                        [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DAllTracks_                            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DAllTracks_                            [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeNorm_                       [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeNorm_                       [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSize2AllTracksNorm_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSize2AllTracksNorm_         [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellChargeSize1AllTracksNorm_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellChargeSize1AllTracksNorm_         [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h4CellsCharge_                           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h4CellsCharge_                           [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h4CellsAllTracks_                        [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h4CellsAllTracks_                        [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h4CellsChargeNorm_                       [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h4CellsChargeNorm_                       [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        hCellChargeCoarse_                       [p]->GetXaxis()->SetTitle("x (um)"            );
        hCellChargeCoarse_                       [p]->GetYaxis()->SetTitle("y (um)"            );
        hCellChargeCoarseNorm_                   [p]->GetXaxis()->SetTitle("x (um)"            );
        hCellChargeCoarseNorm_                   [p]->GetYaxis()->SetTitle("y (um)"            );
        hHitsNotOnRowCol_                        [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hHitsNotOnRowCol_                        [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        //        hCutsControl_                            [p]-> attach some legend...!
        hChargeNotOnRowCol_                      [p]->GetXaxis()->SetTitle("charge (electrons)");


        h1DXcellCharge_                          [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellCharge_                          [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DXcellChargeSumLE2_                    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeSumLE2_                    [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DXcellChargeSumLE3_                    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeSumLE3_                    [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DXcellChargeNormToAll_                 [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeNormToAll_                 [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DXcellChargeSumLE2NormToAll_           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeSumLE2NormToAll_           [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DXcellChargeSumLE3NormToAll_           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeSumLE3NormToAll_           [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DXcellChargeSecondHit_                 [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeSecondHit_                 [p]->GetYaxis()->SetTitle("charge (electrons)");
        h2DXcellCharge_                          [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellCharge_                          [p]->GetYaxis()->SetTitle("charge (electrons)");
        h2DXcellChargeSecondHit_                 [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellChargeSecondHit_                 [p]->GetYaxis()->SetTitle("charge (electrons)");
        h2DXcellChargeSumLE2_                    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellChargeSumLE2_                    [p]->GetYaxis()->SetTitle("charge (electrons)");
        h2DXcellChargeSumLE3_                    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellChargeSumLE3_                    [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DXallTracks_                           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeNorm_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellSingleHits_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellDoubleHits_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcell3Hits_                           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellSingleHits_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellSingleHits_                      [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DXcellDoubleHits_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellDoubleHits_                      [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        hHitsNotONRowColVsXSlope_                [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hHitsNotONRowColVsXSlope_                [p]->GetYaxis()->SetTitle("x slope (1/um)"    );
        hHitsNotOnRowColProjX_                   [p]->GetXaxis()->SetTitle("long pitch (um)"   );

        h1DYcellCharge_                          [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellCharge_                          [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DYcellChargeSumLE2_                    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeSumLE2_                    [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DYcellChargeSumLE3_                    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeSumLE3_                    [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DYcellChargeNormToAll_                 [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DYcellChargeNormToAll_                 [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DYcellChargeSumLE2NormToAll_           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DYcellChargeSumLE2NormToAll_           [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DYcellChargeSumLE3NormToAll_           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DYcellChargeSumLE3NormToAll_           [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DYcellChargeSecondHit_                 [p]->GetXaxis()->SetTitle("short pitch (um)"   );
        h1DYcellChargeSecondHit_                 [p]->GetYaxis()->SetTitle("charge (electrons)");
        h2DYcellCharge_                          [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellCharge_                          [p]->GetYaxis()->SetTitle("charge (electrons)");
        h2DYcellChargeSecondHit_                 [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeSecondHit_                 [p]->GetYaxis()->SetTitle("charge (electrons)");
        h2DYcellChargeSumLE2_                    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeSumLE2_                    [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DYallTracks_                           [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYallTracksSize2_                      [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYallTracksNoElectrodes_               [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeNorm_                      [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellSingleHits_                      [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellDoubleHits_                      [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcell3Hits_                           [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellSingleHits_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DYcellSingleHits_                      [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellDoubleHits_                      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DYcellDoubleHits_                      [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        hHitsNotONRowColVsYSlope_                [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hHitsNotONRowColVsYSlope_                [p]->GetYaxis()->SetTitle("y slope (1/um)"    );
        hHitsNotOnRowColProjY_                   [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hXAsimmetry_                             [p]->GetXaxis()->SetTitle("Asimmetry"         );
        hXAsimmetry0_                            [p]->GetXaxis()->SetTitle("Asimmetry on one side");
        h2DXAsimmetryLandau_                     [p]->GetXaxis()->SetTitle("charge (electrons)");
        h2DXAsimmetryLandau_                     [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DXcellChargeAsimmetry_                 [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellChargeAsimmetry_                 [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DXcellChargeAsimmetry_                 [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeAsimmetry_                 [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DXcellChargeAsimmetryInv_              [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DXcellChargeAsimmetryInv_              [p]->GetYaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeAsimmetryInv_              [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DXcellChargeAsimmetryInv_              [p]->GetYaxis()->SetTitle("long pitch (um)"   );
        h2DXcellChargeAsimmetryUnconstrained_    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXcellChargeAsimmetryUnconstrained_    [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DXcellChargeAsimmetryUnconstrained_    [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeAsimmetryUnconstrained_    [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DXcellChargeAsimmetryUnconstrainedInv_ [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DXcellChargeAsimmetryUnconstrainedInv_ [p]->GetYaxis()->SetTitle("long pitch (um)"   );
        h1DXcellChargeAsimmetryUnconstrainedInv_ [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DXcellChargeAsimmetryUnconstrainedInv_ [p]->GetYaxis()->SetTitle("long pitch (um)"   );
        h2DXCellChargeAsimmetrySizeLE2_          [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXCellChargeAsimmetrySizeLE2_          [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DXCellChargeAsimmetrySizeLE2_          [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h1DXCellChargeAsimmetrySizeLE2_          [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DXCellChargeAsimmetryY_                [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DXCellChargeAsimmetryY_                [p]->GetYaxis()->SetTitle("X Asimmetry"       );
        h1DXEtaDistribution_                     [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DXEtaDerivativeDistribution_           [p]->GetXaxis()->SetTitle("Long pitch (um)"   );
        h1DXEtaDerivativeDistribution_           [p]->GetYaxis()->SetTitle("dEta/dx"           );
        h2DXCellChargeAsimmetryCell_             [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DXCellChargeAsimmetryCell_             [p]->GetYaxis()->SetTitle("short pitch (um)"  );

        hYAsimmetry_                                           [p]->GetXaxis()->SetTitle("Asimmetry"         );
        hYAsimmetry0_                                          [p]->GetXaxis()->SetTitle("Asimmetry on one side");
        h2DYAsimmetryLandau_                                   [p]->GetXaxis()->SetTitle("charge (electrons)");
        h2DYAsimmetryLandau_                                   [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetry_                               [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetry_                               [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYCellChargeAsimmetryUpRows4Rows_                    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYCellChargeAsimmetryUpRows4Rows_                    [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYCellChargeAsimmetryDownRows4Rows_                  [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYCellChargeAsimmetryDownRows4Rows_                  [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DYcellChargeAsimmetry_                               [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeAsimmetry_                               [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInv_                            [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInv_                            [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryInv4Rows_                       [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInv4Rows_                       [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryInvCutOnEntries_                [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInvCutOnEntries_                [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryInvRows1And2Of4Rows_            [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInvRows1And2Of4Rows_            [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryInvRows4And3Of4Rows_            [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInvRows4And3Of4Rows_            [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryInvRows1And2Of4RowsCutOnEntries_[p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInvRows1And2Of4RowsCutOnEntries_[p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryInvRows4And3Of4RowsCutOnEntries_[p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInvRows4And3Of4RowsCutOnEntries_[p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryInvUpRows4Rows_                 [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInvUpRows4Rows_                 [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryInvDownRows4Rows_               [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInvDownRows4Rows_               [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeAsimmetryInv_                            [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DYcellChargeAsimmetryInv_                            [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryUnconstrained_                  [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryUnconstrained_                  [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DYcellChargeAsimmetryUnconstrained_                  [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeAsimmetryUnconstrained_                  [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryUnconstrainedInv_               [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryUnconstrainedInv_               [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeAsimmetryUnconstrainedInv_               [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DYcellChargeAsimmetryUnconstrainedInv_               [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYCellChargeAsimmetrySizeLE2_                        [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYCellChargeAsimmetrySizeLE2_                        [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DYCellChargeAsimmetrySizeLE2_                        [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYCellChargeAsimmetrySizeLE2_                        [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYCellChargeAsimmetryX_                              [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DYCellChargeAsimmetryX_                              [p]->GetYaxis()->SetTitle("Y Asimmetry"       );
        h1DYEtaDistribution_                                   [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DYEtaDerivativeDistribution_                         [p]->GetXaxis()->SetTitle("Short pitch (um)"  );
        h1DYEtaDerivativeDistribution_                         [p]->GetYaxis()->SetTitle("dEta/dy"           );
        h2DYCellChargeAsimmetryCell_                           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DYCellChargeAsimmetryCell_                           [p]->GetYaxis()->SetTitle("short pitch (um)"  );

        STDLINE("Fitting phase",ACWhite);

        STDLINE("fXAsimmetryFit",ACWhite);
        TF1* fXAsimmetryFit = new TF1("fXAsimmetryFit","pol1",-0.8,0.8);
        if(h1DXcellChargeAsimmetryInv_[p]->GetEntries()!=0) h1DXcellChargeAsimmetryInv_[p]->Fit(fXAsimmetryFit,"QR");

        STDLINE("fYAsimmetryFit",ACWhite);
        TF1* fYAsimmetryFit  = new TF1("fYAsimmetryFit","pol1",-0.8,0.8);
        if(h1DYcellChargeAsimmetryInv_[p]->GetEntries()!=0) h1DYcellChargeAsimmetryInv_[p]->Fit(fYAsimmetryFit,"QR");

        if (p==22)
	  {
            ss << "Fit h1DYcellChargeAsimmetryInv_:   "
               << "intercept: " << fYAsimmetryFit->GetParameter(0)
               << "slope: "     << fYAsimmetryFit->GetParameter(1) ;
            STDLINE(ss.str().c_str(),ACWhite) ;
	  }

        if(h1DYcellChargeAsimmetryInvRows1And2Of4Rows_[p]->GetEntries()!=0) h1DYcellChargeAsimmetryInvRows1And2Of4Rows_[p]->Fit(fYAsimmetryFit,"QR");
        if(p==22)
        {
            ss << "Fit h1DYcellChargeAsimmetryInvRows1And2Of4Rows_:   "
               << "intercept: " << fYAsimmetryFit->GetParameter(0)
               << "slope: "     << fYAsimmetryFit->GetParameter(1) ;
            STDLINE(ss.str().c_str(),ACWhite) ;
        }

        if(h1DYcellChargeAsimmetryInvRows4And3Of4Rows_[p]->GetEntries()!=0) h1DYcellChargeAsimmetryInvRows4And3Of4Rows_[p]->Fit(fYAsimmetryFit,"QR");
        if(p==22)
        {
            ss << "Fit h1DYcellChargeAsimmetryInvRows4And3Of4Rows_:   "
               << "intercept: " << fYAsimmetryFit->GetParameter(0)
               << "slope: "     << fYAsimmetryFit->GetParameter(1) ;
            STDLINE(ss.str().c_str(),ACWhite) ;
        }
    }

    STDLINE("calculateMeanCharge",ACWhite) ;
    calculateMeanCharge();
    STDLINE("Done!!",ACGreen) ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::load(TFile* file)
{
    //destroy();

    std::string hName;
    std::string dirName;
    std::string planeName;

    file->cd("/");
    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        planeName = thePlaneMapping_->getPlaneName(p);

        /*--------------------------------------------------------------------------landau-----------------------------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/Landau/";

        hName  = "hLandauClusterSize1_" + planeName;
        hLandauClusterSize1_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize1_[p] << std::endl;

        hName  = "hCellLandau_"+ planeName;
        hCellLandau_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hCellLandau_[p] << std::endl;

        hName  = "hCellLandau3D_" + planeName;
        hCellLandau3D_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hCellLandau3D_[p] << std::endl;

        hName  = "hCellLandau3DElectrodes_" + planeName;
        hCellLandau3DElectrodes_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hCellLandau3DElectrodes_[p] << std::endl;

        hName  = "hCellLandauSinglePixel_" + planeName;
        hCellLandauSinglePixel_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hCellLandauSinglePixel_[p] << std::endl;

        hName  = "hLandauClusterSize2_" + planeName;
        hLandauClusterSize2_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize2_[p] << std::endl;

        hName  = "hLandauClusterSize2ChargeOver5000_" + planeName;
        hLandauClusterSize2ChargeOver5000_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize2ChargeOver5000_[p] << std::endl;

        hName  = "hLandauClusterSize2sameCol_"                                + planeName;
        hLandauClusterSize2sameCol_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize2sameCol_[p] << std::endl;

        hName  = "hLandauClusterSize2sameRow_"                                + planeName;
        hLandauClusterSize2sameRow_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize2sameRow_[p] << std::endl;

        hName  = "hLandauClusterSize3_"                                       + planeName;
        hLandauClusterSize3_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize3_[p] << std::endl;

        hName  = "hLandauClusterSize3ChargeOver5000_" + planeName;
        hLandauClusterSize3ChargeOver5000_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize3ChargeOver5000_[p] << std::endl;

        hName  = "hLandauClusterSize3sameCol_"                                + planeName;
        hLandauClusterSize3sameCol_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize3sameCol_[p] << std::endl;

        hName  = "hLandauClusterSize3sameRow_"                                + planeName;
        hLandauClusterSize3sameRow_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize3sameRow_[p] << std::endl;

        hName  = "hLandauClusterSize4_" + planeName;
        hLandauClusterSize4_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize4_[p] << std::endl;

        /*--------------------------------------------------------------------------cluster size-----------------------------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/ClusterSize/";

        hName  = "hClusterSize_"                   + planeName;
        hClusterSize_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hClusterSize_[p] << std::endl;

        hName  = "hClusterSizeStandardCutsThreshold_"               + planeName;
        hClusterSizeStandardCutsThreshold_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hClusterSizeStandardCutsThreshold_[p] << std::endl;

        hName  = "hClusterSizeStandardCutsThresholdAndCellLandau_"               + planeName;
        hClusterSizeStandardCutsThresholdAndCellLandau_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hClusterSizeStandardCutsThresholdAndCellLandau_[p] << std::endl;

        hName  = "hNumberOfCols_"                  + planeName;
        hNumberOfCols_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hNumberOfCols_[p] << std::endl;

        hName  = "hNumberOfRows_"                  + planeName;
        hNumberOfRows_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hNumberOfRows_[p] << std::endl;

        hName  = "hClusterSizeDistribution1s_"           + planeName;
        hClusterSizeDistribution1s_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hClusterSizeDistribution1s_[p] << std::endl;

        hName  = "hClusterSizeDistribution2s_"           + planeName;
        hClusterSizeDistribution2s_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hClusterSizeDistribution2s_[p] << std::endl;

        hName  = "hClusterSizeNormalization_"           + planeName;
        hClusterSizeNormalization_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hClusterSizeNormalization_[p] << std::endl;

        hName  = "h2DClusterSize_" + planeName;
        h2DClusterSize_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DClusterSize_ [p] << std::endl;

        /*------------------------------------------------------------------------2D cell charge--------------------------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/2DCellCharge/";
        
        hName  = "h2DallTracks_"+ planeName;
        h2DAllTracks_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DAllTracks_[p] << std::endl;



	// @@@ Please do not modify this code @@@
        hName  = "h2DCellChargeNorm_" + planeName;
        h2DCellChargeNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeNorm_[p] << std::endl;

        hName  = "h2DCellCharge_" + planeName;
        h2DCellCharge_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge_[p] << std::endl;

        hName  = "h4CellsChargeNorm_" + planeName;
        h4CellsChargeNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h4CellsChargeNorm_[p] << std::endl;
        
        hName  = "h4CellsCharge_" + planeName;
        h4CellsCharge_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h4CellsCharge_[p] << std::endl;
	// ======================================



        hName  = "h2DCellClusterCharge_"+ planeName;
        h2DCellClusterCharge_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellClusterCharge_[p] << std::endl;

        hName  = "h1DPixelYTrackResiduals_"+ planeName;
        h1DPixelYTrackResiduals_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DPixelYTrackResiduals_[p] << std::endl;

        hName  = "h1DPixelYTrackResidualsNorm_"+ planeName;
        h1DPixelYTrackResidualsNorm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DPixelYTrackResidualsNorm_[p] << std::endl;

        /*---------------------------------------------------- 4 Rows Histograms (2D cell charge) ----------------------------------------------------------------------------------------------------------------------------*/

        dirName = "Charge/" + planeName + "/2DCellCharge4Rows25x600/";

        hName  = "h2DCellPixelCharge4Rows_"+ planeName;
        h2DCellPixelCharge4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4Rows_[p] << std::endl;

        hName  = "h2DCellClusterCharge4Rows_"+ planeName;
        h2DCellClusterCharge4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellClusterCharge4Rows_[p] << std::endl;

        hName  = "h2DClusterCharge4RowsClusterSize2_"+ planeName;
        h2DClusterCharge4RowsClusterSize2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DClusterCharge4RowsClusterSize2_[p] << std::endl;

        hName  = "h2DClusterCharge4RowsClusterSize3_"+ planeName;
        h2DClusterCharge4RowsClusterSize3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DClusterCharge4RowsClusterSize3_[p] << std::endl;

        hName  = "h2DClusterCharge4RowsClusterSize4_"+ planeName;
        h2DClusterCharge4RowsClusterSize4_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DClusterCharge4RowsClusterSize4_[p] << std::endl;

        hName  = "h2DAllTracks4Rows_ "+ planeName;
        h2DAllTracks4Rows_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DAllTracks4Rows_ [p] << std::endl;

        hName  = "h2DCellCharge4RowsOnly50_"+ planeName;
        h2DCellCharge4RowsOnly50_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge4RowsOnly50_ [p] << std::endl;

        hName  = "h2DClusterSize4Rows_"+ planeName;
        h2DClusterSize4Rows_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DClusterSize4Rows_ [p] << std::endl;

        hName  = "h2DCellCharge4RowsNorm_"+ planeName;
        h2DCellCharge4RowsNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge4RowsNorm_[p] << std::endl;

        hName  = "h2DCellChargeSecondHit4Rows_ "+ planeName;
        h2DCellChargeSecondHit4RowsSameRow_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsSameRow_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsNorm_ "+ planeName;
        h2DCellChargeSecondHit4RowsSameRowNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsSameRowNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsSameCol_ "+ planeName;
        h2DCellChargeSecondHit4RowsSameCol_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsSameCol_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsSameColNorm_ "+ planeName;
        h2DCellChargeSecondHit4RowsSameColNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsSameColNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsNormClusterSize3_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3Norm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3Norm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3LowCharge_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3LowCharge_ [p] << std::endl;
        hName  = "h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3HighCharge_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3HighCharge_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ [p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize1_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize1_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize1_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsExcept50ClusterSize1_"+ planeName;
        h2DCellPixelCharge4RowsExcept50ClusterSize1_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsExcept50ClusterSize1_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize2_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize2_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize3_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize3_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize4_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize4_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize4_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize1Norm_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize1Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize1Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_"+ planeName;
        h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize2Norm_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize2Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize2Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize3Norm_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize3Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize3Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize4Norm_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize4Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize4Norm_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_"+ planeName;
        h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_"+ planeName;
        h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_"+ planeName;
        h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_"+ planeName;
        h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_"+ planeName;
        h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_"+ planeName;
        h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_"+ planeName;
        h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_"+ planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow1vsRows1And2_"+ planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow1vsRows1And2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2BetweenRow1vsRows1And2_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow2vsRows1And2_"+ planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow2vsRows1And2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2BetweenRow2vsRows1And2_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_"+ planeName;
        h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_"+ planeName;
        h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_"+ planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_"+ planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_"+ planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_"+ planeName;
        h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_[p] << std::endl;

        hName  = "h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_"+ planeName;
        h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_[p] << std::endl;

        hName  = "h1DPixelYTrackResiduals4Rows_"+ planeName;
        h1DPixelYTrackResiduals4Rows_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DPixelYTrackResiduals4Rows_[p] << std::endl;

        hName  = "h1DPixelYTrackResiduals4RowsNorm_"+ planeName;
        h1DPixelYTrackResiduals4RowsNorm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DPixelYTrackResiduals4RowsNorm_[p] << std::endl;

        /*---------------------------------------------------- 2 Rows Histograms (2D cell charge) ----------------------------------------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/2DCellCharge2Rows50x300/";

        hName  = "h2DCellPixelCharge2Rows_"+ planeName;
        h2DCellPixelCharge2Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2Rows_[p] << std::endl;

        hName  = "h2DCellClusterCharge2Rows_"+ planeName;
        h2DCellClusterCharge2Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellClusterCharge2Rows_[p] << std::endl;

        hName  = "h2DAllTracks2Rows_ "+ planeName;
        h2DAllTracks2Rows_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DAllTracks2Rows_ [p] << std::endl;

        hName  = "h1DPixelYTrackResiduals2Rows_"+ planeName;
        h1DPixelYTrackResiduals2Rows_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DPixelYTrackResiduals2Rows_[p] << std::endl;

        hName  = "h1DPixelYTRackResiduals2RowsNorm_"+ planeName;
        h1DPixelYTRackResiduals2RowsNorm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DPixelYTRackResiduals2RowsNorm_[p] << std::endl;

        hName  = "h2DCellCharge2RowsOnly50_"+ planeName;
        h2DCellCharge2RowsOnly50_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge2RowsOnly50_ [p] << std::endl;

        hName  = "h2DClusterSize2Rows_"+ planeName;
        h2DClusterSize2Rows_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DClusterSize2Rows_ [p] << std::endl;

        hName  = "h2DCellCharge2RowsNorm_"+ planeName;
        h2DCellCharge2RowsNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge2RowsNorm_[p] << std::endl;

        hName  = "h2DCellChargeSecondHit2Rows_ "+ planeName;
        h2DCellChargeSecondHit2RowsSameRow_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsSameRow_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHitNorm_ "+ planeName;
        h2DCellChargeSecondHit2RowsSameRowNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsSameRowNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit2RowsSameCol_ "+ planeName;
        h2DCellChargeSecondHit2RowsSameCol_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsSameCol_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit2RowsSameColNorm_ "+ planeName;
        h2DCellChargeSecondHit2RowsSameColNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsSameColNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3_ "+ planeName;
        h2DCellChargeSecondHit2RowsClusterSize3_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsClusterSize3_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit2RowsNormClusterSize3_ "+ planeName;
        h2DCellChargeSecondHit2RowsClusterSize3Norm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsClusterSize3Norm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3LowCharge_ "+ planeName;
        h2DCellChargeSecondHit2RowsClusterSize3LowCharge_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsClusterSize3LowCharge_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_ "+ planeName;
        h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3HighCharge_ "+ planeName;
        h2DCellChargeSecondHit2RowsClusterSize3HighCharge_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsClusterSize3HighCharge_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_ "+ planeName;
        h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_ [p] << std::endl;

        hName  = "h2DCellPixelCharge2RowsClusterSize1_"+ planeName;
        h2DCellPixelCharge2RowsClusterSize1_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2RowsClusterSize1_[p] << std::endl;

        hName  = "h2DCellPixelCharge2RowsClusterSize2_"+ planeName;
        h2DCellPixelCharge2RowsClusterSize2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2RowsClusterSize2_[p] << std::endl;

        hName  = "h2DCellPixelCharge2RowsClusterSize3_"+ planeName;
        h2DCellPixelCharge2RowsClusterSize3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2RowsClusterSize3_[p] << std::endl;

        hName  = "h2DCellPixelCharge2RowsClusterSize4_"+ planeName;
        h2DCellPixelCharge2RowsClusterSize4_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2RowsClusterSize4_[p] << std::endl;

        hName  = "h2DCellPixelCharge2RowsClusterSize1Norm_"+ planeName;
        h2DCellPixelCharge2RowsClusterSize1Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2RowsClusterSize1Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge2RowsClusterSize2Norm_"+ planeName;
        h2DCellPixelCharge2RowsClusterSize2Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2RowsClusterSize2Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge2RowsClusterSize3Norm_"+ planeName;
        h2DCellPixelCharge2RowsClusterSize3Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2RowsClusterSize3Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge2RowsClusterSize4Norm_"+ planeName;
        h2DCellPixelCharge2RowsClusterSize4Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge2RowsClusterSize4Norm_[p] << std::endl;

        hName  = "h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_"+ planeName;
        h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_[p] << std::endl;


        /*----------------------------------------------------------------2D cell charge-----------------------------------------------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/2DCellCharge/";

        /*---------------------------------------------------- 4 Rows Histograms (2D cell charge) ----------------------------------------------------------------------------------------------------------------------------*/
        hName  = "h2DCellPixelCharge4Rows_"+ planeName;
        h2DCellPixelCharge4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4Rows_[p] << std::endl;

        hName  = "h2DCellClusterCharge4Rows_"+ planeName;
        h2DCellClusterCharge4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellClusterCharge4Rows_[p] << std::endl;

        hName  = "h2DAllTracks4Rows_ "+ planeName;
        h2DAllTracks4Rows_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DAllTracks4Rows_ [p] << std::endl;

        hName  = "h2DCellCharge4RowsOnly50_"+ planeName;
        h2DCellCharge4RowsOnly50_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge4RowsOnly50_ [p] << std::endl;

        hName  = "h2DCellCharge4RowsNorm_"+ planeName;
        h2DCellCharge4RowsNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge4RowsNorm_[p] << std::endl;

        hName  = "h2DCellChargeSecondHit4Rows_ "+ planeName;
        h2DCellChargeSecondHit4RowsSameRow_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsSameRow_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHitNorm_ "+ planeName;
        h2DCellChargeSecondHit4RowsSameRowNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsSameRowNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsSameCol_ "+ planeName;
        h2DCellChargeSecondHit4RowsSameCol_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsSameCol_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsSameColNorm_ "+ planeName;
        h2DCellChargeSecondHit4RowsSameColNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsSameColNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsNormClusterSize3_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3Norm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3Norm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3LowCharge_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3LowCharge_ [p] << std::endl;
        hName  = "h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3HighCharge_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3HighCharge_ [p] << std::endl;

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ "+ planeName;
        h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ [p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize1_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize1_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize1_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize2_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize2_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize3_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize3_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize1Norm_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize1Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize1Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize2Norm_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize2Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize2Norm_[p] << std::endl;

        hName  = "h2DCellPixelCharge4RowsClusterSize3Norm_"+ planeName;
        h2DCellPixelCharge4RowsClusterSize3Norm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellPixelCharge4RowsClusterSize3Norm_[p] << std::endl;

        /*----------------------------------------------------------------2D cell charge-----------------------------------------------------------------------------------------------------------------------------------*/

        hName  = "h2DCellChargeSecondHit_"+ planeName;
        h2DCellChargeSecondHit_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSecondHit_[p] << std::endl;
        
        hName  = "h2DCellChargeNum_"+ planeName;
        h2DCellChargeNum_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeNum_[p] << std::endl;
        
        hName  = "h2DCellChargeSize2Norm_"+ planeName;
        h2DCellChargeSize2AllTracksNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSize2AllTracksNorm_[p] << std::endl;
        
        hName  = "h2DCellChargeNormSize1_"+ planeName;
        h2DCellChargeSize1AllTracksNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeSize1AllTracksNorm_[p] << std::endl;
        
        hName  = "h4CellsAllTracks_"+ planeName;
        h4CellsAllTracks_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h4CellsAllTracks_[p] << std::endl;
        
        hName  = "hCellChargeCoarse_"+ planeName;
        hCellChargeCoarse_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hCellChargeCoarse_[p] << std::endl;
        
        hName  = "hCellChargeCoarseNorm_"+ planeName;
        hCellChargeCoarseNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hCellChargeCoarseNorm_[p] << std::endl;
        
	hName  = "h4HitsCharge_"+ planeName;
        h4HitsCharge_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h4HitsCharge_[p] << std::endl;
        
        hName  = "h4Hits_"+ planeName;
        h4Hits_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h4Hits_[p] << std::endl;
        
        hName  = "hCutsControl_"+ planeName;
        hCutsControl_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hCutsControl_[p] << std::endl;
        
        hName  = "hHitsNotOnRowCol_" + planeName;
        hHitsNotOnRowCol_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hHitsNotOnRowCol_[p] << std::endl;
        
        hName  = "hChargeNotOnRowCol_" + planeName;
        hChargeNotOnRowCol_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hChargeNotOnRowCol_[p] << std::endl;

        /*--------------------------------------------------------------------------2D cell charge - X coordinate---------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/XcellCharge2D/";
        
        hName  = "h2DXcellCharge_"                                      + planeName;
        h2DXcellCharge_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellCharge_[p] << std::endl;

        hName  = "h2DXcellChargeSecondHit_"                             + planeName;
        h2DXcellChargeSecondHit_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellChargeSecondHit_[p] << std::endl;
        
        hName  = "h2DXcellChargeSumLE2_"                                + planeName;
        h2DXcellChargeSumLE2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellChargeSumLE2_[p] << std::endl;

        hName  = "h2DXcellChargeSumLE3_"                                + planeName;
        h2DXcellChargeSumLE3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellChargeSumLE3_[p] << std::endl;

        hName  = "h2DXcellDoubleHits_"                                  + planeName;
        h2DXcellDoubleHits_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellDoubleHits_[p] << std::endl;

        hName  = "h2DXcellSingleHits_"                                  + planeName;
        h2DXcellSingleHits_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellSingleHits_[p] << std::endl;

        hName  = "hHitsNotONRowColVsXSlope_" + planeName;
        hHitsNotONRowColVsXSlope_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hHitsNotONRowColVsXSlope_[p] << std::endl;

        hName  = "hHitsNotOnRowColProjX_" + planeName;
        hHitsNotOnRowColProjX_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hHitsNotOnRowColProjX_[p] << std::endl;

        /*------------------------------------------------------------------------2D cell charge - Y coordinate----------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/YcellCharge2D/";
        
        hName  = "h2DYcellCharge_"+ planeName;
        h2DYcellCharge_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellCharge_[p] << std::endl;

        hName  = "h2DYcellChargeSecondHit_"+ planeName;
        h2DYcellChargeSecondHit_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeSecondHit_[p] << std::endl;

        hName  = "h2DYcellChargeSumLE2_"+ planeName;
        h2DYcellChargeSumLE2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeSumLE2_[p] << std::endl;

        hName  = "h2DYcellChargeSumLE3_"+ planeName;
        h2DYcellChargeSumLE3_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeSumLE3_[p] << std::endl;

        hName  = "h2DYcellDoubleHits_"+ planeName;
        h2DYcellDoubleHits_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellDoubleHits_[p] << std::endl;

        hName  = "h2DYcellSingleHits_"+ planeName;
        h2DYcellSingleHits_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellSingleHits_[p] << std::endl;

        hName  = "hHitsNotONRowColVsYSlope_" + planeName;
        hHitsNotONRowColVsYSlope_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hHitsNotONRowColVsYSlope_[p] << std::endl;

        hName  = "hHitsNotONRowColProjY_" + planeName;
        hHitsNotOnRowColProjY_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hHitsNotOnRowColProjY_[p] << std::endl;

        /*---------------------------------------------------------------------------1D cell charge - X coordinate--------------------------------------------------------------------- -----------------------*/
        dirName = "Charge/" + planeName + "/XcellCharge1D/";
        
        hName  = "h1DXcellCharge_"+ planeName;
        h1DXcellCharge_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellCharge_[p] << std::endl;

        hName  = "h1DXcellChargeNormToAll_"+ planeName;
        h1DXcellChargeNormToAll_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeNormToAll_[p] << std::endl;

        hName  = "h1DXcellChargeSumLE2_"+ planeName;
        h1DXcellChargeSumLE2_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeSumLE2_[p] << std::endl;

        hName  = "h1DXcellChargeSumLE2NormToAll_"+ planeName;
        h1DXcellChargeSumLE2NormToAll_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeSumLE2NormToAll_[p] << std::endl;

        hName  = "h1DXcellChargeSumLE3_" + planeName;
        h1DXcellChargeSumLE3_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeSumLE3_[p] << std::endl;

        hName  = "h1DXcellChargeSumLE3NormToAll_"+ planeName;
        h1DXcellChargeSumLE3NormToAll_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeSumLE3NormToAll_[p] << std::endl;

        hName  = "h1DXcellChargeSecondHit_"+ planeName;
        h1DXcellChargeSecondHit_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeSecondHit_[p] << std::endl;

        hName  = "h1DXallTracks_"+ planeName;
        h1DXallTracks_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXallTracks_[p] << std::endl;

        hName  = "h1DXcellChargeNorm_"+ planeName;
        h1DXcellChargeNorm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeNorm_[p] << std::endl;

        hName  = "h1DXcellChargeSumLE2Norm_"+ planeName;
        h1DXcellChargeSumLE2Norm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeSumLE2Norm_[p] << std::endl;

        hName  = "h1DXcellChargeSumLE3Norm_"+ planeName;
        h1DXcellChargeSumLE3Norm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeSumLE3Norm_[p] << std::endl;

        hName  = "h1DXcellSingleHits_"+ planeName;
        h1DXcellSingleHits_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellSingleHits_[p] << std::endl;

        hName  = "h1DXcellDoubleHits_"+ planeName;
        h1DXcellDoubleHits_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellDoubleHits_[p] << std::endl;

        hName  = "h1DXcell3Hits_"+ planeName;
        h1DXcell3Hits_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcell3Hits_[p] << std::endl;

        hName  = "h1DXcellChargeSecondHitNorm_"+ planeName;
        h1DXcellChargeSecondHitNorm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXcellChargeSecondHitNorm_[p] << std::endl;
        
        /*---------------------------------------------------------------------------1D cell charge - Y coordinate-------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/YcellCharge1D/";

        hName  = "h1DYcellCharge_"+ planeName;
        h1DYcellCharge_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellCharge_[p] << std::endl;

        hName  = "h1DYcellChargeNormToAll_"+ planeName;
        h1DYcellChargeNormToAll_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeNormToAll_[p] << std::endl;

        hName  = "h1DYcellChargeSumLE2_"+ planeName;
        h1DYcellChargeSumLE2_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeSumLE2_[p] << std::endl;

        hName  = "h1DYcellChargeSumLE2NormToAll_" + planeName;
        h1DYcellChargeSumLE2NormToAll_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeSumLE2NormToAll_[p] << std::endl;

        hName  = "h1DYcellChargeSumLE3_"+ planeName;
        h1DYcellChargeSumLE3_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeSumLE3_[p] << std::endl;

        hName  = "h1DYcellChargeSumLE3NormToAll_" + planeName;
        h1DYcellChargeSumLE3NormToAll_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeSumLE3NormToAll_[p] << std::endl;

        hName  = "h1DYcellChargeSecondHit_"+ planeName;
        h1DYcellChargeSecondHit_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeSecondHit_[p] << std::endl;

        hName  = "h1DYallTracks_"+ planeName;
        h1DYallTracks_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYallTracks_[p] << std::endl;

        hName  = "h1DYallTracksSize2_"+ planeName;
        h1DYallTracksSize2_.push_back((TH1F*)file->Get((dirName+hName).c_str()));

        hName  = "h1DYallTracksNoElectrodes_"+ planeName;
        h1DYallTracksNoElectrodes_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYallTracksNoElectrodes_[p] << std::endl;

        hName  = "h1DYcellChargeNorm_"  + planeName;
        h1DYcellChargeNorm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeNorm_[p] << std::endl;

        hName  = "h1DYcellChargeSumLE2Norm_"+ planeName;
        h1DYcellChargeSumLE2Norm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeSumLE2Norm_[p] << std::endl;

        hName  = "h1DYcellChargeSumLE3Norm_"+ planeName;
        h1DYcellChargeSumLE3Norm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeSumLE3Norm_[p] << std::endl;

        hName  = "h1DYcellSingleHits_" + planeName;
        h1DYcellSingleHits_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellSingleHits_[p] << std::endl;

        hName  = "h1DYcellDoubleHits_" + planeName;
        h1DYcellDoubleHits_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellDoubleHits_[p] << std::endl;

        hName  = "h1DYcell3Hits_"+ planeName;
        h1DYcell3Hits_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcell3Hits_[p] << std::endl;

        hName  = "h1DYcellChargeSecondHitNorm_"+ planeName;
        h1DYcellChargeSecondHitNorm_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYcellChargeSecondHitNorm_[p] << std::endl;

        /*----------------------------------------------------------------------------X mmetrmmetry-----------------------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/XAsimmetry/";

        hName  = "hXAsimmetry_"+ planeName;
        hXAsimmetry_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hXAsimmetry_[p] << std::endl;

        hName  = "hXAsimmetry0_"+ planeName;
        hXAsimmetry0_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hXAsimmetry0_[p] << std::endl;
        hName  = "h2DXAsimmetryLandau_"+ planeName;
        h2DXAsimmetryLandau_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXAsimmetryLandau_[p] << std::endl;

        hName  = "h2DXcellChargeAsimmetry_"+ planeName;
        h2DXcellChargeAsimmetry_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellChargeAsimmetry_[p] << std::endl;

        hName  = "h2DXcellChargeAsimmetryInv_"+ planeName;
        h2DXcellChargeAsimmetryInv_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellChargeAsimmetryInv_[p] << std::endl;

        hName  = "h2DXcellChargeAsimmetryUnconstrained_"+ planeName;
        h2DXcellChargeAsimmetryUnconstrained_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellChargeAsimmetryUnconstrained_[p] << std::endl;

        hName  = "h2DXcellChargeAsimmetryUnconstrainedInv_"+ planeName;
        h2DXcellChargeAsimmetryUnconstrainedInv_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXcellChargeAsimmetryUnconstrainedInv_[p] << std::endl;

        hName  = "h2DXcellChargeAsimmetrySizeLE2_"+ planeName;
        h2DXCellChargeAsimmetrySizeLE2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXCellChargeAsimmetrySizeLE2_[p] << std::endl;

        hName  = "h2DXCellChargeAsimmetryCell_"+ planeName;
        h2DXCellChargeAsimmetryCell_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXCellChargeAsimmetryCell_[p] << std::endl;

        hName  = "h2DXCellChargeAsimmetryCellNorm_"+ planeName;
        h2DXCellChargeAsimmetryCellNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXCellChargeAsimmetryCellNorm_[p] << std::endl;

        hName  = "h2DXCellChargeAsimmetryY_"+ planeName;
        h2DXCellChargeAsimmetryY_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DXCellChargeAsimmetryY_[p] << std::endl;

        hName = "h1DXEtaDerivativeDistribution_" + planeName;
        h1DXEtaDerivativeDistribution_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DXEtaDerivativeDistribution_[p] << std::endl;

        /*---------------------------------------------------------------------------Y Asimmetry-------------------------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/YAsimmetry/";

        hName  = "hYAsimmetry_"                                                     + planeName;
        hYAsimmetry_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hYAsimmetry_[p] << std::endl;

        hName  = "hYAsimmetry0_"                                                     + planeName;
        hYAsimmetry0_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hYAsimmetry0_[p] << std::endl;

        hName  = "h2DYAsimmetryLandau_"                                             + planeName;
        h2DYAsimmetryLandau_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYAsimmetryLandau_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetry_"                                         + planeName;
        h2DYcellChargeAsimmetry_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetry_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetryInv_"                                      + planeName;
        h2DYcellChargeAsimmetryInv_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInv_[p] << std::endl;

        dirName = "Charge/" + planeName + "/YAsimmetry4Rows25x600/";

        hName  = "h2DYcellChargeAsimmetryInv4Rows_"                                      + planeName;
        h2DYcellChargeAsimmetryInv4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInv4Rows_[p] << std::endl;

        dirName = "Charge/" + planeName + "/YAsimmetry/";

        hName  = "h2DYcellChargeAsimmetryInvCutOnEntries_"                                      + planeName;
        h2DYcellChargeAsimmetryInvCutOnEntries_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInvCutOnEntries_[p] << std::endl;

        dirName = "Charge/" + planeName + "/YAsimmetry4Rows25x600/";

        hName  = "h2DYcellChargeAsimmetryInvRows1And2Of4Rows_"                                      + planeName;
        h2DYcellChargeAsimmetryInvRows1And2Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInvRows1And2Of4Rows_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetryInvRows1And2Of4RowsCutOnEntries_"                                      + planeName;
        h2DYcellChargeAsimmetryInvRows1And2Of4RowsCutOnEntries_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInvRows1And2Of4RowsCutOnEntries_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetryInvRows4And3Of4Rows_"                                      + planeName;
        h2DYcellChargeAsimmetryInvRows4And3Of4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInvRows4And3Of4Rows_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetryInvRows4And3Of4RowsCutOnEntries_"                                      + planeName;
        h2DYcellChargeAsimmetryInvRows4And3Of4RowsCutOnEntries_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInvRows4And3Of4RowsCutOnEntries_[p] << std::endl;

        hName  = "h2DYCellChargeAsimmetryUpRows4Rows_"                                         + planeName;
        h2DYCellChargeAsimmetryUpRows4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYCellChargeAsimmetryUpRows4Rows_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetryInvUpRows4Rows_"                                      + planeName;
        h2DYcellChargeAsimmetryInvUpRows4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInvUpRows4Rows_[p] << std::endl;

        hName  = "h2DYCellChargeAsimmetryDownRows4Rows_"                                         + planeName;
        h2DYCellChargeAsimmetryDownRows4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYCellChargeAsimmetryDownRows4Rows_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetryInvDownRows4Rows_"                                      + planeName;
        h2DYcellChargeAsimmetryInvDownRows4Rows_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryInvDownRows4Rows_[p] << std::endl;

        dirName = "Charge/" + planeName + "/YAsimmetry/";

        hName  = "h2DYcellChargeAsimmetryUnconstrained_"                            + planeName;
        h2DYcellChargeAsimmetryUnconstrained_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryUnconstrained_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetryUnconstrainedInv_"                         + planeName;
        h2DYcellChargeAsimmetryUnconstrainedInv_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYcellChargeAsimmetryUnconstrainedInv_[p] << std::endl;

        hName  = "h2DYcellChargeAsimmetrySizeLE2_"                                  + planeName;
        h2DYCellChargeAsimmetrySizeLE2_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYCellChargeAsimmetrySizeLE2_[p] << std::endl;

        hName  = "h2DYCellChargeAsimmetryCell_"                                      + planeName;
        h2DYCellChargeAsimmetryCell_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYCellChargeAsimmetryCell_[p] << std::endl;

        hName  = "h2DYCellChargeAsimmetryCellNorm_"                                      + planeName;
        h2DYCellChargeAsimmetryCellNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYCellChargeAsimmetryCellNorm_[p] << std::endl;

        hName  = "h2DYCellChargeAsimmetryX_"                                         + planeName;
        h2DYCellChargeAsimmetryX_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DYCellChargeAsimmetryX_[p] << std::endl;

        hName = "h1DYEtaDerivativeDistribution_" + planeName;
        h1DYEtaDerivativeDistribution_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h1DYEtaDerivativeDistribution_[p] << std::endl;

    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::book(void)
{
    destroy();

    std::string hName;
    std::string hTitle;
    std::string planeName;
    std::stringstream ss;

    float xPitch;
    float yPitch;
    float binSize     =     5;//Size in microns
    int   removedBins =     0;
    float removeHalf  =     0;
    bool  centerBins  = false;
    if(centerBins)
    {
        removedBins = 1;
        removeHalf = binSize/2.;
    }

    theAnalysisManager_->cd("/");
    theAnalysisManager_->mkdir("Charge");

    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        planeName = thePlaneMapping_->getPlaneName(p);
        theAnalysisManager_->cd("Charge");
        theAnalysisManager_->mkdir(planeName);

        xPitch   =   atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
        yPitch   =   atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

        /*--------------------------------------------------------------------------cluster size-----------------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("ClusterSize");

        hName  = "hClusterSize_"                   + planeName;
        hTitle = "Cluster size distribution "      + planeName;
        hClusterSize_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 10, 0, 10)));

        hName  = "hClusterSizeStandardCutsThreshold_"               + planeName;
        hTitle = "Cluster size distribution (Standard cuts Threshold) "+ planeName;
        hClusterSizeStandardCutsThreshold_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 10, 0, 10)));

        hName  = "hClusterSizeStandardCutsThresholdAndCellLandau_"               + planeName;
        hTitle = "Cluster size distribution (Standard cuts Threshold and cell Landau) "+ planeName;
        hClusterSizeStandardCutsThresholdAndCellLandau_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 10, 0, 10)));

        hName  = "hNumberOfCols_"                  + planeName;
        hTitle = "Number of columns distribution " + planeName;
        hNumberOfCols_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 10, 0, 10)));

        hName  = "hNumberOfRows_"                  + planeName;
        hTitle = "Number of rows distribution "    + planeName;
        hNumberOfRows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 10, 0, 10)));

        hName  = "hClusterSizeDistribution1s_"           + planeName;
        hTitle = "Size 1 cluster distribution on pixel " + planeName;
        hClusterSizeDistribution1s_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "hClusterSizeDistribution2s_"           + planeName;
        hTitle = "Size 2 cluster distribution on pixel " + planeName;
        hClusterSizeDistribution2s_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "hClusterSizeNormalization_"           + planeName;
        hTitle = "Cluster size distribution on pixel normalization " + planeName;
        hClusterSizeNormalization_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        theAnalysisManager_->cd("Charge/" + planeName);

        theAnalysisManager_->mkdir("ClusterSize4Rows25x600");

        hName  = "h2DClusterSize4Rows_"                   + planeName;
        hTitle = "Cluster size distribution 4 Rows Cell " + planeName;
        h2DClusterSize4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName);
        theAnalysisManager_->mkdir("ClusterSize2Rows50x300");

        hName  = "h2DClusterSize2Rows_"                         + planeName;
        hTitle = "Cluster size distribution 2 Rows Cell " + planeName;
        h2DClusterSize2Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*----------------------------------------------------------------------Landau distributions-----------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("Landau");

        hName  = "hLandauClusterSize1_"                                       + planeName;
        hTitle = "Charge distribution for single hits "                       + planeName;
        hLandauClusterSize1_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hCellLandau_"                                               + planeName;
        hTitle = "Charge distribution for single hits in a fiducial window "  + planeName;
        hCellLandau_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hCellLandau3D_"                                             + planeName;
        hTitle = "Charge distribution for single hits in a fiducial window conceived for 3Ds 2E"  + planeName;
        hCellLandau3D_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hCellLandau3DElectrodes_"                                             + planeName;
        hTitle = "Charge distribution for single hits on 3D 2E electrodes"  + planeName;
        hCellLandau3DElectrodes_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hCellLandauSinglePixel_"                                               + planeName;
        hTitle = "Charge distribution foreach pixel in any cluster in a fiducial window "  + planeName;
        hCellLandauSinglePixel_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize2_"                                                  + planeName;
        hTitle = "Charge distribution of the track pointed hit for a cluster of size 2 " + planeName;
        hLandauClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize2ChargeOver5000_"                                       + planeName;
        hTitle = "Charge distribution for 2 adjacent hits (Charge over 5000 in each pixel) " + planeName;
        hLandauClusterSize2ChargeOver5000_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize2sameCol_"                                + planeName;
        hTitle = "Charge distribution for 2 ajacent hits on the same column " + planeName;
        hLandauClusterSize2sameCol_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize2sameRow_"                                + planeName;
        hTitle = "Charge distribution for 2 ajacent hits on the same row "    + planeName;
        hLandauClusterSize2sameRow_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize3_"                                       + planeName;
        hTitle = "Charge distribution for 3 adjacent hits "                   + planeName;
        hLandauClusterSize3_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize3ChargeOver5000_"                                       + planeName;
        hTitle = "Charge distribution for 3 adjacent hits (Charge over 5000 in each pixel) " + planeName;
        hLandauClusterSize3ChargeOver5000_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize3sameCol_"                                + planeName;
        hTitle = "Charge distribution for 3 ajacent hits on the same column " + planeName;
        hLandauClusterSize3sameCol_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize3sameRow_"                                + planeName;
        hTitle = "Charge distribution for 3 ajacent hits on the same row "    + planeName;
        hLandauClusterSize3sameRow_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize4_"                                       + planeName;
        hTitle = "Charge distribution for 4 adjacent hits "                       + planeName;
        hLandauClusterSize4_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*------------------------------------------------------------------------2D cell charge--------------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("2DCellCharge");

        hName  = "h2DallTracks_"                   + planeName;
        hTitle = "Cell charge normalization "      + planeName;
        h2DAllTracks_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "h1DPixelYTrackResiduals_"                   + planeName;
        hTitle = "Y residuals (all Clusters) "      + planeName;
        h1DPixelYTrackResiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/5. - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "h1DPixelYTrackResidualsNorm_"                   + planeName;
        hTitle = "Y residuals (all Clusters) "      + planeName;
        h1DPixelYTrackResidualsNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/5. - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        theAnalysisManager_->cd("Charge/" + planeName);

        theAnalysisManager_->mkdir("2DCellCharge4Rows25x600");

        hName  = "h1DPixelYTrackResiduals4Rows_"                   + planeName;
        hTitle = "Y Track residuals 4 Rows(all Clusters) "      + planeName;
        h1DPixelYTrackResiduals4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)4*(yPitch/5. - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h1DPixelYTrackResiduals4RowsNorm_"                   + planeName;
        hTitle = "Y residuals 4 Rows(all Clusters) "      + planeName;
        h1DPixelYTrackResiduals4RowsNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)4*(yPitch/5. - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName);

        theAnalysisManager_->mkdir("2DCellCharge2Rows50x300");

        hName  = "h1DPixelYTrackResiduals2Rows_"                   + planeName;
        hTitle = "Y residuals 2 Rows(all Clusters) "      + planeName;
        h1DPixelYTrackResiduals2Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)2*(yPitch/5. - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h1DPixelYTRackResiduals2RowsNorm_"                   + planeName;
        hTitle = "Y residuals 2 Rows(all Clusters) "      + planeName;
        h1DPixelYTRackResiduals2RowsNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)2*(yPitch/5. - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge/");

        hName  = "h2DCellClusterCharge_"                  + planeName;
        hTitle = "Cell Cluster charge 2D distribution "    + planeName;
        h2DCellClusterCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "h2DCellChargeSecondHit_"                  + planeName;
        hTitle = "Cell charge 2D distribution, second hit "    + planeName;
        h2DCellChargeSecondHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "h2DCellChargeNum_"                          + planeName;
        hTitle = "Cell charge 2D distribution not normalized " + planeName;
        h2DCellChargeNum_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "h2DCellChargeSize2Norm_"              + planeName;
        hTitle = "Cell charge normalization for cluster size 2 "     + planeName;
        h2DCellChargeSize2AllTracksNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch+2, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));
        //        h2DCellChargeSize2AllTracksNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)xPitch/2 - 0, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, 2*(int)yPitch/2 - 0, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));
        //        if(p == 22)
        //        {
        //           STDLINE("Charge:h2DCellChargeSize2AllTracksNorm_ : ",ACWhite);
        //           STDLINE("",ACWhite);
        //           ss.str(""); ss << "Y bins: "<< (int)(yPitch/2);
        //           STDLINE(ss.str(),ACGreen);
        //        }

        hName  = "h2DCellChargeNormSize1_"              + planeName;
        hTitle = "Cell charge normalization for cluster size 1 "     + planeName;
        h2DCellChargeSize1AllTracksNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)xPitch/2 - 0, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, 2*(int)yPitch/2 - 0, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "h4CellsAllTracks_"               + planeName;
        hTitle = "4 cells charge normalization "   + planeName;
        h4CellsAllTracks_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "hCellChargeCoarse_"              + planeName;
        hTitle = "Cell charge 2D distribution "    + planeName;
        hCellChargeCoarse_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/(binSize*2.), -(xPitch/2), xPitch/2, (int)yPitch/(binSize*2.), -(yPitch/2), yPitch/2)));

        hName  = "hCellChargeCoarseNorm_"          + planeName;
        hTitle = "Cell charge normalization "      + planeName;
        hCellChargeCoarseNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/(binSize*2.), -(xPitch/2), xPitch/2, (int)yPitch/(binSize*2.), -(yPitch/2), yPitch/2)));



	// @@@ Please do not modify this code @@@
        hName  = "h2DCellChargeNorm_"              + planeName;
        hTitle = "Cell charge normalization "      + planeName;
        h2DCellChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

        hName  = "h2DCellCharge_"                  + planeName;
        hTitle = "Cell charge 2D distribution "    + planeName;
        h2DCellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

        hName  = "h4CellsChargeNorm_"              + planeName;
        hTitle = "4 cells charge normalization "   + planeName;
        h4CellsChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

        hName  = "h4CellsCharge_"                  + planeName;
        hTitle = "4 cells charge 2D distribution " + planeName;
        h4CellsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

        hName  = "h2DClusterSize_"                 + planeName;
        hTitle = "Cluster size distribution  "     + planeName;
        h2DClusterSize_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));
	// ======================================



        hName  = "h4HitsCharge_"                   + planeName;
        hTitle = "4 hits charge 2D distribution "  + planeName;
        h4HitsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/5, -xPitch, xPitch, (int)yPitch/5, -yPitch, yPitch)));

        hName  = "h4Hits_"                         + planeName;
        hTitle = "4 hits 2D distribution "         + planeName;
        h4Hits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/5, -xPitch, xPitch, (int)yPitch/5, -yPitch, yPitch)));

        hName  = "hCutsControl_"                             + planeName;
        hTitle = "Events surviving different level of cuts " + planeName;
        hCutsControl_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 6, 0, 6 )));

        hName  = "hHitsNotOnRowCol_"                  + planeName;
        hTitle = "Distribution of hits with the hit pixel not equal to the predicted "    + planeName;
        hHitsNotOnRowCol_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "hChargeNotOnRowCol_" + planeName;
        hTitle = "Charge distributionn for hits whose tracks don't point on the same pixel " + planeName;
        hChargeNotOnRowCol_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge4Rows25x600");

        hName  = "h2DCellPixelCharge4Rows_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution" + planeName;
        h2DCellPixelCharge4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge2Rows50x300");

        hName  = "h2DCellPixelCharge2Rows_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution" + planeName;
        h2DCellPixelCharge2Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge4Rows25x600");

        hName  = "h2DCellClusterCharge4Rows_"                         + planeName;
        hTitle = "4 Rows cell cluster charge 2D distribution" + planeName;
        h2DCellClusterCharge4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DClusterCharge4RowsClusterSize2_"                         + planeName;
        hTitle = "4 Rows cluster charge 2D distribution (Size 2) " + planeName;
        h2DClusterCharge4RowsClusterSize2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DClusterCharge4RowsClusterSize3_"                         + planeName;
        hTitle = "4 Rows cluster charge 2D distribution (Size 3) " + planeName;
        h2DClusterCharge4RowsClusterSize3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DClusterCharge4RowsClusterSize4_"                         + planeName;
        hTitle = "4 Rows cluster charge 2D distribution (Size 4) " + planeName;
        h2DClusterCharge4RowsClusterSize4_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge2Rows50x300");

        hName  = "h2DCellClusterCharge2Rows_"                         + planeName;
        hTitle = "2 Rows cell cluster charge 2D distribution" + planeName;
        h2DCellClusterCharge2Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge4Rows25x600");

        hName  = "h2DAllTracks4Rows_"                   + planeName;
        hTitle = "4 Rows Cell charge normalization (All tracks) "      + planeName;
        h2DAllTracks4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge2Rows50x300");

        hName  = "h2DAllTracks2Rows_"                   + planeName;
        hTitle = "2 Rows Cell charge normalization (All tracks) "      + planeName;
        h2DAllTracks2Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge4Rows25x600");

        hName  = "h2DCellCharge4RowsOnly50_"                         + planeName;
        hTitle = "Row 50 cell charge 2D distribution" + planeName;
        h2DCellCharge4RowsOnly50_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellCharge4RowsNorm_"                         + planeName;
        hTitle = "4 Rows Cell Charge Normalization  " + planeName;
        h2DCellCharge4RowsNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsSameRow_"                    + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, second hit (Same Row) " + planeName;
        h2DCellChargeSecondHit4RowsSameRow_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsSameRowNorm_"                         + planeName;
        hTitle = "4 Rows Cell Charge Normalization Second Hit (Same Row) " + planeName;
        h2DCellChargeSecondHit4RowsSameRowNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsSameColNorm_"                         + planeName;
        hTitle = "4 Rows Cell Charge Normalization Second Hit (Same Column) " + planeName;
        h2DCellChargeSecondHit4RowsSameColNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsSameCol_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, second hit (Same Column) " + planeName;
        h2DCellChargeSecondHit4RowsSameCol_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3Norm_"                         + planeName;
        hTitle = "4 Rows Cell Charge Normalization Second Hit (Cluster Size 3 ) " + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3Norm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, second hit (ClusterSize 3) " + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3LowCharge_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, ClusterSize 3 Low Charge " + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, ClusterSize 3 Low Charge Normalization" + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3HighCharge_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, ClusterSize 3: High Charge " + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, ClusterSize 3 High Charge Normalization" + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize1_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize1" + planeName;
        h2DCellPixelCharge4RowsClusterSize1_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsExcept50ClusterSize1_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize1 (Row 50 excluded) " + planeName;
        h2DCellPixelCharge4RowsExcept50ClusterSize1_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize2_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize2" + planeName;
        h2DCellPixelCharge4RowsClusterSize2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize3_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize3" + planeName;
        h2DCellPixelCharge4RowsClusterSize3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize4_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize4" + planeName;
        h2DCellPixelCharge4RowsClusterSize4_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize1Norm_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize1Norm" + planeName;
        h2DCellPixelCharge4RowsClusterSize1Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize1Norm (row 50 excluded) " + planeName;
        h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize2Norm_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize2Norm" + planeName;
        h2DCellPixelCharge4RowsClusterSize2Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize3Norm_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize3Norm" + planeName;
        h2DCellPixelCharge4RowsClusterSize3Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize4Norm_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize4Norm" + planeName;
        h2DCellPixelCharge4RowsClusterSize4Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)4*(yPitch/binSize - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_"                         + planeName;
        hTitle = "4 Rows Cluster Size 4 pixel charge row 1 vs row 2" + planeName;
        h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_"                         + planeName;
        hTitle = "4 Rows Cluster Size 4 pixel charge row 2 vs row 3" + planeName;
        h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_"                         + planeName;
        hTitle = "4 Rows Cluster Size 4 pixel charge row 4 vs row 3" + planeName;
        h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 4 pixel charge rows 1 and 4 vs rows 2 and 3, external Rows" + planeName;
        h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 4 pixel charge rows 1 and 4 vs rows 2 and 3, internal Rows" + planeName;
        h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_"                         + planeName;
        hTitle = "4 Rows Cluster Size 3 pixel charge row 1 vs row 2" + planeName;
        h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_"                         + planeName;
        hTitle = "4 Rows Cluster Size 3 pixel charge row 2 vs row 3" + planeName;
        h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_"      + planeName;
        hTitle = "4 Rows Cluster Size 3 pixel charge row 4 vs row 3" + planeName;
        h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 3 pixel charge rows 1 and 4 vs rows 2 and 3, external Rows" + planeName;
        h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 3 pixel charge rows 1 and 4 vs rows 2 and 3, internal Rows" + planeName;
        h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 1 vs row 2" + planeName;
        h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 2 vs row 3" + planeName;
        h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 4 vs row 3" + planeName;
        h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge rows 1 and 4 vs rows 2 and 3, external Rows" + planeName;
        h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge rows 1 and 4 vs rows 2 and 3, internal Rows" + planeName;
        h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 1 vs row 2 (restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow1vsRowAnd2_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 1 vs rows 1 and 2 (restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow1vsRows1And2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow2vsRowAnd2_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 2 vs rows 1 and 2 (restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow2vsRows1And2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 1 vs row 2 (From 1 - restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 1 vs row 2 (From 2 - restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 4 vs rows 4 and 3 (restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 3 vs rows 4 and 3 (restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 4 vs row 3 (restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 4 vs row 3 (From 4 - restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        hName  = "h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_"                         + planeName;
        hTitle = "4 Rows Cluster Size 2 pixel charge row 4 vs row 3 (From 3 - restricted area) " + planeName;
        h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));

        theAnalysisManager_->cd("Charge/" + planeName + "/2DCellCharge2Rows50x300");

        hName  = "h2DCellCharge2RowsOnly50_"                         + planeName;
        hTitle = "Row 50 cell charge 2D distribution" + planeName;
        h2DCellCharge2RowsOnly50_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellCharge2RowsNorm_"                         + planeName;
        hTitle = "2 Rows Cell Charge Normalization  " + planeName;
        h2DCellCharge2RowsNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsSameRow_"                    + planeName;
        hTitle = "2 Rows Cell charge 2D distribution, second hit (Same Row) " + planeName;
        h2DCellChargeSecondHit2RowsSameRow_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsSameRowNorm_"                         + planeName;
        hTitle = "2 Rows Cell Charge Normalization Second Hit (Same Row) " + planeName;
        h2DCellChargeSecondHit2RowsSameRowNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsSameColNorm_"                         + planeName;
        hTitle = "2 Rows Cell Charge Normalization Second Hit (Same Column) " + planeName;
        h2DCellChargeSecondHit2RowsSameColNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsSameCol_"                         + planeName;
        hTitle = "2 Rows Cell charge 2D distribution, second hit (Same Column) " + planeName;
        h2DCellChargeSecondHit2RowsSameCol_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsNormClusterSize3_"                         + planeName;
        hTitle = "2 Rows Cell Charge Normalization Second Hit (Cluster Size 3 ) " + planeName;
        h2DCellChargeSecondHit2RowsClusterSize3Norm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3_"                         + planeName;
        hTitle = "2 Rows Cell charge 2D distribution, second hit (ClusterSize 3) " + planeName;
        h2DCellChargeSecondHit2RowsClusterSize3_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3LowCharge_"                         + planeName;
        hTitle = "2 Rows Cell charge 2D distribution, ClusterSize 3 Low Charge " + planeName;
        h2DCellChargeSecondHit2RowsClusterSize3LowCharge_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_"                         + planeName;
        hTitle = "2 Rows Cell charge 2D distribution, ClusterSize 3 Low Charge Normalization" + planeName;
        h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3HighCharge_"                         + planeName;
        hTitle = "2 Rows Cell charge 2D distribution, ClusterSize 3: High Charge " + planeName;
        h2DCellChargeSecondHit2RowsClusterSize3HighCharge_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_"                         + planeName;
        hTitle = "2 Rows Cell charge 2D distribution, ClusterSize 3 High Charge Normalization" + planeName;
        h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge2RowsClusterSize1_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution ClusterSize1" + planeName;
        h2DCellPixelCharge2RowsClusterSize1_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge2RowsClusterSize2_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution ClusterSize2" + planeName;
        h2DCellPixelCharge2RowsClusterSize2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge2RowsClusterSize3_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution ClusterSize3" + planeName;
        h2DCellPixelCharge2RowsClusterSize3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge2RowsClusterSize4_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution ClusterSize4" + planeName;
        h2DCellPixelCharge2RowsClusterSize4_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge2RowsClusterSize1Norm_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution ClusterSize1Norm" + planeName;
        h2DCellPixelCharge2RowsClusterSize1Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge2RowsClusterSize2Norm_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution ClusterSize2Norm" + planeName;
        h2DCellPixelCharge2RowsClusterSize2Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge2RowsClusterSize3Norm_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution ClusterSize3Norm" + planeName;
        h2DCellPixelCharge2RowsClusterSize3Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DCellPixelCharge2RowsClusterSize4Norm_"                         + planeName;
        hTitle = "2 Rows cell pixel charge 2D distribution ClusterSize4Norm" + planeName;
        h2DCellPixelCharge2RowsClusterSize4Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)2*(yPitch/binSize - removedBins), -2*((yPitch/2) + removeHalf), 2*(yPitch/2 - removeHalf))));

        hName  = "h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_"                         + planeName;
        hTitle = "2 Rows Cluster Size 2 pixel charge row 1 vs row 2 " + planeName;
        h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, 0, 50000, 100, 0, 50000)));


        theAnalysisManager_->cd("Charge/" + planeName);

        /*------------------------------------------------------------------------2D cell charge landau--------------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("2DCellChargeLandau");

        std::stringstream sName;
        hCellChargeCoarseLandau_.push_back(std::vector<TH1F*>());
        for(int x=0; x<(int)xPitch/(binSize*2.); x++)
            for(int y=0; y<(int)yPitch/(binSize*2.); y++)
            {
                sName.str("");
                sName << -(xPitch/2) + 5 + x*10 << "," << -(yPitch/2) + 5 + y*10;
                hName  = "hCellChargeCoarseLandau_"  + sName.str()  + planeName;
                hTitle = "Cell charge Landau ("      + sName.str()  + ")" + planeName;
                hCellChargeCoarseLandau_[hCellChargeCoarseLandau_.size()-1].push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));
            }


        theAnalysisManager_->cd("Charge/" + planeName);

        /*--------------------------------------------------------------------------2D cell charge - X coordinate---------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("XcellCharge2D");

        hName  = "h2DXcellCharge_"                                      + planeName;
        hTitle = "Predicted cell charge vs. X coordinate "              + planeName;
        h2DXcellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, 100, -10000, 100000)));

        hName  = "h2DXcellChargeSecondHit_"                             + planeName;
        hTitle = "Predicted cell charge vs. X coordinate "              + planeName;
        h2DXcellChargeSecondHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, 100, -10000, 100000)));

        hName  = "h2DXcellChargeSumLE2_"                                + planeName;
        hTitle = "Up to 2 adjacent hits total charge vs. X coordinate " + planeName;
        h2DXcellChargeSumLE2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2, 100, -10000, 100000)));

        hName  = "h2DXcellChargeSumLE3_"                                + planeName;
        hTitle = "Up to 3 adjacent hits total charge vs. X coordinate " + planeName;
        h2DXcellChargeSumLE3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2, 100, -10000, 100000)));

        hName  = "h2DXcellDoubleHits_"                                  + planeName;
        hTitle = "Double hits distribution vs. X coordinate "           + planeName;
        h2DXcellDoubleHits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "h2DXcellSingleHits_"                                  + planeName;
        hTitle = "Single hits distribution vs. X coordinate "           + planeName;
        h2DXcellSingleHits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "hHitsNotONRowColVsXSlope_" + planeName;
        hTitle = "x coordinate vs x slope for hits whose tracks don't point at the same pixel" + planeName;
        hHitsNotONRowColVsXSlope_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2, 200, -0.000015,  0.000015)));

        hName  = "hHitsNotONRowColProjX_" + planeName;
        hTitle = "x distribution for hits whose tracks don't point at the same pixel" + planeName;
        hHitsNotOnRowColProjX_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*------------------------------------------------------------------------2D cell charge - Y coordinate----------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("YcellCharge2D");

        hName  = "h2DYcellCharge_"                                      + planeName;
        hTitle = "Predicted cell charge vs. Y coordinate "              + planeName;
        h2DYcellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2, -(yPitch/2), yPitch/2, 100, -10000, 100000)));

        hName  = "h2DYcellChargeSecondHit_"                             + planeName;
        hTitle = "Predicted cell charge vs. Y coordinate "              + planeName;
        h2DYcellChargeSecondHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2, -(yPitch/2), yPitch/2, 100, -10000, 100000)));

        hName  = "h2DYcellChargeSumLE2_"                                + planeName;
        hTitle = "Up to 2 adjacent hits total charge vs. Y coordinate " + planeName;
        h2DYcellChargeSumLE2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0., 100, -10000, 100000)));

        hName  = "h2DYcellChargeSumLE3_"                                + planeName;
        hTitle = "Up to 3 adjacent hits total charge vs. Y coordinate " + planeName;
        h2DYcellChargeSumLE3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0., 100, -10000, 100000)));

        hName  = "h2DYcellDoubleHits_"                                  + planeName;
        hTitle = "Double hits vs. Y coordinate "                        + planeName;
        h2DYcellDoubleHits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0., (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "h2DYcellSingleHits_"                                  + planeName;
        hTitle = "Single hits vs. Y coordinate "                        + planeName;
        h2DYcellSingleHits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0., (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf)));

        hName  = "hHitsNotONRowColVsYSlope_" + planeName;
        hTitle = "y coordinate vs y slope for hits whose tracks don't point at the same pixel" + planeName;
        hHitsNotONRowColVsYSlope_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2, 200, -0.000015,  0.000015)));

        hName  = "hHitsNotONRowColProjY_" + planeName;
        hTitle = "y distribution for hits whose tracks don't point at the same pixel" + planeName;
        hHitsNotOnRowColProjY_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*---------------------------------------------------------------------------1D cell charge - X coordinate--------------------------------------------------------------------- -----------------------*/
        theAnalysisManager_->mkdir("XcellCharge1D");

        hName  = "h1DXcellCharge_"                                                             + planeName;
        hTitle = "Predicted cell charge - X coordinate (normalized to hits) "                  + planeName;
        h1DXcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeNormToAll_"                                                    + planeName;
        hTitle = "Predicted cell charge - X coordinate (normalized to tracks) "                + planeName;
        h1DXcellChargeNormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeSumLE2_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge - X coordinate (normalized to hits) "     + planeName;
        h1DXcellChargeSumLE2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeSumLE2NormToAll_"                                              + planeName;
        hTitle = "Up to 2 adjacent hits total charge - X coordinate (normalized to tracks) "   + planeName;
        h1DXcellChargeSumLE2NormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeSumLE3_"                                                       + planeName;
        hTitle = "Up to 3 adjacent hits total charge - X coordinate (normalized to hits) "     + planeName;
        h1DXcellChargeSumLE3_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeSumLE3NormToAll_"                                              + planeName;
        hTitle = "Up to 3 adjacent hits total charge - X coordinate (normalized to tracks) "   + planeName;
        h1DXcellChargeSumLE3NormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeSecondHit_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge, second hit - X coordinate (normalized to tracks) "     + planeName;
        h1DXcellChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXallTracks_"                                                              + planeName;
        hTitle = "All tracks normalization - X coordinate "                                    + planeName;
        h1DXallTracks_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeNorm_"                                                         + planeName;
        hTitle = "Predicted cell charge - X coordinate - all hits normalization "                + planeName;
        h1DXcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeSumLE2Norm_"                                                   + planeName;
        hTitle = "Up to 2 adjacent hits total charge - X coordinate - all hits normalization " + planeName;
        h1DXcellChargeSumLE2Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeSumLE3Norm_"                                                   + planeName;
        hTitle = "Up to 3 adjacent hits total charge - X coordinate - all hits normalization " + planeName;
        h1DXcellChargeSumLE3Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellSingleHits_"                                                         + planeName;
        hTitle = "Single hits distribution - X coordinate "                                    + planeName;
        h1DXcellSingleHits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellDoubleHits_"                                                         + planeName;
        hTitle = "Double hits distribution - X coordinate "                                    + planeName;
        h1DXcellDoubleHits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcell3Hits_"                                                              + planeName;
        hTitle = "3 adjacent hits distribution - X coordinate "                                + planeName;
        h1DXcell3Hits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellChargeSecondHitNorm_"                                                    + planeName;
        hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate "     + planeName;
        h1DXcellChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*---------------------------------------------------------------------------1D cell charge - Y coordinate-------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("YcellCharge1D");

        hName  = "h1DYcellCharge_"                                                             + planeName;
        hTitle = "Predicted cell charge - Y coordinate (normalized to hits) "                  + planeName;
        h1DYcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));

        hName  = "h1DYcellChargeNormToAll_"                                                    + planeName;
        hTitle = "Predicted cell charge - Y coordinate (normalized to tracks) "                + planeName;
        h1DYcellChargeNormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellChargeSumLE2_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge - Y coordinate (normalized to hits) "     + planeName;
        h1DYcellChargeSumLE2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellChargeSumLE2NormToAll_"                                              + planeName;
        hTitle = "Up to 2 adjacent hits total charge - Y coordinate (normalized to tracks) "   + planeName;
        h1DYcellChargeSumLE2NormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellChargeSumLE3_"                                                       + planeName;
        hTitle = "Up to 3 adjacent hits total charge - Y coordinate (normalized to hits) "     + planeName;
        h1DYcellChargeSumLE3_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellChargeSumLE3NormToAll_"                                              + planeName;
        hTitle = "Up to 3 adjacent hits total charge - Y coordinate (normalized to tracks)"    + planeName;
        h1DYcellChargeSumLE3NormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellChargeSecondHit_"                                                                  + planeName;
        hTitle = "Up to 2 adjacent hits total charge, second hit - Y coordinate (normalized to tracks) "     + planeName;
        h1DYcellChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));

        hName  = "h1DYallTracks_"                                                              + planeName;
        hTitle = "All tracks normalization - Y coordinate "                                    + planeName;
        h1DYallTracks_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) +1,yPitch/2 - 0)));

        hName  = "h1DYallTracksSize2_"                                                              + planeName;
        hTitle = "All tracks normalization - Y coordinate (Size 2) "                                    + planeName;
        h1DYallTracksSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200,-100.,100.)));

        hName  = "h1DYallTracksNoElectrodes_"                                                              + planeName;
        hTitle = "All tracks normalization - Y coordinate "                                    + planeName;
        h1DYallTracksNoElectrodes_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellChargeNorm_"                                                         + planeName;
        hTitle = "Predicted cell charge - Y coordinate - all hits normalization "              + planeName;
        h1DYcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));

        hName  = "h1DYcellChargeSumLE2Norm_"                                                   + planeName;
        hTitle = "Up to 2 adjacent hits total charge - Y coordinate - all hits normalization " + planeName;
        h1DYcellChargeSumLE2Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellChargeSumLE3Norm_"                                                   + planeName;
        hTitle = "Up to 3 adjacent hits total charge - Y coordinate - all hits normalization " + planeName;
        h1DYcellChargeSumLE3Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellSingleHits_"                                                         + planeName;
        hTitle = "Single hits distribution - Y coordinate "                                    + planeName;
        h1DYcellSingleHits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellDoubleHits_"                                                         + planeName;
        hTitle = "Double hits distribution - Y coordinate "                                    + planeName;
        h1DYcellDoubleHits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcell3Hits_"                                                              + planeName;
        hTitle = "3 adjacent hits distribution - Y coordinate "                                + planeName;
        h1DYcell3Hits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2 - 0,-(yPitch/2) + 1.,yPitch/2 - 0.)));

        hName  = "h1DYcellChargeSecondHitNorm_"                                                    + planeName;
        hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate "     + planeName;
        h1DYcellChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*----------------------------------------------------------------------------X Asimmetry-----------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("XAsimmetry");

        hName  = "hXAsimmetry_"                                                     + planeName;
        hTitle = "Distribution of charge asimmetry values in X coordinate "         + planeName;
        hXAsimmetry_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 100, -1, 1)));

        hName  = "hXAsimmetry0_"                                                     + planeName;
        hTitle = "Distribution of one-sided charge asimmetry values in X coordinate "         + planeName;
        hXAsimmetry0_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 100, 0, 1)));

        hName  = "h2DXAsimmetryLandau_"                                             + planeName;
        hTitle = "L/R charge asimmetry vs charge - X coordinate "                   + planeName;
        h2DXAsimmetryLandau_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 92, 5000, 120000, 20, -1, 1)));



	// @@@ Please do not modify this code @@@
        hName  = "h2DXcellChargeAsimmetry_"                                         + planeName;
        hTitle = "L/R charge asimmetry - X coordinate "                             + planeName;
        h2DXcellChargeAsimmetry_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), xPitch, -xPitch/2, xPitch/2, 100, -1.1, 1.1)));

        hName  = "h2DXcellChargeAsimmetryInv_"                                      + planeName;
        hTitle = "L/R charge asimmetry - X coordinate "                             + planeName;
        h2DXcellChargeAsimmetryInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, -1.1, 1.1, xPitch, -xPitch/2, xPitch/2)));
	// ======================================



        hName  = "h2DXcellChargeAsimmetryUnconstrained_"                            + planeName;
        hTitle = "L/R unconstrained charge asimmetry - X coordinate "               + planeName;
        h2DXcellChargeAsimmetryUnconstrained_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, 100, -1, 1)));

        hName  = "h2DXcellChargeAsimmetryUnconstrainedInv_"                         + planeName;
        hTitle = "L/R unconstrained charge asimmetry - X coordinate "               + planeName;
        h2DXcellChargeAsimmetryUnconstrainedInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 20, -1, 1, (int)xPitch/2, -(xPitch/2), xPitch/2)));

        hName  = "h2DXcellChargeAsimmetrySizeLE2_"                                  + planeName;
        hTitle = "L/R charge asimmetry for cluster size <= 2 "                      + planeName;
        h2DXCellChargeAsimmetrySizeLE2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)xPitch/2 - 0, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, 102, -1.1, 1.1)));

        hName  = "h2DXCellChargeAsimmetryCell_"                                      + planeName;
        hTitle = "Distribution of mean x asymmetry on cell "                             + planeName;
        h2DXCellChargeAsimmetryCell_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/5, -(xPitch/2), xPitch/2, (int)yPitch/5, -(yPitch/2), yPitch/2)));

        hName  = "h2DXCellChargeAsimmetryCellNorm_"                                      + planeName;
        hTitle = "Distribution of mean x asymmetry on cell, normalization "                             + planeName;
        h2DXCellChargeAsimmetryCellNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/5, -(xPitch/2), xPitch/2, (int)yPitch/5, -(yPitch/2), yPitch/2)));

        hName  = "h2DXCellChargeAsimmetryY_"                                         + planeName;
        hTitle = "L/R charge asimmetry - X coordinate - on Y "                             + planeName;
        h2DXCellChargeAsimmetryY_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/5, -(yPitch/2), yPitch/2, 102, -1.1, 1.1)));

        hName = "h1DXEtaDerivativeDistribution_" + planeName;
        hTitle = "Distribution of eta derivative function for coordinate x, " + planeName;
        h1DXEtaDerivativeDistribution_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), 2*(int)xPitch/2 - 0, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*---------------------------------------------------------------------------Y Asimmetry-------------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("YAsimmetry");

        hName  = "hYAsimmetry_"                                                     + planeName;
        hTitle = "Distribution of charge asimmetry values in Y coordinate "         + planeName;
        hYAsimmetry_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 100, -1, 1)));

        hName  = "hYAsimmetry0_"                                                     + planeName;
        hTitle = "Distribution of one-sided charge asimmetry values in Y coordinate "         + planeName;
        hYAsimmetry0_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 100, 0, 1)));

        hName  = "h2DYAsimmetryLandau_"                                             + planeName;
        hTitle = "L/R charge asimmetry vs charge - Y coordinate "                   + planeName;
        h2DYAsimmetryLandau_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 92, 5000, 120000, 20, -1, 1)));



	// @@@ Please do not modify this code @@@
        hName  = "h2DYcellChargeAsimmetry_"                                         + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate "                             + planeName;
        h2DYcellChargeAsimmetry_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), yPitch, -yPitch/2, yPitch/2 , 100, -1.1, 1.1)));

        hName  = "h2DYcellChargeAsimmetryInv_"                                      + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate "                             + planeName;
        h2DYcellChargeAsimmetryInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 100, -1.1, 1.1, yPitch, -yPitch/2, yPitch/2)));
	// ======================================



        theAnalysisManager_->cd("Charge/" + planeName);

        theAnalysisManager_->mkdir("YAsimmetry4Rows25x600");

        hName  = "h2DYcellChargeAsimmetryInv4Rows_"                                      + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate 4 Rows "                             + planeName;
        h2DYcellChargeAsimmetryInv4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, (int)200, -100, 100)));// range +- 100 is not beacause of the pitch's dimension but because of the error on the track

        hName  = "h2DYcellChargeAsimmetryInvCutOnEntries_"                                      + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate CutOnEntries "                             + planeName;
        h2DYcellChargeAsimmetryInvCutOnEntries_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, (int)200, -100, 100)));// range +- 100 is not beacause of the pitch's dimension but because of the error on the track

        hName  = "h2DYcellChargeAsimmetryInvRows1And2Of4Rows_"                                      + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate Rows1And2Of4Rows "                             + planeName;
        h2DYcellChargeAsimmetryInvRows1And2Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, (int)200, -100, 100)));// range +- 100 is not beacause of the pitch's dimension but because of the error on the track

        hName  = "h2DYcellChargeAsimmetryInvRows1And2Of4RowsCutOnEntries_"                                      + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate Rows1And2Of4Rows CutOnEntries "                             + planeName;
        h2DYcellChargeAsimmetryInvRows1And2Of4RowsCutOnEntries_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, (int)200, -100, 100)));// range +- 100 is not beacause of the pitch's dimension but because of the error on the track

        hName  = "h2DYcellChargeAsimmetryInvRows4And3Of4Rows_"                                      + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate Rows4And3Of4Rows "                             + planeName;
        h2DYcellChargeAsimmetryInvRows4And3Of4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, (int)200, -100, 100)));// range +- 100 is not beacause of the pitch's dimension but because of the error on the track

        hName  = "h2DYcellChargeAsimmetryInvRows4And3Of4RowsCutOnEntries_"                                      + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate Rows4And3Of4Rows CutOnEntries "                            + planeName;
        h2DYcellChargeAsimmetryInvRows4And3Of4RowsCutOnEntries_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, (int)200, -100, 100)));// range +- 100 is not beacause of the pitch's dimension but because of the error on the track

        hName  = "h2DYCellChargeAsimmetryUpRows4Rows_"                              + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate - Up Rows of 4 Rows "         + planeName;
        h2DYCellChargeAsimmetryUpRows4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch+1, -(yPitch/2)-0.5, yPitch/2+0.5 , 102, -1.1, 1.1)));

        hName  = "h2DYcellChargeAsimmetryInvUpRows4Rows_"                           + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate - Up Rows of 4 Rows "         + planeName;
        h2DYcellChargeAsimmetryInvUpRows4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, (int)yPitch - 0, -(yPitch/2)-0.5, yPitch/2+0.5)));

        hName  = "h2DYCellChargeAsimmetryDownRows4Rows_"                            + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate - Down Rows of 4 Rows "       + planeName;
        h2DYCellChargeAsimmetryDownRows4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch+1, -(yPitch/2)-0.5, yPitch/2+0.5 , 102, -1.1, 1.1)));

        hName  = "h2DYcellChargeAsimmetryInvDownRows4Rows_"                         + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate - Up Rows of 4 Rows "         + planeName;
        h2DYcellChargeAsimmetryInvDownRows4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, (int)yPitch - 0, -(yPitch/2)-0.5, yPitch/2+0.5)));

        theAnalysisManager_->cd("Charge/" + planeName+"/YAsimmetry/");

        hName  = "h2DYcellChargeAsimmetryUnconstrained_"                            + planeName;
        hTitle = "L/R unconstrained charge asimmetry - Y coordinate "               + planeName;
        h2DYcellChargeAsimmetryUnconstrained_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(),(int)yPitch/2 - 0, -(yPitch/2) + 1, yPitch/2 - 0, 100, -1, 1)));

        hName  = "h2DYcellChargeAsimmetryUnconstrainedInv_"                         + planeName;
        hTitle = "L/R unconstrained charge asimmetry - Y coordinate "               + planeName;
        h2DYcellChargeAsimmetryUnconstrainedInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 20, -1, 1, (int)yPitch/2 - 0, -(yPitch/2) + 1, yPitch/2 - 0)));

        hName  = "h2DYcellChargeAsimmetrySizeLE2_"                                  + planeName;
        hTitle = "L/R charge asimmetry for cluster size <= 2 "                      + planeName;
        h2DYCellChargeAsimmetrySizeLE2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)yPitch/2 - 0, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf, 102, -1.1, 1.1)));

        hName  = "h2DYCellChargeAsimmetryCell_"                                      + planeName;
        hTitle = "Distribution of mean y asymmetry on cell "                             + planeName;
        h2DYCellChargeAsimmetryCell_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/5, -(xPitch/2), xPitch/2, (int)yPitch/5, -(yPitch/2), yPitch/2)));

        hName  = "h2DYCellChargeAsimmetryCellNorm_"                                      + planeName;
        hTitle = "Distribution of mean y asymmetry on cell, normalization "                             + planeName;
        h2DYCellChargeAsimmetryCellNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/5, -(xPitch/2), xPitch/2, (int)yPitch/5, -(yPitch/2), yPitch/2)));

        hName  = "h2DYCellChargeAsimmetryX_"                                         + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate - on X "                             + planeName;
        h2DYCellChargeAsimmetryX_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/5, -(xPitch/2), xPitch/2, 102, -1.1, 1.1)));

        hName = "h1DYEtaDerivativeDistribution_" + planeName;
        hTitle = "Distribution of eta derivative function for coordinate y, " + planeName;
        h1DYEtaDerivativeDistribution_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), 2*(int)yPitch/2 - 0, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*---------------------------------------------------------------------------Run Number-------------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("RunNumber");
        /*
        TMap * aMapX = NEW_THREADED(TMap);
        std::stringstream ss;
        for (int i = 888; i < 921; ++i)
        {
            TObject * iObj;
            ss.str("");
            ss << i;
            iObj->SetName(ss.str().c_str());
            *aMapX(iObj) = new TVectorT<std::pair<Float_t, Float_t>* >;
        }
        mXMeanCharge_.push_back(NEW_THREADED(aMapX));

        hName = "XMeanCharge_" + planeName;
        hTitle = "Mean charge x position as function of run number, " + planeName;
        hXMeanCharge_.push_back(new TH1F(hName.c_str(), hTitle.c_str(), 35, 887, 922));

        TMap * aMapY = NEW_THREADED(TMap);
        for (int i = 888; i < 921; ++i)
        {
            TObject * iObj;
            ss.str("");
            ss << i;
            iObj->SetName(ss.str().c_str());
            *aMapY(iObj) = new TVectorT<std::pair<Float_t, Float_t>* >;
        }
        mYMeanCharge_.push_back(NEW_THREADED(aMapY);

        hName = "YMeanCharge_" + planeName;
        hTitle = "Mean charge y position as function of run number, " + planeName;
        hYMeanCharge_.push_back(new TH1F(hName.c_str(), hTitle.c_str(), 35, 887, 922));

        No time for this shit! */
        std::map<int, TH2F*> aMap;
        std::map<int, TH2F*> aMapNorm;
        std::stringstream sss;
        int minRunNumber = 1000000000;
        int maxRunNumber = -1;
        for(std::map<int,int>::iterator runIt = runNumberEntries_.begin(); runIt != runNumberEntries_.end(); runIt++)
        {
            sss.str("");
            sss << runIt->first;
            hName = "xyChargeDistribution_run" + sss.str() + "_" + planeName;
            hTitle = "Charge distribution, run number #" + sss.str() + ", " + planeName;
            aMap[runIt->first] = NEW_THREADED(TH2F (hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf));
            hName = "ChargeNormalization_run" + sss.str() + "_" + planeName;
            hTitle = "Normalization histogram, run number #" + sss.str() + ", " + planeName;
            aMapNorm[runIt->first] = NEW_THREADED(TH2F (hName.c_str(), hTitle.c_str(), (int)xPitch/binSize - removedBins, -(xPitch/2) + removeHalf, xPitch/2 - removeHalf, (int)yPitch/binSize - removedBins, -(yPitch/2) + removeHalf, yPitch/2 - removeHalf));

            if (runIt->first > maxRunNumber) maxRunNumber = runIt->first;
            if (runIt->first < minRunNumber) minRunNumber = runIt->first;
        }
        mXYMeanCharge_.push_back(aMap);
        mXYNorm_      .push_back(aMapNorm);

        hName = "XMeanCharge_" + planeName;
        hTitle = "Mean charge along x as function of run number, " + planeName;
        hXMeanCharge_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), maxRunNumber-minRunNumber+1, minRunNumber, maxRunNumber+1));
        hName = "YMeanCharge_" + planeName;
        hTitle = "Mean charge along y as function of run number, " + planeName;
        hYMeanCharge_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), maxRunNumber-minRunNumber+1, minRunNumber, maxRunNumber+1));

    }
}

