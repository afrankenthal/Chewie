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

#include "EfficiencyOuterTracker.h"

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
#define ONLYdoubleHITS true // Process only clusters of size 2


//=======================================================================
EfficiencyOuterTracker::EfficiencyOuterTracker(AnalysisManager* analysisManager, int nOfThreads) :
    Analysis(analysisManager, nOfThreads),
    thePlaneMapping_(0),
    theWindowsManager_(0),
    theXmlParser_(analysisManager->getXmlParser())
{
    STDLINE("Running EfficiencyOuterTracker analysis",ACCyan);

    thePlaneMapping_ = new PlanesMapping();
}

//=======================================================================
EfficiencyOuterTracker::~EfficiencyOuterTracker(void)
{
    if (thePlaneMapping_) delete thePlaneMapping_;

    destroy();
}

//=======================================================================
void EfficiencyOuterTracker::destroy(void)
{
    if (Analysis::fDoNotDelete_) return;

    std::vector<TH1F*>::iterator it1;
    std::vector<TH2F*>::iterator it2;

    for (it1=hEfficiency_                     .begin(); it1!=hEfficiency_  	      	         .end(); it1++) delete *it1; hEfficiency_		              .clear();
    for (it1=hEfficiencyNorm_                 .begin(); it1!=hEfficiencyNorm_	      	     .end(); it1++) delete *it1; hEfficiencyNorm_		          .clear();

    for (it1=hEfficiencyRef_                  .begin(); it1!=hEfficiencyRef_  	      	     .end(); it1++) delete *it1; hEfficiencyRef_	              .clear();
    for (it1=hEfficiencyRefNorm_              .begin(); it1!=hEfficiencyRefNorm_	      	 .end(); it1++) delete *it1; hEfficiencyRefNorm_	          .clear();

    for (it1=hEfficiencyStub_                 .begin(); it1!=hEfficiencyStub_	      	     .end(); it1++) delete *it1; hEfficiencyStub_                 .clear();
    for (it1=hEfficiencyStubNorm_             .begin(); it1!=hEfficiencyStubNorm_	         .end(); it1++) delete *it1; hEfficiencyStubNorm_ 	          .clear();

    for (it1=hEfficiencyStubRef_              .begin(); it1!=hEfficiencyStubRef_	      	 .end(); it1++) delete *it1; hEfficiencyStubRef_              .clear();
    for (it1=hEfficiencyStubRefNorm_          .begin(); it1!=hEfficiencyStubRefNorm_	     .end(); it1++) delete *it1; hEfficiencyStubRefNorm_ 	      .clear();

    //nabin
    for (it2=h2DClusterPositionLocal_         .begin(); it2!=h2DClusterPositionLocal_	     .end(); it2++) delete *it2; h2DClusterPositionLocal_		  .clear();

    for (it2=h2DEfficiency_                   .begin(); it2!=h2DEfficiency_	      	         .end(); it2++) delete *it2; h2DEfficiency_		              .clear();
    for (it2=h2DEfficiencyNorm_               .begin(); it2!=h2DEfficiencyNorm_	      	     .end(); it2++) delete *it2; h2DEfficiencyNorm_ 	          .clear();

    for (it2=h2DEfficiencyRef_                .begin(); it2!=h2DEfficiencyRef_	      	     .end(); it2++) delete *it2; h2DEfficiencyRef_                .clear();
    for (it2=h2DEfficiencyRefNorm_            .begin(); it2!=h2DEfficiencyRefNorm_	         .end(); it2++) delete *it2; h2DEfficiencyRefNorm_ 	          .clear();

    for (it2=h2DEfficiencyStub_               .begin(); it2!=h2DEfficiencyStub_	      	      .end(); it2++) delete *it2; h2DEfficiencyStub_              .clear();
    for (it2=h2DEfficiencyStubNorm_           .begin(); it2!=h2DEfficiencyStubNorm_	          .end(); it2++) delete *it2; h2DEfficiencyStubNorm_ 	      .clear();

    for (it2=h2DEfficiencyStubRef_            .begin(); it2!=h2DEfficiencyStubRef_	      	  .end(); it2++) delete *it2; h2DEfficiencyStubRef_           .clear();
    for (it2=h2DEfficiencyStubRefNorm_        .begin(); it2!=h2DEfficiencyStubRefNorm_	      .end(); it2++) delete *it2; h2DEfficiencyStubRefNorm_ 	  .clear();

    for (it2=h2DFakeHitsEfficiency_           .begin(); it2!=h2DFakeHitsEfficiency_	      	  .end(); it2++) delete *it2; h2DFakeHitsEfficiency_		  .clear();
    for (it2=h2DFakeHitsEfficiencyNorm_       .begin(); it2!=h2DFakeHitsEfficiencyNorm_	      .end(); it2++) delete *it2; h2DFakeHitsEfficiencyNorm_ 	  .clear();

    for (it2=h2D4cellEfficiency_              .begin(); it2!=h2D4cellEfficiency_	      	  .end(); it2++) delete *it2; h2D4cellEfficiency_	          .clear();
    for (it2=h2D4cellEfficiencyNorm_          .begin(); it2!=h2D4cellEfficiencyNorm_          .end(); it2++) delete *it2; h2D4cellEfficiencyNorm_	      .clear();

    for (it2=hCellEfficiency_                 .begin(); it2!=hCellEfficiency_	      	      .end(); it2++) delete *it2; hCellEfficiency_	              .clear();
    for (it2=hCellEfficiencyNorm_             .begin(); it2!=hCellEfficiencyNorm_              .end(); it2++) delete *it2; hCellEfficiencyNorm_           .clear();

    for (it2=hCellEfficiencyEvenColumns_      .begin(); it2!=hCellEfficiencyEvenColumns_       .end(); it2++) delete *it2; hCellEfficiencyEvenColumns_     .clear();
    for (it2=hCellEfficiencyEvenColumnsNorm_  .begin(); it2!=hCellEfficiencyEvenColumnsNorm_   .end(); it2++) delete *it2; hCellEfficiencyEvenColumnsNorm_ .clear();

    for (it2=hCellEfficiencyOddColumns_       .begin(); it2!=hCellEfficiencyOddColumns_        .end(); it2++) delete *it2; hCellEfficiencyOddColumns_       .clear();
    for (it2=hCellEfficiencyOddColumnsNorm_   .begin(); it2!=hCellEfficiencyOddColumnsNorm_    .end(); it2++) delete *it2; hCellEfficiencyOddColumnsNorm_   .clear();

    for (it1=h1DXcellEfficiencyFirstHit_      .begin(); it1!=h1DXcellEfficiencyFirstHit_       .end(); it1++) delete *it1; h1DXcellEfficiencyFirstHit_      .clear();
    for (it1=h1DXcellEfficiencySecondHit_     .begin(); it1!=h1DXcellEfficiencySecondHit_      .end(); it1++) delete *it1; h1DXcellEfficiencySecondHit_     .clear();
    for (it1=h1DXcellEfficiencyNorm_          .begin(); it1!=h1DXcellEfficiencyNorm_           .end(); it1++) delete *it1; h1DXcellEfficiencyNorm_	        .clear();

    for (it1=h1DclusterSizes_                   .begin(); it1!=h1DclusterSizes_                .end(); it1++) delete *it1; h1DclusterSizes_              	.clear();
    for (it1=h1DclusterSizeVsXStripPosition_    .begin(); it1!=h1DclusterSizeVsXStripPosition_ .end(); it1++) delete *it1; h1DclusterSizeVsXStripPosition_ 	.clear();
    for (it1=h1DclusterSizeVsXStripPosition_norm_    .begin(); it1!=h1DclusterSizeVsXStripPosition_norm_ .end(); it1++) delete *it1; h1DclusterSizeVsXStripPosition_norm_ 	.clear();

    for (it1=hAverageHitsperEvent_              .begin(); it1!=hAverageHitsperEvent_           .end(); it1++) delete *it1; hAverageHitsperEvent_            .clear();
    for (it1=TotalEvent_                        .begin(); it1!=TotalEvent_                     .end(); it1++) delete *it1; TotalEvent_                      .clear();
    for (it1=checkineff_                        .begin(); it1!=checkineff_                     .end(); it1++) delete *it1; checkineff_                      .clear();
    for (it1=diffFirstSecondHist_               .begin(); it1!=diffFirstSecondHist_            .end(); it1++) delete *it1; diffFirstSecondHist_             .clear();
    for (it1=dut23stub_                         .begin(); it1!=dut23stub_                      .end(); it1++) delete *it1; dut23stub_                       .clear();
    //nabin
    for (it1=hNumberOfClusters_                 .begin(); it1!=hNumberOfClusters_              .end(); it1++) delete *it1; hNumberOfClusters_               .clear();
}

