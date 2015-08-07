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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Charge::Charge(AnalysisManager* analysisManager, int nOfThreads) :
    Analysis                       (analysisManager, nOfThreads)
  , thePlaneMapping_               (0)
  , langaus_                       (0)
  , theWindowsManager_             (0)
  , theXmlParser_                  (analysisManager->getXmlParser())
  , standardCutsThreshold_         (0)
  , standardCutsMaximumCharge_     (0)
  , standardCutsMinimumTotalCharge_(0)
  , standardCutsMaximumTotalCharge_(0)
{
    thePlaneMapping_ = new PlanesMapping();

    for(int p=0; p<4; p++)
    {
        parMin_      [p] = 0;
        parMax_      [p] = 0;
        isMinToLimit_[p] = 0;
        isMaxToLimit_[p] = 0;
    }
    totEventsControl_ = 0;

    langaus_ = new TF1("langaus",Utilities::langaus,0,60000,4);
    langaus_->SetParNames("Width","MPV","Area","GSigma");
    langaus_->SetLineColor(kBlue);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Charge::~Charge(void)
{
    if(thePlaneMapping_)
    {
        delete thePlaneMapping_;
        thePlaneMapping_ = 0;
    }

    if(langaus_)
    {
        delete langaus_;
        langaus_ = 0;
    }

    destroy();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::setInvincible(bool cannotBeKilled)
{
    cannotBeDestroyed_ = cannotBeKilled;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::destroy()
{
    if(Analysis::fDoNotDelete_ || cannotBeDestroyed_) return;

    /*--------------------------------------------------------------------------------------------Cluster size--------------------------------------------------------------------------------------------------*/
    for(std::vector<TH1F*>::iterator it=hClusterSize_                           .begin(); it!=hClusterSize_                           .end(); it++) delete *it; hClusterSize_                           .clear();
    for(std::vector<TH1F*>::iterator it=hClusterSizeStandardCutsThreshold_      .begin(); it!=hClusterSizeStandardCutsThreshold_      .end(); it++) delete *it; hClusterSizeStandardCutsThreshold_      .clear();
    for(std::vector<TH1F*>::iterator it=hClusterSizeStandardCutsThresholdAndCellLandau_.begin(); it!=hClusterSizeStandardCutsThresholdAndCellLandau_.end(); it++) delete *it; hClusterSizeStandardCutsThresholdAndCellLandau_.clear();
    for(std::vector<TH1F*>::iterator it=hNumberOfCols_                          .begin(); it!=hNumberOfCols_                          .end(); it++) delete *it; hNumberOfCols_                          .clear();
    for(std::vector<TH1F*>::iterator it=hNumberOfRows_                          .begin(); it!=hNumberOfRows_                          .end(); it++) delete *it; hNumberOfRows_                          .clear();
    for(std::vector<TH2F*>::iterator it=hClusterSizeDistribution1s_             .begin(); it!=hClusterSizeDistribution1s_             .end(); it++) delete *it; hClusterSizeDistribution1s_             .clear();
    for(std::vector<TH2F*>::iterator it=hClusterSizeDistribution2s_             .begin(); it!=hClusterSizeDistribution2s_             .end(); it++) delete *it; hClusterSizeDistribution2s_             .clear();

    /*-----------------------------------------------------------------------------------------Landau distributions---------------------------------------------------------------------------------------------*/
    for(std::vector<TH1F*>::iterator it=hLandauClusterSize1_                    .begin(); it!=hLandauClusterSize1_                    .end(); it++) delete *it; hLandauClusterSize1_                    .clear();
    for(std::vector<TH1F*>::iterator it=hLandauClusterSize2_                    .begin(); it!=hLandauClusterSize2_                    .end(); it++) delete *it; hLandauClusterSize2_                    .clear();
    for(std::vector<TH1F*>::iterator it=hLandauClusterSize2sameCol_             .begin(); it!=hLandauClusterSize2sameCol_             .end(); it++) delete *it; hLandauClusterSize2sameCol_             .clear();
    for(std::vector<TH1F*>::iterator it=hLandauClusterSize2sameRow_             .begin(); it!=hLandauClusterSize2sameRow_             .end(); it++) delete *it; hLandauClusterSize2sameRow_             .clear();
    for(std::vector<TH1F*>::iterator it=hLandauClusterSize3_                    .begin(); it!=hLandauClusterSize3_                    .end(); it++) delete *it; hLandauClusterSize2_                    .clear();
    for(std::vector<TH1F*>::iterator it=hLandauClusterSize3sameCol_             .begin(); it!=hLandauClusterSize3sameCol_             .end(); it++) delete *it; hLandauClusterSize2sameCol_             .clear();
    for(std::vector<TH1F*>::iterator it=hLandauClusterSize3sameRow_             .begin(); it!=hLandauClusterSize3sameRow_             .end(); it++) delete *it; hLandauClusterSize2sameRow_             .clear();
    for(std::vector<TH1F*>::iterator it=hCellLandau_                            .begin(); it!=hCellLandau_                            .end(); it++) delete *it; hCellLandau_                            .clear();
    for(std::vector<TH1F*>::iterator it=hCellLandau3D_                          .begin(); it!=hCellLandau3D_                          .end(); it++) delete *it; hCellLandau3D_                          .clear();
    for(std::vector<TH1F*>::iterator it=hCellLandau3DElectrodes_                .begin(); it!=hCellLandau3DElectrodes_                .end(); it++) delete *it; hCellLandau3DElectrodes_                .clear();

    /*-------------------------------------------------------------------------------------------2D cell charge-------------------------------------------------------------------------------------------------*/
    for(std::vector<TH2F*>::iterator it=h2DCellCharge_                          .begin(); it!=h2DCellCharge_                          .end(); it++) delete *it; h2DCellCharge_                          .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeNum_                       .begin(); it!=h2DCellChargeNum_                       .end(); it++) delete *it; h2DCellChargeNum_                       .clear();
    for(std::vector<TH2F*>::iterator it=h2DAllTracks_                           .begin(); it!=h2DAllTracks_                           .end(); it++) delete *it; h2DAllTracks_                           .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeNorm_                      .begin(); it!=h2DCellChargeNorm_                      .end(); it++) delete *it; h2DCellChargeNorm_                      .clear();
    for(std::vector<TH2F*>::iterator it=h4CellsCharge_                          .begin(); it!=h4CellsCharge_                          .end(); it++) delete *it; h4CellsCharge_                          .clear();
    for(std::vector<TH2F*>::iterator it=h4CellsAllTracks_                       .begin(); it!=h4CellsAllTracks_                       .end(); it++) delete *it; h4CellsAllTracks_                       .clear();
    for(std::vector<TH2F*>::iterator it=h4CellsChargeNorm_                      .begin(); it!=h4CellsChargeNorm_                      .end(); it++) delete *it; h4CellsChargeNorm_                      .clear();
    for(std::vector<TH2F*>::iterator it=h4Hits_                                 .begin(); it!=h4Hits_                                 .end(); it++) delete *it; h4Hits_                                 .clear();
    for(std::vector<TH2F*>::iterator it=h4HitsCharge_                           .begin(); it!=h4HitsCharge_                           .end(); it++) delete *it; h4HitsCharge_                           .clear();
    for(std::vector<TH2F*>::iterator it=hCellChargeCoarse_                      .begin(); it!=hCellChargeCoarse_                      .end(); it++) delete *it; hCellChargeCoarse_                      .clear();
    for(std::vector<TH2F*>::iterator it=hCellChargeCoarseNorm_                  .begin(); it!=hCellChargeCoarseNorm_                  .end(); it++) delete *it; hCellChargeCoarseNorm_                  .clear();
    for(std::vector<TH2F*>::iterator it=hHitsNotOnRowCol_                       .begin(); it!=hHitsNotOnRowCol_                       .end(); it++) delete *it; hHitsNotOnRowCol_                       .clear();
    for(std::vector<TH1F*>::iterator it=hCutsControl_                           .begin(); it!=hCutsControl_                           .end(); it++) delete *it; hCutsControl_                           .clear();
    for(std::vector<TH1F*>::iterator it=hChargeNotOnRowCol_                     .begin(); it!=hChargeNotOnRowCol_                     .end(); it++) delete *it; hChargeNotOnRowCol_                     .clear();

    /*----------------------------------------------2D cell charge Landau--------------------------------------------*/
    for(std::vector<std::vector<TH1F*> >::iterator it=hCellChargeCoarseLandau_.begin(); it!=hCellChargeCoarseLandau_.end(); it++)
        for(std::vector<TH1F*>::iterator it2=it->begin(); it2!=it->end(); it2++) delete *it2;
    hCellChargeCoarseLandau_.clear();

    /*------------------------------------------------------------------------------------2D cell charge - X coordinate-----------------------------------------------------------------------------------------*/
    for(std::vector<TH2F*>::iterator it=h2DXcellCharge_                         .begin(); it!=h2DXcellCharge_                         .end(); it++) delete *it; h2DXcellCharge_                         .clear();
    for(std::vector<TH2F*>::iterator it=h2DXcellChargeSumLE2_                   .begin(); it!=h2DXcellChargeSumLE2_                   .end(); it++) delete *it; h2DXcellChargeSumLE2_                   .clear();
    for(std::vector<TH2F*>::iterator it=h2DXcellSingleHits_                     .begin(); it!=h2DXcellSingleHits_                     .end(); it++) delete *it; h2DXcellSingleHits_                     .clear();
    for(std::vector<TH2F*>::iterator it=h2DXcellDoubleHits_                     .begin(); it!=h2DXcellDoubleHits_                     .end(); it++) delete *it; h2DXcellDoubleHits_                     .clear();
    for(std::vector<TH2F*>::iterator it=hHitsNotONRowColVsXSlope_               .begin(); it!=hHitsNotONRowColVsXSlope_               .end(); it++) delete *it; hHitsNotONRowColVsXSlope_               .clear();

    /*------------------------------------------------------------------------------------2D cell charge - Y coordinate-----------------------------------------------------------------------------------------*/
    for(std::vector<TH2F*>::iterator it=h2DYcellCharge_                         .begin(); it!=h2DYcellCharge_                         .end(); it++) delete *it; h2DYcellCharge_                         .clear();
    for(std::vector<TH2F*>::iterator it=h2DYcellChargeSumLE2_                   .begin(); it!=h2DYcellChargeSumLE2_                   .end(); it++) delete *it; h2DYcellChargeSumLE2_                   .clear();
    for(std::vector<TH2F*>::iterator it=h2DYcellSingleHits_                     .begin(); it!=h2DYcellSingleHits_                     .end(); it++) delete *it; h2DYcellSingleHits_                     .clear();
    for(std::vector<TH2F*>::iterator it=h2DYcellDoubleHits_                     .begin(); it!=h2DYcellDoubleHits_                     .end(); it++) delete *it; h2DYcellDoubleHits_                     .clear();
    for(std::vector<TH2F*>::iterator it=hHitsNotONRowColVsYSlope_               .begin(); it!=hHitsNotONRowColVsYSlope_               .end(); it++) delete *it; hHitsNotONRowColVsYSlope_               .clear();

    /*----------------------------------------------Ï-------------------------------------------4 Rows cell charge ------------------------------------------------------------------------------------------------------*/
    for(std::vector<TH2F*>::iterator it=h2DCellPixelCharge4Rows_                               .begin(); it!=h2DCellPixelCharge4Rows_                                 .end(); it++) delete *it; h2DCellPixelCharge4Rows_                                .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellClusterCharge4Rows_                             .begin(); it!=h2DCellClusterCharge4Rows_                               .end(); it++) delete *it; h2DCellClusterCharge4Rows_                              .clear();
    for(std::vector<TH2F*>::iterator it=h2DAllTracks4Rows_                                     .begin(); it!=h2DAllTracks4Rows_                                       .end(); it++) delete *it; h2DAllTracks4Rows_                                      .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellCharge4RowsOnly50_                              .begin(); it!=h2DCellCharge4RowsOnly50_                                .end(); it++) delete *it; h2DCellCharge4RowsOnly50_                               .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellCharge4RowsClusterSize_                         .begin(); it!=h2DCellCharge4RowsClusterSize_                           .end(); it++) delete *it; h2DCellCharge4RowsClusterSize_                          .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellCharge4RowsNorm_                                .begin(); it!=h2DCellCharge4RowsNorm_                                  .end(); it++) delete *it; h2DCellCharge4RowsNorm_                                 .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsSameRow_                    .begin(); it!=h2DCellChargeSecondHit4RowsSameRow_                      .end(); it++) delete *it; h2DCellChargeSecondHit4RowsSameRow_                     .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsSameRowNorm_                .begin(); it!=h2DCellChargeSecondHit4RowsSameRowNorm_                  .end(); it++) delete *it; h2DCellChargeSecondHit4RowsSameRowNorm_                 .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsSameCol_                    .begin(); it!=h2DCellChargeSecondHit4RowsSameCol_                      .end(); it++) delete *it; h2DCellChargeSecondHit4RowsSameCol_                     .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsSameColNorm_                .begin(); it!=h2DCellChargeSecondHit4RowsSameColNorm_                  .end(); it++) delete *it; h2DCellChargeSecondHit4RowsSameColNorm_                 .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsClusterSize3_               .begin(); it!=h2DCellChargeSecondHit4RowsClusterSize3_                 .end(); it++) delete *it; h2DCellChargeSecondHit4RowsClusterSize3_                .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsClusterSize3LowCharge_      .begin(); it!=h2DCellChargeSecondHit4RowsClusterSize3LowCharge_        .end(); it++) delete *it; h2DCellChargeSecondHit4RowsClusterSize3LowCharge_       .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_  .begin(); it!=h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_    .end(); it++) delete *it; h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_   .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsClusterSize3HighCharge_     .begin(); it!=h2DCellChargeSecondHit4RowsClusterSize3HighCharge_       .end(); it++) delete *it; h2DCellChargeSecondHit4RowsClusterSize3HighCharge_      .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ .begin(); it!=h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_   .end(); it++) delete *it; h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_  .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellPixelCharge4RowsClusterSize1_                   .begin(); it!=h2DCellPixelCharge4RowsClusterSize1_                     .end(); it++) delete *it; h2DCellPixelCharge4RowsClusterSize1_                    .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellPixelCharge4RowsClusterSize2_                   .begin(); it!=h2DCellPixelCharge4RowsClusterSize2_                     .end(); it++) delete *it; h2DCellPixelCharge4RowsClusterSize2_                    .clear();
    for(std::vector<TH2F*>::iterator it=h2DCellPixelCharge4RowsClusterSize3_                   .begin(); it!=h2DCellPixelCharge4RowsClusterSize3_                     .end(); it++) delete *it; h2DCellPixelCharge4RowsClusterSize3_                    .clear();


    /*----------------------------------------------Ï--------------------------------------1D cell charge - X coordinate----------------------------------------------------------------------------------------*/
    for(std::vector<TH1F*>::iterator it=h1DXcellCharge_                         .begin(); it!=h1DXcellCharge_                         .end(); it++) delete *it; h1DXcellCharge_                         .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeNormToAll_                .begin(); it!=h1DXcellChargeNormToAll_                .end(); it++) delete *it; h1DXcellChargeNormToAll_                .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeSumLE2_                   .begin(); it!=h1DXcellChargeSumLE2_                   .end(); it++) delete *it; h1DXcellChargeSumLE2_                   .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeSumLE2NormToAll_          .begin(); it!=h1DXcellChargeSumLE2NormToAll_          .end(); it++) delete *it; h1DXcellChargeSumLE2NormToAll_          .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeSumLE3_                   .begin(); it!=h1DXcellChargeSumLE3_                   .end(); it++) delete *it; h1DXcellChargeSumLE3_                   .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeSumLE3NormToAll_          .begin(); it!=h1DXcellChargeSumLE3NormToAll_          .end(); it++) delete *it; h1DXcellChargeSumLE3NormToAll_          .clear();
    for(std::vector<TH1F*>::iterator it=h1DXallTracks_                          .begin(); it!=h1DXallTracks_                          .end(); it++) delete *it; h1DXallTracks_                          .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeNorm_                     .begin(); it!=h1DXcellChargeNorm_                     .end(); it++) delete *it; h1DXcellChargeNorm_                     .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeSumLE2Norm_               .begin(); it!=h1DXcellChargeSumLE2Norm_               .end(); it++) delete *it; h1DXcellChargeSumLE2Norm_               .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeSumLE3Norm_               .begin(); it!=h1DXcellChargeSumLE3Norm_               .end(); it++) delete *it; h1DXcellChargeSumLE3Norm_               .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellSingleHits_                     .begin(); it!=h1DXcellSingleHits_                     .end(); it++) delete *it; h1DXcellSingleHits_                     .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellDoubleHits_                     .begin(); it!=h1DXcellDoubleHits_                     .end(); it++) delete *it; h1DXcellDoubleHits_                     .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcell3Hits_                          .begin(); it!=h1DXcell3Hits_                          .end(); it++) delete *it; h1DXcell3Hits_                          .clear();

    /*------------------------------------------------------------------------------------1D charge-Y residuals------------------------------------------------------------------------------------------------*/
    for(std::vector<TH1F*>::iterator it=h1DYcellCharge_                         .begin(); it!=h1DYcellCharge_                         .end(); it++) delete *it; h1DYcellCharge_                         .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeNormToAll_                .begin(); it!=h1DYcellChargeNormToAll_                .end(); it++) delete *it; h1DYcellChargeNormToAll_                .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeSumLE2_                   .begin(); it!=h1DYcellChargeSumLE2_                   .end(); it++) delete *it; h1DYcellChargeSumLE2_                   .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeSumLE2NormToAll_          .begin(); it!=h1DYcellChargeSumLE2NormToAll_          .end(); it++) delete *it; h1DYcellChargeSumLE2NormToAll_          .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeSumLE3_                   .begin(); it!=h1DYcellChargeSumLE3_                   .end(); it++) delete *it; h1DYcellChargeSumLE3_                   .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeSumLE3NormToAll_          .begin(); it!=h1DYcellChargeSumLE3NormToAll_          .end(); it++) delete *it; h1DYcellChargeSumLE3NormToAll_          .clear();
    for(std::vector<TH1F*>::iterator it=h1DYallTracks_                          .begin(); it!=h1DYallTracks_                          .end(); it++) delete *it; h1DYallTracks_                          .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeNorm_                     .begin(); it!=h1DYcellChargeNorm_                     .end(); it++) delete *it; h1DYcellChargeNorm_                     .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeSumLE2Norm_               .begin(); it!=h1DYcellChargeSumLE2Norm_               .end(); it++) delete *it; h1DYcellChargeSumLE2Norm_               .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeSumLE3Norm_               .begin(); it!=h1DYcellChargeSumLE3Norm_               .end(); it++) delete *it; h1DYcellChargeSumLE3Norm_               .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellSingleHits_                     .begin(); it!=h1DYcellSingleHits_                     .end(); it++) delete *it; h1DYcellSingleHits_                     .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellDoubleHits_                     .begin(); it!=h1DYcellDoubleHits_                     .end(); it++) delete *it; h1DYcellDoubleHits_                     .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcell3Hits_                          .begin(); it!=h1DYcell3Hits_                          .end(); it++) delete *it; h1DYcell3Hits_                          .clear();

    /*----------------------------------------------------------------------------------------X Asimmetry------------------------------------------------------------------------------------------------------*/
    for(std::vector<TH1F*>::iterator it=hXAsimmetry_                            .begin(); it!=hXAsimmetry_                            .end(); it++) delete *it; hXAsimmetry_                            .clear();
    for(std::vector<TH2F*>::iterator it=h2DXAsimmetryLandau_                    .begin(); it!=h2DXAsimmetryLandau_                    .end(); it++) delete *it; h2DXAsimmetryLandau_                    .clear();
    for(std::vector<TH2F*>::iterator it=h2DXcellChargeAsimmetry_                .begin(); it!=h2DXcellChargeAsimmetry_                .end(); it++) delete *it; h2DXcellChargeAsimmetry_                .clear();
    for(std::vector<TH2F*>::iterator it=h2DXcellChargeAsimmetryInv_             .begin(); it!=h2DXcellChargeAsimmetryInv_             .end(); it++) delete *it; h2DXcellChargeAsimmetryInv_             .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeAsimmetryInv_             .begin(); it!=h1DXcellChargeAsimmetryInv_             .end(); it++) delete *it; h1DXcellChargeAsimmetryInv_             .clear();
    //for(std::vector<TH2F*>::iterator it=h2DXcellChargeAsimmetryInv_             .begin(); it!=h2DXcellChargeAsimmetryInv_             .end(); it++) delete *it; h2DXcellChargeAsimmetryInv_             .clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeAsimmetry_                .begin(); it!=h1DXcellChargeAsimmetry_                .end(); it++) delete *it; h1DXcellChargeAsimmetry_                .clear();
    for(std::vector<TH2F*>::iterator it=h2DXcellChargeAsimmetryUnconstrained_   .begin(); it!=h2DXcellChargeAsimmetryUnconstrained_   .end(); it++) delete *it; h2DXcellChargeAsimmetryUnconstrained_   .clear();
    for(std::vector<TH2F*>::iterator it=h2DXcellChargeAsimmetryUnconstrainedInv_.begin(); it!=h2DXcellChargeAsimmetryUnconstrainedInv_.end(); it++) delete *it; h2DXcellChargeAsimmetryUnconstrainedInv_.clear();
    for(std::vector<TH1F*>::iterator it=h1DXcellChargeAsimmetryUnconstrained_   .begin(); it!=h1DXcellChargeAsimmetryUnconstrained_   .end(); it++) delete *it; h1DXcellChargeAsimmetryUnconstrained_   .clear();
    for(std::vector<TH2F*>::iterator it=h2DXCellChargeAsimmetrySizeLE2_         .begin(); it!=h2DXCellChargeAsimmetrySizeLE2_         .end(); it++) delete *it; h2DXCellChargeAsimmetrySizeLE2_         .clear();
    for(std::vector<TH1F*>::iterator it=h1DXCellChargeAsimmetrySizeLE2_         .begin(); it!=h1DXCellChargeAsimmetrySizeLE2_         .end(); it++) delete *it; h1DXCellChargeAsimmetrySizeLE2_         .clear();

    /*----------------------------------------------------------------------------------------Y Asimmetry------------------------------------------------------------------------------------------------------*/
    for(std::vector<TH1F*>::iterator it=hYAsimmetry_                            .begin(); it!=hYAsimmetry_                            .end(); it++) delete *it; hYAsimmetry_                            .clear();
    for(std::vector<TH2F*>::iterator it=h2DYAsimmetryLandau_                    .begin(); it!=h2DYAsimmetryLandau_                    .end(); it++) delete *it; h2DYAsimmetryLandau_                    .clear();
    for(std::vector<TH2F*>::iterator it=h2DYcellChargeAsimmetry_                .begin(); it!=h2DYcellChargeAsimmetry_                .end(); it++) delete *it; h2DYcellChargeAsimmetry_                .clear();
    for(std::vector<TH2F*>::iterator it=h2DYcellChargeAsimmetryInv_             .begin(); it!=h2DYcellChargeAsimmetryInv_             .end(); it++) delete *it; h2DYcellChargeAsimmetryInv_             .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeAsimmetry_                .begin(); it!=h1DYcellChargeAsimmetry_                .end(); it++) delete *it; h1DYcellChargeAsimmetry_                .clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeAsimmetryInv_             .begin(); it!=h1DYcellChargeAsimmetryInv_             .end(); it++) delete *it; h1DYcellChargeAsimmetryInv_             .clear();
    for(std::vector<TH2F*>::iterator it=h2DYAsimmetryLandau_                    .begin(); it!=h2DYAsimmetryLandau_                    .end(); it++) delete *it; h2DYAsimmetryLandau_                    .clear();
    for(std::vector<TH2F*>::iterator it=h2DYcellChargeAsimmetryUnconstrained_   .begin(); it!=h2DYcellChargeAsimmetryUnconstrained_   .end(); it++) delete *it; h2DYcellChargeAsimmetryUnconstrained_   .clear();
    for(std::vector<TH2F*>::iterator it=h2DYcellChargeAsimmetryUnconstrainedInv_.begin(); it!=h2DYcellChargeAsimmetryUnconstrainedInv_.end(); it++) delete *it; h2DYcellChargeAsimmetryUnconstrainedInv_.clear();
    for(std::vector<TH1F*>::iterator it=h1DYcellChargeAsimmetryUnconstrained_   .begin(); it!=h1DYcellChargeAsimmetryUnconstrained_   .end(); it++) delete *it; h1DYcellChargeAsimmetryUnconstrained_   .clear();
    for(std::vector<TH2F*>::iterator it=h2DYCellChargeAsimmetrySizeLE2_         .begin(); it!=h2DYCellChargeAsimmetrySizeLE2_         .end(); it++) delete *it; h2DYCellChargeAsimmetrySizeLE2_         .clear();
    for(std::vector<TH1F*>::iterator it=h1DYCellChargeAsimmetrySizeLE2_         .begin(); it!=h1DYCellChargeAsimmetrySizeLE2_         .end(); it++) delete *it; h1DYCellChargeAsimmetrySizeLE2_         .clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Charge::langausFit(TH1* histo, double *fitParameters)
{
    TAxis* xAxis           ;
    double range           ;
    double integral        ;
    double gausPar      [3];
    double landauPar    [3];
    double fitRange     [2];
    double startValues  [4];
    double parsLowLimit [4];
    double parsHighLimit[4];

    TF1* landau = new TF1("myLandau","landau",0,60000);
    TF1* gaus   = new TF1("myGaus"  ,"gaus"  ,0,60000);

    fitRange[0]=0.4*(histo->GetMean());
    fitRange[1]=1.8*(histo->GetMean());

    gaus->SetRange(fitRange[0],fitRange[1]);
    histo->Fit(gaus,"0QR");
    for(int p=0; p<3; p++)
        gausPar[p] = gaus->GetParameter(p);

    landau->SetRange(fitRange[0],fitRange[1]);
    histo->Fit(landau,"0QR");
    for(int p=0; p<3; p++)
        landauPar[p] = landau->GetParameter(p);

    xAxis    = histo->GetXaxis();
    range    = xAxis->GetXmax() - xAxis->GetXmin();
    integral = ((histo->Integral())*range)/(histo->GetNbinsX());

    startValues[0]=landauPar[2];
    startValues[1]=landauPar[1];
    startValues[2]=integral    ;
    startValues[3]=gausPar[2]  ;

    parsLowLimit [0] = startValues[0] - 0.68*startValues[0];
    parsHighLimit[0] = startValues[0] + 0.68*startValues[0];
    parsLowLimit [1] = startValues[1] - 0.68*startValues[1];
    parsHighLimit[1] = startValues[1] + 0.68*startValues[1];
    parsLowLimit [2] = startValues[2] - 0.68*startValues[2];
    parsHighLimit[2] = startValues[2] + 0.68*startValues[2];
    parsLowLimit [3] = startValues[3] - 0.68*startValues[3];
    parsHighLimit[3] = startValues[3] + 0.68*startValues[3];

    langaus_->SetRange(fitRange[0],fitRange[1]);
    langaus_->SetParameters(startValues);

    for (int p=0; p<4; p++) {
        langaus_->SetParLimits(p, parsLowLimit[p], parsHighLimit[p]);
    }

    TFitResultPtr r = histo->Fit(langaus_,"QRBL");
    int fitStatus = r;

    langaus_->GetParameters(fitParameters);

    return fitStatus;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::fitCharge(int planeID)
{
    std::stringstream ss;
    double fitParameters[4];

    /*--------------------------------------------------------*/
    STDLINE("All single hits:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hLandauClusterSize1_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);
    /*--------------------------------------------------------*/
    STDLINE("Single hits in the cell fiducial window:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hCellLandau_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("Single hits in the 3D cell fiducial window:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hCellLandau3D_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("Single pixel in the cell fiducial window", ACWhite);
    STDLINE("", ACWhite);

    langausFit(hCellLandauSinglePixel_[planeID], fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("All double hits:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hLandauClusterSize2_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("Double hits on the same row:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hLandauClusterSize2sameRow_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("Double hits on the same column:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hLandauClusterSize2sameCol_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("All 3 hits:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hLandauClusterSize3sameCol_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("3 hits on the same row:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hLandauClusterSize3sameRow_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("3 hits on the same column:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hLandauClusterSize3sameCol_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);

    /*--------------------------------------------------------*/
    STDLINE("3 hits on the same row:",ACWhite);
    STDLINE("",ACWhite);

    langausFit(hLandauClusterSize3sameRow_[planeID],fitParameters);

    ss.str(""); ss << "Width: " << fitParameters[0];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "MPV: " << fitParameters[1];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "Area: " << fitParameters[2];
    STDLINE(ss.str(),ACGreen);

    ss.str(""); ss << "GSigma: " << fitParameters[3];
    STDLINE(ss.str(),ACGreen);

    STDLINE("",ACWhite);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::setErrorsBar(int planeID)
{
    std::stringstream hName;
    std::string planeName;

    planeName = thePlaneMapping_->getPlaneName(planeID);
    theAnalysisManager_->cd("/Charge/" + planeName + "/XAsimmetry");

    hName.str(""); hName << "h1DXcellChargeAsimmetry_" << planeName;
    h1DXcellChargeAsimmetry_.push_back((TH1F*)h2DXcellChargeAsimmetry_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    hName.str(""); hName << "h1DXcellChargeAsimmetryInv_" << planeName;
    h1DXcellChargeAsimmetryInv_.push_back((TH1F*)h2DXcellChargeAsimmetryInv_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    hName.str(""); hName << "h1DXcellChargeAsimmetryUnconstrained_" << planeName;
    h1DXcellChargeAsimmetryUnconstrained_.push_back((TH1F*)h2DXcellChargeAsimmetryUnconstrained_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    hName.str(""); hName << "h1DXcellChargeAsimmetryUnconstrainedInv_" << planeName;
    h1DXcellChargeAsimmetryUnconstrainedInv_.push_back((TH1F*)h2DXcellChargeAsimmetryUnconstrainedInv_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    hName.str(""); hName << "h1DXcellChargeAsimmetrySizeLE2_" << planeName;
    h1DXCellChargeAsimmetrySizeLE2_.push_back((TH1F*)h2DXCellChargeAsimmetrySizeLE2_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    theAnalysisManager_->cd("/Charge/" + planeName + "/YAsimmetry");

    hName.str(""); hName << "h1DYcellChargeAsimmetry_" << planeName;
    h1DYcellChargeAsimmetry_.push_back((TH1F*)h2DYcellChargeAsimmetry_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    hName.str(""); hName << "h1DYcellChargeAsimmetryInv_" << planeName;
    h1DYcellChargeAsimmetryInv_.push_back((TH1F*)h2DYcellChargeAsimmetryInv_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    hName.str(""); hName << "h1DYcellChargeAsimmetryUnconstrained_" << planeName;
    h1DYcellChargeAsimmetryUnconstrained_.push_back((TH1F*)h2DYcellChargeAsimmetryUnconstrained_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    hName.str(""); hName << "h1DYcellChargeAsimmetryUnconstrainedInv_" << planeName;
    h1DYcellChargeAsimmetryUnconstrainedInv_.push_back((TH1F*)h2DYcellChargeAsimmetryUnconstrainedInv_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    hName.str(""); hName << "h1DYcellChargeAsimmetrySizeLE2_" << planeName;
    h1DYCellChargeAsimmetrySizeLE2_.push_back((TH1F*)h2DYCellChargeAsimmetrySizeLE2_[planeID]->ProfileX(hName.str().c_str(),1,-1));

    theAnalysisManager_->cd("/Charge/" + planeName + "/XcellCharge2D");

    hName.str(""); hName << "hHitsNotONRowColVsXSlopeProfileX_" << planeName;
    hHitsNotONRowColVsXSlopeProfileX_.push_back((TH1F*)hHitsNotONRowColVsXSlope_[planeID]->ProfileX(hName.str().c_str(),150,-150));

    hName.str(""); hName << "hHitsNotONRowColVsXSlopeProfileY_" << planeName;
    hHitsNotONRowColVsXSlopeProfileY_.push_back((TH1F*)hHitsNotONRowColVsXSlope_[planeID]->ProfileY(hName.str().c_str(),0.000015,-0.000015));

    theAnalysisManager_->cd("/Charge/" + planeName + "/YcellCharge2D");

    hName.str(""); hName << "hHitsNotONRowColVsYSlopeProfileX_" << planeName;
    hHitsNotONRowColVsXSlopeProfileX_.push_back((TH1F*)hHitsNotONRowColVsYSlope_[planeID]->ProfileX(hName.str().c_str(),150,-150));

    hName.str(""); hName << "hHitsNotONRowColVsYSlopeProfileY_" << planeName;
    hHitsNotONRowColVsYSlopeProfileY_.push_back((TH1F*)hHitsNotONRowColVsYSlope_[planeID]->ProfileY(hName.str().c_str(),0.000015,-0.000015));

    theAnalysisManager_->cd("/Charge/" + planeName + "/XcellCharge1D");

    double binError;

    int nBinsX = h2DXcellCharge_[planeID]->GetNbinsX();
    TH1D* hXchargeTmp[nBinsX];
    for(int bX=1; bX<=nBinsX; ++bX)
    {
        hName.str(""); hName << "Xcharge_Proj_bin_" << bX;
        hXchargeTmp[bX-1] = h2DXcellCharge_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);
        if(hXchargeTmp[bX-1]->GetEntries()!=0)
        {
            binError = hXchargeTmp[bX-1]->GetRMS()/sqrt(hXchargeTmp[bX-1]->GetEntries());
            h1DXcellCharge_[planeID]->SetBinError(bX,binError);
        }
        else
            continue;
    }

    for(int p=0; p<nBinsX; p++) hXchargeTmp[p]->Delete("0");

    nBinsX = h2DXcellChargeSumLE2_[planeID]->GetNbinsX();
    TH1D* hXchargeSumLE2Tmp[nBinsX];
    for(int bX=1; bX<=nBinsX; ++bX)
    {
        hName.str(""); hName << "XchargeSumLE2_Proj_bin_" << bX;
        hXchargeSumLE2Tmp[bX-1] = h2DXcellChargeSumLE2_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);
        if(hXchargeSumLE2Tmp[bX-1]->GetEntries()!=0)
        {
            binError = hXchargeSumLE2Tmp[bX-1]->GetRMS()/sqrt(hXchargeSumLE2Tmp[bX-1]->GetEntries());
            h1DXcellChargeSumLE2_[planeID]->SetBinError(bX,binError);
        }
        else
            continue;
    }

    for(int p=0; p<nBinsX; p++) hXchargeSumLE2Tmp[p]->Delete("0");

    nBinsX = h2DXcellChargeSumLE3_[planeID]->GetNbinsX();
    TH1D* hXchargeSumLE3Tmp[nBinsX];
    for(int bX=1; bX<=nBinsX; ++bX)
    {
        hName.str(""); hName << "XchargeSumLE3_Proj_bin_" << bX;
        hXchargeSumLE3Tmp[bX-1] = h2DXcellChargeSumLE3_[planeID]->ProjectionY(hName.str().c_str(),bX,bX);
        if(hXchargeSumLE3Tmp[bX-1]->GetEntries()!=0)
        {
            binError = hXchargeSumLE3Tmp[bX-1]->GetRMS()/sqrt(hXchargeSumLE3Tmp[bX-1]->GetEntries());
            h1DXcellChargeSumLE3_[planeID]->SetBinError(bX,binError);
        }
        else
            continue;
    }

    for(int p=0; p<nBinsX; p++) hXchargeSumLE3Tmp[p]->Delete("0");

    theAnalysisManager_->cd("/Charge/" + planeName + "/YcellCharge1D");

    int nBinsY = h2DYcellCharge_[planeID]->GetNbinsX();
    TH1D* hYchargeTmp[nBinsX];
    for(int bY=1; bY<=nBinsY; ++bY)
    {
        hName.str(""); hName << "Ycharge_Proj_bin_" << bY;
        hYchargeTmp[bY-1] = h2DYcellCharge_[planeID]->ProjectionY(hName.str().c_str(),bY,bY);
        if(hYchargeTmp[bY-1]->GetEntries()!=0)
        {
            binError = hYchargeTmp[bY-1]->GetRMS()/sqrt(hYchargeTmp[bY-1]->GetEntries());
            h1DYcellCharge_[planeID]->SetBinError(bY,binError);
        }
        else
            continue;
    }

    for(int p=0; p<nBinsY; p++) hYchargeTmp[p]->Delete("0");

    nBinsY = h2DYcellChargeSumLE2_[planeID]->GetNbinsX();
    TH1D* hYchargeSumLE2Tmp[nBinsX];
    for(int bY=1; bY<=nBinsY; ++bY)
    {
        hName.str(""); hName << "YchargeSumLE2_Proj_bin_" << bY;
        hYchargeSumLE2Tmp[bY-1] = h2DYcellChargeSumLE2_[planeID]->ProjectionY(hName.str().c_str(),bY,bY);
        if(hYchargeSumLE2Tmp[bY-1]->GetEntries()!=0)
        {
            binError = hYchargeSumLE2Tmp[bY-1]->GetRMS()/sqrt(hYchargeSumLE2Tmp[bY-1]->GetEntries());
            h1DYcellChargeSumLE2_[planeID]->SetBinError(bY,binError);
        }
        else
            continue;
    }

    for(int p=0; p<nBinsY; p++) hYchargeSumLE2Tmp[p]->Delete("0");

    nBinsY = h2DYcellChargeSumLE3_[planeID]->GetNbinsX();
    TH1D* hYchargeSumLE3Tmp[nBinsX];
    for(int bY=1; bY<=nBinsY; ++bY)
    {
        hName.str(""); hName << "YchargeSumLE3_Proj_bin_" << bY;
        hYchargeSumLE3Tmp[bY-1] = h2DYcellChargeSumLE3_[planeID]->ProjectionY(hName.str().c_str(),bY,bY);
        if(hYchargeSumLE3Tmp[bY-1]->GetEntries()!=0)
        {
            binError = hYchargeSumLE3Tmp[bY-1]->GetRMS()/sqrt(hYchargeSumLE3Tmp[bY-1]->GetEntries());
            h1DYcellChargeSumLE3_[planeID]->SetBinError(bY,binError);
        }
        else
            continue;
    }

    for(int p=0; p<nBinsY; p++) hYchargeSumLE3Tmp[p]->Delete("0");

    double errX1[nBinsX];
    double errX2[nBinsX];
    double errY1[nBinsY];
    double errY2[nBinsY];

    for (int ix = 0; ix < nBinsX; ++ix)
    {
        errX1[ix] = sqrt((1/h1DXcellSingleHits_[planeID]->GetBinContent(ix)) + (1/h1DXallTracks_[planeID]->GetBinContent(ix)));
        errX2[ix] = sqrt((1/h1DXcellDoubleHits_[planeID]->GetBinContent(ix)) + (1/h1DXallTracks_[planeID]->GetBinContent(ix)));
    }
    for (int iy = 0; iy < nBinsY; ++iy)
    {
        errY1[iy] = sqrt((1/h1DYcellSingleHits_[planeID]->GetBinContent(iy)) + (1/h1DYallTracks_[planeID]->GetBinContent(iy)));
        errY2[iy] = sqrt((1/h1DYcellDoubleHits_[planeID]->GetBinContent(iy)) + (1/h1DYallTracks_[planeID]->GetBinContent(iy)));
    }

    h1DXcellSingleHits_           [planeID]->Divide(h1DXallTracks_           [planeID]);
    h1DXcellDoubleHits_           [planeID]->Divide(h1DXallTracks_           [planeID]);
    h1DYcellSingleHits_           [planeID]->Divide(h1DYallTracks_           [planeID]);
    h1DYcellDoubleHits_           [planeID]->Divide(h1DYallTracks_           [planeID]);

    for (int jx = 0; jx < nBinsX; ++jx)
    {
        h1DXcellSingleHits_[planeID]->SetBinError(jx, h1DXcellSingleHits_[planeID]->GetBinContent(jx)*errX1[jx]);
        h1DXcellDoubleHits_[planeID]->SetBinError(jx, h1DXcellDoubleHits_[planeID]->GetBinContent(jx)*errX2[jx]);
    }
    for (int jy = 0; jy < nBinsY; ++jy)
    {
        h1DYcellSingleHits_[planeID]->SetBinError(jy, h1DYcellSingleHits_[planeID]->GetBinContent(jy)*errY1[jy]);
        h1DYcellDoubleHits_[planeID]->SetBinError(jy, h1DYcellDoubleHits_[planeID]->GetBinContent(jy)*errY2[jy]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::clusterSize(int planeID, const Data& data, int threadNumber)
{
    if( data.getHasHit(planeID) )
    {
        THREADED(hClusterSize_ [planeID])->Fill(data.getClusterSize(planeID));
        THREADED(hNumberOfCols_[planeID])->Fill(data.getNumberOfCols(planeID));
        THREADED(hNumberOfRows_[planeID])->Fill(data.getNumberOfRows(planeID));

        if(data.getClusterSize(planeID) <= 4)
        {
            const  Window* theWindow = theWindowsManager_->getWindow(planeID);
            bool passStandardCuts = true;
            for(int h=0; h<data.getClusterSize(planeID); h++)
            {
                if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber()) //hits must fall in the window
                       || !data.getIsPixelCalibrated(h,planeID)                                                                           //pixels must be calibrated
                       ||  data.getClusterPixelCharge(h,planeID) < standardCutsThreshold_   )                                            //charge must be over threshold

                {
                    passStandardCuts = false;
                    //                 std::cout << __PRETTY_FUNCTION__ << "w: " << !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber())
                    //                                                                                 << " c: " << !data.getIsPixelCalibrated(h,planeID)
                    //                                                                                 << " p: " << data.getClusterPixelCharge(h,planeID)
                    //                                                                                 << " t: " << standardCutsThreshold_ << std::endl;
                }
            }
            if(passStandardCuts)
            {
                THREADED(hClusterSizeStandardCutsThreshold_ [planeID])->Fill(data.getClusterSize(planeID));

                if(cutsFormulas_.find("cell Landau") != cutsFormulas_.end())
                    if (cutsFormulas_["cell Landau"][threadNumber]->EvalInstance())
                        THREADED(hClusterSizeStandardCutsThresholdAndCellLandau_[planeID])->Fill(data.getClusterSize(planeID));
            }
        }
    }

    if(!data.getIsInDetector(planeID))
        return;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindowAbout(col,row,run, thePlaneMapping_->getPlaneType(planeID)) )
        return;

    float maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())                                   ;
    float maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str())                                   ;

    if(!data.getHasHit(planeID) )
        return;

    if( data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY )
        return;

    if(!passStandardCuts(planeID,data))
        return;

    if(!passCalibrationsCut(planeID,data))
        return;

    if (data.getClusterCharge(planeID) > standardCutsMaximumTotalCharge_)
        return;

    float xRes = 0;
    float yRes = 0;

    if( data.getXPixelResidualLocal(planeID) > 0 )
        xRes = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
    else if( data.getXPixelResidualLocal(planeID) <= 0 )
        xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);

    if( data.getYPixelResidualLocal(planeID) > 0 )
        yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
    else if( data.getYPixelResidualLocal(planeID) <= 0 )
        yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);

    int clusterSize = data.getClusterSize(planeID);

    if (clusterSize == 2)
    {
        for(int h=0; h<clusterSize; h++)
        {
            if(    data.getClusterPixelRow      (h,planeID) == row
                   && data.getClusterPixelCol   (h,planeID) == col)
                //&& data.getIsPixelCalibrated (h,planeID)
                //&& data.getClusterPixelCharge(h,planeID) > standardCutsThreshold_
                //&& data.getClusterPixelCharge(h,planeID) < standardCutsMaximumCharge_   )
            {
                THREADED(hClusterSizeDistribution2s_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
                THREADED(hClusterSizeNormalization_   [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
                break;
            }
        }
    }

    if(clusterSize > 1)
        return;

    int hitId = -1;

    for(int h=0; h<clusterSize; h++)
    {
        if(       data.getClusterPixelRow      (h,planeID) == row
                  && data.getClusterPixelCol   (h,planeID) == col)
            //&& data.getIsPixelCalibrated (h,planeID)
            //&& data.getClusterPixelCharge(h,planeID) > standardCutsThreshold_
            //&& data.getClusterPixelCharge(h,planeID) < standardCutsMaximumCharge_   )
        {
            THREADED(hClusterSizeDistribution1s_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
            //THREADED(hClusterSizeNormalization_   [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
            hitId = h;
            break;
        }
    }

    if (hitId == -1)
    {
        for (int h=0; h<clusterSize; h++)
        {
            //if (data.getClusterPixelCharge(l,planeID) < standardCutsThreshold_ || data.getClusterPixelCharge(l,planeID) > standardCutsMaximumCharge_ || !data.getIsPixelCalibrated(l,planeID))
            //  continue;

            if (xRes <= 0 && data.getClusterPixelCol(h,planeID) - col == 1 && data.getClusterPixelRow(h,planeID) == row )
            {
                THREADED(hClusterSizeDistribution1s_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
                THREADED(hClusterSizeNormalization_   [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
            }
            else if (xRes > 0 && data.getClusterPixelCol(h,planeID) - col == -1 && data.getClusterPixelRow(h,planeID) == row )
            {
                THREADED(hClusterSizeDistribution1s_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
                THREADED(hClusterSizeNormalization_   [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
            }
            if (yRes <= 0 && data.getClusterPixelRow(h,planeID) - row == 1 && data.getClusterPixelCol(h,planeID) == col )
            {
                THREADED(hClusterSizeDistribution1s_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
                THREADED(hClusterSizeNormalization_   [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
            }
            else if (yRes > 0 && data.getClusterPixelRow(h,planeID) - row == -1 && data.getClusterPixelCol(h,planeID) == col )
            {
                THREADED(hClusterSizeDistribution1s_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
                THREADED(hClusterSizeNormalization_   [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
            }
        }
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::clusterLandau(bool pass, int planeID, const Data& data, int threadNumber)
{
    //cut -> "cluster Landau"
    if( !pass || !data.getHasHit(planeID) )
        return;

    int clusterSize = data.getClusterSize(planeID);

    if( clusterSize > 3 )
        return;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);

    for(int h=0; h<clusterSize; h++)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber()) //hits must fall in the window
               || !data.getIsPixelCalibrated(h,planeID)                                                                           //pixels must be calibrated
               ||  data.getClusterPixelCharge(h,planeID) < standardCutsThreshold_   )                                             //charge must be over threashold
            return;
    }

    int clusterCharge = data.getClusterCharge(planeID);

    if( clusterSize == 1 )
        THREADED(hLandauClusterSize1_[planeID])->Fill(clusterCharge);
    else if( clusterSize == 2 )
        THREADED(hLandauClusterSize2_[planeID])->Fill(clusterCharge);
    else if( clusterSize == 3 )
        THREADED(hLandauClusterSize3_[planeID])->Fill(clusterCharge);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::cellLandau(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getHasHit(planeID) )
        return;

    int size = data.getClusterSize(planeID);
    if (size > 4) return;
    const Window* theWindow = theWindowsManager_->getWindow(planeID);

    for(int h=0; h<size; ++h)
    {
        if(!theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),data.getRunNumber()) || !data.getIsPixelCalibrated (h,planeID)) return;
        THREADED(hCellLandauSinglePixel_[planeID])->Fill(data.getClusterPixelCharge(h, planeID));
    }

    if( size != 1 )
        return;

    if(    theWindow->checkWindow(data.getClusterPixelCol(0,planeID),data.getClusterPixelRow(0,planeID),data.getRunNumber())
           && data.getIsPixelCalibrated(0,planeID)
           && data.getClusterCharge(planeID) > standardCutsThreshold_  )
        THREADED(hCellLandau_[planeID])->Fill(data.getClusterCharge(planeID));

    if (data.getXPixelResidualLocal(planeID) > -65 &&
            data.getXPixelResidualLocal(planeID) <  65 &&
            data.getYPixelResidualLocal(planeID) > -40 &&
            data.getYPixelResidualLocal(planeID) <  40)
    {
        if (data.getXPixelResidualLocal(planeID) > -52.5 &&
                data.getXPixelResidualLocal(planeID) < -22.5 &&
                data.getYPixelResidualLocal(planeID) > -15   &&
                data.getYPixelResidualLocal(planeID) <  15)
            THREADED(hCellLandau3DElectrodes_[planeID])->Fill(data.getClusterCharge(planeID)); //hits in the range of the left electrode (-37.5, 0), within 15 um
        else if (data.getXPixelResidualLocal(planeID) >  22.5 &&
                 data.getXPixelResidualLocal(planeID) <  52.5 &&
                 data.getYPixelResidualLocal(planeID) > -15   &&
                 data.getYPixelResidualLocal(planeID) <  15)
            THREADED(hCellLandau3DElectrodes_[planeID])->Fill(data.getClusterCharge(planeID)); //hits in the range of the right electrode (37.5, 0), within 15 um
        else
            THREADED(hCellLandau3D_[planeID])->Fill(data.getClusterCharge(planeID));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::xLandau(bool pass, int planeID, const Data &data, int threadNumber)
{
    if( !pass || !data.getHasHit(planeID) || data.getClusterSize(planeID) < 2 )
        return;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);
    if (size > 4) return;
    for(int h=0; h<size; ++h)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) //hits must fall in the window
               ||  data.getClusterPixelRow   (h,planeID) != row                                               //hits must be on the same row (sharing is along the row - x direction)
               || !data.getIsPixelCalibrated (h,planeID)                                                      //pixels must be calibrated
               ||  data.getClusterPixelCharge(h,planeID) < standardCutsThreshold_  )                          //charge must be over threshold
            return;
    }

    if( size == 2)
        THREADED(hLandauClusterSize2sameRow_[planeID])->Fill(data.getClusterCharge(planeID));
    else if( size == 3)
        THREADED(hLandauClusterSize3sameRow_[planeID])->Fill(data.getClusterCharge(planeID));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::yLandau(bool pass, int planeID, const Data &data, int threadNumber)
{
    if( !pass || !data.getHasHit(planeID) || data.getClusterSize(planeID) < 2 )
        return;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);
    if (size > 4) return;
    for(int h=0; h<size; ++h)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) //hits must fall in the window
               ||  data.getClusterPixelCol   (h,planeID) !=  col                                                  //hits must be on the same column (sharing is along the column - y direction)
               || !data.getIsPixelCalibrated (h,planeID)                                                          //pixels must be calibrated
               ||  data.getClusterPixelCharge(h,planeID) < standardCutsThreshold_ )                               //charge must be over threshold
            return;
    }

    if( size ==2 )
        THREADED(hLandauClusterSize2sameCol_[planeID])->Fill(data.getClusterCharge(planeID));
    else if( size == 3 )
        THREADED(hLandauClusterSize3sameCol_[planeID])->Fill(data.getClusterCharge(planeID));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Charge::cellCharge(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getIsInDetector(planeID) )
        return;

    float maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())                                   ;
    float maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str())                                   ;

    if( data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY )
        return;

    float xPixelResidual = data.getXPixelResidualLocal(planeID);
    float yPixelResidual = data.getYPixelResidualLocal(planeID);
    float xPixelEdgeResidual = 0; //Residual from the edge of the pixel (the divide between 2 pixels)
    float yPixelEdgeResidual = 0; //Residual from the edge of the pixel (the divide between 2 pixels)

    if( xPixelResidual > 0 )
        xPixelEdgeResidual = -data.getXPitchLocal(planeID)/2 + xPixelResidual;
    else if( xPixelResidual <= 0 )
        xPixelEdgeResidual = xPixelResidual + data.getXPitchLocal(planeID)/2;

    if( yPixelResidual > 0 )
        yPixelEdgeResidual = -data.getYPitchLocal(planeID)/2 + yPixelResidual;
    else if( yPixelResidual <= 0 )
        yPixelEdgeResidual = yPixelResidual + data.getYPitchLocal(planeID)/2;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    THREADED(h2DAllTracks_      [planeID])->Fill(xPixelResidual,yPixelResidual);
    THREADED(h2DAllTracks4Rows_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));

    if (data.getClusterSize(planeID) == 1)
        THREADED(h2DCellChargeSize1AllTracksNorm_[planeID])->Fill(xPixelResidual, yPixelResidual);
    else if (data.getClusterSize(planeID) == 2)
        THREADED(h2DCellChargeSize2AllTracksNorm_[planeID])->Fill(xPixelResidual, yPixelResidual);

    THREADED(h4CellsAllTracks_[planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual);

    int size = data.getClusterSize(planeID); //ClusterSize

    ////////////////////////////////////////////////////////////////
    //FROM NOW ON ONLY TRACKS WITH A HIT ON THE PLANE ARE CONSIDERED
    ////////////////////////////////////////////////////////////////
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

    for(int h=0; h<size; h++)
    {
        if(    data.getClusterPixelRow      (h,planeID) == row
               && data.getClusterPixelCol   (h,planeID) == col
               //&& data.getIsPixelCalibrated (h,planeID)
               //&& data.getClusterPixelCharge(h,planeID) > standardCutsThreshold_
               //&& data.getClusterPixelCharge(h,planeID) < standardCutsMaximumCharge_
               )
        {
            THREADED(h4CellsChargeNorm_       [planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual);
            THREADED(h2DCellCharge_           [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(h,planeID));
            THREADED(h2DCellChargeNum_        [planeID])->Fill(xPixelResidual,yPixelResidual,data.getClusterPixelCharge(h,planeID));
            THREADED(h2DCellChargeNorm_       [planeID])->Fill(xPixelResidual,yPixelResidual);

            /*-------------------------------------------------/////     4 ROWS HISTOGRAMS     ////------------------------------------------------------------------------------------------------------------------------------------------*/

            THREADED(h2DCellCharge4RowsNorm_        [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
            THREADED(h2DCellPixelCharge4Rows_       [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
            THREADED(h2DCellClusterCharge4Rows_     [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterCharge(planeID));
            THREADED(h2DCellCharge4RowsClusterSize_ [planeID])->Fill(xPixelResidual,yPixelResidual+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterSize(planeID));

            if(size==1)
            {
                THREADED(h2DCellPixelCharge4RowsClusterSize1_     [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DCellPixelCharge4RowsClusterSize1Norm_ [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
            }



            if (size == 2)
            {
                THREADED(h2DCellPixelCharge4RowsClusterSize2_     [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DCellPixelCharge4RowsClusterSize2Norm_ [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));


                for (int g=0; g<size; g++)
                {
                    if (g != h
                            //&& data.getIsPixelCalibrated (g,planeID)
                            //&& data.getClusterPixelCharge(g,planeID) > standardCutsThreshold_
                            //&& data.getClusterPixelCharge(g,planeID) < standardCutsMaximumCharge_
                            )
                    {
                        //                        if ( (xPixelEdgeResidual >= 0 && data.getClusterPixelCol(g,planeID) - col == 1 && data.getClusterPixelRow(g,planeID) == row)
                        //                             || (xPixelEdgeResidual <  0 && data.getClusterPixelCol(g,planeID) - col ==  -1 && data.getClusterPixelRow(g,planeID) == row))
                        //                        {
                        //                            THREADED(h2DCellChargeSecondHit4RowsSameRow_      [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                        //                            THREADED(h2DCellChargeSecondHit4RowsSameRowNorm_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                        //                        }
                        //                        else if  ((yPixelEdgeResidual >  0 && data.getClusterPixelRow(g,planeID) - row == 1 && data.getClusterPixelCol(g,planeID) == col)
                        //                                  || (yPixelEdgeResidual <= 0 && data.getClusterPixelRow(g,planeID) - row ==  -1 && data.getClusterPixelCol(g,planeID) == col))
                        if  (data.getClusterPixelCol(g,planeID) == col)
                        {
                            THREADED(h2DCellChargeSecondHit4RowsSameCol_     [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                            THREADED(h2DCellChargeSecondHit4RowsSameColNorm_ [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                        }

                    }

                }
            }


            else if (size == 3)
            {
                THREADED(h2DCellPixelCharge4RowsClusterSize3_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                THREADED(h2DCellPixelCharge4RowsClusterSize3Norm_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));

                for (int g=0; g<size; g++)
                {

                    if (g != h) //&& data.getIsPixelCalibrated (g,planeID)//&& data.getClusterPixelCharge(g,planeID) > standardCutsThreshold_//&& data.getClusterPixelCharge(g,planeID) < standardCutsMaximumCharge_


                        if ( data.getClusterPixelCol(g,planeID) == col)
                        {
                            THREADED(h2DCellChargeSecondHit4RowsClusterSize3_    [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                            THREADED(h2DCellChargeSecondHit4RowsClusterSize3Norm_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));


                            if(data.getClusterPixelCharge(g,planeID)<data.getClusterPixelCharge(h,planeID))

                            {

                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowCharge_    [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighCharge_    [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                            }


                            else if (data.getClusterPixelCharge(h,planeID)<data.getClusterPixelCharge(g,planeID))
                            {

                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowCharge_    [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighCharge_    [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(g,planeID));
                                THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID));
                            }
                        }
                }
            }





            //  if(row=50)
            //    THREADED(h2DCellCharge4RowsOnly50_      [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID)+((float)((row-50)%4)-1.5)*data.getYPitchLocal(planeID),data.getClusterPixelCharge(h,planeID));


            /*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
            //if (planeID == 9) ++totEventsControl_;//FIXME THIS IS HARDCODED AND RELATES TO A PLANE ID THAT NOW HAS CHANGED
            THREADED(h4CellsCharge_          [planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual,data.getClusterPixelCharge(h,planeID));
            THREADED(hCellChargeCoarse_      [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID),data.getClusterPixelCharge(h,planeID));
            THREADED(hCellChargeCoarseNorm_  [planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
            TH2F* histo = THREADED(hCellChargeCoarse_[planeID]);
            int histoN = (histo->GetXaxis()->FindBin(data.getXPixelResidualLocal(planeID))-1)*histo->GetNbinsY() + histo->GetYaxis()->FindBin(data.getYPixelResidualLocal(planeID))-1;

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
            if (data.getClusterPixelCharge(l,planeID) < standardCutsThreshold_ || data.getClusterPixelCharge(l,planeID) > standardCutsMaximumCharge_ || !data.getIsPixelCalibrated(l,planeID))
                continue;

            if (xPixelEdgeResidual <= 0 && data.getClusterPixelCol(l,planeID) - col == 1 && data.getClusterPixelRow(l,planeID) == row )
            {
                THREADED(h2DCellChargeSecondHit_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID),data.getClusterPixelCharge(l,planeID));
            }
            else if (xPixelEdgeResidual > 0 && data.getClusterPixelCol(l,planeID) - col == -1 && data.getClusterPixelRow(l,planeID) == row )
            {
                THREADED(h2DCellChargeSecondHit_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID),data.getClusterPixelCharge(l,planeID));
            }
            if (yPixelEdgeResidual <= 0 && data.getClusterPixelRow(l,planeID) - row == 1 && data.getClusterPixelCol(l,planeID) == col )
            {
                THREADED(h2DCellChargeSecondHit_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID),data.getClusterPixelCharge(l,planeID));
            }
            else if (yPixelEdgeResidual > 0 && data.getClusterPixelRow(l,planeID) - row == -1 && data.getClusterPixelCol(l,planeID) == col )
            {
                THREADED(h2DCellChargeSecondHit_[planeID])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID),data.getClusterPixelCharge(l,planeID));
            }
        }
    }

    if( size == 4 )
    {

        for(int h=0; h<size; ++h)
        {
            if( !theWindow->checkWindow( data.getClusterPixelCol(h,planeID) , data.getClusterPixelRow(h,planeID),run ) ||
                    !data.getIsPixelCalibrated(h,planeID) ||
                    data.getClusterPixelCharge(h,planeID) < standardCutsThreshold_ ||
                    data.getClusterPixelCharge(h,planeID) > standardCutsMaximumCharge_ ) return;
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
                    xPixelEdgeResidual = +data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;
                    yPixelEdgeResidual = +data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
                }
                else if(isYmin && isXmin)
                {
                    xPixelEdgeResidual = +data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2;
                    yPixelEdgeResidual = +data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;
                }
                else if(isYmax && isXmax)
                {
                    xPixelEdgeResidual = +data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
                    yPixelEdgeResidual = +data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
                }
                else if(isYmin && isXmax)
                {
                    xPixelEdgeResidual = +data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
                    yPixelEdgeResidual = +data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2;
                }

                THREADED(h4Hits_      [planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual);
                THREADED(h4HitsCharge_[planeID])->Fill(xPixelEdgeResidual,yPixelEdgeResidual,data.getClusterPixelCharge(firstHit,planeID));
                break;
            }
        }

    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::xChargeDivision(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getIsInDetector(planeID) )
        return;

    if (data.getYPixelResidualLocal(planeID) > 20 || data.getYPixelResidualLocal(planeID) < -20)
        return;

    float maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())                                   ;
    float maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str())                                   ;

    if( data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY )
        return;

    float xRes = 0;
    float yRes = 0;

    if(data.getXPixelResidualLocal(planeID) > 0)
        xRes = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
    else if(data.getXPixelResidualLocal(planeID) <= 0)
        xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);

    if(data.getYPixelResidualLocal(planeID) > 0)
        yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
    else if(data.getYPixelResidualLocal(planeID) <= 0)
        yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindowAbout(col,row,run,thePlaneMapping_->getPlaneType(planeID)) )
        return;

    THREADED(h1DXallTracks_[planeID])->Fill(xRes);

    int size = data.getClusterSize(planeID);
    if( data.getHasHit(planeID) && size <= 3 )
    {
        for(int h=0; h<size; ++h)
        {
            if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) //hits are in the window
                   || !data.getIsPixelCalibrated(h,planeID)                                                           //pixels are calibrated
                   ||  data.getClusterPixelRow    (h,planeID) != row                                                  //hits are on the same row (sharing is along the row - x direction)
                   ||  data.getClusterPixelCharge (h,planeID) < standardCutsThreshold_                                //charge is over threshold
                   ||  data.getClusterPixelCharge (h,planeID) > standardCutsMaximumCharge_   )                        //maximum allowed charge for this physics
                return;
        }

        int hitID = -1;
        for(int h=0; h<size; ++h)
        {
            if( data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col )
            {
                THREADED(h2DXcellCharge_          [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
                THREADED(h1DXcellCharge_          [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
                THREADED(h1DXcellChargeNormToAll_ [planeID])->Fill(xRes,data.getClusterPixelCharge(h,planeID));
                THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(h, planeID));
                THREADED(h1DXcellChargeSecondHitNorm_[planeID])->Fill(xRes);
                THREADED(h1DXcellChargeNorm_[planeID])->Fill(xRes);
                hitID = h;
                if (size > 1)
                {
                    for (int g = 0; g < size; ++g)
                    {
                        if (g != hitID)
                        {
                            if (xRes >= 0 && data.getClusterPixelCol(g,planeID) - col == 1 && data.getClusterPixelRow(g,planeID) == row)
                            {
                                THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(g, planeID));
                                THREADED(h1DXcellChargeSecondHitNorm_[planeID])->Fill(xRes);
                            }
                            else if (xRes < 0 && data.getClusterPixelCol(g,planeID) - col == -1 && data.getClusterPixelRow(g,planeID) == row)
                            {
                                THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(g, planeID));
                                THREADED(h1DXcellChargeSecondHitNorm_[planeID])->Fill(xRes);
                            }
                        }
                    }
                }
                break;
            }
        }

        if( hitID == -1 )
        {
            for (int l =0; l<size; ++l)
            {
                if (xRes <= 0 && data.getClusterPixelCol(l,planeID) - col == 1 && data.getClusterPixelRow(l,planeID) == row )
                {
                    THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(l, planeID));
                    THREADED(h1DXcellChargeSecondHitNorm_[planeID])->Fill(xRes);
                }
                else if (xRes > 0 && data.getClusterPixelCol(l,planeID) - col == -1 && data.getClusterPixelRow(l,planeID) == row )
                {
                    THREADED(h1DXcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(l, planeID));
                    THREADED(h1DXcellChargeSecondHitNorm_[planeID])->Fill(xRes);
                }
            }
            return;
        }

        if( size == 1 )
        {
            THREADED(h2DXcellSingleHits_ [planeID])->Fill(xRes,yRes);
            THREADED(h1DXcellSingleHits_ [planeID])->Fill(xRes);
        }
        else if( size == 2 )
        {
            for(int h=0; h<size; ++h)
            {
                if( h == hitID )
                    continue;
                else if(    (data.getXPixelResidualLocal(planeID)  < 0  && (col - data.getClusterPixelCol(h,planeID)) ==  1)
                            || (data.getXPixelResidualLocal(planeID)  > 0  && (col - data.getClusterPixelCol(h,planeID)) == -1) )
                    return;
            }

            THREADED(h1DXcellDoubleHits_[planeID])->Fill(xRes);
            THREADED(h2DXcellDoubleHits_[planeID])->Fill(xRes,yRes);
        }

        if( data.getClusterCharge(planeID) <= standardCutsMaximumTotalCharge_ )
        {
            if( size <= 2 )
            {
                THREADED(h1DXcellChargeSumLE2Norm_      [planeID])->Fill(xRes);
                THREADED(h1DXcellChargeSumLE2_          [planeID])->Fill(xRes,data.getClusterCharge(planeID));
                THREADED(h1DXcellChargeSumLE2NormToAll_ [planeID])->Fill(xRes,data.getClusterCharge(planeID));
                THREADED(h2DXcellChargeSumLE2_          [planeID])->Fill(xRes,data.getClusterCharge(planeID));
            }
            if( size <= 3 )
            {
                THREADED(h1DXcellChargeSumLE3Norm_      [planeID])->Fill(xRes);
                THREADED(h1DXcellChargeSumLE3_          [planeID])->Fill(xRes,data.getClusterCharge(planeID));
                THREADED(h1DXcellChargeSumLE3NormToAll_ [planeID])->Fill(xRes,data.getClusterCharge(planeID));
                THREADED(h2DXcellChargeSumLE3_          [planeID])->Fill(xRes,data.getClusterCharge(planeID));
            }
            if( size == 3 )
                THREADED(h1DXcell3Hits_                 [planeID])->Fill(yRes);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::xAsimmetry(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getIsInDetector(planeID))
        return;
    if (data.getYPixelResidualLocal(planeID) > 30 || data.getYPixelResidualLocal(planeID) < -30)
        return;

    float maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())                                   ;

    if( data.getXPitchLocal(planeID) > maxPitchX )
        return;

    float xRes = 0;

    if(data.getXPixelResidualLocal(planeID) > 0)
        xRes = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
    else if(data.getXPixelResidualLocal(planeID) <= 0)
        xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);
    if (size > 4) return;

    int  hitID = -1;
    for(int h=0; h<size; ++h)
    {
        if( data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col )
        {
            hitID = h;
            break;
        }
    }

    if( hitID == -1 )
        return;

    for(int h=0; h<size; ++h)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run)  //hits are in the window
               || !data.getIsPixelCalibrated(h,planeID)                                                            //pixels are calibrated
               ||  data.getClusterPixelRow    (h,planeID) != row                                                   //hits are on the same row (sharing is along the row - x direction)
               ||  data.getClusterPixelCharge (h,planeID) <  standardCutsThreshold_                                           //charge is over threshold
               ||  data.getClusterPixelCharge (h,planeID) >  standardCutsMaximumCharge_                                            //maximum allowed charge for this physics
               )
            return;
    }

    if( data.getHasHit(planeID) && size == 2 )
    {
        float Asimmetry   = 0;
        int   totalCharge = 0;
        int   chargeLeft  = 0;
        int   chargeRight = 0;
        float Asimmetry0  = 0;
        for(int h=0; h<size; ++h)
        {
            if( h == hitID )
                continue;
            if(data.getXPixelResidualLocal(planeID) > 0 && (col - data.getClusterPixelCol(h,planeID)) == -1)//il secondo hit e' a DX della predetta
            {
                chargeRight = data.getClusterPixelCharge(h    ,planeID);
                chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
                break;
            }
            else if(data.getXPixelResidualLocal(planeID) <= 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)//il secondo hit e' a SX della predetta
            {
                chargeRight  = data.getClusterPixelCharge(hitID,planeID);
                chargeLeft = data.getClusterPixelCharge(h    ,planeID);
                break;
            }
            else if(data.getXPixelResidualLocal(planeID) > 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)
            {
                chargeRight  = data.getClusterPixelCharge(hitID,planeID);
                chargeLeft = data.getClusterPixelCharge(h    ,planeID);
                xRes = data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
                break;
            }
            else if(data.getXPixelResidualLocal(planeID) <= 0 && (col - data.getClusterPixelCol(h,planeID)) ==  -1)
            {
                chargeRight  = data.getClusterPixelCharge(h    ,planeID);
                chargeLeft = data.getClusterPixelCharge(hitID,planeID);
                xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);
                break;
            }
            else
                return;
        }

        totalCharge = chargeLeft + chargeRight;
        if (totalCharge > 0)
        {
            Asimmetry = (float)(chargeLeft - chargeRight)/totalCharge;
            Asimmetry0 = (float)(chargeLeft/totalCharge);
        }
        if( totalCharge >= standardCutsMinimumTotalCharge_ && totalCharge <= standardCutsMaximumTotalCharge_)
        {
            THREADED(h2DXcellChargeAsimmetry_                [planeID])->Fill(xRes, Asimmetry);
            THREADED(h2DXCellChargeAsimmetryY_               [planeID])->Fill(data.getYPixelResidualLocal(planeID), Asimmetry);
            THREADED(h2DXCellChargeAsimmetrySizeLE2_         [planeID])->Fill(xRes, Asimmetry);
            THREADED(h2DXcellChargeAsimmetryInv_             [planeID])->Fill(Asimmetry, xRes);
            THREADED(h2DXCellChargeAsimmetryCell_            [planeID])->Fill(data.getXPixelResidualLocal(planeID), data.getYPixelResidualLocal(planeID), Asimmetry);
            THREADED(h2DXCellChargeAsimmetryCellNorm_        [planeID])->Fill(data.getXPixelResidualLocal(planeID), data.getYPixelResidualLocal(planeID));
            THREADED(h2DXAsimmetryLandau_                    [planeID])->Fill(totalCharge, Asimmetry);
            THREADED(hXAsimmetry_                            [planeID])->Fill(Asimmetry);
            THREADED(hXAsimmetry0_                           [planeID])->Fill(Asimmetry0);
        }
    }

    if( data.getHasHit(planeID) && size == 1 )
    {
        float Asimmetry   = 0;
        int   totalCharge = 0;

        if(data.getClusterPixelCol(0,planeID) == col)//la cella colpita coincide con la predetta
        {
            if(data.getXPixelResidualLocal(planeID) > 0)//la traccia ha colpito a sx
                Asimmetry = 1;
            else if(data.getXPixelResidualLocal(planeID) <= 0)//la traccia ha colpito a dx
                Asimmetry = -1;
        }
        else if(data.getXPixelResidualLocal(planeID) >  0 && (col - data.getClusterPixelCol(0,planeID)) ==  -1)//la cella colpita e' a DX della predetta
        {
            Asimmetry = 1;
            xRes =  data.getXPixelResidualLocal(planeID) - data.getXPitchLocal(planeID)/2;
        }
        else if(data.getXPixelResidualLocal(planeID) <= 0 && (col - data.getClusterPixelCol(0,planeID)) == 1)//la cella colpita e' a SX della predetta
        {
            Asimmetry = -1;
            xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);
        }
        else
            return;

        totalCharge = data.getClusterPixelCharge(0,planeID);

        THREADED(h2DXCellChargeAsimmetrySizeLE2_[planeID])->Fill(xRes, Asimmetry);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::xAsimmetryUnconstr(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getIsInDetector(planeID))
        return;

    float maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())                                   ;

    if( data.getXPixelPitchLocalUnconstrained(planeID) > maxPitchX )
        return;
    if (data.getYPixelResidualLocal(planeID) > 30 || data.getYPixelResidualLocal(planeID) < -30)
        return;

    float xResUnconstrained = 0  ;

    if(data.getXPixelResidualLocalUnconstrained(planeID) > 0)
        xResUnconstrained = data.getXPixelPitchLocalUnconstrained(planeID)/2 - data.getXPixelResidualLocalUnconstrained(planeID);
    else if(data.getXPixelResidualLocalUnconstrained(planeID) <= 0)
        xResUnconstrained = -(data.getXPixelResidualLocalUnconstrained(planeID) + data.getXPixelPitchLocalUnconstrained(planeID)/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);
    if( data.getHasHit(planeID) && size == 2 )
    {
        for(int h=0; h<size; ++h)
        {
            if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run)  //hits are in the window
                   || !data.getIsPixelCalibrated(h,planeID)                                                           //pixels are calibrated
                   ||  data.getClusterPixelRow    (h,planeID) != row                                                  //hits are on the same row (sharing is along the row - x direction)
                   ||  data.getClusterPixelCharge (h,planeID) < standardCutsThreshold_                                           //charge is over threshold
                   ||  data.getClusterPixelCharge (h,planeID) > standardCutsMaximumCharge_   )                                        //maximum allowed charge for this physics
                return;
        }

        int  hitID = -1;
        for(int h=0; h<size; ++h)
        {
            if( data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col )
            {
                hitID = h;
                break;
            }
        }

        if( hitID == -1 )
            return;

        float Asimmetry   = 0;
        int   totalCharge = 0;
        int   chargeLeft  = 0;
        int   chargeRight = 0;
        for(int h=0; h<size; ++h)
        {
            if( h == hitID )
                continue;
            else if(data.getXPixelResidualLocalUnconstrained(planeID) > 0 && (col - data.getClusterPixelCol(h,planeID)) == 1)//il secondo hit e' a SN della predetta
            {
                chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
                chargeRight = data.getClusterPixelCharge(hitID,planeID);
                break;
            }
            else if(data.getXPixelResidualLocalUnconstrained(planeID) <= 0 && (col - data.getClusterPixelCol(h,planeID)) == -1)//il secondo hit e' a DX della predetta
            {
                chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
                chargeRight = data.getClusterPixelCharge(h    ,planeID);
                break;
            }
            else if(data.getXPixelResidualLocalUnconstrained(planeID) > 0 && (col - data.getClusterPixelCol(h,planeID)) == -1)
            {
                chargeLeft        = data.getClusterPixelCharge(hitID,planeID);
                chargeRight       = data.getClusterPixelCharge(h    ,planeID);
                xResUnconstrained = - data.getXPixelResidualLocalUnconstrained(planeID) - data.getXPixelPitchLocalUnconstrained(planeID)/2;
                break;
            }
            else if(data.getXPixelResidualLocalUnconstrained(planeID) < 0 && (col - data.getClusterPixelCol(h,planeID)) ==  1)
            {
                chargeLeft        = data.getClusterPixelCharge(h    ,planeID);
                chargeRight       = data.getClusterPixelCharge(hitID,planeID);
                xResUnconstrained = -(data.getXPixelResidualLocalUnconstrained(planeID) - data.getXPixelPitchLocalUnconstrained(planeID)/2);
                break;
            }
            else
                return;
        }

        totalCharge = chargeLeft + chargeRight;
        Asimmetry = (float)(chargeLeft - chargeRight)/totalCharge;

        if( totalCharge >= standardCutsMinimumTotalCharge_ && totalCharge <= standardCutsMaximumTotalCharge_ )
        {
            THREADED(h2DXcellChargeAsimmetryUnconstrained_   [planeID])->Fill(xResUnconstrained, Asimmetry);
            THREADED(h2DXcellChargeAsimmetryUnconstrainedInv_[planeID])->Fill(Asimmetry, xResUnconstrained);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::yChargeDivision(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getIsInDetector(planeID))
        return;

    float maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())                                   ;
    float maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str())                                   ;

    if( data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY )
        return;
    if (data.getXPixelResidualLocal(planeID) > 55 || data.getXPixelResidualLocal(planeID) < -55)
        return;

    float xRes = 0;
    float yRes = 0;

    if(data.getXPixelResidualLocal(planeID) > 0)
        xRes = -data.getXPitchLocal(planeID)/2 + data.getXPixelResidualLocal(planeID);
    else if(data.getXPixelResidualLocal(planeID) <= 0)
        xRes = (data.getXPixelResidualLocal(planeID) + data.getXPitchLocal(planeID)/2);

    if(data.getYPixelResidualLocal(planeID) > 0)
        yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
    else if(data.getYPixelResidualLocal(planeID) <= 0)
        yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    THREADED(h1DYallTracks_[planeID])->Fill(yRes);
    if (fabs(data.getXPixelResidualLocal(planeID)) > 35)
    {
        THREADED(h1DYallTracksNoElectrodes_[planeID])->Fill(yRes);
    }

    int size = data.getClusterSize(planeID);
    if( data.getHasHit(planeID) && data.getClusterSize(planeID) <= 3 )
    {
        for(int h=0; h<size; ++h)
        {
            if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run) // hits are in the window
                   || !data.getIsPixelCalibrated(h,planeID)                                                       //pixels are calibrated
                   ||  data.getClusterPixelCol    (h,planeID) !=  col                                             //hits are on the same column (sharing is along the column - y direction)
                   ||  data.getClusterPixelCharge (h,planeID) <  standardCutsThreshold_                           //charge is over threshold
                   ||  data.getClusterPixelCharge (h,planeID) > standardCutsMaximumCharge_    )                                   //maximum allowed charge for this physics
                return;
        }

        int hitID = -1;
        for(int h=0; h<size; ++h)
        {
            if(data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col)
            {
                THREADED(h1DYcellCharge_          [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));//yRes=1er cuadro rojo
                THREADED(h1DYcellChargeNormToAll_ [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
                THREADED(h2DYcellCharge_          [planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
                if (fabs(data.getXPixelResidualLocal(planeID)) > 35)
                {
                    THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(h,planeID));
                    THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(yRes);
                    if (size > 1)
                    {
                        for (int g = 0; g < size; ++g)
                        {
                            if (g != hitID)
                            {
                                if (yRes > 0 && data.getClusterPixelRow(g,planeID) - row == 1 && data.getClusterPixelCol(g,planeID) == col)
                                {
                                    THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(xRes,data.getClusterPixelCharge(g, planeID));
                                    THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(xRes);
                                }
                                else if (yRes <= 0 && data.getClusterPixelRow(g,planeID) - row == -1 && data.getClusterPixelCol(g,planeID) == col)
                                {
                                    THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(g, planeID));
                                    THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(yRes);
                                }
                            }
                        }
                    }
                }
                THREADED(h1DYcellChargeNorm_[planeID])->Fill(yRes);
                hitID = h;
                break;
            }
        }

        if( hitID == -1 && fabs(data.getXPixelResidualLocal(planeID)) > 35)
        {
            for (int l =0; l<size; ++l)
            {
                if (yRes <= 0 && data.getClusterPixelRow(l,planeID) - row == 1 && data.getClusterPixelCol(l,planeID) == col )
                {
                    THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(l, planeID));
                    THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(yRes);
                }
                else if (yRes > 0 && data.getClusterPixelRow(l,planeID) - row == -1 && data.getClusterPixelCol(l,planeID) == col )
                {
                    THREADED(h1DYcellChargeSecondHit_[planeID])->Fill(yRes,data.getClusterPixelCharge(l, planeID));
                    THREADED(h1DYcellChargeSecondHitNorm_[planeID])->Fill(yRes);
                }
            }
            return;
        }

        if( size == 1 )
        {
            THREADED(h1DYcellSingleHits_ [planeID])->Fill(yRes);
            THREADED(h2DYcellSingleHits_ [planeID])->Fill(xRes,yRes);
        }
        else if( size == 2 )
        {
            for(int h=0; h<size; ++h)
            {
                if( h == hitID )
                    continue;
                else if(    (data.getYPixelResidualLocal(planeID) < 0 && (row - data.getClusterPixelRow(h,planeID)) ==  +1)
                            || (data.getYPixelResidualLocal(planeID) > 0 && (row - data.getClusterPixelRow(h,planeID)) == 1)  )
                    return;

            }

            THREADED(h1DYcellDoubleHits_[planeID])->Fill(yRes);
            THREADED(h2DYcellDoubleHits_[planeID])->Fill(xRes,yRes);
        }

        if( data.getClusterCharge(planeID) <= standardCutsMaximumTotalCharge_ )
        {
            if( size <= 2 )
            {

                THREADED(h1DYcellChargeSumLE2_         [planeID])->Fill(yRes,data.getClusterCharge(planeID));
                THREADED(h1DYcellChargeSumLE2NormToAll_[planeID])->Fill(yRes,data.getClusterCharge(planeID));
                THREADED(h1DYcellChargeSumLE2Norm_     [planeID])->Fill(yRes);
                THREADED(h2DYcellChargeSumLE2_         [planeID])->Fill(yRes,data.getClusterCharge(planeID));
            }
            if ( size <= 3 )
            {
                THREADED(h1DYcellChargeSumLE3_         [planeID])->Fill(yRes,data.getClusterCharge(planeID));
                THREADED(h1DYcellChargeSumLE3NormToAll_[planeID])->Fill(yRes,data.getClusterCharge(planeID));
                THREADED(h1DYcellChargeSumLE3Norm_     [planeID])->Fill(yRes);
                THREADED(h2DYcellChargeSumLE3_         [planeID])->Fill(yRes,data.getClusterCharge(planeID));
            }
            if( size == 3 )
                THREADED(h1DYcell3Hits_                [planeID])->Fill(yRes);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::yAsimmetry(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getIsInDetector(planeID))
        return;

    float maxPitchY = 100;

    if( data.getYPitchLocal(planeID) > maxPitchY )
        return;
    if (data.getXPixelResidualLocal(planeID) > 55 || data.getXPixelResidualLocal(planeID) < -55)
        return;

    float yRes = 0;

    if(data.getYPixelResidualLocal(planeID) > 0)
        yRes = -data.getYPitchLocal(planeID)/2 + data.getYPixelResidualLocal(planeID);
    else if(data.getYPixelResidualLocal(planeID) <= 0)
        yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);
    if (size > 4) return;

    int hitID = -1;
    for(int h=0; h<size; ++h)
    {
        if( data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col )
        {
            hitID = h;
            break;
        }
    }

    if( hitID == -1 )
        return;


    for(int h=0; h<size; ++h)
    {
        if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run)  // hits are in the window
               || !data.getIsPixelCalibrated(h,planeID)                                                        //pixels are calibrated
               ||  data.getClusterPixelCol    (h,planeID) != col                                               //hits are on the same column (sharing is along the column - y direction)
               ||  data.getClusterPixelCharge (h,planeID) <  standardCutsThreshold_                            //charge is over threshold
               ||  data.getClusterPixelCharge (h,planeID) >  standardCutsMaximumCharge_    )                                   //maximum allowed charge for this physics
            return;
    }

    if( data.getHasHit(planeID) && size == 2 )
    {
        float Asimmetry   = 0;
        int   totalCharge = 0;
        int   chargeLeft  = 0;
        int   chargeRight = 0;
        for(int h=0; h<size; ++h)
        {
            if( h == hitID )
                continue;
            else if(data.getYPixelResidualLocal(planeID) > 0 && (row - data.getClusterPixelRow(h,planeID)) == -1)//il secondo hit e' a Dx della predetta
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
                chargeRight = data.getClusterPixelCharge(hitID,planeID);
                chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
                yRes = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
                break;
            }
            else if(data.getYPixelResidualLocal(planeID) <= 0 && (row - data.getClusterPixelRow(h,planeID)) ==  -1)
            {
                chargeRight = data.getClusterPixelCharge(h    ,planeID);
                chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
                yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);
                break;
            }
            else
                return;
        }

        totalCharge = chargeLeft + chargeRight;
        Asimmetry = (float)(chargeLeft - chargeRight)/totalCharge;
        float Asimmetry0 = (float)(chargeLeft/totalCharge);

        if( totalCharge >= standardCutsMinimumTotalCharge_ && totalCharge <= standardCutsMaximumTotalCharge_ )
        {
            THREADED(h2DYAsimmetryLandau_                    [planeID])->Fill(totalCharge,Asimmetry);
            THREADED(hYAsimmetry_                            [planeID])->Fill(Asimmetry);
            THREADED(hYAsimmetry0_                           [planeID])->Fill(Asimmetry0);
            THREADED(h2DYcellChargeAsimmetry_                [planeID])->Fill(yRes, Asimmetry);
            THREADED(h2DYCellChargeAsimmetryX_               [planeID])->Fill(data.getXPixelResidualLocal(planeID), Asimmetry);
            THREADED(h2DYCellChargeAsimmetrySizeLE2_         [planeID])->Fill(yRes, Asimmetry);
            THREADED(h2DYcellChargeAsimmetryInv_             [planeID])->Fill(Asimmetry, yRes);
            THREADED(h2DYCellChargeAsimmetryCell_            [planeID])->Fill(data.getXPixelResidualLocal(planeID), data.getYPixelResidualLocal(planeID), Asimmetry);
            THREADED(h2DYCellChargeAsimmetryCellNorm_        [planeID])->Fill(data.getXPixelResidualLocal(planeID), data.getYPixelResidualLocal(planeID));
        }
    }
    if(data.getHasHit(planeID) && size == 1)
    {
        float Asimmetry   = 0;
        int   totalCharge = 0;

        if(data.getClusterPixelRow(0,planeID) == row)//la cella colpita coincide con la predetta
        {
            if(data.getYPixelResidualLocal(planeID) > 0)//la traccia ha colpito a sx
                Asimmetry = 1;
            else if(data.getYPixelResidualLocal(planeID) <= 0)//la traccia ha colpito a dx
                Asimmetry = -1;
        }
        else if(data.getYPixelResidualLocal(planeID) >  0 && (row - data.getClusterPixelRow(0,planeID)) ==  -1)//la cella colpita e' a DX della predetta
        {
            Asimmetry =  1;
            yRes = data.getYPixelResidualLocal(planeID) - data.getYPitchLocal(planeID)/2;
        }
        else if(data.getYPixelResidualLocal(planeID) <= 0 && (row - data.getClusterPixelRow(0,planeID)) == 1)//la cella colpita e' a SX della predetta
        {
            Asimmetry = -1;
            yRes = (data.getYPixelResidualLocal(planeID) + data.getYPitchLocal(planeID)/2);
        }
        else
            return;

        totalCharge = data.getClusterPixelCharge(0,planeID);

        THREADED(h2DYCellChargeAsimmetrySizeLE2_[planeID])->Fill(yRes, Asimmetry);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::yAsimmetryUnconstr(bool pass, int planeID, const Data& data, int threadNumber)
{
    if( !pass || !data.getIsInDetector(planeID))
        return;

    float maxPitchY = 100;

    if( data.getYPixelPitchLocalUnconstrained(planeID) > maxPitchY )
        return;
    if (data.getXPixelResidualLocal(planeID) > 55 || data.getXPixelResidualLocal(planeID) < -55)
        return;

    float yResUnconstrained  = 0;

    if(data.getYPixelResidualLocalUnconstrained(planeID) > 0)
        yResUnconstrained = data.getYPixelPitchLocalUnconstrained(planeID)/2 - data.getYPixelResidualLocalUnconstrained(planeID);
    else if(data.getYPixelResidualLocalUnconstrained(planeID) <= 0)
        yResUnconstrained = -(data.getYPixelResidualLocalUnconstrained(planeID) + data.getYPixelPitchLocalUnconstrained(planeID)/2);

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);
    if( data.getHasHit(planeID) && size == 2 )
    {
        for(int h=0; h<size; ++h)
        {
            if(    !theWindow->checkWindow(data.getClusterPixelCol(h,planeID),data.getClusterPixelRow(h,planeID),run)  // hits are in the window
                   || !data.getIsPixelCalibrated(h,planeID)                                                        //pixels are calibrated
                   ||  data.getClusterPixelCol    (h,planeID) != col                                               //hits are on the same column (sharing is along the column - y direction)
                   ||  data.getClusterPixelCharge (h,planeID) <  standardCutsThreshold_                            //charge is over threshold
                   ||  data.getClusterPixelCharge (h,planeID) >  standardCutsMaximumCharge_    )                                   //maximum allowed charge for this physics
                return;
        }

        int hitID = -1;
        for(int h=0; h<size; ++h)
        {
            if( data.getClusterPixelRow(h,planeID) == row && data.getClusterPixelCol(h,planeID) == col )
            {
                hitID = h;
                break;
            }
        }

        if( hitID == -1 )
            return;

        float Asimmetry   = 0;
        int   totalCharge = 0;
        int   chargeLeft  = 0;
        int   chargeRight = 0;
        for(int h=0; h<size; ++h)
        {
            if( h == hitID )
                continue;
            else if(data.getYPixelResidualLocalUnconstrained(planeID) > 0 && (row - data.getClusterPixelRow(h,planeID)) == 1)//il secondo hit e' a SN della predetta
            {
                chargeLeft  = data.getClusterPixelCharge(h    ,planeID);
                chargeRight = data.getClusterPixelCharge(hitID,planeID);
                break;
            }
            else if(data.getYPixelResidualLocalUnconstrained(planeID) <= 0 && (row - data.getClusterPixelRow(h,planeID)) == -1)//il secondo hit e' a DX della predetta
            {
                chargeLeft  = data.getClusterPixelCharge(hitID,planeID);
                chargeRight = data.getClusterPixelCharge(h    ,planeID);
                break;
            }
            else if(data.getYPixelResidualLocalUnconstrained(planeID) > 0 && (row - data.getClusterPixelRow(h,planeID)) == -1)
            {
                chargeLeft        = data.getClusterPixelCharge(hitID,planeID);
                chargeRight       = data.getClusterPixelCharge(h    ,planeID);
                yResUnconstrained = - data.getYPixelResidualLocalUnconstrained(planeID) - data.getYPixelPitchLocalUnconstrained(planeID)/2;
                break;
            }
            else if(data.getYPixelResidualLocalUnconstrained(planeID) < 0 && (row - data.getClusterPixelRow(h,planeID)) ==  1)
            {
                chargeLeft        = data.getClusterPixelCharge(h    ,planeID);
                chargeRight       = data.getClusterPixelCharge(hitID,planeID);
                yResUnconstrained = -(data.getYPixelResidualLocalUnconstrained(planeID) - data.getYPixelPitchLocalUnconstrained(planeID)/2);
                break;
            }
            else
                return;
        }

        totalCharge = chargeLeft + chargeRight;
        Asimmetry = (float)(chargeLeft - chargeRight)/totalCharge;

        if( totalCharge >= standardCutsMinimumTotalCharge_ && totalCharge <= standardCutsMaximumTotalCharge_ )
        {
            THREADED(h2DYcellChargeAsimmetryUnconstrained_   [planeID])->Fill(yResUnconstrained, Asimmetry);
            THREADED(h2DYcellChargeAsimmetryUnconstrainedInv_[planeID])->Fill(Asimmetry, yResUnconstrained);
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::setCutsFormula(std::map<std::string,std::string> cutsList,std::vector<TTree*> tree)
{
    std::vector<TTreeFormula*> formulasVector;

    for(std::map<std::string,std::string>::iterator it=cutsList.begin(); it!=cutsList.end(); it++)
    {
        if((it->first) == "main cut" && (it->second).size()==0)
            STDLINE("WARNING: no main cut set in charge analysis!! Default value = true!", ACRed);

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
void Charge::setParsLimits(void)
{
    std::stringstream ss;

    for(unsigned int pl=0; pl<thePlaneMapping_->getNumberOfPlanes(); pl++)
    {
        if(thePlaneMapping_->getPlaneName(pl).find("Dut") != std::string::npos
                && (theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(pl)]->useCalibrations())
        {
            for(int p=0; p<4; p++)
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
        else
            continue;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Charge::passCalibrationsCut(int planeID, const Data &data)
{
    if(!(theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->useCalibrations())
        return true;

    //std::stringstream ss;

    //isMinToLimit = true  - isMaxToLimit = true  : min <= parValue <= max
    //isMinToLimit = false - isMaxToLimit = true  : parValue <= max
    //isMinToLimit = true  - isMaxToLimit = false : parValue >= min
    //isMinToLimit = false - isMaxToLimit = false : nothing!

    int    row;
    int    col;
    bool   pass = false;
    double parValue;
    if (data.getClusterSize(planeID) > 4) return false;

    for(int h=0; h<data.getClusterSize(planeID); h++)
    {
        row = data.getClusterPixelRow(h,planeID);
        col = data.getClusterPixelCol(h,planeID);
        for(int p=0; p<4; p++)
        {
            parValue = h2DparsPlots_[p]->GetBinContent(h2DparsPlots_[p]->GetXaxis()->FindBin(col),h2DparsPlots_[p]->GetYaxis()->FindBin(row));
            if(isMinToLimit_[p]==false && isMaxToLimit_[p]==false)
                continue;
            else if(isMinToLimit_[p]==true && isMaxToLimit_[p]== true)
            {
                if(parValue>parMin_[p] && parValue<parMax_[p])
                    pass = true;
            }
            else if(isMinToLimit_[p]==true && isMaxToLimit_[p]==false)
            {
                if(parValue>parMin_[p])
                    pass = true;
            }
            else if(isMinToLimit_[p]==false && isMaxToLimit_[p]==true)
            {
                if(parValue<parMax_[p])
                    pass = true;
            }
        }
    }

    return pass;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Charge::passBadPlanesCut (int planeID, const Data &data)
{
    //    bool badPlanes = theXmlParser_->getAnalysesFromString("Charge")->excludeBadPlanes();
    int badPlanesCut = theXmlParser_->getAnalysesFromString("Charge")->getBadPlanesCut();

    //    if (!badPlanes) return true;

    int maxNumberOfEvents = 0;
    for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes() -2; ++p)// -2 is to exclude DUTs
    {
        HistogramWindow * aWindow = (HistogramWindow*)theAnalysisManager_->getWindowsManager()->getWindow(p);
        if (aWindow->getNumberOfEvents() > maxNumberOfEvents)
        {
            //            maxNumberOfEvents = theAnalysisManager_->getWindowsManager()->getWindow(p)->getNumberOfEvens();
            maxNumberOfEvents = aWindow->getNumberOfEvents();
        }
    }

    int minHits = 7;//To calculate efficiency on the telescope
    int excludeMe = 0;
    if(thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) //Dut case
        minHits = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str());
    else if(data.getHasHit(planeID))//Telescope case
    {
        if(data.getClusterSize(planeID) == 1)
            excludeMe = 1;
        else if(data.getClusterSize(planeID) == 2
                && (data.getClusterPixelRow(0,planeID) == data.getClusterPixelRow(1,planeID)
                    || data.getClusterPixelCol(0,planeID) == data.getClusterPixelCol(1,planeID)))
            excludeMe = 1;
    }

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::meanChargePositionRN (bool pass, int planeID, const Data &data, int threadNumber)
{
    if (!pass || !data.getIsInDetector(planeID))
        return;

    float maxPitchX = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().first).c_str())                                   ;
    float maxPitchY = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(planeID)]->getCellPitches().second).c_str())                                   ;

    if( data.getXPitchLocal(planeID) > maxPitchX || data.getYPitchLocal(planeID) > maxPitchY )
        return;

    const Window* theWindow = theWindowsManager_->getWindow(planeID);
    int           row       = data.getRowPredicted(planeID)         ;
    int           col       = data.getColPredicted(planeID)         ;
    int           run       = data.getRunNumber()                   ;

    if( !theWindow->checkWindow(col,row,run) )
        return;

    int size = data.getClusterSize(planeID);

    if( !data.getHasHit(planeID) || size > 4 )
        return;

    for(int h=0; h<size; h++)
    {
        if(    data.getClusterPixelRow   (h,planeID) == row
               && data.getClusterPixelCol   (h,planeID) == col
               && data.getIsPixelCalibrated (h,planeID)
               && data.getClusterPixelCharge(h,planeID) > standardCutsThreshold_
               && data.getClusterPixelCharge(h,planeID) < standardCutsMaximumCharge_   )
        {
            THREADED(mXYNorm_      [planeID][data.getRunNumber()])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID));
            THREADED(mXYMeanCharge_[planeID][data.getRunNumber()])->Fill(data.getXPixelResidualLocal(planeID),data.getYPixelResidualLocal(planeID),data.getClusterPixelCharge(h,planeID));
            //            if (planeID == 9) ++totEventsControl_;
        }
    }
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
    projYCellChargeSize2AllTracksNorm_.push_back((TH1F*)h2DCellChargeSize2AllTracksNorm_[p]->ProjectionY(hName.str().c_str(), -yCellPitch/2, yCellPitch/2));

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::normalizeEtaDistribution (int p)
{
    //    std::stringstream hName;
    //    std::string planeName = thePlaneMapping_->getPlaneName(p);

    for (int j = 1; j < h2DXCellChargeAsimmetrySizeLE2_[p]->GetYaxis()->GetNbins()+1; ++j)
    {
        for (int i = 1; i < h2DXCellChargeAsimmetrySizeLE2_[p]->GetXaxis()->GetNbins()+1; ++i)
        {
            if (fabs((double)h2DXCellChargeAsimmetrySizeLE2_[p]->GetYaxis()->GetBinCenter(j) -1.) < 0.1 || fabs((double)h2DXCellChargeAsimmetrySizeLE2_[p]->GetYaxis()->GetBinCenter(j) +1.) < 0.1)
                h2DXCellChargeAsimmetrySizeLE2_[p]->SetBinContent(i, j, (double)h1DXCellChargeAsimmetrySizeLE2_[p]->GetBinContent(i, j)/(double)projXCellChargeSize1AllTracksNorm_[p]->GetBinContent(i));
            else
                h2DXCellChargeAsimmetrySizeLE2_[p]->SetBinContent(i, j, (double)h1DXCellChargeAsimmetrySizeLE2_[p]->GetBinContent(i, j)/(double)projXCellChargeSize2AllTracksNorm_[p]->GetBinContent(i));
        }
    }

    for (int j = 1; j < h2DYCellChargeAsimmetrySizeLE2_[p]->GetYaxis()->GetNbins()+1; ++j)
    {
        for (int i = 1; i < h2DYCellChargeAsimmetrySizeLE2_[p]->GetXaxis()->GetNbins()+1; ++i)
        {
            if (fabs((double)h2DYCellChargeAsimmetrySizeLE2_[p]->GetYaxis()->GetBinCenter(j) -1.) < 0.1 || fabs((double)h2DYCellChargeAsimmetrySizeLE2_[p]->GetYaxis()->GetBinCenter(j) +1.) < 0.1)
                h2DYCellChargeAsimmetrySizeLE2_[p]->SetBinContent(i, j, (double)h1DYCellChargeAsimmetrySizeLE2_[p]->GetBinContent(i, j)/(double)projYCellChargeSize1AllTracksNorm_[p]->GetBinContent(i));
            else
                h2DYCellChargeAsimmetrySizeLE2_[p]->SetBinContent(i, j, (double)h1DYCellChargeAsimmetrySizeLE2_[p]->GetBinContent(i, j)/(double)projYCellChargeSize2AllTracksNorm_[p]->GetBinContent(i));
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::normalizeEtaInverse (int p)
{
    int ip;
    double xp;
    /*
    for (int j = 1; j < h2DXcellChargeAsimmetryInv_[p]->GetYaxis()->GetNbins()+1; ++j)
    {
        for (int i = 1; i < h2DXcellChargeAsimmetryInv_[p]->GetXaxis()->GetNbins()+1; ++i)
        {
                if((double)projX_[p]->GetBinContent(j)<=0.) continue;
                h2DXcellChargeAsimmetryInv_[p]->SetBinContent(i, j, (double)h2DXcellChargeAsimmetryInv_[p]->GetBinContent(i, j)/(double)projX_[p]->GetBinContent(j));
        }
    }

    h2DXcellChargeAsimmetryInv_[p]->Scale((double)projX_[p]->GetEntries()/(double)projX_[p]->GetXaxis()->GetNbins());
*/

    for (int j = 1; j < h2DXcellChargeAsimmetryInv_[p]->GetYaxis()->GetNbins()+1; ++j)
    {
        for (int i = 1; i < h2DXcellChargeAsimmetryInv_[p]->GetXaxis()->GetNbins()+1; ++i)
        {
            xp = h2DXcellChargeAsimmetryInv_[p]->GetYaxis()->GetBinCenter(j);
            ip = h1DXallTracks_[p]->GetXaxis()->FindBin(xp);
            if((double)h1DXallTracks_[p]->GetBinContent(ip)<=0.) continue;
            h2DXcellChargeAsimmetryInv_[p]->SetBinContent(i, j, (double)h2DXcellChargeAsimmetryInv_[p]->GetBinContent(i, j)/(double)h1DXallTracks_[p]->GetBinContent(ip));
        }
    }

    h2DXcellChargeAsimmetryInv_[p]->Scale((double)h1DXallTracks_[p]->GetEntries()/(double)h1DXallTracks_[p]->GetXaxis()->GetNbins());

    //-----------------------------------------------------------------------------------------------------------------------------//

    for (int j = 1; j < h2DYcellChargeAsimmetryInv_[p]->GetYaxis()->GetNbins()+1; ++j)
    {
        for (int i = 1; i < h2DYcellChargeAsimmetryInv_[p]->GetXaxis()->GetNbins()+1; ++i)
        {
            xp = h2DYcellChargeAsimmetryInv_[p]->GetYaxis()->GetBinCenter(j);
            ip = h1DYallTracks_[p]->GetXaxis()->FindBin(xp);
            if((double)h1DYallTracks_[p]->GetBinContent(ip)<=0.) continue;
            h2DYcellChargeAsimmetryInv_[p]->SetBinContent(i, j, (double)h2DYcellChargeAsimmetryInv_[p]->GetBinContent(i, j)/(double)h1DYallTracks_[p]->GetBinContent(ip));
        }
    }

    h2DYcellChargeAsimmetryInv_[p]->Scale((double)h1DYallTracks_[p]->GetEntries()/(double)h1DYallTracks_[p]->GetXaxis()->GetNbins());

    //-----------------------------------------------------------------------------------------------------------------------------//

    for (int j = 1; j < h2DXcellChargeAsimmetry_[p]->GetYaxis()->GetNbins()+1; ++j)
    {
        for (int i = 1; i < h2DXcellChargeAsimmetry_[p]->GetXaxis()->GetNbins()+1; ++i)
        {
            if((double)projXCellChargeSize2AllTracksNorm_[p]->GetBinContent(i)<=0.) continue;
            //if(i==1 && p==22) std::cout<<"X "<<(double)h2DXcellChargeAsimmetry_[p]->GetBinContent(1,j);
            h2DXcellChargeAsimmetry_[p]->SetBinContent(i, j, (double)h2DXcellChargeAsimmetry_[p]->GetBinContent(i, j)/(double)projXCellChargeSize2AllTracksNorm_[p]->GetBinContent(i));
            //if(i==1 && p==22) std::cout<<" -> "<<(double)h2DXcellChargeAsimmetry_[p]->GetBinContent(1,j)<<std::endl;
        }
    }

    h2DXcellChargeAsimmetry_[p]->Scale((double)projXCellChargeSize2AllTracksNorm_[p]->GetEntries()/(double)projXCellChargeSize2AllTracksNorm_[p]->GetXaxis()->GetNbins());

    //-----------------------------------------------------------------------------------------------------------------------------//

    for (int j = 1; j < h2DYcellChargeAsimmetry_[p]->GetYaxis()->GetNbins()+1; ++j)
    {
        for (int i = 1; i < h2DYcellChargeAsimmetry_[p]->GetXaxis()->GetNbins()+1; ++i)
        {
            if((double)projYCellChargeSize2AllTracksNorm_[p]->GetBinContent(i)==0) continue;
            h2DYcellChargeAsimmetry_[p]->SetBinContent(i, j, (double)h2DYcellChargeAsimmetry_[p]->GetBinContent(i, j)/(double)projYCellChargeSize2AllTracksNorm_[p]->GetBinContent(i));
        }
    }

    h2DYcellChargeAsimmetry_[p]->Scale((double)projYCellChargeSize2AllTracksNorm_[p]->GetEntries()/(double)projYCellChargeSize2AllTracksNorm_[p]->GetXaxis()->GetNbins());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::calculateEtaDerivative (int p)
{
    for (int l = 1; l < h1DXEtaDerivativeDistribution_[p]->GetXaxis()->GetNbins(); ++l)
    {
        h1DXEtaDerivativeDistribution_[p]->SetBinContent(l, double(-h1DXCellChargeAsimmetrySizeLE2_[p]->GetBinContent(l+1) + h1DXCellChargeAsimmetrySizeLE2_[p]->GetBinContent(l))/h1DXCellChargeAsimmetrySizeLE2_[p]->GetBinWidth(l));
    }
    for (int l = 1; l < h1DYEtaDerivativeDistribution_[p]->GetXaxis()->GetNbins(); ++l)
    {
        h1DYEtaDerivativeDistribution_[p]->SetBinContent(l, double(-h1DYCellChargeAsimmetrySizeLE2_[p]->GetBinContent(l+1) + h1DYCellChargeAsimmetrySizeLE2_[p]->GetBinContent(l))/h1DYCellChargeAsimmetrySizeLE2_[p]->GetBinWidth(l));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::calculateMeanCharge ()
{
    for (unsigned int p = 0; p < thePlaneMapping_->getNumberOfPlanes(); ++p)
    {
        for (std::map<int, TH2F*>::iterator it = mXYMeanCharge_[p].begin(); it != mXYMeanCharge_[p].end(); ++it)
        {

            it->second->Divide(mXYNorm_[p][it->first]);//Sono io lo stronzo

            hXMeanCharge_[p]->Fill(it->first, it->second->GetMean(1));
            hYMeanCharge_[p]->Fill(it->first, it->second->GetMean(2));
        }

        hXMeanCharge_[p]->GetXaxis()->SetTitle("Run number");
        hXMeanCharge_[p]->GetYaxis()->SetTitle("Mean x");
        hYMeanCharge_[p]->GetXaxis()->SetTitle("Run number");
        hYMeanCharge_[p]->GetYaxis()->SetTitle("Mean y");
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Charge::passStandardCuts (int planeID, const Data &data)
{
    if(!theXmlParser_->getAnalysesFromString("Charge")->applyStandardCuts())
        return true;

    if (theXmlParser_->getAnalysesFromString("Charge")->excludeBadPlanes())
        return passBadPlanesCut(planeID, data);

    int minHits = 7;//To calculate efficiency on the telescope
    int excludeMe = 0;
    if(thePlaneMapping_->getPlaneName(planeID).find("Dut") != std::string::npos) //Dut case
        minHits = atoi(theXmlParser_->getAnalysesFromString("Charge")->getMinHits().c_str());
    else if(data.getHasHit(planeID))//Telescope case
    {
        if(data.getClusterSize(planeID) == 1)
            excludeMe = 1;
        else if(data.getClusterSize(planeID) == 2
                && (data.getClusterPixelRow(0,planeID) == data.getClusterPixelRow(1,planeID)
                    || data.getClusterPixelCol(0,planeID) == data.getClusterPixelCol(1,planeID)))
            excludeMe = 1;
    }

    if(data.getNumberOfTelescopeHits()-excludeMe >= minHits) {
        return true;
    }
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::beginJob(void)
{
    standardCutsThreshold_         = theXmlParser_->getAnalysesFromString("Charge")->getThreshold();
    standardCutsMaximumCharge_      = theXmlParser_->getAnalysesFromString("Charge")->getMaxCharge();
    standardCutsMinimumTotalCharge_ = theXmlParser_->getAnalysesFromString("Charge")->getMinTotCharge();
    standardCutsMaximumTotalCharge_ = theXmlParser_->getAnalysesFromString("Charge")->getMaxTotCharge();

    theWindowsManager_      = theAnalysisManager_->getWindowsManager();
    theCalibrationsManager_ = theAnalysisManager_->getCalibrationsManager();

    book();

    setParsLimits();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::analyze(const Data& data, int threadNumber)//WARNING: You can't change this name (threadNumber) or the MACRO THREAD won't compile
{
    if(cutsFormulas_.find("main cut") != cutsFormulas_.end() && !cutsFormulas_["main cut"][threadNumber]->EvalInstance())
        return;



    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
        clusterSize(p,data,threadNumber);

    //    STDLINE("STAMPA 1", ACYellow);


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

    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        if(!passStandardCuts(p,data))
            continue;

        if(thePlaneMapping_->getPlaneName(p).find("Dut") != std::string::npos)
        {
            if(!passCalibrationsCut(p,data))
                return;
        }
        //        STDLINE("STAMPA 2", ACYellow);
        //        std::stringstream ss;
        //        ss << "Plane: " << p << ", row: " << data.getRowPredicted(p) << ", col: " << data.getColPredicted(p);
        //        if (p == 8 || p == 9) STDLINE(ss.str(), ACWhite);

        clusterLandau        (clusterLandauCut,p,data,threadNumber);
        cellLandau           (cellLandauCut   ,p,data,threadNumber);
        cellCharge           (cellChargeCut   ,p,data,threadNumber);
        meanChargePositionRN (cellChargeCut   ,p,data,threadNumber);

        xLandau           (cellChargeXCut  ,p,data,threadNumber);
        xChargeDivision   (cellChargeXCut  ,p,data,threadNumber);
        xAsimmetry        (cellChargeXCut  ,p,data,threadNumber);
        xAsimmetryUnconstr(cellChargeXCut  ,p,data,threadNumber);

        yLandau           (cellChargeYCut  ,p,data,threadNumber);
        yChargeDivision   (cellChargeYCut  ,p,data,threadNumber);
        yAsimmetry        (cellChargeYCut  ,p,data,threadNumber);
        yAsimmetryUnconstr(cellChargeYCut  ,p,data,threadNumber);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charge::endJob(void)
{
    std::stringstream ss;

    for(unsigned int p=0; p<thePlaneMapping_->getNumberOfPlanes(); p++)
    {
        std::string planeName = thePlaneMapping_->getPlaneName(p);

        ADD_THREADED(hClusterSize_                            [p]);
        ADD_THREADED(hClusterSizeStandardCutsThreshold_       [p]);
        ADD_THREADED(hClusterSizeStandardCutsThresholdAndCellLandau_[p]);
        ADD_THREADED(hNumberOfCols_                           [p]);
        ADD_THREADED(hNumberOfRows_                           [p]);
        ADD_THREADED(hClusterSizeDistribution1s_              [p]);
        ADD_THREADED(hClusterSizeDistribution2s_              [p]);
        ADD_THREADED(hClusterSizeNormalization_               [p]);

        ADD_THREADED(hLandauClusterSize1_                     [p]);
        ADD_THREADED(hLandauClusterSize2_                     [p]);
        ADD_THREADED(hLandauClusterSize2sameCol_              [p]);
        ADD_THREADED(hLandauClusterSize2sameRow_              [p]);
        ADD_THREADED(hLandauClusterSize3_                     [p]);
        ADD_THREADED(hLandauClusterSize3sameCol_              [p]);
        ADD_THREADED(hLandauClusterSize3sameRow_              [p]);
        ADD_THREADED(hCellLandau_                             [p]);
        ADD_THREADED(hCellLandau3D_                           [p]);
        ADD_THREADED(hCellLandau3DElectrodes_                 [p]);
        ADD_THREADED(hCellLandauSinglePixel_                  [p]);

        ADD_THREADED(h2DCellCharge_                           [p]);
        ADD_THREADED(h2DCellChargeSecondHit_                  [p]);
        ADD_THREADED(h2DCellChargeNum_                        [p]);
        ADD_THREADED(h2DAllTracks_                            [p]);
        ADD_THREADED(h2DCellChargeNorm_                       [p]);
        ADD_THREADED(h2DCellChargeSize2AllTracksNorm_         [p]);
        ADD_THREADED(h2DCellChargeSize1AllTracksNorm_         [p]);
        ADD_THREADED(h4CellsCharge_                           [p]);
        ADD_THREADED(h4CellsAllTracks_                        [p]);
        ADD_THREADED(h4CellsChargeNorm_                       [p]);
        ADD_THREADED(h4Hits_                                  [p]);
        ADD_THREADED(h4HitsCharge_                            [p]);
        ADD_THREADED(hCutsControl_                            [p]);
        ADD_THREADED(hHitsNotOnRowCol_                        [p]);
        ADD_THREADED(hChargeNotOnRowCol_                      [p]);
        ADD_THREADED(hCellChargeCoarse_                       [p]);
        ADD_THREADED(hCellChargeCoarseNorm_                   [p]);



        ADD_THREADED(h2DCellPixelCharge4Rows_                                   [p]);
        ADD_THREADED(h2DCellClusterCharge4Rows_                                 [p]);
        ADD_THREADED(h2DAllTracks4Rows_                                         [p]);
        ADD_THREADED(h2DCellCharge4RowsOnly50_                                  [p]);
        ADD_THREADED(h2DCellCharge4RowsClusterSize_                             [p]);
        ADD_THREADED(h2DCellCharge4RowsNorm_                                    [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsSameRow_                        [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsSameRowNorm_                    [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsSameCol_                        [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsSameColNorm_                    [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsClusterSize3_                   [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsClusterSize3Norm_               [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowCharge_          [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_      [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighCharge_         [p]);
        ADD_THREADED(h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_     [p]);
        ADD_THREADED(h2DCellPixelCharge4RowsClusterSize1_                       [p]);
        ADD_THREADED(h2DCellPixelCharge4RowsClusterSize2_                       [p]);
        ADD_THREADED(h2DCellPixelCharge4RowsClusterSize3_                       [p]);
        ADD_THREADED(h2DCellPixelCharge4RowsClusterSize1Norm_                       [p]);
        ADD_THREADED(h2DCellPixelCharge4RowsClusterSize2Norm_                       [p]);
        ADD_THREADED(h2DCellPixelCharge4RowsClusterSize3Norm_                       [p]);









        for(std::vector<TH1F*>::iterator it=hCellChargeCoarseLandau_[p].begin(); it!=hCellChargeCoarseLandau_[p].end(); it++)
            ADD_THREADED(*it);

        ADD_THREADED(h1DXcellCharge_                          [p]);
        ADD_THREADED(h1DXcellChargeNormToAll_                 [p]);
        ADD_THREADED(h1DXcellChargeSumLE2_                    [p]);
        ADD_THREADED(h1DXcellChargeSumLE2NormToAll_           [p]);
        ADD_THREADED(h1DXcellChargeSumLE3_                    [p]);
        ADD_THREADED(h1DXcellChargeSumLE3NormToAll_           [p]);
        ADD_THREADED(h1DXallTracks_                           [p]);
        ADD_THREADED(h1DXcellChargeNorm_                      [p]);
        ADD_THREADED(h1DXcellChargeSumLE2Norm_                [p]);
        ADD_THREADED(h1DXcellChargeSumLE3Norm_                [p]);
        ADD_THREADED(h1DXcellSingleHits_                      [p]);
        ADD_THREADED(h1DXcellDoubleHits_                      [p]);
        ADD_THREADED(h1DXcell3Hits_                           [p]);
        ADD_THREADED(h1DXcellChargeSecondHit_                 [p]);
        ADD_THREADED(h1DXcellChargeSecondHitNorm_             [p]);

        ADD_THREADED(h2DXcellCharge_                          [p]);
        ADD_THREADED(h2DXcellChargeSumLE2_                    [p]);
        ADD_THREADED(h2DXcellChargeSumLE3_                    [p]);
        ADD_THREADED(h2DXcellSingleHits_                      [p]);
        ADD_THREADED(h2DXcellDoubleHits_                      [p]);
        ADD_THREADED(hHitsNotONRowColVsXSlope_                [p]);
        ADD_THREADED(hHitsNotOnRowColProjX_                   [p]);

        ADD_THREADED(h1DYcellCharge_                          [p]);
        ADD_THREADED(h1DYcellChargeNormToAll_                 [p]);
        ADD_THREADED(h1DYcellChargeSumLE2_                    [p]);
        ADD_THREADED(h1DYcellChargeSumLE2NormToAll_           [p]);
        ADD_THREADED(h1DYcellChargeSumLE3_                    [p]);
        ADD_THREADED(h1DYcellChargeSumLE3NormToAll_           [p]);
        ADD_THREADED(h1DYallTracks_                           [p]);
        ADD_THREADED(h1DYallTracksNoElectrodes_               [p]);
        ADD_THREADED(h1DYcellChargeNorm_                      [p]);
        ADD_THREADED(h1DYcellChargeSumLE2Norm_                [p]);
        ADD_THREADED(h1DYcellChargeSumLE3Norm_                [p]);
        ADD_THREADED(h1DYcellSingleHits_                      [p]);
        ADD_THREADED(h1DYcellDoubleHits_                      [p]);
        ADD_THREADED(h1DYcell3Hits_                           [p]);
        ADD_THREADED(h1DYcellChargeSecondHit_                 [p]);
        ADD_THREADED(h1DYcellChargeSecondHitNorm_             [p]);

        ADD_THREADED(h2DYcellCharge_                          [p]);
        ADD_THREADED(h2DYcellChargeSumLE2_                    [p]);
        ADD_THREADED(h2DYcellChargeSumLE3_                    [p]);
        ADD_THREADED(h2DYcellSingleHits_                      [p]);
        ADD_THREADED(h2DYcellDoubleHits_                      [p]);
        ADD_THREADED(hHitsNotONRowColVsYSlope_                [p]);
        ADD_THREADED(hHitsNotOnRowColProjY_                   [p]);

        ADD_THREADED(hXAsimmetry_                             [p]);
        ADD_THREADED(hXAsimmetry0_                            [p]);
        ADD_THREADED(h2DXAsimmetryLandau_                     [p]);
        ADD_THREADED(h2DXcellChargeAsimmetry_                 [p]);
        ADD_THREADED(h2DXCellChargeAsimmetryY_                [p]);
        ADD_THREADED(h2DXcellChargeAsimmetryInv_              [p]);
        ADD_THREADED(h2DXcellChargeAsimmetryUnconstrained_    [p]);
        ADD_THREADED(h2DXcellChargeAsimmetryUnconstrainedInv_ [p]);
        ADD_THREADED(h2DXCellChargeAsimmetrySizeLE2_          [p]);
        ADD_THREADED(h2DXCellChargeAsimmetryCell_             [p]);
        ADD_THREADED(h2DXCellChargeAsimmetryCellNorm_         [p]);

        ADD_THREADED(hYAsimmetry_                             [p]);
        ADD_THREADED(hYAsimmetry0_                            [p]);
        ADD_THREADED(h2DYAsimmetryLandau_                     [p]);
        ADD_THREADED(h2DYcellChargeAsimmetry_                 [p]);
        ADD_THREADED(h2DYCellChargeAsimmetryX_                [p]);
        ADD_THREADED(h2DYcellChargeAsimmetryInv_              [p]);
        ADD_THREADED(h2DYcellChargeAsimmetryUnconstrained_    [p]);
        ADD_THREADED(h2DYcellChargeAsimmetryUnconstrainedInv_ [p]);
        ADD_THREADED(h2DYCellChargeAsimmetrySizeLE2_          [p]);
        ADD_THREADED(h2DYCellChargeAsimmetryCell_             [p]);
        ADD_THREADED(h2DYCellChargeAsimmetryCellNorm_         [p]);

        //        ADD_THREADED(mXMeanCharge_                            [p]);
        //        ADD_THREADED(mYMeanCharge_                            [p]);

        for(std::map<int,int>::iterator runIt = runNumberEntries_.begin(); runIt != runNumberEntries_.end(); runIt++)
        {
            ADD_THREADED(mXYMeanCharge_[p][runIt->first]);
            ADD_THREADED(mXYNorm_      [p][runIt->first]);
        }

        h2DCellCharge_                [p]->Divide(h2DCellChargeNorm_       [p]); //to normalize with respect to the events that produced a hit
        //h2DCellCharge_                [p]->Divide(h2DAllTracks_             [p]); //to normalize with respect to all the events


        h2DCellPixelCharge4Rows_                               [p]->Divide(h2DAllTracks4Rows_                                                 [p]);
        h2DCellClusterCharge4Rows_                             [p]->Divide(h2DCellCharge4RowsNorm_                                            [p]);
        h2DCellCharge4RowsOnly50_                              [p]->Divide(h2DCellCharge4RowsNorm_                                            [p]);
        h2DCellCharge4RowsClusterSize_                         [p]->Divide(h2DCellCharge4RowsNorm_                                            [p]);
        h2DCellChargeSecondHit4RowsSameRow_                    [p]->Divide(h2DCellChargeSecondHit4RowsSameRowNorm_                            [p]);
        h2DCellChargeSecondHit4RowsSameCol_                    [p]->Divide(h2DCellChargeSecondHit4RowsSameColNorm_                            [p]);
        h2DCellChargeSecondHit4RowsClusterSize3_               [p]->Divide(h2DCellChargeSecondHit4RowsClusterSize3Norm_                       [p]);
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_      [p]->Divide(h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_              [p]);
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_     [p]->Divide(h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_             [p]);
        h2DCellPixelCharge4RowsClusterSize1_                   [p]->Divide(h2DCellPixelCharge4RowsClusterSize1Norm_                           [p]);
        h2DCellPixelCharge4RowsClusterSize2_                   [p]->Divide(h2DCellPixelCharge4RowsClusterSize2Norm_                           [p]);
        h2DCellPixelCharge4RowsClusterSize3_                   [p]->Divide(h2DCellPixelCharge4RowsClusterSize3Norm_                           [p]);



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
        h1DXcellChargeSecondHit_      [p]->Divide(h1DXallTracks_            [p]);
        hCellChargeCoarse_            [p]->Divide(hCellChargeCoarseNorm_    [p]);

        h1DYcellCharge_               [p]->Divide(h1DYcellChargeNorm_       [p]);
        h1DYcellChargeNormToAll_      [p]->Divide(h1DYallTracks_            [p]);
        h1DYcellChargeSumLE2_         [p]->Divide(h1DYcellChargeSumLE2Norm_ [p]);
        h1DYcellChargeSecondHit_      [p]->Divide(h1DYallTracksNoElectrodes_[p]);
        h1DYcellChargeSumLE2NormToAll_[p]->Divide(h1DYallTracks_            [p]);
        h1DYcellChargeSumLE3_         [p]->Divide(h1DYcellChargeSumLE3Norm_ [p]);
        h1DYcellChargeSumLE3NormToAll_[p]->Divide(h1DYallTracks_            [p]);

        h2DXCellChargeAsimmetryCell_  [p]->Divide(h2DXCellChargeAsimmetryCellNorm_[p]);
        h2DYCellChargeAsimmetryCell_  [p]->Divide(h2DYCellChargeAsimmetryCellNorm_[p]);


        if(theXmlParser_->getAnalysesFromString("Charge")->doFits())
        {
            ss.str(""); ss << "Fitting Landau distributions for plane " << planeName << " ... ";
            STDLINE(ss.str(),ACRed);
            STDLINE("",ACWhite);
            fitCharge(p);
            STDLINE("",ACWhite);
        }
        normalizeEtaDistributionSize2 (p);
        //        NormalizeEtaDistribution      (p);
        normalizeEtaInverse(p);

        setErrorsBar(p);

        calculateEtaDerivative(p);

        float resXRange = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(p)]->getCellPitches().first).c_str());
        float resYRange = atof(((theXmlParser_->getPlanes())[thePlaneMapping_->getPlaneName(p)]->getCellPitches().second).c_str());

        h1DXcellChargeAsimmetry_                 [p]->SetMinimum(-1);
        h1DXcellChargeAsimmetry_                 [p]->SetMaximum( 1);
        h1DXcellChargeAsimmetry_                 [p]->SetMarkerStyle(20);
        h1DXcellChargeAsimmetry_                 [p]->SetMarkerSize(0.6);

        h1DXcellChargeAsimmetryInv_              [p]->SetMinimum(-(resXRange/2));
        h1DXcellChargeAsimmetryInv_              [p]->SetMaximum(resXRange/2);
        h1DXcellChargeAsimmetryInv_              [p]->SetMarkerStyle(20);
        h1DXcellChargeAsimmetryInv_              [p]->SetMarkerSize(0.6);

        h1DYcellChargeAsimmetry_                 [p]->SetMinimum(-1);
        h1DYcellChargeAsimmetry_                 [p]->SetMaximum( 1);
        h1DYcellChargeAsimmetry_                 [p]->SetMarkerStyle(20);
        h1DYcellChargeAsimmetry_                 [p]->SetMarkerSize(0.6);

        h1DYcellChargeAsimmetryInv_              [p]->SetMinimum(-(resYRange/2));
        h1DYcellChargeAsimmetryInv_              [p]->SetMaximum(resYRange/2);
        h1DYcellChargeAsimmetryInv_              [p]->SetMarkerStyle(20);
        h1DYcellChargeAsimmetryInv_              [p]->SetMarkerSize(0.6);

        h1DXcellChargeAsimmetryUnconstrained_    [p]->SetMinimum(-1);
        h1DXcellChargeAsimmetryUnconstrained_    [p]->SetMaximum( 1);
        h1DXcellChargeAsimmetryUnconstrained_    [p]->SetMarkerStyle(20);
        h1DXcellChargeAsimmetryUnconstrained_    [p]->SetMarkerSize(0.6);

        h1DXcellChargeAsimmetryUnconstrainedInv_ [p]->SetMinimum(-(resXRange/2));
        h1DXcellChargeAsimmetryUnconstrainedInv_ [p]->SetMaximum(resXRange/2);
        h1DXcellChargeAsimmetryUnconstrainedInv_ [p]->SetMarkerStyle(20);
        h1DXcellChargeAsimmetryUnconstrainedInv_ [p]->SetMarkerSize(0.6);

        h1DXCellChargeAsimmetrySizeLE2_          [p]->SetMinimum(-1.1);
        h1DXCellChargeAsimmetrySizeLE2_          [p]->SetMaximum( 1.1);
        h1DXCellChargeAsimmetrySizeLE2_          [p]->SetMarkerStyle(20);
        h1DXCellChargeAsimmetrySizeLE2_          [p]->SetMarkerSize(0.6);

        h1DYcellChargeAsimmetryUnconstrained_    [p]->SetMinimum(-1);
        h1DYcellChargeAsimmetryUnconstrained_    [p]->SetMaximum( 1);
        h1DYcellChargeAsimmetryUnconstrained_    [p]->SetMarkerStyle(20);
        h1DYcellChargeAsimmetryUnconstrained_    [p]->SetMarkerSize(0.6);

        h1DYcellChargeAsimmetryUnconstrainedInv_ [p]->SetMinimum(-(resYRange/2));
        h1DYcellChargeAsimmetryUnconstrainedInv_ [p]->SetMaximum(resYRange/2);
        h1DYcellChargeAsimmetryUnconstrainedInv_ [p]->SetMarkerStyle(20);
        h1DYcellChargeAsimmetryUnconstrainedInv_ [p]->SetMarkerSize(0.6);

        h1DYCellChargeAsimmetrySizeLE2_          [p]->SetMinimum(-1.1);
        h1DYCellChargeAsimmetrySizeLE2_          [p]->SetMaximum( 1.1);
        h1DYCellChargeAsimmetrySizeLE2_          [p]->SetMarkerStyle(20);
        h1DYCellChargeAsimmetrySizeLE2_          [p]->SetMarkerSize(0.6);

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

        /*h1DXcellChargeNormToAll_                 [p]->SetMarkerStyle(20);
        h1DXcellChargeNormToAll_                 [p]->SetMarkerSize(0.6);
        h1DXcellChargeNormToAll_                 [p]->SetMarkerColor(kBlack);

        h1DXcellChargeSumLE2NormToAll_           [p]->SetMarkerStyle(20);
        h1DXcellChargeSumLE2NormToAll_           [p]->SetMarkerSize(0.6);
        h1DXcellChargeSumLE2NormToAll_           [p]->SetMarkerColor(kRed);

        h1DXcellChargeSumLE3NormToAll_           [p]->SetMarkerStyle(20);
        h1DXcellChargeSumLE3NormToAll_           [p]->SetMarkerSize(0.6);
        h1DXcellChargeSumLE3NormToAll_           [p]->SetMarkerColor(kMagenta);*/

        hClusterSize_                            [p]->GetXaxis()->SetTitle("cluster size"      );
        hClusterSizeStandardCutsThreshold_       [p]->GetXaxis()->SetTitle("cluster size"      );
        hClusterSizeStandardCutsThresholdAndCellLandau_[p]->GetXaxis()->SetTitle("cluster size"      );
        hNumberOfCols_                           [p]->GetXaxis()->SetTitle("number of columns" );
        hNumberOfRows_                           [p]->GetXaxis()->SetTitle("number of rows"    );
        hClusterSizeDistribution1s_              [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hClusterSizeDistribution1s_              [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hClusterSizeDistribution2s_              [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hClusterSizeDistribution2s_              [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        hClusterSizeNormalization_               [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        hClusterSizeNormalization_               [p]->GetXaxis()->SetTitle("short pitch (um)"  );

        hLandauClusterSize1_                     [p]->GetXaxis()->SetTitle("charge (electrons)");
        hCellLandau_                             [p]->GetXaxis()->SetTitle("charge (electrons)");
        hCellLandau3D_                           [p]->GetXaxis()->SetTitle("charge (electrons)");
        hCellLandau3DElectrodes_                 [p]->GetXaxis()->SetTitle("charge (electrons)");
        hCellLandauSinglePixel_                  [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize2_                     [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize2sameRow_              [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize2sameCol_              [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize3_                     [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize3sameRow_              [p]->GetXaxis()->SetTitle("charge (electrons)");
        hLandauClusterSize3sameCol_              [p]->GetXaxis()->SetTitle("charge (electrons)");

        h2DCellCharge_                           [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellCharge_                           [p]->GetYaxis()->SetTitle("short pitch (um)"  );



        /*----------------------------------- 4 Rows ------------------------------------------------------------*/
        h2DCellPixelCharge4Rows_                                  [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellPixelCharge4Rows_                                  [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellClusterCharge4Rows_                                [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellClusterCharge4Rows_                                [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DAllTracks4Rows_                                        [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DAllTracks4Rows_                                        [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellCharge4RowsOnly50_                                 [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellCharge4RowsOnly50_                                 [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DCellCharge4RowsClusterSize_                            [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DCellCharge4RowsClusterSize_                            [p]->GetYaxis()->SetTitle("short pitch (um)"  );
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
        h2DYcellChargeSumLE2_                    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeSumLE2_                    [p]->GetYaxis()->SetTitle("charge (electrons)");
        h1DYallTracks_                           [p]->GetXaxis()->SetTitle("short pitch (um)"  );
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

        TF1 *lin_X = new TF1("lineX","pol1",-0.7,0.7);

        if(h1DXcellChargeAsimmetryInv_[p]->GetEntries()!=0) h1DXcellChargeAsimmetryInv_[p]->Fit(lin_X,"QR");


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

        hYAsimmetry_                             [p]->GetXaxis()->SetTitle("Asimmetry"         );
        hYAsimmetry0_                            [p]->GetXaxis()->SetTitle("Asimmetry on one side");
        h2DYAsimmetryLandau_                     [p]->GetXaxis()->SetTitle("charge (electrons)");
        h2DYAsimmetryLandau_                     [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetry_                 [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetry_                 [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DYcellChargeAsimmetry_                 [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeAsimmetry_                 [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInv_              [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryInv_              [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeAsimmetryInv_              [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DYcellChargeAsimmetryInv_              [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryUnconstrained_    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYcellChargeAsimmetryUnconstrained_    [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DYcellChargeAsimmetryUnconstrained_    [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeAsimmetryUnconstrained_    [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryUnconstrainedInv_ [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h2DYcellChargeAsimmetryUnconstrainedInv_ [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h1DYcellChargeAsimmetryUnconstrainedInv_ [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DYcellChargeAsimmetryUnconstrainedInv_ [p]->GetYaxis()->SetTitle("short pitch (um)"  );
        h2DYCellChargeAsimmetrySizeLE2_          [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h2DYCellChargeAsimmetrySizeLE2_          [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h1DYCellChargeAsimmetrySizeLE2_          [p]->GetXaxis()->SetTitle("short pitch (um)"  );
        h1DYCellChargeAsimmetrySizeLE2_          [p]->GetYaxis()->SetTitle("Asimmetry"         );
        h2DYCellChargeAsimmetryX_                [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DYCellChargeAsimmetryX_                [p]->GetYaxis()->SetTitle("Y Asimmetry"       );
        h1DYEtaDistribution_                     [p]->GetXaxis()->SetTitle("Asimmetry"         );
        h1DYEtaDerivativeDistribution_           [p]->GetXaxis()->SetTitle("Short pitch (um)"  );
        h1DYEtaDerivativeDistribution_           [p]->GetYaxis()->SetTitle("dEta/dy"           );
        h2DYCellChargeAsimmetryCell_             [p]->GetXaxis()->SetTitle("long pitch (um)"   );
        h2DYCellChargeAsimmetryCell_             [p]->GetYaxis()->SetTitle("short pitch (um)"  );

        TF1 *lin_Y = new TF1("lineY","pol1",-0.7,0.7);

        if(h1DYcellChargeAsimmetryInv_[p]->GetEntries()!=0) h1DYcellChargeAsimmetryInv_[p]->Fit(lin_Y,"QR");


    }

    calculateMeanCharge();
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

        hName  = "hLandauClusterSize2sameCol_"                                + planeName;
        hLandauClusterSize2sameCol_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize2sameCol_[p] << std::endl;

        hName  = "hLandauClusterSize2sameRow_"                                + planeName;
        hLandauClusterSize2sameRow_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize2sameRow_[p] << std::endl;

        hName  = "hLandauClusterSize3_"                                       + planeName;
        hLandauClusterSize3_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize3_[p] << std::endl;

        hName  = "hLandauClusterSize3sameCol_"                                + planeName;
        hLandauClusterSize3sameCol_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize3sameCol_[p] << std::endl;

        hName  = "hLandauClusterSize3sameRow_"                                + planeName;
        hLandauClusterSize3sameRow_.push_back((TH1F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << hLandauClusterSize3sameRow_[p] << std::endl;

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

        /*------------------------------------------------------------------------2D cell charge--------------------------------------------------------------------------------------------------------------*/
        dirName = "Charge/" + planeName + "/2DCellCharge/";
        
        hName  = "h2DallTracks_"+ planeName;
        h2DAllTracks_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DAllTracks_[p] << std::endl;
        
        hName  = "h2DCellChargeNorm_"+ planeName;
        h2DCellChargeNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellChargeNorm_[p] << std::endl;

        hName  = "h2DCellCharge_"+ planeName;
        h2DCellCharge_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge_[p] << std::endl;
        
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

        hName  = "h2DCellCharge4RowsClusterSize_"+ planeName;
        h2DCellCharge4RowsClusterSize_ .push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h2DCellCharge4RowsClusterSize_ [p] << std::endl;

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
        
        hName  = "h4CellsChargeNorm_"+ planeName;
        h4CellsChargeNorm_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h4CellsChargeNorm_[p] << std::endl;
        
        hName  = "h4CellsCharge_"+ planeName;
        h4CellsCharge_.push_back((TH2F*)file->Get((dirName+hName).c_str()));
        std::cout << __PRETTY_FUNCTION__ << dirName+hName << h4CellsCharge_[p] << std::endl;
        
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

        /*----------------------------------------------------------------------------X Asimmetry-----------------------------------------------------------------------------------------------------------*/
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

    float resXRange;
    float resYRange;
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

        resXRange   =   atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().first).c_str());
        resYRange   =   atof(((theXmlParser_->getPlanes())[planeName]->getCellPitches().second).c_str());

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
        hClusterSizeDistribution1s_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "hClusterSizeDistribution2s_"           + planeName;
        hTitle = "Size 2 cluster distribution on pixel " + planeName;
        hClusterSizeDistribution2s_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "hClusterSizeNormalization_"           + planeName;
        hTitle = "Cluster size distribution on pixel normalization " + planeName;
        hClusterSizeNormalization_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DCellCharge4RowsClusterSize_"                         + planeName;
        hTitle = "Cluster size distribution 4 Rows Cell " + planeName;
        h2DCellCharge4RowsClusterSize_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

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

        hName  = "hLandauClusterSize2_"                                       + planeName;
        hTitle = "Charge distribution for 2 adjacent hits "                   + planeName;
        hLandauClusterSize2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize2sameCol_"                                + planeName;
        hTitle = "Charge distribution for 2 ajacent hits on the same column " + planeName;
        hLandauClusterSize2sameCol_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize2sameRow_"                                + planeName;
        hTitle = "Charge distribution for 2 ajacent hits on the same row "    + planeName;
        hLandauClusterSize2sameRow_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize3_"                                       + planeName;
        hTitle = "Charge distribution for 3 adjacent hits "                   + planeName;
        hLandauClusterSize3_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize3sameCol_"                                + planeName;
        hTitle = "Charge distribution for 3 ajacent hits on the same column " + planeName;
        hLandauClusterSize3sameCol_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "hLandauClusterSize3sameRow_"                                + planeName;
        hTitle = "Charge distribution for 3 ajacent hits on the same row "    + planeName;
        hLandauClusterSize3sameRow_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*------------------------------------------------------------------------2D cell charge--------------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("2DCellCharge");

        hName  = "h2DallTracks_"                   + planeName;
        hTitle = "Cell charge normalization "      + planeName;
        h2DAllTracks_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DCellChargeNorm_"              + planeName;
        hTitle = "Cell charge normalization 2"     + planeName;
        h2DCellChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DCellCharge_"                  + planeName;
        hTitle = "Cell charge 2D distribution "    + planeName;
        h2DCellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DCellChargeSecondHit_"                  + planeName;
        hTitle = "Cell charge 2D distribution, second hit "    + planeName;
        h2DCellChargeSecondHit_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DCellChargeNum_"                          + planeName;
        hTitle = "Cell charge 2D distribution not normalized " + planeName;
        h2DCellChargeNum_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DCellChargeSize2Norm_"              + planeName;
        hTitle = "Cell charge normalization for cluster size 2 "     + planeName;
        h2DCellChargeSize2AllTracksNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)resXRange/2 - 0, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, 2*(int)resYRange/2 - 0, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DCellChargeNormSize1_"              + planeName;
        hTitle = "Cell charge normalization for cluster size 1 "     + planeName;
        h2DCellChargeSize1AllTracksNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)resXRange/2 - 0, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, 2*(int)resYRange/2 - 0, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h4CellsAllTracks_"               + planeName;
        hTitle = "4 cells charge normalization "   + planeName;
        h4CellsAllTracks_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "hCellChargeCoarse_"              + planeName;
        hTitle = "Cell charge 2D distribution "    + planeName;
        hCellChargeCoarse_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/(binSize*2.), -(resXRange/2), resXRange/2, (int)resYRange/(binSize*2.), -(resYRange/2), resYRange/2)));

        hName  = "hCellChargeCoarseNorm_"          + planeName;
        hTitle = "Cell charge normalization "      + planeName;
        hCellChargeCoarseNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/(binSize*2.), -(resXRange/2), resXRange/2, (int)resYRange/(binSize*2.), -(resYRange/2), resYRange/2)));

        hName  = "h4CellsChargeNorm_"              + planeName;
        hTitle = "4 cells charge normalization 2 " + planeName;
        h4CellsChargeNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h4CellsCharge_"                  + planeName;
        hTitle = "4 cells charge 2D distribution " + planeName;
        h4CellsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h4HitsCharge_"                   + planeName;
        hTitle = "4 hits charge 2D distribution "  + planeName;
        h4HitsCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/5, -resXRange, resXRange, (int)resYRange/5, -resYRange, resYRange)));

        hName  = "h4Hits_"                         + planeName;
        hTitle = "4 hits 2D distribution "         + planeName;
        h4Hits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/5, -resXRange, resXRange, (int)resYRange/5, -resYRange, resYRange)));

        hName  = "hCutsControl_"                             + planeName;
        hTitle = "Events surviving different level of cuts " + planeName;
        hCutsControl_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 6, 0, 6 )));

        hName  = "hHitsNotOnRowCol_"                  + planeName;
        hTitle = "Distribution of hits with the hit pixel not equal to the predicted "    + planeName;
        hHitsNotOnRowCol_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "hChargeNotOnRowCol_" + planeName;
        hTitle = "Charge distributionn for hits whose tracks don't point on the same pixel " + planeName;
        hChargeNotOnRowCol_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));

        hName  = "h2DCellPixelCharge4Rows_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution" + planeName;
        h2DCellPixelCharge4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellClusterCharge4Rows_"                         + planeName;
        hTitle = "4 Rows cell cluster charge 2D distribution" + planeName;
        h2DCellClusterCharge4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DAllTracks4Rows_"                   + planeName;
        hTitle = "4 Rows Cell charge normalization (All tracks) "      + planeName;
        h2DAllTracks4Rows_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellCharge4RowsOnly50_"                         + planeName;
        hTitle = "Row 50 cell charge 2D distribution" + planeName;
        h2DCellCharge4RowsOnly50_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellCharge4RowsNorm_"                         + planeName;
        hTitle = "4 Rows Cell Charge Normalization  " + planeName;
        h2DCellCharge4RowsNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsSameRow_"                    + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, second hit (Same Row) " + planeName;
        h2DCellChargeSecondHit4RowsSameRow_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsSameRowNorm_"                         + planeName;
        hTitle = "4 Rows Cell Charge Normalization Second Hit (Same Row) " + planeName;
        h2DCellChargeSecondHit4RowsSameRowNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsSameColNorm_"                         + planeName;
        hTitle = "4 Rows Cell Charge Normalization Second Hit (Same Column) " + planeName;
        h2DCellChargeSecondHit4RowsSameColNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsSameCol_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, second hit (Same Column) " + planeName;
        h2DCellChargeSecondHit4RowsSameCol_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsNormClusterSize3_"                         + planeName;
        hTitle = "4 Rows Cell Charge Normalization Second Hit (Cluster Size 3 ) " + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3Norm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, second hit (ClusterSize 3) " + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3LowCharge_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, ClusterSize 3 Low Charge " + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3LowCharge_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, ClusterSize 3 Low Charge Normalization" + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3HighCharge_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, ClusterSize 3: High Charge " + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3HighCharge_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_"                         + planeName;
        hTitle = "4 Rows Cell charge 2D distribution, ClusterSize 3 High Charge Normalization" + planeName;
        h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_ .push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize1_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize1" + planeName;
        h2DCellPixelCharge4RowsClusterSize1_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize2_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize2" + planeName;
        h2DCellPixelCharge4RowsClusterSize2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize3_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize3" + planeName;
        h2DCellPixelCharge4RowsClusterSize3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize1Norm_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize1Norm" + planeName;
        h2DCellPixelCharge4RowsClusterSize1Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize2Norm_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize2Norm" + planeName;
        h2DCellPixelCharge4RowsClusterSize2Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        hName  = "h2DCellPixelCharge4RowsClusterSize3Norm_"                         + planeName;
        hTitle = "4 Rows cell pixel charge 2D distribution ClusterSize3Norm" + planeName;
        h2DCellPixelCharge4RowsClusterSize3Norm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)4*(resYRange/binSize - removedBins), -4*((resYRange/2) + removeHalf), 4*(resYRange/2 - removeHalf))));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*------------------------------------------------------------------------2D cell charge landau--------------------------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("2DCellChargeLandau");

        std::stringstream sName;
        hCellChargeCoarseLandau_.push_back(std::vector<TH1F*>());
        for(int x=0; x<(int)resXRange/(binSize*2.); x++)
            for(int y=0; y<(int)resYRange/(binSize*2.); y++)
            {
                sName.str("");
                sName << -(resXRange/2) + 5 + x*10 << "," << -(resYRange/2) + 5 + y*10;
                hName  = "hCellChargeCoarseLandau_"  + sName.str()  + planeName;
                hTitle = "Cell charge Landau ("      + sName.str()  + ")" + planeName;
                hCellChargeCoarseLandau_[hCellChargeCoarseLandau_.size()-1].push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), 200, -10000, 100000)));
            }


        theAnalysisManager_->cd("Charge/" + planeName);

        /*--------------------------------------------------------------------------2D cell charge - X coordinate---------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("XcellCharge2D");

        hName  = "h2DXcellCharge_"                                      + planeName;
        hTitle = "Predicted cell charge vs. X coordinate "              + planeName;
        h2DXcellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2, 100, -10000, 100000)));

        hName  = "h2DXcellChargeSumLE2_"                                + planeName;
        hTitle = "Up to 2 adjacent hits total charge vs. X coordinate " + planeName;
        h2DXcellChargeSumLE2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2, 100, -10000, 100000)));

        hName  = "h2DXcellChargeSumLE3_"                                + planeName;
        hTitle = "Up to 3 adjacent hits total charge vs. X coordinate " + planeName;
        h2DXcellChargeSumLE3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2, 100, -10000, 100000)));

        hName  = "h2DXcellDoubleHits_"                                  + planeName;
        hTitle = "Double hits distribution vs. X coordinate "           + planeName;
        h2DXcellDoubleHits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DXcellSingleHits_"                                  + planeName;
        hTitle = "Single hits distribution vs. X coordinate "           + planeName;
        h2DXcellSingleHits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "hHitsNotONRowColVsXSlope_" + planeName;
        hTitle = "x coordinate vs x slope for hits whose tracks don't point at the same pixel" + planeName;
        hHitsNotONRowColVsXSlope_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2, 200, -0.000015,  0.000015)));

        hName  = "hHitsNotONRowColProjX_" + planeName;
        hTitle = "x distribution for hits whose tracks don't point at the same pixel" + planeName;
        hHitsNotOnRowColProjX_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*------------------------------------------------------------------------2D cell charge - Y coordinate----------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("YcellCharge2D");

        hName  = "h2DYcellCharge_"                                      + planeName;
        hTitle = "Predicted cell charge vs. Y coordinate "              + planeName;
        h2DYcellCharge_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0., 100, -10000, 100000)));

        hName  = "h2DYcellChargeSumLE2_"                                + planeName;
        hTitle = "Up to 2 adjacent hits total charge vs. Y coordinate " + planeName;
        h2DYcellChargeSumLE2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0., 100, -10000, 100000)));

        hName  = "h2DYcellChargeSumLE3_"                                + planeName;
        hTitle = "Up to 3 adjacent hits total charge vs. Y coordinate " + planeName;
        h2DYcellChargeSumLE3_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0., 100, -10000, 100000)));

        hName  = "h2DYcellDoubleHits_"                                  + planeName;
        hTitle = "Double hits vs. Y coordinate "                        + planeName;
        h2DYcellDoubleHits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0., (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "h2DYcellSingleHits_"                                  + planeName;
        hTitle = "Single hits vs. Y coordinate "                        + planeName;
        h2DYcellSingleHits_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0., (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf)));

        hName  = "hHitsNotONRowColVsYSlope_" + planeName;
        hTitle = "y coordinate vs y slope for hits whose tracks don't point at the same pixel" + planeName;
        hHitsNotONRowColVsYSlope_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/2,-(resYRange/2),resYRange/2, 200, -0.000015,  0.000015)));

        hName  = "hHitsNotONRowColProjY_" + planeName;
        hTitle = "y distribution for hits whose tracks don't point at the same pixel" + planeName;
        hHitsNotOnRowColProjY_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*---------------------------------------------------------------------------1D cell charge - X coordinate--------------------------------------------------------------------- -----------------------*/
        theAnalysisManager_->mkdir("XcellCharge1D");

        hName  = "h1DXcellCharge_"                                                             + planeName;
        hTitle = "Predicted cell charge - X coordinate (normalized to hits) "                  + planeName;
        h1DXcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeNormToAll_"                                                    + planeName;
        hTitle = "Predicted cell charge - X coordinate (normalized to tracks) "                + planeName;
        h1DXcellChargeNormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeSumLE2_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge - X coordinate (normalized to hits) "     + planeName;
        h1DXcellChargeSumLE2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeSumLE2NormToAll_"                                              + planeName;
        hTitle = "Up to 2 adjacent hits total charge - X coordinate (normalized to tracks) "   + planeName;
        h1DXcellChargeSumLE2NormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeSumLE3_"                                                       + planeName;
        hTitle = "Up to 3 adjacent hits total charge - X coordinate (normalized to hits) "     + planeName;
        h1DXcellChargeSumLE3_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeSumLE3NormToAll_"                                              + planeName;
        hTitle = "Up to 3 adjacent hits total charge - X coordinate (normalized to tracks) "   + planeName;
        h1DXcellChargeSumLE3NormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeSecondHit_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge, second hit - X coordinate (normalized to tracks) "     + planeName;
        h1DXcellChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXallTracks_"                                                              + planeName;
        hTitle = "All tracks normalization - X coordinate "                                    + planeName;
        h1DXallTracks_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeNorm_"                                                         + planeName;
        hTitle = "Predicted cell charge - X coordinate - all hits normalization "                + planeName;
        h1DXcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeSumLE2Norm_"                                                   + planeName;
        hTitle = "Up to 2 adjacent hits total charge - X coordinate - all hits normalization " + planeName;
        h1DXcellChargeSumLE2Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeSumLE3Norm_"                                                   + planeName;
        hTitle = "Up to 3 adjacent hits total charge - X coordinate - all hits normalization " + planeName;
        h1DXcellChargeSumLE3Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellSingleHits_"                                                         + planeName;
        hTitle = "Single hits distribution - X coordinate "                                    + planeName;
        h1DXcellSingleHits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellDoubleHits_"                                                         + planeName;
        hTitle = "Double hits distribution - X coordinate "                                    + planeName;
        h1DXcellDoubleHits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcell3Hits_"                                                              + planeName;
        hTitle = "3 adjacent hits distribution - X coordinate "                                + planeName;
        h1DXcell3Hits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        hName  = "h1DXcellChargeSecondHitNorm_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate "     + planeName;
        h1DXcellChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resXRange/2,-(resXRange/2),resXRange/2)));

        theAnalysisManager_->cd("Charge/" + planeName);

        /*---------------------------------------------------------------------------1D cell charge - Y coordinate-------------------------------------------------------------------------------------------*/
        theAnalysisManager_->mkdir("YcellCharge1D");

        hName  = "h1DYcellCharge_"                                                             + planeName;
        hTitle = "Predicted cell charge - Y coordinate (normalized to hits) "                  + planeName;
        h1DYcellCharge_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeNormToAll_"                                                    + planeName;
        hTitle = "Predicted cell charge - Y coordinate (normalized to tracks) "                + planeName;
        h1DYcellChargeNormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeSumLE2_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge - Y coordinate (normalized to hits) "     + planeName;
        h1DYcellChargeSumLE2_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeSumLE2NormToAll_"                                              + planeName;
        hTitle = "Up to 2 adjacent hits total charge - Y coordinate (normalized to tracks) "   + planeName;
        h1DYcellChargeSumLE2NormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeSumLE3_"                                                       + planeName;
        hTitle = "Up to 3 adjacent hits total charge - Y coordinate (normalized to hits) "     + planeName;
        h1DYcellChargeSumLE3_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeSumLE3NormToAll_"                                              + planeName;
        hTitle = "Up to 3 adjacent hits total charge - Y coordinate (normalized to tracks)"    + planeName;
        h1DYcellChargeSumLE3NormToAll_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeSecondHit_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge, second hit - Y coordinate (normalized to tracks) "     + planeName;
        h1DYcellChargeSecondHit_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) +1,resYRange/2 - 0)));

        hName  = "h1DYallTracks_"                                                              + planeName;
        hTitle = "All tracks normalization - Y coordinate "                                    + planeName;
        h1DYallTracks_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYallTracksNoElectrodes_"                                                              + planeName;
        hTitle = "All tracks normalization - Y coordinate "                                    + planeName;
        h1DYallTracksNoElectrodes_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeNorm_"                                                         + planeName;
        hTitle = "Predicted cell charge - Y coordinate - all hits normalization "              + planeName;
        h1DYcellChargeNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeSumLE2Norm_"                                                   + planeName;
        hTitle = "Up to 2 adjacent hits total charge - Y coordinate - all hits normalization " + planeName;
        h1DYcellChargeSumLE2Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeSumLE3Norm_"                                                   + planeName;
        hTitle = "Up to 3 adjacent hits total charge - Y coordinate - all hits normalization " + planeName;
        h1DYcellChargeSumLE3Norm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellSingleHits_"                                                         + planeName;
        hTitle = "Single hits distribution - Y coordinate "                                    + planeName;
        h1DYcellSingleHits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellDoubleHits_"                                                         + planeName;
        hTitle = "Double hits distribution - Y coordinate "                                    + planeName;
        h1DYcellDoubleHits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcell3Hits_"                                                              + planeName;
        hTitle = "3 adjacent hits distribution - Y coordinate "                                + planeName;
        h1DYcell3Hits_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2 - 0,-(resYRange/2) + 1.,resYRange/2 - 0.)));

        hName  = "h1DYcellChargeSecondHitNorm_"                                                       + planeName;
        hTitle = "Up to 2 adjacent hits total charge, second hit distribution - X coordinate "     + planeName;
        h1DYcellChargeSecondHitNorm_.push_back(NEW_THREADED(TH1F(hName.c_str(), hTitle.c_str(), (int)resYRange/2,-(resYRange/2),resYRange/2)));

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

        hName  = "h2DXcellChargeAsimmetry_"                                         + planeName;
        hTitle = "L/R charge asimmetry - X coordinate "                             + planeName;
        h2DXcellChargeAsimmetry_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)resXRange/2, -(resXRange/2), resXRange/2, 102, -1.1, 1.1)));

        hName  = "h2DXcellChargeAsimmetryInv_"                                      + planeName;
        hTitle = "L/R charge asimmetry - X coordinate "                             + planeName;
        h2DXcellChargeAsimmetryInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, 2*(int)resXRange/2, -(resXRange/2), resXRange/2)));

        hName  = "h2DXcellChargeAsimmetryUnconstrained_"                            + planeName;
        hTitle = "L/R unconstrained charge asimmetry - X coordinate "               + planeName;
        h2DXcellChargeAsimmetryUnconstrained_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/2, -(resXRange/2), resXRange/2, 100, -1, 1)));

        hName  = "h2DXcellChargeAsimmetryUnconstrainedInv_"                         + planeName;
        hTitle = "L/R unconstrained charge asimmetry - X coordinate "               + planeName;
        h2DXcellChargeAsimmetryUnconstrainedInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 20, -1, 1, (int)resXRange/2, -(resXRange/2), resXRange/2)));

        hName  = "h2DXcellChargeAsimmetrySizeLE2_"                                  + planeName;
        hTitle = "L/R charge asimmetry for cluster size <= 2 "                      + planeName;
        h2DXCellChargeAsimmetrySizeLE2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)resXRange/2 - 0, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, 102, -1.1, 1.1)));

        hName  = "h2DXCellChargeAsimmetryCell_"                                      + planeName;
        hTitle = "Distribution of mean x asymmetry on cell "                             + planeName;
        h2DXCellChargeAsimmetryCell_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/5, -(resXRange/2), resXRange/2, (int)resYRange/5, -(resYRange/2), resYRange/2)));

        hName  = "h2DXCellChargeAsimmetryCellNorm_"                                      + planeName;
        hTitle = "Distribution of mean x asymmetry on cell, normalization "                             + planeName;
        h2DXCellChargeAsimmetryCellNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/5, -(resXRange/2), resXRange/2, (int)resYRange/5, -(resYRange/2), resYRange/2)));

        hName  = "h2DXCellChargeAsimmetryY_"                                         + planeName;
        hTitle = "L/R charge asimmetry - X coordinate - on Y "                             + planeName;
        h2DXCellChargeAsimmetryY_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resYRange/5, -(resYRange/2), resYRange/2, 102, -1.1, 1.1)));

        hName = "h1DXEtaDerivativeDistribution_" + planeName;
        hTitle = "Distribution of eta derivative function for coordinate x, " + planeName;
        h1DXEtaDerivativeDistribution_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), 2*(int)resXRange/2 - 0, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf));

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

        hName  = "h2DYcellChargeAsimmetry_"                                         + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate "                             + planeName;
        h2DYcellChargeAsimmetry_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)resYRange/2 - 0, -(resYRange/2) + 1, resYRange/2 - 0, 102, -1.1, 1.1)));

        hName  = "h2DYcellChargeAsimmetryInv_"                                      + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate "                             + planeName;
        h2DYcellChargeAsimmetryInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 102, -1.1, 1.1, 2*(int)resYRange/2 - 0, -(resYRange/2) + 1, resYRange/2 - 0)));

        hName  = "h2DYcellChargeAsimmetryUnconstrained_"                            + planeName;
        hTitle = "L/R unconstrained charge asimmetry - Y coordinate "               + planeName;
        h2DYcellChargeAsimmetryUnconstrained_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(),(int)resYRange/2 - 0, -(resYRange/2) + 1, resYRange/2 - 0, 100, -1, 1)));

        hName  = "h2DYcellChargeAsimmetryUnconstrainedInv_"                         + planeName;
        hTitle = "L/R unconstrained charge asimmetry - Y coordinate "               + planeName;
        h2DYcellChargeAsimmetryUnconstrainedInv_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 20, -1, 1, (int)resYRange/2 - 0, -(resYRange/2) + 1, resYRange/2 - 0)));

        hName  = "h2DYcellChargeAsimmetrySizeLE2_"                                  + planeName;
        hTitle = "L/R charge asimmetry for cluster size <= 2 "                      + planeName;
        h2DYCellChargeAsimmetrySizeLE2_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), 2*(int)resYRange/2 - 0, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf, 102, -1.1, 1.1)));

        hName  = "h2DYCellChargeAsimmetryCell_"                                      + planeName;
        hTitle = "Distribution of mean y asymmetry on cell "                             + planeName;
        h2DYCellChargeAsimmetryCell_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/5, -(resXRange/2), resXRange/2, (int)resYRange/5, -(resYRange/2), resYRange/2)));

        hName  = "h2DYCellChargeAsimmetryCellNorm_"                                      + planeName;
        hTitle = "Distribution of mean y asymmetry on cell, normalization "                             + planeName;
        h2DYCellChargeAsimmetryCellNorm_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/5, -(resXRange/2), resXRange/2, (int)resYRange/5, -(resYRange/2), resYRange/2)));

        hName  = "h2DYCellChargeAsimmetryX_"                                         + planeName;
        hTitle = "L/R charge asimmetry - Y coordinate - on X "                             + planeName;
        h2DYCellChargeAsimmetryX_.push_back(NEW_THREADED(TH2F(hName.c_str(), hTitle.c_str(), (int)resXRange/5, -(resXRange/2), resXRange/2, 102, -1.1, 1.1)));

        hName = "h1DYEtaDerivativeDistribution_" + planeName;
        hTitle = "Distribution of eta derivative function for coordinate y, " + planeName;
        h1DYEtaDerivativeDistribution_.push_back(new TH1F (hName.c_str(), hTitle.c_str(), 2*(int)resYRange/2 - 0, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf));

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
            aMap[runIt->first] = NEW_THREADED(TH2F (hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf));
            hName = "ChargeNormalization_run" + sss.str() + "_" + planeName;
            hTitle = "Normalization histogram, run number #" + sss.str() + ", " + planeName;
            aMapNorm[runIt->first] = NEW_THREADED(TH2F (hName.c_str(), hTitle.c_str(), (int)resXRange/binSize - removedBins, -(resXRange/2) + removeHalf, resXRange/2 - removeHalf, (int)resYRange/binSize - removedBins, -(resYRange/2) + removeHalf, resYRange/2 - removeHalf));

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

