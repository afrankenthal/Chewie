#ifndef HistogramWindow_h
#define HistogramWindow_h

#include "Window.h"
#include "Data.h"

class TTree;
class TH2F;
class TH1F;

class HistogramWindow : public Window
{
public:
    HistogramWindow (std::string name, int nBinsX, int nBinsY, int totalEvents);
    ~HistogramWindow(void                                    );

    bool checkWindow            (float col, float row               ) const;
    bool checkWindowAbout       (float col, float row               ) const;
    void calculateWindow        (int   planeID , const Data& aData,
                                 int   lowerCol, int   higherCol  ,
                                 int   lowerRow, int   higherRow    );
    void removePixel            (int   col     , int   row          );
    void calculateTimeEfficiency(void                               );
    void sumThreaded            (void                               ){;}

    int  getNumberOfEvents      (void                               );

    std::pair<int,int> getNbins(void)const {return std::make_pair(nBinsX_,nBinsY_);}

private:
    TH2F* theHWindow_          ;
    TH2F* the2HTimeWindow_     ;
    TH2F* the2HTimeWindow_norm_;
    TH1F* the1HTimeWindow_     ;

    int totalEvents_           ;
}; 

#endif