//=======================================================================
void EfficiencyOuterTracker::beginJob(void)
{
    theWindowsManager_ = theAnalysisManager_->getWindowsManager();

    if (theXmlParser_->getScan()->getScanValues().size() == 0) book();
    else
    {
        STDLINE("Unknown option: scan values",ACRed);
        exit(EXIT_FAILURE);
    }

    cumulative=0;
    nevent=0;

}

//=======================================================================
void EfficiencyOuterTracker::analyze(const Data& data, int threadNumber)
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
    if (ONLYdoubleHITS == false)
    {
        for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
        {
            if ((thePlaneMapping_->getPlaneName(p).find("Dut")   == std::string::npos) &&
                    (thePlaneMapping_->getPlaneName(p).find("Strip") == std::string::npos) &&
                    (data.getClusterSize(p) != 2)) return;
        }
    }


    int entry  = data.getEventChewieNumber();
    //std::cout<< "number of entry = "<< entry <<" , nStripHits = "<< data.getNumberOfStripHits() <<" , nTelescopeHits = "<< data.getNumberOfTelescopeHits() <<std::endl;

    int run    = data.getRunNumber();

    for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        if (!passStandardCuts(p,data)) continue;
        //if (!theWindowsManager_->getWindow(p)->checkGoodRun(run,entry)) continue;
        planeEfficiency (passMainCut           ,p,data,threadNumber);
        cellEfficiency  (passCellEfficiencyCut ,p,data,threadNumber);
        xCellEfficiency (passXCellEfficiencyCut,p,data,threadNumber);
        //yCellEfficiency (passYCellEfficiencyCut,p,data,threadNumber);
        //xEdgeEfficiency (passXCellEfficiencyCut,p,data,threadNumber);
        //yEdgeEfficiency (passYCellEfficiencyCut,p,data,threadNumber);

        //THREADED(hAverageHitsperEvent_[p]) = (TH1F*) THREADED(h1DclusterSizes_[p])->Clone();
    }
    //int dut2row = data.getMeanRow(24);
    float dut2col = data.getMeanCol(24);
    //int dut3row = data.getMeanRow(25);
    float dut3col = data.getMeanCol(25);
    int   dut2size= data.getClusterSize(24);
    int   dut3size= data.getClusterSize(25);

    if ( ( data.getHasStub(24) && data.getHasStub(25) ) && data.getHasHit(24) && data.getHasHit(25))
    {
        //if ( dut2size <= 4 && dut3size <=4 ){
        THREADED(dut23stub_   [24]) -> Fill( dut2col - dut3col ); //dut2 --> top ; dut3 --> bottom, dut2 --> CBC2
        THREADED(dut23stub_   [25]) -> Fill( dut2col - dut3col ); //
        //}
    }

    //std::cout<<"cumulative = "<<cumulative<<std::endl;
    //std::cout<<"Number of event = "<<nevent<<std::endl;

    //for (unsigned int g = 0; g < thePlaneMapping_->getNumberOfPlanes(); g++){
    //     THREADED(hAverageHitsperEvent_                [g])->Fill(cumulative);
    //     THREADED(TotalEvent_                          [g])->Fill(nevent);
    // }

}

