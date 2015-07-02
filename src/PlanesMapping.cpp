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

    positionToStation_[0 ]  = 5;
    positionToStation_[1 ]  = 5;
    positionToStation_[2 ]  = 5;
    positionToStation_[3 ]  = 5;
    positionToStation_[4 ]  = 6;
    positionToStation_[5 ]  = 6;
    positionToStation_[6 ]  = 6;
    positionToStation_[7 ]  = 6;
    positionToStation_[8 ]  = 2;
    positionToStation_[9 ]  = 2;
    positionToStation_[10]  = 2;
    positionToStation_[11]  = 2;
    positionToStation_[12]  = 0;
    positionToStation_[13]  = 0;
    positionToStation_[14]  = 0;
    positionToStation_[15]  = 0;
    positionToStation_[16]  = 7;
    positionToStation_[17]  = 7;
    positionToStation_[18]  = 7;
    positionToStation_[19]  = 7;
    positionToStation_[20]  = 7;
    positionToStation_[21]  = 7;
    positionToStation_[22]  = 4;
    positionToStation_[23]  = 4;
    positionToStation_[24]  = 4;
    //positionToMonicelliPlane_[25] = 4;

    positionToPlaquette_[0 ]  = 0;
    positionToPlaquette_[1 ]  = 1;
    positionToPlaquette_[2 ]  = 2;
    positionToPlaquette_[3 ]  = 3;
    positionToPlaquette_[4 ]  = 0;
    positionToPlaquette_[5 ]  = 1;
    positionToPlaquette_[6 ]  = 2;
    positionToPlaquette_[7 ]  = 3;
    positionToPlaquette_[8 ]  = 0;
    positionToPlaquette_[9 ]  = 1;
    positionToPlaquette_[10]  = 2;
    positionToPlaquette_[11]  = 3;
    positionToPlaquette_[12]  = 0;
    positionToPlaquette_[13]  = 1;
    positionToPlaquette_[14]  = 2;
    positionToPlaquette_[15]  = 3;
    positionToPlaquette_[16]  = 0;
    positionToPlaquette_[17]  = 1;
    positionToPlaquette_[18]  = 2;
    positionToPlaquette_[19]  = 3;
    positionToPlaquette_[20]  = 4;
    positionToPlaquette_[21]  = 5;
    positionToPlaquette_[22]  = 0;
    positionToPlaquette_[23]  = 1;
    positionToPlaquette_[24]  = 2;
    //positionToPlaquette_[25] = 3;

    positionToType_[0 ]  = 1;
    positionToType_[1 ]  = 1;
    positionToType_[2 ]  = 1;
    positionToType_[3 ]  = 1;
    positionToType_[4 ]  = 1;
    positionToType_[5 ]  = 1;
    positionToType_[6 ]  = 1;
    positionToType_[7 ]  = 1;
    positionToType_[8 ]  = 0;
    positionToType_[9 ]  = 0;
    positionToType_[10]  = 0;
    positionToType_[11]  = 0;
    positionToType_[12]  = 0;
    positionToType_[13]  = 0;
    positionToType_[14]  = 0;
    positionToType_[15]  = 0;
    positionToType_[16]  = 1;
    positionToType_[17]  = 1;
    positionToType_[18]  = 1;
    positionToType_[19]  = 1;
    positionToType_[20]  = 1;
    positionToType_[21]  = 1;
    positionToType_[22]  = 0;
    positionToType_[23]  = 0;
    positionToType_[24]  = 0;
    //positionToType_[25] = 3;

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
    if(positionToStation_.find(position) != positionToStation_.end())
        return positionToStation_[position];
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
    if(positionToPlaquette_.find(position) != positionToPlaquette_.end())
        return positionToPlaquette_[position];
    else
    {
        std::stringstream ss;
        ss << "ERROR: Plane number " << position << " doesn't exist!";
        STDLINE(ss.str(), ACRed);
        return -1;
    }
}

//////////////////////////////////////////////////////////////////////////
int PlanesMapping::getPlaneType(int position)
{
    if(positionToType_.find(position) != positionToType_.end())
        return positionToType_[position];
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
