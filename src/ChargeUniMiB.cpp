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

#include "ChargeUniMiB.h"

#include "AnalysisManager.h"
#include "WindowsManager.h"
#include "Window.h"
#include "ThreadUtilities.h"
#include "PlanesMapping.h"
#include "MessageTools.h"
#include "XmlParser.h"
#include "XmlPlane.h"
#include "XmlAnalysis.h"
#include "HistogramWindow.h"

#include "Utilities.h"
#include "CalibrationsManager.h"

#include <TH2F.h>
#include <TF1.h>
#include <TStyle.h>

#include <iostream>
#include <iomanip>
#include <cmath>


// @@@ Hard coded parameters @@@
#define maxChargeDeltaRay 13600. // = 8000 (MPV Landau for 100 um bulk thickness) * 1.7
#define ONLYdoubleHITS    false  // Process only clusters of size 2
#define ETAhalfRANGE      0.5    // Eta fit range = [-ETAhalfRANGE, +ETAhalfRANGE]
// ============================


//=======================================================================
ChargeUniMiB::ChargeUniMiB(AnalysisManager* analysisManager, int nOfThreads) :
  Analysis(analysisManager, nOfThreads),
  thePlaneMapping_(0),
  theWindowsManager_(0),
  theXmlParser_(analysisManager->getXmlParser()),
  standardCutsPixelMinimumCharge_(0),
  standardCutsPixelMaximumCharge_(0),
  standardCutsClusterMinimumCharge_(0),
  standardCutsClusterMaximumCharge_(0)
{
  STDLINE("Running ChargeUniMiB analysis",ACCyan);

  thePlaneMapping_ = new PlanesMapping();
}

//=======================================================================
ChargeUniMiB::~ChargeUniMiB(void)
{
  if (thePlaneMapping_) delete thePlaneMapping_;
  
  destroy();
}

//=======================================================================
void ChargeUniMiB::destroy()
{
  if (Analysis::fDoNotDelete_) return;

  std::vector<TH1F*>::iterator it1;
  std::vector<TH2F*>::iterator it2;

  for(it1=hCellLandau_                   .begin(); it1!=hCellLandau_                   .end(); it1++) delete *it1; hCellLandau_                  .clear();
  for(it1=hCellLandauOdd_                .begin(); it1!=hCellLandauOdd_                .end(); it1++) delete *it1; hCellLandauOdd_               .clear();
  for(it1=hCellLandauOddLeft_            .begin(); it1!=hCellLandauOddLeft_            .end(); it1++) delete *it1; hCellLandauOddLeft_           .clear();
  for(it1=hCellLandauOddRight_           .begin(); it1!=hCellLandauOddRight_           .end(); it1++) delete *it1; hCellLandauOddRight_          .clear();
  for(it1=hCellLandauEven_               .begin(); it1!=hCellLandauEven_               .end(); it1++) delete *it1; hCellLandauEven_              .clear();
  for(it1=hCellLandauEvenLeft_           .begin(); it1!=hCellLandauEvenLeft_           .end(); it1++) delete *it1; hCellLandauEvenLeft_          .clear();
  for(it1=hCellLandauEvenRight_          .begin(); it1!=hCellLandauEvenRight_          .end(); it1++) delete *it1; hCellLandauEvenRight_         .clear();
  for(it1=hClusterSize_                  .begin(); it1!=hClusterSize_                  .end(); it1++) delete *it1; hClusterSize_                 .clear();

  for(it1=hLandauClusterSize1_           .begin(); it1!=hLandauClusterSize1_           .end(); it1++) delete *it1; hLandauClusterSize1_          .clear();
  for(it1=hLandauClusterSize2_           .begin(); it1!=hLandauClusterSize2_           .end(); it1++) delete *it1; hLandauClusterSize2_          .clear();
  for(it1=hLandauClusterSize2sameCol_    .begin(); it1!=hLandauClusterSize2sameCol_    .end(); it1++) delete *it1; hLandauClusterSize2sameCol_   .clear();
  for(it1=hLandauClusterSize2sameRow_    .begin(); it1!=hLandauClusterSize2sameRow_    .end(); it1++) delete *it1; hLandauClusterSize2sameRow_   .clear();
  for(it1=hCellLandauSinglePixel_        .begin(); it1!=hCellLandauSinglePixel_        .end(); it1++) delete *it1; hCellLandauSinglePixel_       .clear();


  for(it1=h1DXcellCharge_                .begin(); it1!=h1DXcellCharge_                .end(); it1++) delete *it1; h1DXcellCharge_               .clear();
  for(it1=h1DXcellChargeNorm_            .begin(); it1!=h1DXcellChargeNorm_            .end(); it1++) delete *it1; h1DXcellChargeNorm_           .clear();
  
  for(it1=h1DXcellChargeSecondHit_       .begin(); it1!=h1DXcellChargeSecondHit_       .end(); it1++) delete *it1; h1DXcellChargeSecondHit_      .clear();
  for(it1=h1DXcellChargeSecondHitNorm_   .begin(); it1!=h1DXcellChargeSecondHitNorm_   .end(); it1++) delete *it1; h1DXcellChargeSecondHitNorm_  .clear();
  
  for(it1=h1DYcellCharge_                .begin(); it1!=h1DYcellCharge_                .end(); it1++) delete *it1; h1DYcellCharge_               .clear();
  for(it1=h1DYcellChargeNorm_            .begin(); it1!=h1DXcellChargeNorm_            .end(); it1++) delete *it1; h1DXcellChargeNorm_           .clear();

  for(it1=h1DYcellChargeSecondHit_       .begin(); it1!=h1DYcellChargeSecondHit_       .end(); it1++) delete *it1; h1DYcellChargeSecondHit_      .clear();
  for(it1=h1DYcellChargeSecondHitNorm_   .begin(); it1!=h1DYcellChargeSecondHitNorm_   .end(); it1++) delete *it1; h1DYcellChargeSecondHitNorm_  .clear();


  for(it2=h2DClusterSize_                .begin(); it2!=h2DClusterSize_                .end(); it2++) delete *it2; h2DClusterSize_               .clear();

  for(it2=h2DCharge_                     .begin(); it2!=h2DCharge_                     .end(); it2++) delete *it2; h2DCharge_                    .clear();
  for(it2=h2DChargeNorm_                 .begin(); it2!=h2DChargeNorm_                 .end(); it2++) delete *it2; h2DChargeNorm_                .clear();

  for(it2=h2DCellCharge_                 .begin(); it2!=h2DCellCharge_                 .end(); it2++) delete *it2; h2DCellCharge_                .clear();
  for(it2=h2DCellChargeNorm_             .begin(); it2!=h2DCellChargeNorm_             .end(); it2++) delete *it2; h2DCellChargeNorm_            .clear();

  for(it2=h2DCellChargeOdd_              .begin(); it2!=h2DCellChargeOdd_              .end(); it2++) delete *it2; h2DCellChargeOdd_             .clear();
  for(it2=h2DCellChargeOddNorm_          .begin(); it2!=h2DCellChargeOddNorm_          .end(); it2++) delete *it2; h2DCellChargeOddNorm_         .clear();

  for(it2=h2DCellChargeEven_             .begin(); it2!=h2DCellChargeEven_             .end(); it2++) delete *it2; h2DCellChargeEven_            .clear();
  for(it2=h2DCellChargeEvenNorm_         .begin(); it2!=h2DCellChargeEvenNorm_         .end(); it2++) delete *it2; h2DCellChargeEvenNorm_        .clear();
  
  for(it2=h4CellsCharge_                 .begin(); it2!=h4CellsCharge_                 .end(); it2++) delete *it2; h4CellsCharge_                .clear();
  for(it2=h4CellsChargeNorm_             .begin(); it2!=h4CellsChargeNorm_             .end(); it2++) delete *it2; h4CellsChargeNorm_            .clear();
  
  for(it2=h2DXcellCharge_                .begin(); it2!=h2DXcellCharge_                .end(); it2++) delete *it2; h2DXcellCharge_               .clear();
  for(it2=h2DXcellChargeSecondHit_       .begin(); it2!=h2DXcellChargeSecondHit_       .end(); it2++) delete *it2; h2DXcellChargeSecondHit_      .clear();
  
  for(it2=h2DYcellCharge_                .begin(); it2!=h2DYcellCharge_                .end(); it2++) delete *it2; h2DYcellCharge_               .clear();
  for(it2=h2DYcellChargeSecondHit_       .begin(); it2!=h2DYcellChargeSecondHit_       .end(); it2++) delete *it2; h2DYcellChargeSecondHit_      .clear();

  
  for(it2=h2DXcellChargeAsymmetry_       .begin(); it2!=h2DXcellChargeAsymmetry_       .end(); it2++) delete *it2; h2DXcellChargeAsymmetry_      .clear();
  for(it1=h1DXcellChargeAsymmetry_       .begin(); it1!=h1DXcellChargeAsymmetry_       .end(); it1++) delete *it1; h1DXcellChargeAsymmetry_      .clear();
  for(it2=h2DXcellChargeAsymmetryInv_    .begin(); it2!=h2DXcellChargeAsymmetryInv_    .end(); it2++) delete *it2; h2DXcellChargeAsymmetryInv_   .clear();
  for(it1=h1DXcellChargeAsymmetryInv_    .begin(); it1!=h1DXcellChargeAsymmetryInv_    .end(); it1++) delete *it1; h1DXcellChargeAsymmetryInv_   .clear();
  
  for(it2=h2DYcellChargeAsymmetry_       .begin(); it2!=h2DYcellChargeAsymmetry_       .end(); it2++) delete *it2; h2DYcellChargeAsymmetry_      .clear();
  for(it1=h1DYcellChargeAsymmetry_       .begin(); it1!=h1DYcellChargeAsymmetry_       .end(); it1++) delete *it1; h1DYcellChargeAsymmetry_      .clear();
  for(it2=h2DYcellChargeAsymmetryInv_    .begin(); it2!=h2DYcellChargeAsymmetryInv_    .end(); it2++) delete *it2; h2DYcellChargeAsymmetryInv_   .clear();
  for(it1=h1DYcellChargeAsymmetryInv_    .begin(); it1!=h1DYcellChargeAsymmetryInv_    .end(); it1++) delete *it1; h1DYcellChargeAsymmetryInv_   .clear();
}

