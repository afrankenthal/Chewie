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

#ifndef HistogramWindow_h
#define HistogramWindow_h

#include "Window.h"
#include "Data.h"
#include <vector>
#include <map>

class TTree;
class TH2F;
class TH1F;
class PlanesMapping;

class HistogramWindow : public Window
{
public:
    HistogramWindow (std::string name, int binXMin, int binXMax, int binYMin, int binYMax, std::map<int,int> runNumberEntries);
    ~HistogramWindow(void                                                   );



    bool checkWindow            (float col, float row      , int runNumber) const;
    bool checkWindow_v1         (float col, float row      , int runNumber , int planeID  ) const;
    bool checkGoodRun           (int runNumber             , int entry                    ) const;
    bool checkWindowAbout       (float col, float row      , int runNumber , int type     ) const;
    bool byPassCheckWindow      (void                                         ) const;
    bool checkTimeWindow        (float col, int eventNumber, int runNumber  ) const;
    bool AnticheckWindowAbout   (float col, float row, int runNumber, int type ) const;
    void label                  (int runNumber                              ) const;
    bool checkTimeWindowAbout   (float col, int eventNumber, int runNumber  ) const;
    void calculateWindow        (int   planeID , const Data& aData,
                                 int   lowerCol, int   higherCol  ,
                                 int   lowerRow, int   higherRow            );
    void removePixel            (int   col     , int row                    );
    void calculateTimeEfficiency(void                                       );
    void sumThreaded            (void                                       ){;}

    int  getNumberOfEvents      (void                                       );
    double runAverageEfficiency (int runNumber                              ) const;
//    void latencyPlots           (int runNumber                              );
    void combineLatencyPlots    (void                                       );
    std::pair<int,int> getNbins(void)const {return std::make_pair(nBinsX_,nBinsY_);}
    PlanesMapping*                                     thePlaneMapping_;

    //void label                  (const Data& data                     );

private:

    int binXMaxv1;
    int binXMinv1;
    int binYMaxv1;
    int binYMinv1;

    int nBinsXv1_;
    int nBinsYv1_;

    std::map<int,TH2F*> theHWindow_            ;
    std::map<int,TH2F*> theHWindowV1_          ;
    std::map<int,TH2F*> theH2TimeWindow_       ;
    std::map<int,TH2F*> theH2TimeWindow_norm_  ;
    std::map<int,TH1F*> theH1TimeWindow_       ;

    std::map<int,TH2F*> theHWindowClusterSize1_;
    std::map<int,TH2F*> theHWindowClusterSize2_;
    std::map<int,TH2F*> theHWindowClusterSize3_;
    std::map<int,TH2F*> theHWindowClusterSize4_;

    std::map<int,TH1F*> theStripIndex_;
    std::map<int,TH1F*> theRowIndex_;

    std::map<int,TH1F*> EfficiencyVsEventFull_norm_;
    std::map<int,TH1F*> EfficiencyVsEventFull_     ;
    std::map<int,TH1F*> EfficiencyVsEventGood_     ;
    std::map<int,TH2F*> EfficiencyVsEventFull2D_   ;
    std::map<int,TH2F*> EfficiencyVsEventGood2D_   ;

    std::map<int,TH1F*> EfficiencyVsTriggerLatency_;
    std::map<int,TH1F*> EfficiencyVsTriggerLatency_norm_;
    std::map<int,TH1F*> EfficiencyVsTriggerLatencyFull_;
    std::map<int,TH1F*> EfficiencyVsTriggerLatencyFull_norm_;

};

#endif







