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

#include "EfficiencyAmericans.h"

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
//>> wsi 07/12/17
#include <TProfile.h>
//<< wsi 07/12/17

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>


// @@@ Hard coded parameters @@@
#define ONLYdoubleHITS false // Process only clusters of size 2
#define firstColPitch 300
#define lastColPitch  300
#define firstRowPitch 100
#define lastRowPitch  200
// ============================


//=======================================================================
EfficiencyAmericans::EfficiencyAmericans(AnalysisManager* analysisManager, int nOfThreads) :
  Analysis(analysisManager, nOfThreads),
  thePlaneMapping_(0),
  theWindowsManager_(0),
  theXmlParser_(analysisManager->getXmlParser())
{
  STDLINE("Running EfficiencyAmericans analysis",ACCyan);

  thePlaneMapping_ = new PlanesMapping();
}

//=======================================================================
EfficiencyAmericans::~EfficiencyAmericans(void)
{
  if (thePlaneMapping_) delete thePlaneMapping_;
  
  destroy();
}

//=======================================================================
void EfficiencyAmericans::destroy(void)
{
  if (Analysis::fDoNotDelete_) return;

  std::vector<TH1F*>::iterator it1;
  std::vector<TH2F*>::iterator it2;
  //>> wsi 07/12/17
  std::vector<TProfile*>::iterator it3;
  //<< wsi 07/12/17

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

  //>> wsi 07/12/17
  for (it2=hCellEfficiencyOddEvenColumns_       .begin(); it2!=hCellEfficiencyOddEvenColumns_        .end(); it2++) delete *it2; hCellEfficiencyOddEvenColumns_       .clear();
  for (it2=hCellEfficiencyOddEvenColumnsNorm_   .begin(); it2!=hCellEfficiencyOddEvenColumnsNorm_    .end(); it2++) delete *it2; hCellEfficiencyOddEvenColumnsNorm_   .clear();

  for (it3=hCellEfficiencyOddEvenColumnsProfile_       .begin(); it3!=hCellEfficiencyOddEvenColumnsProfile_        .end(); it3++) delete *it3; hCellEfficiencyOddEvenColumnsProfile_       .clear();
  for (it3=hCellEfficiencyOddEvenColumnsProfileNorm_   .begin(); it3!=hCellEfficiencyOddEvenColumnsProfileNorm_    .end(); it3++) delete *it3; hCellEfficiencyOddEvenColumnsProfileNorm_   .clear();
  //<< wsi 07/12/17

  for (it1=h1DXcellEfficiencyFirstHit_      .begin(); it1!=h1DXcellEfficiencyFirstHit_       .end(); it1++) delete *it1; h1DXcellEfficiencyFirstHit_      .clear();
  for (it1=h1DXcellEfficiencySecondHit_     .begin(); it1!=h1DXcellEfficiencySecondHit_      .end(); it1++) delete *it1; h1DXcellEfficiencySecondHit_     .clear();
  //>> wsi 11/12/17
  for (it1=h1DXcellEfficiencyFourthHit_     .begin(); it1!=h1DXcellEfficiencyFourthHit_      .end(); it1++) delete *it1; h1DXcellEfficiencyFourthHit_     .clear();
  //<< wsi 11/12/17
  for (it1=h1DXcellEfficiencyNorm_          .begin(); it1!=h1DXcellEfficiencyNorm_           .end(); it1++) delete *it1; h1DXcellEfficiencyNorm_	  .clear();

  for (it1=h1DYcellEfficiencyFirstHit_      .begin(); it1!=h1DYcellEfficiencyFirstHit_       .end(); it1++) delete *it1; h1DYcellEfficiencyFirstHit_      .clear();
  for (it1=h1DYcellEfficiencySecondHit_     .begin(); it1!=h1DYcellEfficiencySecondHit_      .end(); it1++) delete *it1; h1DYcellEfficiencySecondHit_     .clear();
  //>> wsi 11/12/17
  for (it1=h1DYcellEfficiencyFourthHit_     .begin(); it1!=h1DYcellEfficiencyFourthHit_      .end(); it1++) delete *it1; h1DYcellEfficiencyFourthHit_     .clear();
  //<< wsi 11/12/17
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
void EfficiencyAmericans::beginJob(void)
{
  theWindowsManager_ = theAnalysisManager_->getWindowsManager();

  std::cout << __PRETTY_FUNCTION__ << " IAM DOING THE NON-AMNERICAN STUFF" << std::endl;
  if (theXmlParser_->getScan()->getScanValues().size() == 0) book();
  else
    {
      STDLINE("Unknown option: scan values",ACRed);
      exit(EXIT_FAILURE);
    }
}

//=======================================================================
void EfficiencyAmericans::analyze(const Data& data, int threadNumber)
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

      //>> wsi 15/12/17
      //if (thePlaneMapping_->getPlaneName(p)=="Dut1")
      //{
      //    int colPredicted = data.getColPredicted(p);
      //    //if (colPredicted<0) return;
      //    if (!((colPredicted+13)%24==0 || (colPredicted+13)%24==1)) return;
      //}
      //<< wsi 15/12/17

      //>> wsi 16/1/18
      if (thePlaneMapping_->getPlaneName(p)=="Dut0" || thePlaneMapping_->getPlaneName(p)=="Dut1")
      {
        int colPredicted = data.getColPredicted(p);
        int rowPredicted = data.getRowPredicted(p);
//        if (thePlaneMapping_->getPlaneName(p)=="Dut1") {std::cout<<"Haha, I found you! ("<<colPredicted<<","<<rowPredicted<<")"<<std::endl;}
        //if (!( ((colPredicted-9)%18 == 7 || (colPredicted-9)%18 == 8 || (colPredicted-9)%18 == 9 || (colPredicted-9)%18 == 10) &&
        //        (rowPredicted%12 == 4 || rowPredicted%12 == 5 || rowPredicted%12 == 6 || rowPredicted%12 == 7) &&
        //        (colPredicted>9) && (rowPredicted<156) ) )
        //{return;}
        //if (thePlaneMapping_->getPlaneName(p)=="Dut1") {std::cout<<"Haha, I found you! ("<<colPredicted<<","<<rowPredicted<<")"<< std::endl;}
      }

      planeEfficiency (passMainCut           ,p,data,threadNumber);
      cellEfficiency  (passCellEfficiencyCut ,p,data,threadNumber);
      xCellEfficiency (passXCellEfficiencyCut,p,data,threadNumber);
      yCellEfficiency (passYCellEfficiencyCut,p,data,threadNumber);
      xEdgeEfficiency (passXCellEfficiencyCut,p,data,threadNumber);
      yEdgeEfficiency (passYCellEfficiencyCut,p,data,threadNumber);
    }
}