//=======================================================================
void ChargeUniMiB::setErrorsBar(int planeID)
{
  std::string       planeName = thePlaneMapping_->getPlaneName(planeID);
  std::stringstream hName;
  double            binError;
  int               nBins;


  theAnalysisManager_->cd("/Charge/" + planeName + "/XAsymmetry");

  hName.str(""); hName << "h1DXcellChargeAsymmetry_" << planeName;
  h1DXcellChargeAsymmetry_.push_back((TH1F*)h2DXcellChargeAsymmetry_[planeID]->ProfileX(hName.str().c_str(),1,-1));

  hName.str(""); hName << "h1DXcellChargeAsymmetryInv_" << planeName;
  h1DXcellChargeAsymmetryInv_.push_back((TH1F*)h2DXcellChargeAsymmetryInv_[planeID]->ProfileX(hName.str().c_str(),1,-1));


  theAnalysisManager_->cd("/Charge/" + planeName + "/YAsymmetry");

  hName.str(""); hName << "h1DYcellChargeAsymmetry_" << planeName;
  h1DYcellChargeAsymmetry_.push_back((TH1F*)h2DYcellChargeAsymmetry_[planeID]->ProfileX(hName.str().c_str(),1,-1));

  hName.str(""); hName << "h1DYcellChargeAsymmetryInv_" << planeName;
  h1DYcellChargeAsymmetryInv_.push_back((TH1F*)h2DYcellChargeAsymmetryInv_[planeID]->ProfileX(hName.str().c_str(),1,-1));


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
void ChargeUniMiB::clusterSize(int planeID, const Data& data, int threadNumber)
{
  if (!data.getIsInDetector(planeID) || !data.getHasHit(planeID)) return;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;


  THREADED(hClusterSize_[planeID])->Fill(data.getClusterSize(planeID));
}

//=======================================================================
void ChargeUniMiB::cellLandau(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int clusterSize = 1;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) != clusterSize) return;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  if (!theWindow->checkWindow(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber()) // Hits are in the window
      ||  data.getClusterPixelRow    (0,planeID) != rowPredicted                                                         // Track is on cluster
      ||  data.getClusterPixelCol    (0,planeID) != colPredicted                                                         // Track is on cluster
      || !data.getIsPixelCalibrated  (0,planeID)                                                                         // Pixels are calibrated
      ||  data.getClusterPixelCharge (0,planeID) < standardCutsPixelMinimumCharge_                                       // Charge is over threshold
      ||  data.getClusterPixelCharge (0,planeID) > standardCutsPixelMaximumCharge_)                                      // Maximum allowed charge for this physics
    return;


  THREADED(hCellLandau_[planeID])->Fill(data.getClusterCharge(planeID));

  if (((int)colPredicted)%2 == 0)
    {
      THREADED(hCellLandauEven_[planeID])->Fill(data.getClusterCharge(planeID));

      if (data.getXPixelResidualLocal(planeID) > 0) THREADED(hCellLandauEvenRight_[planeID])->Fill(data.getClusterCharge(planeID));
      else                                          THREADED(hCellLandauEvenLeft_[planeID])->Fill(data.getClusterCharge(planeID));
    }
  else
    {
      THREADED(hCellLandauOdd_[planeID])->Fill(data.getClusterCharge(planeID));

      if (data.getXPixelResidualLocal(planeID) > 0) THREADED(hCellLandauOddRight_[planeID])->Fill(data.getClusterCharge(planeID));
      else                                          THREADED(hCellLandauOddLeft_[planeID])->Fill(data.getClusterCharge(planeID));
    }
}

