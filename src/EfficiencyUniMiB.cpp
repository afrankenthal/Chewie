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

#include "EfficiencyUniMiB.h"

#include "AnalysisManager.h"
#include "WindowsManager.h"
#include "Window.h"
#include "ThreadUtilities.h"
#include "PlanesMapping.h"
#include "MessageTools.h"
#include "XmlParser.h"
#include "XmlAnalysis.h"
#include "XmlPlane.h"
#include "HistogramWindow.h"

#include "XmlWindow.h"
#include "XmlScan.h"

#include <TH2F.h>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>


// @@@ Hard coded parameters @@@
#define ONLYdoubleHITS false // Process only clusters of size 2
// ============================


//=======================================================================
EfficiencyUniMiB::EfficiencyUniMiB(AnalysisManager* analysisManager, int nOfThreads) :
  Analysis(analysisManager, nOfThreads),
  thePlaneMapping_(0),
  theWindowsManager_(0),
  theXmlParser_(analysisManager->getXmlParser())
{
  STDLINE("Running EfficiencyUniMiB analysis",ACCyan);

  thePlaneMapping_ = new PlanesMapping();
}

//=======================================================================
EfficiencyUniMiB::~EfficiencyUniMiB(void)
{
  if (thePlaneMapping_) delete thePlaneMapping_;
  
  destroy();
}

//=======================================================================
void EfficiencyUniMiB::destroy(void)
{
  if (Analysis::fDoNotDelete_) return;

  std::vector<TH1F*>::iterator it1;
  std::vector<TH2F*>::iterator it2;

  for (it1=hEfficiency_                     .begin(); it1!=hEfficiency_  	      	     .end(); it1++) delete *it1; hEfficiency_		          .clear();
  for (it1=hEfficiencyNorm_                 .begin(); it1!=hEfficiencyNorm_	      	     .end(); it1++) delete *it1; hEfficiencyNorm_		  .clear();

  for (it1=hEfficiencyRef_                  .begin(); it1!=hEfficiencyRef_  	      	     .end(); it1++) delete *it1; hEfficiencyRef_	          .clear();
  for (it1=hEfficiencyRefNorm_              .begin(); it1!=hEfficiencyRefNorm_	      	     .end(); it1++) delete *it1; hEfficiencyRefNorm_	          .clear();

  for (it2=h2DEfficiency_                   .begin(); it2!=h2DEfficiency_	      	     .end(); it2++) delete *it2; h2DEfficiency_		          .clear();
  for (it2=h2DEfficiencyNorm_               .begin(); it2!=h2DEfficiencyNorm_	      	     .end(); it2++) delete *it2; h2DEfficiencyNorm_ 	          .clear();

  for (it2=h2DEfficiencyRef_                .begin(); it2!=h2DEfficiencyRef_	      	     .end(); it2++) delete *it2; h2DEfficiencyRef_                .clear();
  for (it2=h2DEfficiencyRefNorm_            .begin(); it2!=h2DEfficiencyRefNorm_	     .end(); it2++) delete *it2; h2DEfficiencyRefNorm_ 	          .clear();

  for (it2=h2D4cellEfficiency_              .begin(); it2!=h2D4cellEfficiency_	      	     .end(); it2++) delete *it2; h2D4cellEfficiency_	          .clear();
  for (it2=h2D4cellEfficiencyNorm_          .begin(); it2!=h2D4cellEfficiencyNorm_           .end(); it2++) delete *it2; h2D4cellEfficiencyNorm_	  .clear();

  for (it2=hCellEfficiency_                 .begin(); it2!=hCellEfficiency_	      	     .end(); it2++) delete *it2; hCellEfficiency_	          .clear();
  for (it2=hCellEfficiencyNorm_             .begin(); it2!=hCellEfficiencyNorm_              .end(); it2++) delete *it2; hCellEfficiencyNorm_             .clear();

  for (it2=hCellEfficiencyEvenColumns_      .begin(); it2!=hCellEfficiencyEvenColumns_       .end(); it2++) delete *it2; hCellEfficiencyEvenColumns_      .clear();
  for (it2=hCellEfficiencyEvenColumnsNorm_  .begin(); it2!=hCellEfficiencyEvenColumnsNorm_   .end(); it2++) delete *it2; hCellEfficiencyEvenColumnsNorm_  .clear();

  for (it2=hCellEfficiencyOddColumns_       .begin(); it2!=hCellEfficiencyOddColumns_        .end(); it2++) delete *it2; hCellEfficiencyOddColumns_       .clear();
  for (it2=hCellEfficiencyOddColumnsNorm_   .begin(); it2!=hCellEfficiencyOddColumnsNorm_    .end(); it2++) delete *it2; hCellEfficiencyOddColumnsNorm_   .clear();

  for (it1=h1DXcellEfficiencyFirstHit_      .begin(); it1!=h1DXcellEfficiencyFirstHit_       .end(); it1++) delete *it1; h1DXcellEfficiencyFirstHit_      .clear();
  for (it1=h1DXcellEfficiencySecondHit_     .begin(); it1!=h1DXcellEfficiencySecondHit_      .end(); it1++) delete *it1; h1DXcellEfficiencySecondHit_     .clear();
  for (it1=h1DXcellEfficiencyNorm_          .begin(); it1!=h1DXcellEfficiencyNorm_           .end(); it1++) delete *it1; h1DXcellEfficiencyNorm_	  .clear();

  for (it1=h1DYcellEfficiencyFirstHit_      .begin(); it1!=h1DYcellEfficiencyFirstHit_       .end(); it1++) delete *it1; h1DYcellEfficiencyFirstHit_      .clear();
  for (it1=h1DYcellEfficiencySecondHit_     .begin(); it1!=h1DYcellEfficiencySecondHit_      .end(); it1++) delete *it1; h1DYcellEfficiencySecondHit_     .clear();
  for (it1=h1DYcellEfficiencyNorm_          .begin(); it1!=h1DYcellEfficiencyNorm_           .end(); it1++) delete *it1; h1DYcellEfficiencyNorm_	  .clear();

  for (it1=h1DXcellEdgeRightEfficiency_     .begin(); it1!=h1DXcellEdgeRightEfficiency_      .end(); it1++) delete *it1; h1DXcellEdgeRightEfficiency_	  .clear();
  for (it1=h1DXcellEdgeRightEfficiencyNorm_ .begin(); it1!=h1DXcellEdgeRightEfficiencyNorm_  .end(); it1++) delete *it1; h1DXcellEdgeRightEfficiencyNorm_ .clear();

  for (it1=h1DXcellEdgeLeftEfficiency_      .begin(); it1!=h1DXcellEdgeLeftEfficiency_       .end(); it1++) delete *it1; h1DXcellEdgeLeftEfficiency_	  .clear();
  for (it1=h1DXcellEdgeLeftEfficiencyNorm_  .begin(); it1!=h1DXcellEdgeLeftEfficiencyNorm_   .end(); it1++) delete *it1; h1DXcellEdgeLeftEfficiencyNorm_  .clear();

  for (it1=h1DYcellEdgeUpEfficiency_        .begin(); it1!=h1DYcellEdgeUpEfficiency_         .end(); it1++) delete *it1; h1DYcellEdgeUpEfficiency_	  .clear();
  for (it1=h1DYcellEdgeUpEfficiencyNorm_    .begin(); it1!=h1DYcellEdgeUpEfficiencyNorm_     .end(); it1++) delete *it1; h1DYcellEdgeUpEfficiencyNorm_    .clear();

  for (it1=h1DYcellEdgeDownEfficiency_      .begin(); it1!=h1DYcellEdgeDownEfficiency_       .end(); it1++) delete *it1; h1DYcellEdgeDownEfficiency_	  .clear();
  for (it1=h1DYcellEdgeDownEfficiencyNorm_  .begin(); it1!=h1DYcellEdgeDownEfficiencyNorm_   .end(); it1++) delete *it1; h1DYcellEdgeDownEfficiencyNorm_  .clear();
}

