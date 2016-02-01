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
    virtual bool               checkWindowAbout       (float col     , float row        , int runNumber, int type ) const = 0;
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