//=======================================================================
void ChargeUniMiB::clusterLandau(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;
  int pixelCell = -1;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;
  int clusterSize = data.getClusterSize(planeID);


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < clusterSize; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
      	  || !data.getIsPixelCalibrated(h,planeID)                                                           // Pixels are calibrated
	  ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
	  ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
      	return;

      if (data.getClusterPixelRow(h,planeID) == rowPredicted && data.getClusterPixelCol(h,planeID) == colPredicted) pixelCell = h;
    }


  if      (clusterSize == 1) THREADED(hLandauClusterSize1_[planeID])->Fill(data.getClusterCharge(planeID));
  else if (clusterSize == 2) THREADED(hLandauClusterSize2_[planeID])->Fill(data.getClusterCharge(planeID));

  if      (pixelCell != -1)  THREADED(hCellLandauSinglePixel_[planeID])->Fill(data.getClusterPixelCharge(pixelCell, planeID));
}

//=======================================================================
void ChargeUniMiB::planeCharge(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;
  bool foundCluster = false;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;
  int clusterSize = data.getClusterSize(planeID);


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < clusterSize; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	  || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	  ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
	  ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
	return;

      if ((data.getClusterPixelRow(h,planeID) == rowPredicted) &&
	  (data.getClusterPixelCol(h,planeID) == colPredicted))
	foundCluster = true;
    }
  if (foundCluster == false) return;


  // #####################################################################
  // # Compute charge only for cells that are surrounded by "good" cells #
  // #####################################################################
  if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)))
    {
      THREADED(h2DChargeNorm_[planeID])->Fill(colPredicted,rowPredicted);
      if (data.getHasHit(planeID)) THREADED(h2DCharge_[planeID])->Fill(colPredicted,rowPredicted,data.getClusterCharge(planeID));
    }
}

//=======================================================================
void ChargeUniMiB::cellCharge(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float maxPitchX;
  float maxPitchY;
  float xPixelResidual;
  float yPixelResidual;
  float xPixelEdgeResidual = 0;
  float yPixelEdgeResidual = 0;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;
  int clusterSize = data.getClusterSize(planeID);

  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY) return;

  xPixelResidual = data.getXPixelResidualLocal(planeID);
  yPixelResidual = data.getYPixelResidualLocal(planeID);

  if (xPixelResidual > 0)       xPixelEdgeResidual = xPixelResidual - data.getXPitchLocal(planeID)/2;
  else if (xPixelResidual <= 0) xPixelEdgeResidual = xPixelResidual + data.getXPitchLocal(planeID)/2;

  if (yPixelResidual > 0)       yPixelEdgeResidual = yPixelResidual - data.getYPitchLocal(planeID)/2;
  else if (yPixelResidual <= 0) yPixelEdgeResidual = yPixelResidual + data.getYPitchLocal(planeID)/2;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < clusterSize; h++)
    {
      if (theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) &&
	  data.getClusterPixelRow   (h,planeID) == rowPredicted &&
	  data.getClusterPixelCol   (h,planeID) == colPredicted &&
	  data.getIsPixelCalibrated (h,planeID)                 &&
	  data.getClusterPixelCharge(h,planeID) > standardCutsPixelMinimumCharge_ &&
	  data.getClusterPixelCharge(h,planeID) < standardCutsPixelMaximumCharge_)
	{
	  THREADED(h4CellsCharge_    [planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual,data.getClusterPixelCharge(h,planeID));
	  THREADED(h4CellsChargeNorm_[planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual);

	  THREADED(h2DCellCharge_    [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(h,planeID));
	  THREADED(h2DCellChargeNorm_[planeID])->Fill(xPixelResidual,yPixelResidual);

	  if (((int)colPredicted)%2 == 0)
	    {	      
	      THREADED(h2DCellChargeEven_    [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(h,planeID));
	      THREADED(h2DCellChargeEvenNorm_[planeID])->Fill(xPixelResidual,yPixelResidual);
	    }
	  else
	    {
	      THREADED(h2DCellChargeOdd_    [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(h,planeID));
	      THREADED(h2DCellChargeOddNorm_[planeID])->Fill(xPixelResidual,yPixelResidual);
	    }

	  THREADED(h2DClusterSize_   [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterSize(planeID));
	}
    }
}

//=======================================================================
void ChargeUniMiB::xLandau(bool pass, int planeID, const Data &data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int clusterSize = 2;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) != clusterSize) return;
  

  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < clusterSize; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	  || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	  ||  data.getClusterPixelRow    (h,planeID) != rowPredicted                                         // Hits are on the same row (sharing is along the row - x direction)
	  ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
	  ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
	return;
    }

  
  THREADED(hLandauClusterSize2sameRow_[planeID])->Fill(data.getClusterCharge(planeID));
}

//=======================================================================
void ChargeUniMiB::yLandau(bool pass, int planeID, const Data &data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int clusterSize = 2;


  if (!pass || !data.getHasHit(planeID) || data.getClusterSize(planeID) != clusterSize) return;
  

  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < clusterSize; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	  || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	  ||  data.getClusterPixelCol    (h,planeID) != colPredicted                                         // Hits are on the same column (sharing is along the columm - y direction)
	  ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
	  ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
	return;
    }


  THREADED(hLandauClusterSize2sameCol_[planeID])->Fill(data.getClusterCharge(planeID));
}