//=======================================================================
void EfficiencyUniMiB::beginJob(void)
{
  theWindowsManager_ = theAnalysisManager_->getWindowsManager();
  
  if (theXmlParser_->getScan()->getScanValues().size() == 0) book();
  else                                                       
    {
      STDLINE("Unknown option: scan values",ACRed);
      exit(EXIT_FAILURE);
    }
}

//=======================================================================
void EfficiencyUniMiB::analyze(const Data& data, int threadNumber)
{
  bool passMainCut = true;
  if (cutsFormulas_.find("main cut") != cutsFormulas_.end())
    passMainCut = cutsFormulas_["main cut"][threadNumber]->EvalInstance();
  
  if (!passMainCut) return;
  
  bool passCellEfficiencyCut = true;
  if (cutsFormulas_.find("cell efficiency") != cutsFormulas_.end())
    passCellEfficiencyCut = cutsFormulas_["cell efficiency"][threadNumber]->EvalInstance();
      
  bool passXCellEfficiencyCut = true;
  if (cutsFormulas_.find("cell efficiency X") != cutsFormulas_.end())
    passXCellEfficiencyCut = cutsFormulas_["cell efficiency X"][threadNumber]->EvalInstance();
      
  bool passYCellEfficiencyCut = true;
  if (cutsFormulas_.find("cell efficiency Y") != cutsFormulas_.end())
    passYCellEfficiencyCut = cutsFormulas_["cell efficiency Y"][threadNumber]->EvalInstance();


  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      if (!passStandardCuts(p,data)) continue;
	  
	  
      // ######################################################
      // # Require all telescope planes with cluster size = 2 #
      // ######################################################
      if (ONLYdoubleHITS == true)
	for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
	  if ((p > 7) && (p < 16) && (data.getClusterSize(p) != 2)) return;
	  
	  
      planeEfficiency (passMainCut           ,p,data,threadNumber);
      cellEfficiency  (passCellEfficiencyCut ,p,data,threadNumber);
      xCellEfficiency (passXCellEfficiencyCut,p,data,threadNumber);
      yCellEfficiency (passYCellEfficiencyCut,p,data,threadNumber);
      xEdgeEfficiency (passXCellEfficiencyCut,p,data,threadNumber);
      yEdgeEfficiency (passYCellEfficiencyCut,p,data,threadNumber);
    }
}

