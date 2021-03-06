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

#include "ChargeAmericans.h"

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
#include <fstream>
#include <iomanip>
#include <cmath>


// @@@ Hard coded parameters @@@
#define maxChargeDeltaRay 13600. // = 8000 (MPV Landau for 100 um bulk thickness) * 1.7
#define ONLYdoubleHITS    false  // Process only clusters of size 2
#define ETAhalfRANGE      0.5    // Eta fit range = [-ETAhalfRANGE, +ETAhalfRANGE]
// ============================


//=======================================================================
ChargeAmericans::ChargeAmericans(AnalysisManager* analysisManager, int nOfThreads) :
  Analysis(analysisManager, nOfThreads),
  thePlaneMapping_(0),
  theWindowsManager_(0),
  theXmlParser_(analysisManager->getXmlParser()),
  thePitchTranslation25x100_(new SmallPitchTranslation25x100()),
  standardCutsPixelMinimumCharge_(0),
  standardCutsPixelMaximumCharge_(0),
  standardCutsClusterMinimumCharge_(0),
  standardCutsClusterMaximumCharge_(0)
{
  STDLINE("Running ChargeAmericans analysis",ACCyan);

  thePlaneMapping_ = new PlanesMapping();

}

//=======================================================================
ChargeAmericans::~ChargeAmericans(void)
{
  if (thePlaneMapping_) delete thePlaneMapping_;
  
  destroy();
}

//=======================================================================
void ChargeAmericans::destroy()
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
  
  for(it1=h1DXcellChargeFourthHit_       .begin(); it1!=h1DXcellChargeFourthHit_       .end(); it1++) delete *it1; h1DXcellChargeFourthHit_      .clear();
  for(it1=h1DXcellChargeFourthHitNorm_   .begin(); it1!=h1DXcellChargeFourthHitNorm_   .end(); it1++) delete *it1; h1DXcellChargeFourthHitNorm_  .clear();
  
  for(it1=h1DXcellOddCharge_                .begin(); it1!=h1DXcellOddCharge_                .end(); it1++) delete *it1; h1DXcellOddCharge_               .clear();
  for(it1=h1DXcellOddChargeNorm_            .begin(); it1!=h1DXcellOddChargeNorm_            .end(); it1++) delete *it1; h1DXcellOddChargeNorm_           .clear();
  
  for(it1=h1DXcellOddChargeSecondHit_       .begin(); it1!=h1DXcellOddChargeSecondHit_       .end(); it1++) delete *it1; h1DXcellOddChargeSecondHit_      .clear();
  for(it1=h1DXcellOddChargeSecondHitNorm_   .begin(); it1!=h1DXcellOddChargeSecondHitNorm_   .end(); it1++) delete *it1; h1DXcellOddChargeSecondHitNorm_  .clear();
  
  for(it1=h1DXcellOddChargeFourthHit_       .begin(); it1!=h1DXcellOddChargeFourthHit_       .end(); it1++) delete *it1; h1DXcellOddChargeFourthHit_      .clear();
  for(it1=h1DXcellOddChargeFourthHitNorm_   .begin(); it1!=h1DXcellOddChargeFourthHitNorm_   .end(); it1++) delete *it1; h1DXcellOddChargeFourthHitNorm_  .clear();
  
  for(it1=h1DXcellEvenCharge_                .begin(); it1!=h1DXcellEvenCharge_                .end(); it1++) delete *it1; h1DXcellEvenCharge_               .clear();
  for(it1=h1DXcellEvenChargeNorm_            .begin(); it1!=h1DXcellEvenChargeNorm_            .end(); it1++) delete *it1; h1DXcellEvenChargeNorm_           .clear();
  
  for(it1=h1DXcellEvenChargeSecondHit_       .begin(); it1!=h1DXcellEvenChargeSecondHit_       .end(); it1++) delete *it1; h1DXcellEvenChargeSecondHit_      .clear();
  for(it1=h1DXcellEvenChargeSecondHitNorm_   .begin(); it1!=h1DXcellEvenChargeSecondHitNorm_   .end(); it1++) delete *it1; h1DXcellEvenChargeSecondHitNorm_  .clear();
  
  for(it1=h1DXcellEvenChargeFourthHit_       .begin(); it1!=h1DXcellEvenChargeFourthHit_       .end(); it1++) delete *it1; h1DXcellEvenChargeFourthHit_      .clear();
  for(it1=h1DXcellEvenChargeFourthHitNorm_   .begin(); it1!=h1DXcellEvenChargeFourthHitNorm_   .end(); it1++) delete *it1; h1DXcellEvenChargeFourthHitNorm_  .clear();

  for(it1=h1DYcellCharge_                .begin(); it1!=h1DYcellCharge_                .end(); it1++) delete *it1; h1DYcellCharge_               .clear();
  for(it1=h1DYcellChargeNorm_            .begin(); it1!=h1DYcellChargeNorm_            .end(); it1++) delete *it1; h1DYcellChargeNorm_           .clear();

  for(it1=h1DYcellChargeSecondHit_       .begin(); it1!=h1DYcellChargeSecondHit_       .end(); it1++) delete *it1; h1DYcellChargeSecondHit_      .clear();
  for(it1=h1DYcellChargeSecondHitNorm_   .begin(); it1!=h1DYcellChargeSecondHitNorm_   .end(); it1++) delete *it1; h1DYcellChargeSecondHitNorm_  .clear();

  for(it1=h1DYcellChargeFourthHit_       .begin(); it1!=h1DYcellChargeFourthHit_       .end(); it1++) delete *it1; h1DYcellChargeFourthHit_      .clear();
  for(it1=h1DYcellChargeFourthHitNorm_   .begin(); it1!=h1DYcellChargeFourthHitNorm_   .end(); it1++) delete *it1; h1DYcellChargeFourthHitNorm_  .clear();


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
  
  for(it2=h16CellsCharge_                 .begin(); it2!=h16CellsCharge_                 .end(); it2++) delete *it2; h16CellsCharge_                .clear();
  for(it2=h16CellsChargeNorm_             .begin(); it2!=h16CellsChargeNorm_             .end(); it2++) delete *it2; h16CellsChargeNorm_            .clear();

  for(it2=h2DXcellCharge_                .begin(); it2!=h2DXcellCharge_                .end(); it2++) delete *it2; h2DXcellCharge_               .clear();
  for(it2=h2DXcellChargeSecondHit_       .begin(); it2!=h2DXcellChargeSecondHit_       .end(); it2++) delete *it2; h2DXcellChargeSecondHit_      .clear();
  for(it2=h2DXcellChargeFourthHit_       .begin(); it2!=h2DXcellChargeFourthHit_       .end(); it2++) delete *it2; h2DXcellChargeFourthHit_      .clear();
  
  for(it2=h2DXcellOddCharge_                .begin(); it2!=h2DXcellOddCharge_                .end(); it2++) delete *it2; h2DXcellOddCharge_               .clear();
  for(it2=h2DXcellOddChargeSecondHit_       .begin(); it2!=h2DXcellOddChargeSecondHit_       .end(); it2++) delete *it2; h2DXcellOddChargeSecondHit_      .clear();
  for(it2=h2DXcellOddChargeFourthHit_       .begin(); it2!=h2DXcellOddChargeFourthHit_       .end(); it2++) delete *it2; h2DXcellOddChargeFourthHit_      .clear();
  
  for(it2=h2DXcellEvenCharge_                .begin(); it2!=h2DXcellEvenCharge_                .end(); it2++) delete *it2; h2DXcellEvenCharge_               .clear();
  for(it2=h2DXcellEvenChargeSecondHit_       .begin(); it2!=h2DXcellEvenChargeSecondHit_       .end(); it2++) delete *it2; h2DXcellEvenChargeSecondHit_      .clear();
  for(it2=h2DXcellEvenChargeFourthHit_       .begin(); it2!=h2DXcellEvenChargeFourthHit_       .end(); it2++) delete *it2; h2DXcellEvenChargeFourthHit_      .clear();

  for(it2=h2DYcellCharge_                .begin(); it2!=h2DYcellCharge_                .end(); it2++) delete *it2; h2DYcellCharge_               .clear();
  for(it2=h2DYcellChargeSecondHit_       .begin(); it2!=h2DYcellChargeSecondHit_       .end(); it2++) delete *it2; h2DYcellChargeSecondHit_      .clear();
  for(it2=h2DYcellChargeFourthHit_       .begin(); it2!=h2DYcellChargeFourthHit_       .end(); it2++) delete *it2; h2DYcellChargeFourthHit_      .clear();

  
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
void ChargeAmericans::setErrorsBar(int planeID)
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

  /* GENERAL CELL */
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

  nBins = h2DXcellChargeFourthHit_[planeID]->GetNbinsX();
  TH1D* hXchargeFourthHitTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "XchargeFourthHit_Proj_bin_" << bX;
      hXchargeFourthHitTmp[bX-1] = h2DXcellChargeFourthHit_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeFourthHitTmp[bX-1]->GetEntries() != 0)
    {
      binError = hXchargeFourthHitTmp[bX-1]->GetRMS() / sqrt(hXchargeFourthHitTmp[bX-1]->GetEntries());
      h1DXcellChargeFourthHit_[planeID]->SetBinError(bX,binError);
    }
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeFourthHitTmp[p]->Delete("0");

  /* ODD CELL */
  nBins = h2DXcellOddCharge_[planeID]->GetNbinsX();
  TH1D* hXchargeOddTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "Xcharge_Proj_bin_" << bX;
      hXchargeOddTmp[bX-1] = h2DXcellOddCharge_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeOddTmp[bX-1]->GetEntries() != 0)
	{
	  binError = hXchargeOddTmp[bX-1]->GetRMS() / sqrt(hXchargeOddTmp[bX-1]->GetEntries());
	  h1DXcellOddCharge_[planeID]->SetBinError(bX,binError);
	}
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeOddTmp[p]->Delete("0");

  nBins = h2DXcellOddChargeSecondHit_[planeID]->GetNbinsX();
  TH1D* hXchargeSecondHitOddTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "XchargeSecondHit_Proj_bin_" << bX;
      hXchargeSecondHitOddTmp[bX-1] = h2DXcellOddChargeSecondHit_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeSecondHitOddTmp[bX-1]->GetEntries() != 0)
	{
	  binError = hXchargeSecondHitOddTmp[bX-1]->GetRMS() / sqrt(hXchargeSecondHitOddTmp[bX-1]->GetEntries());
	  h1DXcellOddChargeSecondHit_[planeID]->SetBinError(bX,binError);
	}
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeSecondHitOddTmp[p]->Delete("0");

  nBins = h2DXcellOddChargeFourthHit_[planeID]->GetNbinsX();
  TH1D* hXchargeFourthHitOddTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "XchargeFourthHit_Proj_bin_" << bX;
      hXchargeFourthHitOddTmp[bX-1] = h2DXcellOddChargeFourthHit_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeFourthHitOddTmp[bX-1]->GetEntries() != 0)
    {
      binError = hXchargeFourthHitOddTmp[bX-1]->GetRMS() / sqrt(hXchargeFourthHitOddTmp[bX-1]->GetEntries());
      h1DXcellOddChargeFourthHit_[planeID]->SetBinError(bX,binError);
    }
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeFourthHitOddTmp[p]->Delete("0");

  /* EVEN CELL */
  nBins = h2DXcellEvenCharge_[planeID]->GetNbinsX();
  TH1D* hXchargeEvenTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "Xcharge_Proj_bin_" << bX;
      hXchargeEvenTmp[bX-1] = h2DXcellEvenCharge_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeEvenTmp[bX-1]->GetEntries() != 0)
	{
	  binError = hXchargeEvenTmp[bX-1]->GetRMS() / sqrt(hXchargeEvenTmp[bX-1]->GetEntries());
	  h1DXcellEvenCharge_[planeID]->SetBinError(bX,binError);
	}
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeEvenTmp[p]->Delete("0");

  nBins = h2DXcellEvenChargeSecondHit_[planeID]->GetNbinsX();
  TH1D* hXchargeSecondHitEvenTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "XchargeSecondHit_Proj_bin_" << bX;
      hXchargeSecondHitEvenTmp[bX-1] = h2DXcellEvenChargeSecondHit_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeSecondHitEvenTmp[bX-1]->GetEntries() != 0)
	{
	  binError = hXchargeSecondHitEvenTmp[bX-1]->GetRMS() / sqrt(hXchargeSecondHitEvenTmp[bX-1]->GetEntries());
	  h1DXcellEvenChargeSecondHit_[planeID]->SetBinError(bX,binError);
	}
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeSecondHitEvenTmp[p]->Delete("0");

  nBins = h2DXcellEvenChargeFourthHit_[planeID]->GetNbinsX();
  TH1D* hXchargeFourthHitEvenTmp[nBins];
  for (int bX = 1; bX <= nBins; bX++)
    {
      hName.str(""); hName << "XchargeFourthHit_Proj_bin_" << bX;
      hXchargeFourthHitEvenTmp[bX-1] = h2DXcellEvenChargeFourthHit_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);

      if (hXchargeFourthHitEvenTmp[bX-1]->GetEntries() != 0)
    {
      binError = hXchargeFourthHitEvenTmp[bX-1]->GetRMS() / sqrt(hXchargeFourthHitEvenTmp[bX-1]->GetEntries());
      h1DXcellEvenChargeFourthHit_[planeID]->SetBinError(bX,binError);
    }
      else continue;
    }
  for (int p = 0; p < nBins; p++) hXchargeFourthHitEvenTmp[p]->Delete("0");

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

  nBins = h2DYcellChargeFourthHit_[planeID]->GetNbinsX();
  TH1D* hYchargeFourthHitTmp[nBins];
  for(int bY = 1; bY <= nBins; bY++)
    {
      hName.str(""); hName << "YchargeFourthHit_Proj_bin_" << bY;
      hYchargeFourthHitTmp[bY-1] = h2DYcellChargeFourthHit_[planeID]->ProjectionY(hName.str().c_str(),bY,bY);

      if (hYchargeFourthHitTmp[bY-1]->GetEntries() != 0)
    {
      binError = hYchargeFourthHitTmp[bY-1]->GetRMS() / sqrt(hYchargeFourthHitTmp[bY-1]->GetEntries());
      h1DYcellChargeFourthHit_[planeID]->SetBinError(bY,binError);
    }
      else continue;
    }
  for (int p = 0; p < nBins; p++) hYchargeFourthHitTmp[p]->Delete("0");
}