//=======================================================================
void EfficiencyOuterTracker::endJob(void)
{

    std::stringstream ss;
    std::stringstream ss0;
    std::stringstream ss1;
    std::stringstream ss2;
    std::stringstream ss3;

    double efficiency0;
    double Ntracks0;
    double error0;

    double efficiency1;
    double Ntracks1;
    double error1;

    double efficiency2;
    double Ntracks2;
    double error2;

    double efficiency3;
    double Ntracks3;
    double error3;

    for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        std::string planeName = thePlaneMapping_->getPlaneName(p);
        ss.str("") ; ss << "Adding threads for plane " << p;
        STDLINE(ss.str().c_str(),ACYellow);

        ADD_THREADED(hEfficiency_                	      [p]);
        ADD_THREADED(hEfficiencyNorm_            	      [p]);

        ADD_THREADED(hEfficiencyRef_                      [p]);
        ADD_THREADED(hEfficiencyRefNorm_         	      [p]);

        ADD_THREADED(hEfficiencyStub_          	          [p]);
        ADD_THREADED(hEfficiencyStubNorm_                 [p]);

        ADD_THREADED(hEfficiencyStubRef_          	      [p]);
        ADD_THREADED(hEfficiencyStubRefNorm_              [p]);

        //nabin
        ADD_THREADED(h2DClusterPositionLocal_             [p]);

        ADD_THREADED(h2DEfficiency_              	      [p]);
        ADD_THREADED(h2DEfficiencyNorm_          	      [p]);

        ADD_THREADED(h2DEfficiencyRef_           	      [p]);
        ADD_THREADED(h2DEfficiencyRefNorm_                [p]);

        ADD_THREADED(h2DEfficiencyStubRef_          	  [p]);
        ADD_THREADED(h2DEfficiencyStubRefNorm_            [p]);

        ADD_THREADED(h2DEfficiencyStub_          	      [p]);
        ADD_THREADED(h2DEfficiencyStubNorm_               [p]);

        ADD_THREADED(h2DFakeHitsEfficiency_               [p]);
        ADD_THREADED(h2DFakeHitsEfficiencyNorm_           [p]);

        ADD_THREADED(h2D4cellEfficiency_         	      [p]);
        ADD_THREADED(h2D4cellEfficiencyNorm_     	      [p]);

        ADD_THREADED(hCellEfficiency_            	      [p]);
        ADD_THREADED(hCellEfficiencyNorm_        	      [p]);

        ADD_THREADED(hCellEfficiencyEvenColumns_          [p]);
        ADD_THREADED(hCellEfficiencyEvenColumnsNorm_      [p]);

        ADD_THREADED(hCellEfficiencyOddColumns_           [p]);
        ADD_THREADED(hCellEfficiencyOddColumnsNorm_       [p]);

        ADD_THREADED(h1DXcellEfficiencyFirstHit_ 	      [p]);
        ADD_THREADED(h1DXcellEfficiencySecondHit_	      [p]);
        ADD_THREADED(h1DXcellEfficiencyNorm_     	      [p]);

        //ADD_THREADED(h1DYcellEfficiencyFirstHit_ 	      [p]);
        //ADD_THREADED(h1DYcellEfficiencySecondHit_	      [p]);
        //ADD_THREADED(h1DYcellEfficiencyNorm_     	      [p]);

        //ADD_THREADED(h1DXcellEdgeRightEfficiency_	      [p]);
        //ADD_THREADED(h1DXcellEdgeRightEfficiencyNorm_   [p]);

        //ADD_THREADED(h1DXcellEdgeLeftEfficiency_	      [p]);
        //ADD_THREADED(h1DXcellEdgeLeftEfficiencyNorm_    [p]);

        //ADD_THREADED(h1DYcellEdgeUpEfficiency_	      [p]);
        //ADD_THREADED(h1DYcellEdgeUpEfficiencyNorm_      [p]);

        //ADD_THREADED(h1DYcellEdgeDownEfficiency_	      [p]);
        //ADD_THREADED(h1DYcellEdgeDownEfficiencyNorm_    [p]);

        ADD_THREADED(h1DclusterSizeVsXStripPosition_   	  [p]);
        ADD_THREADED(h1DclusterSizes_                	  [p]);
        ADD_THREADED(h1DclusterSizeVsXStripPosition_norm_ [p]);

        ADD_THREADED(hAverageHitsperEvent_                [p]);
        ADD_THREADED(TotalEvent_                          [p]);
        ADD_THREADED(checkineff_                          [p]);
        ADD_THREADED(diffFirstSecondHist_                 [p]);
        ADD_THREADED(dut23stub_                           [p]);
        //nabin
        ADD_THREADED(hNumberOfClusters_                   [p]);

        STDLINE("Threading phase completed",ACGreen);


        STDLINE("Filling phase...",ACWhite);

        hEfficiency_                 [p]->Divide(hEfficiencyNorm_                 [p]);

        h2DEfficiency_               [p]->Divide(h2DEfficiencyNorm_               [p]);

        hEfficiencyRef_              [p]->Divide(hEfficiencyRefNorm_              [p]);

        h2DEfficiencyRef_            [p]->Divide(h2DEfficiencyRefNorm_            [p]);

        hEfficiencyStub_             [p]->Divide(hEfficiencyStubNorm_             [p]);

        h2DEfficiencyStub_           [p]->Divide(h2DEfficiencyStubNorm_           [p]);

        hEfficiencyStubRef_          [p]->Divide(hEfficiencyStubRefNorm_          [p]);

        h2DEfficiencyStubRef_        [p]->Divide(h2DEfficiencyStubRefNorm_        [p]);

        h2DFakeHitsEfficiency_       [p]->Divide(h2DFakeHitsEfficiencyNorm_       [p]);

        h2D4cellEfficiency_          [p]->Divide(h2D4cellEfficiencyNorm_          [p]);

        hCellEfficiency_             [p]->Divide(hCellEfficiencyNorm_             [p]);

        hCellEfficiencyEvenColumns_  [p]->Divide(hCellEfficiencyEvenColumnsNorm_  [p]);

        hCellEfficiencyOddColumns_   [p]->Divide(hCellEfficiencyOddColumnsNorm_   [p]);

        h1DXcellEfficiencyFirstHit_  [p]->Divide(h1DXcellEfficiencyNorm_	        [p]);

        h1DXcellEfficiencySecondHit_ [p]->Divide(h1DXcellEfficiencyNorm_	        [p]);

        //h1DYcellEfficiencyFirstHit_  [p]->Divide(h1DYcellEfficiencyNorm_	        [p]);

        //h1DYcellEfficiencySecondHit_ [p]->Divide(h1DYcellEfficiencyNorm_	        [p]);

        h1DclusterSizeVsXStripPosition_ [p]->Divide(h1DclusterSizeVsXStripPosition_norm_ [p]);

        //h1DclusterSizes_                [p]->Divide(h1DclusterSizes_                [p]);

        diffFirstSecondHist_            [p]->Divide(h1DXcellEfficiencyNorm_	        [p]);

        diffFirstSecondHist_            [p]->Add(h1DXcellEfficiencySecondHit_[p],-1);

        /*
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
*/
        //hAverageHitsperEvent_            [p]->Divide(TotalEvent_ [p]);

        // ######################
        // # Setting error bars #
        // ######################
        setErrorsBar(p);


        STDLINE("Setting styles...",ACWhite);

        h1DXcellEfficiencyFirstHit_     [p]->SetMarkerStyle(20);
        h1DXcellEfficiencyFirstHit_     [p]->SetMarkerSize(0.6);

        h1DXcellEfficiencySecondHit_    [p]->SetMarkerStyle(20);
        h1DXcellEfficiencySecondHit_    [p]->SetMarkerSize(0.6);

        h1DclusterSizes_                [p]->SetMarkerStyle(20);
        h1DclusterSizes_                [p]->SetMarkerSize(0.6);

        h1DclusterSizeVsXStripPosition_ [p]->SetMarkerStyle(20);
        h1DclusterSizeVsXStripPosition_ [p]->SetMarkerSize(0.6);

        h1DclusterSizeVsXStripPosition_norm_ [p]->SetMarkerStyle(20);
        h1DclusterSizeVsXStripPosition_norm_ [p]->SetMarkerSize(0.6);

        //h1DYcellEfficiencyFirstHit_     [p]->SetMarkerStyle(20);
        //h1DYcellEfficiencyFirstHit_     [p]->SetMarkerSize(0.6);

        //h1DYcellEfficiencySecondHit_    [p]->SetMarkerStyle(20);
        //h1DYcellEfficiencySecondHit_    [p]->SetMarkerSize(0.6);

        //h1DXcellEdgeRightEfficiency_    [p]->SetMarkerStyle(20);
        //h1DXcellEdgeRightEfficiency_    [p]->SetMarkerSize(0.6);

        //h1DXcellEdgeLeftEfficiency_     [p]->SetMarkerStyle(20);
        //h1DXcellEdgeLeftEfficiency_     [p]->SetMarkerSize(0.6);

        //h1DYcellEdgeUpEfficiency_       [p]->SetMarkerStyle(20);
        //h1DYcellEdgeUpEfficiency_       [p]->SetMarkerSize(0.6);

        //h1DYcellEdgeDownEfficiency_     [p]->SetMarkerStyle(20);
        //h1DYcellEdgeDownEfficiency_     [p]->SetMarkerSize(0.6);


        //nabin
        h2DClusterPositionLocal_        [p]->GetXaxis()->SetTitle("xLocalPosition");
        h2DClusterPositionLocal_        [p]->GetYaxis()->SetTitle("yLocalPosition");

        h2DEfficiency_                  [p]->GetXaxis()->SetTitle("strip");
        h2DEfficiency_                  [p]->GetYaxis()->SetTitle("projected track");

        h2DEfficiencyNorm_              [p]->GetXaxis()->SetTitle("strip");
        h2DEfficiencyNorm_              [p]->GetYaxis()->SetTitle("projected track");

        h2DEfficiencyRef_               [p]->GetXaxis()->SetTitle("strip");
        h2DEfficiencyRef_               [p]->GetYaxis()->SetTitle("projected track");

        h2DEfficiencyRefNorm_           [p]->GetXaxis()->SetTitle("strip");
        h2DEfficiencyRefNorm_           [p]->GetYaxis()->SetTitle("projected track");

        h2DEfficiencyStub_              [p]->GetXaxis()->SetTitle("strip");
        h2DEfficiencyStub_              [p]->GetYaxis()->SetTitle("projected track");

        h2DEfficiencyStubNorm_          [p]->GetXaxis()->SetTitle("strip");
        h2DEfficiencyStubNorm_          [p]->GetYaxis()->SetTitle("projected track");

        h2DEfficiencyStubRef_           [p]->GetXaxis()->SetTitle("strip");
        h2DEfficiencyStubRef_           [p]->GetYaxis()->SetTitle("projected track");

        h2DEfficiencyStubRefNorm_       [p]->GetXaxis()->SetTitle("strip");
        h2DEfficiencyStubRefNorm_       [p]->GetYaxis()->SetTitle("projected track");

        h2DFakeHitsEfficiency_          [p]->GetXaxis()->SetTitle("strip");
        h2DFakeHitsEfficiency_          [p]->GetYaxis()->SetTitle("fake projected track");

        h2DFakeHitsEfficiencyNorm_      [p]->GetXaxis()->SetTitle("strip");
        h2DFakeHitsEfficiencyNorm_      [p]->GetYaxis()->SetTitle("fake projected track");

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

        h1DXcellEfficiencyFirstHit_     [p]->GetXaxis()->SetTitle("long pitch (um)");
        h1DXcellEfficiencyFirstHit_     [p]->GetYaxis()->SetTitle("efficiency");
        h1DXcellEfficiencySecondHit_    [p]->GetXaxis()->SetTitle("long pitch (um)");
        h1DXcellEfficiencySecondHit_    [p]->GetYaxis()->SetTitle("efficiency");
        h1DXcellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("long pitch (um)");

        //h1DXcellEdgeRightEfficiency_    [p]->GetXaxis()->SetTitle("long pitch (um)");
        //h1DXcellEdgeRightEfficiency_    [p]->GetYaxis()->SetTitle("efficiency");
        //h1DXcellEdgeRightEfficiencyNorm_[p]->GetXaxis()->SetTitle("long pitch (um)");

        //h1DXcellEdgeLeftEfficiency_     [p]->GetXaxis()->SetTitle("long pitch (um)");
        //h1DXcellEdgeLeftEfficiency_     [p]->GetYaxis()->SetTitle("efficiency");
        //h1DXcellEdgeLeftEfficiencyNorm_ [p]->GetXaxis()->SetTitle("long pitch (um)");

        //h1DYcellEfficiencyFirstHit_     [p]->GetXaxis()->SetTitle("short pitch (um)");
        //h1DYcellEfficiencyFirstHit_     [p]->GetYaxis()->SetTitle("efficiency");
        //h1DYcellEfficiencySecondHit_    [p]->GetXaxis()->SetTitle("short pitch (um)");
        //h1DYcellEfficiencySecondHit_    [p]->GetYaxis()->SetTitle("efficiency");
        //h1DYcellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("short pitch (um)");

        //h1DYcellEdgeUpEfficiency_       [p]->GetXaxis()->SetTitle("short pitch (um)");
        //h1DYcellEdgeUpEfficiency_       [p]->GetYaxis()->SetTitle("efficiency");
        //h1DYcellEdgeUpEfficiencyNorm_   [p]->GetXaxis()->SetTitle("short pitch (um)");

        //h1DYcellEdgeDownEfficiency_     [p]->GetXaxis()->SetTitle("short pitch (um)");
        //h1DYcellEdgeDownEfficiency_     [p]->GetYaxis()->SetTitle("efficiency");
        //h1DYcellEdgeDownEfficiencyNorm_ [p]->GetXaxis()->SetTitle("short pitch (um)");

        h1DclusterSizes_                  [p]->GetXaxis()->SetTitle("cluster size");
        h1DclusterSizes_                  [p]->GetYaxis()->SetTitle("count");

        h1DclusterSizeVsXStripPosition_   [p]->GetXaxis()->SetTitle("short pitch (um)");
        h1DclusterSizeVsXStripPosition_   [p]->GetYaxis()->SetTitle("average cluster size");

        hAverageHitsperEvent_             [p]->GetXaxis()->SetTitle("value");
        hAverageHitsperEvent_             [p]->GetYaxis()->SetTitle("Average hits per event");

        TotalEvent_                       [p]->GetXaxis()->SetTitle("value");
        TotalEvent_                       [p]->GetYaxis()->SetTitle("Average hits per event");

        checkineff_                       [p]->GetXaxis()->SetTitle("chirality");
        checkineff_                       [p]->GetYaxis()->SetTitle("Number Of Events");

        diffFirstSecondHist_              [p]->GetXaxis()->SetTitle("pitch (um)");
        diffFirstSecondHist_              [p]->GetYaxis()->SetTitle("efficiency");

        dut23stub_                        [p]->GetXaxis()->SetTitle("#Delta X [strip]");
        dut23stub_                        [p]->GetYaxis()->SetTitle("Number of events");
        //nabin
        hNumberOfClusters_                [p]->GetXaxis()->SetTitle("Number of clusters");
        hNumberOfClusters_                [p]->GetYaxis()->SetTitle("count");

        // ##############################
        // # Print efficiency on screen #
        // ##############################

        if (p==24 || p==25){
            efficiency0  = hEfficiency_    [p]->GetBinContent(1);
            Ntracks0     = hEfficiencyNorm_[p]->GetBinContent(1);
            error0       = sqrt(efficiency0 * (1 - efficiency0) / Ntracks0);

            ss0.str("");
            ss0 << "Detector: "                 << std::setw(27) << thePlaneMapping_->getPlaneName(p)
                << " Efficiency (Overall): "     << std::setw(4)  << std::setprecision(7) << efficiency0*100.
                << " +- "                        << std::setw(4)  << std::setprecision(3) << error0*100.
                << " Entries: "                  << hEfficiencyNorm_[p]->GetBinContent(1);


            efficiency1  = hEfficiencyRef_    [p]->GetBinContent(1);
            Ntracks1     = hEfficiencyRefNorm_[p]->GetBinContent(1);
            error1       = sqrt(efficiency1 * (1 - efficiency1) / Ntracks1);

            ss1.str("");
            ss1 << "Detector: "                 << std::setw(27) << thePlaneMapping_->getPlaneName(p)
                << " Efficiency (Overall Ref): " << std::setw(4)  << std::setprecision(7) << efficiency1*100.
                << " +- "                        << std::setw(4)  << std::setprecision(3) << error1*100.
                << " Entries: "                  << hEfficiencyRefNorm_[p]->GetBinContent(1);

            efficiency2  = hEfficiencyStub_    [p]->GetBinContent(1);
            Ntracks2     = hEfficiencyStubNorm_[p]->GetBinContent(1);
            error2       = sqrt(efficiency2 * (1 - efficiency2) / Ntracks2);

            ss2.str("");
            ss2 << "Detector: "                 << std::setw(27) << thePlaneMapping_->getPlaneName(p)
                << " Efficiency (Stub): "        << std::setw(4)  << std::setprecision(7) << efficiency2*100.
                << " +- "                        << std::setw(4)  << std::setprecision(3) << error2*100.
                << " Entries: "                  << hEfficiencyStubNorm_[p]->GetBinContent(1);

            efficiency3  = hEfficiencyStubRef_    [p]->GetBinContent(1);
            Ntracks3     = hEfficiencyStubRefNorm_[p]->GetBinContent(1);
            error3       = sqrt(efficiency3 * (1 - efficiency3) / Ntracks3);

            ss3.str("");
            ss3 << "Detector: "                 << std::setw(27) << thePlaneMapping_->getPlaneName(p)
                << " Efficiency (Stub Ref): "    << std::setw(4)  << std::setprecision(7) << efficiency3*100.
                << " +- "                        << std::setw(4)  << std::setprecision(3) << error3*100.
                << " Entries: "                  << hEfficiencyStubRefNorm_[p]->GetBinContent(1);



            STDLINE(ss0.str(),ACLightPurple);
            STDLINE(ss1.str(),ACLightPurple);
            STDLINE(ss2.str(),ACLightPurple);
            STDLINE(ss3.str(),ACLightPurple);
        }


    }
}