//=======================================================================
void EfficiencyAmericans::endJob(void)
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

      //>> wsi 07/12/17
      ADD_THREADED(hCellEfficiencyOddEvenColumns_     [p]);
      ADD_THREADED(hCellEfficiencyOddEvenColumnsNorm_ [p]);

      ADD_THREADED(hCellEfficiencyOddEvenColumnsProfile_     [p]);
      ADD_THREADED(hCellEfficiencyOddEvenColumnsProfileNorm_ [p]);
      //<< wsi 07/12/17

      ADD_THREADED(h1DXcellEfficiencyFirstHit_ 	      [p]);
      ADD_THREADED(h1DXcellEfficiencySecondHit_	      [p]);
      //>> wsi 11/12/17
      ADD_THREADED(h1DXcellEfficiencyFourthHit_	      [p]);
      //<< wsi 11/12/17
      ADD_THREADED(h1DXcellEfficiencyNorm_     	      [p]);

      ADD_THREADED(h1DYcellEfficiencyFirstHit_ 	      [p]);
      ADD_THREADED(h1DYcellEfficiencySecondHit_	      [p]);
      //>> wsi 11/12/17
      ADD_THREADED(h1DYcellEfficiencyFourthHit_	      [p]);
      //<< wsi 11/12/17
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

      hEfficiency_                  [p]->Divide(hEfficiencyNorm_                  [p]);

      h2DEfficiency_                [p]->Divide(h2DEfficiencyNorm_                [p]);

      hEfficiencyRef_               [p]->Divide(hEfficiencyRefNorm_               [p]);

      h2DEfficiencyRef_             [p]->Divide(h2DEfficiencyRefNorm_             [p]);

      h2D4cellEfficiency_           [p]->Divide(h2D4cellEfficiencyNorm_           [p]);

      hCellEfficiency_              [p]->Divide(hCellEfficiencyNorm_              [p]);

      hCellEfficiencyEvenColumns_   [p]->Divide(hCellEfficiencyEvenColumnsNorm_   [p]);

      hCellEfficiencyOddColumns_    [p]->Divide(hCellEfficiencyOddColumnsNorm_    [p]);

      //>> wsi 07/12/17
      hCellEfficiencyOddEvenColumns_[p]->Divide(hCellEfficiencyOddEvenColumnsNorm_[p]);
      hCellEfficiencyOddEvenColumnsProfile_[p]->Divide(hCellEfficiencyOddEvenColumnsProfileNorm_[p]);
      //<< wsi 07/12/17

      h1DXcellEfficiencyFirstHit_   [p]->Divide(h1DXcellEfficiencyNorm_	          [p]);

      h1DXcellEfficiencySecondHit_  [p]->Divide(h1DXcellEfficiencyNorm_	          [p]);

      //>> wsi 11/12/17
      h1DXcellEfficiencyFourthHit_  [p]->Divide(h1DXcellEfficiencyNorm_	          [p]);
      //<< wsi 11/12/17

      h1DYcellEfficiencyFirstHit_   [p]->Divide(h1DYcellEfficiencyNorm_	          [p]);

      h1DYcellEfficiencySecondHit_  [p]->Divide(h1DYcellEfficiencyNorm_	          [p]);

      //>> wsi 11/12/17
      h1DYcellEfficiencyFourthHit_  [p]->Divide(h1DYcellEfficiencyNorm_	          [p]);
      //<< wsi 11/12/17

      for (int i = 0; i < h1DXcellEdgeRightEfficiency_[p]->GetNbinsX(); i++)
	{
	  if (h1DXcellEdgeRightEfficiencyNorm_[p]->GetBinContent(i+1) > 0)
	    h1DXcellEdgeRightEfficiency_[p]->SetBinContent(i+1,h1DXcellEdgeRightEfficiency_[p]->GetBinContent(i+1) / h1DXcellEdgeRightEfficiencyNorm_[p]->GetBinContent(i+1));
	  
	  if (h1DXcellEdgeLeftEfficiencyNorm_[p]->GetBinContent(i+1) > 0)
	    h1DXcellEdgeLeftEfficiency_[p]->SetBinContent(i+1,h1DXcellEdgeLeftEfficiency_[p]->GetBinContent(i+1) / h1DXcellEdgeLeftEfficiencyNorm_[p]->GetBinContent(i+1));
	}

      for (int i = 0; i < h1DYcellEdgeUpEfficiency_[p]->GetNbinsX(); i++)
	{
	  if (h1DYcellEdgeUpEfficiencyNorm_[p]->GetBinContent(i+1) > 0)
	    h1DYcellEdgeUpEfficiency_[p]->SetBinContent(i+1,h1DYcellEdgeUpEfficiency_[p]->GetBinContent(i+1) / h1DYcellEdgeUpEfficiencyNorm_[p]->GetBinContent(i+1));
	  
	  if (h1DYcellEdgeDownEfficiencyNorm_[p]->GetBinContent(i+1) > 0)
	    h1DYcellEdgeDownEfficiency_[p]->SetBinContent(i+1,h1DYcellEdgeDownEfficiency_[p]->GetBinContent(i+1) / h1DYcellEdgeDownEfficiencyNorm_[p]->GetBinContent(i+1));
	}


      // ######################
      // # Setting error bars #
      // ######################
      setErrorsBar(p);


      STDLINE("Setting styles...",ACWhite);

      h1DXcellEfficiencyFirstHit_     [p]->SetMarkerStyle(20);
      h1DXcellEfficiencyFirstHit_     [p]->SetMarkerSize(0.6);

      h1DXcellEfficiencySecondHit_    [p]->SetMarkerStyle(20);
      h1DXcellEfficiencySecondHit_    [p]->SetMarkerSize(0.6);

      h1DXcellEfficiencyFourthHit_    [p]->SetMarkerStyle(20);
      h1DXcellEfficiencyFourthHit_    [p]->SetMarkerSize(0.6);

      h1DYcellEfficiencyFirstHit_     [p]->SetMarkerStyle(20);
      h1DYcellEfficiencyFirstHit_     [p]->SetMarkerSize(0.6);

      h1DYcellEfficiencySecondHit_    [p]->SetMarkerStyle(20);
      h1DYcellEfficiencySecondHit_    [p]->SetMarkerSize(0.6);

      h1DYcellEfficiencyFourthHit_    [p]->SetMarkerStyle(20);
      h1DYcellEfficiencyFourthHit_    [p]->SetMarkerSize(0.6);

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

      h2D4cellEfficiency_             [p]->GetXaxis()->SetTitle("long pitch (um)");
      h2D4cellEfficiency_             [p]->GetYaxis()->SetTitle("short pitch (um)");

      h2D4cellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("long pitch (um)");
      h2D4cellEfficiencyNorm_         [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiency_                [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiency_                [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiencyNorm_            [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyNorm_            [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiencyEvenColumns_     [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyEvenColumns_     [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiencyEvenColumnsNorm_ [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyEvenColumnsNorm_ [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiencyOddColumns_      [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyOddColumns_      [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiencyOddColumnsNorm_  [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyOddColumnsNorm_  [p]->GetYaxis()->SetTitle("short pitch (um)");

      //>> wsi 07/12/17
      hCellEfficiencyOddEvenColumns_      [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyOddEvenColumns_      [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiencyOddEvenColumnsNorm_  [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyOddEvenColumnsNorm_  [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiencyOddEvenColumnsProfile_      [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyOddEvenColumnsProfile_      [p]->GetYaxis()->SetTitle("short pitch (um)");

      hCellEfficiencyOddEvenColumnsProfileNorm_  [p]->GetXaxis()->SetTitle("long pitch (um)");
      hCellEfficiencyOddEvenColumnsProfileNorm_  [p]->GetYaxis()->SetTitle("short pitch (um)");
      //<< wsi 07/12/17

      h1DXcellEfficiencyFirstHit_     [p]->GetXaxis()->SetTitle("long pitch (um)");
      h1DXcellEfficiencyFirstHit_     [p]->GetYaxis()->SetTitle("efficiency");
      h1DXcellEfficiencySecondHit_    [p]->GetXaxis()->SetTitle("long pitch (um)");
      h1DXcellEfficiencySecondHit_    [p]->GetYaxis()->SetTitle("efficiency");
      h1DXcellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("long pitch (um)");

      h1DXcellEdgeRightEfficiency_    [p]->GetXaxis()->SetTitle("long pitch (um)");
      h1DXcellEdgeRightEfficiency_    [p]->GetYaxis()->SetTitle("efficiency");
      h1DXcellEdgeRightEfficiencyNorm_[p]->GetXaxis()->SetTitle("long pitch (um)");

      h1DXcellEdgeLeftEfficiency_     [p]->GetXaxis()->SetTitle("long pitch (um)");
      h1DXcellEdgeLeftEfficiency_     [p]->GetYaxis()->SetTitle("efficiency");
      h1DXcellEdgeLeftEfficiencyNorm_ [p]->GetXaxis()->SetTitle("long pitch (um)");

      h1DYcellEfficiencyFirstHit_     [p]->GetXaxis()->SetTitle("short pitch (um)");
      h1DYcellEfficiencyFirstHit_     [p]->GetYaxis()->SetTitle("efficiency");
      h1DYcellEfficiencySecondHit_    [p]->GetXaxis()->SetTitle("short pitch (um)");
      h1DYcellEfficiencySecondHit_    [p]->GetYaxis()->SetTitle("efficiency");
      h1DYcellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("short pitch (um)");

      h1DYcellEdgeUpEfficiency_       [p]->GetXaxis()->SetTitle("short pitch (um)");
      h1DYcellEdgeUpEfficiency_       [p]->GetYaxis()->SetTitle("efficiency");
      h1DYcellEdgeUpEfficiencyNorm_   [p]->GetXaxis()->SetTitle("short pitch (um)");

      h1DYcellEdgeDownEfficiency_     [p]->GetXaxis()->SetTitle("short pitch (um)");
      h1DYcellEdgeDownEfficiency_     [p]->GetYaxis()->SetTitle("efficiency");
      h1DYcellEdgeDownEfficiencyNorm_ [p]->GetXaxis()->SetTitle("short pitch (um)");


      // ##############################
      // # Print efficiency on screen #
      // ##############################
      efficiency = hEfficiency_    [p]->GetBinContent(1);
      Ntracks    = hEfficiencyNorm_[p]->GetBinContent(1);
      error      = sqrt(efficiency * (1 - efficiency) / Ntracks);

      ss.str("");
      ss << "Detector: " << std::setw(27) << thePlaneMapping_->getPlaneName(p)
      << " Efficiency: " << std::setw(4)  << std::setprecision(7) << efficiency*100.
      << " +- "          << std::setw(4)  << std::setprecision(3) << error*100.
      << " Entries: "    << hEfficiencyRefNorm_[p]->GetBinContent(1);

      STDLINE(ss.str(),ACLightPurple);
    }
}

//=======================================================================
void EfficiencyAmericans::book(void)
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
      theAnalysisManager_->cd("Efficiency");
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

      //>> wsi 11/12/17
      hName  = "h1DXcellEfficiencyFourthHit_"                           + planeName;
      hTitle = "1D cell Efficiency - X coordinate fourth hit "          + planeName;
      h1DXcellEfficiencyFourthHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));
      //<< wsi 11/12/17

      hName  = "h1DXcellEfficiencyNorm_"                                + planeName;
      hTitle = "1D cell Efficiency - X coordinate normalization "       + planeName;
      h1DXcellEfficiencyNorm_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));


      hName  = "h1DXcellEdgeRightEfficiency_"                           + planeName;
      hTitle = "1D edge-right Efficiency - X coordinate "               + planeName;
      h1DXcellEdgeRightEfficiency_.push_back    (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)2*lastColPitch/binSize,-lastColPitch,lastColPitch)));

      hName  = "h1DXcellEdgeRightEfficiencyNorm_"                       + planeName;
      hTitle = "1D edge-right Efficiency - X coordinate normalization " + planeName;
      h1DXcellEdgeRightEfficiencyNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)2*lastColPitch/binSize,-lastColPitch,lastColPitch)));


      hName  = "h1DXcellEdgeLeftEfficiency_"                            + planeName;
      hTitle = "1D edge-left Efficiency - X coordinate "                + planeName;
      h1DXcellEdgeLeftEfficiency_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)2*firstColPitch/binSize,-firstColPitch,firstColPitch)));

      hName  = "h1DXcellEdgeLeftEfficiencyNorm_"                        + planeName;
      hTitle = "1D edge-left Efficiency - X coordinate normalization "  + planeName;
      h1DXcellEdgeLeftEfficiencyNorm_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)2*firstColPitch/binSize,-firstColPitch,firstColPitch)));


      hName  = "h1DYcellEfficiencyFirstHit_"                            + planeName;
      hTitle = "1D cell Efficiency - Y coordinate first hit "           + planeName;
      h1DYcellEfficiencyFirstHit_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "h1DYcellEfficiencySecondHit_"                           + planeName;
      hTitle = "1D cell Efficiency - Y coordinate second hit "          + planeName;
      h1DYcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      //>> wsi 11/12/17
      hName  = "h1DYcellEfficiencyFourthHit_"                           + planeName;
      hTitle = "1D cell Efficiency - Y coordinate fourth hit "          + planeName;
      h1DYcellEfficiencyFourthHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch/2,yPitch/2)));
      //<< wsi 11/12/17

      hName  = "h1DYcellEfficiencyNorm_"                                + planeName;
      hTitle = "1D cell Efficiency - Y coordinate normalization "       + planeName;
      h1DYcellEfficiencyNorm_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize,-yPitch/2,yPitch/2)));


      hName  = "h1DYcellEdgeUpEfficiency_"                              + planeName;
      hTitle = "1D edge-up Efficiency - Y coordinate "                  + planeName;
      h1DYcellEdgeUpEfficiency_.push_back       (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)2*lastRowPitch/binSize,-lastRowPitch,lastRowPitch)));

      hName  = "h1DYcellEdgeUpEfficiencyNorm_"                          + planeName;
      hTitle = "1D edge-up Efficiency - Y coordinate normalization "    + planeName;
      h1DYcellEdgeUpEfficiencyNorm_.push_back   (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)2*lastRowPitch/binSize,-lastRowPitch,lastRowPitch)));


      hName  = "h1DYcellEdgeDownEfficiency_"                            + planeName;
      hTitle = "1D edge-down Efficiency - Y coordinate "                + planeName;
      h1DYcellEdgeDownEfficiency_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)2*firstRowPitch/binSize,-firstRowPitch,firstRowPitch)));

      hName  = "h1DYcellEdgeDownEfficiencyNorm_"                        + planeName;
      hTitle = "1D edge-down Efficiency - Y coordinate normalization "  + planeName;
      h1DYcellEdgeDownEfficiencyNorm_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)2*firstRowPitch/binSize,-firstRowPitch,firstRowPitch)));


      // #################
      // # 2D histograms #
      // #################
      hName  = "2DEfficiency_"                                          + planeName;
      hTitle = "2D efficiency distribution "                            + planeName;
      h2DEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

      hName  = "2DEfficiencyNorm_"                                      + planeName;
      hTitle = "2D efficiency normalization "                           + planeName;
      h2DEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));


      hName  = "2DEfficiencyRef_"                                       + planeName;
      hTitle = "2D efficiency distribution ref. "                       + planeName;
      h2DEfficiencyRef_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

      hName  = "2DEfficiencyRefNorm_"                                   + planeName;
      hTitle = "2D efficiency ref. normalization "                      + planeName;
      h2DEfficiencyRefNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));




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

      //>> wsi 07/12/17
      hName  = "hCellEfficiencyOddEvenColumns_"                         + planeName;
      hTitle = "Cell efficiency odd + even columns "                    + planeName;
      hCellEfficiencyOddEvenColumns_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),2*(int)xPitch/binSize,-xPitch,xPitch,(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "hCellEfficiencyOddEvenColumnsNorm_"                     + planeName;
      hTitle = "Cell efficiency normalization odd + even columns "      + planeName;
      hCellEfficiencyOddEvenColumnsNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),2*(int)xPitch/binSize,-xPitch,xPitch,(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "hCellEfficiencyOddEvenColumnsProfile_"                         + planeName;
      hTitle = "Cell efficiency odd + even columns profile"                    + planeName;
      hCellEfficiencyOddEvenColumnsProfile_.push_back(NEW_THREADED(TProfile(hName.c_str(),hTitle.c_str(),2*(int)xPitch/binSize,-xPitch,xPitch,-yPitch/2,yPitch/2)));

      hName  = "hCellEfficiencyOddEvenColumnsProfileNorm_"                     + planeName;
      hTitle = "Cell efficiency normalization odd + even columns profile "      + planeName;
      hCellEfficiencyOddEvenColumnsProfileNorm_.push_back(NEW_THREADED(TProfile(hName.c_str(),hTitle.c_str(),2*(int)xPitch/binSize,-xPitch,xPitch,-yPitch/2,yPitch/2)));
      //<< wsi 07/12/17

      hName  = "h2D4cellEfficiency_"                                    + planeName;
      hTitle = "4 cell efficiency "                                     + planeName;
      h2D4cellEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));

      hName  = "h2D4cellEfficiencyNorm_"                                + planeName;
      hTitle = "4 cell efficiency normalization "                       + planeName;
      h2D4cellEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2,(int)yPitch/binSize,-yPitch/2,yPitch/2)));
    }
}

