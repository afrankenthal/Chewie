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

#ifndef TRACKS_H
#define TRACKS_H

#include "Analysis.h"

#include <TTreeFormula.h>
#include <TFile.h>

#include <vector>
#include <iostream>


class TH1F           ;
class TH2F           ;
class AnalysisManager;
class PlanesMapping  ;
class XmlParser      ;
class XmlPlane       ;

class Tracks : public Analysis
{
public:
    Tracks  (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
    ~Tracks (void                                                    );

    void beginJob       (void                                                                );
    void analyze        (const Data&                       data    , int threadNumber        );
    void endJob         (void                                                                );
    void setCutsFormula (std::map<std::string,std::string> cutsList, std::vector<TTree*> tree);

    void getInFile      (TFile *                                                             ){}
    void numberOfTracksAnalysis   (const Data& data, int threadNumber             );

private:
    void PositionUnfolded_prepare (int planeID, const Data& data, int threadNumber);
    void PositionUnfolded_book    ();

    void book             (void                              );
    void destroy          (void                              );
    bool passStandardCuts (int   planeID,  const Data& data  );
    bool passBadPlanesCut (int          ,  const Data&       ) {return true;} //fake

    //void makeHistograms   (const Data& data, int threadNumber);

    PlanesMapping*         thePlaneMapping_    ;

    TH1F*                  hChi2_              ;
    TH1F*                   hNumberofHit_ ;
    TH1F*                   hTracksPlane_;
    TH1F* hTracksPlaneTelescopeHit_;
    TH1F*                  hXSlope_            ;
    TH1F*                  hSigmaXSlope_       ;
    TH1F*                  hYSlope_            ;
    TH1F*                  hSigmaYSlope_       ;
    TH1F*                  hXIntercept_        ;
    TH1F*                  hSigmaXIntercept_   ;
    TH1F*                  hYIntercept_        ;
    TH1F*                  hSigmaYIntercept_   ;
    TH1F*                  hXSlopeInsideWindow_ ;
    TH1F*                  hXSlopeOutsideWindow_;
    TH1F*                  hYSlopeInsideWindow_ ;
    TH1F*                  hYSlopeOutsideWindow_;
    TH2F*                  hXSlopeMedium_       ;
    TH2F*                  hYSlopeMedium_       ;
    std::vector<TH2F*>     hClusterHits_       ;
    std::vector<TH1F*>     hClusterProfileX_   ;
    std::vector<TH1F*>     hClusterProfileY_   ;
    std::vector<TH1F*>     hXError_            ;
    std::vector<TH1F*>     hYError_            ;
    std::vector<TH2F*>     hTracksProjRC_      ;
    std::vector<TH2F*>     hTracksProjXY_      ;
    std::vector<TH2F*>     hTracksProjXYPix_   ;
    std::vector<TH2F*>     hTracksProjXYPixAboveBeam_  ;
    std::vector<TH2F*>     hTracksProjXYPixBelowBeam_  ;
    std::vector<TH2F*>     hTracksProjXYPixLeftOfBeam_ ;
    std::vector<TH2F*>     hTracksProjXYPixRightOfBeam_;
    std::vector<TH2F*>     hTracksProjXYPixQuad1_;
    std::vector<TH2F*>     hTracksProjXYPixQuad2_;
    std::vector<TH2F*>     hTracksProjXYPixQuad3_;
    std::vector<TH2F*>     hTracksProjXYPixQuad4_;
    std::vector<TH2F*>     hTracksProjRCQuad1_;
    std::vector<TH2F*>     hTracksProjRCQuad2_;
    std::vector<TH2F*>     hTracksProjRCQuad3_;
    std::vector<TH2F*>     hTracksProjRCQuad4_;
    std::vector<TH2F*>     hTracksProjRCEdges_;
    std::vector<TH2F*>     hTracksProjRCRecon_;
    std::vector<TH1F*>     hTracksProjXPixError_;
    std::vector<TH1F*>     hTracksProjYPixError_;
    std::vector<TH1F*>     hXdistribution1D_      ;
    std::vector<TH1F*>     hXdistribution2D_      ;
    std::vector<TH2F*>     hXChargedistribution1D_;
    std::vector<TH2F*>     hXChargedistribution2D_;
    std::vector<TH1F*>     hYdistribution1D_      ;
    std::vector<TH1F*>     hYdistribution2D_      ;
    std::vector<TH2F*>     hYChargedistribution1D_;
    std::vector<TH2F*>     hYChargedistribution2D_;
    std::vector<TH2F*>     XProbabilityFunc_;
    std::vector<TH2F*>     YProbabilityFunc_;
    std::vector<TH2F*>     hClusterHitsNorm_;
    std::vector<TH2F*>     ClusterSizeHits_;
    std::vector<TH2F*>     hXSlopeDistribution_;
    std::vector<TH2F*>     hYSlopeDistribution_;

    std::vector<std::map<int, TH2F*> > hTrackHitsCols_;
    std::vector<int>                   cols_          ;//to declare which cols to select, initiated in constructor

    TH1F*                hNumberOfTracks_  ;
    TH1F*                hRunNumber_       ;
    TH1F*                hEventNumber_     ;
    TH1F*                hTracksVsEvNumber_;
    std::map<int, TH1F*> mEvNumberRun_     ;
};

#endif // TRACKS_H
