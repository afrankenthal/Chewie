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

#include "XmlAnalysis.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////
XmlAnalysis::XmlAnalysis(QDomNode& node)
{
    thisNode_ = node;
}

///////////////////////////////////////////////////////////////////////////////////////////
std::string XmlAnalysis::getType(void)
{
    return thisNode_.toElement().attribute("Type").toStdString();
}

///////////////////////////////////////////////////////////////////////////////////////////
bool XmlAnalysis::isToRun(void)
{
    std::string isToRun = thisNode_.toElement().attribute("IsToRun").toStdString();

    if(isToRun=="yes")
        return true;
    else if(isToRun=="no")
        return false;
    else
        return false;
}
///////////////////////////////////////////////////////////////////////////////////////////
bool XmlAnalysis::doFits(void)
{
    std::string doFits = thisNode_.toElement().attribute("DoFits").toStdString();

    if(doFits=="yes")
        return true;
    else if(doFits=="no")
        return false;
    else
        return false;
}

///////////////////////////////////////////////////////////////////////////////////////////
bool XmlAnalysis::applyStandardCuts(void)
{
    std::string applyStandardCut = thisNode_.toElement().attribute("StandardCut").toStdString();

    if(applyStandardCut=="yes")
        return true;
    else if(applyStandardCut=="no")
        return false;
    else
        return false;
}

///////////////////////////////////////////////////////////////////////////////////////////
std::string XmlAnalysis::getCut(std::string cutType)
{
    QDomNodeList cutsList = thisNode_.toElement().elementsByTagName("Cut");
    for(int c=0; c<cutsList.size(); c++)
    {
        if(cutType == cutsList.at(c).toElement().attribute("CutType").toStdString())
            return (cutsList.at(c).toElement().attribute("CutString")).toStdString();
    }
    return "";
}

///////////////////////////////////////////////////////////////////////////////////////////
QDomNode& XmlAnalysis::getCutNode(std::string cutType)
{
    QDomNodeList cutsList = thisNode_.toElement().elementsByTagName("Cut");
    for(int c=0; c<cutsList.size(); c++)
    {
        cutNode_ = cutsList.at(c);
        if(cutType == cutsList.at(c).toElement().attribute("CutType").toStdString())
            return cutNode_;
    }
    return cutNode_;
}

///////////////////////////////////////////////////////////////////////////////////////////
void XmlAnalysis::setCutsList(void)
{
    QDomNodeList cutsList = thisNode_.toElement().elementsByTagName("Cut");

    if(cutsList.size()==0)
        return;

    for(int c=0; c<cutsList.size(); c++)
        cutsList_[cutsList.at(c).toElement().attribute("CutType").toStdString()] = cutsList.at(c).toElement().attribute("CutString").toStdString();
}

///////////////////////////////////////////////////////////////////////////////////////////
std::pair<bool,bool> XmlAnalysis::isParToLimit(int parID)
{
    QDomNode calibCut = thisNode_.toElement().elementsByTagName("CalibCut").at(0);
    QDomNode par = calibCut.toElement().elementsByTagName("Par").at(parID);
    QDomNode min = par.toElement().elementsByTagName("Min").at(0);
    QDomNode max = par.toElement().elementsByTagName("Max").at(0);

    bool limitMin = false;
    bool limitMax = false;

    if(min.toElement().text()=="X")
        limitMin = false;
    else
        limitMin = true;

    if(max.toElement().text()=="X")
        limitMax = false;
    else
        limitMax = true;

    return std::make_pair(limitMin,limitMax);
}

///////////////////////////////////////////////////////////////////////////////////////////
std::pair<std::string,std::string> XmlAnalysis::getParLimits(int parID)
{
    QDomNode calibCut = thisNode_.toElement().elementsByTagName("CalibCut").at(0);
    QDomNode par = calibCut.toElement().elementsByTagName("Par").at(parID);
    QDomNode min = par.toElement().elementsByTagName("Min").at(0);
    QDomNode max = par.toElement().elementsByTagName("Max").at(0);

    return std::make_pair(min.toElement().text().toStdString(),max.toElement().text().toStdString());
}

///////////////////////////////////////////////////////////////////////////////////////////
std::pair<double,double> XmlAnalysis::getParLimits(int parID, bool flag)
{
    if(!flag)
        return std::make_pair(0,0);

    QDomNode calibCut = thisNode_.toElement().elementsByTagName("CalibCut").at(0);
    QDomNode par = calibCut.toElement().elementsByTagName("Par").at(parID);
    QDomNode min = par.toElement().elementsByTagName("Min").at(0);
    QDomNode max = par.toElement().elementsByTagName("Max").at(0);

    return std::make_pair(min.toElement().text().toDouble(),max.toElement().text().toDouble());
}

///////////////////////////////////////////////////////////////////////////////////////////
std::string XmlAnalysis::getMinHits(void)
{
    return thisNode_.toElement().elementsByTagName("MinHits").at(0).toElement().text().toStdString();
}

///////////////////////////////////////////////////////////////////////////////////////////
int XmlAnalysis::getPixelMinimumCharge(void)
{
    return thisNode_.toElement().elementsByTagName("PixelMinimumCharge").at(0).toElement().text().toInt();
}

///////////////////////////////////////////////////////////////////////////////////////////
int XmlAnalysis::getPixelMaximumCharge(void)
{
    return thisNode_.toElement().elementsByTagName("PixelMaximumCharge").at(0).toElement().text().toInt();
}

///////////////////////////////////////////////////////////////////////////////////////////
int XmlAnalysis::getClusterMinimumCharge(void)
{
    return thisNode_.toElement().elementsByTagName("ClusterMinimumCharge").at(0).toElement().text().toInt();
}

///////////////////////////////////////////////////////////////////////////////////////////
int XmlAnalysis::getClusterMaximumCharge(void)
{
    return thisNode_.toElement().elementsByTagName("ClusterMaximumCharge").at(0).toElement().text().toInt();
}

//////////////////////////////////////////////////////////////////////////////////////////
int XmlAnalysis::getPriority(void)
{
    return thisNode_.toElement().attribute("Priority").toInt();
}


//////////////////////////////////////////////////////////////////////////////////////////
std::string XmlAnalysis::getModule(void)
{
    return thisNode_.toElement().attribute("Module").toStdString();
}
//////////////////////////////////////////////////////////////////////////////////////////
bool XmlAnalysis::excludeBadPlanes(void)
{
    std::string exclude = thisNode_.toElement().attribute("ExcludeBadPlanes").toStdString();

    if(exclude=="yes")
        return true;
    else if(exclude=="no")
        return false;
    else
        return false;
}
//////////////////////////////////////////////////////////////////////////////////////////
int XmlAnalysis::getBadPlanesCut(void)
{
    return thisNode_.toElement().attribute("BadPlanesHits").toInt();
}