//=======================================================================
void EfficiencyAmericans::setErrorsBar(int planeID)
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
	if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
	else            error = 0;
        h1DXcellEfficiencyFirstHit_[planeID]->SetBinError(b,error);

        efficiency = h1DXcellEfficiencySecondHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEfficiencyNorm_[planeID]->GetBinContent(b);
	if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
	else            error = 0;
        h1DXcellEfficiencySecondHit_[planeID]->SetBinError(b,error);

        //>> wsi 11/12/17
        efficiency = h1DXcellEfficiencyFourthHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEfficiencyNorm_[planeID]->GetBinContent(b);
    if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
    else            error = 0;
        h1DXcellEfficiencyFourthHit_[planeID]->SetBinError(b,error);
        //<< wsi 11/12/17
    }

    nBins = h1DXcellEdgeRightEfficiency_[planeID]->GetNbinsX();
    for(int b = 1; b <= nBins; b++)
    {
        efficiency = h1DXcellEdgeRightEfficiency_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEdgeRightEfficiencyNorm_[planeID]->GetBinContent(b);
	if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
	else            error = 0;
        h1DXcellEdgeRightEfficiency_[planeID]->SetBinError(b,error);

        efficiency = h1DXcellEdgeLeftEfficiency_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEdgeLeftEfficiencyNorm_[planeID]->GetBinContent(b);
	if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
	else            error = 0;
        h1DXcellEdgeLeftEfficiency_[planeID]->SetBinError(b,error);
    }

    nBins = h1DYcellEfficiencyFirstHit_[planeID]->GetNbinsX();
    for(int b = 1; b <= nBins; b++)
    {
        efficiency = h1DYcellEfficiencyFirstHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEfficiencyNorm_[planeID]->GetBinContent(b);
	if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
	else            error = 0;
        h1DYcellEfficiencyFirstHit_[planeID]->SetBinError(b,error);

        efficiency = h1DYcellEfficiencySecondHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEfficiencyNorm_[planeID]->GetBinContent(b);
	if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
	else            error = 0;
        h1DYcellEfficiencySecondHit_[planeID]->SetBinError(b,error);

        //>> wsi 11/12/17
        efficiency = h1DYcellEfficiencyFourthHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEfficiencyNorm_[planeID]->GetBinContent(b);
    if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
    else            error = 0;
        h1DYcellEfficiencyFourthHit_[planeID]->SetBinError(b,error);
        //<< wsi 11/12/17
    }

    nBins = h1DYcellEdgeUpEfficiency_[planeID]->GetNbinsX();
    for(int b = 1; b <= nBins; b++)
    {
        efficiency = h1DYcellEdgeUpEfficiency_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEdgeUpEfficiencyNorm_[planeID]->GetBinContent(b);
	if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
	else            error = 0;
        h1DYcellEdgeUpEfficiency_[planeID]->SetBinError(b,error);

        efficiency = h1DYcellEdgeDownEfficiency_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEdgeDownEfficiencyNorm_[planeID]->GetBinContent(b);
	if (Ntrack > 0) error = sqrt(efficiency * (1-efficiency) / Ntrack);
	else            error = 0;
        h1DYcellEdgeDownEfficiency_[planeID]->SetBinError(b,error);
    }
}