//=======================================================================
void ChargeAmericans::clusterSize(int planeID, const Data& data, int threadNumber)
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
  //>> wsi 22/12/17
  //if (!theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID))) return;
  //<< wsi 22/12/17

  THREADED(hClusterSize_[planeID])->Fill(data.getClusterSize(planeID));
}

//=======================================================================
void ChargeAmericans::cellLandau(bool pass, int planeID, const Data& data, int threadNumber)
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
void ChargeAmericans::clusterLandau(bool pass, int planeID, const Data& data, int threadNumber)
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
void ChargeAmericans::planeCharge(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;
  bool foundCluster = false;

  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < data.getClusterSize(planeID); h++)
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
  //if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)))
    //{
      THREADED(h2DChargeNorm_[planeID])->Fill(colPredicted,rowPredicted);
      if (data.getHasHit(planeID)) THREADED(h2DCharge_[planeID])->Fill(colPredicted, rowPredicted,data.getClusterCharge(planeID));
   // }
}

//=======================================================================
void ChargeAmericans::cellCharge(bool pass, int planeID, const Data& data, int threadNumber)
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


  // making 2x2 pixel matrix plot
  float xPixelSingleSideEdgeResidual = 0;
  float yPixelSingleSideEdgeResidual = 0;    

  //left bottom
  if ((colPredicted-9)%18==8 && rowPredicted%12==5)
  {
    xPixelSingleSideEdgeResidual = xPixelResidual - data.getXPitchLocal(planeID)/2;
    yPixelSingleSideEdgeResidual = yPixelResidual - data.getYPitchLocal(planeID)/2;
  }
  //left top
  else if ((colPredicted-9)%18==8 && rowPredicted%12==6)
  {
    xPixelSingleSideEdgeResidual = xPixelResidual - data.getXPitchLocal(planeID)/2;
    yPixelSingleSideEdgeResidual = yPixelResidual + data.getYPitchLocal(planeID)/2;
  }
  //right bottom
  else if ((colPredicted-9)%18==9 && rowPredicted%12==5)
  {
    xPixelSingleSideEdgeResidual = xPixelResidual + data.getXPitchLocal(planeID)/2;
    yPixelSingleSideEdgeResidual = yPixelResidual - data.getYPitchLocal(planeID)/2;
  }
  //right top
  else if ((colPredicted-9)%18==9 && rowPredicted%12==6)
  {
    xPixelSingleSideEdgeResidual = xPixelResidual + data.getXPitchLocal(planeID)/2;
    yPixelSingleSideEdgeResidual = yPixelResidual + data.getYPitchLocal(planeID)/2;
  }

  // making 4x4 pixel matrix plot
  float xPixelResidual4x4 = 0;
  float yPixelResidual4x4 = 0;

  // (0,0)
  if ((colPredicted-9)%18==7 && rowPredicted%12==4)
  {
    xPixelResidual4x4 = xPixelResidual - data.getXPitchLocal(planeID)/2*3;
    yPixelResidual4x4 = yPixelResidual - data.getYPitchLocal(planeID)/2*3;
  }
  // (0,1)
  else if ((colPredicted-9)%18==7 && rowPredicted%12==5)
  {
    xPixelResidual4x4 = xPixelResidual - data.getXPitchLocal(planeID)/2*3;
    yPixelResidual4x4 = yPixelResidual - data.getYPitchLocal(planeID)/2*1;
  }
  // (0,2)
  else if ((colPredicted-9)%18==7 && rowPredicted%12==6)
  {
    xPixelResidual4x4 = xPixelResidual - data.getXPitchLocal(planeID)/2*3;
    yPixelResidual4x4 = yPixelResidual + data.getYPitchLocal(planeID)/2*1;
  }
  // (0,3)
  else if ((colPredicted-9)%18==7 && rowPredicted%12==7)
  {
    xPixelResidual4x4 = xPixelResidual - data.getXPitchLocal(planeID)/2*3;
    yPixelResidual4x4 = yPixelResidual + data.getYPitchLocal(planeID)/2*3;
  }
  // (1,0)
  else if ((colPredicted-9)%18==8 && rowPredicted%12==4)
  {
    xPixelResidual4x4 = xPixelResidual - data.getXPitchLocal(planeID)/2*1;
    yPixelResidual4x4 = yPixelResidual - data.getYPitchLocal(planeID)/2*3;
  }
  // (1,1)
  else if ((colPredicted-9)%18==8 && rowPredicted%12==5)
  {
    xPixelResidual4x4 = xPixelResidual - data.getXPitchLocal(planeID)/2*1;
    yPixelResidual4x4 = yPixelResidual - data.getYPitchLocal(planeID)/2*1;
  }
  // (1,2)
  else if ((colPredicted-9)%18==8 && rowPredicted%12==6)
  {
    xPixelResidual4x4 = xPixelResidual - data.getXPitchLocal(planeID)/2*1;
    yPixelResidual4x4 = yPixelResidual + data.getYPitchLocal(planeID)/2*1;
  }
  // (1,3)
  else if ((colPredicted-9)%18==8 && rowPredicted%12==7)
  {
    xPixelResidual4x4 = xPixelResidual - data.getXPitchLocal(planeID)/2*1;
    yPixelResidual4x4 = yPixelResidual + data.getYPitchLocal(planeID)/2*3;
  }
  // (2,0)
  else if ((colPredicted-9)%18==9 && rowPredicted%12==4)
  {
    xPixelResidual4x4 = xPixelResidual + data.getXPitchLocal(planeID)/2*1;
    yPixelResidual4x4 = yPixelResidual - data.getYPitchLocal(planeID)/2*3;
  }
  // (2,1)
  else if ((colPredicted-9)%18==9 && rowPredicted%12==5)
  {
    xPixelResidual4x4 = xPixelResidual + data.getXPitchLocal(planeID)/2*1;
    yPixelResidual4x4 = yPixelResidual - data.getYPitchLocal(planeID)/2*1;
  }
  // (2,2)
  else if ((colPredicted-9)%18==9 && rowPredicted%12==6)
  {
    xPixelResidual4x4 = xPixelResidual + data.getXPitchLocal(planeID)/2*1;
    yPixelResidual4x4 = yPixelResidual + data.getYPitchLocal(planeID)/2*1;
  }
  // (2,3)
  else if ((colPredicted-9)%18==9 && rowPredicted%12==7)
  {
    xPixelResidual4x4 = xPixelResidual + data.getXPitchLocal(planeID)/2*1;
    yPixelResidual4x4 = yPixelResidual + data.getYPitchLocal(planeID)/2*3;
  }
  // (3,0)
  else if ((colPredicted-9)%18==10 && rowPredicted%12==4)
  {
    xPixelResidual4x4 = xPixelResidual + data.getXPitchLocal(planeID)/2*3;
    yPixelResidual4x4 = yPixelResidual - data.getYPitchLocal(planeID)/2*3;
  }
  // (3,1)
  else if ((colPredicted-9)%18==10 && rowPredicted%12==5)
  {
    xPixelResidual4x4 = xPixelResidual + data.getXPitchLocal(planeID)/2*3;
    yPixelResidual4x4 = yPixelResidual - data.getYPitchLocal(planeID)/2*1;
  }
  // (3,2)
  else if ((colPredicted-9)%18==10 && rowPredicted%12==6)
  {
    xPixelResidual4x4 = xPixelResidual + data.getXPitchLocal(planeID)/2*3;
    yPixelResidual4x4 = yPixelResidual + data.getYPitchLocal(planeID)/2*1;
  }
  // (3,3)
  else if ((colPredicted-9)%18==10 && rowPredicted%12==7)
  {
    xPixelResidual4x4 = xPixelResidual + data.getXPitchLocal(planeID)/2*3;
    yPixelResidual4x4 = yPixelResidual + data.getYPitchLocal(planeID)/2*3;
  }

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  // Filling 4x4 pixel matrix plot
  for (int h = 0; h < clusterSize; h++)
  {
    if (theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) &&
	    data.getClusterPixelRow   (h,planeID) == rowPredicted &&
	    data.getClusterPixelCol   (h,planeID) == colPredicted &&
      data.getIsPixelCalibrated (h,planeID)                 &&
	    data.getClusterPixelCharge(h,planeID) > standardCutsPixelMinimumCharge_ &&
	    data.getClusterPixelCharge(h,planeID) < standardCutsPixelMaximumCharge_)
	    {
        THREADED(h16CellsCharge_    [planeID])->Fill(xPixelResidual4x4, yPixelResidual4x4, data.getClusterPixelCharge(h,planeID));
        THREADED(h16CellsChargeNorm_[planeID])->Fill(xPixelResidual4x4, yPixelResidual4x4);
      }

  }

  //>> wsi 21/12/17
  //if (!theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID))) return;
  //<< wsi 21/12/17

  for (int h = 0; h < clusterSize; h++)
    {
      if (theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) &&
	  data.getClusterPixelRow   (h,planeID) == rowPredicted &&
	  data.getClusterPixelCol   (h,planeID) == colPredicted &&
      data.getIsPixelCalibrated (h,planeID)                 &&
	  data.getClusterPixelCharge(h,planeID) > standardCutsPixelMinimumCharge_ &&
	  data.getClusterPixelCharge(h,planeID) < standardCutsPixelMaximumCharge_)
	{

	  THREADED(h4CellsCharge_    [planeID])->Fill(xPixelSingleSideEdgeResidual,yPixelSingleSideEdgeResidual,data.getClusterPixelCharge(h,planeID));
	  THREADED(h4CellsChargeNorm_[planeID])->Fill(xPixelSingleSideEdgeResidual,yPixelSingleSideEdgeResidual);

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
    
    THREADED(h2DClusterSize_[planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterSize(planeID));
	  //THREADED(h2DClusterSize_[planeID])->Fill(xPixelSingleSideEdgeResidual,yPixelSingleSideEdgeResidual,data.getClusterSize(planeID));
	}
    }
}

