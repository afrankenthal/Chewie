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

#ifndef RESOLUTIONOUTERTRACKER_H
#define RESOLUTIONOUTERTRACKER_H

#include "Analysis.h"

#include <TTreeFormula.h>

#include <vector>
#include <iostream>
#include <TH2F.h>

class TH1F           ;
class TH2F           ;
class TTreeFormula   ;
class AnalysisManager;
class WindowsManager ;
class PlanesMapping  ;
class XmlParser      ;

class ResolutionOuterTracker : public Analysis
{
public:
    ResolutionOuterTracker  (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
    ~ResolutionOuterTracker (void                                                    );

    void beginJob            (void                                                                );
    void analyze             (const Data&          data                         , int threadNumber);
    void endJob              (void                                                                );
    void setCutsFormula      (std::map<std::string,std::string> cutsList, std::vector<TTree*> tree);
    bool passStandardCuts    (int planeID                               , const Data& data        );
    bool passBadPlanesCut    (int  planeID, const Data& data                            );

    void   getInFile                   (TFile *                                                             ){}
    void   fitResolutionOuterTracker               (int planeID                                               );
    double smearedSquare               (double *xx, double *par                                   );
    void   fitXResiduals               (int planeID                                               );
    void   fitXresidualsSize1          (int planeID                                               );
    void   fitXresidualsSize2          (int planeID                                               );
    void   fitXresidualsSize3          (int planeID                                               );
    void   fitXresidualsSize4          (int planeID                                               );
    void   fitXresidualsCalculated     (int planeID                                               );
    void   fitXresidualsCalculatedSize2(int planeID                                               );
    void   fitXresidualsCalculatedSize3(int planeID                                               );
    void   fitXresidualsCalculatedSize4(int planeID                                               );
    void   fitXresidualsDigital        (int planeID                                               );
    void   fitYResiduals               (int planeID                                               );
    void   fitYresidualsSize1          (int planeID                                               );
    void   fitYresidualsSize2          (int planeID                                               );
    void   fitYresidualsSize3          (int planeID                                               );
    void   fitYresidualsSize4          (int planeID                                               );
    void   fitYresidualsCalculated     (int planeID                                               );
    void   fitYresidualsCalculatedSize2(int planeID                                               );
    void   fitYresidualsCalculatedSize3(int planeID                                               );
    void   fitYresidualsCalculatedSize4(int planeID                                               );
    void   fitYresidualsDigital        (int planeID                                               );

private:
    void book                (void                                                      );
    void destroy             (void                                                      );

    void predictedErrors     (bool pass, int planeID, const Data& data, int threadNumber);
    void calculateXresiduals (bool pass, int planeID, const Data& data, int threadNumber);
    void calculateYresiduals (bool pass, int planeID, const Data& data, int threadNumber);
    void xResolutionOuterTracker         (bool pass, int planeID, const Data& data, int threadNumber);
    void yResolutionOuterTracker         (bool pass, int planeID, const Data& data, int threadNumber);
    void customProfileX      (std::vector<TH2F *>h2D, std::vector<TH1F *>prof, int planeID, const char* outputName, const char* outputTitle, int yBinMin, int yBinMax, int minNumberOfEntries);

    int nevent;

    PlanesMapping*                                     thePlaneMapping_    ;
    std::map< std::string,std::vector<TTreeFormula*> > cutsFormulas_       ;
    const WindowsManager*                              theWindowsManager_  ;
    XmlParser*                                         theXmlParser_       ;

    std::vector<TH1F*>          hXResiduals_                             ;
    std::vector<TH1F*>          hXResidualCalculated_                    ;
    std::vector<TH1F*>          hXResidualCalculatedSize2_               ;
    std::vector<TH2F*>          hX2DResidualCalculatedSize2_             ;
    std::vector<TH1F*>          hXResidualCalculatedSize3_               ;
    std::vector<TH2F*>          hX2DResidualCalculatedSize3_             ;
    std::vector<TH1F*>          hXResidualCalculatedSize4_               ;
    std::vector<TH2F*>          hX2DResidualCalculatedSize4_             ;
    std::vector<TH1F*>          hXResidualsClusterSize1_                 ;
    std::vector<TH2F*>          hX2DResidualsClusterSize1_               ;
    std::vector<TH1F*>          hXResidualsClusterSize2_                 ;
    std::vector<TH2F*>          hX2DResidualsClusterSize2_               ;
    std::vector<TH1F*>          hXResidualsClusterSize3_                 ;
    std::vector<TH1F*>          hXResidualsClusterSize4_                 ;
    std::vector<TH1F*>          hXResidualsDigital_                      ;
    std::vector<TH2F*>          hX2DResidualsDigital_                    ;
    std::vector<TH1F*>          hYResiduals_                             ;
    std::vector<TH1F*>          hYResidualCalculated_                    ;
    std::vector<TH1F*>          hYResidualCalculatedSize2_               ;
    std::vector<TH2F*>          hY2DResidualCalculatedSize2_             ;
    std::vector<TH1F*>          hYResidualCalculatedSize2Row1of4Rows_    ;
    std::vector<TH1F*>          hYResidualCalculatedSize2Row2of4Rows_    ;
    std::vector<TH1F*>          hYResidualCalculatedSize2Row3of4Rows_    ;
    std::vector<TH1F*>          hYResidualCalculatedSize2Row4of4Rows_    ;
    std::vector<TH1F*>          hLandauChargeDownSize2Row1of4Rows_       ;
    std::vector<TH1F*>          hLandauChargeUpSize2Row2of4Rows_         ;
    std::vector<TH1F*>          hLandauChargeDownSize2Row3of4Rows_       ;
    std::vector<TH1F*>          hLandauChargeUpSize2Row4of4Rows_         ;
    std::vector<TH1F*>          hYResidualCalculatedSize3_               ;
    std::vector<TH2F*>          hY2DResidualCalculatedSize3_             ;
    std::vector<TH1F*>          hYResidualCalculatedSize4_               ;
    std::vector<TH2F*>          hY2DResidualCalculatedSize4_             ;
    std::vector<TH1F*>          hYResidualsClusterSize1_                 ;
    std::vector<TH2F*>          hY2DResidualsClusterSize1_               ;
    std::vector<TH1F*>          hYResidualsClusterSize2_                 ;
    std::vector<TH1F*>          hYResidualsClusterSize3_                 ;
    std::vector<TH1F*>          hYResidualsClusterSize4_                 ;
    std::vector<TH1F*>          hYResidualsDigital_                      ;
    std::vector<TH2F*>          hY2DResidualsDigital_                    ;
    std::map<std::string,TH1F*> hPredictedXErrors_                       ;
    std::map<std::string,TH1F*> hPredictedYErrors_                       ;
    std::vector<TH1F*>          hSigmaXIntercept_                        ;
    std::vector<TH1F*>          hSigmaYIntercept_                        ;
    std::vector<TH1F*>          hSigmaXSlope_                            ;
    std::vector<TH1F*>          hSigmaYSlope_                            ;
    std::vector<TH2F*>          h2DCorrelationsResidualXvsX_             ;
    std::vector<TH2F*>          h2DCorrelationsResidualXvsY_             ;
    std::vector<TH2F*>          h2DCorrelationsResidualYvsY_             ;
    std::vector<TH2F*>          h2DCorrelationsResidualYvsX_             ;
    std::vector<TH1F*>          hCorrelationsResidualXvsX_               ;
    std::vector<TH1F*>          hCorrelationsResidualXvsY_               ;
    std::vector<TH1F*>          hCorrelationsResidualYvsY_               ;
    std::vector<TH1F*>          hCorrelationsResidualYvsX_               ;
    std::vector<TH1F*>          hYResidualCalculatedSize2Row1of2Rows_    ;
    std::vector<TH1F*>          hYResidualCalculatedSize2Row2of2Rows_    ;
    TF1*                        fSmearedSquare_                          ;



};

#endif // ResolutionOuterTracker_H
