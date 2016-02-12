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

#ifndef __EventConverter_H__
#define __EventConverter_H__

#include "EventManager.h"
#include "Threader.h"
#include "PlanesMapping.h"
#include "Data.h"

#include <sstream>
#include <string>
#include <vector>

class Event;
class Data;
class TTree;
class Geometry;
class EventHeader;
class PlanesMapping;

//====================================================================
class EventConverter : public Threader
{
public:
                EventConverter (EventManager* eventManager,
                                int           nOfThreads   );
               ~EventConverter (void                       ){;}

    void        convert        (Event&        event,
                                int           e            );
    void        beginJob       (void                       );
    void        endJob         (void                       );
    int         runConverter   (void                       );

    void        setRunNumber   (int           r            ) {runNumber_ = r;}

private:
    int          execute       (int           threadNumber );

    EventManager*       theEventManager_ ;
    EventHeader*        theHeader_       ;
    Geometry*           theGeometry_     ;
    TTree*              outTree_         ;
    Data                theData_         ;
    PlanesMapping       thePlanesMapping_;
    std::vector<Event>  eventVector_     ;
    int                 counter_         ;
    int                 runNumber_       ;
    std::stringstream   ss_              ;
};

#endif
