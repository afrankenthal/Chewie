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

#include "ResolutionUniMiB.h"

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

#include <TH2F.h>
#include <TF1.h>
#include <TFile.h>

#include <iostream>


// @@@ Hard coded parameters @@@
#define ONLYdoubleHITS false // Process only clusters of size 2
// ============================


//=======================================================================
ResolutionUniMiB::ResolutionUniMiB(AnalysisManager* analysisManager, int nOfThreads) :
  Analysis(analysisManager,nOfThreads),
  thePlaneMapping_(0),
  theWindowsManager_(0),
  theXmlParser_(analysisManager->getXmlParser())
  
{
  STDLINE("Running ResolutionUniMiB analysis",ACCyan);

  thePlaneMapping_ = new PlanesMapping();
}

//=======================================================================
ResolutionUniMiB::~ResolutionUniMiB(void)
{
  if (thePlaneMapping_) delete thePlaneMapping_;

  destroy();
}

//=======================================================================
void ResolutionUniMiB::destroy(void)
{
  if (Analysis::fDoNotDelete_) return;
  
  std::vector<TH1F*>::iterator it1;
  std::vector<TH2F*>::iterator it2;
  
  for (it1=hXResiduals_                .begin(); it1!=hXResiduals_                .end(); it1++) delete *it1; hXResiduals_                .clear();
  for (it1=hYResiduals_                .begin(); it1!=hYResiduals_                .end(); it1++) delete *it1; hYResiduals_                .clear();
  
  for (it1=hXResidualCalculatedSize2_  .begin(); it1!=hXResidualCalculatedSize2_  .end(); it1++) delete *it1; hXResidualCalculatedSize2_  .clear();
  for (it1=hYResidualCalculatedSize2_  .begin(); it1!=hYResidualCalculatedSize2_  .end(); it1++) delete *it1; hYResidualCalculatedSize2_  .clear();
  
  for (it1=hXResidualsClusterSize2_    .begin(); it1!=hXResidualsClusterSize2_    .end(); it1++) delete *it1; hXResidualsClusterSize2_    .clear();
  for (it1=hYResidualsClusterSize2_    .begin(); it1!=hYResidualsClusterSize2_    .end(); it1++) delete *it1; hYResidualsClusterSize2_    .clear();
  
  for (std::map<std::string,TH1F*>::iterator it=hPredictedXErrors_ .begin(); it!=hPredictedXErrors_.end(); it++) delete it->second; hPredictedXErrors_.clear();
  for (std::map<std::string,TH1F*>::iterator it=hPredictedYErrors_ .begin(); it!=hPredictedYErrors_.end(); it++) delete it->second; hPredictedYErrors_.clear();
  
  for (it2=h2DCorrelationsResidualXvsX_.begin(); it2!=h2DCorrelationsResidualXvsX_.end(); it2++) delete *it2; h2DCorrelationsResidualXvsX_.clear();
  for (it2=h2DCorrelationsResidualXvsY_.begin(); it2!=h2DCorrelationsResidualXvsY_.end(); it2++) delete *it2; h2DCorrelationsResidualXvsY_.clear();
  for (it2=h2DCorrelationsResidualYvsY_.begin(); it2!=h2DCorrelationsResidualYvsY_.end(); it2++) delete *it2; h2DCorrelationsResidualYvsY_.clear();
  for (it2=h2DCorrelationsResidualYvsX_.begin(); it2!=h2DCorrelationsResidualYvsX_.end(); it2++) delete *it2; h2DCorrelationsResidualYvsX_.clear();
  
  for (it1=hCorrelationsResidualXvsX_  .begin(); it1!=hCorrelationsResidualXvsX_  .end(); it1++) delete *it1; hCorrelationsResidualXvsX_  .clear();
  for (it1=hCorrelationsResidualXvsY_  .begin(); it1!=hCorrelationsResidualXvsY_  .end(); it1++) delete *it1; hCorrelationsResidualXvsY_  .clear();
  for (it1=hCorrelationsResidualYvsY_  .begin(); it1!=hCorrelationsResidualYvsY_  .end(); it1++) delete *it1; hCorrelationsResidualYvsY_  .clear();
  for (it1=hCorrelationsResidualYvsX_  .begin(); it1!=hCorrelationsResidualYvsX_  .end(); it1++) delete *it1; hCorrelationsResidualYvsX_  .clear();
}