//=======================================================================
void EfficiencyUniMiB::endJob(void)
{
  std::stringstream ss;

  double efficiency;
  double Ntracks;
  double error;

  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      std::string planeName = thePlaneMapping_->getPlaneName(p);
      ss.str("") ; ss << "Adding threads for plane " << p;
      STDLINE(ss.str().c_str(),ACYellow);

      ADD_THREADED(hEfficiency_                	      [p]);
      ADD_THREADED(hEfficiencyNorm_            	      [p]);

      ADD_THREADED(hEfficiencyRef_                    [p]);
      ADD_THREADED(hEfficiencyRefNorm_         	      [p]);

      ADD_THREADED(h2DEfficiency_              	      [p]);
      ADD_THREADED(h2DEfficiencyNorm_          	      [p]);

      ADD_THREADED(h2DEfficiencyRef_           	      [p]);
      ADD_THREADED(h2DEfficiencyRefNorm_              [p]);

      ADD_THREADED(h2D4cellEfficiency_         	      [p]);
      ADD_THREADED(h2D4cellEfficiencyNorm_     	      [p]);

      ADD_THREADED(hCellEfficiency_            	      [p]);
      ADD_THREADED(hCellEfficiencyNorm_        	      [p]);

      ADD_THREADED(hCellEfficiencyEvenColumns_        [p]);
      ADD_THREADED(hCellEfficiencyEvenColumnsNorm_    [p]);

      ADD_THREADED(hCellEfficiencyOddColumns_         [p]);
      ADD_THREADED(hCellEfficiencyOddColumnsNorm_     [p]);

      ADD_THREADED(h1DXcellEfficiencyFirstHit_ 	      [p]);
      ADD_THREADED(h1DXcellEfficiencySecondHit_	      [p]);
      ADD_THREADED(h1DXcellEfficiencyNorm_     	      [p]);

      ADD_THREADED(h1DYcellEfficiencyFirstHit_ 	      [p]);
      ADD_THREADED(h1DYcellEfficiencySecondHit_	      [p]);
      ADD_THREADED(h1DYcellEfficiencyNorm_     	      [p]);

      ADD_THREADED(h1DXcellEdgeRightEfficiency_	      [p]);
      ADD_THREADED(h1DXcellEdgeRightEfficiencyNorm_   [p]);

      ADD_THREADED(h1DXcellEdgeLeftEfficiency_	      [p]);
      ADD_THREADED(h1DXcellEdgeLeftEfficiencyNorm_    [p]);

      ADD_THREADED(h1DYcellEdgeUpEfficiency_	      [p]);
      ADD_THREADED(h1DYcellEdgeUpEfficiencyNorm_      [p]);

      ADD_THREADED(h1DYcellEdgeDownEfficiency_	      [p]);
      ADD_THREADED(h1DYcellEdgeDownEfficiencyNorm_    [p]);

      STDLINE("Threading phase completed",ACGreen);


      STDLINE("Filling phase...",ACWhite);

      hEfficiency_                 [p]->Divide(hEfficiencyNorm_                 [p]);

      h2DEfficiency_               [p]->Divide(h2DEfficiencyNorm_               [p]);

      hEfficiencyRef_              [p]->Divide(hEfficiencyRefNorm_              [p]);

      h2DEfficiencyRef_            [p]->Divide(h2DEfficiencyRefNorm_            [p]);

      h2D4cellEfficiency_          [p]->Divide(h2D4cellEfficiencyNorm_          [p]);

      hCellEfficiency_             [p]->Divide(hCellEfficiencyNorm_             [p]);

      hCellEfficiencyEvenColumns_  [p]->Divide(hCellEfficiencyEvenColumnsNorm_  [p]);

      hCellEfficiencyOddColumns_   [p]->Divide(hCellEfficiencyOddColumnsNorm_   [p]);

      h1DXcellEfficiencyFirstHit_  [p]->Divide(h1DXcellEfficiencyNorm_	        [p]);

      h1DXcellEfficiencySecondHit_ [p]->Divide(h1DXcellEfficiencyNorm_	        [p]);

      h1DYcellEfficiencyFirstHit_  [p]->Divide(h1DYcellEfficiencyNorm_	        [p]);

      h1DYcellEfficiencySecondHit_ [p]->Divide(h1DYcellEfficiencyNorm_	        [p]);

      h1DXcellEdgeRightEfficiency_ [p]->Divide(h1DXcellEdgeRightEfficiencyNorm_ [p]);

      h1DXcellEdgeLeftEfficiency_  [p]->Divide(h1DXcellEdgeLeftEfficiencyNorm_  [p]);

      h1DYcellEdgeUpEfficiency_    [p]->Divide(h1DYcellEdgeUpEfficiencyNorm_    [p]);

      h1DYcellEdgeDownEfficiency_  [p]->Divide(h1DYcellEdgeDownEfficiencyNorm_  [p]);


      // ######################
      // # Setting error bars #
      // ######################
      setErrorsBar(p);


      STDLINE("Setting styles...",ACWhite);

      h1DXcellEfficiencyFirstHit_     [p]->SetMarkerStyle(20);
      h1DXcellEfficiencyFirstHit_     [p]->SetMarkerSize(0.6);

      h1DXcellEfficiencySecondHit_    [p]->SetMarkerStyle(20);
      h1DXcellEfficiencySecondHit_    [p]->SetMarkerSize(0.6);

      h1DYcellEfficiencyFirstHit_     [p]->SetMarkerStyle(20);
      h1DYcellEfficiencyFirstHit_     [p]->SetMarkerSize(0.6);

      h1DYcellEfficiencySecondHit_    [p]->SetMarkerStyle(20);
      h1DYcellEfficiencySecondHit_    [p]->SetMarkerSize(0.6);

      h1DXcellEdgeRightEfficiency_    [p]->SetMarkerStyle(20);
      h1DXcellEdgeRightEfficiency_    [p]->SetMarkerSize(0.6);

      h1DXcellEdgeLeftEfficiency_     [p]->SetMarkerStyle(20);
      h1DXcellEdgeLeftEfficiency_     [p]->SetMarkerSize(0.6);

      h1DYcellEdgeUpEfficiency_       [p]->SetMarkerStyle(20);
      h1DYcellEdgeUpEfficiency_       [p]->SetMarkerSize(0.6);

      h1DYcellEdgeDownEfficiency_     [p]->SetMarkerStyle(20);
      h1DYcellEdgeDownEfficiency_     [p]->SetMarkerSize(0.6);


      h2DEfficiency_                  [p]->GetXaxis()->SetTitle("column");
      h2DEfficiency_                  [p]->GetYaxis()->SetTitle("row"   );

      h2DEfficiencyNorm_              [p]->GetXaxis()->SetTitle("column");
      h2DEfficiencyNorm_              [p]->GetYaxis()->SetTitle("row"   );

      h2DEfficiencyRef_               [p]->GetXaxis()->SetTitle("column");
      h2DEfficiencyRef_               [p]->GetYaxis()->SetTitle("row"   );

      h2DEfficiencyRefNorm_           [p]->GetXaxis()->SetTitle("column");
      h2DEfficiencyRefNorm_           [p]->GetYaxis()->SetTitle("row"   );

      h2D4cellEfficiency_             [p]->GetXaxis()->SetTitle("x (um)");
      h2D4cellEfficiency_             [p]->GetYaxis()->SetTitle("y (um)");

      h2D4cellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("x (um)");
      h2D4cellEfficiencyNorm_         [p]->GetYaxis()->SetTitle("y (um)");

      hCellEfficiency_                [p]->GetXaxis()->SetTitle("x (um)");
      hCellEfficiency_                [p]->GetYaxis()->SetTitle("y (um)");

      hCellEfficiencyNorm_            [p]->GetXaxis()->SetTitle("x (um)");
      hCellEfficiencyNorm_            [p]->GetYaxis()->SetTitle("y (um)");

      hCellEfficiencyEvenColumns_     [p]->GetXaxis()->SetTitle("x (um)");
      hCellEfficiencyEvenColumns_     [p]->GetYaxis()->SetTitle("y (um)");

      hCellEfficiencyEvenColumnsNorm_ [p]->GetXaxis()->SetTitle("x (um)");
      hCellEfficiencyEvenColumnsNorm_ [p]->GetYaxis()->SetTitle("y (um)");

      hCellEfficiencyOddColumns_      [p]->GetXaxis()->SetTitle("x (um)");
      hCellEfficiencyOddColumns_      [p]->GetYaxis()->SetTitle("y (um)");

      hCellEfficiencyOddColumnsNorm_  [p]->GetXaxis()->SetTitle("x (um)");
      hCellEfficiencyOddColumnsNorm_  [p]->GetYaxis()->SetTitle("y (um)");

      h1DXcellEfficiencyFirstHit_     [p]->GetXaxis()->SetTitle("x (um)");
      h1DXcellEfficiencySecondHit_    [p]->GetXaxis()->SetTitle("x (um)");
      h1DXcellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("x (um)");

      h1DXcellEdgeRightEfficiency_    [p]->GetXaxis()->SetTitle("x (um)");
      h1DXcellEdgeRightEfficiencyNorm_[p]->GetXaxis()->SetTitle("x (um)");

      h1DXcellEdgeLeftEfficiency_     [p]->GetXaxis()->SetTitle("x (um)");
      h1DXcellEdgeLeftEfficiencyNorm_ [p]->GetXaxis()->SetTitle("x (um)");

      h1DYcellEfficiencyFirstHit_     [p]->GetXaxis()->SetTitle("y (um)");
      h1DYcellEfficiencySecondHit_    [p]->GetXaxis()->SetTitle("y (um)");
      h1DYcellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("y (um)");

      h1DYcellEdgeUpEfficiency_       [p]->GetXaxis()->SetTitle("y (um)");
      h1DYcellEdgeUpEfficiencyNorm_   [p]->GetXaxis()->SetTitle("y (um)");

      h1DYcellEdgeDownEfficiency_     [p]->GetXaxis()->SetTitle("y (um)");
      h1DYcellEdgeDownEfficiencyNorm_ [p]->GetXaxis()->SetTitle("y (um)");


      // ##############################
      // # Print efficiency on screen #
      // ##############################
      efficiency = hEfficiencyRef_    [p]->GetBinContent(1);
      Ntracks    = hEfficiencyRefNorm_[p]->GetBinContent(1);
      error      = sqrt(efficiency * (1 - efficiency) / Ntracks);

      ss.str("");
      ss << "Detector: "    << std::setw(27) << thePlaneMapping_->getPlaneName(p)
	 << " efficiency: " << std::setw(4)  << std::setprecision(7) << efficiency*100.
	 << " +- "          << std::setw(4)  << std::setprecision(3) << error*100.;

      STDLINE(ss.str(),ACLightPurple);
    }
}

