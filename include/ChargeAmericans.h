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

#ifndef CHARGEAMERICANS_H
#define CHARGEAMERICANS_H

#include "Analysis.h"
#include "smallPitchTranslation25x100.h"

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
class SmallPitchTranslation25x100;

class CalibrationsManager;

class ChargeAmericans : public Analysis
{
 public:
   ChargeAmericans            (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
  ~ChargeAmericans            (void);

  void beginJob            (void);
  void analyze             (const Data& data, int threadNumber);
  void endJob              (void);

  void setCutsFormula      (std::map<std::string,std::string> cutsList, std::vector<TTree*> tree);
  bool passStandardCuts    (int planeID, const Data& data);

 private:
  void book                (void);
  void destroy             (void);

  void clusterSize         (int planeID, const Data& data, int threadNumber);
  void cellLandau          (bool pass, int planeID, const Data& data, int threadNumber);
  void clusterLandau       (bool pass, int planeID, const Data& data, int threadNumber);
  void planeCharge         (bool pass, int planeID, const Data& data, int threadNumber);
  void cellCharge          (bool pass, int planeID, const Data& data, int threadNumber);
  void xLandau             (bool pass, int planeID, const Data& data, int threadNumber);
  void yLandau             (bool pass, int planeID, const Data& data, int threadNumber);

  void xChargeDivision     (bool pass, int planeID, const Data& data, int threadNumber);
  void xChargeDivision_v1  (bool pass, int planeID, const Data& data, int threadNumber);
  void xAsymmetry          (bool pass, int planeID, const Data& data, int threadNumber);

  void yChargeDivision     (bool pass, int planeID, const Data& data, int threadNumber);
  void yAsymmetry          (bool pass, int planeID, const Data& data, int threadNumber);

  bool passCalibrationsCut (int  planeID, const Data& data);
  void setErrorsBar        (int  planeID);

  PlanesMapping*                                     thePlaneMapping_;
  std::map< std::string,std::vector<TTreeFormula*> > cutsFormulas_;
  const WindowsManager*                              theWindowsManager_;
  XmlParser*                                         theXmlParser_;
  SmallPitchTranslation25x100*                       thePitchTranslation25x100_;

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
  std::vector<TH1F*>    hCellLandauOddLeft_;
  std::vector<TH1F*>    hCellLandauOddRight_;
  std::vector<TH1F*>    hCellLandauEven_;
  std::vector<TH1F*>    hCellLandauEvenLeft_;
  std::vector<TH1F*>    hCellLandauEvenRight_;

  std::vector<TH1F*>    hLandauClusterSize1_;
  std::vector<TH1F*>    hLandauClusterSize2_;
  std::vector<TH1F*>    hLandauClusterSize2sameCol_;
  std::vector<TH1F*>    hLandauClusterSize2sameRow_;
  std::vector<TH1F*>    hCellLandauSinglePixel_;

  std::vector<TH1F*>    h1DXcellCharge_;
  std::vector<TH1F*>    h1DXcellChargeNorm_;
  std::vector<TH1F*>    h1DXcellOddCharge_;
  std::vector<TH1F*>    h1DXcellOddChargeNorm_;
  std::vector<TH1F*>    h1DXcellEvenCharge_;
  std::vector<TH1F*>    h1DXcellEvenChargeNorm_;

  std::vector<TH1F*>    h1DXcellChargeSecondHit_;
  std::vector<TH1F*>    h1DXcellChargeSecondHitNorm_;
  std::vector<TH1F*>    h1DXcellOddChargeSecondHit_;
  std::vector<TH1F*>    h1DXcellOddChargeSecondHitNorm_;
  std::vector<TH1F*>    h1DXcellEvenChargeSecondHit_;
  std::vector<TH1F*>    h1DXcellEvenChargeSecondHitNorm_;

	//>> wsi 11/12/17
  std::vector<TH1F*>    h1DXcellChargeFourthHit_;
  std::vector<TH1F*>    h1DXcellChargeFourthHitNorm_;
  std::vector<TH1F*>    h1DXcellOddChargeFourthHit_;
  std::vector<TH1F*>    h1DXcellOddChargeFourthHitNorm_;
  std::vector<TH1F*>    h1DXcellEvenChargeFourthHit_;
  std::vector<TH1F*>    h1DXcellEvenChargeFourthHitNorm_;

	//<< wsi 11/12/17
  
  std::vector<TH1F*>    h1DYcellCharge_;
  std::vector<TH1F*>    h1DYcellChargeNorm_;
  
  std::vector<TH1F*>    h1DYcellChargeSecondHit_;
  std::vector<TH1F*>    h1DYcellChargeSecondHitNorm_;

  //>> wsi 11/12/17
  std::vector<TH1F*>    h1DYcellChargeFourthHit_;
  std::vector<TH1F*>    h1DYcellChargeFourthHitNorm_;
	//<< wsi 11/12/17

  std::vector<TH2F*>    h2DCharge_;
  std::vector<TH2F*>    h2DChargeNorm_;


  std::vector<TH2F*>    h2DCellCharge_;
  std::vector<TH2F*>    h2DCellChargeNorm_;

  std::vector<TH2F*>    h2DCellChargeOdd_;
  std::vector<TH2F*>    h2DCellChargeOddNorm_;

  std::vector<TH2F*>    h2DCellChargeEven_;
  std::vector<TH2F*>    h2DCellChargeEvenNorm_;

  std::vector<TH2F*>    h4CellsCharge_;
  std::vector<TH2F*>    h4CellsChargeNorm_;

  std::vector<TH2F*>    h16CellsCharge_;
  std::vector<TH2F*>    h16CellsChargeNorm_;

  std::vector<TH2F*>    h2DClusterSize_;

  std::vector<TH2F*>    h2DXcellCharge_;
  std::vector<TH2F*>    h2DXcellChargeSecondHit_;
  std::vector<TH2F*>    h2DXcellChargeFourthHit_;

  std::vector<TH2F*>    h2DXcellOddCharge_;
  std::vector<TH2F*>    h2DXcellOddChargeSecondHit_;
  std::vector<TH2F*>    h2DXcellOddChargeFourthHit_;

  std::vector<TH2F*>    h2DXcellEvenCharge_;
  std::vector<TH2F*>    h2DXcellEvenChargeSecondHit_;
  std::vector<TH2F*>    h2DXcellEvenChargeFourthHit_;

  std::vector<TH2F*>    h2DYcellCharge_;
  std::vector<TH2F*>    h2DYcellChargeSecondHit_;
  std::vector<TH2F*>    h2DYcellChargeFourthHit_;

  std::vector<TH2F*>    h2DXcellChargeAsymmetry_;
  std::vector<TH1F*>    h1DXcellChargeAsymmetry_;
  std::vector<TH2F*>    h2DXcellChargeAsymmetryInv_;
  std::vector<TH1F*>    h1DXcellChargeAsymmetryInv_;

  std::vector<TH2F*>    h2DYcellChargeAsymmetry_;
  std::vector<TH1F*>    h1DYcellChargeAsymmetry_;
  std::vector<TH2F*>    h2DYcellChargeAsymmetryInv_;
  std::vector<TH1F*>    h1DYcellChargeAsymmetryInv_;
};

#endif
