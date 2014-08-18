#include "PlanesMapping.h"
#include "MessageTools.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////////
PlanesMapping::PlanesMapping()
{
    planeToPosition_["Strip_Telescope_Upstream0"]   = 0;
    planeToPosition_["Strip_Telescope_Upstream1"]   = 1;
    planeToPosition_["Strip_Telescope_Upstream2"]   = 2;
    planeToPosition_["Strip_Telescope_Upstream3"]   = 3;
    planeToPosition_["Strip_Telescope_Upstream4"]   = 4;
    planeToPosition_["Strip_Telescope_Upstream5"]   = 5;
    planeToPosition_["Strip_Telescope_Upstream6"]   = 6;
    planeToPosition_["Strip_Telescope_Upstream7"]   = 7;
    planeToPosition_["Telescope_Upstream0"]         = 8;
    planeToPosition_["Telescope_Upstream1"]         = 9;
    planeToPosition_["Telescope_Upstream2"]         = 10;
    planeToPosition_["Telescope_Upstream3"]         = 11;
    planeToPosition_["Telescope_Downstream0"]       = 12;
    planeToPosition_["Telescope_Downstream1"]       = 13;
    planeToPosition_["Telescope_Downstream2"]       = 14;
    planeToPosition_["Telescope_Downstream3"]       = 15;
    planeToPosition_["Strip_Telescope_Downstream0"] = 16;
    planeToPosition_["Strip_Telescope_Downstream1"] = 17;
    planeToPosition_["Strip_Telescope_Downstream2"] = 18;
    planeToPosition_["Strip_Telescope_Downstream3"] = 19;
    planeToPosition_["Strip_Telescope_Downstream4"] = 20;
    planeToPosition_["Strip_Telescope_Downstream5"] = 21;
    planeToPosition_["Dut0"]                        = 22;
    planeToPosition_["Dut1"]                        = 23;
    planeToPosition_["Dut2"]                        = 24;
    //planeToPosition_["Dut3"]                        = 25;

    positionToPlane_[0 ]  = "Strip_Telescope_Upstream0"  ;
    positionToPlane_[1 ]  = "Strip_Telescope_Upstream1"  ;
    positionToPlane_[2 ]  = "Strip_Telescope_Upstream2"  ;
    positionToPlane_[3 ]  = "Strip_Telescope_Upstream3"  ;
    positionToPlane_[4 ]  = "Strip_Telescope_Upstream4"  ;
    positionToPlane_[5 ]  = "Strip_Telescope_Upstream5"  ;
    positionToPlane_[6 ]  = "Strip_Telescope_Upstream6"  ;
    positionToPlane_[7 ]  = "Strip_Telescope_Upstream7"  ;
    positionToPlane_[8 ]  = "Telescope_Upstream0"        ;
    positionToPlane_[9 ]  = "Telescope_Upstream1"        ;
    positionToPlane_[10]  = "Telescope_Upstream2"        ;
    positionToPlane_[11]  = "Telescope_Upstream3"        ;
    positionToPlane_[12]  = "Telescope_Downstream0"      ;
    positionToPlane_[13]  = "Telescope_Downstream1"      ;
    positionToPlane_[14]  = "Telescope_Downstream2"      ;
    positionToPlane_[15]  = "Telescope_Downstream3"      ;
    positionToPlane_[16]  = "Strip_Telescope_Downstream0";
    positionToPlane_[17]  = "Strip_Telescope_Downstream1";
    positionToPlane_[18]  = "Strip_Telescope_Downstream2";
    positionToPlane_[19]  = "Strip_Telescope_Downstream3";
    positionToPlane_[20]  = "Strip_Telescope_Downstream4";
    positionToPlane_[21]  = "Strip_Telescope_Downstream5";
    positionToPlane_[22]  = "Dut0"                       ;
    positionToPlane_[23]  = "Dut1"                       ;
    positionToPlane_[24]  = "Dut2"                       ;
    //positionToPlane_[25] = "Dut3"                        ;

    positionToMonicelliPlane_[0 ]  = "Station: 5 - Plaq: 0";
    positionToMonicelliPlane_[1 ]  = "Station: 5 - Plaq: 1";
    positionToMonicelliPlane_[2 ]  = "Station: 5 - Plaq: 2";
    positionToMonicelliPlane_[3 ]  = "Station: 5 - Plaq: 3";
    positionToMonicelliPlane_[4 ]  = "Station: 6 - Plaq: 0";
    positionToMonicelliPlane_[5 ]  = "Station: 6 - Plaq: 1";
    positionToMonicelliPlane_[6 ]  = "Station: 6 - Plaq: 2";
    positionToMonicelliPlane_[7 ]  = "Station: 6 - Plaq: 3";
    positionToMonicelliPlane_[8 ]  = "Station: 2 - Plaq: 0";
    positionToMonicelliPlane_[9 ]  = "Station: 2 - Plaq: 1";
    positionToMonicelliPlane_[10]  = "Station: 2 - Plaq: 2";
    positionToMonicelliPlane_[11]  = "Station: 2 - Plaq: 3";
    positionToMonicelliPlane_[12]  = "Station: 0 - Plaq: 0";
    positionToMonicelliPlane_[13]  = "Station: 0 - Plaq: 1";
    positionToMonicelliPlane_[14]  = "Station: 0 - Plaq: 2";
    positionToMonicelliPlane_[15]  = "Station: 0 - Plaq: 3";
    positionToMonicelliPlane_[16]  = "Station: 7 - Plaq: 0";
    positionToMonicelliPlane_[17]  = "Station: 7 - Plaq: 1";
    positionToMonicelliPlane_[18]  = "Station: 7 - Plaq: 2";
    positionToMonicelliPlane_[19]  = "Station: 7 - Plaq: 3";
    positionToMonicelliPlane_[20]  = "Station: 7 - Plaq: 4";
    positionToMonicelliPlane_[21]  = "Station: 7 - Plaq: 5";
    positionToMonicelliPlane_[22]  = "Station: 4 - Plaq: 0";
    positionToMonicelliPlane_[23]  = "Station: 4 - Plaq: 1";
    positionToMonicelliPlane_[24]  = "Station: 4 - Plaq: 2";
    //positionToMonicelliPlane_[25] = "Station: 4 - Plaq: 3";
}