//=======================================================================
void EfficiencyAmericans::planeEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  if (!pass || !data.getIsInDetector(planeID)) return;

  const Window* theWindow    = theWindowsManager_->getWindow(planeID);
  int           rowPredicted = data.getRowPredicted(planeID);
  int           colPredicted = data.getColPredicted(planeID);
  int           run          = data.getRunNumber();

  //if (/*thePlaneMapping_->getPlaneName(planeID)=="Dut0" ||*/ thePlaneMapping_->getPlaneName(planeID)=="Dut1")
  //{std::cout<<"WSi ("<<colPredicted<<","<<rowPredicted<<") "<<thePlaneMapping_->getPlaneName(planeID)<<std::endl;}
  // #########################################
  // # Check if track and hits are in window #
  // #########################################
  if (!theWindow->checkWindow(colPredicted,rowPredicted,run)) return;

  //>> wsi We keep only what we want
  //>> wsi 16/1/18: no longer needed
  //if (thePlaneMapping_->getPlaneName(planeID)=="Dut1" &&
  //        (colPredicted%4==2 || colPredicted%4==1) )
  //    return;
  //<< wsi 07/12/17
//  if (/*thePlaneMapping_->getPlaneName(planeID)=="Dut0" ||*/ thePlaneMapping_->getPlaneName(planeID)=="Dut1")
//  {std::cout<<"WSi ("<<colPredicted<<","<<rowPredicted<<") "<<thePlaneMapping_->getPlaneName(planeID)<<std::endl;}

  // #########################################################################
  // # Compute efficiency only for cells that are surrounded by "good" cells #
  // #########################################################################
  if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)/*,
                                       thePlaneMapping_->getPlaneName(planeID)=="Dut1"*/))
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
void EfficiencyAmericans::cellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
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

  //>> wsi 11/12/17