//=======================================================================
void ResolutionUniMiB::predictedErrors(bool pass, int planeID, const Data& data, int threadNumber)
{
    if (!pass) return;

    std::string planeName = thePlaneMapping_->getPlaneName(planeID);
    THREADED(hPredictedXErrors_[planeName])->Fill(data.getXErrorPredictedGlobal(planeID));
    THREADED(hPredictedYErrors_[planeName])->Fill(data.getYErrorPredictedGlobal(planeID));
}


//=======================================================================
void ResolutionUniMiB::calculateXresiduals(bool pass, int planeID, const Data &data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;
  

  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;


  // ############################################################################################
  // # These residuals are computed only if thracks have cluster size 2 on each telescope plane #
  // ############################################################################################
  int counter = 0;
  for (int p = 8; p < 16; p++) if (data.getClusterSize(p) == 2) counter++;
  if (counter != 8) return;


  std::string planeName;
  std::string toGet;
  float maxPitchX;
  float xPixelResidual;
  float xPixelEdgeResidual = 0;

  planeName      = thePlaneMapping_->getPlaneName(planeID);
  maxPitchX      = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
  xPixelResidual = data.getXPixelResidualLocal(planeID);

  if (data.getXPitchLocal(planeID) > maxPitchX) return;


  if (xPixelResidual > 0) xPixelEdgeResidual = xPixelResidual - data.getXPitchLocal(planeID)/2;
  else                    xPixelEdgeResidual = xPixelResidual + data.getXPitchLocal(planeID)/2;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  int           size         = data.getClusterSize(planeID);
    
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < size; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	  || !data.getIsPixelCalibrated(h,planeID)                                                           // Pixels are calibrated
	  ||  data.getClusterPixelRow  (h,planeID) != rowPredicted)                                          // Hits are on the same row (sharing is along the row - x direction)
	return;
    }


  int   hitID       = -1;
  int   totalCharge =  0;
  int   chargeLeft  =  0;
  int   chargeRight =  0;
  float asimmetry   =  0;
  float xMeasured;


  // ############################################
  // # Require cluster with col = predicted col #
  // ############################################
  for (int h = 0; h < size; h++)
    {
      if (data.getClusterPixelCol(h,planeID) == colPredicted)
	{
	  hitID = h;
	  break;
	}
    }
  if (hitID == -1) return;


  if (size == 2)
    {
      for (int h = 0; h < size; h++)
	{
	  if (xPixelResidual > 0 && (colPredicted - data.getClusterPixelCol(h,planeID)) == -1)
	    {
	      chargeRight = data.getClusterPixelCharge(h    ,planeID);
	      chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
	      break;
	    }
	  else if (xPixelResidual <= 0 && (colPredicted - data.getClusterPixelCol(h,planeID)) == 1)
	    {
	      chargeRight = data.getClusterPixelCharge(hitID,planeID);
	      chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
	      break;
	    }
	  else if (xPixelResidual > 0 && (colPredicted - data.getClusterPixelCol(h,planeID)) == 1)
	    {
	      chargeRight = data.getClusterPixelCharge(hitID,planeID);
	      chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
	      xPixelEdgeResidual = xPixelResidual + data.getXPitchLocal(planeID)/2;
	      break;
	    }
	  else if (xPixelResidual < 0 && (colPredicted - data.getClusterPixelCol(h,planeID)) == -1)
	    {
	      chargeRight = data.getClusterPixelCharge(h    ,planeID);
	      chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
	      xPixelEdgeResidual = (xPixelResidual - data.getXPitchLocal(planeID)/2);
	      break;
	    }
	}
    }

  totalCharge = chargeLeft + chargeRight;
  asimmetry   = (float)(chargeLeft - chargeRight) / totalCharge;

  if (asimmetry >= -1. && asimmetry <= 1.)
    {
      toGet = "Charge/" + planeName +  "/XAsimmetry/h1DXcellChargeAsimmetryInv_" + planeName;

      if ((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))
	{
	  // #########################
	  // # Correct for asimmetry #
	  // #########################
	  if ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fXAsimmetryFit") != NULL)
	    {
	      xMeasured = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fXAsimmetryFit"))->Eval(asimmetry);
	      if (size == 2) THREADED(hXResidualCalculatedSize2_[planeID])->Fill(xMeasured - xPixelEdgeResidual);
	    }
	}
    }
}

