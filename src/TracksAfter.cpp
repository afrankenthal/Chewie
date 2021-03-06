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

#include "TracksAfter.h"

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

#include <TH1F.h>
#include <TH2F.h>
#include <TThread.h>
#include <TFile.h>
#include <TTreeFormula.h>
#include <TMath.h>
#include <TF1.h>
#include <Rtypes.h>
#include <TROOT.h>

#include <iostream>
#include <cmath>


TracksAfter::TracksAfter(AnalysisManager* analysisManager, int nOfThreads) :
  Analysis(analysisManager, nOfThreads), thePlaneMapping_(0)
{
  thePlaneMapping_ = new PlanesMapping();
  inFile_ = 0;
}

TracksAfter::~TracksAfter(void)
{
  if(thePlaneMapping_)
    {
      delete thePlaneMapping_;
      thePlaneMapping_ = 0;
    }
  
  destroy();
}

void TracksAfter::destroy(void)
{
  if(Analysis::fDoNotDelete_) return;
}

bool TracksAfter::passCalibrationsCut(int planeID, const Data &/*data*/)
{
  XmlParser* theParser = theAnalysisManager_->getXmlParser();
  
  if(!(theParser->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->useCalibrations())
    return true;
  
  bool pass = true;
  return pass;
}

void TracksAfter::getInFile (TFile * infile)
{
  if (inFile_) delete inFile_;
  inFile_ = infile;
}

bool TracksAfter::passStandardCuts(int planeID, const Data &data)
{
  XmlParser* theXmlParser_ = theAnalysisManager_->getXmlParser();
  if (!theXmlParser_->getAnalysesFromString("Charge")->applyStandardCuts()) return true;
  
  int minHits   = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str()) - 1;
  int excludeMe = 0;
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) minHits += 1;
  else if (data.getHasHit(planeID) && data.getClusterSize(planeID) <= 2) excludeMe = 1;
  
  if (data.getNumberOfTelescopeClustersSizeLE2() - excludeMe >= minHits) return true;
  else                                                                   return false;
}

void TracksAfter::beginJob(void)
{
  book();
}

void TracksAfter::positionUnfolded (int planeID)
{
    float pn1x, pc1x, pn2x, pc2x;
    float pn1y, pc1y, pn2y, pc2y;
    float binSize = 5;
    float pixelLengthX = 150;
    float pixelLengthY = 100;

    if (!hXdistribution1D_[planeID]) std::cout << "Didn't get the Histo hXdistribution1D " << planeID << "!!!\n";
    if (!hXdistribution2D_[planeID]) std::cout << "Didn't get the Histo hXdistribution2D " << planeID << "!!!\n";
    if (!hXChargedistribution1D_[planeID]) std::cout << "Didn't get the Histo hXChargedistribution1D " << planeID << "!!!\n";
    if (!hXChargedistribution1D_[planeID]) std::cout << "Didn't get the Histo hXChargedistribution2D " << planeID << "!!!\n";
    if (!hYdistribution1D_[planeID]) std::cout << "Didn't get the Histo hYdistribution1D " << planeID << "!!!\n";
    if (!hYdistribution2D_[planeID]) std::cout << "Didn't get the Histo hYdistribution2D " << planeID << "!!!\n";
    if (!hYChargedistribution1D_[planeID]) std::cout << "Didn't get the Histo hYChargedistribution1D " << planeID << "!!!\n";
    if (!hYChargedistribution2D_[planeID]) std::cout << "Didn't get the Histo hYChargedistribution2D " << planeID << "!!!\n";

    for (float x = -pixelLengthX/2 + binSize/2; x < pixelLengthX/2 + binSize/2; x += binSize) {
        for (float c = 8; c < 80008; c += 16) {
            pn1x = hXdistribution1D_[planeID]->GetBinContent(hXdistribution1D_[planeID]->FindBin(x));
            pn2x = hXdistribution2D_[planeID]->GetBinContent(hXdistribution2D_[planeID]->FindBin(x));
            pc1x = hXChargedistribution1D_[planeID]->GetBinContent(hXChargedistribution1D_[planeID]->FindBin(x, c));
            pc2x = hXChargedistribution2D_[planeID]->GetBinContent(hXChargedistribution2D_[planeID]->FindBin(x, c));
            XProbabilityFunc_[planeID]->Fill(x, c, pn1x*pc1x + pn2x*pc2x);
        }
    }

    for (float y = -pixelLengthY/2 + binSize/2; y < pixelLengthY/2 + binSize/2; y += binSize) {
        for (float c = 8; c < 80008; c += 16) {
            pn1y = hYdistribution1D_[planeID]->GetBinContent(hYdistribution1D_[planeID]->FindBin(y));
            pn2y = hYdistribution2D_[planeID]->GetBinContent(hYdistribution2D_[planeID]->FindBin(y));
            pc1y = hYChargedistribution1D_[planeID]->GetBinContent(hYChargedistribution1D_[planeID]->FindBin(y, c));
            pc2y = hYChargedistribution2D_[planeID]->GetBinContent(hYChargedistribution2D_[planeID]->FindBin(y, c));
            YProbabilityFunc_[planeID]->Fill(y, c, pn1y*pc1y + pn2y*pc2y);
        }
    }

}

float TracksAfter::findMaxHisto2DX (int planeID, float charge)
{
  float binSize = 5;
  float pixelLengthX = 150;
  
  float x = -pixelLengthX/2 + binSize/2;
  float xmax = x;
  float val = XProbabilityFunc_[planeID]->GetBinContent(hXChargedistribution1D_[planeID]->FindBin(x, charge));
  float valmax = val;
  for (x += binSize; x < pixelLengthX/2 + binSize/2; x += binSize) {
    val = XProbabilityFunc_[planeID]->GetBinContent(hXChargedistribution1D_[planeID]->FindBin(x, charge));
    if (val > valmax) {
      valmax = val;
      xmax = x;
    }
  }
  return xmax;
}

float TracksAfter::findMaxHisto2DY (int planeID, float charge)
{
    float binSize = 5;
    float pixelLengthY = 100;

    float x = -pixelLengthY/2 + binSize/2;
    float xmax = x;
    float val = YProbabilityFunc_[planeID]->GetBinContent(hYChargedistribution1D_[planeID]->FindBin(x, charge));
    float valmax = val;
    for (x += binSize; x < pixelLengthY/2 + binSize/2; x += binSize) {
        val = YProbabilityFunc_[planeID]->GetBinContent(hYChargedistribution1D_[planeID]->FindBin(x, charge));
        if (val > valmax) {
            valmax = val;
            xmax = x;
        }
    }
    return xmax;
}

void TracksAfter::unfold (int planeID, float charge, float xp, float yp, int threadNumber)
{
  float xr = findMaxHisto2DX (planeID, charge);
  float yr = findMaxHisto2DY (planeID, charge);
  
  THREADED(XYReconstructed_[planeID])->Fill(xr, yr);
  THREADED(XResidualsReconstructed_[planeID])->Fill(xp - xr);
  THREADED(YResidualsReconstructed_[planeID])->Fill(yp - yr);
}

