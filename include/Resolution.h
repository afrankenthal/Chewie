#ifndef RESOLUTION_H
#define RESOLUTION_H

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

class Resolution : public Analysis
{
public:
    Resolution  (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
    ~Resolution (void                                                    );

    void beginJob            (void                                                                );
    void analyze             (const Data&                       data    , int threadNumber        );
    void endJob              (void                                                                );
    void setCutsFormula      (std::map<std::string,std::string> cutsList, std::vector<TTree*> tree);
    bool passStandardCuts    (int planeID                               , const Data& data        );
    bool passBadPlanesCut    (int  planeID, const Data& data                            );

    void   getInFile                 (TFile *                                                             ){}
    void   fitResolution             (int planeID                                               );
    double smearedSquare             (double *xx, double *par                                   );
    void   fitXresidualsSize1        (int planeID                                               );
    void   fitXresidualsCalculated   (int planeID                                               );
    void   fitXresidualsDigital      (int planeID                                               );
    void   fitYresidualsSize1        (int planeID                                               );
    void   fitYresidualsCalculated   (int planeID                                               );
    void   fitYresidualsDigital      (int planeID                                               );

private:
    void book                (void                                                      );
    void destroy             (void                                                      );

    void predictedErrors     (bool pass, int planeID, const Data& data, int threadNumber);
    void calculateXresiduals (bool pass, int planeID, const Data& data, int threadNumber);
    void calculateYresiduals (bool pass, int planeID, const Data& data, int threadNumber);
    void xResolution         (bool pass, int planeID, const Data& data, int threadNumber);
    void yResolution         (bool pass, int planeID, const Data& data, int threadNumber);
    void customProfileX      (std::vector<TH2F *>h2D, std::vector<TH1F *>prof, int planeID, const char* outputName, const char* outputTitle, int yBinMin, int yBinMax, int minNumberOfEntries);


    PlanesMapping*                                     thePlaneMapping_    ;
    std::map< std::string,std::vector<TTreeFormula*> > cutsFormulas_       ;
    const WindowsManager*                              theWindowsManager_  ;

    std::vector<TH1F*>          hXresiduals_                  ;
    std::vector<TH1F*>          hXresidualCalculated_         ;
    std::vector<TH2F*>          hX2DresidualCalculated_       ;
    std::vector<TH1F*>          hXresidualsClusterSize1_      ;
    std::vector<TH2F*>          hX2DresidualsClusterSize1_    ;
    std::vector<TH1F*>          hXresidualsClusterSize2_      ;
    std::vector<TH1F*>          hXresidualsDigital_           ;
    std::vector<TH2F*>          hX2DresidualsDigital_         ;
    std::vector<TH1F*>          hYresiduals_                  ;
    std::vector<TH1F*>          hYresidualCalculated_         ;
    std::vector<TH2F*>          hY2DresidualCalculated_       ;
    std::vector<TH1F*>          hYresidualsClusterSize1_      ;
    std::vector<TH2F*>          hY2DresidualsClusterSize1_    ;
    std::vector<TH1F*>          hYresidualsClusterSize2_      ;
    std::vector<TH1F*>          hYresidualsDigital_           ;
    std::vector<TH2F*>          hY2DresidualsDigital_         ;
    std::map<std::string,TH1F*> hPredictedXErrors_            ;
    std::map<std::string,TH1F*> hPredictedYErrors_            ;
    std::vector<TH1F*>          hSigmaXIntercept_             ;
    std::vector<TH1F*>          hSigmaYIntercept_             ;
    std::vector<TH1F*>          hSigmaXSlope_                 ;
    std::vector<TH1F*>          hSigmaYSlope_                 ;
    std::vector<TH2F*>          h2DCorrelationsResidualXvsX_  ;
    std::vector<TH2F*>          h2DCorrelationsResidualXvsY_  ;
    std::vector<TH2F*>          h2DCorrelationsResidualYvsY_  ;
    std::vector<TH2F*>          h2DCorrelationsResidualYvsX_  ;
    std::vector<TH1F*>          hCorrelationsResidualXvsX_    ;
    std::vector<TH1F*>          hCorrelationsResidualXvsY_    ;
    std::vector<TH1F*>          hCorrelationsResidualYvsY_    ;
    std::vector<TH1F*>          hCorrelationsResidualYvsX_    ;
    TF1*                        fSmearedSquare_               ;
};

#endif // RESOLUTION_H