//=======================================================================
void ChargeUniMiB::xChargeDivision(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float xRes = 0;
  float maxPitchX;
  float maxPitchY;


  if (!pass || !data.getIsInDetector(planeID)) return;

  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY) return;

  if (data.getXPixelResidualLocal(planeID) > 0)       xRes = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
  else if (data.getXPixelResidualLocal(planeID) <= 0) xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  bool myReturn = false;
  if ((theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) == true) && (data.getClusterSize(planeID) <= maxClusterSize))
    {
      for (int h = 0; h < data.getClusterSize(planeID); h++)
	{
	  if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	      || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	      ||  data.getClusterPixelRow    (h,planeID) != rowPredicted                                         // Hits are on the same row (sharing is along the row - x direction)
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
	      if (data.getClusterPixelRow(h,planeID) == rowPredicted && data.getClusterPixelCol(h,planeID) == colPredicted)
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
	      if (xRes <= 0 && data.getClusterPixelCol(h,planeID) - colPredicted == 1 && data.getClusterPixelRow(h,planeID) == rowPredicted)
		{
		  THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h, planeID));
		  break;
		}
	      else if (xRes > 0 && data.getClusterPixelCol(h,planeID) - colPredicted == -1 && data.getClusterPixelRow(h,planeID) == rowPredicted)
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
void ChargeUniMiB::xAsymmetry(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float maxPitchX;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;
  int clusterSize = data.getClusterSize(planeID);

  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX) return;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < clusterSize; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	  || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	  ||  data.getClusterPixelRow    (h,planeID) != rowPredicted                                         // Hits are on the same column (sharing is along the column - y direction)
	  ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
	  ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
	return;
    }


  if (clusterSize == 2)
    {
      float asymmetry   = 0;
      int   totalCharge = 0;
      int   chargeLeft  = 0;
      int   chargeRight = 0;
      float xPredicted  = data.getXPredictedLocal(planeID);
      float xMeasured   = (data.getXClusterPixelCenterLocal(0, planeID) + data.getXClusterPixelCenterLocal(1, planeID))/2;
      float xResidual   = xPredicted - xMeasured;
      
      if (data.getXClusterPixelCenterLocal(0, planeID) > data.getXClusterPixelCenterLocal(1, planeID))
	{
	  chargeRight = data.getClusterPixelCharge(0, planeID);
	  chargeLeft  = data.getClusterPixelCharge(1, planeID);
	}
      else if (data.getXClusterPixelCenterLocal(0, planeID) < data.getXClusterPixelCenterLocal(1, planeID))
	{
	  chargeRight = data.getClusterPixelCharge(1, planeID);
	  chargeLeft  = data.getClusterPixelCharge(0, planeID);
	}
      
      totalCharge = chargeLeft + chargeRight;
      
      if (totalCharge > 0 && totalCharge < maxChargeDeltaRay)
	{
	  asymmetry = (float)(chargeLeft - chargeRight) / (float)totalCharge;
	  
	  if (totalCharge >= standardCutsPixelMinimumCharge_ && totalCharge <= standardCutsPixelMaximumCharge_)
	    {
	      THREADED(h2DXcellChargeAsymmetry_   [planeID])->Fill(xResidual, asymmetry);
	      THREADED(h2DXcellChargeAsymmetryInv_[planeID])->Fill(asymmetry, xResidual);
	    }
	}
    }
}

//=======================================================================
  void ChargeUniMiB::yChargeDivision(bool pass, int planeID, const Data& data, int threadNumber)
  {
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float yRes = 0;
  float maxPitchX;
  float maxPitchY;


  if (!pass || !data.getIsInDetector(planeID)) return;
  
  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY) return;
    
  if (data.getYPixelResidualLocal(planeID) > 0)       yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
  else if (data.getYPixelResidualLocal(planeID) <= 0) yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  bool myReturn = false;
  if ((theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) == true) && (data.getClusterSize(planeID) <= maxClusterSize))
    {
      for (int h = 0; h < data.getClusterSize(planeID); h++)
	{
	  if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	      || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	      ||  data.getClusterPixelCol    (h,planeID) != colPredicted                                         // Hits are on the same column (sharing is along the column - y direction)
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
	      if (data.getClusterPixelRow(h,planeID) == rowPredicted && data.getClusterPixelCol(h,planeID) == colPredicted)
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
	      if (yRes <= 0 && data.getClusterPixelRow(h,planeID) - rowPredicted == 1 && data.getClusterPixelCol(h,planeID) == colPredicted)
		{
		  THREADED(h2DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
		  THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h, planeID));
		  break;
		}
	      else if (yRes > 0 && data.getClusterPixelRow(h,planeID) - rowPredicted == -1 && data.getClusterPixelCol(h,planeID) == colPredicted)
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
void ChargeUniMiB::yAsymmetry(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float maxPitchY;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;
  int clusterSize = data.getClusterSize(planeID);

  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getYPitchLocal(planeID) > maxPitchY) return;
  

  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < clusterSize; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	  || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
	  ||  data.getClusterPixelCol    (h,planeID) != colPredicted                                         // Hits are on the same column (sharing is along the column - y direction)
	  ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
	  ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
	return;
    }


  if (clusterSize == 2)
    {
      float asymmetry   = 0;
      int   totalCharge = 0;
      int   chargeDown  = 0;
      int   chargeUp    = 0;
      float yPredicted  = data.getYPredictedLocal(planeID);
      float yMeasured   = (data.getYClusterPixelCenterLocal(0, planeID) + data.getYClusterPixelCenterLocal(1, planeID))/2;
      float yResidual   = yPredicted - yMeasured;
      
      if (data.getYClusterPixelCenterLocal(0, planeID) > data.getYClusterPixelCenterLocal(1, planeID))
	{
	  chargeUp   = data.getClusterPixelCharge(0, planeID);
	  chargeDown = data.getClusterPixelCharge(1, planeID);
	}
      else if (data.getYClusterPixelCenterLocal(0, planeID) < data.getYClusterPixelCenterLocal(1, planeID))
	{
	  chargeUp   = data.getClusterPixelCharge(1, planeID);
	  chargeDown = data.getClusterPixelCharge(0, planeID);
	}

      totalCharge = chargeDown + chargeUp;

      if (totalCharge > 0 && totalCharge < maxChargeDeltaRay)
	{
	  asymmetry = (float)(chargeDown - chargeUp) / (float)totalCharge;
	  
	  if (totalCharge >= standardCutsPixelMinimumCharge_ && totalCharge <= standardCutsPixelMaximumCharge_)
	  {
            THREADED(h2DYcellChargeAsymmetry_   [planeID])->Fill(yResidual, asymmetry);
            THREADED(h2DYcellChargeAsymmetryInv_[planeID])->Fill(asymmetry, yResidual);
	  }
	}
    }
}