//=======================================================================
void EfficiencyUniMiB::book(void)
{
  destroy();

  std::string hName;
  std::string hTitle;
  std::string planeName;

  int         nBinsX;
  int         nBinsY;

  float       xPitch;
  float       yPitch;
  float       binSize = 5; // [um]

  int         lowerCol;
  int         higherCol;
  int         lowerRow;
  int         higherRow;


  theAnalysisManager_->cd("/");
  theAnalysisManager_->mkdir("Efficiency");


  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
      planeName = thePlaneMapping_->getPlaneName(p);

      xPitch    = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
      yPitch    = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

      lowerCol  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerCol ()).c_str());
      higherCol = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherCol()).c_str());
      lowerRow  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerRow ()).c_str());
      higherRow = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherRow()).c_str());

      nBinsX = abs(lowerCol - higherCol);
      nBinsY = abs(lowerRow - higherRow);

      if (nBinsY <= 0) nBinsY = 1; // Planes which are not in the geometry file have lowerRow = higherRow = 0,
                                   // this produces an unexpected warning

      theAnalysisManager_->cd("Efficiency");
      theAnalysisManager_->mkdir(planeName);
      theAnalysisManager_->mkdir("Efficiency");


      // #################
      // # 1D histograms #
      // #################
      hName  = "Efficiency_"                                            + planeName;
      hTitle = "Overall efficiency "                                    + planeName;
      hEfficiency_.push_back                (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

      hName  = "EfficiencyNorm_"                                        + planeName;
      hTitle = "Overall efficiency normalization "                      + planeName;
      hEfficiencyNorm_.push_back            (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));


      hName  = "EfficiencyRef_"                                         + planeName;
      hTitle = "Overall efficiency ref. "                               + planeName;
      hEfficiencyRef_.push_back             (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

      hName  = "EfficiencyRefNorm_"                                     + planeName;
      hTitle = "Overall efficiency ref. normalization "                 + planeName;
      hEfficiencyRefNorm_.push_back         (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));


      hName  = "h1DXcellEfficiencyFirstHit_"                            + planeName;
      hTitle = "1D Cell efficiency - X coordinate first hit "           + planeName;
      h1DXcellEfficiencyFirstHit_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));

      hName  = "h1DXcellEfficiencySecondHit_"                           + planeName;
      hTitle = "1D cell Efficiency - X coordinate second hit "          + planeName;
      h1DXcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));

      hName  = "h1DXcellEfficiencyNorm_"                                + planeName;
      hTitle = "1D cell Efficiency - X coordinate normalization "       + planeName;
      h1DXcellEfficiencyNorm_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));


      hName  = "h1DXcellEdgeRightEfficiency_"                           + planeName;
      hTitle = "1D edge-right Efficiency - X coordinate "               + planeName;
      h1DXcellEdgeRightEfficiency_.push_back    (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch,xPitch)));

      hName  = "h1DXcellEdgeRightEfficiencyNorm_"                       + planeName;
      hTitle = "1D edge-right Efficiency - X coordinate normalization " + planeName;
      h1DXcellEdgeRightEfficiencyNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch,xPitch)));


      hName  = "h1DXcellEdgeLeftEfficiency_"                            + planeName;
      hTitle = "1D edge-left Efficiency - X coordinate "                + planeName;
      h1DXcellEdgeLeftEfficiency_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch,xPitch)));

      hName  = "h1DXcellEdgeLeftEfficiencyNorm_"                        + planeName;
      hTitle = "1D edge-left Efficiency - X coordinate normalization "  + planeName;
      h1DXcellEdgeLeftEfficiencyNorm_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch,xPitch)));


      hName  = "h1DYcellEfficiencyFirstHit_"                            + planeName;
      hTitle = "1D cell Efficiency - Y coordinate first hit "           + planeName;
      h1DYcellEfficiencyFirstHit_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "h1DYcellEfficiencySecondHit_"                           + planeName;
      hTitle = "1D cell Efficiency - Y coordinate second hit "          + planeName;
      h1DYcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "h1DYcellEfficiencyNorm_"                                + planeName;
      hTitle = "1D cell Efficiency - Y coordinate normalization "       + planeName;
      h1DYcellEfficiencyNorm_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch/2,yPitch/2)));


      hName  = "h1DYcellEdgeUpEfficiency_"                              + planeName;
      hTitle = "1D edge-up Efficiency - Y coordinate "                  + planeName;
      h1DYcellEdgeUpEfficiency_.push_back       (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch,yPitch)));

      hName  = "h1DYcellEdgeUpEfficiencyNorm_"                          + planeName;
      hTitle = "1D edge-up Efficiency - Y coordinate normalization "    + planeName;
      h1DYcellEdgeUpEfficiencyNorm_.push_back   (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch,yPitch)));


      hName  = "h1DYcellEdgeDownEfficiency_"                            + planeName;
      hTitle = "1D edge-down Efficiency - Y coordinate "                + planeName;
      h1DYcellEdgeDownEfficiency_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch,yPitch)));

      hName  = "h1DYcellEdgeDownEfficiencyNorm_"                        + planeName;
      hTitle = "1D edge-down Efficiency - Y coordinate normalization "  + planeName;
      h1DYcellEdgeDownEfficiencyNorm_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch,yPitch)));


      // #################
      // # 2D histograms #
      // #################
      hName  = "2DEfficiency_"                                          + planeName;
      hTitle = "2D efficiency distribution "                            + planeName;
      h2DEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));

      hName  = "2DEfficiencyNorm_"                                      + planeName;
      hTitle = "2D efficiency normalization "                           + planeName;
      h2DEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));


      hName  = "2DEfficiencyRef_"                                       + planeName;
      hTitle = "2D efficiency distribution ref. "                       + planeName;
      h2DEfficiencyRef_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));

      hName  = "2DEfficiencyRefNorm_"                                   + planeName;
      hTitle = "2D efficiency ref. normalization "                      + planeName;
      h2DEfficiencyRefNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));




      theAnalysisManager_->cd("Efficiency/" + planeName);
      theAnalysisManager_->mkdir("CellEfficiency");


      // #################
      // # 2D histograms #
      // #################
      hName  = "hCellEfficiency_"                                       + planeName;
      hTitle = "Cell efficiency "                                       + planeName;
      hCellEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "hCellEfficiencyNorm_"                                   + planeName;
      hTitle = "Cell efficiency normalization "                         + planeName;
      hCellEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));


      hName  = "hCellEfficiencyEvenColumns_"                            + planeName;
      hTitle = "Cell efficiency even columns "                          + planeName;
      hCellEfficiencyEvenColumns_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "hCellEfficiencyEvenColumnsNorm_"                        + planeName;
      hTitle = "Cell efficiency normalization even columns "            + planeName;
      hCellEfficiencyEvenColumnsNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));


      hName  = "hCellEfficiencyOddColumns_"                             + planeName;
      hTitle = "Cell efficiency odd columns "                           + planeName;
      hCellEfficiencyOddColumns_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "hCellEfficiencyOddColumnsNorm_"                         + planeName;
      hTitle = "Cell efficiency normalization odd columns "             + planeName;
      hCellEfficiencyOddColumnsNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));


      hName  = "h2D4cellEfficiency_"                                    + planeName;
      hTitle = "4 cell efficiency "                                     + planeName;
      h2D4cellEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "h2D4cellEfficiencyNorm_"                                + planeName;
      hTitle = "4 cell efficiency normalization "                       + planeName;
      h2D4cellEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));
    }
}

