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

///////////////////////////////////////////////////////////////////////////////////////////
HistogramWindow::HistogramWindow(std::string name, int nBinsX, int nBinsY, int totalEvents) :
    Window(name,nBinsX,nBinsY)
  ,theHWindow_(0)
  ,the2HTimeWindow_(0)
  ,the2HTimeWindow_norm_(0)
  ,the1HTimeWindow_(0)
  ,totalEvents_(totalEvents)
{
    nBinsX_ = nBinsX;
    nBinsY_ = nBinsY;

    int EventBinSize = 1000;

    totalEvents_/=EventBinSize;
    totalEvents_*=EventBinSize;//for having a number multiple of EventBinSize

    theHWindow_ = new TH2F ((name_+"_Window").c_str(),name_.c_str(),nBinsX_,0,nBinsX_,nBinsY_,0,nBinsY_);

    the2HTimeWindow_ = new TH2F ((name_+"_TimeWindow_H2").c_str(),(name_+"_TimeWindow_H2").c_str(),nBinsX_,0,nBinsX_,totalEvents_/EventBinSize,0,totalEvents_);
    the2HTimeWindow_norm_ = new TH2F ((name_+"_TimeWindow_norm_H2").c_str(),(name_+"_TimeWindow_norm_H2").c_str(),nBinsX_,0,nBinsX_,totalEvents_/EventBinSize,0,totalEvents_);
    the1HTimeWindow_ = new TH1F ((name_ +"_TimeWindow_").c_str(),(name_ +"_TimeWindow_").c_str(),nBinsX_,0,nBinsX_);
}

///////////////////////////////////////////////////////////////////////////////////////////
HistogramWindow::~HistogramWindow(void)
{
    if(!Window::fDoNotDelete_ && theHWindow_)
        delete theHWindow_;
    theHWindow_ = 0;

    if(!Window::fDoNotDelete_ && the1HTimeWindow_ )
        delete the1HTimeWindow_;
    the1HTimeWindow_ = 0;

    delete the2HTimeWindow_;
    delete the2HTimeWindow_norm_;
}

///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkWindow(float col, float row) const
{
    TAxis* xAxis = theHWindow_->GetXaxis() ;
    TAxis* yAxis = theHWindow_->GetYaxis() ;

    if(theHWindow_->GetCellContent(xAxis->FindBin(col),yAxis->FindBin(row)) != 0)
        return true;
    else
        return false;
}
///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkWindowAbout(float col, float row) const
{
    TAxis* xAxis = theHWindow_->GetXaxis() ;
    TAxis* yAxis = theHWindow_->GetYaxis() ;

    if(theHWindow_->GetCellContent(xAxis->FindBin(col  ),yAxis->FindBin(row  )) != 0 &&
       theHWindow_->GetCellContent(xAxis->FindBin(col-1),yAxis->FindBin(row  )) != 0 &&
       theHWindow_->GetCellContent(xAxis->FindBin(col+1),yAxis->FindBin(row  )) != 0 &&
       theHWindow_->GetCellContent(xAxis->FindBin(col  ),yAxis->FindBin(row-1)) != 0 &&
       theHWindow_->GetCellContent(xAxis->FindBin(col-1),yAxis->FindBin(row-1)) != 0 &&
       theHWindow_->GetCellContent(xAxis->FindBin(col+1),yAxis->FindBin(row-1)) != 0 &&
       theHWindow_->GetCellContent(xAxis->FindBin(col  ),yAxis->FindBin(row+1)) != 0 &&
       theHWindow_->GetCellContent(xAxis->FindBin(col-1),yAxis->FindBin(row+1)) != 0 &&
       theHWindow_->GetCellContent(xAxis->FindBin(col+1),yAxis->FindBin(row+1)) != 0 )
        return true;
    else
        return false;
}////////////////////////////////////////////////////////////////////////////////////
void HistogramWindow::calculateWindow(int planeID, const Data& data, int lowerCol, int higherCol, int lowerRow, int higherRow)
{
    //std::stringstream ss;

    float col  = data.getMeanCol(planeID);
    float row  = data.getMeanRow(planeID);
    int   nRow = data.getNumberOfRows(planeID);
    int   nCol = data.getNumberOfCols(planeID);

    if( data.getIsInDetector(planeID) && row >= lowerRow && col >= lowerCol && row <= higherRow && col <= higherCol ){

        the2HTimeWindow_norm_->Fill(col,data.getEventNumber());

        if( data.getHasHit(planeID) )

            the2HTimeWindow_->Fill(col,data.getEventNumber());

    }

    if( data.getHasHit(planeID) && data.getIsInDetector(planeID) && row >= lowerRow && col >= lowerCol && row <= higherRow && col <= higherCol )
    {



        if(nRow==1 && nCol==1)
            theHWindow_->Fill(col,row);
        else if(nRow>1 && nCol==1)
        {
            if( ceil(nRow/2.) != nRow/2. ) //nRow odd
            {
                theHWindow_->Fill(col,row);
                for(int r=1; r<nRow; r++)
                {
                    if( ceil(r/2.) == r/2. )
                    {
                        if( (row+r/2.) <= higherRow )
                            theHWindow_->Fill(col,row+r/2.);
                        if( (row-r/2.) >= lowerRow )
                            theHWindow_->Fill(col,row-r/2.);
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
                            theHWindow_->Fill(col,row+r/2.);
                        if( (row-r/2.) >= lowerRow )
                            theHWindow_->Fill(col,row-r/2.);
                    }
                }
            }
        }
        else if(nCol>1 && nRow==1)
        {
            if( ceil(nCol/2.) != nCol/2. )//nCol odd
            {
                theHWindow_->Fill(col,row);
                for(int c=1; c<nCol; c++)
                {
                    if( ceil(c/2.) == c/2. )
                    {
                        if( (col+c/2.) <= higherCol )
                            theHWindow_->Fill(col+c/2.,row);
                        else if( (col-c/2.) >= lowerCol )
                            theHWindow_->Fill(col-c/2.,row);
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
                            theHWindow_->Fill(col+c/2.,row);
                        if( (col-c/2.) >= lowerCol )
                            theHWindow_->Fill(col-c/2.,row);
                    }
                }
            }
        }


    }
}

////////////////////////////////////////////////////////////////////////////////////
void HistogramWindow::removePixel(int col, int row)
{
    TAxis* xAxis = theHWindow_->GetXaxis() ;
    TAxis* yAxis = theHWindow_->GetYaxis() ;

    theHWindow_->SetBinContent(xAxis->FindBin(col),yAxis->FindBin(row),0);
}

////////////////////////////////////////////////////////////////////////////////////
int HistogramWindow::getNumberOfEvents (void)
{
    return (const int)theHWindow_->GetEntries();
}

/////////////////////////////////////////////////////////////////////////////////////
void HistogramWindow::calculateTimeEfficiency(void)
{
    the2HTimeWindow_->Divide(the2HTimeWindow_norm_);
}