//=======================================================================
void ChargeAmericans::xLandau(bool pass, int planeID, const Data &data, int threadNumber)
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
void ChargeAmericans::yLandau(bool pass, int planeID, const Data &data, int threadNumber)
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
void ChargeAmericans::xChargeDivision(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float xRes = 0;
  float yRes = 0;
  float xRes2Edge = 0;
  float maxPitchX;
  float maxPitchY;


  if (!pass || !data.getIsInDetector(planeID)) return;

  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY) return;

  if (data.getXPixelResidualLocal(planeID) > 0)       xRes2Edge = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
  else if (data.getXPixelResidualLocal(planeID) <= 0) xRes2Edge = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);

  xRes = data.getXPixelResidualLocal(planeID);
  yRes = data.getYPixelResidualLocal(planeID);

  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  bool myReturn = false;
  bool mySecondReturn = false;
  if (/*(theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) == true) &&*/ (data.getClusterSize(planeID) <= maxClusterSize))
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

      for (int h = 0; h < data.getClusterSize(planeID); h++)
      {
        if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run)     // Hits are in the window
            || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
            ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
            ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
        {
          mySecondReturn = true;
          break;
        }
      }



      if ((data.getHasHit(planeID) == true) && (myReturn == false))
      {
          THREADED(h1DXcellChargeNorm_         [planeID])->Fill(xRes2Edge);
          THREADED(h1DXcellChargeSecondHitNorm_[planeID])->Fill(xRes2Edge);
          if (colPredicted%2==0)
          {
            THREADED(h1DXcellEvenChargeNorm_         [planeID])->Fill(xRes);
            THREADED(h1DXcellEvenChargeSecondHitNorm_[planeID])->Fill(xRes);
          }
          else
          {
            THREADED(h1DXcellOddChargeNorm_         [planeID])->Fill(xRes);
            THREADED(h1DXcellOddChargeSecondHitNorm_[planeID])->Fill(xRes);
          }
        for (int h = 0; h < data.getClusterSize(planeID); h++)
        {
          if (data.getClusterPixelRow(h,planeID) == rowPredicted && data.getClusterPixelCol(h,planeID) == colPredicted)
          {
            THREADED(h2DXcellCharge_          [planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
            //THREADED(h2DXcellChargeSecondHit_ [planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellCharge_          [planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
            //THREADED(h1DXcellChargeSecondHit_ [planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
            if (colPredicted%2==0)
            {
              THREADED(h2DXcellEvenCharge_          [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              //THREADED(h2DXcellEvenChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              THREADED(h1DXcellEvenCharge_          [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              //THREADED(h1DXcellEvenChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
            }
            else
            {
              THREADED(h2DXcellOddCharge_          [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              //THREADED(h2DXcellOddChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              THREADED(h1DXcellOddCharge_          [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              //THREADED(h1DXcellOddChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
            }
            break;
          }		
        }

        if (data.getClusterSize(planeID) == 1) {
            THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
            THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
        }
        else if (data.getClusterSize(planeID) == 2) {
            // only add horizontal clusters
            if (data.getClusterPixelRow(0,planeID) == data.getClusterPixelRow(1,planeID)) {
                THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
                THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
            }
        }
        else { // cluster size 4 (or 3), using only up to 2 hits
            double chargeCumulative = 0;
            double chargeCumEven = 0, chargeCumOdd = 0;
            int count = 0;

            for (int h = 0; h < data.getClusterSize(planeID); h++) {
                if (count == 2) break;

                if ((xRes2Edge <= 0 && data.getClusterPixelCol(h,planeID) - colPredicted == 1 && data.getClusterPixelRow(h,planeID) == rowPredicted) ||
                        (xRes2Edge > 0 && data.getClusterPixelCol(h,planeID) - colPredicted == -1 && data.getClusterPixelRow(h,planeID) == rowPredicted) ||
                        (data.getClusterPixelRow(h, planeID) == rowPredicted && data.getClusterPixelCol(h, planeID) == colPredicted))
                {
                    chargeCumulative += data.getClusterPixelCharge(h,planeID);
                    count++;
                    if (colPredicted % 2 == 0)
                        chargeCumEven += data.getClusterPixelCharge(h,planeID);
                    else
                        chargeCumOdd += data.getClusterPixelCharge(h,planeID);
                }
            }

            THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,chargeCumulative);
            THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,chargeCumulative);
            if (chargeCumEven != 0)
                THREADED(h2DXcellEvenCharge_ [planeID])->Fill(xRes,chargeCumEven);
            if (chargeCumOdd != 0)
                THREADED(h2DXcellOddCharge_  [planeID])->Fill(xRes,chargeCumOdd);

        }

        /*for (int h = 0; h < data.getClusterSize(planeID); h++)
        {
          if (xRes2Edge <= 0 && data.getClusterPixelCol(h,planeID) - colPredicted == 1 && data.getClusterPixelRow(h,planeID) == rowPredicted)
          {
            THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
            if (colPredicted%2==0)
            {
              THREADED(h2DXcellEvenChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              THREADED(h1DXcellEvenChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
            }
            else
            {
              THREADED(h2DXcellOddChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              THREADED(h1DXcellOddChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
            }
            break;
          }
          else if (xRes2Edge > 0 && data.getClusterPixelCol(h,planeID) - colPredicted == -1 && data.getClusterPixelRow(h,planeID) == rowPredicted)
          {
            THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
            if (colPredicted%2==0)
            {
              THREADED(h2DXcellEvenChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              THREADED(h1DXcellEvenChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
            }
            else
            {
              THREADED(h2DXcellOddChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
              THREADED(h1DXcellOddChargeSecondHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
            }
            break;
          }
        }*/
      }

      if ((data.getHasHit(planeID) == true && mySecondReturn == false)) {

          if (data.getClusterSize(planeID) == 1) {
              THREADED(h2DXcellChargeFourthHit_ [planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
              THREADED(h1DXcellChargeFourthHit_ [planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
              THREADED(h1DXcellChargeFourthHitNorm_[planeID])->Fill(xRes2Edge);
              if (colPredicted%2==0)
              {
                THREADED(h2DXcellEvenChargeFourthHit_[planeID])->Fill(xRes, data.getClusterCharge(planeID));
                THREADED(h1DXcellEvenChargeFourthHitNorm_[planeID])->Fill(xRes);
                THREADED(h1DXcellEvenChargeFourthHit_[planeID])->Fill(xRes,data.getClusterCharge(planeID));
              }
              else
              {
                THREADED(h2DXcellOddChargeFourthHit_[planeID])->Fill(xRes, data.getClusterCharge(planeID));
                THREADED(h1DXcellOddChargeFourthHitNorm_[planeID])->Fill(xRes);
                THREADED(h1DXcellOddChargeFourthHit_[planeID])->Fill(xRes,data.getClusterCharge(planeID));
              }
          }
          else if (data.getClusterSize(planeID) == 2) {
              if (data.getClusterPixelRow(0, planeID) == data.getClusterPixelRow(1, planeID)) {
                THREADED(h2DXcellChargeFourthHit_ [planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
                THREADED(h1DXcellChargeFourthHit_ [planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
                THREADED(h1DXcellChargeFourthHitNorm_[planeID])->Fill(xRes2Edge);
                if (colPredicted%2==0)
                {
                  THREADED(h2DXcellEvenChargeFourthHit_[planeID])->Fill(xRes, data.getClusterCharge(planeID));
                  THREADED(h1DXcellEvenChargeFourthHitNorm_[planeID])->Fill(xRes);
                  THREADED(h1DXcellEvenChargeFourthHit_[planeID])->Fill(xRes,data.getClusterCharge(planeID));
                }
                else
                {
                  THREADED(h2DXcellOddChargeFourthHit_[planeID])->Fill(xRes, data.getClusterCharge(planeID));
                  THREADED(h1DXcellOddChargeFourthHitNorm_[planeID])->Fill(xRes);
                  THREADED(h1DXcellOddChargeFourthHit_[planeID])->Fill(xRes,data.getClusterCharge(planeID));
                }
              }
          }
          else if (data.getClusterSize(planeID) == 3 || data.getClusterSize(planeID) == 4) {
              double chargeTop = 0, chargeSecond = 0;
              int pixelTop, pixelSecond;
              for (int h = 0; h < data.getClusterSize(planeID); h++) {
                  if (data.getClusterPixelCharge(h, planeID) > chargeTop) {
                      pixelSecond = pixelTop;
                      chargeSecond = chargeTop;
                      pixelTop = h;
                      chargeTop = data.getClusterPixelCharge(h, planeID);
                  }
                  else if (data.getClusterPixelCharge(h, planeID) > chargeSecond) {
                      pixelSecond = h;
                      chargeSecond = data.getClusterPixelCharge(h, planeID);
                  }
              }
              if (data.getClusterPixelRow(pixelTop, planeID) == data.getClusterPixelRow(pixelSecond, planeID)) {
                  THREADED(h2DXcellChargeFourthHit_ [planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
                  THREADED(h1DXcellChargeFourthHit_ [planeID])->Fill(xRes2Edge,data.getClusterCharge(planeID));
                  THREADED(h1DXcellChargeFourthHitNorm_[planeID])->Fill(xRes2Edge);
                  if (colPredicted%2==0)
                  {
                    THREADED(h2DXcellEvenChargeFourthHit_[planeID])->Fill(xRes, data.getClusterCharge(planeID));
                    THREADED(h1DXcellEvenChargeFourthHitNorm_[planeID])->Fill(xRes);
                    THREADED(h1DXcellEvenChargeFourthHit_[planeID])->Fill(xRes,data.getClusterCharge(planeID));
                  }
                  else
                  {
                    THREADED(h2DXcellOddChargeFourthHit_[planeID])->Fill(xRes, data.getClusterCharge(planeID));
                    THREADED(h1DXcellOddChargeFourthHitNorm_[planeID])->Fill(xRes);
                    THREADED(h1DXcellOddChargeFourthHit_[planeID])->Fill(xRes,data.getClusterCharge(planeID));
                  }
              }
          }
      }
    
//      if ((data.getHasHit(planeID) == true) && (mySecondReturn == false))
//      {
//           THREADED(h1DXcellChargeFourthHitNorm_[planeID])->Fill(xRes2Edge);
//           if (colPredicted%2==0)
//           {
//             THREADED(h1DXcellEvenChargeFourthHitNorm_[planeID])->Fill(xRes);
//           }
//           else
//           {
//             THREADED(h1DXcellOddChargeFourthHitNorm_[planeID])->Fill(xRes);
//           }
//        for (int h = 0; h < data.getClusterSize(planeID); h++)
//        {
//          if (data.getClusterPixelRow(h,planeID) == rowPredicted && data.getClusterPixelCol(h,planeID) == colPredicted)
//          {
//            THREADED(h2DXcellChargeFourthHit_ [planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            THREADED(h1DXcellChargeFourthHit_ [planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            if (colPredicted%2==0)
//            {
//              THREADED(h2DXcellEvenChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellEvenChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            else
//            {
//              THREADED(h2DXcellOddChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellOddChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            break;
//          }
//        }

//        for (int h = 0; h < data.getClusterSize(planeID); h++)
//        {
//          if (xRes2Edge <= 0 && data.getClusterPixelCol(h,planeID) - colPredicted == 1 && data.getClusterPixelRow(h,planeID) == rowPredicted)
//          {
//            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            if (colPredicted%2==0)
//            {
//              THREADED(h2DXcellEvenChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellEvenChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            else
//            {
//              THREADED(h2DXcellOddChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellOddChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            break;
//          }
//          else if (xRes2Edge > 0 && data.getClusterPixelCol(h,planeID) - colPredicted == -1 && data.getClusterPixelRow(h,planeID) == rowPredicted)
//          {
//            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            if (colPredicted%2==0)
//            {
//              THREADED(h2DXcellEvenChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellEvenChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            else
//            {
//              THREADED(h2DXcellOddChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellOddChargeFourthHit_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            break;
//          }
//        }

//        for (int h = 0; h < data.getClusterSize(planeID); h++)
//        {
//          if (yRes<=0 && data.getClusterPixelRow(h,planeID)-rowPredicted==-1 && data.getClusterPixelCol(h,planeID)==colPredicted)
//          {
//            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            if (colPredicted%2==0)
//            {
//              THREADED(h2DXcellEvenChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellEvenChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            else
//            {
//              THREADED(h2DXcellOddChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellOddChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            break;
//          }
//          else if (yRes>0 && data.getClusterPixelRow(h,planeID)-rowPredicted==1 && data.getClusterPixelCol(h,planeID)==colPredicted)
//          {
//            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2Edge,data.getClusterPixelCharge(h,planeID));
//            if (colPredicted%2==0)
//            {
//              THREADED(h2DXcellEvenChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellEvenChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            else
//            {
//              THREADED(h2DXcellOddChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//              THREADED(h1DXcellOddChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            }
//            break;
//          }
//          /*if (abs(data.getClusterPixelRow(h,planeID)-rowPredicted)==1 &&
//              abs(data.getClusterPixelCol(h,planeID)-colPredicted)<=1 )
//          {
//            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
//            break;
//          }*/
//        }
//      } // mySecondReturn

  }// checkWindow
}


//=======================================================================
void ChargeAmericans::xChargeDivision_v1 (bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  float xRes = 0;
  float xRes2cell = 0;
  float yRes = 0;
  float maxPitchX;
  float maxPitchY;


  if (!pass || !data.getIsInDetector(planeID)) return;

  maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  if (data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY) return;

  if (data.getXPixelResidualLocal(planeID) > 0)       xRes = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
  else if (data.getXPixelResidualLocal(planeID) <= 0) xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);

  yRes = data.getYPixelResidualLocal(planeID);

  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  bool myReturn = false;
  bool mySecondReturn = false;
  if ((theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) == true) && (data.getClusterSize(planeID) <= maxClusterSize))
    {
      if ((colPredicted+13)%24==0)
      {
        if (xRes>0) xRes2cell = xRes - data.getXPitchLocal(planeID);
        else xRes2cell = xRes;
      }
      if ((colPredicted+13)%24==1)
      {
        if (xRes<=0) xRes2cell = xRes + data.getXPitchLocal(planeID);
        else xRes2cell = xRes;
      }

      for (int h = 0; h < data.getClusterSize(planeID); h++)
      {
        if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
            || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
            //||  data.getClusterPixelRow    (h,planeID) != rowPredicted                                         // Hits are on the same row (sharing is along the row - x direction)
            ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
            ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
        {
          myReturn = true;
          break;
        }
      }

      for (int h = 0; h < data.getClusterSize(planeID); h++)
      {
        if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run)     // Hits are in the window
            || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
            ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
            ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
        {
          mySecondReturn = true;
          break;
        }
      }

      THREADED(h1DXcellChargeNorm_         [planeID])->Fill(xRes2cell);
      THREADED(h1DXcellChargeSecondHitNorm_[planeID])->Fill(xRes2cell);
      THREADED(h1DXcellChargeFourthHitNorm_[planeID])->Fill(xRes2cell);

      if ((data.getHasHit(planeID) == true) && (myReturn == false))
      {
        for (int h = 0; h < data.getClusterSize(planeID); h++)
        {
          if (data.getClusterPixelRow(h,planeID) == rowPredicted && data.getClusterPixelCol(h,planeID) == colPredicted)
          {
            THREADED(h2DXcellCharge_          [planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h2DXcellChargeSecondHit_ [planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h2DXcellChargeFourthHit_ [planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellCharge_          [planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeSecondHit_ [planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeFourthHit_ [planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            break;
          }		
        }

        for (int h = 0; h < data.getClusterSize(planeID); h++)
        {
          if (xRes <= 0 && data.getClusterPixelCol(h,planeID) - colPredicted == 1 && data.getClusterPixelRow(h,planeID) == rowPredicted)
          {
            THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));

            break;
          }
          else if (xRes > 0 && data.getClusterPixelCol(h,planeID) - colPredicted == -1 && data.getClusterPixelRow(h,planeID) == rowPredicted)
          {
            THREADED(h2DXcellChargeSecondHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            break;
          }
        }
      }
      
      if ((data.getHasHit(planeID) == true) && (mySecondReturn == false))
      {

        for (int h = 0; h < data.getClusterSize(planeID); h++)
        {
          /*if (yRes<=0 && data.getClusterPixelRow(h,planeID)-rowPredicted==-1 && data.getClusterPixelCol(h,planeID)==colPredicted) 
          {
            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            break;
          }
          else if (yRes>0 && data.getClusterPixelRow(h,planeID)-rowPredicted==1 && data.getClusterPixelCol(h,planeID)==colPredicted)
          {
            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            break;
          }*/
          if (abs(data.getClusterPixelRow(h,planeID)-rowPredicted)==1 &&
              abs(data.getClusterPixelCol(h,planeID)-colPredicted)<=1 )
          {
            THREADED(h2DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            THREADED(h1DXcellChargeFourthHit_[planeID])->Fill(xRes2cell,data.getClusterPixelCharge(h,planeID));
            break;
          }
        }
      }// mySecondReturn

  }// checkWindow

}

//=======================================================================
void ChargeAmericans::xAsymmetry(bool pass, int planeID, const Data& data, int threadNumber)
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
  void ChargeAmericans::yChargeDivision(bool pass, int planeID, const Data& data, int threadNumber)
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
    
  if (data.getYPixelResidualLocal(planeID) > 0)       yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
  else if (data.getYPixelResidualLocal(planeID) <= 0) yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);

  xRes = data.getXPixelResidualLocal(planeID);

  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  bool myReturn = false;
  bool mySecondReturn = false;
  if (/*(theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) == true) &&*/ (data.getClusterSize(planeID) <= maxClusterSize))
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

    for (int h = 0; h < data.getClusterSize(planeID); h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run)     // Hits are in the window
          || !data.getIsPixelCalibrated  (h,planeID)                                                         // Pixels are calibrated
          ||  data.getClusterPixelCharge (h,planeID) < standardCutsPixelMinimumCharge_                       // Charge is over threshold
          ||  data.getClusterPixelCharge (h,planeID) > standardCutsPixelMaximumCharge_)                      // Maximum allowed charge for this physics
      {
        mySecondReturn = true;
        break;
      }
    }


    if ((data.getHasHit(planeID) == true) && (myReturn == false))
    {
        THREADED(h1DYcellChargeNorm_         [planeID])->Fill(yRes);
        THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(yRes);
      for (int h = 0; h < data.getClusterSize(planeID); h++)
      {
        if (data.getClusterPixelRow(h,planeID) == rowPredicted && data.getClusterPixelCol(h,planeID) == colPredicted)
        {

          THREADED(h2DYcellCharge_          [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          //THREADED(h2DYcellChargeSecondHit_ [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          THREADED(h1DYcellCharge_          [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          //THREADED(h1DYcellChargeSecondHit_ [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          break;
        }
      }

      if (data.getClusterSize(planeID) == 1) {
          THREADED(h2DYcellChargeSecondHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
          THREADED(h1DYcellChargeSecondHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
      }
      else if (data.getClusterSize(planeID) == 2) {
          // only add vertical clusters
          if (data.getClusterPixelCol(0, planeID) == data.getClusterPixelCol(1, planeID)) {
              THREADED(h2DYcellChargeSecondHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
              THREADED(h1DYcellChargeSecondHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
          }
      }
      else { // cluster size 4 (or 3), using only up to 2 hits

          double chargeCumulative = 0;
          int count = 0;

          for (int h = 0; h < data.getClusterSize(planeID); h++)
          {
              if (count == 2) break;

              if (yRes <= 0 && data.getClusterPixelRow(h,planeID) - rowPredicted == 1 && data.getClusterPixelCol(h,planeID) == colPredicted)
              {
                  count++;
                  chargeCumulative += data.getClusterPixelCharge(h, planeID);
                  //THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(yRes);
                  //THREADED(h2DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
                  //THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
                  //break;
              }
              else if (yRes > 0 && data.getClusterPixelRow(h,planeID) - rowPredicted == -1 && data.getClusterPixelCol(h,planeID) == colPredicted)
              {
                  count++;
                  chargeCumulative += data.getClusterPixelCharge(h, planeID);
                  //THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(yRes);
                  //THREADED(h2DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
                  //THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
                  //break;
              }
              else if (data.getClusterPixelRow(h, planeID) == rowPredicted && data.getClusterPixelCol(h, planeID) == colPredicted) {
                  count++;
                  chargeCumulative += data.getClusterPixelCharge(h, planeID);
              }
          }

          THREADED(h2DYcellChargeSecondHit_[planeID])->Fill(yRes,chargeCumulative);
          THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,chargeCumulative);

      }

    }

    // using up to 4 hits
    if ((data.getHasHit(planeID) == true) && (mySecondReturn == false))
    {
        // add all size 1 clusters
        if (data.getClusterSize(planeID) == 1) {
           THREADED(h2DYcellChargeFourthHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
           THREADED(h1DYcellChargeFourthHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
           THREADED(h1DYcellChargeFourthHitNorm_[planeID])->Fill(yRes);
        }
        // only add vertical size 2 clusters
        else if (data.getClusterSize(planeID) == 2) {
            if (data.getClusterPixelCol(0, planeID) == data.getClusterPixelCol(1, planeID)) {
              THREADED(h2DYcellChargeFourthHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
              THREADED(h1DYcellChargeFourthHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
              THREADED(h1DYcellChargeFourthHitNorm_[planeID])->Fill(yRes);
            }
        }
        // for sizes 3 and above, make sure pixels with 2 largest charges are vertical
        else if (data.getClusterSize(planeID) == 3 || data.getClusterSize(planeID) == 4) {
            double chargeTop = 0, chargeSecond = 0;
            int pixelTop, pixelSecond;
            for (int h = 0; h < data.getClusterSize(planeID); h++) {
                if (data.getClusterPixelCharge(h, planeID) > chargeTop) {
                    pixelSecond = pixelTop;
                    chargeSecond = chargeTop;
                    pixelTop = h;
                    chargeTop = data.getClusterPixelCharge(h, planeID);
                }
                else if (data.getClusterPixelCharge(h, planeID) > chargeSecond) {
                    pixelSecond = h;
                    chargeSecond = data.getClusterPixelCharge(h, planeID);
                }
            }
            if (data.getClusterPixelCol(pixelTop, planeID) == data.getClusterPixelCol(pixelSecond, planeID)) {
                THREADED(h2DYcellChargeFourthHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
                THREADED(h1DYcellChargeFourthHit_ [planeID])->Fill(yRes,data.getClusterCharge(planeID));
                THREADED(h1DYcellChargeFourthHitNorm_[planeID])->Fill(yRes);
            }
        }
            /*
      for (int h = 0; h < data.getClusterSize(planeID); h++)
      {
        if (data.getClusterPixelRow(h,planeID) == rowPredicted && data.getClusterPixelCol(h,planeID) == colPredicted)
        {
          THREADED(h2DYcellChargeFourthHit_ [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          THREADED(h1DYcellChargeFourthHit_ [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          break;
        }
      }

      for (int h = 0; h < data.getClusterSize(planeID); h++)
      {
        if (yRes <= 0 && data.getClusterPixelRow(h,planeID) - rowPredicted == 1 && data.getClusterPixelCol(h,planeID) == colPredicted)
        {
          THREADED(h2DYcellChargeFourthHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          THREADED(h1DYcellChargeFourthHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          break;
        }
        else if (yRes > 0 && data.getClusterPixelRow(h,planeID) - rowPredicted == -1 && data.getClusterPixelCol(h,planeID) == colPredicted)
        {
          THREADED(h2DYcellChargeFourthHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          THREADED(h1DYcellChargeFourthHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          break;
        }
      }

      for (int h = 0; h < data.getClusterSize(planeID); h++)
      {
          // AF only add fourth hit if cl > 2, otherwise also adding x-only clusters
        if (data.getClusterSize(planeID) > 2 && xRes>0 && data.getClusterPixelCol(h,planeID)-colPredicted==1 && data.getClusterPixelRow(h,planeID)==rowPredicted)
        {
          THREADED(h2DYcellChargeFourthHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          THREADED(h1DYcellChargeFourthHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          break; 
        }
        else if (data.getClusterSize(planeID) > 2 && xRes<=0 && data.getClusterPixelCol(h,planeID)-colPredicted==-1 && data.getClusterPixelRow(h,planeID)==rowPredicted)
        {
          THREADED(h2DYcellChargeFourthHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          THREADED(h1DYcellChargeFourthHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
          break;  
        }
      } */
    }//mySecondReturn

  }//checkWindow
}

//=======================================================================
void ChargeAmericans::yAsymmetry(bool pass, int planeID, const Data& data, int threadNumber)
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
void ChargeAmericans::setCutsFormula(std::map<std::string,std::string> cutsList,std::vector<TTree*> tree)
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
bool ChargeAmericans::passCalibrationsCut(int planeID, const Data &data)
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
bool ChargeAmericans::passStandardCuts(int planeID, const Data &data)
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
void ChargeAmericans::beginJob(void)
{
    std::cout << __PRETTY_FUNCTION__ << " IAM DOING THE AMNERICAN STUFF" << std::endl;

  standardCutsPixelMinimumCharge_   = theXmlParser_->getAnalysesFromString("Charge")->getPixelMinimumCharge();
  standardCutsPixelMaximumCharge_   = theXmlParser_->getAnalysesFromString("Charge")->getPixelMaximumCharge();
  standardCutsClusterMinimumCharge_ = theXmlParser_->getAnalysesFromString("Charge")->getClusterMinimumCharge();
  standardCutsClusterMaximumCharge_ = theXmlParser_->getAnalysesFromString("Charge")->getClusterMaximumCharge();
  
  theWindowsManager_      = theAnalysisManager_->getWindowsManager();
  theCalibrationsManager_ = theAnalysisManager_->getCalibrationsManager();

  book();

}

//=======================================================================
void ChargeAmericans::analyze(const Data& data, int threadNumber)
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

      bool kSmallPixel = true;
      // make sure all pixel hits are from small pixels in 25x100
      if (thePlaneMapping_->getPlaneName(p).find("Dut0") != std::string::npos && data.getClusterSize(p) <= 4) {

          for (int h = 0; h < data.getClusterSize(p); h++) {
              unsigned int temp_col = data.getClusterPixelCol(h, p);
              unsigned int temp_row = data.getClusterPixelRow(h, p);
              // need to convert from sensor (312 cols) to roc (52 cols) coords
              try {
                  thePitchTranslation25x100_->fromSensorToRocCoords(&temp_col, &temp_row);
                      if (!thePitchTranslation25x100_->isSmallPixel(temp_col, temp_row))
                           kSmallPixel = false;
              }
              catch (const std::out_of_range & oor) {
                  std::cerr << "[ChargeAmericans] Out of Range error: " << oor.what() << '\n';
                  kSmallPixel = false;
              }
          }
      }
      if (!kSmallPixel) continue;

      if ((thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos) && (!passCalibrationsCut(p,data)))
	{
	  std::cout << __PRETTY_FUNCTION__ << "Calibration check not passed" << std::endl;
	  return;
	}


      //if (thePlaneMapping_->getPlaneName(p)=="Dut1")
      //{
      //  int colPredicted = data.getColPredicted(p);
      //  if (!((colPredicted+13)%24==0 || (colPredicted+13)%24==1)) return;

      //  clusterLandau   (clusterLandauCut,p,data,threadNumber);
      //  cellLandau      (cellLandauCut,   p,data,threadNumber);
      //  cellCharge      (cellChargeCut,   p,data,threadNumber);
      //  planeCharge     (true,            p,data,threadNumber);

      //  xLandau         (cellChargeXCut,  p,data,threadNumber);
      //  xChargeDivision (cellChargeXCut,  p,data,threadNumber);
      //  xAsymmetry      (cellChargeXCut,  p,data,threadNumber);

      //  yLandau         (cellChargeYCut,  p,data,threadNumber);
      //  yChargeDivision (cellChargeYCut,  p,data,threadNumber);
      //  yAsymmetry      (cellChargeYCut,  p,data,threadNumber);
      //}
      //else
      //{
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
      //}
    }
}

//=======================================================================
void ChargeAmericans::endJob(void)
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
      ADD_THREADED(h1DXcellOddCharge_                          [p]);
      ADD_THREADED(h1DXcellEvenCharge_                          [p]);
      ADD_THREADED(h1DXcellChargeNorm_                      [p]);
      ADD_THREADED(h1DXcellOddChargeNorm_                      [p]);
      ADD_THREADED(h1DXcellEvenChargeNorm_                      [p]);

      ADD_THREADED(h1DXcellChargeSecondHit_                 [p]);
      ADD_THREADED(h1DXcellChargeSecondHitNorm_             [p]);
      ADD_THREADED(h1DXcellOddChargeSecondHit_                 [p]);
      ADD_THREADED(h1DXcellOddChargeSecondHitNorm_             [p]);
      ADD_THREADED(h1DXcellEvenChargeSecondHit_                 [p]);
      ADD_THREADED(h1DXcellEvenChargeSecondHitNorm_             [p]);

      ADD_THREADED(h1DXcellChargeFourthHit_                 [p]);
      ADD_THREADED(h1DXcellChargeFourthHitNorm_             [p]);
      ADD_THREADED(h1DXcellOddChargeFourthHit_                 [p]);
      ADD_THREADED(h1DXcellOddChargeFourthHitNorm_             [p]);
      ADD_THREADED(h1DXcellEvenChargeFourthHit_                 [p]);
      ADD_THREADED(h1DXcellEvenChargeFourthHitNorm_             [p]);

      ADD_THREADED(h1DYcellCharge_                          [p]);
      ADD_THREADED(h1DYcellChargeNorm_                      [p]);

      ADD_THREADED(h1DYcellChargeSecondHit_                 [p]);
      ADD_THREADED(h1DYcellChargeSecondHitNorm_             [p]);

			ADD_THREADED(h1DYcellChargeFourthHit_                 [p]);
      ADD_THREADED(h1DYcellChargeFourthHitNorm_             [p]);


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

      ADD_THREADED(h16CellsCharge_                           [p]);
      ADD_THREADED(h16CellsChargeNorm_                       [p]);

      ADD_THREADED(h2DXcellCharge_                          [p]);
      ADD_THREADED(h2DXcellChargeSecondHit_                 [p]);
      ADD_THREADED(h2DXcellChargeFourthHit_                 [p]);
      
      ADD_THREADED(h2DXcellOddCharge_                          [p]);
      ADD_THREADED(h2DXcellOddChargeSecondHit_                 [p]);
      ADD_THREADED(h2DXcellOddChargeFourthHit_                 [p]);
      
      ADD_THREADED(h2DXcellEvenCharge_                          [p]);
      ADD_THREADED(h2DXcellEvenChargeSecondHit_                 [p]);
      ADD_THREADED(h2DXcellEvenChargeFourthHit_                 [p]);

      ADD_THREADED(h2DYcellCharge_                          [p]);
      ADD_THREADED(h2DYcellChargeSecondHit_                 [p]);
      ADD_THREADED(h2DYcellChargeFourthHit_                 [p]);

      ADD_THREADED(h2DXcellChargeAsymmetry_                 [p]);
      ADD_THREADED(h2DXcellChargeAsymmetryInv_              [p]);

      ADD_THREADED(h2DYcellChargeAsymmetry_                 [p]);
      ADD_THREADED(h2DYcellChargeAsymmetryInv_              [p]);

      STDLINE("Threading phase completed",ACGreen);


      STDLINE("Filling phase...",ACWhite);

      h1DXcellCharge_               [p]->Divide(h1DXcellChargeNorm_          [p]);
      h1DXcellChargeSecondHit_      [p]->Divide(h1DXcellChargeSecondHitNorm_ [p]);
      //>> wsi 12/12/17
      h1DXcellChargeFourthHit_      [p]->Divide(h1DXcellChargeFourthHitNorm_ [p]);
      //<< wsi 12/12/17
      
      h1DXcellOddCharge_               [p]->Divide(h1DXcellOddChargeNorm_          [p]);
      h1DXcellOddChargeSecondHit_      [p]->Divide(h1DXcellOddChargeSecondHitNorm_ [p]);
      h1DXcellOddChargeFourthHit_      [p]->Divide(h1DXcellOddChargeFourthHitNorm_ [p]);
      
      h1DXcellEvenCharge_               [p]->Divide(h1DXcellEvenChargeNorm_          [p]);
      h1DXcellEvenChargeSecondHit_      [p]->Divide(h1DXcellEvenChargeSecondHitNorm_ [p]);
      h1DXcellEvenChargeFourthHit_      [p]->Divide(h1DXcellEvenChargeFourthHitNorm_ [p]);

      h1DYcellCharge_               [p]->Divide(h1DYcellChargeNorm_          [p]);
      h1DYcellChargeSecondHit_      [p]->Divide(h1DYcellChargeSecondHitNorm_ [p]);
      //>> wsi 12/12/17
      h1DYcellChargeFourthHit_      [p]->Divide(h1DYcellChargeFourthHitNorm_ [p]);
      //<< wsi 12/12/17

      h2DCellCharge_                [p]->Divide(h2DCellChargeNorm_           [p]);
      h2DCharge_                    [p]->Divide(h2DChargeNorm_               [p]);
      h2DCellChargeOdd_             [p]->Divide(h2DCellChargeOddNorm_        [p]);
      h2DCellChargeEven_            [p]->Divide(h2DCellChargeEvenNorm_       [p]);
      h2DClusterSize_               [p]->Divide(h2DCellChargeNorm_           [p]);
      h4CellsCharge_                [p]->Divide(h4CellsChargeNorm_           [p]);
      h16CellsCharge_               [p]->Divide(h16CellsChargeNorm_           [p]);

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

      //>> wsi 12/12/17
      h1DXcellChargeFourthHit_   [p]->SetMarkerStyle(20);
      h1DXcellChargeFourthHit_   [p]->SetMarkerSize(0.6);

      h1DYcellChargeFourthHit_   [p]->SetMarkerStyle(20);
      h1DYcellChargeFourthHit_   [p]->SetMarkerSize(0.6);
      //<< wsi 12/12/17
      
      h1DXcellOddCharge_            [p]->SetMarkerStyle(20);
      h1DXcellOddCharge_            [p]->SetMarkerSize(0.6);

      h1DXcellOddChargeSecondHit_   [p]->SetMarkerStyle(20);
      h1DXcellOddChargeSecondHit_   [p]->SetMarkerSize(0.6);

      h1DXcellOddChargeFourthHit_   [p]->SetMarkerStyle(20);
      h1DXcellOddChargeFourthHit_   [p]->SetMarkerSize(0.6);

      h1DXcellEvenCharge_            [p]->SetMarkerStyle(20);
      h1DXcellEvenCharge_            [p]->SetMarkerSize(0.6);

      h1DXcellEvenChargeSecondHit_   [p]->SetMarkerStyle(20);
      h1DXcellEvenChargeSecondHit_   [p]->SetMarkerSize(0.6);

      h1DXcellEvenChargeFourthHit_   [p]->SetMarkerStyle(20);
      h1DXcellEvenChargeFourthHit_   [p]->SetMarkerSize(0.6);


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

      h16CellsCharge_             [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h16CellsCharge_             [p]->GetYaxis()->SetTitle("short pitch (um)"  );

      h16CellsChargeNorm_         [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h16CellsChargeNorm_         [p]->GetYaxis()->SetTitle("short pitch (um)"  );


      h1DXcellCharge_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellChargeNorm_        [p]->GetXaxis()->SetTitle("long pitch (um)"   );

      h1DXcellChargeSecondHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellChargeSecondHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");

      //>> wsi 12/12/17
      h1DXcellChargeFourthHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellChargeFourthHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellChargeFourthHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");
      //<< wsi 12/12/17
      
      h1DXcellOddCharge_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellOddCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellOddChargeNorm_        [p]->GetXaxis()->SetTitle("long pitch (um)"   );

      h1DXcellOddChargeSecondHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellOddChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellOddChargeSecondHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");

      h1DXcellOddChargeFourthHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellOddChargeFourthHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellOddChargeFourthHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");
      
      h1DXcellEvenCharge_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellEvenCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellEvenChargeNorm_        [p]->GetXaxis()->SetTitle("long pitch (um)"   );

      h1DXcellEvenChargeSecondHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellEvenChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellEvenChargeSecondHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");

      h1DXcellEvenChargeFourthHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DXcellEvenChargeFourthHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DXcellEvenChargeFourthHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellCharge_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellChargeSecondHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellChargeFourthHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellChargeFourthHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      
      h2DXcellOddCharge_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellOddCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellOddChargeSecondHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellOddChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellOddChargeFourthHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellOddChargeFourthHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      
      h2DXcellEvenCharge_            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellEvenCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellEvenChargeSecondHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellEvenChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DXcellEvenChargeFourthHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h2DXcellEvenChargeFourthHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");

      h1DYcellCharge_            [p]->GetXaxis()->SetTitle("short pitch (um)"  );
      h1DYcellCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DYcellChargeNorm_        [p]->GetXaxis()->SetTitle("short pitch (um)"  );

      h1DYcellChargeSecondHit_   [p]->GetXaxis()->SetTitle("short pitch (um)"   );
      h1DYcellChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DYcellChargeSecondHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");

      //>> wsi 12/12/17
      h1DYcellChargeFourthHit_   [p]->GetXaxis()->SetTitle("long pitch (um)"   );
      h1DYcellChargeFourthHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");
      h1DYcellChargeFourthHitNorm_[p]->GetYaxis()->SetTitle("charge (electrons)");
      //<< wsi 12/12/17

      h2DYcellCharge_            [p]->GetXaxis()->SetTitle("short pitch (um)"  );
      h2DYcellCharge_            [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DYcellChargeSecondHit_   [p]->GetXaxis()->SetTitle("short pitch (um)"  );
      h2DYcellChargeSecondHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");

      h2DYcellChargeFourthHit_   [p]->GetXaxis()->SetTitle("short pitch (um)"  );
      h2DYcellChargeFourthHit_   [p]->GetYaxis()->SetTitle("charge (electrons)");

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
void ChargeAmericans::book(void)
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
  //float binSize = 2.5; // [um]

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
      h2DCellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)xPitch/binSize, -(xPitch/2), xPitch/2, 2*(int)yPitch/binSize, -(yPitch/2), yPitch/2)));

      hName  = "h2DCellChargeNorm_"              + planeName;
      hTitle = "Cell charge normalization "      + planeName;
      h2DCellChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)xPitch/binSize, -(xPitch/2), xPitch/2, 2*(int)yPitch/binSize, -(yPitch/2), yPitch/2)));

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
      h4CellsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)xPitch/binSize, -(xPitch), xPitch, 2*(int)yPitch/binSize, -(yPitch), yPitch)));
      //h4CellsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));
      
      hName  = "h4CellsChargeNorm_"              + planeName;
      hTitle = "4 cells charge normalization "   + planeName;
      h4CellsChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)xPitch/binSize, -(xPitch), xPitch, 2*(int)yPitch/binSize, -(yPitch), yPitch)));
      //h4CellsChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/binSize, -(xPitch/2), xPitch/2, (int)yPitch/binSize, -(yPitch/2), yPitch/2)));
      
      hName  = "h16CellsCharge_"                  + planeName;
      hTitle = "4x4 cells charge 2D distribution " + planeName;
      h16CellsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 4*(int)xPitch/binSize, -(xPitch)*2, xPitch*2, 4*(int)yPitch/binSize, -(yPitch)*2, yPitch*2)));

      hName  = "h16CellsChargeNorm_"             + planeName;
      hTitle = "4x4 cells charge normalization " + planeName;
      h16CellsChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 4*(int)xPitch/binSize, -(xPitch)*2, xPitch*2, 4*(int)yPitch/binSize, -(yPitch)*2, yPitch*2)));

      hName  = "h2DClusterSize_"                 + planeName;
      hTitle = "Cluster size distribution  "     + planeName;
      //h2DClusterSize_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)xPitch/binSize, -(xPitch), xPitch, 2*(int)yPitch/binSize, -(yPitch), yPitch)));
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

      hName  = "h2DXcellChargeFourthHit_"                             + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "              + planeName;
      h2DXcellChargeFourthHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));
      
      // Odd
      hName  = "h2DXcellOddCharge_"                                      + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "                 + planeName;
      h2DXcellOddCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));

      hName  = "h2DXcellOddChargeSecondHit_"                             + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "                 + planeName;
      h2DXcellOddChargeSecondHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));

      hName  = "h2DXcellOddChargeFourthHit_"                             + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "                 + planeName;
      h2DXcellOddChargeFourthHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));
      
      // Even
      hName  = "h2DXcellEvenCharge_"                                      + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "                  + planeName;
      h2DXcellEvenCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));

      hName  = "h2DXcellEvenChargeSecondHit_"                             + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "                  + planeName;
      h2DXcellEvenChargeSecondHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));

      hName  = "h2DXcellEvenChargeFourthHit_"                             + planeName;
      hTitle = "Predicted cell charge vs. X coordinate "                  + planeName;
      h2DXcellEvenChargeFourthHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)xPitch/2, -(xPitch/2), xPitch/2, nBinsCharge, 0, 50000)));


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

      hName  = "h2DYcellChargeFourthHit_"                             + planeName;
      hTitle = "Predicted cell charge vs. Y coordinate "              + planeName;
      h2DYcellChargeFourthHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)yPitch/2, -(yPitch/2), yPitch/2, nBinsCharge, 0, 50000)));



      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("XcellCharge1D");


      // #################
      // # 1D histograms #
      // #################
      hName  = "h1DXcellCharge_"                                                             + planeName;
      hTitle = "Predicted cell charge - X coordinate (normalized to hits) "                  + planeName;
      h1DXcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch)/2,xPitch/2)));

      hName  = "h1DXcellOddCharge_"                                                             + planeName;
      hTitle = "Predicted cell charge - X coordinate (normalized to hits) "                     + planeName;
      h1DXcellOddCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      
      hName  = "h1DXcellEvenCharge_"                                                             + planeName;
      hTitle = "Predicted cell charge - X coordinate (normalized to hits) "                      + planeName;
      h1DXcellEvenCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      /*----------------*/
      hName  = "h1DXcellChargeNorm_"                                                         + planeName;
      hTitle = "Predicted cell charge - X coordinate - all hits normalization "              + planeName;
      h1DXcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch)/2,xPitch/2)));

      hName  = "h1DXcellOddChargeNorm_"                                                             + planeName;
      hTitle = "Predicted cell charge - X coordinate  - all hits normalization "                    + planeName;
      h1DXcellOddChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      
      hName  = "h1DXcellEvenChargeNorm_"                                                             + planeName;
      hTitle = "Predicted cell charge - X coordinate - all hits normalization "                      + planeName;
      h1DXcellEvenChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      /*++++++++++++++++*/
      hName  = "h1DXcellChargeSecondHit_"                                                              + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit - X coordinate (normalized to tracks) " + planeName;
      h1DXcellChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch)/2,xPitch/2)));
      
      hName  = "h1DXcellOddChargeSecondHit_"                                                              + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit - X coordinate (normalized to tracks) "    + planeName;
      h1DXcellOddChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      
      hName  = "h1DXcellEvenChargeSecondHit_"                                                              + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit - X coordinate (normalized to tracks) "    + planeName;
      h1DXcellEvenChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      /*---------------*/
      hName  = "h1DXcellChargeSecondHitNorm_"                                                + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate " + planeName;
      h1DXcellChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch)/2,xPitch/2)));
      
      hName  = "h1DXcellOddChargeSecondHitNorm_"                                                + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate "    + planeName;
      h1DXcellOddChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      
      hName  = "h1DXcellEvenChargeSecondHitNorm_"                                                + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate "     + planeName;
      h1DXcellEvenChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

			//>> wsi 11/12/17
			hName  = "h1DXcellChargeFourthHit_"                                                              + planeName;
      hTitle = "Up to 4 adjacent hits total charge, fourth hit - X coordinate (normalized to tracks) " + planeName;
      h1DXcellChargeFourthHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch)/2,xPitch/2)));

      hName  = "h1DXcellOddChargeFourthHit_"                                                              + planeName;
      hTitle = "Up to 4 adjacent hits total charge, fourth hit - X coordinate (normalized to tracks) "    + planeName;
      h1DXcellOddChargeFourthHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

      hName  = "h1DXcellEvenChargeFourthHit_"                                                              + planeName;
      hTitle = "Up to 4 adjacent hits total charge, fourth hit - X coordinate (normalized to tracks) "     + planeName;
      h1DXcellEvenChargeFourthHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      /*--------------*/
      hName  = "h1DXcellChargeFourthHitNorm_"                                                + planeName;
      hTitle = "Up to 4 adjacent hits total charge, fourth hit distribution - X coordinate " + planeName;
      h1DXcellChargeFourthHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch)/2,xPitch/2)));

      hName  = "h1DXcellOddChargeFourthHitNorm_"                                                + planeName;
      hTitle = "Up to 4 adjacent hits total charge, fourth hit distribution - X coordinate "    + planeName;
      h1DXcellOddChargeFourthHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));
      
      hName  = "h1DXcellEvenChargeFourthHitNorm_"                                                + planeName;
      hTitle = "Up to 4 adjacent hits total charge, fourth hit distribution - X coordinate "     + planeName;
      h1DXcellEvenChargeFourthHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)xPitch/2,-(xPitch/2),xPitch/2)));

			//<< wsi 11/12/17





      theAnalysisManager_->cd("Charge/" + planeName);
      theAnalysisManager_->mkdir("YcellCharge1D");


      // #################
      // # 1D histograms #
      // #################
      hName  = "h1DYcellCharge_"                                                             + planeName;
      hTitle = "Predicted cell charge - Y coordinate (normalized to hits) "                  + planeName;
      h1DYcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));