//  if (thePlaneMapping_->getPlaneName(planeID)=="Dut1")
//  {
//      if (colPredicted%4==3)
//      {
//          xRes4Cells = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
//          if (rowPredicted%2==1)
//              yRes4Cells = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
//          else
//              yRes4Cells = data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;

//      }
//      else if (colPredicted%4==0)
//      {
//          xRes4Cells = data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;
//          if (rowPredicted%2==1)
//              yRes4Cells = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
//          else
//              yRes4Cells = data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;
//      }
//  }
  //<< wsi 11/12/17

  if (
      theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)/*,
                                  thePlaneMapping_->getPlaneName(planeID)=="Dut1"*/) &&
      data.getXPitchLocal(planeID) <= maxPitchX &&
      data.getYPitchLocal(planeID) <= maxPitchY)
    {
      THREADED(hCellEfficiencyNorm_   [planeID])->Fill(xRes,yRes);
      THREADED(h2D4cellEfficiencyNorm_[planeID])->Fill(xRes4Cells,yRes4Cells);

      //>> wsi 07/12/17
      if (((int)colPredicted)%2 == 0) {
          THREADED(hCellEfficiencyEvenColumnsNorm_[planeID])->Fill(xRes,yRes);
          THREADED(hCellEfficiencyOddEvenColumnsNorm_[planeID])->Fill(xRes+data.getXPitchLocal(planeID)/2,yRes);
          THREADED(hCellEfficiencyOddEvenColumnsProfileNorm_[planeID])->Fill(xRes+data.getXPitchLocal(planeID)/2,yRes);
      }
      else
      {
          THREADED(hCellEfficiencyOddColumnsNorm_[planeID])->Fill(xRes,yRes);
          THREADED(hCellEfficiencyOddEvenColumnsNorm_[planeID])->Fill(xRes-data.getXPitchLocal(planeID)/2,yRes);
          THREADED(hCellEfficiencyOddEvenColumnsProfileNorm_[planeID])->Fill(xRes-data.getXPitchLocal(planeID)/2,yRes);
      }
      //<< wsi 07/12/17


      if (data.getHasHit(planeID))
	{
	  THREADED(hCellEfficiency_   [planeID])->Fill(xRes,yRes);
	  THREADED(h2D4cellEfficiency_[planeID])->Fill(xRes4Cells,yRes4Cells);

      //>> wsi 07/12/17
      if (((int)colPredicted)%2 == 0) {
          THREADED(hCellEfficiencyEvenColumns_[planeID])->Fill(xRes,yRes);
          THREADED(hCellEfficiencyOddEvenColumns_[planeID])->Fill(xRes+data.getXPitchLocal(planeID)/2,yRes);
          THREADED(hCellEfficiencyOddEvenColumnsProfile_[planeID])->Fill(xRes+data.getXPitchLocal(planeID)/2,yRes);
      }
      else
      {
          THREADED(hCellEfficiencyOddColumns_[planeID])->Fill(xRes,yRes);
          THREADED(hCellEfficiencyOddEvenColumns_[planeID])->Fill(xRes-data.getXPitchLocal(planeID)/2,yRes);
          THREADED(hCellEfficiencyOddEvenColumnsProfile_[planeID])->Fill(xRes-data.getXPitchLocal(planeID)/2,yRes);
      }
      //<< wsi 07/12/17

	}
    }
}

