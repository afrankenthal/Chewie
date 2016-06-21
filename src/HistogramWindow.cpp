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

#include "HistogramWindow.h"
#include "Data.h"
#include "MessageTools.h"

#include <TH2F.h>
#include <TAxis.h>
#include <TTree.h>
#include <TROOT.h>
#include <math.h>

#include <iostream>
#include <sstream>
#include <cassert>

///////////////////////////////////////////////////////////////////////////////////////////
HistogramWindow::HistogramWindow(std::string name, int binXMin, int binXMax, int binYMin, int binYMax, std::map<int,int> runNumberEntries) :
    Window(name)
{
    nBinsX_ = binXMax - binXMin +1;
    nBinsY_ = binYMax - binYMin +1;

    for(std::map<int,int>::iterator runIt = runNumberEntries.begin(); runIt != runNumberEntries.end(); runIt++)
    {


        int EventBinSize = 1000;
        int totalEvents  = runIt->second;

        totalEvents/=EventBinSize;
        totalEvents*=EventBinSize;
        totalEvents+=EventBinSize;//to have a number multiple of EventBinSize

        std::stringstream ss;
        ss.str("");
        ss << runIt->first;

        theHWindow_            .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_Window_Run"+ss.str()).c_str()            ,(name_+"_Window_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));
        theH2TimeWindow_       .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_TimeWindow_H2_Run"+ss.str()).c_str()     ,(name_+"_TimeWindow_H2_Run"+ss.str()).c_str()     ,nBinsX_, binXMin, binXMax+1,totalEvents/EventBinSize,0       ,totalEvents)));
        theH2TimeWindow_norm_  .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_TimeWindow_norm_H2_Run"+ss.str()).c_str(),(name_+"_TimeWindow_norm_H2_Run"+ss.str()).c_str(),nBinsX_, binXMin, binXMax+1,totalEvents/EventBinSize,0       ,totalEvents)));
        theH1TimeWindow_       .insert(std::pair<int,TH1F*> (runIt->first ,new TH1F ((name_ +"_TimeWindow_Run"+ss.str()).c_str()       ,(name_ +"_TimeWindow_Run"+ss.str()).c_str()       ,nBinsX_, binXMin, binXMax+1                                              )));

        theHWindowClusterSize1_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowClusterSize1_Run"+ss.str()).c_str(),(name_+"_WindowClusterSize1_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));
        theHWindowClusterSize2_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowClusterSize2_Run"+ss.str()).c_str(),(name_+"_WindowClusterSize2_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));
        theHWindowClusterSize3_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowClusterSize3_Run"+ss.str()).c_str(),(name_+"_WindowClusterSize3_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));
        theHWindowClusterSize4_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowClusterSize4_Run"+ss.str()).c_str(),(name_+"_WindowClusterSize4_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));



    }
}

///////////////////////////////////////////////////////////////////////////////////////////
HistogramWindow::~HistogramWindow(void)
{
    for(std::map<int,TH2F*>::iterator it=theHWindow_.begin(); it != theHWindow_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=theHWindowClusterSize1_.begin(); it != theHWindowClusterSize1_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=theHWindowClusterSize2_.begin(); it != theHWindowClusterSize2_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=theHWindowClusterSize3_.begin(); it != theHWindowClusterSize3_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=theHWindowClusterSize4_.begin(); it != theHWindowClusterSize4_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH1F*>::iterator it=theH1TimeWindow_.begin(); it != theH1TimeWindow_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=theH2TimeWindow_.begin(); it != theH2TimeWindow_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=theH2TimeWindow_norm_.begin(); it != theH2TimeWindow_norm_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkWindow(float col, float row, int runNumber) const
{
  TAxis* xAxis = theHWindow_.find(runNumber)->second->GetXaxis();
  TAxis* yAxis = theHWindow_.find(runNumber)->second->GetYaxis();

  if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),yAxis->FindBin(row)) != 0) return true;
  return false;
}

