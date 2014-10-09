#ifndef HistogramWindow_h
#define HistogramWindow_h

#include "Window.h"
#include "Data.h"
#include <vector>
#include <map>

class TTree;
class TH2F;
class TH1F;

class HistogramWindow : public Window
{
public:
    HistogramWindow (std::string name, int binXMin, int binXMax, int binYMin, int binYMax, std::map<int,int> runNumberEntries);
    ~HistogramWindow(void                                                   );

    bool checkWindow            (float col, float row      , int runNumber  ) const;
    bool checkWindowAbout       (float col, float row      , int runNumber  ) const;
    bool checkTimeWindow        (float col, int eventNumber, int runNumber  ) const;
    bool checkTimeWindowAbout   (float col, int eventNumber, int runNumber  ) const;
    void calculateWindow        (int   planeID , const Data& aData,
                                 int   lowerCol, int   higherCol  ,
                                 int   lowerRow, int   higherRow            );
    void removePixel            (int   col     , int row                    );
    void calculateTimeEfficiency(void                                       );
    void sumThreaded            (void                                       ){;}

    int  getNumberOfEvents      (void                                       );

    std::pair<int,int> getNbins(void)const {return std::make_pair(nBinsX_,nBinsY_);}

private:
    std::map<int,TH2F*> theHWindow_          ;
    std::map<int,TH2F*> theH2TimeWindow_     ;
    std::map<int,TH2F*> theH2TimeWindow_norm_;
    std::map<int,TH1F*> theH1TimeWindow_     ;


}; 

#endif