//=======================================================================
void EfficiencyAmericans::xCellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
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
      if      (data.getXPixelResidualLocal(planeID) > 0)  xRes = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
      else if (data.getXPixelResidualLocal(planeID) <= 0) xRes = data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;

      //>> wsi 11/12/17
//      if (thePlaneMapping_->getPlaneName(planeID)=="Dut1")
//      {
//          if (colPredicted%4==3)
//              xRes = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
//          else if (colPredicted%4==0)
//              xRes = data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;
//      }
      //<< wsi 11/12/17
    }
  else return;

  if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)/*,
                                  thePlaneMapping_->getPlaneName(planeID)=="Dut1"*/) &&
      theWindow->checkTimeWindowAbout(colPredicted,event,run/*,
                                             thePlaneMapping_->getPlaneName(planeID)=="Dut1"*/))
    {
      THREADED(h1DXcellEfficiencyNorm_[planeID])->Fill(xRes);
      
      if (data.getHasHit(planeID))
	{
      if (data.getClusterSize(planeID) >= maxClusterSize)
	    {
	      THREADED(h1DXcellEfficiencyFirstHit_ [planeID])->Fill(xRes);
	      THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
          THREADED(h1DXcellEfficiencyFourthHit_[planeID])->Fill(xRes);
	      return;
	    }
	  
	  bool isOk = false;
	  for (int h = 0; h < data.getClusterSize(planeID); h++)
	    {
	      if ((data.getClusterPixelCol(h,planeID) == colPredicted) && (data.getClusterPixelRow(h,planeID) == rowPredicted))
          {
              THREADED(h1DXcellEfficiencyFirstHit_ [planeID])->Fill(xRes);
              THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
              THREADED(h1DXcellEfficiencyFourthHit_[planeID])->Fill(xRes);
              isOk = true;
              break;
          }
	    }
	  
	  if (isOk) return;
	  else
	    {
	      isOk = false;

          //>> wsi 11/12/17
          for (int h = 0; h < data.getClusterSize(planeID); h++)
          {
              if ( (abs(rowPredicted-data.getClusterPixelRow(h,planeID)) + abs(colPredicted-data.getClusterPixelCol(h,planeID)) == 1) ||
                   (abs(rowPredicted-data.getClusterPixelRow(h,planeID)) == 1 && abs(colPredicted-data.getClusterPixelCol(h,planeID)) == 1))
              {
                  THREADED(h1DXcellEfficiencyFourthHit_[planeID])->Fill(xRes);
                  if (data.getClusterPixelRow(h,planeID) == rowPredicted)
                  {
                      THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
                      break;
                  }
                  break;
              }
          }
          //<< wsi 11/12/17
	    }
	}
    }
}