///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkWindowAbout(float col, float row, int runNumber, int type) const
{
    TAxis* xAxis = theHWindow_.find(runNumber)->second->GetXaxis() ;
    TAxis* yAxis = theHWindow_.find(runNumber)->second->GetYaxis() ;
    if(type==0)
    {
        if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(row-1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),yAxis->FindBin(row-1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),yAxis->FindBin(row-1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(row+1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),yAxis->FindBin(row+1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),yAxis->FindBin(row+1)) != 0 )
            return true;
        else
            return false;
    }
    else if(type==1)
    {
        if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(0.)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),yAxis->FindBin(0.)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),yAxis->FindBin(0.)) != 0 )

            return true;
        else
            return false;

    }
    else
    {
        std::cout << __PRETTY_FUNCTION__ << "I only know 2 types (0, 1) while this is type: " << type << ". IMPOSSIBLE!" << " row: " << row << " col: " << col << std::endl;
        assert(0);
    }
}

////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkTimeWindow(float col, int eventNumber, int runNumber) const
{

    if(theH1TimeWindow_.find(runNumber)->second->GetBinContent(col) >=  eventNumber)
        return true;
    else
        return false;

}

///////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkTimeWindowAbout(float col, int eventNumber, int runNumber) const
{

    if(theH1TimeWindow_.find(runNumber)->second->GetBinContent(col-1) >=  eventNumber &&
            theH1TimeWindow_.find(runNumber)->second->GetBinContent(col+1) >=  eventNumber &&
            theH1TimeWindow_.find(runNumber)->second->GetBinContent(col  ) >=  eventNumber )
        return true;
    else
        return false;

}