//=======================================================================
void ResolutionUniMiB::calculateYresiduals(bool pass, int planeID, const Data &data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;
  
  
  // ############################################################################################
  // # These residuals are computed only if thracks have cluster size 2 on each telescope plane #
  // ############################################################################################
  int counter = 0;
  for (int p = 8; p < 16; p++) if (data.getClusterSize(p) == 2) counter++;
  if (counter != 8) return;
  
  
  std::string planeName;
  std::string toGet;
  float maxPitchY;
  float yPixelResidual;
  float yPixelEdgeResidual = 0;

  planeName      = thePlaneMapping_->getPlaneName(planeID);
  maxPitchY      = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());
  yPixelResidual = data.getYPixelResidualLocal(planeID);

  if (data.getYPitchLocal(planeID) > maxPitchY) return;


  if (yPixelResidual > 0) yPixelEdgeResidual = yPixelResidual - data.getYPitchLocal(planeID)/2;
  else                    yPixelEdgeResidual = yPixelResidual + data.getYPitchLocal(planeID)/2;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  int           size         = data.getClusterSize(planeID);
  
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  for (int h = 0; h < size; h++)
    {
      if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // Hits are in the window
	  || !data.getIsPixelCalibrated(h,planeID)                                                           // Pixels are calibrated
	  ||  data.getClusterPixelCol  (h,planeID) != colPredicted)                                          // Hits are on the same column (sharing is along the column - y direction)
	return;
    }
  

  int   hitID       = -1;
  int   totalCharge =  0;
  int   chargeDown  =  0;
  int   chargeUp    =  0;
  float asimmetry   =  0;
  float yMeasured;


  // ############################################
  // # Require cluster with raw = predicted raw #
  // ############################################
  for (int h = 0 ; h < size; h++)
    {
      if (data.getClusterPixelRow(h,planeID) == rowPredicted)
	{
	  hitID = h;
	  break;
	}
    }
  if (hitID == -1) return;
  

  if (size == 2)
    {
      for (int h = 0; h < size; ++h)
        {
	  if (yPixelResidual > 0 && (rowPredicted - data.getClusterPixelRow(h,planeID)) == -1)
            {
	      chargeUp   = data.getClusterPixelCharge(h    ,planeID);
	      chargeDown = data.getClusterPixelCharge(hitID,planeID);
	      break;
            }
	  else if (yPixelResidual <= 0 && (rowPredicted - data.getClusterPixelRow(h,planeID)) == 1)
            {
	      chargeUp   = data.getClusterPixelCharge(hitID,planeID);
	      chargeDown = data.getClusterPixelCharge(h    ,planeID);
	      break;
            }
	  else if (yPixelResidual > 0 && (rowPredicted - data.getClusterPixelRow(h,planeID)) == 1)
            {
	      chargeUp   = data.getClusterPixelCharge(hitID,planeID);
	      chargeDown = data.getClusterPixelCharge(h    ,planeID);
	      yPixelEdgeResidual = yPixelResidual + data.getYPitchLocal(planeID)/2;
	      break;
            }
	  else if (yPixelResidual < 0 && (rowPredicted - data.getClusterPixelRow(h,planeID)) == -1)
            {
	      chargeUp   = data.getClusterPixelCharge(h    ,planeID);
	      chargeDown = data.getClusterPixelCharge(hitID,planeID);
	      yPixelEdgeResidual = yPixelResidual - data.getYPitchLocal(planeID)/2;
	      break;
            }
        }
    }

  totalCharge = chargeDown + chargeUp;
  asimmetry   = (float)(chargeDown - chargeUp)/totalCharge;

  if (asimmetry >= -1. && asimmetry <= 1.)
    {
      toGet = "Charge/" + planeName +  "/YAsimmetry/h1DYcellChargeAsimmetryInv_" + planeName;
	
      if ((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))
	{
	  // #########################
	  // # Correct for asimmetry #
	  // #########################
	  if ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fYAsimmetryFit") != NULL)
	    {
	      yMeasured = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fYAsimmetryFit"))->Eval(asimmetry);
	      if (size == 2) THREADED(hYResidualCalculatedSize2_[planeID])->Fill(yMeasured - yPixelEdgeResidual);
	    }
	}
    }
}

