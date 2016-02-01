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

#ifndef CALIBRATIONSMANAGER_H
#define CALIBRATIONSMANAGER_H

#include "EventManager.h"

#include "TFile.h"

#include <map>
#include <string>

#define NPARS     4
/*#define NCOLS    52
#define NROWS    80
#define FIRSTROW  0
#define LASTROW  79
#define FIRSTCOL  0
#define LASTCOL  51*/

class Geometry     ;
class PlanesMapping;
class TH1F         ;
class TH2F         ;
class TF1          ;

class CalibrationsManager
{
public:
    CalibrationsManager  (EventManager* eventManager = 0);
    ~CalibrationsManager (void                          );

    void   openCalibPlotsFile (std::string fileName);
    void   copyHistograms     (void                );

    TH2F*  getParHisto        (int         parID   ){return h2DparsPlots_[parID];}

    void   setDut             (std::string dut     ){dut_        =      dut;}
    void   setInFileName      (std::string fileName){inFileName_ = fileName;}
    void   setWindow          (void                );
    void   bookParsPlots      (void                );
    void   bookCalibHistos    (void                );
    void   makeParsPlots      (void                );
    void   makeCalibHistos    (void                );
    void   drawFitFunction    (void                );
    void   destroyCalibHistos (void                );
    void   destroyParsPlots   (void                );

private:
    EventManager*                        theEventManager_             ;
    Geometry*                            theGeometry_                 ;
    PlanesMapping*                       thePlanesMapping_            ;
    std::string                          inFileName_                  ;
    TFile*                               calibPlotsFile_              ;
    std::string                          dut_                         ;
    int                                  firstRow_                    ;
    int                                  lastRow_                     ;
    int                                  firstCol_                    ;
    int                                  lastCol_                     ;

    TF1*                                 calFunk_                     ;
    TF1*                                 calFunkInv_                  ;
    TH1F*                                h1DparsPlots_         [NPARS];
    TH2F*                                h2DparsPlots_         [NPARS];
    std::map< int, std::map<int,TH1F*> > hPixelCalibration_           ;
    TH1F*                                hThreshold_                  ;
    TH2F*                                h2DThreshold_                ;
    TH1F*                                h1Dresidual_                 ;
    TH2F*                                h2Dresidual_                 ;
    TH2F*                                hCorrelations_ [NPARS][NPARS];
    TH2F*                                hFirstBin_                   ;
    TH2F*                                hLastBin_                    ;
};

#endif // CALIBRATIONSMANAGER_H
