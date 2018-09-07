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

#include "ResolutionAmericans.h"

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
ResolutionAmericans::ResolutionAmericans(AnalysisManager* analysisManager, int nOfThreads) :
  Analysis(analysisManager,nOfThreads),
  thePlaneMapping_(0),
  theWindowsManager_(0),
  theXmlParser_(analysisManager->getXmlParser()),
  thePitchTranslation25x100_(new SmallPitchTranslation25x100()),
  thePitchTranslation50x50_(new SmallPitchTranslation50x50())
  
{
  STDLINE("Running ResolutionAmericans analysis",ACCyan);

  thePlaneMapping_ = new PlanesMapping();
}

//=======================================================================
ResolutionAmericans::~ResolutionAmericans(void)
{
  if (thePlaneMapping_) delete thePlaneMapping_;

  destroy();
}

//=======================================================================
void ResolutionAmericans::destroy(void)
{
  if (Analysis::fDoNotDelete_) return;
  
  std::vector<TH1F*>::iterator it1;
  std::vector<TH2F*>::iterator it2;
  
  for (it1=hXResiduals_                .begin(); it1!=hXResiduals_                .end(); it1++) delete *it1; hXResiduals_                .clear();
  for (it1=hYResiduals_                .begin(); it1!=hYResiduals_                .end(); it1++) delete *it1; hYResiduals_                .clear();
  
  for (it1=hXResidualCalculatedSize2_  .begin(); it1!=hXResidualCalculatedSize2_  .end(); it1++) delete *it1; hXResidualCalculatedSize2_  .clear();
  for (it1=hYResidualCalculatedSize2_  .begin(); it1!=hYResidualCalculatedSize2_  .end(); it1++) delete *it1; hYResidualCalculatedSize2_  .clear();
  
  for (it1=hXResidualsClusterSize2_    .begin(); it1!=hXResidualsClusterSize2_    .end(); it1++) delete *it1; hXResidualsClusterSize2_    .clear();
  for (it1=hXResidualsClusterSize1_    .begin(); it1!=hXResidualsClusterSize1_    .end(); it1++) delete *it1; hXResidualsClusterSize1_    .clear();
  for (it1=hXResidualsClusterSize1Right_    .begin(); it1!=hXResidualsClusterSize1Right_    .end(); it1++) delete *it1; hXResidualsClusterSize1Right_    .clear();
  for (it1=hXResidualsClusterSize1Left_    .begin(); it1!=hXResidualsClusterSize1Left_    .end(); it1++) delete *it1; hXResidualsClusterSize1Left_    .clear();

  for (it1=hYResidualsClusterSize2_    .begin(); it1!=hYResidualsClusterSize2_    .end(); it1++) delete *it1; hYResidualsClusterSize2_    .clear();
  for (it1=hYResidualsClusterSize1_    .begin(); it1!=hYResidualsClusterSize1_    .end(); it1++) delete *it1; hYResidualsClusterSize1_    .clear();
  for (it1=hYResidualsClusterSize1Right_    .begin(); it1!=hYResidualsClusterSize1Right_    .end(); it1++) delete *it1; hYResidualsClusterSize1Right_    .clear();
  for (it1=hYResidualsClusterSize1Left_    .begin(); it1!=hYResidualsClusterSize1Left_    .end(); it1++) delete *it1; hYResidualsClusterSize1Left_    .clear();
  
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
void ResolutionAmericans::predictedErrors(bool pass, int planeID, const Data& data, int threadNumber)
{
    if (!pass) return;

    std::string planeName = thePlaneMapping_->getPlaneName(planeID);
    THREADED(hPredictedXErrors_[planeName])->Fill(data.getXErrorPredictedGlobal(planeID));
    THREADED(hPredictedYErrors_[planeName])->Fill(data.getYErrorPredictedGlobal(planeID));
}


//=======================================================================
void ResolutionAmericans::calculateXresiduals(bool pass, int planeID, const Data &data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;
  

  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;


  // ############################################################################################
  // # These residuals are computed only if thracks have cluster size 2 on each telescope plane #
  // ############################################################################################
//  int counter = 0;
//  for (int p = 8; p < 16; p++) if (data.getClusterSize(p) == 2) counter++;
//  if (counter != 8) return;


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
  float asymmetry   =  0;
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
  asymmetry   = (float)(chargeLeft - chargeRight) / totalCharge;

  if (asymmetry >= -1. && asymmetry <= 1.)
    {
      toGet = "Charge/" + planeName +  "/XAsymmetry/h1DXcellChargeAsymmetryInv_" + planeName;

      if ((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))
	{
	  // #########################
	  // # Correct for asymmetry #
	  // #########################
	  if ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fXAsymmetryFit") != NULL)
	    {
	      xMeasured = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fXAsymmetryFit"))->Eval(asymmetry);
	      if (size == 2) THREADED(hXResidualCalculatedSize2_[planeID])->Fill(xMeasured - xPixelEdgeResidual);
	    }
	}
    }
}

//=======================================================================
void ResolutionAmericans::calculateYresiduals(bool pass, int planeID, const Data &data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;


  if (!pass || !data.getIsInDetector(planeID) || !data.getHasHit(planeID) || data.getClusterSize(planeID) > maxClusterSize) return;
  
  
  // ############################################################################################
  // # These residuals are computed only if thracks have cluster size 2 on each telescope plane #
  // ############################################################################################
//  int counter = 0;
//  for (int p = 8; p < 16; p++) if (data.getClusterSize(p) == 2) counter++;
//  if (counter != 8) return;
  
  
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
  float asymmetry   =  0;
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
  asymmetry   = (float)(chargeDown - chargeUp)/totalCharge;

  if (asymmetry >= -1. && asymmetry <= 1.)
    {
      toGet = "Charge/" + planeName +  "/YAsymmetry/h1DYcellChargeAsymmetryInv_" + planeName;
	
      if ((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))
	{
	  // #########################
	  // # Correct for asymmetry #
	  // #########################
	  if ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fYAsymmetryFit") != NULL)
	    {
	      yMeasured = ((TF1*)((TH1F*)theAnalysisManager_->getOutFile_()->Get(toGet.c_str()))->GetFunction("fYAsymmetryFit"))->Eval(asymmetry);
	      if (size == 2) THREADED(hYResidualCalculatedSize2_[planeID])->Fill(yMeasured - yPixelEdgeResidual);
	    }
	}
    }
}

//=======================================================================
void ResolutionAmericans::xResolution(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;

  if (thePlaneMapping_->getPlaneName(planeID) == "Dut0")
      numEventsBeforeAnyCuts[0]++;
  else if (thePlaneMapping_->getPlaneName(planeID) == "Dut1")
      numEventsBeforeAnyCuts[1]++;


  if (!pass || !data.getBelongsToTrack(planeID)) return;

  if (thePlaneMapping_->getPlaneName(planeID) == "Dut0")
      numEventsAfterFirstCuts[0]++;
  else if (thePlaneMapping_->getPlaneName(planeID) == "Dut1")
      numEventsAfterFirstCuts[1]++;
  
  
  // ############################################################################################
  // # These residuals are computed only if thracks have cluster size 2 on each telescope plane #
  // ############################################################################################
//  int counter = 0;
//  for (int p = 8; p < 16; p++) if (data.getClusterSize(p) == 2) counter++;
//  if (counter != 8) return;

  if (thePlaneMapping_->getPlaneName(planeID) == "Dut0")
      numEventsAfterTelescopeCuts[0]++;
  else if (thePlaneMapping_->getPlaneName(planeID) == "Dut1")
      numEventsAfterTelescopeCuts[1]++;


  const Window* theWindow = theWindowsManager_->getWindow(planeID);
  int           size      = data.getClusterSize(planeID);
  if (size > maxClusterSize) return;

//  // make sure all pixel hits are from small pixels in 25x100
//  for (int h = 0; h < size; h++) {
//      unsigned int temp_col = data.getClusterPixelCol(h, planeID);
//      unsigned int temp_row = data.getClusterPixelRow(h, planeID);
//      if (thePlaneMapping_->getPlaneName(planeID) == "Dut0") {
//          // need to convert from sensor (312 cols) to roc (52 cols) coords
//          thePitchTranslation25x100_->fromSensorToRocCoords(&temp_col, &temp_row);
//          if (!thePitchTranslation25x100_->isSmallPixel(temp_col, temp_row))
//            return;
//      }
//  }

  for (int h = 0; h < size; h++)
    if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber())) return;

  if (thePlaneMapping_->getPlaneName(planeID) == "Dut0")
      numEventsAfterCheckWindowCuts[0]++;
  else if (thePlaneMapping_->getPlaneName(planeID) == "Dut1")
      numEventsAfterCheckWindowCuts[1]++;