//=======================================================================
void ChargeUniMiB::setCutsFormula(std::map<std::string,std::string> cutsList,std::vector<TTree*> tree)
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

//=======================================================================
bool ChargeUniMiB::passCalibrationsCut(int planeID, const Data &data)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;


  if (!(theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->useCalibrations()) return true;
  if (data.getClusterSize(planeID) > maxClusterSize)                                             return false;

  return true;
}

//=======================================================================
bool ChargeUniMiB::passStandardCuts(int planeID, const Data &data)
{
  if (!theXmlParser_->getAnalysesFromString("Charge")->applyStandardCuts()) return true;

  int minHits   = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str()) - 1;
  int excludeMe = 0;
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) minHits += 1;
  else if (data.getHasHit(planeID) && data.getClusterSize(planeID) <= 2) excludeMe = 1;

  if (data.getNumberOfTelescopeHits() - excludeMe >= minHits) return true;
  else                                                        return false;
}

//=======================================================================
void ChargeUniMiB::beginJob(void)
{
  standardCutsPixelMinimumCharge_   = theXmlParser_->getAnalysesFromString("Charge")->getPixelMinimumCharge();
  standardCutsPixelMaximumCharge_   = theXmlParser_->getAnalysesFromString("Charge")->getPixelMaximumCharge();
  standardCutsClusterMinimumCharge_ = theXmlParser_->getAnalysesFromString("Charge")->getClusterMinimumCharge();
  standardCutsClusterMaximumCharge_ = theXmlParser_->getAnalysesFromString("Charge")->getClusterMaximumCharge();
  
  theWindowsManager_      = theAnalysisManager_->getWindowsManager();
  theCalibrationsManager_ = theAnalysisManager_->getCalibrationsManager();

  book();
}

//=======================================================================
void ChargeUniMiB::analyze(const Data& data, int threadNumber)
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
    

  // ######################################################
  // # Require all telescope planes with cluster size = 2 #
  // ######################################################
  if (ONLYdoubleHITS == true)
    {
      for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
	{
	  if ((thePlaneMapping_->getPlaneName(p).find("Dut")   == std::string::npos) && 
	      (thePlaneMapping_->getPlaneName(p).find("Strip") == std::string::npos) &&
	      (data.getClusterSize(p) != 2)) return;
	}
    }


  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      if (!passStandardCuts(p,data)) continue;

      if ((thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos) && (!passCalibrationsCut(p,data)))
	{
	  std::cout << __PRETTY_FUNCTION__ << "Calibration check not passed" << std::endl;
	  return;
	}

      clusterLandau   (clusterLandauCut,p,data,threadNumber);
      cellLandau      (cellLandauCut,   p,data,threadNumber);
      cellCharge      (cellChargeCut,   p,data,threadNumber);
      planeCharge     (true,            p,data,threadNumber);

      xLandau         (cellChargeXCut,  p,data,threadNumber);
      xChargeDivision (cellChargeXCut,  p,data,threadNumber);
      xAsymmetry      (cellChargeXCut,  p,data,threadNumber);

      yLandau         (cellChargeYCut,  p,data,threadNumber);
      yChargeDivision (cellChargeYCut,  p,data,threadNumber);
      yAsymmetry      (cellChargeYCut,  p,data,threadNumber);
    }
}

