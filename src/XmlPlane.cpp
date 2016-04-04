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

#include "XmlPlane.h"
#include "XmlWindow.h"
#include <utility>
#include "MessageTools.h"
//#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////
XmlPlane::XmlPlane(QDomNode& node)
{
    thisNode_ = node;
    QDomNode window = node.toElement().elementsByTagName("Window").at(0);
    theWindow_ = new XmlWindow(window);
    calibNode_ = thisNode_.toElement().elementsByTagName("Calibrations").at(0);
}

//////////////////////////////////////////////////////////////////////////////////////
XmlPlane::~XmlPlane()
{
    if(theWindow_)
        delete theWindow_;
}

//////////////////////////////////////////////////////////////////////////////////////
std::pair<std::string,std::string> XmlPlane::getChipsNumber(void)
{
    std::string xChipsNumber = thisNode_.toElement().attribute("XRocs").toStdString();
    std::string yChipsNumber = thisNode_.toElement().attribute("YRocs").toStdString();
    return std::make_pair(xChipsNumber,yChipsNumber);
}

//////////////////////////////////////////////////////////////////////////////////////
std::pair<std::string,std::string> XmlPlane::getCellPitches(void)
{
    std::string xCellPitch = thisNode_.toElement().attribute("XPitch").toStdString();
    std::string yCellPitch = thisNode_.toElement().attribute("YPitch").toStdString();
    return std::make_pair(xCellPitch,yCellPitch);
}

//////////////////////////////////////////////////////////////////////////////////////
std::string XmlPlane::getZRotation(void)
{
    return thisNode_.toElement().attribute("ZRotation").toStdString();
}

//////////////////////////////////////////////////////////////////////////////////////
std::string XmlPlane::getZPosition(void)
{
    return thisNode_.toElement().attribute("ZPosition").toStdString();
}

//////////////////////////////////////////////////////////////////////////////////////
std::string XmlPlane::getName(void)
{
    return thisNode_.toElement().attribute("Name").toStdString();
}

//////////////////////////////////////////////////////////////////////////////////////
std::string XmlPlane::getId(void)
{
    return thisNode_.toElement().attribute("Id").toStdString();
}

//////////////////////////////////////////////////////////////////////////////////////
std::string XmlPlane::getCalibration(void)
{
    return thisNode_.toElement().attribute("Calibration").toStdString();
}

//////////////////////////////////////////////////////////////////////////////////////
std::string XmlPlane::getCalibPlotsFile (void)
{
    return getCalibNode().toElement().elementsByTagName("File").at(0).toElement().text().toStdString();
}

//////////////////////////////////////////////////////////////////////////////////////
bool XmlPlane::useCalibrations(void)
{
    std::string use = getCalibNode().toElement().attribute("Use").toStdString();
    if(use == "yes")
        return true;
    else if (use == "no")
        return false;
    else
        return false;
}
