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

#ifndef CHARGEUNIMIB_H
#define CHARGEUNIMIB_H

#include "Analysis.h"

#include <TTreeFormula.h>

#include <vector>
#include <iostream>


class TH1F;
class TH2F;
class TTreeFormula;

class AnalysisManager;
class WindowsManager;
class PlanesMapping;
class XmlParser;

class CalibrationsManager;

class ChargeUniMiB : public Analysis
{
 public:
   ChargeUniMiB            (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
  ~ChargeUniMiB            (void);

  void beginJob            (void);
  void analyze             (const Data& data, int threadNumber);
  void endJob              (void);

  void setCutsFormula      (std::map<std::string,std::string> cutsList, std::vector<TTree*> tree);
  bool passStandardCuts    (int planeID, const Data& data);
  bool passBadPlanesCut    (int planeID, const Data& data);

 private:
  void book                (void);
  void destroy             (void);

  void clusterSize         (int planeID, const Data& data, int threadNumber);
  void cellLandau          (bool pass, int planeID, const Data& data, int threadNumber);
  void clusterLandau       (bool pass, int planeID, const Data& data, int threadNumber);
  void cellCharge          (bool pass, int planeID, const Data& data, int threadNumber);
  void xLandau             (bool pass, int planeID, const Data& data, int threadNumber);
  void yLandau             (bool pass, int planeID, const Data& data, int threadNumber);

  void xChargeDivision     (bool pass, int planeID, const Data& data, int threadNumber);
  void xAsimmetry          (bool pass, int planeID, const Data& data, int threadNumber);

  void yChargeDivision     (bool pass, int planeID, const Data& data, int threadNumber);
  void yAsimmetry          (bool pass, int planeID, const Data& data, int threadNumber);

  bool passCalibrationsCut (int  planeID, const Data& data);
  void setErrorsBar        (int  planeID);

  PlanesMapping*                                     thePlaneMapping_;
  std::map< std::string,std::vector<TTreeFormula*> > cutsFormulas_;
  const WindowsManager*                              theWindowsManager_;
  XmlParser*                                         theXmlParser_;

  CalibrationsManager*                               theCalibrationsManager_;

  int standardCutsPixelMinimumCharge_;
  int standardCutsPixelMaximumCharge_;
  int standardCutsClusterMinimumCharge_;
  int standardCutsClusterMaximumCharge_;


  // ##############
  // # Histograms #
  // ##############
  std::vector<TH1F*>    hClusterSize_;
  std::vector<TH1F*>    hCellLandau_;
  std::vector<TH1F*>    hCellLandauOdd_;
  std::vector<TH1F*>    hCellLandauEven_;

  std::vector<TH1F*>    hLandauClusterSize1_;
  std::vector<TH1F*>    hLandauClusterSize2_;
  std::vector<TH1F*>    hLandauClusterSize2sameCol_;
  std::vector<TH1F*>    hLandauClusterSize2sameRow_;

  std::vector<TH1F*>    h1DXcellCharge_;
  std::vector<TH1F*>    h1DXcellChargeNorm_;
  
  std::vector<TH1F*>    h1DXcellChargeSecondHit_;
  std::vector<TH1F*>    h1DXcellChargeSecondHitNorm_;
  
  std::vector<TH1F*>    h1DYcellCharge_;
  std::vector<TH1F*>    h1DYcellChargeNorm_;
  
  std::vector<TH1F*>    h1DYcellChargeSecondHit_;
  std::vector<TH1F*>    h1DYcellChargeSecondHitNorm_;


  std::vector<TH2F*>    h2DCellCharge_;
  std::vector<TH2F*>    h2DCellChargeNorm_;

  std::vector<TH2F*>    h2DCellChargeOdd_;
  std::vector<TH2F*>    h2DCellChargeOddNorm_;

  std::vector<TH2F*>    h2DCellChargeEven_;
  std::vector<TH2F*>    h2DCellChargeEvenNorm_;

  std::vector<TH2F*>    h4CellsCharge_;
  std::vector<TH2F*>    h4CellsChargeNorm_;

  std::vector<TH2F*>    h2DClusterSize_;

  std::vector<TH2F*>    h2DXcellCharge_;
  std::vector<TH2F*>    h2DXcellChargeSecondHit_;

  std::vector<TH2F*>    h2DYcellCharge_;
  std::vector<TH2F*>    h2DYcellChargeSecondHit_;


  std::vector<TH2F*>    h2DXcellChargeAsimmetry_;
  std::vector<TH1F*>    h1DXcellChargeAsimmetry_;
  std::vector<TH2F*>    h2DXcellChargeAsimmetryInv_;
  std::vector<TH1F*>    h1DXcellChargeAsimmetryInv_;

  std::vector<TH2F*>    h2DYcellChargeAsimmetry_;
  std::vector<TH1F*>    h1DYcellChargeAsimmetry_;
  std::vector<TH2F*>    h2DYcellChargeAsimmetryInv_;
  std::vector<TH1F*>    h1DYcellChargeAsimmetryInv_;
};

#endif