//=======================================================================
void EfficiencyOuterTracker::book(void)
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

        nBinsX = abs(lowerCol - higherCol) + 1;
        nBinsY = abs(lowerRow - higherRow) + 1;

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

        hName  = "EfficiencyStub_"                                        + planeName;
        hTitle = "Overall stub efficiency "                               + planeName;
        hEfficiencyStub_.push_back                (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "EfficiencyStubNorm_"                                    + planeName;
        hTitle = "Overall stub efficiency normalization "                 + planeName;
        hEfficiencyStubNorm_.push_back            (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "EfficiencyStubRef_"                                     + planeName;
        hTitle = "Overall stub efficiency ref. "                          + planeName;
        hEfficiencyStubRef_.push_back             (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "EfficiencyStubRefNorm_"                                 + planeName;
        hTitle = "Overall stub efficiency ref. normalization "            + planeName;
        hEfficiencyStubRefNorm_.push_back         (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "h1DXcellEfficiencyFirstHit_"                            + planeName;
        hTitle = "1D Cell efficiency - X coordinate first hit "           + planeName;
        //h1DXcellEfficiencyFirstHit_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(xPitch/binSize),-xPitch/2,xPitch/2)));
        h1DXcellEfficiencyFirstHit_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(95/5),(-xPitch/2)-2.5,(xPitch/2)+2.5)));

        hName  = "h1DXcellEfficiencySecondHit_"                           + planeName;
        hTitle = "1D cell Efficiency - X coordinate second hit "          + planeName;
        //h1DXcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));
        h1DXcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(95/5),(-xPitch/2)-2.5,(xPitch/2)+2.5)));

        hName  = "h1DXcellEfficiencyNorm_"                                + planeName;
        hTitle = "1D cell Efficiency - X coordinate normalization "       + planeName;
        //h1DXcellEfficiencyNorm_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));
        h1DXcellEfficiencyNorm_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(95/5),(-xPitch/2)-2.5,(xPitch/2)+2.5)));

        hName  = "h1DclusterSizeVsXStripPosition_"                            + planeName;
        hTitle = "X Coordinate vs Cluster Size"           + planeName;
        h1DclusterSizeVsXStripPosition_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));

        hName  = "h1DclusterSizes_"                            + planeName;
        hTitle = "Counts for Cluster Sizes"           + planeName;
        h1DclusterSizes_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),15,-0.5,14.5)));

        hName  = "h1DclusterSizeVsXStripPosition_norm_"                            + planeName;
        hTitle = "X Coordinate vs Cluster Size"           + planeName;
        h1DclusterSizeVsXStripPosition_norm_.push_back (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));

        hName  = "hAverageHitsperEvent_"              + planeName;
        hTitle = "Average Hits per Event"             + planeName;
        hAverageHitsperEvent_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "TotalEvent_"              + planeName;
        hTitle = "Total Event"             + planeName;
        TotalEvent_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "checkineff_"             +planeName;
        hTitle = "check inefficiency"      +planeName;
        checkineff_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),3,-1.5,1.5)));

        hName  = "diffFirstSecondHist_"     +planeName;
        hTitle = "First Hits Eff - Second Hits Eff" + planeName;
        //diffFirstSecondHist_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize,-xPitch/2,xPitch/2)));
        diffFirstSecondHist_.push_back     (NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(95/5),(-xPitch/2)-2.5,(xPitch/2)+2.5)));
        //diffFirstSecondHist_.push_back(NEW_THREADED(TH1F()));

        hName  = "dut23stub_"               +planeName;
        hTitle = "Stub direction on plane"          + planeName;
        dut23stub_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 80 , -10. , 30. )));
        //nabin
        hName  = "hNumberOfClusters_"          +planeName;
        hTitle = "Number of clusters"          + planeName;
        hNumberOfClusters_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), 30 , 0. , 30. )));

        // #################
        // # 2D histograms #
        // #################

        //nabin
        hName  = "h2DClusterPositionLocal_"                                          + planeName;
        hTitle = "2D Cluster Local Position "                                        + planeName;
        h2DClusterPositionLocal_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));


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

        hName  = "2DEfficiencyStub_"                                      + planeName;
        hTitle = "2D stub efficiency distribution"                        + planeName;
        h2DEfficiencyStub_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

        hName  = "2DEfficiencyStubNorm_"                                  + planeName;
        hTitle = "2D stub efficiency normalization "                      + planeName;
        h2DEfficiencyStubNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

        hName  = "2DEfficiencyStubRef_"                                   + planeName;
        hTitle = "2D stub efficiency distribution ref. "                  + planeName;
        h2DEfficiencyStubRef_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

        hName  = "2DEfficiencyStubRefNorm_"                               + planeName;
        hTitle = "2D stub efficiency ref. normalization "                 + planeName;
        h2DEfficiencyStubRefNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

        hName  = "2DFakeHitsEfficiency_"                                            + planeName;
        hTitle = "2D Fake Hits efficiency distribution "                            + planeName;
        h2DFakeHitsEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

        hName  = "2DFakeHitsEfficiencyNorm_"                                      + planeName;
        hTitle = "2D Fake Hits efficiency normalization "                           + planeName;
        h2DFakeHitsEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol + 1,nBinsY,lowerRow,higherRow + 1)));

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
void EfficiencyOuterTracker::setErrorsBar(int planeID)
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
    }
    /*
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
    */
}