void TracksAfter::fitEtaFunc (int planeID, std::string type)
{
    std::stringstream ss;

    XAsimmetryFunc_[planeID]->SetLineColor(kRed);
    XAsimmetryFuncInv_[planeID]->SetLineColor(kBlue);

    XAsimmetryFunc_[planeID]->SetParName(0, "Thickness                        ");
    XAsimmetryFunc_[planeID]->SetParName(1, "2*mu*DepletionVoltage            ");
    XAsimmetryFunc_[planeID]->SetParName(2, "DepletionVoltage                 ");
    XAsimmetryFunc_[planeID]->SetParName(3, "AppliedVoltage + DepletionVoltage");
    XAsimmetryFunc_[planeID]->SetParName(4, "4*DiffusionConstant              ");
    XAsimmetryFunc_[planeID]->SetParName(5, "ScaleFactor                      ");

    XAsimmetryFuncInv_[planeID]->SetParName(0, "Intercept");
    XAsimmetryFuncInv_[planeID]->SetParName(1, "Slope    ");

    if (type == "Size2")
      {
        XAsimmetryFunc_[planeID]->SetRange(-20., 20.);
        XAsimmetryFunc_[planeID]->FixParameter(5, 1. );
        XAsimmetryFunc_[planeID]->FixParameter(0, 500);
        XAsimmetryFunc_[planeID]->FixParameter(1, 0.02);
        XAsimmetryFunc_[planeID]->FixParameter(2, 5.);
        XAsimmetryFunc_[planeID]->FixParameter(3, 505.);
        XAsimmetryFunc_[planeID]->FixParameter(4, 0.09);	
      }    
    else if (type == "SizeLE2")
      {
        XAsimmetryFunc_[planeID]->FixParameter(5, 1. );
        XAsimmetryFunc_[planeID]->FixParameter(0, 500);
        XAsimmetryFunc_[planeID]->FixParameter(1, 1.5);
        XAsimmetryFunc_[planeID]->FixParameter(2, 5.);
        XAsimmetryFunc_[planeID]->FixParameter(3, 505.);
        XAsimmetryFunc_[planeID]->FixParameter(4, 0.07);
      }    
    else if (type == "SizeLE2Inv")
      {
        XAsimmetryFuncInv_[planeID]->SetLineColor(kBlue); //No need to worry about fixing/setting parameters, fitting a rect should be easy... It's just a check
      }    
    else {
      STDLINE("Type of size constraint not recognized!", ACRed);
      return;
    }
    
    STDLINE("Fitting eta function in coordinate x for plane " + thePlaneMapping_->getPlaneName(planeID) + ", size constraint: " + type, ACGreen);
    
    if (type == "Size2")
      {
        if (planeID == 9)
	  hXAsimmetry_[planeID]->Fit(XAsimmetryFunc_[planeID], ""); // fitting method "QRBL"?
      }
    else if (type == "SizeLE2")
      {
        if (planeID == 9)
	  hXAsimmetryLE2_[planeID]->Fit(XAsimmetryFunc_[planeID], "L"); // fitting method "QRBL"?
      }
    else if (type == "SizeLE2Inv")
      {
        if (planeID == 9)
	  hXAsimmetryInv_[planeID]->Fit(XAsimmetryFuncInv_[planeID], "", "", -0.7, 0.7); // fitting method "QRBL"?
      }
    
    ss.str("");
    ss << XAsimmetryFunc_[planeID]->GetParameter(0) << " +/- " << XAsimmetryFunc_[planeID]->GetParError(0);
    STDLINE("Thickness (um):                         " + ss.str(), ACYellow);
    ss.str("");
    ss << XAsimmetryFunc_[planeID]->GetParameter(1) << " +/- " << XAsimmetryFunc_[planeID]->GetParError(1);
    STDLINE("2 * mobility * depletion voltage (1/s): " + ss.str(), ACYellow);
    ss.str("");
    ss << XAsimmetryFunc_[planeID]->GetParameter(2) << " +/- " << XAsimmetryFunc_[planeID]->GetParError(2);
    STDLINE("Depletion voltage (V):                  " + ss.str(), ACYellow);
    ss.str("");
    ss << XAsimmetryFunc_[planeID]->GetParameter(3) << " +/- " << XAsimmetryFunc_[planeID]->GetParError(3);
    STDLINE("Applied + depletion voltage (V):        " + ss.str(), ACYellow);
    ss.str("");
    ss << XAsimmetryFunc_[planeID]->GetParameter(4) << " +/- " << XAsimmetryFunc_[planeID]->GetParError(4);
    STDLINE("4 * Diffusion Constant (um*um/s):       " + ss.str(), ACYellow);

    YAsimmetryFunc_[planeID]->SetLineColor(kRed);
    YAsimmetryFuncInv_[planeID]->SetLineColor(kBlue);

    YAsimmetryFunc_[planeID]->SetParName(0, "Thickness                        ");
    YAsimmetryFunc_[planeID]->SetParName(1, "2*mu*DepletionVoltage            ");
    YAsimmetryFunc_[planeID]->SetParName(2, "DepletionVoltage                 ");
    YAsimmetryFunc_[planeID]->SetParName(3, "AppliedVoltage + DepletionVoltage");
    YAsimmetryFunc_[planeID]->SetParName(4, "4*DiffusionConstant              ");
    YAsimmetryFunc_[planeID]->SetParName(5, "ScaleFactor                      ");

    YAsimmetryFuncInv_[planeID]->SetParName(0, "Intercept");

    if (type == "Size2")
    {
        YAsimmetryFunc_[planeID]->FixParameter(5, 1.);
        YAsimmetryFunc_[planeID]->FixParameter(0, 500);
        YAsimmetryFunc_[planeID]->FixParameter(1, 0.02);
        YAsimmetryFunc_[planeID]->FixParameter(2, 5.);
        YAsimmetryFunc_[planeID]->FixParameter(3, 505.);
        YAsimmetryFunc_[planeID]->FixParameter(4, 0.09);	
    }
    else if (type == "SizeLE2")
      {
        YAsimmetryFunc_[planeID]->FixParameter(5, 1.);
        YAsimmetryFunc_[planeID]->FixParameter(0, 500);
        YAsimmetryFunc_[planeID]->FixParameter(1, 1.5);
        YAsimmetryFunc_[planeID]->FixParameter(2, 5.);
        YAsimmetryFunc_[planeID]->FixParameter(3, 505.);
        YAsimmetryFunc_[planeID]->FixParameter(4, 0.07);
      }    
    else if (type == "SizeLE2Inv")
      {
        XAsimmetryFuncInv_[planeID]->SetLineColor(kBlue); //No need to worry about fixing/setting parameters, fitting a rect should be easy... It's just a check
      }    
    else {
      STDLINE("Type of size constraint not recognized!", ACRed);
      return;
    }
    
    STDLINE("Fitting eta function in coordinate y for plane " + thePlaneMapping_->getPlaneName(planeID) + ", size constraint: " + type, ACGreen);
    
    if (type == "Size2")
      {
        if (planeID == 9)
	  hYAsimmetry_[planeID]->Fit(YAsimmetryFunc_[planeID], ""); // fitting method "QRBL"?
      }
    else if (type == "SizeLE2")
      {
        if (planeID == 9)
	  hYAsimmetryLE2_[planeID]->Fit(YAsimmetryFunc_[planeID], "L"); // fitting method "QRBL"?
      }
    else if (type == "SizeLE2Inv")
      {
        if (planeID == 9)
	  hYAsimmetryInv_[planeID]->Fit(YAsimmetryFuncInv_[planeID], "", "", -0.6, 0.6); // fitting method "QRBL"?
      }
    
    ss.str("");
    ss << YAsimmetryFunc_[planeID]->GetParameter(0) << " +/- " << YAsimmetryFunc_[planeID]->GetParError(0);
    STDLINE("Thickness (um):                         " + ss.str(), ACYellow);
    ss.str("");
    ss << YAsimmetryFunc_[planeID]->GetParameter(1) << " +/- " << YAsimmetryFunc_[planeID]->GetParError(1);
    STDLINE("2 * mobility * depletion voltage (1/s): " + ss.str(), ACYellow);
    ss.str("");
    ss << YAsimmetryFunc_[planeID]->GetParameter(2) << " +/- " << YAsimmetryFunc_[planeID]->GetParError(2);
    STDLINE("Depletion voltage (V):                  " + ss.str(), ACYellow);
    ss.str("");
    ss << YAsimmetryFunc_[planeID]->GetParameter(3) << " +/- " << YAsimmetryFunc_[planeID]->GetParError(3);
    STDLINE("Applied + depletion voltage (V):        " + ss.str(), ACYellow);
    ss.str("");
    ss << YAsimmetryFunc_[planeID]->GetParameter(4) << " +/- " << YAsimmetryFunc_[planeID]->GetParError(4);
    STDLINE("4 * Diffusion Constant (um*um/s):       " + ss.str(), ACYellow);

}

