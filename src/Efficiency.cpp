#include "Efficiency.h"
#include "AnalysisManager.h"
#include "WindowsManager.h"
#include "Window.h"
#include "ThreadUtilities.h"
#include "PlanesMapping.h"
#include "MessageTools.h"
#include "XmlParser.h"
#include "XmlPlane.h"
#include "XmlWindow.h"
#include "XmlScan.h"
#include "XmlAnalysis.h"
#include "HistogramWindow.h"

#include <TH1F.h>
#include <TH2F.h>
#include <TThread.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TTreeFormula.h>
#include <Rtypes.h>
#include <TEfficiency.h>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Efficiency::Efficiency(AnalysisManager* analysisManager, int nOfThreads) :
    Analysis          (analysisManager, nOfThreads)
  , thePlaneMapping_  (0               )
  , theWindowsManager_(0               )
  , theXmlParser_     (analysisManager->getXmlParser())

{
    thePlaneMapping_ = new PlanesMapping();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Efficiency::~Efficiency(void)
{
    if(thePlaneMapping_)
        delete thePlaneMapping_;

    destroy();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::destroy(void)
{
    if(Analysis::fDoNotDelete_) return;
    //STDLINE("Deleting histos", ACRed);

    std::vector<TH1F*>::iterator it1;
    std::vector<TH2F*>::iterator it2;
    for(it1=hEfficiency_                     .begin(); it1!=hEfficiency_  	      	         .end(); it1++) delete *it1; hEfficiency_		              .clear();
    for(it1=hEfficiencyNorm_                 .begin(); it1!=hEfficiencyNorm_	      	     .end(); it1++) delete *it1; hEfficiencyNorm_		          .clear();
    for(it2=h2DEfficiency_                   .begin(); it2!=h2DEfficiency_	      	         .end(); it2++) delete *it2; h2DEfficiency_		              .clear();
    for(it2=h2DEfficiencyNorm_               .begin(); it2!=h2DEfficiencyNorm_	      	     .end(); it2++) delete *it2; h2DEfficiencyNorm_ 	          .clear();
    for(it1=hEfficiencyRef_                  .begin(); it1!=hEfficiencyRef_  	      	     .end(); it1++) delete *it1; hEfficiencyRef_	          .clear();
    for(it1=hEfficiencyRefNorm_              .begin(); it1!=hEfficiencyRefNorm_	      	     .end(); it1++) delete *it1; hEfficiencyRefNorm_	          .clear();
    for(it2=h2DEfficiencyRef_                .begin(); it2!=h2DEfficiencyRef_	      	     .end(); it2++) delete *it2; h2DEfficiencyRef_	          .clear();
    for(it2=h2DEfficiencyRefNorm_            .begin(); it2!=h2DEfficiencyRefNorm_	     .end(); it2++) delete *it2; h2DEfficiencyRefNorm_ 	          .clear();
    for(it2=h2D4cellEfficiency_              .begin(); it2!=h2D4cellEfficiency_	      	     .end(); it2++) delete *it2; h2D4cellEfficiency_	          .clear();
    for(it2=h2D4cellEfficiencyNorm_          .begin(); it2!=h2D4cellEfficiencyNorm_     	 .end(); it2++) delete *it2; h2D4cellEfficiencyNorm_	      .clear();
    for(it2=h2DInefficiency_                 .begin(); it2!=h2DInefficiency_	      	     .end(); it2++) delete *it2; h2DInefficiency_		          .clear();
    for(it2=hCellEfficiency_                 .begin(); it2!=hCellEfficiency_	      	     .end(); it2++) delete *it2; hCellEfficiency_		          .clear();
    for(it2=hCellEfficiencyEvenColumns_      .begin(); it2!=hCellEfficiencyEvenColumns_      .end(); it2++) delete *it2; hCellEfficiencyEvenColumns_      .clear();
    for(it2=hCellEfficiencyOddColumns_       .begin(); it2!=hCellEfficiencyOddColumns_       .end(); it2++) delete *it2; hCellEfficiencyOddColumns_       .clear();
    for(it2=hCellEfficiencyNorm_             .begin(); it2!=hCellEfficiencyNorm_             .end(); it2++) delete *it2; hCellEfficiencyNorm_             .clear();
    for(it2=hCellEfficiencyEvenColumnsNorm_  .begin(); it2!=hCellEfficiencyEvenColumnsNorm_  .end(); it2++) delete *it2; hCellEfficiencyEvenColumnsNorm_  .clear();
    for(it2=hCellEfficiencyOddColumnsNorm_   .begin(); it2!=hCellEfficiencyOddColumnsNorm_   .end(); it2++) delete *it2; hCellEfficiencyOddColumnsNorm_   .clear();
    for(it2=hCellInefficiency_               .begin(); it2!=hCellInefficiency_	      	     .end(); it2++) delete *it2; hCellInefficiency_ 	          .clear();
    for(it1=h1DXcellEfficiencyFirstHit_      .begin(); it1!=h1DXcellEfficiencyFirstHit_ 	 .end(); it1++) delete *it1; h1DXcellEfficiencyFirstHit_      .clear();
    for(it1=h1DXcellEfficiencySecondHit_     .begin(); it1!=h1DXcellEfficiencySecondHit_	 .end(); it1++) delete *it1; h1DXcellEfficiencySecondHit_     .clear();
    for(it1=h1DXcellEfficiencyNorm_          .begin(); it1!=h1DXcellEfficiencyNorm_     	 .end(); it1++) delete *it1; h1DXcellEfficiencyNorm_	      .clear();
    for(it1=h1DYcellEfficiencyFirstHit_      .begin(); it1!=h1DYcellEfficiencyFirstHit_ 	 .end(); it1++) delete *it1; h1DYcellEfficiencyFirstHit_      .clear();
    for(it1=h1DYcellEfficiencySecondHit_     .begin(); it1!=h1DYcellEfficiencySecondHit_	 .end(); it1++) delete *it1; h1DYcellEfficiencySecondHit_     .clear();
    for(it1=h1DYcellEfficiencyNorm_          .begin(); it1!=h1DYcellEfficiencyNorm_     	 .end(); it1++) delete *it1; h1DYcellEfficiencyNorm_	      .clear();
    for(it1=h1DYcellEfficiency4RowsNorm_     .begin(); it1!=h1DYcellEfficiency4RowsNorm_  	 .end(); it1++) delete *it1; h1DYcellEfficiency4RowsNorm_     .clear();
    for(it1=h1DYcellEfficiency4Rows_         .begin(); it1!=h1DYcellEfficiency4Rows_     	 .end(); it1++) delete *it1; h1DYcellEfficiency4Rows_         .clear();
    for(it1=hXCellEfficiencyRestricted_      .begin(); it1!=hXCellEfficiencyRestricted_ 	 .end(); it1++) delete *it1; hXCellEfficiencyRestricted_      .clear();
    for(it2=hCellEfficiencyEdgeRight_        .begin(); it2!=hCellEfficiencyEdgeRight_   	 .end(); it2++) delete *it2; hCellEfficiencyEdgeRight_        .clear();
    for(it1=hCellEfficiencyEdgeCentral1D_    .begin(); it1!=hCellEfficiencyEdgeCentral1D_    .end(); it1++) delete *it1; hCellEfficiencyEdgeCentral1D_    .clear();
    for(it1=hCellEfficiencyEdgeCentralNorm1D_.begin(); it1!=hCellEfficiencyEdgeCentralNorm1D_.end(); it1++) delete *it1; hCellEfficiencyEdgeCentralNorm1D_.clear();
    for(it1=h1EfficiencyEdgeRight_           .begin(); it1!=h1EfficiencyEdgeRight_  	     .end(); it1++) delete *it1; h1EfficiencyEdgeRight_           .clear();
    for(it1=h1EfficiencyEdgeLeft_            .begin(); it1!=h1EfficiencyEdgeLeft_   	     .end(); it1++) delete *it1; h1EfficiencyEdgeLeft_	          .clear();
    for(it1=h1EfficiencyEdgeUp_              .begin(); it1!=h1EfficiencyEdgeUp_     	     .end(); it1++) delete *it1; h1EfficiencyEdgeUp_              .clear();
    for(it1=h1EfficiencyEdgeDown_            .begin(); it1!=h1EfficiencyEdgeDown_   	     .end(); it1++) delete *it1; h1EfficiencyEdgeDown_            .clear();
    for(it1=h1EfficiencyEdgeRightNorm_       .begin(); it1!=h1EfficiencyEdgeRightNorm_       .end(); it1++) delete *it1; h1EfficiencyEdgeRightNorm_       .clear();
    for(it1=h1EfficiencyEdgeLeftNorm_        .begin(); it1!=h1EfficiencyEdgeLeftNorm_        .end(); it1++) delete *it1; h1EfficiencyEdgeLeftNorm_        .clear();
    for(it1=h1EfficiencyEdgeUpNorm_          .begin(); it1!=h1EfficiencyEdgeUpNorm_          .end(); it1++) delete *it1; h1EfficiencyEdgeUpNorm_	      .clear();
    for(it1=h1EfficiencyEdgeDownNorm_        .begin(); it1!=h1EfficiencyEdgeDownNorm_        .end(); it1++) delete *it1; h1EfficiencyEdgeDownNorm_        .clear();

    for(it1=hCellEfficiencyEdgeLeft1D_cl1_       .begin(); it1!=hCellEfficiencyEdgeLeft1D_cl1_       .end(); it1++) delete *it1;hCellEfficiencyEdgeLeft1D_cl1_       .clear();
    for(it1=hCellEfficiencyEdgeLeft1D_cl2_       .begin(); it1!=hCellEfficiencyEdgeLeft1D_cl2_       .end(); it1++) delete *it1;hCellEfficiencyEdgeLeft1D_cl2_       .clear();
    for(it1=hCellEfficiencyEdgeLeft1D_cl_        .begin(); it1!=hCellEfficiencyEdgeLeft1D_cl_        .end(); it1++) delete *it1;hCellEfficiencyEdgeLeft1D_cl_        .clear();
    for(it1=hCellEfficiencyEdgeLeft1DNorm_cl2_   .begin(); it1!=hCellEfficiencyEdgeLeft1DNorm_cl2_   .end(); it1++) delete *it1;hCellEfficiencyEdgeLeft1DNorm_cl2_   .clear();
    for(it1=hCellEfficiencyEdgeRight1D_cl1_      .begin(); it1!=hCellEfficiencyEdgeRight1D_cl1_      .end(); it1++) delete *it1;hCellEfficiencyEdgeRight1D_cl1_      .clear();
    for(it1=hCellEfficiencyEdgeRight1D_cl2_      .begin(); it1!=hCellEfficiencyEdgeRight1D_cl2_      .end(); it1++) delete *it1;hCellEfficiencyEdgeRight1D_cl2_      .clear();
    for(it1=hCellEfficiencyEdgeRight1D_cl_       .begin(); it1!=hCellEfficiencyEdgeRight1D_cl_       .end(); it1++) delete *it1;hCellEfficiencyEdgeRight1D_cl_       .clear();
    for(it1=hCellEfficiencyEdgeRight1DNorm_cl2_  .begin(); it1!=hCellEfficiencyEdgeRight1DNorm_cl2_  .end(); it1++) delete *it1;hCellEfficiencyEdgeRight1DNorm_cl2_  .clear();
    for(it1=hCellEfficiencyEdgeCentral1D_cl1_    .begin(); it1!=hCellEfficiencyEdgeCentral1D_cl1_    .end(); it1++) delete *it1;hCellEfficiencyEdgeCentral1D_cl1_    .clear();
    for(it1=hCellEfficiencyEdgeCentral1D_cl2_    .begin(); it1!=hCellEfficiencyEdgeCentral1D_cl2_    .end(); it1++) delete *it1;hCellEfficiencyEdgeCentral1D_cl2_    .clear();
    for(it1=hCellEfficiencyEdgeCentral1D_cl_     .begin(); it1!=hCellEfficiencyEdgeCentral1D_cl_     .end(); it1++) delete *it1;hCellEfficiencyEdgeCentral1D_cl_     .clear();
    for(it1=hCellEfficiencyEdgeCentralNorm1D_cl2_.begin(); it1!=hCellEfficiencyEdgeCentralNorm1D_cl2_.end(); it1++) delete *it1;hCellEfficiencyEdgeCentralNorm1D_cl2_.clear();
    for(it1=hCellEfficiencyEdgeLeft1D_cl3_       .begin(); it1!=hCellEfficiencyEdgeLeft1D_cl3_       .end(); it1++) delete *it1;hCellEfficiencyEdgeLeft1D_cl3_       .clear();
    for(it1=hCellEfficiencyEdgeLeft1D_cl4_       .begin(); it1!=hCellEfficiencyEdgeLeft1D_cl4_       .end(); it1++) delete *it1;hCellEfficiencyEdgeLeft1D_cl4_       .clear();
    for(it1=hCellEfficiencyEdgeLeft1DNorm_cl4_   .begin(); it1!=hCellEfficiencyEdgeLeft1DNorm_cl4_   .end(); it1++) delete *it1;hCellEfficiencyEdgeLeft1DNorm_cl4_   .clear();
    for(it1=hCellEfficiencyEdgeRight1D_cl3_      .begin(); it1!=hCellEfficiencyEdgeRight1D_cl3_      .end(); it1++) delete *it1;hCellEfficiencyEdgeRight1D_cl3_      .clear();
    for(it1=hCellEfficiencyEdgeRight1D_cl4_      .begin(); it1!=hCellEfficiencyEdgeRight1D_cl4_      .end(); it1++) delete *it1;hCellEfficiencyEdgeRight1D_cl4_      .clear();
    for(it1=hCellEfficiencyEdgeRight1DNorm_cl4_  .begin(); it1!=hCellEfficiencyEdgeRight1DNorm_cl4_  .end(); it1++) delete *it1;hCellEfficiencyEdgeRight1DNorm_cl4_  .clear();
    for(it1=hCellEfficiencyEdgeCentral1D_cl3_    .begin(); it1!=hCellEfficiencyEdgeCentral1D_cl3_    .end(); it1++) delete *it1;hCellEfficiencyEdgeCentral1D_cl3_    .clear();
    for(it1=hCellEfficiencyEdgeCentral1D_cl4_    .begin(); it1!=hCellEfficiencyEdgeCentral1D_cl4_    .end(); it1++) delete *it1;hCellEfficiencyEdgeCentral1D_cl4_    .clear();
    for(it1=hCellEfficiencyEdgeCentralNorm1D_cl4_.begin(); it1!=hCellEfficiencyEdgeCentralNorm1D_cl4_.end(); it1++) delete *it1;hCellEfficiencyEdgeCentralNorm1D_cl4_.clear();
    for(it2=hCellEfficiencyEdgeRightm1_          .begin(); it2!=hCellEfficiencyEdgeRightm1_     	 .end(); it2++) delete *it2; hCellEfficiencyEdgeRightm1_         .clear();
    for(it2=hCellEfficiencyEdgeRightm3_          .begin(); it2!=hCellEfficiencyEdgeRightm3_     	 .end(); it2++) delete *it2; hCellEfficiencyEdgeRightm3_         .clear();
    for(it2=hCellEfficiencyEdgeRightm5_          .begin(); it2!=hCellEfficiencyEdgeRightm5_     	 .end(); it2++) delete *it2; hCellEfficiencyEdgeRightm5_         .clear();
    for(it2=hCellEfficiencyEdgeRightm2_          .begin(); it2!=hCellEfficiencyEdgeRightm2_     	 .end(); it2++) delete *it2; hCellEfficiencyEdgeRightm2_         .clear();
    for(it2=hCellEfficiencyEdgeRightm4_          .begin(); it2!=hCellEfficiencyEdgeRightm4_     	 .end(); it2++) delete *it2; hCellEfficiencyEdgeRightm4_         .clear();
    for(it2=hCellEfficiencyEdgeLeftp1_           .begin(); it2!=hCellEfficiencyEdgeLeftp1_   	     .end(); it2++) delete *it2; hCellEfficiencyEdgeLeftp1_          .clear();
    for(it2=hCellEfficiencyEdgeLeftp3_           .begin(); it2!=hCellEfficiencyEdgeLeftp3_   	     .end(); it2++) delete *it2; hCellEfficiencyEdgeLeftp3_          .clear();
    for(it2=hCellEfficiencyEdgeLeftp5_           .begin(); it2!=hCellEfficiencyEdgeLeftp5_   	     .end(); it2++) delete *it2; hCellEfficiencyEdgeLeftp5_          .clear();
    for(it2=hCellEfficiencyEdgeLeftp2_           .begin(); it2!=hCellEfficiencyEdgeLeftp2_   	     .end(); it2++) delete *it2; hCellEfficiencyEdgeLeftp2_          .clear();
    for(it2=hCellEfficiencyEdgeLeftp4_           .begin(); it2!=hCellEfficiencyEdgeLeftp4_   	     .end(); it2++) delete *it2; hCellEfficiencyEdgeLeftp4_          .clear();
    for(it2=hCellEfficiencyEdgeLeft_             .begin(); it2!=hCellEfficiencyEdgeLeft_    	     .end(); it2++) delete *it2; hCellEfficiencyEdgeLeft_	         .clear();
    for(it1=hCellEfficiencyEdgeRight1D_          .begin(); it1!=hCellEfficiencyEdgeRight1D_   	     .end(); it1++) delete *it1; hCellEfficiencyEdgeRight1D_         .clear();
    for(it1=hCellEfficiencyEdgeRight1Dm1_        .begin(); it1!=hCellEfficiencyEdgeRight1Dm1_        .end(); it1++) delete *it1; hCellEfficiencyEdgeRight1Dm1_       .clear();
    for(it1=hCellEfficiencyEdgeRight1Dm3_        .begin(); it1!=hCellEfficiencyEdgeRight1Dm3_        .end(); it1++) delete *it1; hCellEfficiencyEdgeRight1Dm3_       .clear();
    for(it1=hCellEfficiencyEdgeRight1Dm5_        .begin(); it1!=hCellEfficiencyEdgeRight1Dm5_        .end(); it1++) delete *it1; hCellEfficiencyEdgeRight1Dm5_       .clear();
    for(it1=hCellEfficiencyEdgeRight1Dm2_        .begin(); it1!=hCellEfficiencyEdgeRight1Dm2_        .end(); it1++) delete *it1; hCellEfficiencyEdgeRight1Dm2_       .clear();
    for(it1=hCellEfficiencyEdgeRight1Dm4_        .begin(); it1!=hCellEfficiencyEdgeRight1Dm4_        .end(); it1++) delete *it1; hCellEfficiencyEdgeRight1Dm4_       .clear();
    for(it1=hCellEfficiencyEdgeLeft1Dp1_         .begin(); it1!=hCellEfficiencyEdgeLeft1Dp1_   	     .end(); it1++) delete *it1; hCellEfficiencyEdgeLeft1Dp1_        .clear();
    for(it1=hCellEfficiencyEdgeLeft1Dp3_         .begin(); it1!=hCellEfficiencyEdgeLeft1Dp3_   	     .end(); it1++) delete *it1; hCellEfficiencyEdgeLeft1Dp3_        .clear();
    for(it1=hCellEfficiencyEdgeLeft1Dp5_         .begin(); it1!=hCellEfficiencyEdgeLeft1Dp5_   	     .end(); it1++) delete *it1; hCellEfficiencyEdgeLeft1Dp5_        .clear();
    for(it1=hCellEfficiencyEdgeLeft1Dp2_         .begin(); it1!=hCellEfficiencyEdgeLeft1Dp2_   	     .end(); it1++) delete *it1; hCellEfficiencyEdgeLeft1Dp2_        .clear();
    for(it1=hCellEfficiencyEdgeLeft1Dp4_         .begin(); it1!=hCellEfficiencyEdgeLeft1Dp4_   	     .end(); it1++) delete *it1; hCellEfficiencyEdgeLeft1Dp4_        .clear();
    for(it1=hCellEfficiencyEdgeLeft1D_           .begin(); it1!=hCellEfficiencyEdgeLeft1D_    	     .end(); it1++) delete *it1; hCellEfficiencyEdgeLeft1D_	         .clear();
    for(it2=hCellEfficiencyEdgeUp_               .begin(); it2!=hCellEfficiencyEdgeUp_               .end(); it2++) delete *it2; hCellEfficiencyEdgeUp_	             .clear();
    for(it2=hCellEfficiencyEdgeDown_             .begin(); it2!=hCellEfficiencyEdgeDown_             .end(); it2++) delete *it2; hCellEfficiencyEdgeDown_	         .clear();
    for(it2=hCellEfficiencyEdgeRightNorm_        .begin(); it2!=hCellEfficiencyEdgeRightNorm_	     .end(); it2++) delete *it2; hCellEfficiencyEdgeRightNorm_       .clear();
    for(it2=hCellEfficiencyEdgeLeftNorm_         .begin(); it2!=hCellEfficiencyEdgeLeftNorm_ 	     .end(); it2++) delete *it2; hCellEfficiencyEdgeLeftNorm_        .clear();
    for(it2=hCellEfficiencyEdgeUpNorm_           .begin(); it2!=hCellEfficiencyEdgeUpNorm_   	     .end(); it2++) delete *it2; hCellEfficiencyEdgeUpNorm_	         .clear();
    for(it2=hCellEfficiencyEdgeDownNorm_         .begin(); it2!=hCellEfficiencyEdgeDownNorm_ 	     .end(); it2++) delete *it2; hCellEfficiencyEdgeDownNorm_        .clear();

    for(std::map< int,std::vector<TH1F*> >::iterator it=scanEfficiencyNorm_.begin(); it!=scanEfficiencyNorm_.end(); it++)
        for(std::vector<TH1F*>::iterator ii=(it->second).begin();ii!=(it->second).end();ii++)
            delete *ii;
    scanEfficiencyNorm_.clear();

    for(std::map< int,std::vector<TH1F*> >::iterator it=scanEfficiencyHistos_.begin(); it!=scanEfficiencyHistos_.end(); it++)
        for(std::vector<TH1F*>::iterator ii=(it->second).begin();ii!=(it->second).end();ii++)
            delete *ii;
    scanEfficiencyHistos_.clear();

    for(std::vector<TGraph*>::iterator      it=scanGraph_       .begin(); it!=scanGraph_       .end(); it++) delete *it; scanGraph_       .clear();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::beginJob(void)
{
    theWindowsManager_ = theAnalysisManager_->getWindowsManager();

    if(theXmlParser_->getScan()->getScanValues().size()==0)
        book();
    else
        scanBook();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::fitEfficiency(int ) //planeID
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::analyze(const Data& data, int threadNumber)//WARNING: You can't change this name (threadNumber) or the MACRO THREAD won't compile
{
    // Filters out bad events
    bool passMainCut =  true;
    if(cutsFormulas_.find("main cut")!=cutsFormulas_.end())
        passMainCut = cutsFormulas_["main cut"][threadNumber]->EvalInstance();

    if(!passMainCut)
        return;

    if(theXmlParser_->getScan()->getScanValues().size()==0)
    {

        bool passCellEfficiencyCut = true;
        if(cutsFormulas_.find("cell efficiency")!=cutsFormulas_.end())
            passCellEfficiencyCut = cutsFormulas_["cell efficiency"][threadNumber]->EvalInstance();

        bool passXCellEfficiencyCut = true;
        if(cutsFormulas_.find("cell efficiency X")!=cutsFormulas_.end())
            passXCellEfficiencyCut = cutsFormulas_["cell efficiency X"][threadNumber]->EvalInstance();

        bool passYCellEfficiencyCut = true;
        if(cutsFormulas_.find("cell efficiency Y")!=cutsFormulas_.end())
            passYCellEfficiencyCut = cutsFormulas_["cell efficiency Y"][threadNumber]->EvalInstance();

        for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
        {
            if(!passStandardCuts(p,data))
                continue;
            //std::cout << __PRETTY_FUNCTION__ << p << " plane " << thePlaneMapping_->getPlaneType(p) << std::endl;
            planeEfficiency (passMainCut           ,p,data,threadNumber);
            cellEfficiency  (passCellEfficiencyCut ,p,data,threadNumber);
            xCellEfficiency (passXCellEfficiencyCut,p,data,threadNumber);
            yCellEfficiency (passYCellEfficiencyCut,p,data,threadNumber);
        }
    }
    else
    {
        std::string inFileName = "Line148fEfficiencycpp";//theAnalysisManager_->getInFileName();
        std::map<std::string,int> scanValues = theXmlParser_->getScan()->getScanValues();

        for(std::map<std::string,int>::iterator it=scanValues.begin(); it!=scanValues.end(); it++)
        {
            if(it->first == inFileName)
            {
                for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
                {
                    if(thePlaneMapping_->getPlaneName(p).find("Dut")!=std::string::npos)
                        scanEfficiency(p,it->second,data,threadNumber);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::endJob(void)
{
    std::stringstream ss;
    std::stringstream hName;
    double efficiency;
    double Ntrack;
    double error;

    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        if(theXmlParser_->getScan()->getScanValues().size()==0)
        {

            ADD_THREADED(hEfficiency_                	      [p]);
            ADD_THREADED(hEfficiencyNorm_            	      [p]);
            ADD_THREADED(h2DEfficiency_              	      [p]);
            ADD_THREADED(h2DEfficiencyNorm_          	      [p]);
            ADD_THREADED(hEfficiencyRef_               	      [p]);
            ADD_THREADED(hEfficiencyRefNorm_           	      [p]);
            ADD_THREADED(h2DEfficiencyRef_             	      [p]);
            ADD_THREADED(h2DEfficiencyRefNorm_         	      [p]);
            ADD_THREADED(h2D4cellEfficiency_         	      [p]);
            ADD_THREADED(h2D4cellEfficiencyNorm_     	      [p]);
            ADD_THREADED(h2DInefficiency_            	      [p]);
            ADD_THREADED(hCellEfficiencyEvenColumnsNorm_      [p]);
            ADD_THREADED(hCellEfficiencyOddColumnsNorm_       [p]);
            ADD_THREADED(hCellEfficiencyNorm_        	      [p]);
            ADD_THREADED(hCellEfficiency_            	      [p]);
            ADD_THREADED(hCellEfficiencyEvenColumns_          [p]);
            ADD_THREADED(hCellEfficiencyOddColumns_           [p]);
            ADD_THREADED(h1DXcellEfficiencyNorm_     	      [p]);
            ADD_THREADED(h1DXcellEfficiencyFirstHit_ 	      [p]);
            ADD_THREADED(h1DXcellEfficiencySecondHit_	      [p]);
            ADD_THREADED(h1DYcellEfficiencyNorm_     	      [p]);
            ADD_THREADED(h1DYcellEfficiency4RowsNorm_ 	      [p]);
            ADD_THREADED(h1DYcellEfficiency4Rows_     	      [p]);
            ADD_THREADED(h1DYcellEfficiencyFirstHit_ 	      [p]);
            ADD_THREADED(h1DYcellEfficiencySecondHit_	      [p]);
            ADD_THREADED(hCellInefficiency_  	     	      [p]);
            ADD_THREADED(h1EfficiencyEdgeRight_   	          [p]);
            ADD_THREADED(h1EfficiencyEdgeLeft_    	          [p]);
            ADD_THREADED(h1EfficiencyEdgeUp_      	          [p]);
            ADD_THREADED(h1EfficiencyEdgeDown_    	          [p]);
            ADD_THREADED(h1EfficiencyEdgeRightNorm_           [p]);
            ADD_THREADED(h1EfficiencyEdgeLeftNorm_            [p]);
            ADD_THREADED(h1EfficiencyEdgeUpNorm_              [p]);
            ADD_THREADED(h1EfficiencyEdgeDownNorm_            [p]);

            ADD_THREADED(hCellEfficiencyEdgeRight_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeCentral1D_        [p]);
            ADD_THREADED(hCellEfficiencyEdgeCentralNorm1D_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm1_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm3_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm2_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm4_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm5_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp1Norm_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp2Norm_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp4Norm_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp3Norm_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp5Norm_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp1_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp3_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp2_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp4_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftp5_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft_    	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeUp_      	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeDown_    	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightNorm_        [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm1Norm_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm2Norm_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm4Norm_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm3Norm_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRightm5Norm_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1D_   	      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm1_        [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm3_        [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm2_        [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm4_        [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm5_        [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp1Norm_     [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp2Norm_     [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp4Norm_     [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp3Norm_     [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp5Norm_     [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp1_         [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp3_         [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp2_         [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp4_         [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1Dp5_         [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1D_           [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1DNorm_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm1Norm_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm2Norm_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm4Norm_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm3Norm_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1Dm5Norm_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1DNorm_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeftNorm_         [p]);
            ADD_THREADED(hCellEfficiencyEdgeUpNorm_           [p]);
            ADD_THREADED(hCellEfficiencyEdgeDownNorm_         [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1D_cl1_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1D_cl1_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1D_cl_   	  [p]);
            ADD_THREADED(hCellEfficiencyEdgeCentral1D_cl1_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1D_cl2_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_   [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1D_cl2_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_  [p]);
            ADD_THREADED(hCellEfficiencyEdgeCentral1D_cl2_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1D_cl3_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1D_cl3_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeCentral1D_cl3_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1D_cl4_       [p]);
            ADD_THREADED(hCellEfficiencyEdgeLeft1DNorm_cl4_   [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1D_cl4_      [p]);
            ADD_THREADED(hCellEfficiencyEdgeRight1DNorm_cl4_  [p]);
            ADD_THREADED(hCellEfficiencyEdgeCentral1D_cl4_    [p]);
            ADD_THREADED(hCellEfficiencyEdgeCentralNorm1D_cl4_[p]);

            hEfficiency_                 [p]->Divide(hEfficiencyNorm_                 [p]);
            h2DEfficiency_               [p]->Divide(h2DEfficiencyNorm_               [p]);
            hEfficiencyRef_              [p]->Divide(hEfficiencyRefNorm_              [p]);
            h2DEfficiencyRef_            [p]->Divide(h2DEfficiencyRefNorm_            [p]);
            h2D4cellEfficiency_          [p]->Divide(h2D4cellEfficiencyNorm_          [p]);
            h2DInefficiency_             [p]->Divide(h2DEfficiencyNorm_               [p]);
            hCellEfficiency_             [p]->Divide(hCellEfficiencyNorm_             [p]);
            hCellInefficiency_           [p]->Divide(hCellEfficiencyNorm_             [p]);
            hCellEfficiencyEvenColumns_  [p]->Divide(hCellEfficiencyEvenColumnsNorm_  [p]);
            hCellEfficiencyOddColumns_   [p]->Divide(hCellEfficiencyOddColumnsNorm_   [p]);
            h1DXcellEfficiencyFirstHit_  [p]->Divide(h1DXcellEfficiencyNorm_	      [p]);
            h1DXcellEfficiencySecondHit_ [p]->Divide(h1DXcellEfficiencyNorm_	      [p]);
            h1DYcellEfficiencyFirstHit_  [p]->Divide(h1DYcellEfficiencyNorm_	      [p]);
            h1DYcellEfficiencySecondHit_ [p]->Divide(h1DYcellEfficiencyNorm_	      [p]);
            h1DYcellEfficiency4Rows_     [p]->Divide(h1DYcellEfficiency4RowsNorm_	  [p]);
            h1EfficiencyEdgeRight_       [p]->Divide(h1EfficiencyEdgeRightNorm_       [p]);
            h1EfficiencyEdgeLeft_        [p]->Divide(h1EfficiencyEdgeLeftNorm_        [p]);
            h1EfficiencyEdgeUp_          [p]->Divide(h1EfficiencyEdgeUpNorm_          [p]);
            h1EfficiencyEdgeDown_        [p]->Divide(h1EfficiencyEdgeDownNorm_        [p]);

            hCellEfficiencyEdgeRight_        [p]->Divide(hCellEfficiencyEdgeRightNorm_    [p]);
            hCellEfficiencyEdgeCentral1D_    [p]->Divide(hCellEfficiencyEdgeCentralNorm1D_[p]);
            hCellEfficiencyEdgeRightm1_      [p]->Divide(hCellEfficiencyEdgeRightm1Norm_  [p]);
            hCellEfficiencyEdgeRightm2_      [p]->Divide(hCellEfficiencyEdgeRightm2Norm_  [p]);
            hCellEfficiencyEdgeRightm4_      [p]->Divide(hCellEfficiencyEdgeRightm4Norm_  [p]);
            hCellEfficiencyEdgeRightm3_      [p]->Divide(hCellEfficiencyEdgeRightm3Norm_  [p]);
            hCellEfficiencyEdgeRightm5_      [p]->Divide(hCellEfficiencyEdgeRightm5Norm_  [p]);
            hCellEfficiencyEdgeLeftp1_       [p]->Divide(hCellEfficiencyEdgeLeftp1Norm_   [p]);
            hCellEfficiencyEdgeLeftp2_       [p]->Divide(hCellEfficiencyEdgeLeftp2Norm_   [p]);
            hCellEfficiencyEdgeLeftp4_       [p]->Divide(hCellEfficiencyEdgeLeftp4Norm_   [p]);
            hCellEfficiencyEdgeLeftp3_       [p]->Divide(hCellEfficiencyEdgeLeftp3Norm_   [p]);
            hCellEfficiencyEdgeLeftp5_       [p]->Divide(hCellEfficiencyEdgeLeftp5Norm_   [p]);
            hCellEfficiencyEdgeLeft_         [p]->Divide(hCellEfficiencyEdgeLeftNorm_     [p]);
            hCellEfficiencyEdgeRight1D_      [p]->Divide(hCellEfficiencyEdgeRight1DNorm_  [p]);
            hCellEfficiencyEdgeRight1Dm1_    [p]->Divide(hCellEfficiencyEdgeRight1Dm1Norm_[p]);
            hCellEfficiencyEdgeRight1Dm2_    [p]->Divide(hCellEfficiencyEdgeRight1Dm2Norm_[p]);
            hCellEfficiencyEdgeRight1Dm4_    [p]->Divide(hCellEfficiencyEdgeRight1Dm4Norm_[p]);
            hCellEfficiencyEdgeRight1Dm3_    [p]->Divide(hCellEfficiencyEdgeRight1Dm3Norm_[p]);
            hCellEfficiencyEdgeRight1Dm5_    [p]->Divide(hCellEfficiencyEdgeRight1Dm5Norm_[p]);
            hCellEfficiencyEdgeLeft1Dp1_     [p]->Divide(hCellEfficiencyEdgeLeft1Dp1Norm_ [p]);
            hCellEfficiencyEdgeLeft1Dp2_     [p]->Divide(hCellEfficiencyEdgeLeft1Dp2Norm_ [p]);
            hCellEfficiencyEdgeLeft1Dp4_     [p]->Divide(hCellEfficiencyEdgeLeft1Dp4Norm_ [p]);
            hCellEfficiencyEdgeLeft1Dp3_     [p]->Divide(hCellEfficiencyEdgeLeft1Dp3Norm_ [p]);
            hCellEfficiencyEdgeLeft1Dp5_     [p]->Divide(hCellEfficiencyEdgeLeft1Dp5Norm_ [p]);
            hCellEfficiencyEdgeLeft1D_       [p]->Divide(hCellEfficiencyEdgeLeft1DNorm_   [p]);
            hCellEfficiencyEdgeUp_           [p]->Divide(hCellEfficiencyEdgeUpNorm_       [p]);
            hCellEfficiencyEdgeDown_         [p]->Divide(hCellEfficiencyEdgeDownNorm_     [p]);
            hCellEfficiencyEdgeLeft1D_cl1_   [p]->Divide(hCellEfficiencyEdgeLeft1DNorm_   [p]);
            hCellEfficiencyEdgeRight1D_cl1_  [p]->Divide(hCellEfficiencyEdgeRight1DNorm_  [p]);
            hCellEfficiencyEdgeCentral1D_cl1_[p]->Divide(hCellEfficiencyEdgeCentralNorm1D_[p]);
            hCellEfficiencyEdgeLeft1D_cl2_   [p]->Divide(hCellEfficiencyEdgeLeft1DNorm_   [p]);
            hCellEfficiencyEdgeRight1D_cl2_  [p]->Divide(hCellEfficiencyEdgeRight1DNorm_  [p]);
            hCellEfficiencyEdgeCentral1D_cl2_[p]->Divide(hCellEfficiencyEdgeCentralNorm1D_[p]);
            hCellEfficiencyEdgeLeft1D_cl3_   [p]->Divide(hCellEfficiencyEdgeLeft1DNorm_   [p]);
            hCellEfficiencyEdgeRight1D_cl3_  [p]->Divide(hCellEfficiencyEdgeRight1DNorm_  [p]);
            hCellEfficiencyEdgeCentral1D_cl3_[p]->Divide(hCellEfficiencyEdgeCentralNorm1D_[p]);
            hCellEfficiencyEdgeLeft1D_cl4_   [p]->Divide(hCellEfficiencyEdgeLeft1DNorm_   [p]);
            hCellEfficiencyEdgeRight1D_cl4_  [p]->Divide(hCellEfficiencyEdgeRight1DNorm_  [p]);
            hCellEfficiencyEdgeCentral1D_cl4_[p]->Divide(hCellEfficiencyEdgeCentralNorm1D_[p]);

            h2DEfficiency_                  [p]->GetXaxis()->SetTitle("column");
            h2DEfficiency_                  [p]->GetYaxis()->SetTitle("row"   );
            h2DInefficiency_                [p]->GetXaxis()->SetTitle("column");
            h2DInefficiency_                [p]->GetYaxis()->SetTitle("row"   );
            hCellEfficiency_                [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEvenColumns_     [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyOddColumns_      [p]->GetXaxis()->SetTitle("x (um)");
            h2D4cellEfficiency_             [p]->GetXaxis()->SetTitle("x (um)");
            h2D4cellEfficiencyNorm_         [p]->GetXaxis()->SetTitle("x (um)");
            h1DXcellEfficiencyFirstHit_     [p]->GetXaxis()->SetTitle("x (um)");
            h1DXcellEfficiencySecondHit_    [p]->GetXaxis()->SetTitle("x (um)");
            h1DYcellEfficiencyFirstHit_     [p]->GetXaxis()->SetTitle("y (um)");
            h1DYcellEfficiencySecondHit_    [p]->GetXaxis()->SetTitle("y (um)");
            h1DYcellEfficiency4RowsNorm_    [p]->GetXaxis()->SetTitle("y (um)");
            h1DYcellEfficiency4RowsNorm_    [p]->GetYaxis()->SetTitle("efficiency");
            h1DYcellEfficiency4Rows_        [p]->GetXaxis()->SetTitle("y (um)");
            h1DYcellEfficiency4Rows_        [p]->GetYaxis()->SetTitle("efficiency");
            hCellEfficiency_                [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEvenColumns_     [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyOddColumns_      [p]->GetYaxis()->SetTitle("y (um)");
            hXCellEfficiencyRestricted_     [p]->GetXaxis()->SetTitle("x (um)");
            hXCellEfficiencyRestricted_     [p]->GetYaxis()->SetTitle("efficiency");
            h2D4cellEfficiency_             [p]->GetYaxis()->SetTitle("y (um)");
            h2D4cellEfficiencyNorm_         [p]->GetYaxis()->SetTitle("y (um)");
            hCellInefficiency_              [p]->GetXaxis()->SetTitle("x (um)");
            hCellInefficiency_              [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRight_       [p]->GetXaxis()->SetTitle("x (um)");
            h1EfficiencyEdgeRight_          [p]->GetXaxis()->SetTitle("x (um)");
            h1EfficiencyEdgeLeft_           [p]->GetXaxis()->SetTitle("x (um)");
            h1EfficiencyEdgeUp_             [p]->GetXaxis()->SetTitle("y (um)");
            h1EfficiencyEdgeDown_           [p]->GetXaxis()->SetTitle("y (um)");
            h1EfficiencyEdgeRightNorm_      [p]->GetXaxis()->SetTitle("x (um)");
            h1EfficiencyEdgeLeftNorm_       [p]->GetXaxis()->SetTitle("x (um)");
            h1EfficiencyEdgeUpNorm_         [p]->GetXaxis()->SetTitle("y (um)");
            h1EfficiencyEdgeDownNorm_       [p]->GetXaxis()->SetTitle("y (um)");

            setErrorsBar(p);

            hCellEfficiencyEdgeCentral1D_        [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeCentralNorm1D_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1D_cl1_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1D_cl_        [p]->GetXaxis()->SetTitle("cluster size");
            hCellEfficiencyEdgeRight1D_cl1_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1D_cl_       [p]->GetXaxis()->SetTitle("cluster size");
            hCellEfficiencyEdgeCentral1D_cl1_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeCentral1D_cl_     [p]->GetXaxis()->SetTitle("cluster size");
            hCellEfficiencyEdgeLeft1D_cl2_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1DNorm_cl2_   [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1D_cl2_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1DNorm_cl2_  [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeCentral1D_cl2_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeCentralNorm1D_cl2_[p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1D_cl3_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1D_cl3_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeCentral1D_cl3_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1D_cl4_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1DNorm_cl4_   [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1D_cl4_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1DNorm_cl4_  [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeCentral1D_cl4_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeCentralNorm1D_cl4_[p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight_            [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm1_          [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm1_          [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm2_          [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm2_          [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm4_          [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm4_          [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm3_          [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm3_          [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm5_          [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm5_          [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeft_             [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft_             [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp1_           [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp1_           [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp2_           [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp2_           [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp4_           [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp4_           [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp3_           [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp3_           [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp5_           [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp5_           [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRight1D_          [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm1_        [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm2_        [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm4_        [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm3_        [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm5_        [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1D_           [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp1_         [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp2_         [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp4_         [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp3_         [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp5_         [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeUp_               [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeUp_               [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeDown_             [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeDown_             [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightNorm_        [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightNorm_        [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm1Norm_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm1Norm_      [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm2Norm_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm2Norm_      [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm3Norm_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm3Norm_      [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm4Norm_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm4Norm_      [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRightm5Norm_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRightm5Norm_      [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp1Norm_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp1Norm_       [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp2Norm_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp2Norm_       [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp3Norm_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp3Norm_       [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp4Norm_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp4Norm_       [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeLeftp5Norm_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftp5Norm_       [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeRight1DNorm_      [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm1Norm_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm2Norm_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm3Norm_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm4Norm_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeRight1Dm5Norm_    [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp1Norm_     [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp2Norm_     [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp3Norm_     [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp4Norm_     [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1Dp5Norm_     [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeft1DNorm_       [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftNorm_         [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeLeftNorm_         [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeUpNorm_           [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeUpNorm_           [p]->GetYaxis()->SetTitle("y (um)");
            hCellEfficiencyEdgeDownNorm_         [p]->GetXaxis()->SetTitle("x (um)");
            hCellEfficiencyEdgeDownNorm_         [p]->GetYaxis()->SetTitle("y (um)");

            setErrorsBar(p);

            efficiency = hEfficiencyRef_    [p]->GetBinContent(1);
            Ntrack     = hEfficiencyRefNorm_[p]->GetBinContent(1);

            error = sqrt(efficiency*(1-efficiency)/Ntrack);


            ss.str("");
            ss << "Detector: "    << std::setw(27) << thePlaneMapping_->getPlaneName(p)
               << " efficiency: " << std::setw(4) << std::setprecision(7) << efficiency*100.
               << " +- "          << std::setw(4) << std::setprecision(3) << error*100.;

            STDLINE(ss.str(),ACLightPurple);
        }
        else if(thePlaneMapping_->getPlaneName(p).find("Dut")!=std::string::npos)
        {
            std::ofstream outfile ("log.txt");
            int pointNumber = 0;
            for(std::map< int,std::vector<TH1F*> >::iterator it=scanEfficiencyHistos_.begin(); it!=scanEfficiencyHistos_.end(); ++it)
            {

                ss.str("");
                ss << "ERROR: This part must be fixed since it assumes only 8 telescope planes!!!!!!";
                STDLINE(ss.str(),ACRed);

                ADD_THREADED((it->second)[p-8]);
                ADD_THREADED((scanEfficiencyNorm_[it->first])[p-8]);
                (it->second)[p-8]->Divide((scanEfficiencyNorm_[it->first])[p-8]);
                scanGraph_[p-8]->SetPoint(pointNumber,it->first,(it->second)[p-8]->GetBinContent(1));
                pointNumber++;

                ss.str("");
                ss << "Detector: " << thePlaneMapping_->getPlaneName(p) << " - Efficiency: " << (it->second)[p-8]->GetBinContent(1) << " - Value: " << it->first;
                STDLINE(ss.str(),ACYellow);
                outfile << ss.str() << std::endl;
            }
            outfile.close();
            theAnalysisManager_->cd("Efficiency/"+thePlaneMapping_->getPlaneName(p));
            scanGraph_[p-8]->Write();
            STDLINE("",ACWhite);
            STDLINE("",ACWhite);
        }

        if(theXmlParser_->getAnalysesFromString("Efficiency")->doFits())
        {
            fitEfficiency(p);
        }
    }



}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::book(void)
{
    destroy();

//    const Window*     theWindow ;
    std::string       hName     ;
    std::stringstream value     ;
    std::string       hTitle    ;
    std::string       planeName ;
    int               nBinsX    ;
    int               nBinsY    ;
    float             xPitch ;
    float             yPitch ;
    //FIXME HARDCODED -> USE theXmlParser_->getPlanes())[planeName]->getCellPitches().first
    //FIXME HARDCODED -> USE theXmlParser_->getPlanes())[planeName]->getCellPitches().first
    float             pixelSizeLeft  = 300;
    float             pixelSizeRight = 300;
    float             pixelSizeUp    = 200;
    float             pixelSizeDown  = 100;
    float             edgeSizeLeft   = 200;
    float             edgeSizeRight  = 200;
    float             edgeSizeUp     = 200;
    float             edgeSizeDown   = 200;
    float             binSize     =     5;
    int               removedBins =     0;
    float             removeHalf  =     0;
    bool              centerBins  = false;
    if(centerBins){
        removedBins = 1;
        removeHalf = binSize/2.;
    }


    theAnalysisManager_->cd("/");
    theAnalysisManager_->mkdir("Efficiency");
    int lowerCol, higherCol, lowerRow, higherRow;
    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        planeName = thePlaneMapping_->getPlaneName(p);
//        theWindow = theWindowsManager_->getWindow(p);

        xPitch   = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
        yPitch   = atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

        theAnalysisManager_->cd("Efficiency");
        theAnalysisManager_->mkdir(planeName);

        theAnalysisManager_->mkdir("Efficiency");

        hName  = "Efficiency_"         + planeName;
        hTitle = "Overall efficiency " + planeName;
        hEfficiency_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "EfficiencyRef_"           + planeName;
        hTitle = "Overall efficiency ref. " + planeName;
        hEfficiencyRef_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "EfficiencyNorm_"                   + planeName;
        hTitle = "Overall efficiency normalization " + planeName;
        hEfficiencyNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "EfficiencyRefNorm_"                     + planeName;
        hTitle = "Overall efficiency ref. normalization " + planeName;
        hEfficiencyRefNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

        hName  = "h1DXcellEfficiencyFirstHit_"                  + planeName;
        hTitle = "1D Cell efficiency - X coordinate first hit " + planeName;
        h1DXcellEfficiencyFirstHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf)));
        //h1DXcellEfficiencyFirstHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellEfficiencySecondHit_"                  + planeName;
        hTitle = "1D cell Efficiency - X coordinate second hit " + planeName;
        h1DXcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf)));
        //h1DXcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DXcellEfficiencyNorm_"                          + planeName;
        hTitle = "1D cell Efficiency - X coordinate normalization " + planeName;
        h1DXcellEfficiencyNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf)));
        //h1DXcellEfficiencyNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/2,-(xPitch/2),xPitch/2)));

        hName  = "h1DYcellEfficiencyFirstHit_"                 + planeName;
        hTitle = "1D cell Efficiency - Y coordinate first hit " + planeName;
        h1DYcellEfficiencyFirstHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));
        //h1DYcellEfficiencyFirstHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/2 - 1,-(yPitch/2) + 1.,yPitch/2 - 1.)));

        hName  = "h1DYcellEfficiencySecondHit_"                  + planeName;
        hTitle = "1D cell Efficiency - Y coordinate second hit " + planeName;
        h1DYcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));
        // h1DYcellEfficiencySecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/2 - 1,-(yPitch/2) + 1.,yPitch/2 - 1.)));

        hName  = "h1DYcellEfficiencyNorm_"                          + planeName;
        hTitle = "1D cell Efficiency - Y coordinate normalization " + planeName;
        h1DYcellEfficiencyNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));
        //h1DYcellEfficiencyNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)yPitch/2 - 1,-(yPitch/2) + 1.,yPitch/2 - 1.)));

        hName  = "h1DYcellEfficiency4RowsNorm_"                           + planeName;
        hTitle = "1D cell Efficiency 4 Rows- Y coordinate normalization " + planeName;
        h1DYcellEfficiency4RowsNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), (int)4*(yPitch/5. - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "h1DYcellEfficiency4Rows_"                 + planeName;
        hTitle = "1D cell Efficiency 4 Rows- Y coordinate " + planeName;
        h1DYcellEfficiency4Rows_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(), (int)4*(yPitch/5. - removedBins), -4*((yPitch/2) + removeHalf), 4*(yPitch/2 - removeHalf))));

        hName  = "hXCellEfficiencyRestricted_"                  + planeName;
        hTitle = "1D cell Efficiency - X coordinate, restricted to (-30, +30), " + planeName;
        hXCellEfficiencyRestricted_.push_back(new TH1F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf));

        lowerCol  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerCol ()).c_str());
        higherCol = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherCol()).c_str());
        lowerRow  = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getLowerRow ()).c_str());
        higherRow = atoi(((theXmlParser_->getPlanes())[planeName]->getWindow()->getHigherRow()).c_str());

        nBinsX = abs(lowerCol-higherCol);//theWindow->getNbins().first;
        nBinsY = abs(lowerRow -higherRow);//theWindow->getNbins().second;

        hName  = "2DEfficiency_"               + planeName;
        hTitle = "2D efficiency distribution " + planeName;
        h2DEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));

        hName  = "2DEfficiencyRef_"                 + planeName;
        hTitle = "2D efficiency distribution ref. " + planeName;
        h2DEfficiencyRef_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));

        hName  = "2DEfficiencyNorm_"         + planeName;
        hTitle = "2D efficiency normalization " + planeName;
        h2DEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));

        hName  = "2DEfficiencyRefNorm_"              + planeName;
        hTitle = "2D efficiency ref. normalization " + planeName;
        h2DEfficiencyRefNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));

        hName  = "2DInefficiency_"  + planeName;
        hTitle = "2D inefficiency " + planeName;
        h2DInefficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),nBinsX,lowerCol,higherCol,nBinsY,lowerRow,higherRow)));


        theAnalysisManager_->cd("Efficiency/" + planeName);
        theAnalysisManager_->mkdir("CellEfficiency");

        hName  =  "CellEfficiencyNorm_"            + planeName;
        hTitle =  "Cell efficiency normalization " + planeName;
        hCellEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));

        hName  =  "CellEfficiencyEvenColumnsNorm_"            + planeName;
        hTitle =  "Cell efficiency normalization even columns" + planeName;
        hCellEfficiencyEvenColumnsNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));

        hName  =  "CellEfficiencyOddColumnsNorm_"            + planeName;
        hTitle =  "Cell efficiency normalization odd columns" + planeName;
        hCellEfficiencyOddColumnsNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));

        hName  = "CellEfficiency_"  + planeName;
        hTitle = "Cell efficiency " + planeName;
        hCellEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));

        hName  = "CellEfficiencyEvenColumns_"  + planeName;
        hTitle = "Cell efficiency even columns" + planeName;
        hCellEfficiencyEvenColumns_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));

        hName  = "CellEfficiencyOddColumns_"  + planeName;
        hTitle = "Cell efficiency odd columns" + planeName;
        hCellEfficiencyOddColumns_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));

        hName  = "4cellEfficiency_"  + planeName;
        hTitle = "4 Cell efficiency " + planeName;
        h2D4cellEfficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));

        hName  = "4cellEfficiencyNorm_"  + planeName;
        hTitle = "4 Cell efficiency normalization " + planeName;
        h2D4cellEfficiencyNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));

        hName  = "CellInefficiency_"  + planeName;
        hTitle = "Cell inefficiency " + planeName;
        hCellInefficiency_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/binSize - removedBins,-(xPitch/2) + removeHalf,xPitch/2 - removeHalf,(int)yPitch/binSize - removedBins,-(yPitch/2) + removeHalf,yPitch/2 - removeHalf)));


        theAnalysisManager_->cd("Efficiency/" + planeName);
        theAnalysisManager_->mkdir("EdgeEfficiency");

        hName  = "h1EfficiencyEdgeRight_"  + planeName;
        hTitle = "Efficiency on the right edge (col 51), " + planeName;
        h1EfficiencyEdgeRight_.    push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(pixelSizeRight + edgeSizeRight)/binSize,-(pixelSizeRight/2.),pixelSizeRight/2. + edgeSizeRight)));

        hName  = "h1EfficiencyEdgeLeft_"  + planeName;
        hTitle = "Efficiency on the left edge (col 0), " + planeName;
        h1EfficiencyEdgeLeft_.     push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(pixelSizeLeft +  edgeSizeLeft)/binSize ,-(pixelSizeLeft/2.)   - edgeSizeLeft ,pixelSizeLeft/2.)));

        hName  = "h1lEfficiencyEdgeUp_"  + planeName;
        hTitle = "Efficiency on the upper edge (row 79), " + planeName;
        h1EfficiencyEdgeUp_.       push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(pixelSizeUp   +  edgeSizeUp)/binSize   ,-(pixelSizeUp/2.)   ,pixelSizeUp/2.    + edgeSizeUp   )));

        hName  = "h1EfficiencyEdgeDown_"  + planeName;
        hTitle = "Efficiency on the lower edge (row 0), " + planeName;
        h1EfficiencyEdgeDown_.     push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(pixelSizeDown +  edgeSizeDown)/binSize ,-(pixelSizeDown/2.)   - edgeSizeDown ,pixelSizeDown/2.)));

        hName  = "h1EfficiencyEdgeRightNorm_"  + planeName;
        hTitle = "Efficiency normalization on the right edge, " + planeName;
        h1EfficiencyEdgeRightNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(pixelSizeRight + edgeSizeRight)/binSize,-(pixelSizeRight/2.),pixelSizeRight/2. + edgeSizeRight)));

        hName  = "h1EfficiencyEdgeLeftNorm_"  + planeName;
        hTitle = "Efficiency normalization normalization on the left edge, " + planeName;
        h1EfficiencyEdgeLeftNorm_. push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(pixelSizeLeft +  edgeSizeLeft)/binSize ,-(pixelSizeLeft/2.)   - edgeSizeLeft ,pixelSizeLeft/2.)));

        hName  = "h1EfficiencyEdgeUpNorm_"  + planeName;
        hTitle = "Efficiency normalization on the upper edge, " + planeName;
        h1EfficiencyEdgeUpNorm_.   push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(pixelSizeUp   +  edgeSizeUp)/binSize   ,-(pixelSizeUp/2.)   ,pixelSizeUp/2.    + edgeSizeUp   )));

        hName  = "h1EfficiencyEdgeDownNorm_"  + planeName;
        hTitle = "Efficiency normalization on the lower edge, " + planeName;
        h1EfficiencyEdgeDownNorm_. push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)(pixelSizeDown +  edgeSizeDown)/binSize ,-(pixelSizeDown/2.)   - edgeSizeDown ,pixelSizeDown/2.)));


        theAnalysisManager_->cd("Efficiency/" + planeName);
        theAnalysisManager_->mkdir("ColumnPlots");

        hName  = "CellEfficiencyEdgeRight_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1D_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight1D_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1D_cl1_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight1D_cl1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1D_cl_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight1D_cl_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),10, 0., 10.)));

        hName  = "CellEfficiencyEdgeRight1D_cl2_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight1D_cl2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1D_cl3_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight1D_cl3_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1D_cl4_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight1D_cl4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1DNorm_cl4_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight1DNorm_cl4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1DNorm_cl2_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeRight1DNorm_cl2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRightNorm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge, " + planeName;
        hCellEfficiencyEdgeRightNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1DNorm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge, " + planeName;
        hCellEfficiencyEdgeRight1DNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm1_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 50), " + planeName;
        hCellEfficiencyEdgeRightm1_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm3_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 48), " + planeName;
        hCellEfficiencyEdgeRightm3_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm5_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 46), " + planeName;
        hCellEfficiencyEdgeRightm5_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm2_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 49), " + planeName;
        hCellEfficiencyEdgeRightm2_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm4_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 47), " + planeName;
        hCellEfficiencyEdgeRightm4_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeCentral1D_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentral1D_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeCentralNorm1D_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentralNorm1D_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeCentral1D_cl1_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentral1D_cl1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeCentral1D_cl_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentral1D_cl_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),10, 0., 10.)));

        hName  = "CellEfficiencyEdgeCentral1D_cl2_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentral1D_cl2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeCentralNorm1D_cl2_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentralNorm1D_cl2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1D_cl1_"  + planeName;
        hTitle = "Cell efficiency on the Left edge (col 51), " + planeName;
        hCellEfficiencyEdgeLeft1D_cl1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1D_cl_"  + planeName;
        hTitle = "Cell efficiency on the Left edge (col 51), " + planeName;
        hCellEfficiencyEdgeLeft1D_cl_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),10, 0., 10.)));

        hName  = "CellEfficiencyEdgeLeft1D_cl2_"  + planeName;
        hTitle = "Cell efficiency on the Left edge (col 51), " + planeName;
        hCellEfficiencyEdgeLeft1D_cl2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1DNorm_cl2_"  + planeName;
        hTitle = "Cell efficiency on the Left edge (col 51), " + planeName;
        hCellEfficiencyEdgeLeft1DNorm_cl2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeCentral1D_cl3_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentral1D_cl3_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeCentral1D_cl4_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentral1D_cl4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeCentralNorm1D_cl4_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 51), " + planeName;
        hCellEfficiencyEdgeCentralNorm1D_cl4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1D_cl3_"  + planeName;
        hTitle = "Cell efficiency on the Left edge (col 51), " + planeName;
        hCellEfficiencyEdgeLeft1D_cl3_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1D_cl4_"  + planeName;
        hTitle = "Cell efficiency on the Left edge (col 51), " + planeName;
        hCellEfficiencyEdgeLeft1D_cl4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1DNorm_cl4_"  + planeName;
        hTitle = "Cell efficiency on the Left edge (col 51), " + planeName;
        hCellEfficiencyEdgeLeft1DNorm_cl4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm1_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 50), " + planeName;
        hCellEfficiencyEdgeRight1Dm1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm3_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 48), " + planeName;
        hCellEfficiencyEdgeRight1Dm3_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm5_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 46), " + planeName;
        hCellEfficiencyEdgeRight1Dm5_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm2_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 49), " + planeName;
        hCellEfficiencyEdgeRight1Dm2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm4_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 47), " + planeName;
        hCellEfficiencyEdgeRight1Dm4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp1_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 1), " + planeName;
        hCellEfficiencyEdgeLeftp1_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp3_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 3), " + planeName;
        hCellEfficiencyEdgeLeftp3_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp5_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 5), " + planeName;
        hCellEfficiencyEdgeLeftp5_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp2_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 2), " + planeName;
        hCellEfficiencyEdgeLeftp2_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp4_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 4), " + planeName;
        hCellEfficiencyEdgeLeftp4_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft_"  + planeName;
        hTitle = "Cell efficiency on the left edge (col 0), " + planeName;
        hCellEfficiencyEdgeLeft_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp1_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 1), " + planeName;
        hCellEfficiencyEdgeLeft1Dp1_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp3_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 3), " + planeName;
        hCellEfficiencyEdgeLeft1Dp3_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp5_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 5), " + planeName;
        hCellEfficiencyEdgeLeft1Dp5_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp2_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 2), " + planeName;
        hCellEfficiencyEdgeLeft1Dp2_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp4_"  + planeName;
        hTitle = "Cell efficiency on the right edge (col 4), " + planeName;
        hCellEfficiencyEdgeLeft1Dp4_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1D_"  + planeName;
        hTitle = "Cell efficiency on the left edge (col 0), " + planeName;
        hCellEfficiencyEdgeLeft1D_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeUp_"  + planeName;
        hTitle = "Cell efficiency on the upper edge (row 79), " + planeName;
        hCellEfficiencyEdgeUp_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/5 - 1,-(xPitch/2) + 2.5,xPitch/2 - 2.5,(int)yPitch/5 - 1,-(yPitch/2) + 2.5,yPitch/2 - 2.5)));

        hName  = "CellEfficiencyEdgeDown_"  + planeName;
        hTitle = "Cell efficiency on the lower edge (row 0), " + planeName;
        hCellEfficiencyEdgeDown_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/5 - 1,-(xPitch/2) + 2.5,xPitch/2 - 2.5,(int)yPitch/5 - 1,-(yPitch/2) + 2.5,yPitch/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm1Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 1 row, " + planeName;
        hCellEfficiencyEdgeRightm1Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm3Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 3 rows, " + planeName;
        hCellEfficiencyEdgeRightm3Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm5Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 5 rows, " + planeName;
        hCellEfficiencyEdgeRightm5Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm2Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 2 rows, " + planeName;
        hCellEfficiencyEdgeRightm2Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRightm4Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 4 rows, " + planeName;
        hCellEfficiencyEdgeRightm4Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp1Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 1 row, " + planeName;
        hCellEfficiencyEdgeLeftp1Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp3Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 3 rows, " + planeName;
        hCellEfficiencyEdgeLeftp3Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp5Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 5 rows, " + planeName;
        hCellEfficiencyEdgeLeftp5Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp2Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 2 rows, " + planeName;
        hCellEfficiencyEdgeLeftp2Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftp4Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 4 rows, " + planeName;
        hCellEfficiencyEdgeLeftp4Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeftNorm_"  + planeName;
        hTitle = "Cell efficiency normalization normalization on the left edge, " + planeName;
        hCellEfficiencyEdgeLeftNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch)*3/2 + 2.5,xPitch*3/2 - 2.5,(int)3*yPitch/5 - 1,-(yPitch*3/2) + 2.5,yPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm1Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 1 row, " + planeName;
        hCellEfficiencyEdgeRight1Dm1Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm3Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 3 rows, " + planeName;
        hCellEfficiencyEdgeRight1Dm3Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm5Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 5 rows, " + planeName;
        hCellEfficiencyEdgeRight1Dm5Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm2Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 2 rows, " + planeName;
        hCellEfficiencyEdgeRight1Dm2Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeRight1Dm4Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the right edge minus 4 rows, " + planeName;
        hCellEfficiencyEdgeRight1Dm4Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp1Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 1 row, " + planeName;
        hCellEfficiencyEdgeLeft1Dp1Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp3Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 3 rows, " + planeName;
        hCellEfficiencyEdgeLeft1Dp3Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp5Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 5 rows, " + planeName;
        hCellEfficiencyEdgeLeft1Dp5Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp2Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 2 rows, " + planeName;
        hCellEfficiencyEdgeLeft1Dp2Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1Dp4Norm_"  + planeName;
        hTitle = "Cell efficiency normalization on the left edge plus 4 rows, " + planeName;
        hCellEfficiencyEdgeLeft1Dp4Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5 - 1,-(xPitch*3/2) + 2.5,xPitch*3/2 - 2.5)));

        hName  = "CellEfficiencyEdgeLeft1DNorm_"  + planeName;
        hTitle = "Cell efficiency normalization normalization on the left edge, " + planeName;
        hCellEfficiencyEdgeLeft1DNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),(int)3*xPitch/5*10,-(xPitch*3/2)*5 + 2.5,(xPitch*3/2)*5 - 2.5)));

        hName  = "CellEfficiencyEdgeUpNorm_"  + planeName;
        hTitle = "Cell efficiency normalization on the upper edge, " + planeName;
        hCellEfficiencyEdgeUpNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/5 - 1,-(xPitch/2) + 2.5,xPitch/2 - 2.5,(int)yPitch/5 - 1,-(yPitch/2) + 2.5,yPitch/2 - 2.5)));

        hName  = "CellEfficiencyEdgeDownNorm_"  + planeName;
        hTitle = "Cell efficiency normalization on the lower edge, " + planeName;
        hCellEfficiencyEdgeDownNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(),hTitle.c_str(),(int)xPitch/5 - 1,-(xPitch/2) + 2.5,xPitch/2 - 2.5,(int)yPitch/5 - 1,-(yPitch/2) + 2.5,yPitch/2 - 2.5)));

    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::scanBook(void)
{
    std::string       hName     ;
    std::stringstream value     ;
    std::string       hTitle    ;
    std::string       planeName ;

    std::map<std::string,int> scanValues = theXmlParser_->getScan()->getScanValues();

    std::vector<TH1F*> hEfficiencyValueDenom;
    std::vector<TH1F*> hEfficiencyValue     ;
    TGraph* tmp = 0;


    theAnalysisManager_->cd("/");
    theAnalysisManager_->mkdir("Efficiency");

    for(std::map<std::string,int>::iterator it=scanValues.begin(); it!=scanValues.end(); it++)
    {
        hEfficiencyValueDenom.clear();
        hEfficiencyValue.clear();

        for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
        {
            planeName = thePlaneMapping_->getPlaneName(p);
            value.str(""); value << it->second;

            if(planeName.find("Dut")!=std::string::npos)
            {
                theAnalysisManager_->cd("Efficiency");
                theAnalysisManager_->mkdir(planeName);

                hName  = "EfficiencyNorm_" + value.str() + "_" + planeName;
                hTitle = "EfficiencyNorm_" + value.str() + "_" + planeName;
                hEfficiencyValueDenom.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

                hName  = "Efficiency_" + value.str() + "_" + planeName;
                hTitle = "Efficiency_" + value.str() + "_" + planeName;
                hEfficiencyValue.push_back(NEW_THREADED(TH1F(hName.c_str(),hTitle.c_str(),1,.5,1.5)));

                hName  = "EfficiencyScan_" + planeName;
                hTitle = "Efficiency scan " + planeName;
                tmp = new TGraph();
                tmp->SetName(hName.c_str());
                tmp->SetTitle(hTitle.c_str());
                tmp->SetMarkerStyle(23);
                tmp->SetMarkerColor(kRed);
                tmp->SetMarkerSize(1.5);
                tmp->SetLineWidth(2);
                tmp->SetLineColor(kBlue);
                scanGraph_.push_back(tmp);
                delete tmp;
            }
        }
        scanEfficiencyNorm_ [atoi(value.str().c_str())] = hEfficiencyValueDenom;
        scanEfficiencyHistos_[atoi(value.str().c_str())] = hEfficiencyValue     ;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::setErrorsBar(int planeID)
{
    double efficiency;
    double Ntrack;
    double error;
    int    nBins;

    nBins = h1DXcellEfficiencyFirstHit_[planeID]->GetNbinsX();
    for(int b=1; b<=nBins; b++)
    {
        efficiency = h1DXcellEfficiencyFirstHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency*(1-efficiency)/Ntrack);
        h1DXcellEfficiencyFirstHit_[planeID]->SetBinError(b,error);
        efficiency = h1DXcellEfficiencySecondHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DXcellEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency*(1-efficiency)/Ntrack);
        h1DXcellEfficiencySecondHit_[planeID]->SetBinError(b,error);
    }

    nBins = h1DYcellEfficiencyFirstHit_[planeID]->GetNbinsX();
    for(int b=1; b<=nBins; b++)
    {
        efficiency = h1DYcellEfficiencyFirstHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency*(1-efficiency)/Ntrack);
        h1DYcellEfficiencyFirstHit_[planeID]->SetBinError(b,error);
        efficiency = h1DYcellEfficiencySecondHit_[planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEfficiencyNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency*(1-efficiency)/Ntrack);
        h1DYcellEfficiencySecondHit_[planeID]->SetBinError(b,error);
    }
    nBins = h1DYcellEfficiency4Rows_[planeID]->GetNbinsX();
    for(int b=1; b<=nBins; b++)
    {
        efficiency = h1DYcellEfficiency4Rows_    [planeID]->GetBinContent(b);
        Ntrack     = h1DYcellEfficiency4RowsNorm_[planeID]->GetBinContent(b);
        error      = sqrt(efficiency*(1-efficiency)/Ntrack);
        h1DYcellEfficiency4Rows_                  [planeID]->SetBinError(b,error);
    }
    nBins = hCellEfficiency_[planeID]->GetNbinsX();
    for (int b = 1; b < nBins+1; b++)
    {
        efficiency = 0;
        Ntrack = 0;
        for (int by = hCellEfficiency_[planeID]->GetYaxis()->FindBin(-30); by < hCellEfficiency_[planeID]->GetYaxis()->FindBin(30); by++)
        {
            efficiency += hCellEfficiency_[planeID]->GetBinContent(b, by);
            Ntrack += 1;
        }
        efficiency = efficiency/Ntrack;
        error = sqrt(efficiency*(1-efficiency)/Ntrack);
        hXCellEfficiencyRestricted_[planeID]->SetBinContent(b, efficiency);
        hXCellEfficiencyRestricted_[planeID]->SetBinError(b, error);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::scanEfficiency(int planeID, int value, const Data& data, int threadNumber)
{
    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    //	float rowMean   = data.getMeanRow(planeID);
    //	float colMean   = data.getMeanCol(planeID);
    float row       = data.getRowPredicted(planeID);
    float col       = data.getColPredicted(planeID);
    int   event = data.getEventChewieNumber();
    int   run   = data.getRunNumber();
    int   hits  = 0;

    for(unsigned int pp=0; pp<thePlaneMapping_->getNumberOfPlanes(); pp++)
    {
        if(data.getHasHit(pp) && thePlaneMapping_->getPlaneName(pp).find("Telescope") != std::string::npos) hits++;
    }

    if(theWindow->checkWindowAbout(col,row,run,thePlaneMapping_->getPlaneType(planeID)) && theWindow->checkTimeWindowAbout(col,event,run) && hits>=8)
        //if(theWindow->checkWindow(col,row) && hits>=8)
    {
        THREADED((scanEfficiencyNorm_[value])[planeID-8])->Fill(1);
        if(data.getHasHit(planeID))
            THREADED((scanEfficiencyHistos_[value])[planeID-8])->Fill(1);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::planeEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
    if(!pass || !data.getIsInDetector(planeID))
        return;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    //        float         rowMean   = data.getMeanRow(planeID);
    //        float         colMean   = data.getMeanCol(planeID);
    float         row       = data.getRowPredicted(planeID);
    float         col       = data.getColPredicted(planeID);
    //int           event     = data.getEventChewieNumber();
    int           run       = data.getRunNumber();

    //if(theWindow->checkWindowAbout(col,row,run) && theWindow->checkTimeWindowAbout(col,event,run))
    //std::cout << __PRETTY_FUNCTION__ << "Row: " << row << " Column: " << col << std::endl;
    if(theWindow->checkWindowAbout(col,row,run,thePlaneMapping_->getPlaneType(planeID))) //Only calculates efficiencies for cells that register hits
    {
        THREADED(hEfficiencyNorm_  [planeID])->Fill(1);
        THREADED(h2DEfficiencyNorm_[planeID])->Fill(col,row);
        if(data.getHasHit(planeID))
        {
            THREADED(hEfficiency_  [planeID])->Fill(1);
            THREADED(h2DEfficiency_[planeID])->Fill(col,row);
        }
        else
        {
            //std::cout << __PRETTY_FUNCTION__  << "Inefficient: IMPOSSIBLE!!!!! " << "Row: " << row << " Column: " << col << std::endl;
            THREADED(h2DInefficiency_[planeID])->Fill(col,row);
        }



    }



    // @@@ Please do not modify this code @@@
    if (!theWindow->checkWindow(col,row,run)) return;

    THREADED(hEfficiencyRefNorm_  [planeID])->Fill(1);
    THREADED(h2DEfficiencyRefNorm_[planeID])->Fill(col,row);
    
    if (data.getHasHit(planeID))
      {
	THREADED(hEfficiencyRef_  [planeID])->Fill(1);
	THREADED(h2DEfficiencyRef_[planeID])->Fill(col,row);
      }
    // ======================================
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::cellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
    if(!pass || !data.getIsInDetector(planeID))
        return;

    const  Window* theWindow = theWindowsManager_->getWindow(planeID);
    float          xRes      = data.getXPixelResidualLocal(planeID)  ;
    float          yRes      = data.getYPixelResidualLocal(planeID)  ;
    float          row       = data.getRowPredicted(planeID)         ;
    float          col       = data.getColPredicted(planeID)         ;
    //int            event     = data.getEventChewieNumber()           ;
    int            run       = data.getRunNumber()                   ;
    float          maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
    float          maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());

    if (data.getColPredicted(planeID) == 51 && data.getRowPredicted(planeID) != 79 )
    {
        THREADED(hCellEfficiencyEdgeRightNorm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeRight1DNorm_[planeID])->Fill(xRes+75.);
        THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID]->FindBin(xRes+75.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeRight1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            THREADED(hCellEfficiencyEdgeRight1D_[planeID])->Fill(xRes+75.);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeRight1D_cl1_[planeID])->Fill(xRes+75.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1D_cl2_[planeID])->Fill(xRes+75.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeRight1D_cl3_[planeID])->Fill(xRes+75.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeRight1D_cl4_[planeID])->Fill(xRes+75.);

            THREADED(hCellEfficiencyEdgeRight_[planeID])->Fill(xRes,yRes);
        }
    }

    if (data.getColPredicted(planeID) == 50 && data.getRowPredicted(planeID) != 79)
    {
        THREADED(hCellEfficiencyEdgeRightm1Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeRight1DNorm_[planeID])->Fill(xRes-1*150);
        if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeRight1D_cl_[planeID])->Fill(xRes-1*150.);
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->Fill(xRes-1*150.);
        THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID]->FindBin(xRes-1*150.), data.getClusterSize(planeID));

        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeRightm1_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeRight1D_[planeID])->Fill(xRes-1*150);
            THREADED(hCellEfficiencyEdgeRight1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeRight1D_cl1_[planeID])->Fill(xRes-1*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1D_cl2_[planeID])->Fill(xRes-1*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeRight1D_cl3_[planeID])->Fill(xRes-1*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeRight1D_cl4_[planeID])->Fill(xRes-1*150.);
        }
    }

    if (data.getColPredicted(planeID) == 48 && data.getRowPredicted(planeID) != 79)
    {
        THREADED(hCellEfficiencyEdgeRightm3Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeRight1DNorm_[planeID])->Fill(xRes-3*150);
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->Fill(xRes-3*150.);
        THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID]->FindBin(xRes-3*300.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeRight1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            THREADED(hCellEfficiencyEdgeRightm3_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeRight1D_[planeID])->Fill(xRes-3*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeRight1D_cl1_[planeID])->Fill(xRes-3*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1D_cl2_[planeID])->Fill(xRes-3*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeRight1D_cl3_[planeID])->Fill(xRes-3*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeRight1D_cl4_[planeID])->Fill(xRes-3*150.);
        }
    }

    if (data.getColPredicted(planeID) == 46 && data.getRowPredicted(planeID) != 79)
    {
        THREADED(hCellEfficiencyEdgeRightm5Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeRight1DNorm_[planeID])->Fill(xRes-5*150);
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->Fill(xRes-5*300.);
        THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID]->FindBin(xRes-5*300.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeRightm5_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeRight1D_[planeID])->Fill(xRes-5*150);
            THREADED(hCellEfficiencyEdgeRight1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeRight1D_cl1_[planeID])->Fill(xRes-5*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1D_cl2_[planeID])->Fill(xRes-5*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeRight1D_cl3_[planeID])->Fill(xRes-5*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeRight1D_cl4_[planeID])->Fill(xRes-5*150.);

        }
    }

    if (data.getColPredicted(planeID) == 47 && data.getRowPredicted(planeID) != 79)
    {
        THREADED(hCellEfficiencyEdgeRightm4Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeRight1DNorm_[planeID])->Fill(xRes-4*150);
        THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID]->FindBin(xRes-4*150.), data.getClusterSize(planeID));
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->Fill(xRes-4*150.);
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeRightm4_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeRight1D_[planeID])->Fill(xRes-4*150);
            THREADED(hCellEfficiencyEdgeRight1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeRight1D_cl1_[planeID])->Fill(xRes-4*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1D_cl2_[planeID])->Fill(xRes-4*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeRight1D_cl3_[planeID])->Fill(xRes-4*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeRight1D_cl4_[planeID])->Fill(xRes-4*150.);
        }
    }

    if (data.getColPredicted(planeID) == 49 && data.getRowPredicted(planeID) != 79)
    {
        THREADED(hCellEfficiencyEdgeRightm2Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeRight1DNorm_[planeID])->Fill(xRes-2*150);
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->Fill(xRes-2*150.);
        THREADED(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeRight1DNorm_cl2_[planeID]->FindBin(xRes-2*150.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeRight1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            THREADED(hCellEfficiencyEdgeRightm2_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeRight1D_[planeID])->Fill(xRes-2*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeRight1D_cl1_[planeID])->Fill(xRes-2*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeRight1D_cl2_[planeID])->Fill(xRes-2*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeRight1D_cl3_[planeID])->Fill(xRes-2*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeRight1D_cl4_[planeID])->Fill(xRes-2*150.);
        }
    }

    if (data.getColPredicted(planeID) == 0 && data.getRowPredicted(planeID) != 79 )
    {
        THREADED(hCellEfficiencyEdgeLeftNorm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_[planeID])->Fill(-xRes+75.);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID]->FindBin(-xRes+75.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeLeft_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeLeft1D_[planeID])->Fill(-xRes+75.);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeLeft1D_cl1_[planeID])->Fill(-xRes+75.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeLeft1D_cl2_[planeID])->Fill(-xRes+75.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeLeft1D_cl3_[planeID])->Fill(-xRes+75.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1D_cl4_[planeID])->Fill(-xRes+75.);

            THREADED(hCellEfficiencyEdgeLeft1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            //if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->Fill(-xRes+75.);
        }
    }

    if (data.getColPredicted(planeID) == 1 && data.getRowPredicted(planeID) != 79 )
    {
        THREADED(hCellEfficiencyEdgeLeftp1Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_[planeID])->Fill(-xRes-1*150);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID]->FindBin(-xRes-1*150.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeLeftp1_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeLeft1D_[planeID])->Fill(-xRes-1*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeLeft1D_cl1_[planeID])->Fill(-xRes-1*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeLeft1D_cl2_[planeID])->Fill(-xRes-1*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeLeft1D_cl3_[planeID])->Fill(-xRes-1*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1D_cl4_[planeID])->Fill(-xRes-1*150.);

            THREADED(hCellEfficiencyEdgeLeft1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            //if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->Fill(-xRes-1*150.);


        }
    }

    if (data.getColPredicted(planeID) == 2 && data.getRowPredicted(planeID) != 79 )
    {
        THREADED(hCellEfficiencyEdgeLeftp2Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_[planeID])->Fill(-xRes-2*150);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID]->FindBin(-xRes-2*150.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeLeftp2_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeLeft1D_[planeID])->Fill(-xRes-2*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeLeft1D_cl1_[planeID])->Fill(-xRes-2*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeLeft1D_cl2_[planeID])->Fill(-xRes-2*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeLeft1D_cl3_[planeID])->Fill(-xRes-2*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1D_cl4_[planeID])->Fill(-xRes-2*150.);

            THREADED(hCellEfficiencyEdgeLeft1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            //if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->Fill(-xRes-2*150.);


        }
    }

    if (data.getColPredicted(planeID) == 3 && data.getRowPredicted(planeID) != 79 )
    {
        THREADED(hCellEfficiencyEdgeLeftp3Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_[planeID])->Fill(-xRes-3*150);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID]->FindBin(-xRes-3*150.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeLeftp3_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeLeft1D_[planeID])->Fill(-xRes-3*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeLeft1D_cl1_[planeID])->Fill(-xRes-3*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeLeft1D_cl2_[planeID])->Fill(-xRes-3*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeLeft1D_cl3_[planeID])->Fill(-xRes-3*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1D_cl4_[planeID])->Fill(-xRes-3*150.);

            THREADED(hCellEfficiencyEdgeLeft1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            //if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->Fill(-xRes-3*150.);

        }
    }

    if (data.getColPredicted(planeID) == 4 && data.getRowPredicted(planeID) != 79 )
    {
        THREADED(hCellEfficiencyEdgeLeftp4Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_[planeID])->Fill(-xRes-4*150);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID]->FindBin(-xRes-4*150.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeLeftp4_[planeID])->Fill(xRes,yRes);
            THREADED(hCellEfficiencyEdgeLeft1D_[planeID])->Fill(-xRes-4*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeLeft1D_cl1_[planeID])->Fill(-xRes-4*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeLeft1D_cl2_[planeID])->Fill(-xRes-4*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeLeft1D_cl3_[planeID])->Fill(-xRes-4*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1D_cl4_[planeID])->Fill(-xRes-4*150.);

            THREADED(hCellEfficiencyEdgeLeft1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            //if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->Fill(-xRes-4*150.);

        }

    }

    if (data.getColPredicted(planeID) == 5 && data.getRowPredicted(planeID) != 79 )
    {
        THREADED(hCellEfficiencyEdgeLeftp5Norm_[planeID])->Fill(xRes,yRes);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_[planeID])->Fill(-xRes-5*150);
        THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID]->FindBin(-xRes-5*150.), data.getClusterSize(planeID));
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeLeftp5_[planeID])->Fill(xRes,yRes);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeLeft1D_cl1_[planeID])->Fill(-xRes-5*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeLeft1D_cl2_[planeID])->Fill(-xRes-5*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeLeft1D_cl3_[planeID])->Fill(-xRes-5*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1D_cl4_[planeID])->Fill(-xRes-5*150.);
            THREADED(hCellEfficiencyEdgeLeft1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            //	if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeLeft1DNorm_cl2_[planeID])->Fill(-xRes-5*150.);

            THREADED(hCellEfficiencyEdgeLeft1D_[planeID])->Fill(-xRes-5*150);
        }

    }

    if (data.getColPredicted(planeID) == 25 && data.getRowPredicted(planeID) != 79 )
    {
        THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID]->FindBin(xRes-3*150.), data.getClusterSize(planeID));
        THREADED(hCellEfficiencyEdgeCentralNorm1D_[planeID])->Fill(xRes-3*150);
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->Fill(xRes-3*150.);
        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeCentral1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            THREADED(hCellEfficiencyEdgeCentral1D_[planeID])->Fill(xRes-3*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeCentral1D_cl1_[planeID])->Fill(xRes-3*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentral1D_cl2_[planeID])->Fill(xRes-3*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeCentral1D_cl3_[planeID])->Fill(xRes-3*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeCentral1D_cl4_[planeID])->Fill(xRes-3*150.);

        }

    }

    if (data.getColPredicted(planeID) == 26 && data.getRowPredicted(planeID) != 79 )
    {

        THREADED(hCellEfficiencyEdgeCentralNorm1D_[planeID])->Fill(xRes-4*150);
        THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID]->FindBin(xRes-4*150.), data.getClusterSize(planeID));
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->Fill(xRes-4*150.);

        if(data.getHasHit(planeID)){

            THREADED(hCellEfficiencyEdgeCentral1D_[planeID])->Fill(xRes-4*150);
            THREADED(hCellEfficiencyEdgeCentral1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeCentral1D_cl1_[planeID])->Fill(xRes-4*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentral1D_cl2_[planeID])->Fill(xRes-4*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeCentral1D_cl3_[planeID])->Fill(xRes-4*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeCentral1D_cl4_[planeID])->Fill(xRes-4*150.);


        }

    }

    if (data.getColPredicted(planeID) == 27 && data.getRowPredicted(planeID) != 79 )
    {

        THREADED(hCellEfficiencyEdgeCentralNorm1D_[planeID])->Fill(xRes-5*150);
        THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID]->FindBin(xRes-5*150.), data.getClusterSize(planeID));
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->Fill(xRes-5*150.);

        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeCentral1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            THREADED(hCellEfficiencyEdgeCentral1D_[planeID])->Fill(xRes-5*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeCentral1D_cl1_[planeID])->Fill(xRes-5*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentral1D_cl2_[planeID])->Fill(xRes-5*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeCentral1D_cl3_[planeID])->Fill(xRes-5*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeCentral1D_cl4_[planeID])->Fill(xRes-5*150.);

        }

    }

    if (data.getColPredicted(planeID) == 24 && data.getRowPredicted(planeID) != 79 )
    {

        THREADED(hCellEfficiencyEdgeCentralNorm1D_[planeID])->Fill(xRes-2*150);
        //if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->Fill(xRes-2*150.);
        THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID]->FindBin(xRes-2*150.), data.getClusterSize(planeID));

        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeCentral1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            THREADED(hCellEfficiencyEdgeCentral1D_[planeID])->Fill(xRes-2*150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeCentral1D_cl1_[planeID])->Fill(xRes-2*150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentral1D_cl2_[planeID])->Fill(xRes-2*150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeCentral1D_cl3_[planeID])->Fill(xRes-2*150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeCentral1D_cl4_[planeID])->Fill(xRes-2*150.);


        }

    }

    if (data.getColPredicted(planeID) == 23 && data.getRowPredicted(planeID) != 79 )
    {

        THREADED(hCellEfficiencyEdgeCentralNorm1D_[planeID])->Fill(xRes-150);
        //		if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->Fill(xRes-150.);
        THREADED(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID])->SetBinContent(hCellEfficiencyEdgeCentralNorm1D_cl2_[planeID]->FindBin(xRes-150.), data.getClusterSize(planeID));

        if(data.getHasHit(planeID)){
            THREADED(hCellEfficiencyEdgeCentral1D_cl_[planeID])->Fill(data.getClusterSize(planeID));
            THREADED(hCellEfficiencyEdgeCentral1D_[planeID])->Fill(xRes-150);
            if(data.getClusterSize(planeID)==1) THREADED(hCellEfficiencyEdgeCentral1D_cl1_[planeID])->Fill(xRes-150.);
            if(data.getClusterSize(planeID)==2) THREADED(hCellEfficiencyEdgeCentral1D_cl2_[planeID])->Fill(xRes-150.);
            if(data.getClusterSize(planeID)==3) THREADED(hCellEfficiencyEdgeCentral1D_cl3_[planeID])->Fill(xRes-150.);
            if(data.getClusterSize(planeID)==4) THREADED(hCellEfficiencyEdgeCentral1D_cl4_[planeID])->Fill(xRes-150.);


        }

    }

    if (data.getRowPredicted(planeID) == 79)
    {
        THREADED(hCellEfficiencyEdgeUpNorm_[planeID])->Fill(xRes,yRes);
        if(data.getHasHit(planeID))
            THREADED(hCellEfficiencyEdgeUp_[planeID])->Fill(xRes,yRes);
    }

    if (data.getRowPredicted(planeID) == 0)
    {
        THREADED(hCellEfficiencyEdgeDownNorm_[planeID])->Fill(xRes,yRes);
        if(data.getHasHit(planeID))
            THREADED(hCellEfficiencyEdgeDown_[planeID])->Fill(xRes,yRes);
    }


    float xRes2 = 0;
    float yRes2 = 0;

    if( data.getXPixelResidualLocal(planeID) > 0 )
        xRes2 = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
    else if( data.getXPixelResidualLocal(planeID) <= 0 )
        xRes2 = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);

    if( data.getYPixelResidualLocal(planeID) > 0 )
        yRes2 = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
    else if( data.getYPixelResidualLocal(planeID) <= 0 )
        yRes2 = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);


    //if(theWindow->checkWindowAbout(col,row,run) && theWindow->checkTimeWindowAbout(col,event,run) && data.getXPitchLocal(planeID)<=maxPitchX && data.getYPitchLocal(planeID)<=maxPitchY)
    if(theWindow->checkWindowAbout(col,row,run,thePlaneMapping_->getPlaneType(planeID)) && data.getXPitchLocal(planeID)<=maxPitchX && data.getYPitchLocal(planeID)<=maxPitchY)
        //if(planeID == 22 && theWindow->checkWindow(col,row,run) && data.getXPitchLocal(planeID)<=maxPitchX && data.getYPitchLocal(planeID)<=maxPitchY)
    {
        THREADED(hCellEfficiencyNorm_[planeID])->Fill(xRes,yRes);
        //        THREADED(hCellEfficiency4RowsNorm_[planeID])->Fill(xRes,yRes+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));

        if(((int)col)%2 == 0)
            THREADED(hCellEfficiencyEvenColumnsNorm_[planeID])->Fill(xRes,yRes);
        else
            THREADED(hCellEfficiencyOddColumnsNorm_[planeID])->Fill(xRes,yRes);

        THREADED(h2D4cellEfficiencyNorm_[planeID])->Fill(xRes2,yRes2);

        if(data.getHasHit(planeID))
        {
            THREADED(hCellEfficiency_[planeID])->Fill(xRes,yRes);
            //            THREADED(hCellEfficiency4Rows_[planeID])->Fill(xRes,yRes+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));

            if(((int)col)%2 == 0)
                THREADED(hCellEfficiencyEvenColumns_[planeID])->Fill(xRes,yRes);
            else
                THREADED(hCellEfficiencyOddColumns_[planeID])->Fill(xRes,yRes);

            THREADED(h2D4cellEfficiency_[planeID])->Fill(xRes2,yRes2);
        }

        else
            THREADED(hCellInefficiency_[planeID])->Fill(xRes,yRes);
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::xCellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{
    if(!pass)
        return;

    const Window* theWindow       = theWindowsManager_->getWindow(planeID) ;
    int           row             = data.getRowPredicted(planeID)          ;
    int           col             = data.getColPredicted(planeID)          ;
    int           event           = data.getEventChewieNumber()            ;
    int           run             = data.getRunNumber()                    ;
    float         maxPitchX       = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str());
    float         xRes            = 0.  ;
    float         sizePixelLeft   = 300;
    float         sizePixelRight  = 300;
    float         edgeLeft        = 200.;
    float         edgeRight       = 200.;
    float         edgeGlobalRight = sizePixelLeft + atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())*50. + sizePixelRight;
    float         xPredicted;

    //FIXME OR UNDERSTANDME!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //FIXME OR UNDERSTANDME!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //FIXME OR UNDERSTANDME!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos && row!=-1)
    {

        xPredicted = data.getXPredictedLocal(planeID);

        if((xPredicted >= edgeGlobalRight - sizePixelRight) && (xPredicted <= edgeGlobalRight + edgeRight)
                && (theWindow->checkWindow(51,row,run) || theWindow->checkWindow(51,row-1,run) || theWindow->checkWindow(51,row+1,run) )
                && theWindow->checkTimeWindow(51,event,run))
        {
            float xResEdge =  xPredicted - edgeGlobalRight + sizePixelRight/2.;
            THREADED(h1EfficiencyEdgeRightNorm_[planeID])->Fill(xResEdge);
            if(data.getHasHit(planeID))
                THREADED(h1EfficiencyEdgeRight_[planeID])->Fill(xResEdge);
        }

        if( (xPredicted >= -edgeLeft) && (xPredicted <= sizePixelLeft)
                && (theWindow->checkWindow(0,row,run) || theWindow->checkWindow(0,row-1,run) || theWindow->checkWindow(0,row+1,run))
                && theWindow->checkTimeWindow(0,event,run))
        {
            float xResEdge =  xPredicted - sizePixelLeft/2.;
            THREADED(h1EfficiencyEdgeLeftNorm_[planeID])->Fill(xResEdge);
            if(data.getHasHit(planeID))
                THREADED(h1EfficiencyEdgeLeft_[planeID])->Fill(xResEdge);
        }

    }


    if(!data.getIsInDetector(planeID))
        return;

    //FIXME HARDCODED VALUE (30) THAT SHOULD BE TAKEN FROM THE GUI
    //FIXME HARDCODED VALUE (30) THAT SHOULD BE TAKEN FROM THE GUI
    //FIXME HARDCODED VALUE (30) THAT SHOULD BE TAKEN FROM THE GUI
    //FIXME HARDCODED VALUE (30) THAT SHOULD BE TAKEN FROM THE GUI
    // Mauro : wredundant cut since it's already applied with "pass"
    // if (data.getYPixelResidualLocal(planeID) > 30 || data.getYPixelResidualLocal(planeID) < -30)
    //     return;

    //    if (fabs(data.getXTrackResidualLocal(planeID)) > 85 && fabs(data.getYTrackResidualLocal(planeID)) > 60)
    //            return;

    if(data.getXPitchLocal(planeID) == maxPitchX)
    {
        if(data.getXPixelResidualLocal(planeID) > 0)
            xRes =-data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
        else if(data.getXPixelResidualLocal(planeID) <= 0)
            xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);
    }
    else
        return;

    if(theWindow->checkWindowAbout(col,row,run,thePlaneMapping_->getPlaneType(planeID)) && theWindow->checkTimeWindowAbout(col,event,run))
        //if(theWindow->checkWindow(col,row))
    {
        THREADED(h1DXcellEfficiencyNorm_[planeID])->Fill(xRes);
        if(data.getHasHit(planeID) /*&& data.getClusterSize(planeID)<=4*/)
        {
            if(data.getClusterSize(planeID)>4)
            {
                //std::cout << "ok!" << std::endl;
                THREADED(h1DXcellEfficiencyFirstHit_ [planeID])->Fill(xRes);
                THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
                return;
            }

            bool isOk = false;
            for(int h=0; h<data.getClusterSize(planeID); h++)
            {
                if((data.getClusterPixelCol(h,planeID) == col) && (data.getClusterPixelRow(h,planeID) == row))
                {
                    THREADED(h1DXcellEfficiencyFirstHit_ [planeID])->Fill(xRes);
                    THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
                    isOk = true;
                    break;
                }

            }
            if(isOk)
                return;
            else
            {
                isOk=false;
                for(int h=0; h<data.getClusterSize(planeID); h++)
                {
                    if(data.getClusterPixelRow(h,planeID) == row)
                    {
                        /*if((data.getXPixelResidualLocal(planeID)>0 && (col-data.getClusterPixelCol(h,planeID))==1) ||
                          (data.getXPixelResidualLocal(planeID)<0 && (col-data.getClusterPixelCol(h,planeID))==-1))*/
                        if( ( (col-data.getClusterPixelCol(h,planeID)) ==  1) || ( (col-data.getClusterPixelCol(h,planeID)) == -1) )
                        {
                            isOk=true;
                            break;
                        }
                    }
                }
                if(!isOk)
                    return;

                THREADED(h1DXcellEfficiencySecondHit_[planeID])->Fill(xRes);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::yCellEfficiency(bool pass, int planeID, const Data& data, int threadNumber)
{

    if(!pass)
        return;


    const Window* theWindow     = theWindowsManager_->getWindow(planeID) ;
    int           row           = data.getRowPredicted(planeID)          ;
    int           col           = data.getColPredicted(planeID)          ;
    int           event         = data.getEventChewieNumber()            ;
    int           run           = data.getRunNumber()                    ;
    float         maxPitchY     = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str());
    float         yRes          = 0.  ;
    float         sizePixelDown = 100.;
    float         sizePixelUp   = 200.;
    float         edgeDown      = 200.;
    float         edgeUp        = 200.;
    float         edgeGlobalUp  = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str())*79. + 200.;

    if(thePlaneMapping_->getPlaneName(planeID).find("Dut")!=std::string::npos && col!=-1)
    {
        float yPredicted = data.getYPredictedLocal(planeID);

        if((yPredicted >= edgeGlobalUp - sizePixelUp) && (yPredicted <= edgeGlobalUp + edgeUp)
                && (theWindow->checkWindow(col,79,run) || theWindow->checkWindow(col-1,79,run) || theWindow->checkWindow(col+1,79,run))
                && (theWindow->checkTimeWindow(col,event,run) || theWindow->checkTimeWindow(col-1,event,run) || theWindow->checkTimeWindow(col+1,event,run) )
                )
        {
            float yResEdge =  yPredicted - edgeGlobalUp + sizePixelUp/2.;
            THREADED(h1EfficiencyEdgeUpNorm_[planeID])->Fill(yResEdge);
            if(data.getHasHit(planeID))
                THREADED(h1EfficiencyEdgeUp_[planeID])->Fill(yResEdge);
        }

        if( yPredicted >= -edgeDown
                && yPredicted <= sizePixelDown
                && (theWindow->checkWindow(col,0,run) || theWindow->checkWindow(col-1,0,run) || theWindow->checkWindow(col+1,0,run))
                && (theWindow->checkTimeWindow(col,event,run) || theWindow->checkTimeWindow(col-1,event,run) || theWindow->checkTimeWindow(col+1,event,run) )
                )
        {
            float yResEdge =  yPredicted - sizePixelDown/2.;
            THREADED(h1EfficiencyEdgeDownNorm_[planeID])->Fill(yResEdge);
            if(data.getHasHit(planeID))
                THREADED(h1EfficiencyEdgeDown_[planeID])->Fill(yResEdge);
        }

    }

    if(!data.getIsInDetector(planeID))
        return;
    //FIXME HARDCODED VALUE (20) THAT SHOULD BE TAKEN FROM THE GUI
    //FIXME HARDCODED VALUE (20) THAT SHOULD BE TAKEN FROM THE GUI
    //FIXME HARDCODED VALUE (20) THAT SHOULD BE TAKEN FROM THE GUI
    //FIXME HARDCODED VALUE (20) THAT SHOULD BE TAKEN FROM THE GUI
    // Mauro : wredundant cut since it's already applied with "pass"
    // if (data.getXPixelResidualLocal(planeID) > 20 || data.getXPixelResidualLocal(planeID) < -20)
    //     return;

    if(data.getYPitchLocal(planeID)<=maxPitchY)
    {
        if(data.getYPixelResidualLocal(planeID)>0)
            yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
        else if(data.getYPixelResidualLocal(planeID)<=0)
            yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);
    }
    else
        return;
    if(row!=50)
    {
        THREADED(h1DYcellEfficiency4RowsNorm_[planeID])->Fill(data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
        if(theWindow->checkWindow(col,row,run))
        {
            THREADED(h1DYcellEfficiency4Rows_    [planeID])->Fill(data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
        }
    }
    if(theWindow->checkWindowAbout(col,row,run, thePlaneMapping_->getPlaneType(planeID)) && theWindow->checkTimeWindowAbout(col,event,run))
        //if(theWindow->checkWindow(col,row))
    {
        THREADED(h1DYcellEfficiencyNorm_[planeID])->Fill(yRes);
        if(data.getHasHit(planeID) /*&& data.getClusterSize(planeID)<=4*/)
        {
            if(data.getClusterSize(planeID)>4)
            {
                //std::cout << "ok!" << std::endl;
                THREADED(h1DYcellEfficiencyFirstHit_ [planeID])->Fill(yRes);
                THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
                return;
            }

            bool isOk = false;
            for(int h=0; h<data.getClusterSize(planeID); h++)
            {
                if(data.getClusterPixelCol(h,planeID)==col && data.getClusterPixelRow(h,planeID)==row)
                {
                    THREADED(h1DYcellEfficiencyFirstHit_[planeID])->Fill(yRes);
                    THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
                    isOk = true;
                    break;
                }
            }
            if(isOk)
                return;
            else
            {
                isOk=false;
                for(int h=0; h<data.getClusterSize(planeID); h++)
                {
                    if(data.getClusterPixelCol(h,planeID)==col)
                    {
                        /*if((data.getYPixelResidualLocal(planeID)>0 && (row-data.getClusterPixelRow(h,planeID))==1) ||
                          (data.getYPixelResidualLocal(planeID)<0 && (row-data.getClusterPixelRow(h,planeID))==-1))*/
                        if( ( (row-data.getClusterPixelRow(h,planeID)) ==  1  ) ||
                                ( (row-data.getClusterPixelRow(h,planeID)) == -1  )  )
                        {
                            isOk=true;
                            break;
                        }
                    }
                }
                if(!isOk)
                    return;

                THREADED(h1DYcellEfficiencySecondHit_[planeID])->Fill(yRes);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Efficiency::setCutsFormula(std::map<std::string,std::string> cutsList, std::vector<TTree*> tree)
{
    std::vector<TTreeFormula*> formulasVector;

    for(std::map<std::string,std::string>::iterator it=cutsList.begin(); it!=cutsList.end(); it++)
    {
        if((it->first) == "main cut" && (it->second).size()==0)
            STDLINE("WARNING: no main cut set in efficiency analysis!! Default value = true!", ACRed);

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
bool Efficiency::passStandardCuts(int planeID, const Data &data)//Requires 8 telescope hits with cluster size <= 2
{
    if(!theXmlParser_->getAnalysesFromString("Efficiency")->applyStandardCuts())
        return true;

    if (theXmlParser_->getAnalysesFromString("Charge")->excludeBadPlanes())
        return passBadPlanesCut(planeID, data);

    int minHits = atoi(theXmlParser_->getAnalysesFromString("Efficiency")->getMinHits().c_str())-1;//To calculate efficiency on the telescope
    int excludeMe = 0;
    if(thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos)//Dut case
        minHits += 1;
    else if(data.getHasHit(planeID) && data.getClusterSize(planeID)<=2)//Telescope case
        excludeMe = 1;

    //    int  hitsOnTelescope = 0;
    //    for (unsigned int i = 0; i < thePlaneMapping_->getNumberOfPlanes(); i++)
    //    {
    //        if (data.getHasHit(i) == true && thePlaneMapping_->getPlaneName(i).find("Dut") == std::string::npos)
    //            hitsOnTelescope++;
    //    }

    if(data.getNumberOfTelescopeClustersSizeLE2() - excludeMe >= minHits)
        //if(hitsOnTelescope >= minHits)
        return true;
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Efficiency::passBadPlanesCut (int planeID, const Data &data)
{
    //    bool badPlanes = theXmlParser_->getAnalysesFromString("Charge")->excludeBadPlanes();
    int badPlanesCut = theXmlParser_->getAnalysesFromString("Charge")->getBadPlanesCut();


    int maxNumberOfEvents = 0;
    for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes() -2; ++p)// -2 is to exclude DUTs
    {
        HistogramWindow * aWindow = (HistogramWindow*)theAnalysisManager_->getWindowsManager()->getWindow(p);
        if (aWindow->getNumberOfEvents() > maxNumberOfEvents)
        {
            maxNumberOfEvents = aWindow->getNumberOfEvents();
        }
    }

    int minHits = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str())-1;//To calculate efficiency on the telescope
    int excludeMe = 0;
    if(thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) //Dut case
        minHits += 1;
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