///////////////////////////////////////////////////////////////////////////////////////
void HistogramWindow::calculateWindow(int planeID, const Data& data, int lowerCol, int higherCol, int lowerRow, int higherRow)
{
  float col   = data.getMeanCol(planeID);
  float row   = data.getMeanRow(planeID);
  int   nRow  = data.getNumberOfRows(planeID);
  int   nCol  = data.getNumberOfCols(planeID);
  int   run   = data.getRunNumber();
  int   entry = data.getEventChewieNumber();
  int   size  = data.getClusterSize(planeID);


  if (data.getIsInDetector(planeID) && row >= lowerRow && col >= lowerCol && row <= higherRow && col <= higherCol)
    {
      theH2TimeWindow_norm_.find(run)->second->Fill(col,entry);
      if (data.getHasHit(planeID)) theH2TimeWindow_.find(run)->second->Fill(col,entry);
    }


  if (data.getHasHit(planeID) && data.getIsInDetector(planeID) && row >= lowerRow && col >= lowerCol && row <= higherRow && col <= higherCol)
    {
      if (nRow == 1 && nCol == 1)
	{
	  theHWindow_.find(run)->second->Fill(col,row);

	  if (size == 1)
	    theHWindowClusterSize1_.find(run)->second->Fill(col,row);
	  if (size == 2)
	    theHWindowClusterSize2_.find(run)->second->Fill(col,row);
	  if (size == 3)
	    theHWindowClusterSize3_.find(run)->second->Fill(col,row);
	  if (size == 4)
	    theHWindowClusterSize4_.find(run)->second->Fill(col,row);
	}
      else if (nRow > 1 && nCol == 1)
	{
	  if (ceil(nRow/2.) != nRow/2.) // nRow odd
	    {
	      theHWindow_.find(run)->second->Fill(col,row);

	      if (size == 1)
		theHWindowClusterSize1_.find(run)->second->Fill(col,row);
	      if (size == 2)
		theHWindowClusterSize2_.find(run)->second->Fill(col,row);
	      if (size == 3)
		theHWindowClusterSize3_.find(run)->second->Fill(col,row);
	      if (size == 4)
		theHWindowClusterSize4_.find(run)->second->Fill(col,row);

	      for (int r = 1; r < nRow; r++)
		{
		  if (ceil(r/2.) == r/2.)
		    {
		      if ((row+r/2.) <= higherRow)
			{
			  theHWindow_.find(run)->second->Fill(col,row+r/2.);

			  if (size == 1)
			    theHWindowClusterSize1_.find(run)->second->Fill(col,row+r/2.);
			  if (size == 2)
			    theHWindowClusterSize2_.find(run)->second->Fill(col,row+r/2.);
			  if (size == 3)
			    theHWindowClusterSize3_.find(run)->second->Fill(col,row+r/2.);
			  if (size == 4)
			    theHWindowClusterSize4_.find(run)->second->Fill(col,row+r/2.);
			}

		      if ((row-r/2.) >= lowerRow)
			{
			  theHWindow_.find(run)->second->Fill(col,row-r/2.);

			  if (size == 1)
			    theHWindowClusterSize1_.find(run)->second->Fill(col,row-r/2.);
			  if (size == 2)
			    theHWindowClusterSize2_.find(run)->second->Fill(col,row-r/2.);
			  if (size == 3)
			    theHWindowClusterSize3_.find(run)->second->Fill(col,row-r/2.);
			  if (size == 4)
			    theHWindowClusterSize4_.find(run)->second->Fill(col,row-r/2.);
			}
		    }
		}
	    }
	  else // nRow even
	    {
	      for (int r = 1; r < nRow; r++)
		{
		  if (ceil(r/2.) != r/2.)
		    {
		      if ((row+r/2.) <= higherRow)
			{
			  theHWindow_.find(run)->second->Fill(col,row+r/2.);

			  if (size == 1)
			    theHWindowClusterSize1_.find(run)->second->Fill(col,row+r/2.);
			  if (size == 2)
			    theHWindowClusterSize2_.find(run)->second->Fill(col,row+r/2.);
			  if (size == 3)
			    theHWindowClusterSize3_.find(run)->second->Fill(col,row+r/2.);
			  if (size == 4)
			    theHWindowClusterSize4_.find(run)->second->Fill(col,row+r/2.);
			}

		      if ((row-r/2.) >= lowerRow)
			{
			  theHWindow_.find(run)->second->Fill(col,row-r/2.);

			  if (size == 1)
			    theHWindowClusterSize1_.find(run)->second->Fill(col,row-r/2.);
			  if (size == 2)
			    theHWindowClusterSize2_.find(run)->second->Fill(col,row-r/2.);
			  if (size == 3)
			    theHWindowClusterSize3_.find(run)->second->Fill(col,row-r/2.);
			  if (size == 4)
			    theHWindowClusterSize4_.find(run)->second->Fill(col,row-r/2.);
			}
		    }
		}
	    }
	}
      else if (nCol > 1 && nRow == 1)
	{
	  if (ceil(nCol/2.) != nCol/2.) // nCol odd
	    {
	      theHWindow_.find(run)->second->Fill(col,row);

	      if (size == 1)
		theHWindowClusterSize1_.find(run)->second->Fill(col,row);
	      if (size == 2)
		theHWindowClusterSize2_.find(run)->second->Fill(col,row);
	      if (size == 3)
		theHWindowClusterSize3_.find(run)->second->Fill(col,row);
	      if (size == 4)
		theHWindowClusterSize4_.find(run)->second->Fill(col,row);

	      for (int c=1; c<nCol; c++)
		{
		  if (ceil(c/2.) == c/2.)
		    {
		      if ((col+c/2.) <= higherCol)
			{
			  theHWindow_.find(run)->second->Fill(col+c/2.,row);

			  if (size == 1)
			    theHWindowClusterSize1_.find(run)->second->Fill(col+c/2.,row);
			  if (size == 2)
			    theHWindowClusterSize2_.find(run)->second->Fill(col+c/2.,row);
			  if (size == 3)
			    theHWindowClusterSize3_.find(run)->second->Fill(col+c/2.,row);
			  if (size == 4)
			    theHWindowClusterSize4_.find(run)->second->Fill(col+c/2.,row);
			}
		      
		      if ((col-c/2.) >= lowerCol)
			{
			  theHWindow_.find(run)->second->Fill(col-c/2.,row);

			  if (size == 1)
			    theHWindowClusterSize1_.find(run)->second->Fill(col-c/2.,row);
			  if (size == 2)
			    theHWindowClusterSize2_.find(run)->second->Fill(col-c/2.,row);
			  if (size == 3)
			    theHWindowClusterSize3_.find(run)->second->Fill(col-c/2.,row);
			  if (size == 4)
			    theHWindowClusterSize4_.find(run)->second->Fill(col-c/2.,row);
			}
		    }
		}
	    }
	  else // nCol even
	    {
	      for (int c = 1; c < nCol; c++)
		{
		  if (ceil(c/2.) != c/2.)
		    {
		      if ((col+c/2.) <= higherCol)
			{
			  theHWindow_.find(run)->second->Fill(col+c/2.,row);

			  if (size == 1)
			    theHWindowClusterSize1_.find(run)->second->Fill(col+c/2.,row);
			  if (size == 2)
			    theHWindowClusterSize2_.find(run)->second->Fill(col+c/2.,row);
			  if (size == 3)
			    theHWindowClusterSize3_.find(run)->second->Fill(col+c/2.,row);
			  if (size == 4)
			    theHWindowClusterSize4_.find(run)->second->Fill(col+c/2.,row);
			}

		      if ((col-c/2.) >= lowerCol)
			{
			  theHWindow_.find(run)->second->Fill(col-c/2.,row);

			  if (size == 1)
			    theHWindowClusterSize1_.find(run)->second->Fill(col-c/2.,row);
			  if (size == 2)
			    theHWindowClusterSize2_.find(run)->second->Fill(col-c/2.,row);
			  if (size == 3)
			    theHWindowClusterSize3_.find(run)->second->Fill(col-c/2.,row);
			  if (size == 4)
			    theHWindowClusterSize4_.find(run)->second->Fill(col-c/2.,row);
			}
		    }
		}
	    }
	}
    }
}

