#ifndef CHARGE_H
#define CHARGE_H

#include "Analysis.h"

#include <TTreeFormula.h>
#include <TMap.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1D.h>

#include <vector>
#include <iostream>


class TH1F               ;
class TH2F               ;
class TH1                ;
class TF1                ;
class TTreeFormula       ;
class TFile              ;
class AnalysisManager    ;
class WindowsManager     ;
class CalibrationsManager;
class PlanesMapping      ;
class XmlParser          ;

class Charge : public Analysis
{
public:
    Charge  (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
    ~Charge (void                                                    );

    void beginJob            (void                                                                );
    void analyze             (const Data&                       data    , int threadNumber        );
    void endJob              (void                                                                );
    void setCutsFormula      (std::map<std::string,std::string> cutsList, std::vector<TTree*> tree);
    bool passStandardCuts    (int planeID                               , const Data& data        );
    bool passBadPlanesCut    (int  planeID, const Data& data                            );

    void getInFile           (TFile *                                                   ){}

    void load                (TFile* file                                               );
    void fitCharge           (int  planeID                                              );
    void fitChargeCorrelations4Rows(int  planeID                                        );
    void setInvincible       (bool cannotBeKilled                                       );

private:
    void book                (void                                                      );
    void destroy             (void                                                      );

    void clusterSize         (           int planeID, const Data& data, int threadNumber);
    void clusterLandau       (bool pass, int planeID, const Data& data, int threadNumber);
    void cellLandau          (bool pass, int planeID, const Data& data, int threadNumber);
    void cellCharge          (bool pass, int planeID, const Data& data, int threadNumber);
    void xLandau             (bool pass, int planeID, const Data& data, int threadNumber);
    void xChargeDivision     (bool pass, int planeID, const Data& data, int threadNumber);
    void xAsimmetry          (bool pass, int planeID, const Data& data, int threadNumber);
    void xAsimmetryUnconstr  (bool pass, int planeID, const Data& data, int threadNumber);
    void yLandau             (bool pass, int planeID, const Data& data, int threadNumber);
    void yChargeDivision     (bool pass, int planeID, const Data& data, int threadNumber);
    void yAsimmetry          (bool pass, int planeID, const Data& data, int threadNumber);
    void yAsimmetryUnconstr  (bool pass, int planeID, const Data& data, int threadNumber);
    void meanChargePositionRN(bool pass, int planeID, const Data& data, int threadNumber);
    void graphChargeClusterSize4(        int planeID, const Data& data                  );


    void normalizeEtaDistributionSize2 (int p);
    void normalizeEtaDistribution      (int p);
    void normalizeEtaInverse           (int p);
    void calculateEtaDerivative        (int p);
    void calculateMeanCharge           (     );

    void setParsLimits       (void                                                      );
    bool passCalibrationsCut (int  planeID, const Data& data                            );
    void setErrorsBar        (int  planeID                                              );
    int  langausFit          (TH1* histo  , double* fitParameters                       );
    int  linearFit           (TH1* histo  , double* fitParameters                       );

    PlanesMapping*                                     thePlaneMapping_       ;
    std::map< std::string,std::vector<TTreeFormula*> > cutsFormulas_          ;
    TF1*                                               langaus_               ;
    TF1*                                               linear_                ;
    const WindowsManager*                              theWindowsManager_     ;
    CalibrationsManager*                               theCalibrationsManager_;
    XmlParser*                                         theXmlParser_          ;

    double parMin_      [4]  ;
    double parMax_      [4]  ;
    bool   isMinToLimit_[4]  ;
    bool   isMaxToLimit_[4]  ;

    int    standardCutsPixelMinimumCharge_  ;
    int    standardCutsPixelMaximumCharge_  ;
    int    standardCutsClusterMinimumCharge_;
    int    standardCutsClusterMaximumCharge_;
    TH2F*  h2DparsPlots_[4]  ;
    bool   cannotBeDestroyed_;

    /*--------------------------------cluster size----------------*/
    std::vector<TH1F*>    hClusterSize_                            ;
    std::vector<TH1F*>    hClusterSizeStandardCutsThreshold_       ;
    std::vector<TH1F*>    hClusterSizeStandardCutsThresholdAndCellLandau_;
    std::vector<TH1F*>    hNumberOfCols_                           ;
    std::vector<TH1F*>    hNumberOfRows_                           ;
    std::vector<TH2F*>    h2DClusterSize_                          ;
    std::vector<TH1D*>    h1DClusterSizeYProjection_               ;
    std::vector<TH2F*>    hClusterSizeDistribution1s_              ;
    std::vector<TH2F*>    hClusterSizeDistribution2s_              ;
    std::vector<TH2F*>    hClusterSizeNormalization_               ;


    /*--------------------------Landau distributions -------------*/
    std::vector<TH1F*>    hLandauClusterSize1_                     ;
    std::vector<TH1F*>    hLandauClusterSize2_                     ;
    std::vector<TH1F*>    hLandauClusterSize2sameCol_              ;
    std::vector<TH1F*>    hLandauClusterSize2sameRow_              ;
    std::vector<TH1F*>    hLandauClusterSize2ChargeOver5000_       ;
    std::vector<TH1F*>    hLandauClusterSize3_                     ;
    std::vector<TH1F*>    hLandauClusterSize3ChargeOver5000_       ;
    std::vector<TH1F*>    hLandauClusterSize3sameCol_              ;
    std::vector<TH1F*>    hLandauClusterSize3sameRow_              ;
    std::vector<TH1F*>    hLandauClusterSize4_                     ;
    std::vector<TH1F*>    hCellLandau_                             ;
    std::vector<TH1F*>    hCellLandau3D_                           ;
    std::vector<TH1F*>    hCellLandau3DElectrodes_                 ;
    std::vector<TH1F*>    hCellLandauSinglePixel_                  ;

    /*--------------------------2D cell charge--------------------*/
    std::vector<TH2F*>    h2DCellCharge_                           ;
    std::vector<TH2F*>    h2DCellChargeSecondHit_                  ;
    std::vector<TH2F*>    h2DCellChargeNum_                        ;
    std::vector<TH2F*>    h2DCellClusterCharge_                    ;
    std::vector<TH1D*>    h1DClusterChargeYProfile_                ;
    std::vector<TH2F*>    h2DAllTracks_                            ;
    std::vector<TH2F*>    h2DCellChargeNorm_                       ;
    std::vector<TH2F*>    h4CellsCharge_                           ;
    std::vector<TH2F*>    h4CellsAllTracks_                        ;
    std::vector<TH2F*>    h4CellsChargeNorm_                       ;
    std::vector<TH2F*>    h4Hits_                                  ;
    std::vector<TH2F*>    h4HitsCharge_                            ;
    std::vector<TH2F*>    h2DCellChargeSize1AllTracksNorm_         ;
    std::vector<TH2F*>    h2DCellChargeSize2AllTracksNorm_         ;
    std::vector<TH2F*>    hCellChargeCoarse_                       ;
    std::vector<TH2F*>    hCellChargeCoarseNorm_                   ;
    std::vector<TH2F*>    hHitsNotOnRowCol_                        ;
    std::vector<TH1F*>    hCutsControl_                            ;
    std::vector<TH1F*>    hChargeNotOnRowCol_                      ;
    std::vector<TH1F*>    h1DPixelYTrackResiduals_                 ;
    std::vector<TH1F*>    h1DPixelYTrackResidualsNorm_             ;

    /*------------2D cell charge Landau-------------*/
    std::vector<std::vector<TH1F*> > hCellChargeCoarseLandau_;


    /*------------2D cell charge - X coordinate--------------------*/
    std::vector<TH2F*>    h2DXcellCharge_                          ;
    std::vector<TH2F*>    h2DXcellChargeSumLE2_                    ;
    std::vector<TH2F*>    h2DXcellChargeSumLE3_                    ;
    std::vector<TH2F*>    h2DXcellDoubleHits_                      ;
    std::vector<TH2F*>    h2DXcellSingleHits_                      ;
    std::vector<TH2F*>    hHitsNotONRowColVsXSlope_                ;
    std::vector<TH1F*>    hHitsNotONRowColVsXSlopeProfileX_        ;
    std::vector<TH1F*>    hHitsNotONRowColVsXSlopeProfileY_        ;

    /*------------------------2D cell charge - Y coordinate------*/
    std::vector<TH2F*>    h2DYcellCharge_                          ;
    std::vector<TH2F*>    h2DYcellChargeSum_                       ;
    std::vector<TH2F*>    h2DYcellChargeSumLE2_                    ;
    std::vector<TH2F*>    h2DYcellChargeSumLE3_                    ;
    std::vector<TH2F*>    h2DYcellDoubleHits_                      ;
    std::vector<TH2F*>    h2DYcellSingleHits_                      ;
    std::vector<TH2F*>    hHitsNotONRowColVsYSlope_                ;
    std::vector<TH1F*>    hHitsNotONRowColVsYSlopePh1DXCellChargeAsimmetryrofileX_        ;
    std::vector<TH1F*>    hHitsNotONRowColVsYSlopeProfileY_        ;

    /*-------------------------1D cell charge - X coordinate------*/
    std::vector<TH1F*>    h1DXcellCharge_                          ;
    std::vector<TH1F*>    h1DXcellChargeNormToAll_                 ;
    std::vector<TH1F*>    h1DXcellChargeSumLE2_                    ;
    std::vector<TH1F*>    h1DXcellChargeSumLE2NormToAll_           ;
    std::vector<TH1F*>    h1DXcellChargeSumLE3_                    ;
    std::vector<TH1F*>    h1DXcellChargeSumLE3NormToAll_           ;
    std::vector<TH1F*>    h1DXcellChargeSecondHit_                 ;
    std::vector<TH1F*>    h1DXcellChargeSecondHitNorm_             ;
    std::vector<TH1F*>    h1DXallTracks_                           ;
    std::vector<TH1F*>    h1DXcellChargeNorm_                      ;
    std::vector<TH1F*>    h1DXcellChargeSumLE2Norm_                ;
    std::vector<TH1F*>    h1DXcellChargeSumLE3Norm_                ;
    std::vector<TH1F*>    h1DXcellSingleHits_                      ;
    std::vector<TH1F*>    h1DXcellDoubleHits_                      ;
    std::vector<TH1F*>    h1DXcell3Hits_                           ;
    std::vector<TH1F*>    projXCellChargeSize2AllTracksNorm_       ;
    std::vector<TH1F*>    projXCellChargeSize1AllTracksNorm_       ;
    std::vector<TH1F*>    projXUp_                                 ;
    std::vector<TH1F*>    projXSize1Up_                            ;
    std::vector<TH1F*>    projXDown_                               ;
    std::vector<TH1F*>    projXSize1Down_                          ;
    std::vector<TH1F*>    hHitsNotOnRowColProjX_                   ;


/*-------------------------1D cell charge - Y coordinate------*/
    std::vector<TH1F*>    h1DYcellCharge_                          ;
    std::vector<TH1F*>    h1DYcellChargeNormToAll_                 ;
    std::vector<TH1F*>    h1DYcellChargeSumLE2_                    ;
    std::vector<TH1F*>    h1DYcellChargeSumLE2NormToAll_           ;
    std::vector<TH1F*>    h1DYcellChargeSumLE3_                    ;
    std::vector<TH1F*>    h1DYcellChargeSumLE3NormToAll_           ;
    std::vector<TH1F*>    h1DYcellChargeSecondHit_                 ;
    std::vector<TH1F*>    h1DYcellChargeSecondHitNorm_             ;
    std::vector<TH1F*>    h1DYallTracks_                           ;
    std::vector<TH1F*>    h1DYallTracksNoElectrodes_               ;
    std::vector<TH1F*>    h1DYcellChargeNorm_                      ;
    std::vector<TH1F*>    h1DYcellChargeSumLE2Norm_                ;
    std::vector<TH1F*>    h1DYcellChargeSumLE3Norm_                ;
    std::vector<TH1F*>    h1DYcellSingleHits_                      ;
    std::vector<TH1F*>    h1DYcellDoubleHits_                      ;
    std::vector<TH1F*>    h1DYcell3Hits_                           ;
    std::vector<TH1F*>    projYCellChargeSize2AllTracksNorm_       ;
    std::vector<TH1F*>    projYCellChargeSize1AllTracksNorm_       ;
    std::vector<TH1F*>    projYUp_                                 ;
    std::vector<TH1F*>    projYSize1Up_                            ;
    std::vector<TH1F*>    projYDown_                               ;
    std::vector<TH1F*>    projYSize1Down_                          ;
    std::vector<TH1F*>    hHitsNotOnRowColProjY_                   ;
    std::vector<TH1F*>    h1DYallTracksSize2_                      ;


    /*------------------------- 4 Rows cell charge  --------------------*/
    std::vector<TH2F*>    h2DCellPixelCharge4Rows_                                 ;
    std::vector<TH2F*>    h2DClusterCharge4RowsClusterSize2_                       ;
    std::vector<TH2F*>    h2DClusterCharge4RowsClusterSize3_                       ;
    std::vector<TH2F*>    h2DClusterCharge4RowsClusterSize4_                       ;
    std::vector<TH2F*>    h2DCellClusterCharge4Rows_                               ;
    std::vector<TH2F*>    h2DClusterSize4Rows_                                     ;
    std::vector<TH2F*>    h2DAllTracks4Rows_                                       ;
    std::vector<TH2F*>    h2DCellCharge4RowsOnly50_                                ;
    std::vector<TH2F*>    h2DCellCharge4RowsNorm_                                  ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsSameRow_                      ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsSameRowNorm_                  ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsSameCol_                      ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsSameColNorm_                  ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsClusterSize3_                 ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsClusterSize3Norm_             ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsClusterSize3LowCharge_        ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsClusterSize3LowChargeNorm_    ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsClusterSize3HighCharge_       ;
    std::vector<TH2F*>    h2DCellChargeSecondHit4RowsClusterSize3HighChargeNorm_   ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsClusterSize1_                     ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsExcept50ClusterSize1_             ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsClusterSize2_                     ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsClusterSize3_                     ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsClusterSize4_                     ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsClusterSize1Norm_                 ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsExcept50ClusterSize1Norm_         ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsClusterSize2Norm_                 ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsClusterSize3Norm_                 ;
    std::vector<TH2F*>    h2DCellPixelCharge4RowsClusterSize4Norm_                 ;
    std::vector<TH1D*>    h1DClusterSize4RowsYProjection_                          ;
    std::vector<TH1D*>    h1DClusterCharge4RowsYProjection_                        ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize4Row1vsRow2Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize4Row2vsRow3Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize4Row4vsRow3Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize4ExternalRows1And4vs2And3_ ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize4InternalRows1And4vs2And3_ ;
    std::vector<TH1F*>    h1DPixelCharge4RowsClusterSize4ExternalProfile_          ;
    std::vector<TH1F*>    h1DPixelCharge4RowsClusterSize4InternalProfile_          ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize3Row1vsRow2Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize3Row2vsRow3Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize3Row4vsRow3Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize3ExternalRows1And4vs2And3_ ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize3InternalRows1And4vs2And3_ ;
    std::vector<TH1F*>    h1DPixelCharge4RowsClusterSize3ExternalProfile_          ;
    std::vector<TH1F*>    h1DPixelCharge4RowsClusterSize3InternalProfile_          ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize2Row1vsRow2Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize2Row2vsRow3Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize2Row4vsRow3Of4Rows_             ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2BetweenRow1vsRow2_        ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2From1BetweenRow1vsRow2_   ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2From2BetweenRow1vsRow2_   ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2BetweenRow1vsRows1And2_   ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2BetweenRow2vsRows1And2_   ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2BetweenRow4vsRow3_        ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2From4BetweenRow4vsRow3_   ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2From3BetweenRow4vsRow3_   ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2BetweenRow4vsRows4And3_   ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2BetweenRow3vsRows4And3_   ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2ExternalRows1And4vs2And3_ ;
    std::vector<TH2F*>    h2DPixelCharge4RowsClusterSize2InternalRows1And4vs2And3_ ;
    std::vector<TH1F*>    h1DPixelCharge4RowsClusterSize2ExternalProfile_          ;
    std::vector<TH1F*>    h1DPixelCharge4RowsClusterSize2InternalProfile_          ;
    std::vector<TH1F*>    h1DPixelYTrackResiduals4Rows_                            ;
    std::vector<TH1F*>    h1DPixelYTrackResiduals4RowsNorm_                        ;

    /*------------------------- 2 Rows cell charge  --------------------*/
    std::vector<TH2F*>    h2DCellPixelCharge2Rows_                                 ;
    std::vector<TH2F*>    h2DCellClusterCharge2Rows_                               ;
    std::vector<TH2F*>    h2DAllTracks2Rows_                                       ;
    std::vector<TH2F*>    h2DCellCharge2RowsOnly50_                                ;
    std::vector<TH2F*>    h2DClusterSize2Rows_                                     ;
    std::vector<TH2F*>    h2DCellCharge2RowsNorm_                                  ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsSameRow_                      ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsSameRowNorm_                  ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsSameCol_                      ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsSameColNorm_                  ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsClusterSize3_                 ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsClusterSize3Norm_             ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsClusterSize3LowCharge_        ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsClusterSize3LowChargeNorm_    ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsClusterSize3HighCharge_       ;
    std::vector<TH2F*>    h2DCellChargeSecondHit2RowsClusterSize3HighChargeNorm_   ;
    std::vector<TH2F*>    h2DCellPixelCharge2RowsClusterSize1_                     ;
    std::vector<TH2F*>    h2DCellPixelCharge2RowsClusterSize2_                     ;
    std::vector<TH2F*>    h2DCellPixelCharge2RowsClusterSize3_                     ;
    std::vector<TH2F*>    h2DCellPixelCharge2RowsClusterSize4_                     ;
    std::vector<TH2F*>    h2DCellPixelCharge2RowsClusterSize1Norm_                 ;
    std::vector<TH2F*>    h2DCellPixelCharge2RowsClusterSize2Norm_                 ;
    std::vector<TH2F*>    h2DCellPixelCharge2RowsClusterSize3Norm_                 ;
    std::vector<TH2F*>    h2DCellPixelCharge2RowsClusterSize4Norm_                 ;
    std::vector<TH1D*>    h1DClusterSize2RowsYProjection_                          ;
    std::vector<TH1D*>    h1DClusterCharge2RowsYProfile_                           ;
    std::vector<TH1F*>    h1DPixelYTrackResiduals2Rows_                            ;
    std::vector<TH1F*>    h1DPixelYTRackResiduals2RowsNorm_                        ;
    std::vector<TH2F*>    h2DPixelChargeClusterSize2Row1vsRow2Of2Rows_             ;


    /*-------------------------X Asimmetry------------------------*/
    std::vector<TH1F*>    hXAsimmetry_                             ;
    std::vector<TH1F*>    hXAsimmetry0_                            ;
    std::vector<TH2F*>    h2DXAsimmetryLandau_                     ;
    std::vector<TH2F*>    h2DXcellChargeAsimmetry_                 ;
    std::vector<TH2F*>    h2DXcellChargeAsimmetryInv_              ;
    std::vector<TH2F*>    h2DXcellChargeAsimmetryUnconstrained_    ;
    std::vector<TH2F*>    h2DXcellChargeAsimmetryUnconstrainedInv_ ;
    std::vector<TH2F*>    h2DXCellChargeAsimmetrySizeLE2_          ;
    std::vector<TH2F*>    h2DXCellChargeAsimmetryY_                ;
    std::vector<TH2F*>    h2DXCellChargeAsimmetryCell_             ;
    std::vector<TH2F*>    h2DXCellChargeAsimmetryCellNorm_         ;
    std::vector<TH1F*>    h1DXcellChargeAsimmetry_                 ;
    std::vector<TH1F*>    h1DXcellChargeAsimmetryInv_              ;
    std::vector<TH1F*>    h1DXcellChargeAsimmetryUnconstrained_    ;
    std::vector<TH1F*>    h1DXcellChargeAsimmetryUnconstrainedInv_ ;
    std::vector<TH1F*>    h1DXCellChargeAsimmetrySizeLE2_          ;
    std::vector<TH1F*>    h1DXEtaDistribution_                     ;
    std::vector<TH1F*>    h1DXEtaDerivativeDistribution_           ;

    /*-------------------------Y Asimmetry------------------------*/
    std::vector<TH1F*>    hYAsimmetry_                                           ;
    std::vector<TH1F*>    hYAsimmetry0_                                          ;
    std::vector<TH2F*>    h2DYAsimmetryLandau_                                   ;
    std::vector<TH2F*>    h2DYCellChargeAsimmetry_                               ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInv_                            ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInvCutOnEntries_                ; //introduced in order to have a better TProfile
    std::vector<TH2F*>    h2DYcellChargeAsimmetryUnconstrained_                  ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryUnconstrainedInv_               ;
    std::vector<TH2F*>    h2DYCellChargeAsimmetrySizeLE2_                        ;
    std::vector<TH2F*>    h2DYCellChargeAsimmetryCell_                           ;
    std::vector<TH2F*>    h2DYCellChargeAsimmetryCellNorm_                       ;
    std::vector<TH2F*>    h2DYCellChargeAsimmetryX_                              ;
    std::vector<TH1F*>    h1DYcellChargeAsimmetry_                               ;
    std::vector<TH1F*>    h1DYcellChargeAsimmetryInv_                            ;
    std::vector<TH1F*>    h1DYcellChargeAsimmetryUnconstrained_                  ;
    std::vector<TH1F*>    h1DYcellChargeAsimmetryUnconstrainedInv_               ;
    std::vector<TH1F*>    h1DYCellChargeAsimmetrySizeLE2_                        ;
    std::vector<TH1F*>    h1DYEtaDistribution_                                   ;
    std::vector<TH1F*>    h1DYEtaDerivativeDistribution_                         ;

    /*-------------------------Y Asimmetry 4 Rows------------------------*/

    std::vector<TH2F*>    h2DYCellChargeAsimmetryUpRows4Rows_                    ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInvUpRows4Rows_                 ;
    std::vector<TH2F*>    h2DYCellChargeAsimmetryDownRows4Rows_                  ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInvDownRows4Rows_               ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInvRows1And2Of4Rows_            ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInvRows1And2Of4RowsCutOnEntries_;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInvRows4And3Of4Rows_            ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInvRows4And3Of4RowsCutOnEntries_;
    std::vector<TH1F*>    h1DYcellChargeAsimmetryInvRows1And2Of4Rows_            ;
    std::vector<TH1F*>    h1DYcellChargeAsimmetryInvRows4And3Of4Rows_            ;
    std::vector<TH2F*>    h2DYcellChargeAsimmetryInv4Rows_                       ;

    std::vector<std::map<int, TH2F*> > mXYMeanCharge_;
    std::vector<std::map<int, TH2F*> > mXYNorm_      ;
    std::vector<TH1F*>                 hXMeanCharge_ ;
    std::vector<TH1F*>                 hYMeanCharge_ ;

    int totEventsControl_;

};

#endif // CHARGE_H