///////////JUST A TEST TO CHECK IF THE ETA FUNCTION WORKS/////////////////////////////////////////////////////////////

void TracksAfter::testPredictedFunc (void)
{
  double pixelLengthY = 100;
  double * xt = new double;
  int n = 0;
  double parameters[6] = {500.,0.5, 70., 570., 0.002, 1.};
  double parameters2[5] = {4.75, 9.81, 1312., 0., 98.};
  double parameters3[9] = {500.,6.5, 0.07, 1., 4.76, 9.28, 1312, 0, 98};
  double sum = 0;
  for (float x = -pixelLengthY/2; x < pixelLengthY/2; x += 1) {
    *xt = x;
    funcPredicted_->SetBinContent(n, Utilities::uniformCenterSmearedPlusConstantNorm(xt, parameters2));
    sum += Utilities::uniformCenterSmearedPlusConstantNorm(xt, parameters2)*1;
    funcPredicted2_->SetBinContent(n, Utilities::etaSmeared(xt, parameters3));
    funcPredicted3_->SetBinContent(n, Utilities::etaInverseFitFunc(xt, parameters));
    ++n;
  }
  std::stringstream ss;
  ss << "The area of the normalized function is " << sum;
  STDLINE(ss.str(), ACYellow);
}

void TracksAfter::loadEtaFunc (std::string type)
{
  float x, y;
  
  double fitParameters[6];
  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); ++p)
    {
      STDLINE("Loading eta functions for plane " + thePlaneMapping_->getPlaneName(p), ACGreen);
      
      if (p != 9) continue;
      if (p != 10) continue;//to avoid any loading when not required
      
      if (type == "Size2")
        {
	  XAsimmetryFunc_[p]->GetParameters(fitParameters);
	  for (int n = 0; n < computedEtaFuncXSize2_[p]->GetNbinsX()+1; ++n)
            {
	      x = computedEtaFuncXSize2_[p]->GetBinCenter(n);
	      computedEtaFuncXSize2_[p]->Fill(x, XAsimmetryFunc_[p]->Eval(x));
            }
	  
	  YAsimmetryFunc_[p]->GetParameters(fitParameters);
	  for (int m = 0; m < computedEtaFuncYSize2_[p]->GetNbinsX()+1; ++m)
            {
	      y = computedEtaFuncYSize2_[p]->GetBinCenter(m);
	      computedEtaFuncYSize2_[p]->Fill(y, YAsimmetryFunc_[p]->Eval(y));
            }
        }
      
      else if (type == "SizeLE2")
        {
	  XAsimmetryFunc_[p]->GetParameters(fitParameters);
	  for (int n = 0; n < computedEtaFuncXSizeLE2_[p]->GetNbinsX()+1; ++n)
            {
	      x = computedEtaFuncXSizeLE2_[p]->GetBinCenter(n);
	      computedEtaFuncXSizeLE2_[p]->Fill(x, XAsimmetryFunc_[p]->Eval(x));
            }
	  
	  YAsimmetryFunc_[p]->GetParameters(fitParameters);
	  for (int m = 0; m < computedEtaFuncYSizeLE2_[p]->GetNbinsX()+1; ++m)
            {
	      y = computedEtaFuncYSizeLE2_[p]->GetBinCenter(m);
	      computedEtaFuncYSizeLE2_[p]->Fill(y, YAsimmetryFunc_[p]->Eval(y));
            }
        }
      
      else {
	STDLINE("Type of size constraint not recognized!", ACRed);
	return;
      }
      
    }
  
}

double TracksAfter::invertInHisto (int planeID, double etaValue, std::string coordinate, std::string type)
{
  float eta;
  float pos;
  double posFound = 1000;
  double etaFound = 1000;
  std::stringstream ss;
  
  if (planeID != 9) return 55;
  if (planeID != 10) return 55; //to avoid inverting when histo is not defined (see similar statement in loadEtaFunc
  
  if (coordinate == "x")
    {
      if (type == "Size2")
        {
	  for (int i = 0; i < computedEtaFuncXSize2_[planeID]->GetNbinsX(); ++i)
            {
	      eta = computedEtaFuncXSize2_[planeID]->GetBinContent(i);
	      pos = computedEtaFuncXSize2_[planeID]->GetBinCenter(i);
	      if (fabs(eta-etaValue) < fabs(etaFound-etaValue)) {
		etaFound = eta;
		posFound = pos;
                }
            }
	  if (posFound != 1000) return posFound;
	  else {
	    ss << etaValue;
	    STDLINE("Couldn't invert eta funtion (x) for eta value " + ss.str() + " in plane " + thePlaneMapping_->getPlaneName(planeID), ACRed);
	    return 1000;
	  }
        }
      
      else if (type == "SizeLE2")
        {
	  for (int i = 0; i < computedEtaFuncXSizeLE2_[planeID]->GetNbinsX(); ++i)
            {
	      eta = computedEtaFuncXSizeLE2_[planeID]->GetBinContent(i);
	      pos = computedEtaFuncXSizeLE2_[planeID]->GetBinCenter(i);
	      if (fabs(eta-etaValue) < fabs(etaFound-etaValue)) {
		etaFound = eta;
		posFound = pos;
	      }
            }
	  if (posFound != 1000) return posFound;
	  else {
	    ss << etaValue;
	    STDLINE("Couldn't invert eta funtion (x) for eta value " + ss.str() + " in plane " + thePlaneMapping_->getPlaneName(planeID), ACRed);
	    return 1000;
	  }
        }      
      else {
	STDLINE("Type of size constraint not recognized!", ACRed);
	return 1000;
      }
      
    }
  else if (coordinate == "y")
    {
      if (type == "Size2")
        {
	  for (int i = 0; i < computedEtaFuncYSize2_[planeID]->GetEntries(); ++i)
            {
	      eta = computedEtaFuncYSize2_[planeID]->GetBinContent(i);
	      pos = computedEtaFuncYSize2_[planeID]->GetBinCenter(i);
	      if (fabs(eta-etaValue) < fabs(etaFound - etaValue)) {
		etaFound = eta;
		posFound = pos;
	      }
            }
	  if (posFound != 1000) return posFound;
	  else {
	    ss << etaValue;
	    STDLINE("Couldn't invert eta funtion (y) for eta value " + ss.str() + " in plane " + thePlaneMapping_->getPlaneName(planeID), ACRed);
	    return 1000;
	  }
        }
      
      else if (type == "SizeLE2")
        {
	  for (int i = 0; i < computedEtaFuncYSizeLE2_[planeID]->GetEntries(); ++i)
            {
	      eta = computedEtaFuncYSizeLE2_[planeID]->GetBinContent(i);
	      pos = computedEtaFuncYSizeLE2_[planeID]->GetBinCenter(i);
	      if (fabs(eta-etaValue) < fabs(etaFound - etaValue)) {
		etaFound = eta;
		posFound = pos;
	      }
            }
	  if (posFound != 1000) return posFound;
	  else {
	    ss << etaValue;
	    STDLINE("Couldn't invert eta funtion (y) for eta value " + ss.str() + " in plane " + thePlaneMapping_->getPlaneName(planeID), ACRed);
	    return 1000;
	  }
        }      
      else {
	STDLINE("Type of size constraint not recognized!", ACRed);
	return 1000;
      }
      
    }
  else
    {
      STDLINE("Coordinate " + coordinate + " not recognized!", ACRed);
      return 0;
    }
}