//=======================================================================
void ResolutionUniMiB::xResolution(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;


  if (!pass || !data.getBelongsToTrack(planeID)) return;
  
  
  // ############################################################################################
  // # These residuals are computed only if thracks have cluster size 2 on each telescope plane #
  // ############################################################################################
  int counter = 0;
  for (int p = 8; p < 16; p++) if (data.getClusterSize(p) == 2) counter++;
  if (counter != 8) return;


  const Window* theWindow = theWindowsManager_->getWindow(planeID);
  int           size      = data.getClusterSize(planeID);
  if (size > maxClusterSize) return;


  for (int h = 0; h < size; h++)
    if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber())) return;

  THREADED(hXResiduals_[planeID])->Fill(data.getXTrackResidualLocal(planeID));

  if (theWindow->checkTimeWindowAbout(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber()))
    {
      THREADED(h2DCorrelationsResidualXvsX_[planeID])->Fill(data.getXPredictedLocal(planeID),data.getXTrackResidualLocal(planeID));
      THREADED(h2DCorrelationsResidualXvsY_[planeID])->Fill(data.getYPredictedLocal(planeID),data.getXTrackResidualLocal(planeID));
    }


  if (size == 2)
    {
      if (data.getClusterPixelRow(0,planeID) != data.getClusterPixelRow(1,planeID)) return;
      
      THREADED(hXResidualsClusterSize2_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
    }
}

//=======================================================================
void ResolutionUniMiB::yResolution(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;
  
  
  if (!pass || !data.getBelongsToTrack(planeID)) return;


  // ############################################################################################
  // # These residuals are computed only if thracks have cluster size 2 on each telescope plane #
  // ############################################################################################
  int counter = 0;
  for (int p = 8; p < 16; p++) if (data.getClusterSize(p) == 2) counter++;
  if (counter != 8) return;


  const Window* theWindow = theWindowsManager_->getWindow(planeID);
  int           size      = data.getClusterSize(planeID);
  if (size > maxClusterSize) return;


  for (int h = 0; h < size; h++)
    if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber())) return;
  
  THREADED(hYResiduals_[planeID])->Fill(data.getYTrackResidualLocal(planeID));

  if (theWindow->checkTimeWindowAbout(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber()))
    {
      THREADED(h2DCorrelationsResidualYvsX_[planeID])->Fill(data.getXPredictedLocal(planeID),data.getYTrackResidualLocal(planeID));
      THREADED(h2DCorrelationsResidualYvsY_[planeID])->Fill(data.getYPredictedLocal(planeID),data.getYTrackResidualLocal(planeID));
    }
  
  
  if (size == 2)
    {
      if (data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID)) return;
      
      THREADED(hYResidualsClusterSize2_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
    }
}

//=======================================================================
void ResolutionUniMiB::setCutsFormula(std::map<std::string,std::string> cutsList, std::vector<TTree*> tree)
{
  std::vector<TTreeFormula*> formulasVector;
  
  for (std::map<std::string,std::string>::iterator it = cutsList.begin(); it != cutsList.end(); it++)
    {
      if ((it->first) == "main cut" && (it->second).size() == 0)
	STDLINE("WARNING: no main cut set in resolution analysis ! Default value = true !", ACRed);
      
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
bool ResolutionUniMiB::passStandardCuts(int planeID, const Data &data)
{
  if (!theXmlParser_->getAnalysesFromString("Resolution")->applyStandardCuts()) return true;

  int minHits = 7; // In order to have 8 telescope hits
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos)
    minHits = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str());

  if (data.getNumberOfTelescopeHits() >= minHits) return true;
  else                                            return false;
}

//=======================================================================
void ResolutionUniMiB::beginJob(void)
{
  theWindowsManager_ = theAnalysisManager_->getWindowsManager();
  book();
}