//=======================================================================
void EfficiencyUniMiB::setErrorsBar(int planeID)
{
    double efficiency;
    double Ntrack;
    double error;
    int    nBins;

    nBins = h1DXcellEfficiencyFirstHit_[planeID]->GetNbinsX();
    for(int b = 1; b <= nBins; b++)
    {
        efficiency = h1DXcellEfficiencyFirstHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency * (1-efficiency) / Ntrack);
        h1DXcellEfficiencyFirstHit_[planeID]->SetBinError(b,error);

        efficiency = h1DXcellEfficiencySecondHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency * (1-efficiency) / Ntrack);
        h1DXcellEfficiencySecondHit_[planeID]->SetBinError(b,error);
    }

    nBins = h1DXcellEdgeRightEfficiency_[planeID]->GetNbinsX();
    for(int b = 1; b <= nBins; b++)
    {
        efficiency = h1DXcellEdgeRightEfficiency_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEdgeRightEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency * (1-efficiency) / Ntrack);
        h1DXcellEdgeRightEfficiency_[planeID]->SetBinError(b,error);

        efficiency = h1DXcellEdgeLeftEfficiency_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEdgeLeftEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency * (1-efficiency) / Ntrack);
        h1DXcellEdgeLeftEfficiency_[planeID]->SetBinError(b,error);
    }

    nBins = h1DYcellEfficiencyFirstHit_[planeID]->GetNbinsX();
    for(int b = 1; b <= nBins; b++)
    {
        efficiency = h1DYcellEfficiencyFirstHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency * (1-efficiency) / Ntrack);
        h1DYcellEfficiencyFirstHit_[planeID]->SetBinError(b,error);

        efficiency = h1DYcellEfficiencySecondHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency * (1-efficiency) / Ntrack);
        h1DYcellEfficiencySecondHit_[planeID]->SetBinError(b,error);
    }

    nBins = h1DYcellEdgeUpEfficiency_[planeID]->GetNbinsX();
    for(int b = 1; b <= nBins; b++)
    {
        efficiency = h1DYcellEdgeUpEfficiency_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEdgeUpEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency * (1-efficiency) / Ntrack);
        h1DYcellEdgeUpEfficiency_[planeID]->SetBinError(b,error);

        efficiency = h1DYcellEdgeDownEfficiency_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEdgeDownEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency * (1-efficiency) / Ntrack);
        h1DYcellEdgeDownEfficiency_[planeID]->SetBinError(b,error);
    }
}

