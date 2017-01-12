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

#ifndef RESOLUTIONUNIMIB_H
#define RESOLUTIONUNIMIB_H

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


class ResolutionUniMiB : public Analysis
{
 public:
   ResolutionUniMiB     (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
  ~ResolutionUniMiB     (void);

  void beginJob         (void);
  void analyze          (const Data& data, int threadNumber);
  void endJob           (void);
  
  void setCutsFormula   (std::map<std::string,std::string> cutsList, std::vector<TTree*> tree);
  bool passStandardCuts (int planeID, const Data& data);

 private:
  void book                (void);
  void destroy             (void);
  
  void predictedErrors     (bool pass, int planeID, const Data& data, int threadNumber);
  void calculateXresiduals (bool pass, int planeID, const Data& data, int threadNumber);
  void calculateYresiduals (bool pass, int planeID, const Data& data, int threadNumber);
  void xResolution         (bool pass, int planeID, const Data& data, int threadNumber);
  void yResolution         (bool pass, int planeID, const Data& data, int threadNumber);
  
  PlanesMapping*                                     thePlaneMapping_;
  std::map< std::string,std::vector<TTreeFormula*> > cutsFormulas_;
  const WindowsManager*                              theWindowsManager_;
  XmlParser*                                         theXmlParser_;
  
  
  // ##############
  // # Histograms #
  // ##############
  std::vector<TH1F*>          hXResiduals_;
  std::vector<TH1F*>          hXResidualCalculated_;
  
  std::vector<TH1F*>          hYResiduals_;
  std::vector<TH1F*>          hYResidualCalculated_;
  
  std::vector<TH1F*>          hXResidualCalculatedSize2_;
  std::vector<TH1F*>          hYResidualCalculatedSize2_;
  
  std::vector<TH1F*>          hXResidualsClusterSize2_;
  std::vector<TH1F*>          hYResidualsClusterSize2_;
  
  std::map<std::string,TH1F*> hPredictedXErrors_;
  std::map<std::string,TH1F*> hPredictedYErrors_;
  
  std::vector<TH2F*>          h2DCorrelationsResidualXvsX_;
  std::vector<TH2F*>          h2DCorrelationsResidualXvsY_;
  std::vector<TH2F*>          h2DCorrelationsResidualYvsY_;
  std::vector<TH2F*>          h2DCorrelationsResidualYvsX_;
  
  std::vector<TH1F*>          hCorrelationsResidualXvsX_;
  std::vector<TH1F*>          hCorrelationsResidualXvsY_;
  std::vector<TH1F*>          hCorrelationsResidualYvsY_;
  std::vector<TH1F*>          hCorrelationsResidualYvsX_;
};

#endif