//=======================================================================
void EfficiencyAmericans::yCellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
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
      //>> wsi 11/12/17
//      if (thePlaneMapping_->getPlaneName(planeID)=="Dut1")
//      {
//          if (rowPredicted%2==1)
//              yRes = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
//          else if (rowPredicted%2==0)
//              yRes = data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;
//      }
      //<< wsi 11/12/17
    }
  else return;
  
  if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)/*,
                                  thePlaneMapping_->getPlaneName(planeID)=="Dut1"*/) &&
      theWindow->checkTimeWindowAbout(colPredicted,event,run))
    {
      THREADED(h1DYcellEfficiencyNorm_[planeID])->Fill(yRes);
	
      if (data.getHasHit(planeID))
	{
      if (data.getClusterSize(planeID) >= maxClusterSize)
	    {
	      THREADED(h1DYcellEfficiencyFirstHit_ [planeID])->Fill(yRes);
	      THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
          THREADED(h1DYcellEfficiencyFourthHit_[planeID])->Fill(yRes);
	      return;
	    }
	    
      bool isOk = false;
      for (int h = 0; h < data.getClusterSize(planeID); h++)
        {
          if ((data.getClusterPixelCol(h,planeID) == colPredicted) && (data.getClusterPixelRow(h,planeID) == rowPredicted))
        {
          THREADED(h1DYcellEfficiencyFirstHit_ [planeID])->Fill(yRes);
          THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
          THREADED(h1DYcellEfficiencyFourthHit_[planeID])->Fill(yRes);
          isOk = true;
          break;
        }
        }

      if (isOk) return;
      else
        {
          isOk = false;

          //>> wsi 11/12/17
          for (int h = 0; h < data.getClusterSize(planeID); h++)
          {
              if ((abs(rowPredicted-data.getClusterPixelRow(h,planeID)) + abs(colPredicted-data.getClusterPixelCol(h,planeID)) == 1) ||
                  (abs(rowPredicted-data.getClusterPixelRow(h,planeID)) == 1 && abs(colPredicted-data.getClusterPixelCol(h,planeID)) == 1))
              {
                  THREADED(h1DYcellEfficiencyFourthHit_[planeID])->Fill(yRes);
                  if (data.getClusterPixelCol(h,planeID) == colPredicted)
                  {
                      THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
                      break;
                  }
                  break;
              }
          }
          //<< wsi 11/12/17
        }
	}
    }
}