//=======================================================================
void EfficiencyUniMiB::planeEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  if (!pass || !data.getIsInDetector(planeID)) return;

  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;


  // #########################################################################
  // # Compute efficiency only for cells that are surrounded by "good" cells #
  // #########################################################################
  if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)))
    {
      THREADED(hEfficiencyNorm_  [planeID])->Fill(1);
      THREADED(h2DEfficiencyNorm_[planeID])->Fill(colPredicted,rowPredicted);

      if (data.getHasHit(planeID))
        {
	  THREADED(hEfficiency_  [planeID])->Fill(1);
	  THREADED(h2DEfficiency_[planeID])->Fill(colPredicted,rowPredicted);
        }
    }


  // ##################################################
  // # Compute efficiency for all cells in acceptance #
  // ##################################################
  THREADED(hEfficiencyRefNorm_  [planeID])->Fill(1);
  THREADED(h2DEfficiencyRefNorm_[planeID])->Fill(colPredicted,rowPredicted);
  
  if (data.getHasHit(planeID))
    {
      THREADED(hEfficiencyRef_  [planeID])->Fill(1);
      THREADED(h2DEfficiencyRef_[planeID])->Fill(colPredicted,rowPredicted);
    }
}

//=======================================================================
void EfficiencyUniMiB::cellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  if (!pass || !data.getIsInDetector(planeID)) return;

  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  float         xRes         = data.getXPixelResidualLocal(planeID);
  float         yRes         = data.getYPixelResidualLocal(planeID);
  float         xRes4Cells   = 0;
  float         yRes4Cells   = 0;
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  float         maxPitchX    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  float         maxPitchY    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;


  if (data.getXPixelResidualLocal(planeID) > 0)       xRes4Cells = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
  else if (data.getXPixelResidualLocal(planeID) <= 0) xRes4Cells = data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;

  if (data.getYPixelResidualLocal(planeID) > 0)       yRes4Cells = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
  else if (data.getYPixelResidualLocal(planeID) <= 0) yRes4Cells = data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;

  if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) &&
      data.getXPitchLocal(planeID) <= maxPitchX &&
      data.getYPitchLocal(planeID) <= maxPitchY)
    {
      THREADED(hCellEfficiencyNorm_   [planeID])->Fill(xRes,yRes);
      THREADED(h2D4cellEfficiencyNorm_[planeID])->Fill(xRes4Cells,yRes4Cells);

      if (((int)colPredicted)%2 == 0) THREADED(hCellEfficiencyEvenColumnsNorm_[planeID])->Fill(xRes,yRes);
      else                            THREADED(hCellEfficiencyOddColumnsNorm_ [planeID])->Fill(xRes,yRes);

      if (data.getHasHit(planeID))
	{
	  THREADED(hCellEfficiency_   [planeID])->Fill(xRes,yRes);
	  THREADED(h2D4cellEfficiency_[planeID])->Fill(xRes4Cells,yRes4Cells);
	    
	  if (((int)colPredicted)%2 == 0) THREADED(hCellEfficiencyEvenColumns_[planeID])->Fill(xRes,yRes);
	  else                            THREADED(hCellEfficiencyOddColumns_ [planeID])->Fill(xRes,yRes);
	}
    }
}