//=======================================================================
void ResolutionUniMiB::analyze(const Data& data, int threadNumber)
{   
  bool mainCut = true;
  if (cutsFormulas_.find("main cut") != cutsFormulas_.end())
    mainCut = cutsFormulas_["main cut"][threadNumber]->EvalInstance();
  
  if (!mainCut) return;

  bool errorsCut = true;
  if (cutsFormulas_.find("errors") != cutsFormulas_.end())
    errorsCut = cutsFormulas_["errors"][threadNumber]->EvalInstance();

  bool xResolutionCut = true;
  if (cutsFormulas_.find("X resolution") != cutsFormulas_.end())
    xResolutionCut = cutsFormulas_["X resolution"][threadNumber]->EvalInstance();

  bool yResolutionCut = true;
  if (cutsFormulas_.find("Y resolution") != cutsFormulas_.end())
    yResolutionCut = cutsFormulas_["Y resolution"][threadNumber]->EvalInstance();


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
	
      calculateXresiduals (xResolutionCut,p,data,threadNumber);
      calculateYresiduals (yResolutionCut,p,data,threadNumber);
      xResolution         (xResolutionCut,p,data,threadNumber);
      yResolution         (yResolutionCut,p,data,threadNumber);
      
      if (thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos)
	predictedErrors(errorsCut,p,data,threadNumber);
    }
}

//=======================================================================
void ResolutionUniMiB::endJob(void)
{
  std::stringstream ss;

  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      std::string planeName = thePlaneMapping_->getPlaneName(p);
      ss.str("") ; ss << "Adding threads for plane " << p;
      STDLINE(ss.str().c_str(),ACYellow);

      ADD_THREADED(hXResiduals_                 [p]);
      ADD_THREADED(hYResiduals_                 [p]);

      ADD_THREADED(hXResidualCalculatedSize2_   [p]);
      ADD_THREADED(hYResidualCalculatedSize2_   [p]);

      ADD_THREADED(hXResidualsClusterSize2_     [p]);
      ADD_THREADED(hYResidualsClusterSize2_     [p]);

      ADD_THREADED(h2DCorrelationsResidualXvsX_ [p]);
      ADD_THREADED(h2DCorrelationsResidualXvsY_ [p]);
      ADD_THREADED(h2DCorrelationsResidualYvsY_ [p]);
      ADD_THREADED(h2DCorrelationsResidualYvsX_ [p]);

      ADD_THREADED(hCorrelationsResidualXvsX_   [p]);
      ADD_THREADED(hCorrelationsResidualXvsY_   [p]);
      ADD_THREADED(hCorrelationsResidualYvsY_   [p]);
      ADD_THREADED(hCorrelationsResidualYvsX_   [p]);

      STDLINE("Threading phase completed",ACGreen);


      STDLINE("Setting styles...",ACWhite);

      hCorrelationsResidualXvsX_   [p]->SetMarkerStyle(20);
      hCorrelationsResidualXvsX_   [p]->SetMarkerSize(0.6);

      hCorrelationsResidualXvsY_   [p]->SetMarkerStyle(20);
      hCorrelationsResidualXvsY_   [p]->SetMarkerSize(0.6);

      hCorrelationsResidualYvsY_   [p]->SetMarkerStyle(20);
      hCorrelationsResidualYvsY_   [p]->SetMarkerSize(0.6);

      hCorrelationsResidualYvsX_   [p]->SetMarkerStyle(20);
      hCorrelationsResidualYvsX_   [p]->SetMarkerSize(0.6);


      hXResiduals_                 [p]->GetXaxis()->SetTitle("x residual (um)");
      hYResiduals_                 [p]->GetXaxis()->SetTitle("y residual (um)");

      hXResidualCalculatedSize2_   [p]->GetXaxis()->SetTitle("x residual (um)");
      hYResidualCalculatedSize2_   [p]->GetXaxis()->SetTitle("y residual (um)");

      hXResidualsClusterSize2_     [p]->GetXaxis()->SetTitle("x residual (um)");
      hYResidualsClusterSize2_     [p]->GetXaxis()->SetTitle("y residual (um)");

      h2DCorrelationsResidualXvsX_ [p]->GetXaxis()->SetTitle("x (um)");
      h2DCorrelationsResidualXvsX_ [p]->GetYaxis()->SetTitle("x residual (um)");

      h2DCorrelationsResidualXvsY_ [p]->GetXaxis()->SetTitle("y (um)");
      h2DCorrelationsResidualXvsY_ [p]->GetYaxis()->SetTitle("x residual (um)");

      h2DCorrelationsResidualYvsY_ [p]->GetXaxis()->SetTitle("y (um)");
      h2DCorrelationsResidualYvsY_ [p]->GetYaxis()->SetTitle("y residual (um)");

      h2DCorrelationsResidualYvsX_ [p]->GetXaxis()->SetTitle("x (um)");
      h2DCorrelationsResidualYvsX_ [p]->GetYaxis()->SetTitle("y residual (um)");


      hCorrelationsResidualXvsX_   [p]->GetXaxis()->SetTitle("x (um)");
      hCorrelationsResidualXvsX_   [p]->GetYaxis()->SetTitle("x residual (um)");
      hCorrelationsResidualXvsX_   [p]->GetYaxis()->SetRangeUser(-20.,20.);

      hCorrelationsResidualXvsY_   [p]->GetXaxis()->SetTitle("y (um)");
      hCorrelationsResidualXvsY_   [p]->GetYaxis()->SetTitle("x residual (um)");
      hCorrelationsResidualXvsY_   [p]->GetYaxis()->SetRangeUser(-20.,20.);

      hCorrelationsResidualYvsY_   [p]->GetXaxis()->SetTitle("y (um)");
      hCorrelationsResidualYvsY_   [p]->GetYaxis()->SetTitle("y residual (um)");
      hCorrelationsResidualYvsY_   [p]->GetYaxis()->SetRangeUser(-20.,20.);

      hCorrelationsResidualYvsX_   [p]->GetXaxis()->SetTitle("x (um)");
      hCorrelationsResidualYvsX_   [p]->GetYaxis()->SetTitle("y residual (um)");
      hCorrelationsResidualYvsX_   [p]->GetYaxis()->SetRangeUser(-20.,20.);

	
      if (thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos)
	{
	  ADD_THREADED(hPredictedXErrors_[thePlaneMapping_->getPlaneName(p)]);
	  ADD_THREADED(hPredictedYErrors_[thePlaneMapping_->getPlaneName(p)]);
	    
	  hPredictedXErrors_[thePlaneMapping_->getPlaneName(p)]->GetXaxis()->SetTitle("x error (um)");
	  hPredictedYErrors_[thePlaneMapping_->getPlaneName(p)]->GetXaxis()->SetTitle("y error (um)");
	}

      std::string hName;
      std::string hTitle;

      theAnalysisManager_->cd("Resolution/" + planeName + "/Correlations");

      hName  = "hCorrelationsResidualXvsX_"    + planeName;
      hTitle = "Correlation X residuals vs X " + planeName;
      Utilities::customProfileX(h2DCorrelationsResidualXvsX_[p],hCorrelationsResidualXvsX_[p],hName.c_str(),hTitle.c_str(),1,-1,0);

      hName  = "hCorrelationsResidualXvsY_"    + planeName;
      hTitle = "Correlation X residuals vs Y " + planeName;
      Utilities::customProfileX(h2DCorrelationsResidualXvsY_[p],hCorrelationsResidualXvsY_[p],hName.c_str(),hTitle.c_str(),1,-1,0);

      hName  = "hCorrelationsResidualYvsY_"    + planeName;
      hTitle = "Correlation Y residuals vs Y " + planeName;
      Utilities::customProfileX(h2DCorrelationsResidualYvsY_[p],hCorrelationsResidualYvsY_[p],hName.c_str(),hTitle.c_str(),1,-1,0);

      hName  = "hCorrelationsResidualYvsX_"    + planeName;
      hTitle = "Correlation Y residuals vs X " + planeName;
      Utilities::customProfileX(h2DCorrelationsResidualYvsX_[p],hCorrelationsResidualYvsX_[p],hName.c_str(),hTitle.c_str(),1,-1,0);
    }
}