double TracksAfter::invertInHistoSingle (double etaValue, TH1F * histo)
{
  float eta;
  float pos;
  double posFound = 1000;
  double etaFound = 1000;
  std::stringstream ss;
  
  for (int i = 0; i < histo->GetNbinsX(); ++i)
    {
      eta = histo->GetBinContent(i);
      pos = histo->GetBinCenter(i);
      if (fabs(eta-etaValue) < fabs(etaFound-etaValue)) {
	etaFound = eta;
	posFound = pos;
      }
    }
  if (posFound != 1000) return posFound;
  else {
    ss << histo->GetName() << " for eta value " << etaValue;
    STDLINE("Couldn't invert eta funtion in graph " +  ss.str(), ACRed);
    return 1000;
  }
}

double TracksAfter::calculateInHisto (double value, TH1F *histo)
{
  int bin = histo->FindBin(value);
  return histo->GetBinContent(bin);
}

void TracksAfter::calculateXResiduals (const Data &data, int planeID, int threadNumber)
{
  if( !data.getIsInDetector( planeID ) || !data.getHasHit( planeID ))
    return;
  if (data.getClusterCharge(planeID) > 20000)
    return;
  
  const Window* theWindow = theAnalysisManager_->getWindowsManager()->getWindow (planeID);
  int           row       = data.getRowPredicted                                (planeID);
  int           col       = data.getColPredicted                                (planeID);
  int           run       = data.getRunNumber                                   ()       ;
  
  int   hitID             = -1;
  int   totalCharge       = 0 ;
  int   chargeLeft        = 0 ;
  int   chargeRight       = 0 ;
  float Asimmetry         = 0 ;
  
  if( !theWindow->checkWindow(col,row,run) ) {
    return;
  }
  
  if (data.getClusterSize(planeID) == 2)
    {
      for(int h=0; h<2; ++h)
        {
	  if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) //hits are in the window
		 || !data.getIsPixelCalibrated(h,planeID)                                                          //pixels are calibrated
		 ||  data.getClusterPixelRow  (h,planeID) != row )                                                 //hits are on the same row (sharing is along the row - x direction)
	    return;
        }
      
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
	  if( h == hitID )
	    continue;
	  if(data.getXPixelResidualLocal(planeID) > 0 && (col - data.getClusterPixelCol(h,planeID)) == -1)//il secondo hit e' a DX della predetta
            {
                chargeRight  = data.getClusterPixelCharge(h    ,planeID);
                chargeLeft = data.getClusterPixelCharge(hitID,planeID);
                break;
            }
	  else if(data.getXPixelResidualLocal(planeID) <= 0 && (col - data.getClusterPixelCol(h,planeID)) == +1)//il secondo hit e' a SX della predetta
            {
	      chargeRight = data.getClusterPixelCharge(hitID,planeID);
	      chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
	      break;
            }
	  else if(data.getXPixelResidualLocal(planeID) > 0 && (col - data.getClusterPixelCol(h,planeID)) == +1)
            {
	      chargeRight = data.getClusterPixelCharge(hitID,planeID);
	      chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
	      break;
            }
	  else if(data.getXPixelResidualLocal(planeID) <= 0 && (col - data.getClusterPixelCol(h,planeID)) ==  -1)
            {
	      chargeRight  = data.getClusterPixelCharge(h    ,planeID);
	      chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
	      break;
            }
	  else
                return;
        }
      
      float Xp = 0;
      
      if(data.getXPixelResidualLocal( planeID ) > 0)
	Xp = -data.getXPitchLocal( planeID )/2 + data.getXPixelResidualLocal( planeID );
      else if(data.getXPixelResidualLocal( planeID ) <= 0)
	Xp = (data.getXPixelResidualLocal( planeID ) + data.getXPitchLocal( planeID )/2);
      
      totalCharge = chargeLeft + chargeRight;
      Asimmetry   = (float)(chargeLeft - chargeRight)/totalCharge;
      if(Asimmetry >= -0.7 && Asimmetry <= 0.7 && totalCharge <= 30000) {
	//            float Xm = invertInHisto(planeID, Asimmetry, "x", "Size2");
	float XmLE2 = invertInHisto(planeID, Asimmetry, "x", "SizeLE2");
	float Xm = XAsimmetryFuncInv_[planeID]->GetParameter(0) + Asimmetry*XAsimmetryFuncInv_[planeID]->GetParameter(1);
	//            float Xm = (-XAsimmetryFuncRect_[planeID]->GetParameter(0) + Asimmetry)/XAsimmetryFuncRect_[planeID]->GetParameter(1);
	//            float Xp = data.getXPixelResidualLocal(planeID);
	THREADED(hXResidualsAsimmetry_[planeID])->Fill(Xm - Xp);
	THREADED(hXResidualsAsimmetryLE2_[planeID])->Fill(XmLE2 - Xp);
      }
    }
  
  else if (data.getClusterSize(planeID) == 1)
    {
      float Xm = (data.getColPredicted(planeID) + 0.5)*data.getXPitchLocal(planeID);
      //        float Xp = data.getXPixelResidualLocal(planeID);
      float Xp = 0;
      if(data.getXPixelResidualLocal( planeID ) > 0)
	Xp = -data.getXPitchLocal( planeID )/2 + data.getXPixelResidualLocal( planeID );
      else if(data.getXPixelResidualLocal( planeID ) <= 0)
	Xp = (data.getXPixelResidualLocal( planeID ) + data.getXPitchLocal( planeID )/2);
      
      THREADED(hXResidualsAsimmetryLE2_[planeID])->Fill(Xm - Xp);
    }
  
  else return;
}