//=======================================================================
void EfficiencyOuterTracker::planeEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
    //std::cout<<thePlaneMapping_->getPlaneName(planeID)<<std::endl;
    if (!pass || !data.getIsInDetector(planeID)) return;

    const Window* theWindow    = theWindowsManager_->getWindow(planeID);
    int           rowPredicted = data.getRowPredicted(planeID);
    int           colPredicted = data.getColPredicted(planeID);
    int           run          = data.getRunNumber();
    //nabin
    std::cout << "Test line 1: " << std::endl;


    //std::cout << "xClusterMeasuredLocal: " << xClusterMeasuredLocal << std::endl;

    // #########################################
    // # Check if track and hits are in window #
    // #########################################
    if (!theWindow->checkWindow_v1(colPredicted,rowPredicted,run,planeID)) return;

    // ##################################################
    // # Compute efficiency for all cells in acceptance #
    // ##################################################
    // thus, dubbed "Ref", without check window
    //std::cout << __PRETTY_FUNCTION__ << "Cluster size: " << data.getNumberOfClusters(planeID) << std::endl;
    //nabin
    THREADED(hNumberOfClusters_      [planeID])->Fill(data.getNumberOfClusters(planeID));

    for(int nC=0; nC<data.getNumberOfClusters(planeID); nC++)
    {
        float         xClusterMeasuredLocal = data.getXClusterMeasuredLocal(nC,planeID);
        float         yClusterMeasuredLocal = data.getYClusterMeasuredLocal(nC,planeID);
        THREADED(h2DClusterPositionLocal_[planeID])->Fill(xClusterMeasuredLocal,yClusterMeasuredLocal);
    }

    THREADED(hEfficiencyRefNorm_  [planeID])->Fill(1);
    THREADED(h2DEfficiencyRefNorm_[planeID])->Fill(colPredicted,rowPredicted);

    if (data.getHasHit(planeID))
    {
        THREADED(hEfficiencyRef_  [planeID])->Fill(1);
        THREADED(h2DEfficiencyRef_[planeID])->Fill(colPredicted,rowPredicted);
    }

    if (data.getHasHit(24) && data.getHasHit(25))
    {
        THREADED(hEfficiencyStubRefNorm_  [planeID])->Fill(1);
        THREADED(h2DEfficiencyStubRefNorm_[planeID])->Fill(colPredicted,rowPredicted);

        if (data.getHasStub(planeID))
        {
            THREADED(hEfficiencyStubRef_[planeID])->Fill(1);
            THREADED(h2DEfficiencyStubRef_[planeID])->Fill(colPredicted,rowPredicted);
        }
    }

    // #########################################################################
    // # Compute efficiency only for cells that are surrounded by "good" cells #
    // #########################################################################
    if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,0)) // overriden for strip only
    {
        THREADED(hEfficiencyNorm_  [planeID])->Fill(1);
        THREADED(h2DEfficiencyNorm_[planeID])->Fill(colPredicted,rowPredicted);

        if (data.getHasHit(planeID))
        {
            THREADED(hEfficiency_  [planeID])->Fill(1);
            THREADED(h2DEfficiency_[planeID])->Fill(colPredicted,rowPredicted);
        }

        if (data.getHasHit(24) && data.getHasHit(25))
        {
            THREADED(hEfficiencyStubNorm_  [planeID])->Fill(1);
            THREADED(h2DEfficiencyStubNorm_[planeID])->Fill(colPredicted,rowPredicted);

            if (data.getHasStub(planeID))
            {
                THREADED(hEfficiencyStub_[planeID])->Fill(1);
                THREADED(h2DEfficiencyStub_[planeID])->Fill(colPredicted,rowPredicted);
            }
        }
    }
}