//=======================================================================
void ResolutionUniMiB::book(void)
{
  destroy();
  
  std::string hName;
  std::string hTitle;
  std::string planeName;
  
  float       xPitch;
  float       yPitch;
  float       halfWidth = 500;
  
  int         lowerCol;
  int         higherCol;
  int         lowerRow;
  int         higherRow;
  int         nBins = 250;


  theAnalysisManager_->cd("/");
  theAnalysisManager_->mkdir("Resolution");

  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      planeName = thePlaneMapping_->getPlaneName(p);

      xPitch    = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
      yPitch    = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

      lowerCol  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerCol()).c_str());
      higherCol = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherCol()).c_str());
      lowerRow  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerRow()).c_str());
      higherRow = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherRow()).c_str());


      int numberOfRows    = higherRow - lowerRow + 1;
      int numberOfColumns = higherCol - lowerCol + 1;
      
      theAnalysisManager_->cd("Resolution");
      theAnalysisManager_->mkdir(planeName);


      theAnalysisManager_->mkdir("XResiduals");

      hName  = "hXResiduals_"                               + planeName;
      hTitle = "X residuals  "                              + planeName;
      hXResiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), nBins, -halfWidth, halfWidth)));

      hName  = "hXResidualCalculatedSize2_"                                   + planeName;
      hTitle = "X residuals calculated from asimmetry fit (Clusters Size 2) " + planeName;
      hXResidualCalculatedSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBins, -halfWidth, halfWidth)));
      
      hName  = "hXResidualsClusterSize2_"                   + planeName;
      hTitle = "X residuals cluster size 2 "                + planeName;
      hXResidualsClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), nBins, -halfWidth, halfWidth)));
      
      
      theAnalysisManager_->cd("Resolution/" + planeName);
      theAnalysisManager_->mkdir("YResiduals");
      
      hName  = "hYResiduals_"                               + planeName;
      hTitle = "Y residuals "                               + planeName;
      hYResiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), nBins, -halfWidth, halfWidth)));

      hName  = "hYResidualCalculatedSize2_"                                  + planeName;
      hTitle = "Y residuals calculated from asimmetry fit (Cluster Size 2) " + planeName;
      hYResidualCalculatedSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBins, -halfWidth, halfWidth)));

      hName  = "hYResidualsClusterSize2_"                   + planeName;
      hTitle = "Y residuals cluster size 2 "                + planeName;
      hYResidualsClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), nBins, -halfWidth, halfWidth)));


      if (planeName.find("Dut") != std::string::npos)
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
	h2DCorrelationsResidualXvsX_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfColumns + 2, (float)xPitch*lowerCol, (float)xPitch*(lowerCol+numberOfColumns + 2), nBins, -halfWidth, halfWidth)));
	
        hName  = "h2DCorrelationsResidualXvsY_"                        + planeName;
        hTitle = "Correlation X residuals vs Y "                       + planeName;
	h2DCorrelationsResidualXvsY_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfRows + 1, (float)yPitch*lowerRow, (float)yPitch*(lowerRow + numberOfRows + 1), nBins, -halfWidth, halfWidth)));
	
        hName  = "h2DCorrelationsResidualYvsY_"                        + planeName;
        hTitle = "Correlation Y residuals vs Y "                       + planeName;
	h2DCorrelationsResidualYvsY_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfRows + 1, (float)yPitch*lowerRow, (float)yPitch*(lowerRow + numberOfRows + 1), nBins, -halfWidth, halfWidth)));

        hName  = "h2DCorrelationsResidualYvsX_"                        + planeName;
        hTitle = "Correlation Y residuals vs X "                        + planeName;
        h2DCorrelationsResidualYvsX_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), numberOfColumns + 2, (float)xPitch*lowerCol, (float)xPitch*(lowerCol+numberOfColumns + 2), nBins, -halfWidth, halfWidth)));

        hName  = "hCorrelationsResidualXvsX_"                          + planeName;
        hTitle = "Correlation X residuals vs X "                       + planeName;
        hCorrelationsResidualXvsX_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), numberOfColumns + 2, (float)xPitch*lowerCol, (float)xPitch*(lowerCol+numberOfColumns + 2))));

        hName  = "hCorrelationsResidualYvsX_"                          + planeName;
        hTitle = "Correlation Y residuals vs X "                       + planeName;
        hCorrelationsResidualYvsX_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), numberOfColumns + 2, (float)xPitch*lowerCol, (float)xPitch*(lowerCol+numberOfColumns + 2))));

        hName  = "hCorrelationsResidualYvsY_"                          + planeName;
        hTitle = "Correlation Y residuals vs Y "                       + planeName;
        hCorrelationsResidualYvsY_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), numberOfRows + 1, (float)yPitch*lowerRow, (float)yPitch*(lowerRow+numberOfRows + 1))));

        hName  = "hCorrelationsResidualXvsY_"                          + planeName;
        hTitle = "Correlation X residuals vs Y "                       + planeName;
        hCorrelationsResidualXvsY_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), numberOfRows + 1, (float)yPitch*lowerRow, (float)yPitch*(lowerRow+numberOfRows + 1))));
    }
}