void TracksAfter::calculateYResiduals (const Data &data, int planeID, int threadNumber)
{
  if( !data.getIsInDetector( planeID ) || !data.getHasHit( planeID ) || data.getClusterSize( planeID ) != 2 )
    return;
  if (data.getClusterCharge(planeID) > 20000)
    return;
  
  const Window* theWindow = theAnalysisManager_->getWindowsManager()->getWindow(planeID);
  int           row       = data.getRowPredicted                               (planeID);
  int           col       = data.getColPredicted                               (planeID);
  int           run       = data.getRunNumber                                  ()       ;
  
  
  if( !theWindow->checkWindow(col,row,run) ) {
    return;
  }
  
  for(int h=0; h<2; ++h)
    {
      if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) //hits are in the window
	     || !data.getIsPixelCalibrated(h,planeID)                                                          //pixels are calibrated
	     ||  data.getClusterPixelCol  (h,planeID) != col )                                                 //hits are on the same col (sharing is along the row - x direction)
	return;
    }
  
  int   hitID       =     -1 ;
  int   totalCharge =      0 ;
  int   chargeLeft  =      0 ;
  int   chargeRight =      0 ;
  float Asimmetry   =      0 ;
  
  if (data.getClusterSize(planeID) == 2)
    {
      for(int h=0; h<2; ++h)
        {
	  if(data.getClusterPixelRow(h,planeID) == row)//mi assicuro che ci sia la cella predetta in uno dei due hit
            {
	      hitID   = h   ;
	      break;
            }
        }
      if( hitID == -1 )
	return;
      
      for(int h=0; h<2; ++h)
        {
	  if( h == hitID )
	    continue;
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
	  else if(data.getYPixelResidualLocal(planeID) > 0 && (row - data.getClusterPixelRow(h,planeID)) == 1)
            {
	      chargeRight       = data.getClusterPixelCharge(hitID,planeID);
	      chargeLeft        = data.getClusterPixelCharge(h    ,planeID);
	      break;
            }
	  else if(data.getYPixelResidualLocal(planeID) < 0 && (row - data.getClusterPixelRow(h,planeID)) ==  -1)
            {
	      chargeRight       = data.getClusterPixelCharge(h    ,planeID);
	      chargeLeft        = data.getClusterPixelCharge(hitID,planeID);
	      break;
            }
	  else
	    return;
        }

        float Xp = 0;

        if(data.getYPixelResidualLocal( planeID ) > 0)
            Xp = -data.getYPitchLocal( planeID )/2 + data.getYPixelResidualLocal( planeID );
        else if(data.getYPixelResidualLocal( planeID ) <= 0)
            Xp = (data.getYPixelResidualLocal( planeID ) + data.getYPitchLocal( planeID )/2);

        totalCharge = chargeLeft + chargeRight;
        Asimmetry   = (float)(chargeLeft - chargeRight)/totalCharge;
        if(Asimmetry >= -0.7 && Asimmetry <= 0.7 && totalCharge <= 30000) {
            float XmLE2 = invertInHisto(planeID, Asimmetry, "y", "SizeLE2");
            float Xm = YAsimmetryFuncInv_[planeID]->GetParameter(0) + Asimmetry*YAsimmetryFuncInv_[planeID]->GetParameter(1);
            THREADED(hYResidualsAsimmetry_[planeID])->Fill(Xm - Xp);
            THREADED(hYResidualsAsimmetryLE2_[planeID])->Fill(XmLE2 - Xp);
        }
    }

    else if (data.getClusterSize(planeID) == 1)
    {
        float Xm = (data.getRowPredicted(planeID) + 0.5)*data.getYPitchLocal(planeID);
        float Xp = 0;
        if(data.getYPixelResidualLocal( planeID ) > 0)
            Xp = -data.getYPitchLocal( planeID )/2 + data.getYPixelResidualLocal( planeID );
        else if(data.getYPixelResidualLocal( planeID ) <= 0)
            Xp = (data.getYPixelResidualLocal( planeID ) + data.getYPitchLocal( planeID )/2);

        THREADED(hYResidualsAsimmetryLE2_[planeID])->Fill(Xm - Xp);
    }

    else return;
}

void TracksAfter::profileChi2Par (double *par, int parNumber, double liminf, double limsup)
{
    std::stringstream ss;
    ss << "chi2ProfilesPar_" << parNumber;
    if ((int)chi2ProfilesPar_.size() == parNumber) chi2ProfilesPar_.push_back(new TH1F(ss.str().c_str(), ss.str().c_str(), 200, liminf, limsup));
    double parTemp = par[parNumber];
    par[parNumber] = liminf;
    double step = (limsup-liminf)/200;
    std::cout << hXAsimmetryLE2_[9]->GetName() << "!!!\n";
    for (int i = 1; i < chi2ProfilesPar_[parNumber]->GetXaxis()->GetNbins(); ++i)
    {
        chi2ProfilesPar_[parNumber]->SetBinContent(i, Utilities::chi2Eta(par, hXAsimmetryLE2_[9])); std::cout << i << ", " << par[parNumber] << ": " << Utilities::chi2Eta(par, hXAsimmetryLE2_[9]) << " !!!\n";
        par[parNumber] += step;
    }
    par[parNumber] = parTemp;
}

void TracksAfter::computeEtaDerivative (int planeID)
{
    double etaD, eta, pos;

    for (int i = 0; i < hXEtaDistribution_[planeID]->GetXaxis()->GetNbins()+1; ++i)
    {
        if (hXEtaDistribution_[planeID]->GetBinContent(i) > 0) etaD = 1/hXEtaDistribution_[planeID]->GetBinContent(i);
        else etaD = 0;
        eta = hXEtaDistribution_[planeID]->GetBinCenter(i);
        pos = invertInHisto(planeID, eta, "x", "SizeLE2");
        computedEtaDerivativeXFitted_[planeID]->Fill(pos, etaD);
        pos = invertInHistoSingle(eta, hXAsimmetryLE2_[planeID]);
        computedEtaDerivativeXData_[planeID]->Fill(pos, etaD);
        pos = invertInHisto(planeID, eta, "x", "Size2");
        computedEtaDerivativeXFittedSize2_[planeID]->Fill(pos, etaD);
        pos = invertInHistoSingle(eta, hXAsimmetry_[planeID]);
        computedEtaDerivativeXDataSize2_[planeID]->Fill(pos, etaD);
        pos = XAsimmetryFuncInv_[planeID]->Eval(eta);
        computedEtaDerivativeXFittedSize2Inv_[planeID]->Fill(pos, etaD);
        pos = calculateInHisto(eta, hXAsimmetryInv_[planeID]);
        computedEtaDerivativeXDataSize2Inv_[planeID]->Fill(pos, etaD);
        computedEtaDerivativeXDataSize2InvNorm_[planeID]->Fill(pos);
    }

    for (int i = 0; i < hYEtaDistribution_[planeID]->GetXaxis()->GetNbins()+1; ++i)
    {
        if (hYEtaDistribution_[planeID]->GetBinContent(i) > 0) etaD = 1/hYEtaDistribution_[planeID]->GetBinContent(i);
        else etaD = 0;
        eta = hYEtaDistribution_[planeID]->GetBinCenter(i);
        pos = invertInHisto(planeID, eta, "y", "SizeLE2");
        computedEtaDerivativeYFitted_[planeID]->Fill(pos, etaD);
        pos = invertInHistoSingle(eta, hYAsimmetryLE2_[planeID]);
        computedEtaDerivativeYData_[planeID]->Fill(pos, etaD);
        pos = invertInHisto(planeID, eta, "y", "Size2");
        computedEtaDerivativeYFittedSize2_[planeID]->Fill(pos, etaD);
        pos = invertInHistoSingle(eta, hYAsimmetry_[planeID]);
        computedEtaDerivativeYDataSize2_[planeID]->Fill(pos, etaD);
        pos = YAsimmetryFuncInv_[planeID]->Eval(eta);
        computedEtaDerivativeYFittedSize2Inv_[planeID]->Fill(pos, etaD);
        pos = calculateInHisto(eta, hYAsimmetryInv_[planeID]);
        computedEtaDerivativeYDataSize2Inv_[planeID]->Fill(pos, etaD);
        computedEtaDerivativeYDataSize2InvNorm_[planeID]->Fill(pos);
    }
    
    computedEtaDerivativeXDataSize2Inv_[planeID]->Divide(computedEtaDerivativeXDataSize2InvNorm_[planeID]);
    computedEtaDerivativeYDataSize2Inv_[planeID]->Divide(computedEtaDerivativeYDataSize2InvNorm_[planeID]);
}