////////////////////////////////////////////////////////////////////////////////////
void HistogramWindow::removePixel(int col, int row)
{

    for(std::map<int,TH2F*>::iterator it=theHWindow_.begin(); it != theHWindow_.end(); ++it){

        TAxis* xAxis = it->second->GetXaxis() ;
        TAxis* yAxis = it->second->GetYaxis() ;


        it->second->SetBinContent(xAxis->FindBin(col),yAxis->FindBin(row),0);

    }

}

////////////////////////////////////////////////////////////////////////////////////
int HistogramWindow::getNumberOfEvents (void)
{

    int numberOfEvents=0;

    for(std::map<int,TH2F*>::iterator it=theHWindow_.begin(); it != theHWindow_.end(); ++it)
        numberOfEvents+=it->second->GetEntries();

    return numberOfEvents;

}

/////////////////////////////////////////////////////////////////////////////////////
void HistogramWindow::calculateTimeEfficiency(void)
{
    for(std::map<int,TH2F*>::iterator it=theH2TimeWindow_.begin(); it!=theH2TimeWindow_.end(); ++it){

        it->second->Divide(theH2TimeWindow_norm_.find(it->first)->second);

        for(int c=1; c<=it->second->GetXaxis()->GetNbins(); ++c){

            int eventDCFreezing=0;

            int maxEventBin = it->second->GetYaxis()->GetNbins();

            if(it->second->GetBinContent(c,maxEventBin)!=0) eventDCFreezing=it->second->GetYaxis()->GetBinUpEdge(maxEventBin);

            else{

                for(int t=it->second->GetYaxis()->GetNbins(); t>0; --t){

                    if(it->second->GetBinContent(c,t)!=0){

                        eventDCFreezing=it->second->GetYaxis()->GetBinLowEdge(t);
                        break;

                    }

                }

            }
            theH1TimeWindow_.find(it->first)->second->Fill(it->second->GetXaxis()->GetBinCenter(c),eventDCFreezing);
        }

    }
}