//  for (int h = 0; h < size; h++) {
//      if (size == 2 && thePlaneMapping_->getPlaneName(planeID) == "Dut1") {
//        if (!theWindow->checkWindowAbout50x50(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber(),
//                                     thePlaneMapping_->getPlaneType(planeID)))
//            return;
//      }
//      else
//        if (!theWindow->checkWindowAbout(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber(),
//                                     thePlaneMapping_->getPlaneType(planeID)))
//            return;
//  }

  if (thePlaneMapping_->getPlaneName(planeID) == "Dut0")
      numEventsAfterCheckWindowAboutCuts[0]++;
  else if (thePlaneMapping_->getPlaneName(planeID) == "Dut1")
      numEventsAfterCheckWindowAboutCuts[1]++;

  if (data.getClusterCharge(planeID) > 8000)
     THREADED(hXResiduals_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
//  if (abs(data.getXTrackResidualLocal(planeID))>38)
//      std::cout<<"WSIWSI\tTrackResidual: "<< data.getXTrackResidualLocal(planeID)<<" Cluster ColRow: ("
//              <<data.getClusterPixelCol(1,planeID)<<", "<<data.getClusterPixelRow(1,planeID)<<")"<<" Predicted ColRow: ("
//             <<data.getColPredicted(planeID)<<", "<<data.getRowPredicted(planeID)<<")"<<thePlaneMapping_->getPlaneName(planeID)<<std::endl;

//  if (theWindow->checkTimeWindowAbout(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber()))
//    {
      THREADED(h2DCorrelationsResidualXvsX_[planeID])->Fill(data.getXPredictedLocal(planeID),data.getXTrackResidualLocal(planeID));
      THREADED(h2DCorrelationsResidualXvsY_[planeID])->Fill(data.getYPredictedLocal(planeID),data.getXTrackResidualLocal(planeID));
//    }


  if (size == 1)
    {
      int col = data.getClusterPixelCol(0,planeID);
      int row = data.getClusterPixelRow(0,planeID);

      THREADED(h2DCorrelationsResidualXvsCharge_[planeID])->Fill(data.getXTrackResidualLocal(planeID), data.getClusterCharge(planeID));


      //if (thePlaneMapping_->getPlaneName(planeID) == "Dut0") {
//      if (data.getClusterPixelRow(0,planeID) != data.getClusterPixelRow(1,planeID)) return;

        // if (((col % 6) != 0 && ((col + 1) % 6) != 0))
          //  return;

      if (data.getClusterCharge(planeID) > 8000)
         THREADED(hXResidualsClusterSize1_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
      
         if (col % 2 == 0 && data.getClusterCharge(planeID) > 8000)
              THREADED(hXResidualsClusterSize1Left_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
         else if ((col+1) % 2 == 0 && data.getClusterCharge(planeID) > 8000)
             THREADED(hXResidualsClusterSize1Right_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
     // }
      //else if (thePlaneMapping_->getPlaneName(planeID) == "Dut1") {
        //  if (((col-9)%18==8 || (col-9)%18==9) && (row%12 == 5 || row%12 == 6)) {
         //      THREADED(hXResidualsClusterSize1_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
         // }
     // }

//      THREADED(hXResidualsClusterSize2_[planeID])->Fill(data.getXPixelResidualLocal(planeID));
//      if (/*data.getXTrackResidualLocal(planeID)<-40*/ thePlaneMapping_->getPlaneName(planeID) == "Dut0")
//          std::cout<<"WSIWSI\tTrackResidual ["<< data.getXTrackResidualLocal(planeID)
//                  <<"] cluster1 ["<<data.getClusterPixelCol(0,planeID)<<" "<<data.getClusterPixelRow(0,planeID)
//                  <<"] cluster2 ["<<data.getClusterPixelCol(1,planeID)<<" "<<data.getClusterPixelRow(1,planeID)
//                  <<"] Predicted ["<<data.getColPredicted(planeID)<<" "<<data.getRowPredicted(planeID)
//                  <<"] "<<thePlaneMapping_->getPlaneName(planeID)<<std::endl;
    }
    else if (size == 2) {
      if (data.getClusterPixelRow(0,planeID) != data.getClusterPixelRow(1,planeID)
              || data.getClusterCharge(planeID) < 8000) return;

      //if (thePlaneMapping_->getPlaneName(planeID) == "Dut0") {
       // if ((data.getClusterPixelCol(0,planeID) % 6) != 0 && ((data.getClusterPixelCol(0,planeID) + 1) % 6) != 0)
        //    return;
        // if ((data.getClusterPixelCol(1,planeID) % 6) != 0 && ((data.getClusterPixelCol(1,planeID) + 1) % 6) != 0)
          //    return;
     // }
      THREADED(hXResidualsClusterSize2_[planeID])->Fill(data.getXTrackResidualLocal(planeID));
  }
}

//=======================================================================
void ResolutionAmericans::yResolution(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 4;
  
  
  if (!pass || !data.getBelongsToTrack(planeID)) return;


  // ############################################################################################
  // # These residuals are computed only if thracks have cluster size 2 on each telescope plane #
  // ############################################################################################
//  int counter = 0;
//  for (int p = 8; p < 16; p++) if (data.getClusterSize(p) == 2) counter++;
//  if (counter != 8) return;


  const Window* theWindow = theWindowsManager_->getWindow(planeID);
  int           size      = data.getClusterSize(planeID);
  if (size > maxClusterSize) return;

//  // make sure all pixel hits are from small pixels in 25x100
//  for (int h = 0; h < size; h++) {
//      unsigned int temp_col = data.getClusterPixelCol(h, planeID);
//      unsigned int temp_row = data.getClusterPixelRow(h, planeID);
//      if (thePlaneMapping_->getPlaneName(planeID) == "Dut0") {
//          // need to convert from sensor (312 cols) to roc (52 cols) coords
//          thePitchTranslation25x100_->fromSensorToRocCoords(&temp_col, &temp_row);
//          if (!thePitchTranslation25x100_->isSmallPixel(temp_col, temp_row))
//            return;
//      }
//  }

  for (int h = 0; h < size; h++)
    if (!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber())) return;
  
//  for (int h = 0; h < size; h++) {
//      if (size == 2 && thePlaneMapping_->getPlaneName(planeID) == "Dut1") {
//        if (!theWindow->checkWindowAbout50x50(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber(),
//                                     thePlaneMapping_->getPlaneType(planeID)))
//            return;
//      }
//      else
//        if (!theWindow->checkWindowAbout(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber(),
//                                     thePlaneMapping_->getPlaneType(planeID)))
//            return;
//  }

  if (data.getClusterCharge(planeID) > 8000)
     THREADED(hYResiduals_[planeID])->Fill(data.getYTrackResidualLocal(planeID));

//  if (theWindow->checkTimeWindowAbout(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber()))
//    {
      THREADED(h2DCorrelationsResidualYvsX_[planeID])->Fill(data.getXPredictedLocal(planeID),data.getYTrackResidualLocal(planeID));
      THREADED(h2DCorrelationsResidualYvsY_[planeID])->Fill(data.getYPredictedLocal(planeID),data.getYTrackResidualLocal(planeID));
//    }

  if (size == 1) {
      int col = data.getClusterPixelCol(0,planeID);
      int row = data.getClusterPixelRow(0,planeID);
      //if (thePlaneMapping_->getPlaneName(planeID) == "Dut0") {

        // if (((col % 6) != 0 && ((col + 1) % 6) != 0))
          //  return;

      if (data.getClusterCharge(planeID) > 8000)
         THREADED(hYResidualsClusterSize1_[planeID])->Fill(data.getYTrackResidualLocal(planeID));

         if (col % 2 == 0 && data.getClusterCharge(planeID) > 8000)
              THREADED(hYResidualsClusterSize1Left_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
         else if ((col+1) % 2 == 0 && data.getClusterCharge(planeID) > 8000)
             THREADED(hYResidualsClusterSize1Right_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
      //}
      //else if (thePlaneMapping_->getPlaneName(planeID) == "Dut1") {
        //  if (((col-9)%18==8 || (col-9)%18==9) && (row%12 == 5 || row%12 == 6)) {
          //     THREADED(hYResidualsClusterSize1_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
          //}
      //}
  }
  
  else if (size == 2)
    {
      if (data.getClusterPixelCol(0,planeID) != data.getClusterPixelCol(1,planeID)
              || data.getClusterCharge(planeID) < 8000) return;
      
      THREADED(hYResidualsClusterSize2_[planeID])->Fill(data.getYTrackResidualLocal(planeID));
    }
}

//=======================================================================
void ResolutionAmericans::setCutsFormula(std::map<std::string,std::string> cutsList, std::vector<TTree*> tree)
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
bool ResolutionAmericans::passStandardCuts(int planeID, const Data &data)
{
  if (!theXmlParser_->getAnalysesFromString("Resolution")->applyStandardCuts()) return true;

  int minHits   = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str()) - 1;
  int excludeMe = 0;
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) minHits += 1;
  else if (data.getHasHit(planeID) && data.getClusterSize(planeID) <= 2) excludeMe = 1;

  if (data.getNumberOfTelescopeHits() - excludeMe >= minHits) return true;
  else                                                        return false;
}

//=======================================================================
void ResolutionAmericans::beginJob(void)
{
    std::cout << __PRETTY_FUNCTION__ << " IAM DOING THE AMNERICAN STUFF" << std::endl;

    theWindowsManager_ = theAnalysisManager_->getWindowsManager();
  book();

      numEventsBeforeAnyCuts[0] = 0;
      numEventsBeforeAnyCuts[1] = 0;
      numEventsAfterFirstCuts[0] = 0;
      numEventsAfterFirstCuts[1] = 0;
      numEventsAfterTelescopeCuts[0] = 0;
      numEventsAfterTelescopeCuts[1] = 0;
      numEventsAfterCheckWindowCuts[0] = 0;
      numEventsAfterCheckWindowCuts[1] = 0;
      numEventsAfterCheckWindowAboutCuts[0] = 0;
      numEventsAfterCheckWindowAboutCuts[1] = 0;
}

//=======================================================================
void ResolutionAmericans::analyze(const Data& data, int threadNumber)
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
                  std::cerr << "[ResolutionAmericans] Out of Range error: " << oor.what() << '\n';
                  kSmallPixel = false;
              }
          }
      }
      if (!kSmallPixel) continue;

      calculateXresiduals (xResolutionCut,p,data,threadNumber);
      calculateYresiduals (yResolutionCut,p,data,threadNumber);
      xResolution         (xResolutionCut,p,data,threadNumber);
      yResolution         (yResolutionCut,p,data,threadNumber);
      
      if (thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos)
        predictedErrors(errorsCut,p,data,threadNumber);
    }
}