//=======================================================================
void EfficiencyOuterTracker::cellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
    if (!pass || !data.getIsInDetector(planeID)) return;

    //if(planeID==22 || planeID==23)
    //std::cout << __PRETTY_FUNCTION__ <<"PASS --> cell eff" <<std::endl;

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
    /*
    if (planeID == 24 || planeID == 25){
        std::cout<<"maxPitchX = "<< maxPitchX <<std::endl;
        std::cout<<"maxPitchY = "<< maxPitchY <<std::endl;

        std::cout<<"data.getXPitchLocal = "<< data.getXPitchLocal(planeID) <<std::endl;
        std::cout<<"data.getYPitchLocal = "<< data.getYPitchLocal(planeID) <<std::endl;
    }
*/
    // #########################################
    // # Check if track and hits are in window #
    // #########################################
    if (!theWindow->checkWindow_v1(colPredicted,rowPredicted,run,planeID)) return;

    if (data.getXPixelResidualLocal(planeID) > 0)       xRes4Cells = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
    else if (data.getXPixelResidualLocal(planeID) <= 0) xRes4Cells = data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;

    if (data.getYPixelResidualLocal(planeID) > 0)       yRes4Cells = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
    else if (data.getYPixelResidualLocal(planeID) <= 0) yRes4Cells = data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;

    //if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID)) &&
    //        data.getXPitchLocal(planeID) <= maxPitchX &&
    //        data.getYPitchLocal(planeID) <= maxPitchY)
    if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,0) && // chekWindowAbout overriden for strip only
            data.getXPitchLocal(planeID) <= maxPitchX &&
            data.getYPitchLocal(planeID) <= maxPitchY
            ) // overriden for strip only
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
void EfficiencyOuterTracker::xCellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
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
    int           clusterSize = data.getClusterSize(planeID);


    // #########################################
    // # Check if track and hits are in window #
    // #########################################
    //std::cout<<"Before checkWindow : planeID = "<<planeID<<": clusterSize = " << clusterSize <<std::endl;

    if (!theWindow->checkWindow_v1(colPredicted,rowPredicted,run,planeID)) return;

    //std::cout<<"After checkWindow : planeID = "<<planeID<<": clusterSize = " << clusterSize <<std::endl;


    if (data.getXPitchLocal(planeID) == maxPitchX)
    {
        if (data.getXPixelResidualLocal(planeID) > 0)       xRes = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
        else if (data.getXPixelResidualLocal(planeID) <= 0) xRes = data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;
    }
    else return;

    //if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,thePlaneMapping_->getPlaneType(planeID))
    if (theWindow->checkWindowAbout(colPredicted,rowPredicted,run,0))
    {
        //if (planeID==24 || planeID==25)
        //std::cout<<"After checkWindowAbout : planeID = "<<planeID<<": clusterSize = " << clusterSize <<std::endl;

        THREADED(h1DXcellEfficiencyNorm_             [planeID])->Fill(xRes);
        if (data.getHasHit(planeID))
        {

            //cumulative+=clusterSize;
            //nevent+=1;

            THREADED(TotalEvent_                          [planeID])->Fill(1);//Not used yet!

            //std::cout<<"Average = "<<cumulative/nevent<<std::endl;

            THREADED(h1DclusterSizes_                    [planeID])->Fill(clusterSize);

            THREADED(h1DclusterSizeVsXStripPosition_norm_[planeID])->Fill(xRes, 1);
            THREADED(h1DclusterSizeVsXStripPosition_     [planeID])->Fill(xRes, data.getClusterSize(planeID));

            if (data.getClusterSize(planeID) > maxClusterSize)
            {
                THREADED(h1DXcellEfficiencyFirstHit_ [planeID])->Fill((xRes)); //norm
                THREADED(diffFirstSecondHist_        [planeID])->Fill(xRes);
                THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
                return;
            }

            bool isOk = false;
            for (int h = 0; h < data.getClusterSize(planeID); h++)
            {
                if ( (data.getClusterPixelCol(h,planeID) == colPredicted) //&& (data.getClusterPixelRow(h,planeID) == rowPredicted)
                     )
                {
                    //std::cout<<" YOU GOT IT : ClusterSize = "<< h <<" ; colPredicted = "<< colPredicted <<" ; data.getClusterPixelCol(h,planeID) = "<< data.getClusterPixelCol(h,planeID)
                    //        <<" ; xRes = "<< xRes <<std::endl;
                    THREADED(h1DXcellEfficiencyFirstHit_ [planeID])->Fill((xRes));
                    THREADED(diffFirstSecondHist_        [planeID])->Fill(xRes);
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
                    //                    if (data.getClusterPixelRow(h,planeID) == rowPredicted)
                    //                    {
                    if (((colPredicted-data.getClusterPixelCol(h,planeID)) == 1) || ((colPredicted-data.getClusterPixelCol(h,planeID)) == -1))
                    {
                        //std::cout<<"NO YOU DONT : ClusterSize = "<< h <<" ; colPredicted = "<< colPredicted <<" ; data.getClusterPixelCol(h,planeID) = "<< data.getClusterPixelCol(h,planeID)
                        //        <<" ; xRes = "<< xRes <<std::endl;
                        if (xRes>0 && xRes>45 )//beloing to the left strip
                        {
                            THREADED(checkineff_[planeID])->Fill(-1);
                        } else if (xRes<0 && xRes>-45 )
                        {
                            THREADED(checkineff_[planeID])->Fill(1);
                        }
                        else{
                            THREADED(checkineff_[planeID])->Fill(0);
                        }


                        isOk = true;
                        break;
                    }
                    //                    }
                }

                if (!isOk) return;

                THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
            }
        }
    }
}

