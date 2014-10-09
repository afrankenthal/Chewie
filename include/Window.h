#ifndef Window_h
#define Window_h

#include <string>
#include <sstream>
#include <vector>
#include "MessageTools.h"


class Data;

class Window
{
public:
    Window          (std::string name);
    virtual ~Window (void                                    );

    virtual bool               checkWindow            (float col     , float row        , int runNumber ) const = 0;
    virtual bool               checkWindowAbout       (float col     , float row        , int runNumber ) const = 0;
    virtual bool               checkTimeWindow        (float col     , int eventNumber  , int runNumber ) const = 0;
    virtual bool               checkTimeWindowAbout   (float col     , int eventNumber  , int runNumber ) const = 0;

    virtual void               calculateWindow        (int   planeID , const Data& aData,
                                                       int   lowerCol, int   higherCol  ,
                                                       int   lowerRow, int   higher                     ) = 0;
    virtual void               calculateTimeEfficiency(void                                             ){;}
    virtual void               removePixel            (int   col     , int   row                        ) = 0;
    virtual void               sumThreaded            (void                                             ){;}
    virtual std::pair<int,int> getNbins               (void                                             ) const = 0;
    void                       setDoNotDelete         (bool  flag                                       ){fDoNotDelete_ = flag;}

//    virtual int                getNumberOfEvents(void                              ){return 0;}

    void                       print           (void                                                    )const {STDLINE("Window name = " + name_, ACYellow);}

protected:
    std::string  name_        ;
    int          nBinsX_      ;
    int          nBinsY_      ;
    bool         fDoNotDelete_;
}; 

#endif