//=======================================================================
void ResolutionAmericans::endJob(void)
{
  std::stringstream ss;

  std::cout << "Num events before any cuts: " << numEventsBeforeAnyCuts[0] << ", "
            << numEventsBeforeAnyCuts[1] << std::endl;
  std::cout << "Num events after first cuts: " << numEventsAfterFirstCuts[0] << ", "
            << numEventsAfterFirstCuts[1] << std::endl;
  std::cout << "Num events after telescope cuts: " << numEventsAfterTelescopeCuts[0] << ", "
            << numEventsAfterTelescopeCuts[1] << std::endl;
  std::cout << "Num events after check window cuts: " << numEventsAfterCheckWindowCuts[0] << ", "
            << numEventsAfterCheckWindowCuts[1] << std::endl;
  std::cout << "Num events after check window about cuts: " << numEventsAfterCheckWindowAboutCuts[0] << ", "
            << numEventsAfterCheckWindowAboutCuts[1] << std::endl;

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

      ADD_THREADED(hXResidualsClusterSize1_     [p]);
      ADD_THREADED(hYResidualsClusterSize1_     [p]);

      ADD_THREADED(hXResidualsClusterSize1Right_     [p]);
      ADD_THREADED(hXResidualsClusterSize1Left_     [p]);
      ADD_THREADED(hYResidualsClusterSize1Right_     [p]);
      ADD_THREADED(hYResidualsClusterSize1Left_     [p]);


      ADD_THREADED(h2DCorrelationsResidualXvsX_ [p]);
      ADD_THREADED(h2DCorrelationsResidualXvsY_ [p]);
      ADD_THREADED(h2DCorrelationsResidualYvsY_ [p]);
      ADD_THREADED(h2DCorrelationsResidualYvsX_ [p]);

      ADD_THREADED(hCorrelationsResidualXvsX_   [p]);
      ADD_THREADED(hCorrelationsResidualXvsY_   [p]);
      ADD_THREADED(hCorrelationsResidualYvsY_   [p]);
      ADD_THREADED(hCorrelationsResidualYvsX_   [p]);

      ADD_THREADED(h2DCorrelationsResidualXvsCharge_[p]);

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

      h2DCorrelationsResidualXvsCharge_ [p]->GetXaxis()->SetTitle("x residual (um)");
      h2DCorrelationsResidualXvsCharge_ [p]->GetYaxis()->SetTitle("Charge (num. electrons)");

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
void ResolutionAmericans::book(void)
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
      hXResiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hXResidualCalculatedSize2_"                                   + planeName;
      hTitle = "X residuals calculated from asymmetry fit (Clusters Size 2) " + planeName;
      hXResidualCalculatedSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));
      
      hName  = "hXResidualsClusterSize2_"                   + planeName;
      hTitle = "X residuals cluster size 2 "                + planeName;
      hXResidualsClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hXResidualsClusterSize1_"                   + planeName;
      hTitle = "X residuals cluster size 1 "                + planeName;
      hXResidualsClusterSize1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hXResidualsClusterSize1Right_"                   + planeName;
      hTitle = "X residuals cluster size 1 Right "                + planeName;
      hXResidualsClusterSize1Right_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hXResidualsClusterSize1Left_"                   + planeName;
      hTitle = "X residuals cluster size 1 Left "                + planeName;
      hXResidualsClusterSize1Left_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));
      
      
      theAnalysisManager_->cd("Resolution/" + planeName);
      theAnalysisManager_->mkdir("YResiduals");
      
      hName  = "hYResiduals_"                               + planeName;
      hTitle = "Y residuals "                               + planeName;
      hYResiduals_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hYResidualCalculatedSize2_"                                  + planeName;
      hTitle = "Y residuals calculated from asymmetry fit (Cluster Size 2) " + planeName;
      hYResidualCalculatedSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hYResidualsClusterSize2_"                   + planeName;
      hTitle = "Y residuals cluster size 2 "                + planeName;
      hYResidualsClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hYResidualsClusterSize1_"                   + planeName;
      hTitle = "Y residuals cluster size 1 "                + planeName;
      hYResidualsClusterSize1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hYResidualsClusterSize1Right_"                   + planeName;
      hTitle = "Y residuals cluster size 1 Right "                + planeName;
      hYResidualsClusterSize1Right_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));

      hName  = "hYResidualsClusterSize1Left_"                   + planeName;
      hTitle = "Y residuals cluster size 1 Left "                + planeName;
      hYResidualsClusterSize1Left_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 3*nBins, -halfWidth, halfWidth)));


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

        hName  = "h2DCorrelationsResidualXvsCharge_"                        + planeName;
        hTitle = "Correlation X residuals vs Charge"                        + planeName;
        h2DCorrelationsResidualXvsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(), nBins, -halfWidth, halfWidth, 1000, 0, 20000)));

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