//=======================================================================
void ChargeUniMiB::endJob(void)
{
  std::stringstream ss;
  double charge;

  STDLINE("",ACWhite);

  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      std::string planeName = thePlaneMapping_->getPlaneName(p);
      ss.str("") ; ss << "Adding threads for plane " << p;
      STDLINE(ss.str().c_str(),ACYellow);

      ADD_THREADED(hCellLandau_                             [p]);
      ADD_THREADED(hCellLandauOdd_                          [p]);
      ADD_THREADED(hCellLandauOddLeft_                      [p]);
      ADD_THREADED(hCellLandauOddRight_                     [p]);
      ADD_THREADED(hCellLandauEven_                         [p]);
      ADD_THREADED(hCellLandauEvenLeft_                     [p]);
      ADD_THREADED(hCellLandauEvenRight_                    [p]);
      ADD_THREADED(hClusterSize_                            [p]);

      ADD_THREADED(hLandauClusterSize1_                     [p]);
      ADD_THREADED(hLandauClusterSize2_                     [p]);
      ADD_THREADED(hLandauClusterSize2sameRow_              [p]);
      ADD_THREADED(hLandauClusterSize2sameCol_              [p]);
      ADD_THREADED(hCellLandauSinglePixel_                  [p]);


      ADD_THREADED(h1DXcellCharge_                          [p]);
      ADD_THREADED(h1DXcellChargeNorm_                      [p]);

      ADD_THREADED(h1DXcellChargeSecondHit_                 [p]);
      ADD_THREADED(h1DXcellChargeSecondHitNorm_             [p]);

      ADD_THREADED(h1DYcellCharge_                          [p]);
      ADD_THREADED(h1DYcellChargeNorm_                      [p]);

      ADD_THREADED(h1DYcellChargeSecondHit_                 [p]);
      ADD_THREADED(h1DYcellChargeSecondHitNorm_             [p]);


      ADD_THREADED(h2DClusterSize_                          [p]);

      ADD_THREADED(h2DCharge_              	            [p]);
      ADD_THREADED(h2DChargeNorm_          	            [p]);

      ADD_THREADED(h2DCellCharge_                           [p]);
      ADD_THREADED(h2DCellChargeNorm_                       [p]);

      ADD_THREADED(h2DCellChargeOdd_                        [p]);
      ADD_THREADED(h2DCellChargeOddNorm_                    [p]);

      ADD_THREADED(h2DCellChargeEven_                       [p]);
      ADD_THREADED(h2DCellChargeEvenNorm_                   [p]);

      ADD_THREADED(h4CellsCharge_                           [p]);
      ADD_THREADED(h4CellsChargeNorm_                       [p]);

      ADD_THREADED(h2DXcellCharge_                          [p]);
      ADD_THREADED(h2DXcellChargeSecondHit_                 [p]);

      ADD_THREADED(h2DYcellCharge_                          [p]);
      ADD_THREADED(h2DYcellChargeSecondHit_                 [p]);


      ADD_THREADED(h2DXcellChargeAsymmetry_                 [p]);
      ADD_THREADED(h2DXcellChargeAsymmetryInv_              [p]);

      ADD_THREADED(h2DYcellChargeAsymmetry_                 [p]);
      ADD_THREADED(h2DYcellChargeAsymmetryInv_              [p]);

      STDLINE("Threading phase completed",ACGreen);


      STDLINE("Filling phase...",ACWhite);

      h1DXcellCharge_               [p]->Divide(h1DXcellChargeNorm_          [p]);
      h1DXcellChargeSecondHit_      [p]->Divide(h1DXcellChargeSecondHitNorm_ [p]);

      h1DYcellCharge_               [p]->Divide(h1DYcellChargeNorm_          [p]);
      h1DYcellChargeSecondHit_      [p]->Divide(h1DYcellChargeSecondHitNorm_ [p]);

      h2DCellCharge_                [p]->Divide(h2DCellChargeNorm_           [p]);
      h2DCharge_                    [p]->Divide(h2DChargeNorm_               [p]);
      h2DCellChargeOdd_             [p]->Divide(h2DCellChargeOddNorm_        [p]);
      h2DCellChargeEven_            [p]->Divide(h2DCellChargeEvenNorm_       [p]);
      h2DClusterSize_               [p]->Divide(h2DCellChargeNorm_           [p]);
      h4CellsCharge_                [p]->Divide(h4CellsChargeNorm_           [p]);


      // ######################
      // # Setting error bars #
      // ######################
      setErrorsBar(p);


      STDLINE("Setting styles...",ACWhite);

      float xPitch = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
      float yPitch = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

      h1DXcellChargeAsymmetry_   [p]->SetMinimum(-1);
      h1DXcellChargeAsymmetry_   [p]->SetMaximum( 1);
      h1DXcellChargeAsymmetry_   [p]->SetMarkerStyle(20);
      h1DXcellChargeAsymmetry_   [p]->SetMarkerSize(0.6);

      h1DXcellChargeAsymmetryInv_[p]->SetMinimum(-xPitch/2);
      h1DXcellChargeAsymmetryInv_[p]->SetMaximum(xPitch/2);
      h1DXcellChargeAsymmetryInv_[p]->SetMarkerStyle(20);
      h1DXcellChargeAsymmetryInv_[p]->SetMarkerSize(0.6);

      h1DYcellChargeAsymmetry_   [p]->SetMinimum(-1);
      h1DYcellChargeAsymmetry_   [p]->SetMaximum( 1);
      h1DYcellChargeAsymmetry_   [p]->SetMarkerStyle(20);
      h1DYcellChargeAsymmetry_   [p]->SetMarkerSize(0.6);

      h1DYcellChargeAsymmetryInv_[p]->SetMinimum(-yPitch/2);
      h1DYcellChargeAsymmetryInv_[p]->SetMaximum(yPitch/2);
      h1DYcellChargeAsymmetryInv_[p]->SetMarkerStyle(20);
      h1DYcellChargeAsymmetryInv_[p]->SetMarkerSize(0.6);

      h1DXcellCharge_            [p]->SetMarkerStyle(20);
      h1DXcellCharge_            [p]->SetMarkerSize(0.6);

      h1DYcellCharge_            [p]->SetMarkerStyle(20);
      h1DYcellCharge_            [p]->SetMarkerSize(0.6);

      h1DXcellChargeSecondHit_   [p]->SetMarkerStyle(20);
      h1DXcellChargeSecondHit_   [p]->SetMarkerSize(0.6);

      h1DYcellChargeSecondHit_   [p]->SetMarkerStyle(20);
      h1DYcellChargeSecondHit_   [p]->SetMarkerSize(0.6);


      hClusterSize_              [p]->GetXaxis()->SetTitle("cluster size"      );
      hCellLandau_               [p]->GetXaxis()->SetTitle("charge (electrons)");
      hCellLandauOdd_            [p]->GetXaxis()->SetTitle("charge (electrons)");
      hCellLandauOddLeft_        [p]->GetXaxis()->SetTitle("charge (electrons)");
      hCellLandauOddRight_       [p]->GetXaxis()->SetTitle("charge (electrons)");
      hCellLandauEven_           [p]->GetXaxis()->SetTitle("charge (electrons)");
      hCellLandauEvenLeft_       [p]->GetXaxis()->SetTitle("charge (electrons)");
      hCellLandauEvenRight_      [p]->GetXaxis()->SetTitle("charge (electrons)");

      hLandauClusterSize1_       [p]->GetXaxis()->SetTitle("charge (electrons)");
      hLandauClusterSize2_       [p]->GetXaxis()->SetTitle("charge (electrons)");
      hLandauClusterSize2sameRow_[p]->GetXaxis()->SetTitle("charge (electrons)");
      hLandauClusterSize2sameCol_[p]->GetXaxis()->SetTitle("charge (electrons)");
      hCellLandauSinglePixel_    [p]->GetXaxis()->SetTitle("charge (electrons)");


      h2DClusterSize_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DClusterSize_            [p]->GetYaxis()->SetTitle("short pitch (um)"  );


      h2DCharge_                 [p]->GetXaxis()->SetTitle("column");
      h2DCharge_                 [p]->GetYaxis()->SetTitle("row"   );

      h2DChargeNorm_             [p]->GetXaxis()->SetTitle("column");
      h2DChargeNorm_             [p]->GetYaxis()->SetTitle("row"   );


      h2DCellCharge_             [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DCellCharge_             [p]->GetYaxis()->SetTitle("short pitch (um)"  );

      h2DCellChargeNorm_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DCellChargeNorm_         [p]->GetYaxis()->SetTitle("short pitch (um)"  );


      h2DCellChargeOdd_          [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DCellChargeOdd_          [p]->GetYaxis()->SetTitle("short pitch (um)"  );

      h2DCellChargeOddNorm_      [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DCellChargeOddNorm_      [p]->GetYaxis()->SetTitle("short pitch (um)"  );


      h2DCellChargeEven_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DCellChargeEven_         [p]->GetYaxis()->SetTitle("short pitch (um)"  );

      h2DCellChargeEvenNorm_     [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DCellChargeEvenNorm_     [p]->GetYaxis()->SetTitle("short pitch (um)"  );


      h4CellsCharge_             [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h4CellsCharge_             [p]->GetYaxis()->SetTitle("short pitch (um)"  );

      h4CellsChargeNorm_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h4CellsChargeNorm_         [p]->GetYaxis()->SetTitle("short pitch (um)"  );


      h1DXcellCharge_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellChargeNorm_        [p]->GetXaxis()->SetTitle("long pitch (um)"   );

      h1DXcellChargeSecondHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellChargeSecondHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellCharge_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellChargeSecondHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");


      h1DYcellCharge_            [p]->GetXaxis()->SetTitle("short pitch (um)"  );
      h1DYcellCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DYcellChargeNorm_        [p]->GetXaxis()->SetTitle("short pitch (um)"  );

      h1DYcellChargeSecondHit_   [p]->GetXaxis()->SetTitle("short pitch (um)"   );
      h1DYcellChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DYcellChargeSecondHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DYcellCharge_            [p]->GetXaxis()->SetTitle("short pitch (um)"  );
      h2DYcellCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DYcellChargeSecondHit_   [p]->GetXaxis()->SetTitle("short pitch (um)"  );
      h2DYcellChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");


      h2DXcellChargeAsymmetry_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellChargeAsymmetry_   [p]->GetYaxis()->SetTitle("Asymmetry"         );

      h1DXcellChargeAsymmetry_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellChargeAsymmetry_   [p]->GetYaxis()->SetTitle("Asymmetry"         );

      h2DXcellChargeAsymmetryInv_[p]->GetXaxis()->SetTitle("Asymmetry"         );
      h2DXcellChargeAsymmetryInv_[p]->GetYaxis()->SetTitle("long pitch (um)"   );

      h1DXcellChargeAsymmetryInv_[p]->GetXaxis()->SetTitle("Asymmetry"         );
      h1DXcellChargeAsymmetryInv_[p]->GetYaxis()->SetTitle("long pitch (um)"   );


      h2DYcellChargeAsymmetry_   [p]->GetXaxis()->SetTitle("shot pitch (um)"   );
      h2DYcellChargeAsymmetry_   [p]->GetYaxis()->SetTitle("Asymmetry"         );

      h1DYcellChargeAsymmetry_   [p]->GetXaxis()->SetTitle("short pitch (um)"  );
      h1DYcellChargeAsymmetry_   [p]->GetYaxis()->SetTitle("Asymmetry"         );

      h2DYcellChargeAsymmetryInv_[p]->GetXaxis()->SetTitle("Asymmetry"         );
      h2DYcellChargeAsymmetryInv_[p]->GetYaxis()->SetTitle("short pitch (um)"  );

      h1DYcellChargeAsymmetryInv_[p]->GetXaxis()->SetTitle("Asymmetry"         );
      h1DYcellChargeAsymmetryInv_[p]->GetYaxis()->SetTitle("short pitch (um)"  );


      STDLINE("Fitting phase",ACWhite);
      gStyle->SetOptFit(1111);

      STDLINE("fXAsymmetryFit",ACWhite);
      TF1* fXAsymmetryFit = new TF1("fXAsymmetryFit","pol1",-ETAhalfRANGE,ETAhalfRANGE);
      if (h1DXcellChargeAsymmetryInv_[p]->GetEntries() != 0) h1DXcellChargeAsymmetryInv_[p]->Fit(fXAsymmetryFit,"R");

      STDLINE("fYAsymmetryFit",ACWhite);
      TF1* fYAsymmetryFit  = new TF1("fYAsymmetryFit","pol1",-ETAhalfRANGE,ETAhalfRANGE);
      if (h1DYcellChargeAsymmetryInv_[p]->GetEntries() != 0) h1DYcellChargeAsymmetryInv_[p]->Fit(fYAsymmetryFit,"R");


      // ###############################
      // # Print mean charge on screen #
      // ###############################
      charge = hCellLandau_[p]->GetMean();

      ss.str("");
      ss << "Detector: " << std::setw(27) << thePlaneMapping_->getPlaneName(p) << " Mean charge: " << std::setw(4) << charge;

      STDLINE(ss.str(),ACLightPurple);
    }
}

//=======================================================================
void ChargeUniMiB::book(void)
{
  destroy();

  std::string hName;
  std::string hTitle;
  std::string planeName;
  std::stringstream ss;

  int nBinsX;
  int nBinsY;

  float xPitch;
  float yPitch;
  float binSize = 5; // [um]

  int lowerCol;
  int higherCol;
  int lowerRow;
  int higherRow;

  int nBinsCharge = 500;
  int nBinsCell   = 100;


  theAnalysisManager_->cd("/");
  theAnalysisManager_->mkdir("Charge");


  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      planeName = thePlaneMapping_->getPlaneName(p);
      theAnalysisManager_->cd("Charge");
      theAnalysisManager_->mkdir(planeName);


      xPitch    = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
      yPitch    = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

      lowerCol  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerCol ()).c_str());
      higherCol = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherCol()).c_str());
      lowerRow  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerRow ()).c_str());
      higherRow = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherRow()).c_str());

      nBinsX    = abs(lowerCol - higherCol) + 1;
      nBinsY    = abs(lowerRow - higherRow) + 1;

      if (nBinsY <= 0) nBinsY = 1; // Planes which are not in the geometry file have lowerRow = higherRow = 0,
                                   // this produces an unexpected warning




      theAnalysisManager_->mkdir("ClusterSize");


      // #################
      // # 1D histograms #
      // #################
      hName  = "hClusterSize_"              + planeName;
      hTitle = "Cluster size distribution " + planeName;
      hClusterSize_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 10, 0, 10)));




      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("Landau");


      // #################
      // # 1D histograms #
      // #################
      hName  = "hCellLandau_"                                              + planeName;
      hTitle = "Charge distribution for single hits in a fiducial window " + planeName;
      hCellLandau_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hCellLandauOdd_"                                                         + planeName;
      hTitle = "Charge distribution for single hits in a fiducial window - odd columns " + planeName;
      hCellLandauOdd_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hCellLandauOddLeft_"                                                          + planeName;
      hTitle = "Charge distribution for single hits in a fiducial window - odd-left columns " + planeName;
      hCellLandauOddLeft_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hCellLandauOddRight_"                                                          + planeName;
      hTitle = "Charge distribution for single hits in a fiducial window - odd-right columns " + planeName;
      hCellLandauOddRight_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hCellLandauEven_"                                                         + planeName;
      hTitle = "Charge distribution for single hits in a fiducial window - even columns " + planeName;
      hCellLandauEven_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hCellLandauEvenLeft_"                                                          + planeName;
      hTitle = "Charge distribution for single hits in a fiducial window - even-left columns " + planeName;
      hCellLandauEvenLeft_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hCellLandauEvenRight_"                                                          + planeName;
      hTitle = "Charge distribution for single hits in a fiducial window - even-right columns " + planeName;
      hCellLandauEvenRight_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hLandauClusterSize1_"                                      + planeName;
      hTitle = "Charge distribution for clusters of size 1 "               + planeName;
      hLandauClusterSize1_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hLandauClusterSize2_"                                      + planeName;
      hTitle = "Charge distribution for clusters of size 2 "               + planeName;
      hLandauClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hLandauClusterSize2sameRow_"                               + planeName;
      hTitle = "Charge distribution for clusters of size 2 on same row "   + planeName;
      hLandauClusterSize2sameRow_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hLandauClusterSize2sameCol_"                               + planeName;
      hTitle = "Charge distribution for clusters of size 2 on same col "   + planeName;
      hLandauClusterSize2sameCol_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));

      hName  = "hCellLandauSinglePixel_"                                   + planeName;
      hTitle = "Charge distribution of the pointed pixel in the cluster "  + planeName;
      hCellLandauSinglePixel_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsCharge, 0, 50000)));




      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("2DCharge");


      // #################
      // # 2D histograms #
      // #################
      hName  = "h2DCharge_"                                         + planeName;
      hTitle = "2D charge distribution "                            + planeName;
      h2DCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

      hName  = "h2DChargeNorm_"                                     + planeName;
      hTitle = "2D charge normalization "                           + planeName;
      h2DChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));




      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("2DCellCharge");


      // #################
      // # 2D histograms #
      // #################
      hName  = "h2DCellCharge_"                  + planeName;
      hTitle = "Cell charge 2D distribution "    + planeName;
      h2DCellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h2DCellChargeNorm_"              + planeName;
      hTitle = "Cell charge normalization "      + planeName;
      h2DCellChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h2DCellChargeOdd_"                           + planeName;
      hTitle = "Cell charge 2D distribution - odd columns "  + planeName;
      h2DCellChargeOdd_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h2DCellChargeOddNorm_"                       + planeName;
      hTitle = "Cell charge normalization - odd columns "    + planeName;
      h2DCellChargeOddNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h2DCellChargeEven_"                          + planeName;
      hTitle = "Cell charge 2D distribution - even columns " + planeName;
      h2DCellChargeEven_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h2DCellChargeEvenNorm_"                      + planeName;
      hTitle = "Cell charge normalization - even columns "   + planeName;
      h2DCellChargeEvenNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h4CellsCharge_"                  + planeName;
      hTitle = "4 cells charge 2D distribution " + planeName;
      h4CellsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h4CellsChargeNorm_"              + planeName;
      hTitle = "4 cells charge normalization "   + planeName;
      h4CellsChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h2DClusterSize_"                 + planeName;
      hTitle = "Cluster size distribution  "     + planeName;
      h2DClusterSize_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));




      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("XcellCharge2D");


      // #################
      // # 2D histograms #
      // #################
      hName  = "h2DXcellCharge_"                                      + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "              + planeName;
      h2DXcellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));

      hName  = "h2DXcellChargeSecondHit_"                             + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "              + planeName;
      h2DXcellChargeSecondHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));




      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("YcellCharge2D");


      // #################
      // # 2D histograms #
      // #################
      hName  = "h2DYcellCharge_"                                      + planeName;
      hTitle = "Predicted cell charge vs. Y coordinate "              + planeName;
      h2DYcellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2, -(yPitch/2), yPitch/2, nBinsCharge, 0, 50000)));

      hName  = "h2DYcellChargeSecondHit_"                             + planeName;
      hTitle = "Predicted cell charge vs. Y coordinate "              + planeName;
      h2DYcellChargeSecondHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2, -(yPitch/2), yPitch/2, nBinsCharge, 0, 50000)));




      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("XcellCharge1D");


      // #################
      // # 1D histograms #
      // #################
      hName  = "h1DXcellCharge_"                                                             + planeName;
      hTitle = "Predicted cell charge - X coordinate (normalized to hits) "                  + planeName;
      h1DXcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

      hName  = "h1DXcellChargeNorm_"                                                         + planeName;
      hTitle = "Predicted cell charge - X coordinate - all hits normalization "              + planeName;
      h1DXcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

      hName  = "h1DXcellChargeSecondHit_"                                                              + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit - X coordinate (normalized to tracks) " + planeName;
      h1DXcellChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

      hName  = "h1DXcellChargeSecondHitNorm_"                                                + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate " + planeName;
      h1DXcellChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));




      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("YcellCharge1D");


      // #################
      // # 1D histograms #
      // #################
      hName  = "h1DYcellCharge_"                                                             + planeName;
      hTitle = "Predicted cell charge - Y coordinate (normalized to hits) "                  + planeName;
      h1DYcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));

      hName  = "h1DYcellChargeNorm_"                                                         + planeName;
      hTitle = "Predicted cell charge - Y coordinate - all hits normalization "              + planeName;
      h1DYcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));

      hName  = "h1DYcellChargeSecondHit_"                                                              + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit - Y coordinate (normalized to tracks) " + planeName;
      h1DYcellChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));

      hName  = "h1DYcellChargeSecondHitNorm_"                                                + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate " + planeName;
      h1DYcellChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));




      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("XAsymmetry");


      // #################
      // # 1D histograms #
      // #################
      hName  = "h2DXcellChargeAsymmetry_"                                         + planeName;
      hTitle = "L/R charge asymmetry - X coordinate "                             + planeName;
      h2DXcellChargeAsymmetry_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch, -(xPitch/2), xPitch/2, nBinsCell, -1.1, 1.1)));

      hName  = "h2DXcellChargeAsymmetryInv_"                                      + planeName;
      hTitle = "L/R charge asymmetry - X coordinate "                             + planeName;
      h2DXcellChargeAsymmetryInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), nBinsCell, -1.1, 1.1, (int)xPitch, -(xPitch/2), xPitch/2)));


      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("YAsymmetry");

      hName  = "h2DYcellChargeAsymmetry_"                                         + planeName;
      hTitle = "L/R charge asymmetry - Y coordinate "                             + planeName;
      h2DYcellChargeAsymmetry_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch, -(yPitch/2), yPitch/2 , nBinsCell, -1.1, 1.1)));

      hName  = "h2DYcellChargeAsymmetryInv_"                                      + planeName;
      hTitle = "L/R charge asymmetry - Y coordinate "                             + planeName;
      h2DYcellChargeAsymmetryInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), nBinsCell, -1.1, 1.1, (int)yPitch, -(yPitch/2), yPitch/2)));
    }
}