//=======================================================================
void EfficiencyUniMiB::xCellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 2;


  if (!pass || !data.getIsInDetector(planeID)) return;

  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  int           event        = data.getEventChewieNumber();
  float         maxPitchX    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  float         xRes         = 0.;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;


  if (data.getXPitchLocal(planeID) == maxPitchX)
    {
      if (data.getXPixelResidualLocal(planeID) > 0)       xRes = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
      else if (data.getXPixelResidualLocal(planeID) <= 0) xRes = data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;
    }
  else return;

  if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) &&
      theWindow->checkTimeWindowAbout(colPredicted,event,run))
    {
      THREADED(h1DXcellEfficiencyNorm_[planeID])->Fill(xRes);
      
      if (data.getHasHit(planeID))
	{
	  if (data.getClusterSize(planeID) > maxClusterSize)
	    {
	      THREADED(h1DXcellEfficiencyFirstHit_ [planeID])->Fill(xRes);
	      THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
	      return;
	    }
	  
	  bool isOk = false;
	  for (int h = 0; h < data.getClusterSize(planeID); h++)
	    {
	      if ((data.getClusterPixelCol(h,planeID) == colPredicted) && (data.getClusterPixelRow(h,planeID) == rowPredicted))
		{
		  THREADED(h1DXcellEfficiencyFirstHit_ [planeID])->Fill(xRes);
		  THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
		  isOk = true;
		  break;
		}
	    }
	  
	  if (isOk) return;
	  else
	    {
	      isOk = false;
	      for (int h = 0; h < data.getClusterSize(planeID); h++)
		{
		  if (data.getClusterPixelRow(h,planeID) == rowPredicted)
		    {
		      if (((colPredicted-data.getClusterPixelCol(h,planeID)) == 1) || ((colPredicted-data.getClusterPixelCol(h,planeID)) == -1))
			{
			  isOk = true;
			  break;
			}
		    }
		}
	      
	      if (!isOk) return;
	      
	      THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
	    }
	}
    }
}

//=======================================================================
void EfficiencyUniMiB::yCellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int maxClusterSize = 2;


  if (!pass || !data.getIsInDetector(planeID)) return;

  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  int           event        = data.getEventChewieNumber();
  float         maxPitchY    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  float         yRes         = 0.;


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;


  if (data.getYPitchLocal(planeID) == maxPitchY)
    {
      if (data.getYPixelResidualLocal(planeID) > 0)       yRes = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
      else if (data.getYPixelResidualLocal(planeID) <= 0) yRes = data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;
    }
  else return;
  
  if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) &&
      theWindow->checkTimeWindowAbout(colPredicted,event,run))
    {
      THREADED(h1DYcellEfficiencyNorm_[planeID])->Fill(yRes);
	
      if (data.getHasHit(planeID))
	{
	  if (data.getClusterSize(planeID) > maxClusterSize)
	    {
	      THREADED(h1DYcellEfficiencyFirstHit_ [planeID])->Fill(yRes);
	      THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
	      return;
	    }
	    
	  bool isOk = false;
	  for (int h = 0; h < data.getClusterSize(planeID); h++)
	    {
	      if ((data.getClusterPixelCol(h,planeID) == colPredicted) && (data.getClusterPixelRow(h,planeID) == rowPredicted))
		{
		  THREADED(h1DYcellEfficiencyFirstHit_ [planeID])->Fill(yRes);
		  THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
		  isOk = true;
		  break;
		}
	    }
	    
	  if (isOk) return;
	  else
	    {
	      isOk = false;
	      for (int h = 0; h < data.getClusterSize(planeID); h++)
		{
		  if (data.getClusterPixelCol(h,planeID) == colPredicted)
		    {
		      if (((rowPredicted-data.getClusterPixelRow(h,planeID)) == 1) || ((rowPredicted-data.getClusterPixelRow(h,planeID)) == -1))
			{
			  isOk = true;
			  break;
			}
		    }
		}
		
	      if (!isOk) return;

	      THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
	    }
	}
    }
}

//=======================================================================
void EfficiencyUniMiB::xEdgeEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int firstRow =  0;
  int lastRow  = 79;


  // if (!pass || !data.getIsInDetector(planeID)) return;
  if (!pass) return;

  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  int           event        = data.getEventChewieNumber();
  float         maxPitchX    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  float         xResRight    = 0.;
  float         xResLeft     = 0.;
  int           clusterSize  = data.getClusterSize(planeID);


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  int lowerCol  = atoi(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getWindow()->getLowerCol()).c_str());
  int higherCol = atoi(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getWindow()->getHigherCol()).c_str());

  for (int h = 0; h < clusterSize; h++)
    {
      if (!(((data.getClusterPixelRow(h,planeID) == lastRow) || (data.getClusterPixelRow(h,planeID) == firstRow)) &&
	    data.getClusterPixelCol(h,planeID) >= lowerCol && data.getClusterPixelCol(h,planeID) <= higherCol)       // Hits are in the window 
	  || !data.getIsPixelCalibrated(h,planeID))                                                                  // Pixels are calibrated
	return;
    }
  

  if (data.getXPitchLocal(planeID) == maxPitchX)
    {
      if (data.getXPixelResidualLocal(planeID) > 0)       xResRight = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
      else if (data.getXPixelResidualLocal(planeID) <= 0) xResLeft  = data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;
    }
  else return;
  
  if (theWindow->checkTimeWindowAbout(colPredicted,event,run))
    {
      if (xResRight != 0) THREADED(h1DXcellEdgeRightEfficiencyNorm_[planeID])->Fill(xResRight);
      else 	          THREADED(h1DXcellEdgeLeftEfficiencyNorm_[planeID])->Fill(xResLeft);

      if (data.getHasHit(planeID))
	{
	  for (int h = 0; h < clusterSize; h++)
	    {
	      if ((data.getClusterPixelRow(h,planeID) == rowPredicted))
		{
		  if (xResRight != 0) THREADED(h1DXcellEdgeRightEfficiency_[planeID])->Fill(xResRight);
		  else                THREADED(h1DXcellEdgeLeftEfficiency_[planeID])->Fill(xResLeft);
		  break;
		}
	    }
	}
    }
}

