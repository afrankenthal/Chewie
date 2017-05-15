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

#include "WindowsManager.h"
#include "PlanesMapping.h"
#include "Window.h"
#include "AnalysisManager.h"
#include "HistogramWindow.h"
#include "Data.h"
#include "MessageTools.h"
#include "XmlParser.h"
#include "XmlPlane.h"
#include "XmlWindow.h"

#include <TTree.h>
#include <TThread.h>
#include <TROOT.h>

#include <sstream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowsManager::WindowsManager(AnalysisManager* analysisManager, int nOfThreads) :
    Analysis            (analysisManager,
                         nOfThreads      )
  ,theAnalysisManager_ (analysisManager    )
  ,thePlanesMapping_   (new PlanesMapping())
  ,theXmlParser_       (0                  )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowsManager::~WindowsManager()
{
    destroy();
    delete thePlanesMapping_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowsManager::destroy(void)
{
    for(std::vector<Window*>::iterator it=windows_.begin(); it!=windows_.end(); it++)
    {
        (*it)->setDoNotDelete(Analysis::fDoNotDelete_);
        delete *it;
    }
    windows_.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowsManager::beginJob(void)
{
    STDLINE("Beginning WindowsManager",ACGreen) ;
    theXmlParser_      = theAnalysisManager_->getXmlParser();
    destroy();
    theAnalysisManager_->mkdir("Windows");
    int lowerCol, higherCol, lowerRow, higherRow;
    for(unsigned int w=0; w<thePlanesMapping_->getNumberOfPlanes(); w++)
    {
        //std::cout << __PRETTY_FUNCTION__ << thePlanesMapping_->getPlaneName(w) << std::endl;
        //int xChipsNumber = atoi((((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getChipsNumber()).first).c_str());
        //int yChipsNumber = atoi((((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getChipsNumber()).second).c_str());
        //int nBinsX = xChipsNumber*52;
        //int nBinsY = yChipsNumber*80;
        //if(thePlanesMapping_->getStation(w) >= 5)
        //{
        //    nBinsX = 640;
        //    nBinsY = 640;
        //}
        lowerCol  = atoi(((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getWindow()->getLowerCol()).c_str());
        higherCol = atoi(((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getWindow()->getHigherCol()).c_str());
        lowerRow  = atoi(((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getWindow()->getLowerRow()).c_str());
        higherRow = atoi(((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getWindow()->getHigherRow()).c_str());

        theAnalysisManager_->cd("/Windows");
        theAnalysisManager_->mkdir(thePlanesMapping_->getPlaneName(w));
        theAnalysisManager_->cd("/Windows/" + thePlanesMapping_->getPlaneName(w));

        windows_.push_back(new HistogramWindow(thePlanesMapping_->getPlaneName(w), lowerCol, higherCol, lowerRow, higherRow, runNumberEntries_));
    }
    STDLINE("Done",ACGreen) ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowsManager::analyze(const Data& data, int )//threadNumber)//WARNING: You can't change this name (threadNumber) or the MACRO THREAD won't compile
{
    int lowerCol, higherCol, lowerRow, higherRow;
// std::stringstream ss ;
// ss << "NumberOfPlanes " << thePlanesMapping_->getNumberOfPlanes() ;
// STDLINE(ss.str(),ACWhite) ;
    for(unsigned int w=0; w<thePlanesMapping_->getNumberOfPlanes(); w++)
    {
        lowerCol  = atoi(((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getWindow()->getLowerCol()).c_str());
        higherCol = atoi(((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getWindow()->getHigherCol()).c_str());
        lowerRow  = atoi(((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getWindow()->getLowerRow()).c_str());
        higherRow = atoi(((theXmlParser_->getPlanes())[thePlanesMapping_->getPlaneName(w)]->getWindow()->getHigherRow()).c_str());
// ss.str("") ;
// ss << lowerCol << " " << higherCol << " " << lowerRow << " " << higherRow ;
// STDLINE(ss.str(),ACWhite) ;
        windows_[w]->calculateWindow(w,data,lowerCol,higherCol,lowerRow,higherRow);
    }
//     STDLINE("Done",ACGreen) ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowsManager::endJob(void)
{
    STDLINE("Ending WindowsManager",ACGreen) ;
    for(unsigned int w=0; w<thePlanesMapping_->getNumberOfPlanes(); w++)
    {
        theXmlParser_->getPlanes()[thePlanesMapping_->getPlaneName(w)]->getWindow()->setRemovedPixels();
        std::vector< std::map<std::string,int> > removedPixels = theXmlParser_->getPlanes()[thePlanesMapping_->getPlaneName(w)]->getWindow()->getRemovedPixels();
        for(unsigned int p=0; p<removedPixels.size(); p++)
        {
            windows_[w]->removePixel(removedPixels[p]["col"],removedPixels[p]["row"]);
        }
        windows_[w]->calculateTimeEfficiency();
        windows_[w]->sumThreaded();
    }
    STDLINE("Done",ACGreen) ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Window* WindowsManager::getWindow(std::string plane) const
{
    if(thePlanesMapping_->getPlanePosition(plane) >= 0)
        return windows_[thePlanesMapping_->getPlanePosition(plane)];
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Window* WindowsManager::getWindow(int plane) const
{
    if(plane < (int)windows_.size())
        return windows_[plane];
    return 0;
}
