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

#ifndef EFFICIENCY_H
#define EFFICIENCY_H

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


class Efficiency : public Analysis
{
 public:
  Efficiency  (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
  ~Efficiency (void);

  void beginJob         (void);
  void analyze          (const Data& data, int threadNumber);
  void endJob           (void);

  void setCutsFormula   (std::map<std::string,std::string> cutsList, std::vector<TTree*> tree);
  bool passStandardCuts (int planeID, const Data& data);
  bool passBadPlanesCut (int planeID, const Data& data);

 private:
  void book            (void);
  void destroy         (void);

  void planeEfficiency (bool pass, int planeID, const Data& data, int threadNumber);
  void cellEfficiency  (bool pass, int planeID, const Data& data, int threadNumber);
  void xCellEfficiency (bool pass, int planeID, const Data& data, int threadNumber);
  void yCellEfficiency (bool pass, int planeID, const Data& data, int threadNumber);

  void setErrorsBar    (int  planeID);

  PlanesMapping*                                     thePlaneMapping_;
  std::map< std::string,std::vector<TTreeFormula*> > cutsFormulas_;
  const WindowsManager*                              theWindowsManager_;
  XmlParser*                                         theXmlParser_;


  // ##############
  // # Histograms #
  // ##############
  std::vector<TH1F*> hEfficiency_;
  std::vector<TH1F*> hEfficiencyNorm_;

  std::vector<TH1F*> hEfficiencyRef_;
  std::vector<TH1F*> hEfficiencyRefNorm_;

  std::vector<TH2F*> h2DEfficiency_;
  std::vector<TH2F*> h2DEfficiencyNorm_;

  std::vector<TH2F*> h2DEfficiencyRef_;
  std::vector<TH2F*> h2DEfficiencyRefNorm_;

  std::vector<TH2F*> h2D4cellEfficiency_;
  std::vector<TH2F*> h2D4cellEfficiencyNorm_;

  std::vector<TH2F*> hCellEfficiency_;
  std::vector<TH2F*> hCellEfficiencyNorm_;

  std::vector<TH2F*> hCellEfficiencyEvenColumns_;
  std::vector<TH2F*> hCellEfficiencyEvenColumnsNorm_;

  std::vector<TH2F*> hCellEfficiencyOddColumns_;
  std::vector<TH2F*> hCellEfficiencyOddColumnsNorm_;

  std::vector<TH1F*> h1DXcellEfficiencyFirstHit_;
  std::vector<TH1F*> h1DXcellEfficiencySecondHit_;
  std::vector<TH1F*> h1DXcellEfficiencyNorm_;

  std::vector<TH1F*> h1DYcellEfficiencyFirstHit_;
  std::vector<TH1F*> h1DYcellEfficiencySecondHit_;
  std::vector<TH1F*> h1DYcellEfficiencyNorm_;
};

#endif