//=======================================================================
void EfficiencyUniMiB::yEdgeEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int firstCol =  0;
  int lastCol  = 51;


  // if (!pass || !data.getIsInDetector(planeID)) return;
  if (!pass) return;

  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();
  int           event        = data.getEventChewieNumber();
  float         maxPitchY    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  float         yResUp       = 0.;
  float         yResDown     = 0.;
  int           clusterSize  = data.getClusterSize(planeID);


  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  int lowerRow  = atoi(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getWindow()->getLowerRow()).c_str());
  int higherRow = atoi(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getWindow()->getHigherRow()).c_str());

  for (int h = 0; h < clusterSize; h++)
    {
      if (!(((data.getClusterPixelRow(h,planeID) == lastCol) || (data.getClusterPixelRow(h,planeID) == firstCol)) &&
	    data.getClusterPixelRow(h,planeID) >= lowerRow && data.getClusterPixelRow(h,planeID) <= higherRow)       // Hits are in the window 
	  || !data.getIsPixelCalibrated(h,planeID))                                                                  // Pixels are calibrated
	return;
    }


  if (data.getYPitchLocal(planeID) == maxPitchY)
    {
      if (data.getYPixelResidualLocal(planeID) > 0)       yResUp   = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
      else if (data.getYPixelResidualLocal(planeID) <= 0) yResDown = data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;
    }
  else return;
  
  if (theWindow->checkTimeWindowAbout(colPredicted,event,run))
    {
      if (yResUp != 0) THREADED(h1DYcellEdgeUpEfficiencyNorm_[planeID])->Fill(yResUp);
      else 	       THREADED(h1DYcellEdgeDownEfficiencyNorm_[planeID])->Fill(yResDown);

      if (data.getHasHit(planeID))
	{
	  for (int h = 0; h < clusterSize; h++)
	    {
	      if ((data.getClusterPixelCol(h,planeID) == colPredicted))
		{
		  if (yResUp != 0) THREADED(h1DYcellEdgeUpEfficiency_[planeID])->Fill(yResUp);
		  else             THREADED(h1DYcellEdgeDownEfficiency_[planeID])->Fill(yResDown);
		  break;
		}
	    }
	}
    }
}

//=======================================================================
void EfficiencyUniMiB::setCutsFormula(std::map<std::string,std::string> cutsList, std::vector<TTree*> tree)
{
  std::vector<TTreeFormula*> formulasVector;

  for (std::map<std::string,std::string>::iterator it = cutsList.begin(); it != cutsList.end(); it++)
    {
      if ((it->first) == "main cut" && (it->second).size() == 0)
	STDLINE("WARNING: no main cut set in efficiency analysis ! Default value = true !", ACRed);
      
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
bool EfficiencyUniMiB::passStandardCuts(int planeID, const Data &data)
{
  if (!theXmlParser_->getAnalysesFromString("Efficiency")->applyStandardCuts()) return true;
  if (theXmlParser_->getAnalysesFromString("Charge")->excludeBadPlanes())       return passBadPlanesCut(planeID, data);

  int minHits   = atoi(theXmlParser_->getAnalysesFromString("Efficiency")->getMinHits().c_str()) - 1;
  int excludeMe = 0;
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) minHits += 1;
  else if(data.getHasHit(planeID) && data.getClusterSize(planeID) <= 2) excludeMe = 1;

  if (data.getNumberOfTelescopeClustersSizeLE2() - excludeMe >= minHits) return true;
  else                                                                   return false;
}

//=======================================================================
bool EfficiencyUniMiB::passBadPlanesCut (int planeID, const Data &data)
{
  int badPlanesCut = theXmlParser_->getAnalysesFromString("Charge")->getBadPlanesCut();

  int maxNumberOfEvents = 0;
  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes() - 2; p++) // -2 is to exclude DUTs
    {
      HistogramWindow* aWindow = (HistogramWindow*)theAnalysisManager_->getWindowsManager()->getWindow(p);
      if (aWindow->getNumberOfEvents() > maxNumberOfEvents) maxNumberOfEvents = aWindow->getNumberOfEvents();
    }
  
  int minHits   = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str())-1;
  int excludeMe = 0;
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) minHits += 1;
  else if(data.getHasHit(planeID))
    {
      if ((data.getClusterSize(planeID) == 1) || (data.getClusterSize(planeID) == 2 && (data.getClusterPixelRow(0,planeID) == data.getClusterPixelRow(1,planeID)
											|| data.getClusterPixelCol(0,planeID) == data.getClusterPixelCol(1,planeID))))
	excludeMe = 1;
    }
  
  for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes() - 2; p++) // -2 is to exclude DUTs
    {
      HistogramWindow* aWindow = (HistogramWindow*)theAnalysisManager_->getWindowsManager()->getWindow(p);
      if (!data.getHasHit(p) && (float)aWindow->getNumberOfEvents() < (float)maxNumberOfEvents * badPlanesCut / 100) excludeMe += 1;
    }
  
  if (data.getNumberOfTelescopeHits() - excludeMe >= minHits) return true;
  else                                                        return false;
}