void TracksAfter::analyze(const Data& data, int threadNumber)//WARNING: You can't change this name (threadNumber) or the MACRO THREAD won't compile
{
    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        if(!passStandardCuts(p,data))
            continue;

        if(thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos)
        {
            if(!passCalibrationsCut(p,data))
                return;
        }
        calculateXResiduals(data, p, threadNumber);
        calculateYResiduals(data, p, threadNumber);
    }
}

void TracksAfter::endJob(void)
{
    for(unsigned int i=0; i<thePlaneMapping_->getNumberOfPlanes(); i++)
    {
        ADD_THREADED(XYReconstructed_[i]        );
        ADD_THREADED(XResidualsReconstructed_[i]);
        ADD_THREADED(YResidualsReconstructed_[i]);

        XYReconstructed_[i]->GetXaxis()->SetTitle("x coordinate (um)");
        XYReconstructed_[i]->GetYaxis()->SetTitle("y coordinate (um)");
        XResidualsReconstructed_[i]->GetXaxis()->SetTitle("x coordinate (um)");
        XResidualsReconstructed_[i]->GetYaxis()->SetTitle("residuals");
        YResidualsReconstructed_[i]->GetXaxis()->SetTitle("y coordinate (um)");
        YResidualsReconstructed_[i]->GetYaxis()->SetTitle("residuals");

        funcPredicted_->GetXaxis()->SetTitle("y coordinate (um)");
        funcPredicted_->GetYaxis()->SetTitle("eta value");
        funcPredicted2_->GetXaxis()->SetTitle("y coordinate (um)");
        funcPredicted2_->GetYaxis()->SetTitle("eta integrand value");
        funcPredicted3_->GetXaxis()->SetTitle("eta integrand value");
        funcPredicted3_->GetYaxis()->SetTitle("y coordinate (um)");

        ADD_THREADED(hXResidualsAsimmetry_[i]);
        hXResidualsAsimmetry_[i]->GetXaxis()->SetTitle("Residuals x (um)");
        ADD_THREADED(hYResidualsAsimmetry_[i]);
        hYResidualsAsimmetry_[i]->GetXaxis()->SetTitle("Residuals y (um)");
        ADD_THREADED(hXResidualsAsimmetryLE2_[i]);
        hXResidualsAsimmetryLE2_[i]->GetXaxis()->SetTitle("Residuals x (um)");
        ADD_THREADED(hYResidualsAsimmetryLE2_[i]);
        hYResidualsAsimmetryLE2_[i]->GetXaxis()->SetTitle("Residuals y (um)");

        computedEtaDerivativeXFitted_          [i]->GetXaxis()->SetTitle("Relative position on long pitch (um)");
        computedEtaDerivativeXFitted_          [i]->GetYaxis()->SetTitle("dEta/dx");
        computedEtaDerivativeYFitted_          [i]->GetXaxis()->SetTitle("Relative position on short pitch (um)");
        computedEtaDerivativeYFitted_          [i]->GetYaxis()->SetTitle("dEta/dy");
        computedEtaDerivativeXData_            [i]->GetXaxis()->SetTitle("Relative position on long pitch (um)");
        computedEtaDerivativeXData_            [i]->GetYaxis()->SetTitle("dEta/dx");
        computedEtaDerivativeYData_            [i]->GetXaxis()->SetTitle("Relative position on short pitch (um)");
        computedEtaDerivativeYData_            [i]->GetYaxis()->SetTitle("dEta/dy");
        computedEtaDerivativeXFittedSize2_     [i]->GetXaxis()->SetTitle("Relative position on long pitch (um)");
        computedEtaDerivativeXFittedSize2_     [i]->GetYaxis()->SetTitle("dEta/dx");
        computedEtaDerivativeYFittedSize2_     [i]->GetXaxis()->SetTitle("Relative position on short pitch (um)");
        computedEtaDerivativeYFittedSize2_     [i]->GetYaxis()->SetTitle("dEta/dy");
        computedEtaDerivativeXDataSize2_       [i]->GetXaxis()->SetTitle("Relative position on long pitch (um)");
        computedEtaDerivativeXDataSize2_       [i]->GetYaxis()->SetTitle("dEta/dx");
        computedEtaDerivativeYDataSize2_       [i]->GetXaxis()->SetTitle("Relative position on short pitch (um)");
        computedEtaDerivativeYDataSize2_       [i]->GetYaxis()->SetTitle("dEta/dy");
        computedEtaDerivativeXDataSize2Inv_    [i]->GetXaxis()->SetTitle("Relative position on long pitch (um)");
        computedEtaDerivativeXDataSize2Inv_    [i]->GetYaxis()->SetTitle("dEta/dx");
        computedEtaDerivativeYDataSize2Inv_    [i]->GetXaxis()->SetTitle("Relative position on short pitch (um)");
        computedEtaDerivativeYDataSize2Inv_    [i]->GetYaxis()->SetTitle("dEta/dy");
        computedEtaDerivativeXFittedSize2Inv_  [i]->GetXaxis()->SetTitle("Relative position on long pitch (um)");
        computedEtaDerivativeXFittedSize2Inv_  [i]->GetYaxis()->SetTitle("dEta/dx");
        computedEtaDerivativeYFittedSize2Inv_  [i]->GetXaxis()->SetTitle("Relative position on short pitch (um)");
        computedEtaDerivativeYFittedSize2Inv_  [i]->GetYaxis()->SetTitle("dEta/dy");
        computedEtaDerivativeXDataSize2InvNorm_[i]->GetXaxis()->SetTitle("Relative position on long pitch (um)");
        computedEtaDerivativeXDataSize2InvNorm_[i]->GetYaxis()->SetTitle("Number of events");
        computedEtaDerivativeYDataSize2InvNorm_[i]->GetXaxis()->SetTitle("Relative position on short pitch (um)");
        computedEtaDerivativeYDataSize2InvNorm_[i]->GetYaxis()->SetTitle("Number of events");
    }
}

