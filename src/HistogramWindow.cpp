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

        theHWindow_          .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_Window_Run"+ss.str()).c_str()            ,(name_+"_Window_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));
        theH2TimeWindow_     .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_TimeWindow_H2_Run"+ss.str()).c_str()     ,(name_+"_TimeWindow_H2_Run"+ss.str()).c_str()     ,nBinsX_, binXMin, binXMax+1,totalEvents/EventBinSize,0       ,totalEvents)));
        theH2TimeWindow_norm_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_TimeWindow_norm_H2_Run"+ss.str()).c_str(),(name_+"_TimeWindow_norm_H2_Run"+ss.str()).c_str(),nBinsX_, binXMin, binXMax+1,totalEvents/EventBinSize,0       ,totalEvents)));
        theH1TimeWindow_     .insert(std::pair<int,TH1F*> (runIt->first ,new TH1F ((name_ +"_TimeWindow_Run"+ss.str()).c_str()       ,(name_ +"_TimeWindow_Run"+ss.str()).c_str()       ,nBinsX_, binXMin, binXMax+1                                              )));

    }
}

///////////////////////////////////////////////////////////////////////////////////////////
HistogramWindow::~HistogramWindow(void)
{
    for(std::map<int,TH2F*>::iterator it=theHWindow_.begin(); it != theHWindow_.end(); ++it){
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
    TAxis* xAxis = theHWindow_.find(runNumber)->second->GetXaxis() ;
    TAxis* yAxis = theHWindow_.find(runNumber)->second->GetYaxis() ;

    //std::cout << __PRETTY_FUNCTION__ << "Col: " << col << " ColBin: " << xAxis->FindBin(col) << std::endl;
    //WARNING THIS METHOD IS WRONG BECAUSE IT NEEDS TO CHECK THE DATA TYPE!
    if(theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col),yAxis->FindBin(row)) != 0)
    {
        //std::cout << __PRETTY_FUNCTION__ << "theHWindow_.find(runNumber)" << std::endl;
        return true;
    }
    return false;
}
///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkWindowAbout(float col, float row, int runNumber, int type) const
{
    TAxis* xAxis = theHWindow_.find(runNumber)->second->GetXaxis() ;
    TAxis* yAxis = theHWindow_.find(runNumber)->second->GetYaxis() ;
    if(type==0)
    {
        if(theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col  ),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col-1),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col+1),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col  ),yAxis->FindBin(row-1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col-1),yAxis->FindBin(row-1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col+1),yAxis->FindBin(row-1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col  ),yAxis->FindBin(row+1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col-1),yAxis->FindBin(row+1)) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col+1),yAxis->FindBin(row+1)) != 0 )
            return true;
        else
            return false;
    }
    else if(type==1)
    {
        if(theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col  ),yAxis->FindBin(0.)) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col-1),yAxis->FindBin(0.)) != 0 &&
                theHWindow_.find(runNumber)->second->GetCellContent(xAxis->FindBin(col+1),yAxis->FindBin(0.)) != 0 )

            return true;
        else
            return false;

    }
    else
    {
        std::cout << __PRETTY_FUNCTION__ << "I only know 2 types (0, 1) while this is type: " << type << ". IMPOSSIBLE!" << " row: " << row << " col: " << col << std::endl;
        assert(0);
    }
}////////////////////////////////////////////////////////////////////////////////////

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

    if( data.getIsInDetector(planeID) && row >= lowerRow && col >= lowerCol && row <= higherRow && col <= higherCol ){

        theH2TimeWindow_norm_.find(run)->second->Fill(col,entry);

        if( data.getHasHit(planeID) )

            theH2TimeWindow_.find(run)->second->Fill(col,entry);

    }
    //std::cout << __PRETTY_FUNCTION__ << "Before Plane: " << planeID << " row: " << row << " col: " << col << std::endl;
    if( data.getHasHit(planeID) && data.getIsInDetector(planeID) && row >= lowerRow && col >= lowerCol && row <= higherRow && col <= higherCol )
    {
        //std::cout << __PRETTY_FUNCTION__ << "After  Plane: " << planeID << " row: " << row << " col: " << col << std::endl;
        if(nRow==1 && nCol==1)
            theHWindow_.find(run)->second->Fill(col,row);
        else if(nRow>1 && nCol==1)
        {
            if( ceil(nRow/2.) != nRow/2. ) //nRow odd
            {
                theHWindow_.find(run)->second->Fill(col,row);
                for(int r=1; r<nRow; r++)
                {
                    if( ceil(r/2.) == r/2. )
                    {
                        if( (row+r/2.) <= higherRow )
                            theHWindow_.find(run)->second->Fill(col,row+r/2.);
                        if( (row-r/2.) >= lowerRow )
                            theHWindow_.find(run)->second->Fill(col,row-r/2.);
                    }
                }
            }
            else  //nRow even
            {
                for(int r=1; r<nRow; r++)
                {
                    if( ceil(r/2.) != r/2. )
                    {
                        if( (row+r/2.) <= higherRow )
                            theHWindow_.find(run)->second->Fill(col,row+r/2.);
                        if( (row-r/2.) >= lowerRow )
                            theHWindow_.find(run)->second->Fill(col,row-r/2.);
                    }
                }
            }
        }
        else if(nCol>1 && nRow==1)
        {
            if( ceil(nCol/2.) != nCol/2. )//nCol odd
            {
                theHWindow_.find(run)->second->Fill(col,row);
                for(int c=1; c<nCol; c++)
                {
                    if( ceil(c/2.) == c/2. )
                    {
                        if( (col+c/2.) <= higherCol )
                            theHWindow_.find(run)->second->Fill(col+c/2.,row);
                        else if( (col-c/2.) >= lowerCol )
                            theHWindow_.find(run)->second->Fill(col-c/2.,row);
                    }
                }
            }
            else//nCol even
            {
                for(int c=1; c<nCol; c++)
                {
                    if( ceil(c/2.) != c/2. )
                    {
                        if( (col+c/2.) <= higherCol )
                            theHWindow_.find(run)->second->Fill(col+c/2.,row);
                        if( (col-c/2.) >= lowerCol )
                            theHWindow_.find(run)->second->Fill(col-c/2.,row);
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