//=======================================================================
void EfficiencyOuterTracker::yCellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
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
    if (!theWindow->checkWindow_v1(colPredicted,rowPredicted,run,planeID)) return;


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

            bool isOk    = false;
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
void EfficiencyOuterTracker::setCutsFormula(std::map<std::string,std::string> cutsList, std::vector<TTree*> tree)
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
bool EfficiencyOuterTracker::passStandardCuts(int planeID, const Data &data)
{
    if (!theXmlParser_->getAnalysesFromString("Efficiency")->applyStandardCuts()) return true;

    int minHits   = atoi(theXmlParser_->getAnalysesFromString("Efficiency")->getMinHits().c_str()) - 1;
    int excludeMe = 0;
    if (thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) minHits += 1;
    else if (data.getHasHit(planeID) && data.getClusterSize(planeID) <= 2) excludeMe = 1;

    if (data.getNumberOfTelescopeClustersSizeLE2() - excludeMe >= minHits) return true;
    else                                                                   return false;
}

//=======================================================================
/*void EfficiencyOuterTracker::countClusterSize(int planeID, const Data &data)
{
    for (int event=0; event<data.; event++)
    {
      if (!planeID == 24 || !planeID == 25) return;

      std::list<std::vector> clusterDataList;

      int xLocalPitch = data.getXPitchLocal(planeID);
      int clusterSize = data.getClusterSize(planeID);
      int eventNumber = data.getEventNumber();

      std::vector<int> clusterData(1, eventNumber);
      clusterData.push_back(clusterSize);
      clusterData.push_back(xLocalPitch);

      clusterDataList.push_back(clusterData);
    }

    int data.getNu

}*/