void TracksAfter::preBook (void)
{

    std::cout << __PRETTY_FUNCTION__ << "Begin!"<< std::endl;
    std::string hName;
    std::string hTitle;
    std::string planeName;
    std::string toGet;

    int binSize = 5;
    int pixelLengthX = 150;
    int pixelLengthY = 100;
    int nBinsX = pixelLengthX/binSize;    //150um
    int nBinsY = pixelLengthY/binSize;    //100um originally, changed binning to 5x5um/bin

    theAnalysisManager_->cd("/");
    theAnalysisManager_->mkdir("TracksAfter");
    theAnalysisManager_->mkdir("Tests");
    std::cout << __PRETTY_FUNCTION__ << "done making dirs!"<< std::endl;

    funcPredicted_ = new TH1F("funcPredicted_", "test function for eta", 100, -50, 50);
    funcPredicted2_ = new TH1F("funcPredicted2_", "test function for eta integrand", 100, -50, 50);
    funcPredicted3_ = new TH1F("funcPredicted3_", "test function for eta inverse", 100, -1.1, 1.1);
    testPredictedFunc();

    theAnalysisManager_->cd("/");
    theAnalysisManager_->mkdir("TracksAfter");

    for(unsigned int i=0; i<thePlaneMapping_->getNumberOfPlanes(); i++) {
        planeName = thePlaneMapping_->getPlaneName(i);

        std::cout << __PRETTY_FUNCTION__ << "Plane: " << planeName << std::endl;
        theAnalysisManager_->cd("TracksAfter");
        theAnalysisManager_->mkdir(planeName);
        theAnalysisManager_->mkdir("Probability");

        hName = "XProbabilityFunc_" + planeName;
        hTitle = "Combined Probability function for x";
        XProbabilityFunc_.push_back(new TH2F(hName.c_str(), hTitle.c_str(), nBinsX, -pixelLengthX/2, pixelLengthX/2, 500, 0, 80000));
        hName = "YProbabilityFunc_" + planeName;
        hTitle = "Combined Probability function for y";
        YProbabilityFunc_.push_back(new TH2F(hName.c_str(), hTitle.c_str(), nBinsY, -pixelLengthY/2, pixelLengthY/2, 500, 0, 80000));

        hName = "XAsimmetryFunc_" + planeName;
        XAsimmetryFunc_.push_back(new TF1(hName.c_str(), Utilities::etaFitFunc, -pixelLengthX/2, pixelLengthX/2, 6));
        hName = "YAsimmetryFunc_" + planeName;
        YAsimmetryFunc_.push_back(new TF1(hName.c_str(), Utilities::etaFitFunc, -pixelLengthY/2, pixelLengthY/2, 6));
        hName = "XAsimmetryFuncInv_" + planeName;
        XAsimmetryFuncInv_.push_back(new TF1(hName.c_str(), "[0] + [1]*x", -pixelLengthX/2, pixelLengthX/2));
        hName = "YAsimmetryFuncInv_" + planeName;
        YAsimmetryFuncInv_.push_back(new TF1(hName.c_str(), "[0] + [1]*x", -pixelLengthY/2, pixelLengthY/2));
        hName = "XAsimmetryFuncRect_" + planeName;
        XAsimmetryFuncRect_.push_back(new TF1(hName.c_str(), "[0] + [1]*x", -pixelLengthX/2, pixelLengthX/2));
        hName = "YAsimmetryFuncRect_" + planeName;
        YAsimmetryFuncRect_.push_back(new TF1(hName.c_str(), "[0] + [1]*x", -pixelLengthY/2, pixelLengthY/2));

//-----------------------------------Imported objects from other analyses-----------------------------------------------------------------
        theAnalysisManager_->cd("/TracksAfter/" + planeName);
        theAnalysisManager_->mkdir("Imported");

        toGet = "Tracks/" + planeName + "/Xdistribution1D_" + planeName;
        hXdistribution1D_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Tracks/" + planeName + "/" + "Xdistribution2D_" + planeName;
        hXdistribution2D_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Tracks/" + planeName + "/" + "XChargedistribution1D_" + planeName;
        hXChargedistribution1D_.push_back((TH2F*)inFile_->Get(toGet.c_str()));
        toGet = "Tracks/" + planeName + "/" + "XChargedistribution2D_" + planeName;
        hXChargedistribution2D_.push_back((TH2F*)inFile_->Get(toGet.c_str()));
        toGet = "Tracks/" + planeName + "/" + "Ydistribution1D_" + planeName;
        hYdistribution1D_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Tracks/" + planeName + "/" + "Ydistribution2D_" + planeName;
        hYdistribution2D_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Tracks/" + planeName + "/" + "YChargedistribution1D_" + planeName;
        hYChargedistribution1D_.push_back((TH2F*)inFile_->Get(toGet.c_str()));
        toGet = "Tracks/" + planeName + "/" + "YChargedistribution2D_" + planeName;
        hYChargedistribution2D_.push_back((TH2F*)inFile_->Get(toGet.c_str()));

        toGet = "Charge/" + planeName + "/Xasimmetry/h1DXcellChargeAsimmetry_" + planeName;
        hXAsimmetry_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Yasimmetry/h1DYcellChargeAsimmetry_" + planeName;
        hYAsimmetry_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Xasimmetry/h1DXcellChargeAsimmetryInv_" + planeName;
        hXAsimmetryInv_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Yasimmetry/h1DYcellChargeAsimmetryInv_" + planeName;
        hYAsimmetryInv_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Xasimmetry/h1DXcellChargeAsimmetrySizeLE2_" + planeName;
        hXAsimmetryLE2_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Yasimmetry/h1DYcellChargeAsimmetrySizeLE2_" + planeName;
        hYAsimmetryLE2_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Xasimmetry/h1DXEtaDistribution_" + planeName;
        hXEtaDistribution_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Yasimmetry/h1DYEtaDistribution_" + planeName;
        hYEtaDistribution_.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Xasimmetry/projX_" + planeName;
        projXSize2.push_back((TH1F*)inFile_->Get(toGet.c_str()));
        toGet = "Charge/" + planeName + "/Yasimmetry/projY_" + planeName;
        projYSize2.push_back((TH1F*)inFile_->Get(toGet.c_str()));


//-------------------------------Support objects-------------------------------------------------------------------------------------------
        theAnalysisManager_->cd("/TracksAfter/" + planeName);
        theAnalysisManager_->mkdir("Utils");

        hName = "computedEtaFuncXSize2_" + planeName;
        hTitle = "Eta function after fitting, x coordinate, size 2";
        computedEtaFuncXSize2_.push_back(new TH1F(hName.c_str(), hTitle.c_str(), 3000, -pixelLengthX/2, pixelLengthX/2));
        hName = "computedEtaFuncYSize2_" + planeName;
        hTitle = "Eta function after fitting, y coordinate, size 2";
        computedEtaFuncYSize2_.push_back(new TH1F(hName.c_str(), hTitle.c_str(), 3000, -pixelLengthY/2, pixelLengthY/2));
        hName = "computedEtaFuncXSizeLE2_" + planeName;
        hTitle = "Eta function after fitting, x coordinate, size <= 2";
        computedEtaFuncXSizeLE2_.push_back(new TH1F(hName.c_str(), hTitle.c_str(), 1000, -pixelLengthX/2, pixelLengthX/2));
        hName = "computedEtaFuncYSizeLE2_" + planeName;
        hTitle = "Eta function after fitting, y coordinate, size <= 2";
        computedEtaFuncYSizeLE2_.push_back(new TH1F(hName.c_str(), hTitle.c_str(), 1000, -pixelLengthY/2, pixelLengthY/2));

    }
}