//////////////////////////////////////////////////////////////////////////
PlanesMapping::~PlanesMapping()
{
}

//////////////////////////////////////////////////////////////////////////
std::string PlanesMapping::getMonicelliPlaneName(int position)
{
    if(positionToMonicelliPlane_.find(position) != positionToMonicelliPlane_.end())
        return positionToMonicelliPlane_[position];
    else
    {
        std::stringstream ss;
        ss << "ERROR: Plane number " << position << " doesn't exist!";
        STDLINE(ss.str(), ACRed);
    }
    return "";
}

//////////////////////////////////////////////////////////////////////////
std::string PlanesMapping::getPlaneName(int position)
{
    if(positionToPlane_.find(position) != positionToPlane_.end())
        return positionToPlane_[position];
    else
    {
        std::stringstream ss;
        ss << "ERROR: Plane number " << position << " doesn't exist!";
        STDLINE(ss.str(), ACRed);
    }
    return "";
}

//////////////////////////////////////////////////////////////////////////
int PlanesMapping::getStation(int position)
{
    if(position>=0 && position<=3)
        return 5;
    else if(position>=4 && position<=7)
        return 6;
    else if(position>=8 && position<=11)
        return 2;
    else if(position>=12 && position<=15)
        return 0;
    else if(position>=16 && position<=21)
        return 7;
    else if(position>=22 && position<=24)
        return 4;
    else
    {
        std::stringstream ss;
        ss << "ERROR: Plane number " << position << " doesn't exist!";
        STDLINE(ss.str(), ACRed);
        return -1;
    }
}

//////////////////////////////////////////////////////////////////////////
int PlanesMapping::getPlaquette(int position)
{
    //    if(position<12)
    if(position<=19)
        return position%4;
    else if(position == 20)
        return 4;
    else if(position == 21)
        return 5;
    else if(position == 22)
        return 0;
    else if(position == 23)
        return 1;
    else if(position == 24)
        return 2;
    else if(position == 25)
        return 3;
    else
    {
        std::stringstream ss;
        ss << "ERROR: Plane number " << position << " doesn't exist!";
        STDLINE(ss.str(), ACRed);
        return -1;
    }
}

//////////////////////////////////////////////////////////////////////////
int PlanesMapping::getPlanePosition(std::string plane)
{
    if(planeToPosition_.find(plane) != planeToPosition_.end())
        return planeToPosition_[plane];
    else
        STDLINE("ERROR: Plane name " + plane + " doesn't exist!", ACRed);
    return -1;
}
