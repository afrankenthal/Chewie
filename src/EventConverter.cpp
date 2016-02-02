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

#include "EventConverter.h"
#include "Data.h"
#include "Geometry.h"
#include "Event.h"
#include "EventHeader.h"

#include <TFile.h>
#include <TTree.h>
#include <TThread.h>

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////////////
EventConverter::EventConverter(EventManager* eventManager, int nOfThreads) :
    Threader        (nOfThreads)
  , theEventManager_(eventManager)
  , theHeader_      (0)
  , theGeometry_    (0)
  , counter_        (0)
{
    for(int t=0; t<Threader::nOfThreads_; t++)
    {
        eventVector_.push_back(Event());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
int EventConverter::execute(int threadNumber)
{
    TThread::Lock();
    int event = theEventManager_->getCurrentEvent(eventVector_[threadNumber]);
    TThread::UnLock();
    if(event >= 0)
        convert(eventVector_[threadNumber],event);
    return event;
}

///////////////////////////////////////////////////////////////////////////////////////////
int EventConverter::runConverter()
{
    startThreads();
    join();
    stopThreads();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
void EventConverter::beginJob()
{
    theHeader_    = theEventManager_->getEventHeaderPointer();
    theGeometry_  = theEventManager_->getGeometryPointer();
    outTree_ = new TTree("CaptanTrack","The Track Tree");
    theData_.branch(outTree_);
}

///////////////////////////////////////////////////////////////////////////////////////////
void EventConverter::convert(Event& event,int e)
{
    STDLINE("Converting...", ACWhite) ;
    Event::plaqMapDef  	                          & theRawData	                        = event.getRawData                            ();
    Event::clustersMapDef	                      & clusters  	                        = event.getClusters                           ();
    Event::clustersHitsMapDef                     & clustersHits	                    = event.getClustersHits                       ();
    Event::residualsMapDef	                      & fittedTrackResiduals                = event.getFittedTrackResiduals               ();
    Event::trackCandidatesDef                     & trackCandidates	                    = event.getTrackCandidates                    ();
    Event::fittedTracksDef                        & fittedTracks                        = event.getFittedTracks                       ();
    Event::fittedTracksCovarianceDef              & fittedTracksCovariance              = event.getFittedTracksCovariance             ();
    Event::chi2VectorDef	                      & fittedTracksChi2                    = event.getFittedTracksChi2                   ();
    Event::unconstrainedFittedTracksDef           & unconstrainedFittedTracks           = event.getUnconstrainedFittedTracks          ();
    Event::unconstrainedFittedTracksCovarianceDef & unconstrainedFittedTracksCovariance = event.getUnconstrainedFittedTracksCovariance();
    Event::unconstrainedChi2VectorDef	          & unconstrainedFittedTracksChi2       = event.getUnconstrainedFittedTracksChi2      ();

    if( trackCandidates.size() == 0 )
        return ;

    bool                     isGood          = false ;
    int                      clusterID       =     0 ;
    unsigned int             size            =     0 ;
    float                    row             =     0 ;
    float                    col             =     0 ;
    int                      rowPredicted    =     0 ;
    int                      colPredicted    =     0 ;
    unsigned int             pixelRow        =     0 ;
    unsigned int             pixelCol        =     0 ;
    double                   xp,yp,zp,xRes,yRes      ;
    double                   xPixelPitch     =     0 ;
    double                   yPixelPitch     =     0 ;
    double                   xPixelCenter    =     0 ;
    double                   yPixelCenter    =     0 ;
    double                   zPixelCenter    =     0 ;
    std::pair<int,int>       rc                      ;
    std::pair<double,double> xyErr                   ;
    std::list<unsigned int>  nRow                    ;
    std::list<unsigned int>  nCol                    ;
    Detector*                detector        =     0 ;
    ROC*                     ROC             =     0 ;
    std::string              planeName               ;
    std::stringstream        ss                      ;
    Data                     dataVector[trackCandidates.size()];

    if(trackCandidates.size() != fittedTracks.size())
        STDLINE("There is something very wrong in the track sizes", ACRed);
    
    for(unsigned int t=0; t<trackCandidates.size(); t++)
    {
        STDLINE("",ACWhite) ;
        dataVector[t].setEventNumber      (e);
        dataVector[t].setRunNumber        (runNumber_); //it is a private variable because it's not taken from Monicelli output, but it is passed before beginJob in method startConverter of EventManager.cpp
        dataVector[t].setNumberOfTracks   (trackCandidates.size());
        dataVector[t].setTrackNumber      (t);
        dataVector[t].setNdof             (2*(trackCandidates[t].size()-2));
        dataVector[t].setChi2             (fittedTracksChi2[t]);
        dataVector[t].setXIntercept       (fittedTracks[t][1]*10);
        dataVector[t].setXSigmaIntercept  (sqrt(fittedTracksCovariance[t](1,1))*10);
        dataVector[t].setYIntercept       (fittedTracks[t][3]*10);
        dataVector[t].setYSigmaIntercept  (sqrt(fittedTracksCovariance[t](3,3))*10);
        dataVector[t].setXSlope           (fittedTracks[t][0]);
        dataVector[t].setXSigmaSlope      (sqrt(fittedTracksCovariance[t](0,0)));
        dataVector[t].setYSlope           (fittedTracks[t][2]);
        dataVector[t].setYSigmaSlope      (sqrt(fittedTracksCovariance[t](2,2)));

        int nTelescopeHits  = 0;
        int clustersSizeLE2 = 0 ;
        STDLINE("",ACWhite) ;
        for(unsigned int p=0; p<thePlanesMapping_.getNumberOfPlanes(); p++)
        {
            ss_.str(""); ss_<<"Plane: " << p;  STDLINE(ss_.str().c_str(),ACWhite) ;
            nRow.clear();
            nCol.clear();
            row       = 0;
            col       = 0;
            planeName = thePlanesMapping_.getMonicelliPlaneName(p);
            STDLINE("",ACWhite) ;
            detector  = theGeometry_->getDetector(thePlanesMapping_.getStation(p), thePlanesMapping_.getPlaquette(p));
            STDLINE("",ACWhite) ;

            if(detector == 0)
                continue;
/*
            STDLINE("",ACWhite) ;
            std::cout << "size: " << unconstrainedFittedTracksChi2.size() << std::endl ;
            for( std::vector<std::map<std::string, Event::vectorDef> >::iterator it = unconstrainedFittedTracksChi2.begin();
                                                                                 it!= unconstrainedFittedTracksChi2.end()  ;
                                                                               ++it)
            {
              std::cout << "   --- > " << *it.first << std::endl ;
            }
*/
            dataVector[t].setChi2Unconstrained           (unconstrainedFittedTracksChi2[t][planeName]                    ,p);
            dataVector[t].setXInterceptUnconstrained     (unconstrainedFittedTracks[t][planeName][1]*10                  ,p);
            dataVector[t].setXSigmaInterceptUnconstrained(sqrt(unconstrainedFittedTracksCovariance[t][planeName](1,1))*10,p);
            dataVector[t].setYInterceptUnconstrained     (unconstrainedFittedTracks[t][planeName][3]*10                  ,p);
            dataVector[t].setYSigmaInterceptUnconstrained(sqrt(unconstrainedFittedTracksCovariance[t][planeName](3,3))*10,p);
            dataVector[t].setXSlopeUnconstrained         (unconstrainedFittedTracks[t][planeName][0]                     ,p);
            dataVector[t].setXSigmaSlopeUnconstrained    (sqrt(unconstrainedFittedTracksCovariance[t][planeName](0,0))   ,p);
            dataVector[t].setYSlopeUnconstrained         (unconstrainedFittedTracks[t][planeName][2]                     ,p);
            dataVector[t].setYSigmaSlopeUnconstrained    (sqrt(unconstrainedFittedTracksCovariance[t][planeName](2,2))   ,p);
            STDLINE("",ACWhite) ;
            if(trackCandidates[t].find(planeName) != trackCandidates[t].end())
            {

                clusterID = (int)trackCandidates[t][planeName]["cluster ID"];

                if(thePlanesMapping_.getPlaneName(p).find("Dut") != std::string::npos)
                {
                    dataVector[t].setNdofUnconstrained(2*(trackCandidates[t].size()-2),p);
                    xyErr = detector->getTrackErrorsOnPlane(fittedTracks[t],fittedTracksCovariance[t]);
                    dataVector[t].setXErrorPredictedGlobal(sqrt(xyErr.first )*10,p);
                    dataVector[t].setYErrorPredictedGlobal(sqrt(xyErr.second)*10,p);
                    detector->flipBackDistance(&xyErr.first,&xyErr.second);
                    dataVector[t].setXErrorPredictedLocal(sqrt(fabs(xyErr.first ))*10,p);
                    dataVector[t].setYErrorPredictedLocal(sqrt(fabs(xyErr.second))*10,p);
                }
                else
                {
                    dataVector[t].setNdofUnconstrained(2*(trackCandidates[t].size()-1-2),p);
                    dataVector[t].setXErrorPredictedGlobal(-1,p);
                    dataVector[t].setYErrorPredictedGlobal(-1,p);
                    dataVector[t].setXErrorPredictedLocal (-1,p);
                    dataVector[t].setYErrorPredictedLocal (-1,p);
                    ++nTelescopeHits;
                    if(trackCandidates[t][planeName]["size"] == 1)
                        ++clustersSizeLE2;
                    else if(trackCandidates[t][planeName]["size"] == 2
                            && (clustersHits[planeName][clusterID][0]["row"] == clustersHits[planeName][clusterID][1]["row"]
                                || clustersHits[planeName][clusterID][0]["col"] == clustersHits[planeName][clusterID][1]["col"]))
                        ++clustersSizeLE2;
                }




                dataVector[t].setHasHit        (true,p);
                //std::cout<<"datatype 1 "<<      clusters[planeName][clusterID]["dataType"] <<" plane "<<p<<" name "<<planeName<<std::endl;
                dataVector[t].setDataType((int)clusters[planeName][clusterID]["dataType"],p);
                dataVector[t].setBelongsToTrack(true,p);
                dataVector[t].setClusterSize   ((int)trackCandidates[t][planeName]["size"], p);
                dataVector[t].setClusterCharge ((int)clusters[planeName][clusterID]["charge"], p);

                size = (unsigned int)clustersHits[planeName][clusterID].size();
                for(unsigned int h=0; h<size; h++)
                {

                    pixelRow = clustersHits[planeName][clusterID][h]["row"];
                    pixelCol = clustersHits[planeName][clusterID][h]["col"];

                    row += (float)pixelRow;
                    col += (float)pixelCol;
                    nRow.push_back(pixelRow);
                    nCol.push_back(pixelCol);
                    if(size<=4)
                    {
                        dataVector[t].setClusterPixelRow(pixelRow,h,p);
                        dataVector[t].setClusterPixelCol(pixelCol,h,p);
                        dataVector[t].setClusterPixelCharge(clustersHits[planeName][clusterID][h]["charge"],h,p);
                        xPixelCenter = detector->getPixelCenterLocalX(pixelCol);
                        yPixelCenter = detector->getPixelCenterLocalY(pixelRow);
                        dataVector[t].setXClusterPixelCenterLocal(xPixelCenter*10,h,p);
                        dataVector[t].setYClusterPixelCenterLocal(yPixelCenter*10,h,p);
                        detector->fromLocalToGlobal(&xPixelCenter,&yPixelCenter,&zPixelCenter);
                        dataVector[t].setXClusterPixelCenterGlobal(xPixelCenter*10,h,p);
                        dataVector[t].setYClusterPixelCenterGlobal(yPixelCenter*10,h,p);
                        ROC = detector->convertPixelToROC(&pixelRow,&pixelCol);
                        dataVector[t].setIsPixelCalibrated(ROC->isPixelCalibrated(pixelRow,pixelCol),h,p);

                        if(!ROC->isPixelCalibrated(pixelRow,pixelCol) && thePlanesMapping_.getPlaneName(p).find("Dut") != std::string::npos)
                        {
                            ss.str("");
                            ss << "WARNING: fit failed for detector " << planeName;
                            ss << " at row " << pixelRow << "," << " col " << pixelCol;
                            ss << " of chip " << ROC->getID();
                            STDLINE(ss.str(),ACRed);
                        }

                    }
                }

                nRow.sort();
                nCol.sort();

                nRow.unique();
                nCol.unique();
                dataVector[t].setMeanCol        (col/size,p);
                if((int)clusters[planeName][clusterID]["dataType"] == 1)//Strip case
                    dataVector[t].setMeanRow(0,p);
                 else
                    dataVector[t].setMeanRow(row/size,p);
                dataVector[t].setNumberOfCols   (nCol.size(),p);
                dataVector[t].setNumberOfRows   (nRow.size(),p);
                dataVector[t].setXMeasuredLocal (clusters[planeName][clusterID]["x"]*10,p);
                dataVector[t].setYMeasuredLocal (clusters[planeName][clusterID]["y"]*10,p);
                dataVector[t].setXMeasuredGlobal(trackCandidates[t][planeName]["x"]*10,p);
                dataVector[t].setYMeasuredGlobal(trackCandidates[t][planeName]["y"]*10,p);

                dataVector[t].setXErrorMeasuredLocal (clusters[planeName][clusterID]["xErr"]*10,p);
                dataVector[t].setYErrorMeasuredLocal (clusters[planeName][clusterID]["yErr"]*10,p);
                dataVector[t].setXErrorMeasuredGlobal(trackCandidates[t][planeName]["xErr"]*10,p);
                dataVector[t].setYErrorMeasuredGlobal(trackCandidates[t][planeName]["yErr"]*10,p);

                xRes = fittedTrackResiduals[t][planeName].first*10;
                yRes = fittedTrackResiduals[t][planeName].second*10;
                dataVector[t].setXTrackResidualGlobal(xRes,p);
                dataVector[t].setYTrackResidualGlobal(yRes,p);
                detector->flipBackDistance(&xRes,&yRes);
                dataVector[t].setXTrackResidualLocal (xRes,p);
                dataVector[t].setYTrackResidualLocal (yRes,p);

                detector->getPredictedGlobal(fittedTracks[t],xp,yp,zp);
                dataVector[t].setXPredictedGlobal(xp*10,p);
                dataVector[t].setYPredictedGlobal(yp*10,p);

                detector->fromGlobalToLocal(&xp,&yp,&zp);
                dataVector[t].setXPredictedLocal(xp*10,p);
                dataVector[t].setYPredictedLocal(yp*10,p);

                rc = detector->getPixelCellFromLocal(xp,yp);
                rowPredicted = rc.first;
                colPredicted = rc.second;

                dataVector[t].setColPredicted(colPredicted,p);
                dataVector[t].setRowPredicted(rowPredicted,p);

                if(colPredicted == -1 || rowPredicted == -1)
                {
                    dataVector[t].setIsInDetector(false,p);
                    //continue;
                }
                else
                {
                    dataVector[t].setIsInDetector(true,p);

                    xPixelPitch = detector->getPixelPitchLocalX((unsigned int)colPredicted)*10;
                    yPixelPitch = detector->getPixelPitchLocalY((unsigned int)rowPredicted)*10;

                    dataVector[t].setXPitchLocal(xPixelPitch,p);
                    dataVector[t].setYPitchLocal(yPixelPitch,p);
                    detector->flipDistance(&xPixelPitch,&yPixelPitch);
                    dataVector[t].setXPitchGlobal(fabs(xPixelPitch),p);
                    dataVector[t].setYPitchGlobal(fabs(yPixelPitch),p);

                    xPixelCenter = detector->getPixelCenterLocalX((unsigned int)colPredicted);
                    yPixelCenter = detector->getPixelCenterLocalY((unsigned int)rowPredicted);

                    detector->getPredictedLocal(fittedTracks[t],xp,yp);
                    xRes = (xp - xPixelCenter)*10;
                    yRes = (yp - yPixelCenter)*10;
                    dataVector[t].setXPixelResidualLocal(xRes,p);
                    dataVector[t].setYPixelResidualLocal(yRes,p);

                    detector->flipDistance(&xRes,&yRes);
                    dataVector[t].setXPixelResidualGlobal(xRes,p);
                    dataVector[t].setYPixelResidualGlobal(yRes,p);
                }

                detector->getPredictedLocal(unconstrainedFittedTracks[t][planeName],xp,yp);
                rc = detector->getPixelCellFromLocal(xp,yp);
                rowPredicted = rc.first;
                colPredicted = rc.second;

                if(colPredicted == -1 || rowPredicted == -1)
                {
                    dataVector[t].setIsInDetectorUnconstrained(false,p);
                    //continue;
                }
                else
                {
                    xPixelPitch = detector->getPixelPitchLocalX((unsigned int)colPredicted)*10;
                    yPixelPitch = detector->getPixelPitchLocalY((unsigned int)rowPredicted)*10;
                    dataVector[t].setXPixelPitchLocalUnconstrained(xPixelPitch,p);
                    dataVector[t].setYPixelPitchLocalUnconstrained(yPixelPitch,p);
                    xPixelCenter = detector->getPixelCenterLocalX((unsigned int)colPredicted);
                    yPixelCenter = detector->getPixelCenterLocalY((unsigned int)rowPredicted);
                    xRes = (xp - xPixelCenter)*10;
                    yRes = (yp - yPixelCenter)*10;
                    dataVector[t].setXPixelResidualLocalUnconstrained(xRes,p);
                    dataVector[t].setYPixelResidualLocalUnconstrained(yRes,p);
                }

            }
            else
            {
                dataVector[t].setNdofUnconstrained(2*(trackCandidates[t].size()-2),p);
                if(thePlanesMapping_.getPlaneName(p).find("Dut") != std::string::npos)
                {
                    xyErr = detector->getTrackErrorsOnPlane(fittedTracks[t],fittedTracksCovariance[t]);
                    dataVector[t].setXErrorPredictedGlobal(sqrt(xyErr.first)*10 ,p);
                    dataVector[t].setYErrorPredictedGlobal(sqrt(xyErr.second)*10,p);
                    detector->flipBackDistance(&xyErr.first,&xyErr.second);
                    dataVector[t].setXErrorPredictedLocal(sqrt(fabs(xyErr.first ))*10,p);
                    dataVector[t].setYErrorPredictedLocal(sqrt(fabs(xyErr.second))*10,p);
                }
                else
                {
                    dataVector[t].setXErrorPredictedGlobal(-1,p);
                    dataVector[t].setYErrorPredictedGlobal(-1,p);
                    dataVector[t].setXErrorPredictedLocal (-1,p);
                    dataVector[t].setYErrorPredictedLocal (-1,p);
                }

                dataVector[t].setHasHit        (false,p);
                //std::cout<<"datatype 2 "<<      clusters[planeName][clusterID]["dataType"] <<" plane "<<p<<" name "<<planeName<<std::endl;
                dataVector[t].setDataType((int)-1,p);
                dataVector[t].setBelongsToTrack(false,p);

                detector->getPredictedGlobal(fittedTracks[t],xp,yp,zp);
                dataVector[t].setXPredictedGlobal(xp*10,p);
                dataVector[t].setYPredictedGlobal(yp*10,p);
                detector->fromGlobalToLocal(&xp,&yp,&zp);
                dataVector[t].setXPredictedLocal(xp*10,p);
                dataVector[t].setYPredictedLocal(yp*10,p);

                rc = detector->getPixelCellFromLocal(xp,yp);
                rowPredicted = rc.first;
                colPredicted = rc.second;

                dataVector[t].setMeanCol (colPredicted,p);
                dataVector[t].setMeanRow (rowPredicted,p);

                dataVector[t].setColPredicted(colPredicted,p);
                dataVector[t].setRowPredicted(rowPredicted,p);

                if(colPredicted == -1 || rowPredicted == -1)
                {
                    dataVector[t].setIsInDetector(false,p);
//                    continue;
                }
                else
                {
                    dataVector[t].setIsInDetector(true,p);

                    xPixelPitch = detector->getPixelPitchLocalX((unsigned int)colPredicted)*10;
                    yPixelPitch = detector->getPixelPitchLocalY((unsigned int)rowPredicted)*10;

                    dataVector[t].setXPitchLocal(xPixelPitch,p);
                    dataVector[t].setYPitchLocal(yPixelPitch,p);
                    detector->flipDistance(&xPixelPitch,&yPixelPitch);
                    dataVector[t].setXPitchGlobal(fabs(xPixelPitch),p);
                    dataVector[t].setYPitchGlobal(fabs(yPixelPitch),p);

                    xPixelCenter = detector->getPixelCenterLocalX((unsigned int)colPredicted);
                    yPixelCenter = detector->getPixelCenterLocalY((unsigned int)rowPredicted);

                    detector->getPredictedLocal(fittedTracks[t],xp,yp);
                    xRes = (xp - xPixelCenter)*10;
                    yRes = (yp - yPixelCenter)*10;
                    dataVector[t].setXPixelResidualLocal(xRes,p);
                    dataVector[t].setYPixelResidualLocal(yRes,p);

                    detector->flipDistance(&xRes,&yRes);
                    dataVector[t].setXPixelResidualGlobal(xRes,p);
                    dataVector[t].setYPixelResidualGlobal(yRes,p);

                    isGood = false;
                    for(std::vector<std::map<std::string,int> >::iterator hits = theRawData[planeName].begin(); hits != theRawData[planeName].end(); hits++)
                    {
                        if((abs(colPredicted - (*hits)["col"]) <= 1) && (abs(rowPredicted - (*hits)["row"]) <= 1))
                        {
                            clusterID = -1;

                            for(Event::aClusterHitsMapDef::iterator itC = clustersHits[planeName].begin(); itC != clustersHits[planeName].end() && clusterID == -1; itC++)
                            {
                                if(isGood)
                                    break;

                                for(Event::hitsDef::iterator itH = itC->second.begin(); itH != itC->second.end() && clusterID == -1; itH++)
                                {
                                    if((*hits)["col"] == (*itH)["col"] && (*hits)["row"] == (*itH)["row"])
                                    {
                                        clusterID = itC->first;
                                        dataVector[t].setHasHit(true,p);
                                        //dataVector[t].setDataType((int)clusters[planeName][clusterID]["dataType"],p);
                                        isGood = true;
                                        break;
                                    }
                                }
                            }

                            size = (unsigned int)clustersHits[planeName][clusterID].size();

                            dataVector[t].setClusterSize  (size,p);
                            dataVector[t].setClusterCharge((int)clusters[planeName][clusterID]["charge"],p);

                            row = 0;
                            col = 0;
                            nRow.clear();
                            nCol.clear();

                            for(unsigned int h=0; h<size; h++)
                            {
                                pixelRow = clustersHits[planeName][clusterID][h]["row"];
                                pixelCol = clustersHits[planeName][clusterID][h]["col"];
                                row += (float)pixelRow;
                                col += (float)pixelCol;
                                nRow.push_back(pixelRow);
                                nCol.push_back(pixelCol);
                                if(size<=4)
                                {
                                    dataVector[t].setClusterPixelRow(pixelRow,h,p);
                                    dataVector[t].setClusterPixelCol(pixelCol,h,p);
                                    dataVector[t].setClusterPixelCharge(clustersHits[planeName][clusterID][h]["charge"],h,p);
                                    xPixelCenter = detector->getPixelCenterLocalX(pixelCol);
                                    yPixelCenter = detector->getPixelCenterLocalY(pixelRow);
                                    dataVector[t].setXClusterPixelCenterLocal(xPixelCenter*10,h,p);
                                    dataVector[t].setYClusterPixelCenterLocal(yPixelCenter*10,h,p);
                                    detector->fromLocalToGlobal(&xPixelCenter,&yPixelCenter,&zPixelCenter);
                                    dataVector[t].setXClusterPixelCenterGlobal(xPixelCenter*10,h,p);
                                    dataVector[t].setYClusterPixelCenterGlobal(yPixelCenter*10,h,p);
                                    ROC = detector->convertPixelToROC(&pixelRow,&pixelCol);
                                    dataVector[t].setIsPixelCalibrated(ROC->isPixelCalibrated(pixelRow,pixelCol),h,p);
                                }
                            }

                            nRow.sort();
                            nCol.sort();

                            nRow.unique();
                            nCol.unique();

                            dataVector[t].setMeanCol (col/size,p);
                            if((int)clusters[planeName][clusterID]["dataType"] == 1)//Strip case
                                dataVector[t].setMeanRow(0,p);
                            else
                                dataVector[t].setMeanRow(row/size,p);
                            //dataVector[t].setMeanRow (row/size,p);
                            dataVector[t].setNumberOfCols(nCol.size(),p);
                            dataVector[t].setNumberOfRows(nRow.size(),p);
                            break;
                        }
                    }
                }

                detector->getPredictedLocal(unconstrainedFittedTracks[t][planeName],xp,yp);
                rc = detector->getPixelCellFromLocal(xp,yp);
                rowPredicted = rc.first;
                colPredicted = rc.second;

                if(colPredicted == -1 || rowPredicted == -1)
                {
                    dataVector[t].setIsInDetectorUnconstrained(false,p);
                    //continue;
                }
                else
                {
                    xPixelPitch = detector->getPixelPitchLocalX((unsigned int)colPredicted)*10;
                    yPixelPitch = detector->getPixelPitchLocalY((unsigned int)rowPredicted)*10;
                    dataVector[t].setXPixelPitchLocalUnconstrained(xPixelPitch,p);
                    dataVector[t].setYPixelPitchLocalUnconstrained(yPixelPitch,p);
                    xPixelCenter = detector->getPixelCenterLocalX((unsigned int)colPredicted);
                    yPixelCenter = detector->getPixelCenterLocalY((unsigned int)rowPredicted);
                    xRes = (xp - xPixelCenter)*10;
                    yRes = (yp - yPixelCenter)*10;
                    dataVector[t].setXPixelResidualLocalUnconstrained(xRes,p);
                    dataVector[t].setYPixelResidualLocalUnconstrained(yRes,p);
                }
            }
            STDLINE("",ACWhite) ;
        }
        dataVector[t].setNumberOfTelescopeHits(nTelescopeHits);
        dataVector[t].setNumberOfTelescopeClustersSizeLE2(clustersSizeLE2);
        STDLINE("",ACWhite) ;
    }

    STDLINE("",ACWhite) ;
    for(unsigned int t=0; t<trackCandidates.size(); t++)
    {
        TThread::Lock();
        dataVector[t].setEventChewieNumber(outTree_->GetEntries());
        theData_= dataVector[t];
        outTree_->Fill();
        TThread::UnLock();
    }
    STDLINE("",ACWhite) ;

}

///////////////////////////////////////////////////////////////////////////////////////////
void EventConverter::endJob()
{
}