//      h1DYcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch,-(yPitch),yPitch)));

      hName  = "h1DYcellChargeNorm_"                                                         + planeName;
      hTitle = "Predicted cell charge - Y coordinate - all hits normalization "              + planeName;
      h1DYcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));
//      h1DYcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch,-(yPitch),yPitch)));

      hName  = "h1DYcellChargeSecondHit_"                                                              + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit - Y coordinate (normalized to tracks) " + planeName;
      h1DYcellChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));
//      h1DYcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch,-(yPitch),yPitch)));

      hName  = "h1DYcellChargeSecondHitNorm_"                                                + planeName;
      hTitle = "Up to 2 adjacent hits total charge, second hit distribution - Y coordinate " + planeName;
      h1DYcellChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));
//      h1DYcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch,-(yPitch),yPitch)));

			//>> wsi 11/12/17
			hName  = "h1DYcellChargeFourthHit_"                                                              + planeName;
      hTitle = "Up to 4 adjacent hits total charge, fourth hit - Y coordinate (normalized to tracks) " + planeName;
      h1DYcellChargeFourthHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));

      hName  = "h1DYcellChargeFourthHitNorm_"                                                + planeName;
      hTitle = "Up to 4 adjacent hits total charge, fourth hit distribution - Y coordinate " + planeName;
      h1DYcellChargeFourthHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)yPitch/2,-(yPitch/2),yPitch/2)));
			//<< wsi 11/12/17




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