//=======================================================================
void EfficiencyAmericans::xEdgeEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int firstRow =  1;
  int lastRow  = 78;
  int firstCol =  0;
  int lastCol  = 51;
  int maxClusterSize = 4;
  
  
  if ((!pass) || (data.getClusterSize(planeID) > maxClusterSize)) return;
  
  int   rowPredicted = data.getRowPredicted(planeID);
  float maxPitchX    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  float maxPitchY    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  float xResRight    = 0.;
  float xResLeft     = 0.;
  int   clusterSize  = data.getClusterSize(planeID);
  
  
  // #################################
  // # Check if tracks are in window #
  // #################################
  if (((data.getYPredictedLocal(planeID) > firstRow*maxPitchY) && (data.getYPredictedLocal(planeID) < (lastRow+1)*maxPitchY)) &&
      ((data.getXPredictedLocal(planeID) < firstColPitch + maxPitchX/2.) || (data.getXPredictedLocal(planeID) > firstColPitch + (lastCol-2)*maxPitchX + maxPitchX/2.)))
    {
      if (data.getXPredictedLocal(planeID) < firstColPitch + maxPitchX/2.)
	{
	  xResLeft = data.getXPredictedLocal(planeID);
	  THREADED(h1DXcellEdgeLeftEfficiencyNorm_[planeID])->Fill(xResLeft);
	}
      
      else
	{
	  xResRight = data.getXPredictedLocal(planeID) - (firstColPitch + (lastCol-1)*maxPitchX + lastColPitch);
	  THREADED(h1DXcellEdgeRightEfficiencyNorm_[planeID])->Fill(xResRight);
	}
      
      
      // ###############################
      // # Check if hits are in window #
      // ###############################
      std::string clusterPosition = "none";
      for (int h = 0; h < clusterSize; h++)
	{
	  if ((data.getClusterPixelRow(h,planeID) >= firstRow) && (data.getClusterPixelRow(h,planeID) <= lastRow))
	    {
	      if      ((data.getClusterPixelCol(h,planeID) == firstCol) || (data.getClusterPixelCol(h,planeID) == firstCol+1)) clusterPosition = "first";
	      else if ((data.getClusterPixelCol(h,planeID) == lastCol)  || (data.getClusterPixelCol(h,planeID) == lastCol-1))  clusterPosition = "last";
	    }
	}
      
      if (clusterPosition == "none") return;
      
      if (data.getHasHit(planeID))
	{
	  for (int h = 0; h < clusterSize; h++)
	    {
	      if ((data.getClusterPixelRow(h,planeID) == rowPredicted) &&
		  (((xResRight != 0.) && (clusterPosition == "last")) || ((xResLeft != 0.) && (clusterPosition == "first"))))
		{
		  if (xResRight != 0.) THREADED(h1DXcellEdgeRightEfficiency_[planeID])->Fill(xResRight);
		  else                 THREADED(h1DXcellEdgeLeftEfficiency_[planeID])->Fill(xResLeft);
		  break;
		}
	    }
	}
    }
}

//=======================================================================
void EfficiencyAmericans::yEdgeEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
  // #####################
  // # Internal constant #
  // #####################
  int firstRow =  0;
  int lastRow  = 79;
  int firstCol =  1;
  int lastCol  = 50;
  int maxClusterSize = 4;
  
  
  if ((!pass) || (data.getClusterSize(planeID) > maxClusterSize)) return;
  
  int   colPredicted = data.getColPredicted(planeID);
  float maxPitchX    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
  float maxPitchY    = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
  float yResUp       = 0.;
  float yResDown     = 0.;
  int   clusterSize  = data.getClusterSize(planeID);
  
  
  // #################################
  // # Check if tracks are in window #
  // #################################
  if (((data.getXPredictedLocal(planeID) > firstCol*maxPitchX) && (data.getXPredictedLocal(planeID) < (lastCol+1)*maxPitchX)) &&
      ((data.getYPredictedLocal(planeID) < firstRowPitch + maxPitchY/2.) || (data.getYPredictedLocal(planeID) > firstRowPitch + (lastRow-2)*maxPitchY + maxPitchY/2.)))
    {
      if (data.getYPredictedLocal(planeID) < firstRowPitch + maxPitchY/2.)
	{
	  yResDown = data.getYPredictedLocal(planeID);
	  THREADED(h1DYcellEdgeDownEfficiencyNorm_[planeID])->Fill(yResDown);
	}
      
      else
	{
	  yResUp = data.getYPredictedLocal(planeID) - (firstRowPitch + (lastRow-1)*maxPitchY + lastRowPitch);
	  THREADED(h1DYcellEdgeUpEfficiencyNorm_[planeID])->Fill(yResUp);
	}
      
      
      // ###############################
      // # Check if hits are in window #
      // ###############################
      std::string clusterPosition = "none";
      for (int h = 0; h < clusterSize; h++)
	{
	  if ((data.getClusterPixelCol(h,planeID) >= firstCol) && (data.getClusterPixelCol(h,planeID) <= lastCol))
	    {
	      if      ((data.getClusterPixelRow(h,planeID) == firstRow) || (data.getClusterPixelRow(h,planeID) == firstRow+1)) clusterPosition = "first";
	      else if ((data.getClusterPixelRow(h,planeID) == lastRow)  || (data.getClusterPixelRow(h,planeID) == lastRow-1))  clusterPosition = "last";
	    }
	}
      
      if (clusterPosition == "none") return;
      
      if (data.getHasHit(planeID))
	{
	  for (int h = 0; h < clusterSize; h++)
	    {
	      if ((data.getClusterPixelCol(h,planeID) == colPredicted) &&
		  (((yResUp != 0.) && (clusterPosition == "last")) || ((yResDown != 0.) && (clusterPosition == "first"))))
		{
		  if (yResUp != 0.) THREADED(h1DYcellEdgeUpEfficiency_[planeID])->Fill(yResUp);
		  else              THREADED(h1DYcellEdgeDownEfficiency_[planeID])->Fill(yResDown);
		  break;
		}
	    }
	}
    }
}

//=======================================================================
void EfficiencyAmericans::setCutsFormula(std::map<std::string,std::string> cutsList, std::vector<TTree*> tree)
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
bool EfficiencyAmericans::passStandardCuts(int planeID, const Data &data)
{
  if (!theXmlParser_->getAnalysesFromString("Efficiency")->applyStandardCuts()) return true;

  int minHits   = atoi(theXmlParser_->getAnalysesFromString("Efficiency")->getMinHits().c_str()) - 1;
  int excludeMe = 0;
  if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) minHits += 1;
  else if (data.getHasHit(planeID) && data.getClusterSize(planeID) <= 2) excludeMe = 1;

  if (data.getNumberOfTelescopeHits() - excludeMe >= minHits) return true;
  else                                                        return false;
}