void TracksAfter::book(void)
{
    destroy();
    preBook();

    std::string hName;
    std::string hTitle;
    std::string planeName;

    int binSize = 5;
    int pixelLengthX = 150;
    int pixelLengthY = 100;
    int nBinsX = pixelLengthX/binSize;    //150um
    int nBinsY = pixelLengthY/binSize;    //100um originally, changed binning to 5x5um/bin

    theAnalysisManager_->cd("/");
    theAnalysisManager_->cd("TracksAfter");

    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        planeName = thePlaneMapping_->getPlaneName(p);

        XAsimmetryFuncInv_[p]->SetLineColor(kGreen);
        YAsimmetryFuncInv_[p]->SetLineColor(kGreen);
        XAsimmetryFuncRect_[p]->SetLineColor(kBlue);
        YAsimmetryFuncRect_[p]->SetLineColor(kBlue);
        XAsimmetryFuncRect_[p]->SetParName(0, "Intercept");
        XAsimmetryFuncRect_[p]->SetParName(1, "Slope");
        YAsimmetryFuncRect_[p]->SetParName(0, "Intercept");
        YAsimmetryFuncRect_[p]->SetParName(1, "Slope");
        hXAsimmetryInv_[p]->Fit(XAsimmetryFuncInv_[p], "QRB", "", -0.7, 0.7); // fitting method "QRBL"?
        hYAsimmetryInv_[p]->Fit(YAsimmetryFuncInv_[p], "QRB", "", -0.7, 0.7); // fitting method "QRBL"?
        hXAsimmetry_[p]->Fit(XAsimmetryFuncRect_[p], "QRB", "", -15, 15);
        hYAsimmetry_[p]->Fit(YAsimmetryFuncRect_[p], "QRB", "", -15, 15);

        theAnalysisManager_->cd("/TracksAfter/" + planeName);
        theAnalysisManager_->mkdir("Unfolded");

        hName = "XYReconstructed_" + planeName;
        hTitle = "Distribution of points from unfolding";
        XYReconstructed_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), nBinsX, -pixelLengthX/2, pixelLengthX/2, nBinsY, -pixelLengthY/2, pixelLengthY/2)));
        hName = "XResidualsReconstructed_" + planeName;
        hTitle = "x residuals after reconstruction from unfolding";
        XResidualsReconstructed_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsX, -pixelLengthX/2, pixelLengthX/2)));
        hName = "YResidualsReconstructed_" + planeName;
        hTitle = "y residuals after reconstruction from unfolding";
        YResidualsReconstructed_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), nBinsY, -pixelLengthY/2, pixelLengthY/2)));

        theAnalysisManager_->cd("/TracksAfter/" + planeName);
        theAnalysisManager_->mkdir("Reconstructed");

        hName  = "hXResidualsAsimmetry_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit, size 2, " + planeName;
        hXResidualsAsimmetry_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));
        hName  = "hYResidualsAsimmetry_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit, size 2, " + planeName;
        hYResidualsAsimmetry_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));
        hName  = "hXResidualsAsimmetryLE2_"                      + planeName;
        hTitle = "X residuals calculated from asimmetry fit, size <= 2, " + planeName;
        hXResidualsAsimmetryLE2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));
        hName  = "hYResidualsAsimmetryLE2_"                      + planeName;
        hTitle = "Y residuals calculated from asimmetry fit, size <= 2, " + planeName;
        hYResidualsAsimmetryLE2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 125, -500, 500)));

        theAnalysisManager_->cd("/TracksAfter/" + planeName);
        theAnalysisManager_->mkdir("X Eta derivative");

        hName = "computedDerivativeXFitted_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with fitted eta function, coordinate x, " + planeName;
        computedEtaDerivativeXFitted_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsX*5, -pixelLengthX/2, pixelLengthX/2));
        hName = "computedDerivativeXData_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with unfitted eta function, coordinate x, " + planeName;
        computedEtaDerivativeXData_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsX*5, -pixelLengthX/2, pixelLengthX/2));
        hName = "computedDerivativeXFittedSize2_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with fitted eta function of size 2, coordinate x, " + planeName;
        computedEtaDerivativeXFittedSize2_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsX*2, -pixelLengthX/2, pixelLengthX/2));
        hName = "computedDerivativeXDataSize2_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with unfitted eta function of size 2, coordinate x, " + planeName;
        computedEtaDerivativeXDataSize2_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsX*2, -pixelLengthX/2, pixelLengthX/2));
        hName = "computedDerivativeXFittedSize2Inv_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with fitted eta inverse function of size 2, coordinate x, " + planeName;
        computedEtaDerivativeXFittedSize2Inv_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsX*5, -pixelLengthX/2, pixelLengthX/2));
        hName = "computedDerivativeXDataSize2Inv_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with unfitted eta function of size 2 inverted, coordinate x, " + planeName;
        computedEtaDerivativeXDataSize2Inv_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsX*5, -pixelLengthX/2, pixelLengthX/2));
        hName = "computedDerivativeXDataSize2InvNorm_" + planeName;
        hTitle = "Number of events to normalize eta derivative from eta distribution, with unfitted eta function of size 2 inverted, coordinate x, " + planeName;
        computedEtaDerivativeXDataSize2InvNorm_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsX*5, -pixelLengthX/2, pixelLengthX/2));

        theAnalysisManager_->cd("/TracksAfter/" + planeName);
        theAnalysisManager_->mkdir("Y Eta derivative");

        hName = "computedDerivativeYFitted_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with fitted eta function, coordinate y, " + planeName;
        computedEtaDerivativeYFitted_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsY*5, -pixelLengthY/2, pixelLengthY/2));
        hName = "computedDerivativeYData_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with unfitted eta function, coordinate y, " + planeName;
        computedEtaDerivativeYData_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsY*5, -pixelLengthY/2, pixelLengthY/2));
        hName = "computedDerivativeYFittedSize2_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with fitted eta function of size 2, coordinate y, " + planeName;
        computedEtaDerivativeYFittedSize2_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsY*2, -pixelLengthY/2, pixelLengthY/2));
        hName = "computedDerivativeYDataSize2_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with unfitted eta function of size 2, coordinate y, " + planeName;
        computedEtaDerivativeYDataSize2_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsY*2, -pixelLengthY/2, pixelLengthY/2));
        hName = "computedDerivativeYFittedSize2Inv_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with fitted eta inverse function of size 2, coordinate y, " + planeName;
        computedEtaDerivativeYFittedSize2Inv_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsY*5, -pixelLengthY/2, pixelLengthY/2));
        hName = "computedDerivativeYDataSize2Inv_" + planeName;
        hTitle = "Computed eta derivative from eta distribution, with unfitted eta function of size 2 inverted, coordinate y, " + planeName;
        computedEtaDerivativeYDataSize2Inv_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsY*5, -pixelLengthY/2, pixelLengthY/2));
        hName = "computedDerivativeYDataSize2InvNorm_" + planeName;
        hTitle = "Number of events to normalize eta derivative from eta distribution, with unfitted eta function of size 2 inverted, coordinate y, " + planeName;
        computedEtaDerivativeYDataSize2InvNorm_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), nBinsY*5, -pixelLengthY/2, pixelLengthY/2));
    }
}
