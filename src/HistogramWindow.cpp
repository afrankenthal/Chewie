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
#include "PlanesMapping.h"

#include <TH2F.h>
#include <TAxis.h>
#include <TTree.h>
#include <TROOT.h>
#include <math.h>
#include <cmath>

#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

///////////////////////////////////////////////////////////////////////////////////////////
HistogramWindow::HistogramWindow(std::string name, int binXMin, int binXMax, int binYMin, int binYMax, std::map<int,int> runNumberEntries) :
    Window(name),
    thePlaneMapping_(0)
{
    thePlaneMapping_ = new PlanesMapping();

    nBinsX_ = binXMax - binXMin +1;
    nBinsY_ = binYMax - binYMin +1;

    binXMaxv1 = 1000; //approximate 10 cm
    binXMinv1 = 0;
    binYMaxv1 = 50000; //5cm
    binYMinv1 = 0;

    //nBinsXv1_ = (binXMaxv1 - binXMinv1 +1);
    //nBinsYv1_ = (binYMaxv1 - binYMinv1 +1);
    nBinsXv1_ = 1000;
    nBinsYv1_ = 1000;

    //bool isFirstLoop = true;

    //threshold = 0;

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

        theHWindow_            .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_Window_Run"+ss.str()).c_str()              ,(name_+"_Window_Run"+ss.str()).c_str()              ,nBinsX_  , binXMin  , binXMax+1  , nBinsY_/13                 , binYMin,binYMax+1  )));
        theHWindowV1_          .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowV1_Run"+ss.str()).c_str()            ,(name_+"_WindowV1_Run"+ss.str()).c_str()            ,nBinsXv1_, binXMinv1, binXMaxv1+1, nBinsYv1_            , binYMinv1,binYMaxv1+1  )));

        theH2TimeWindow_       .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_TimeWindow_H2_Run"+ss.str()).c_str()     ,(name_+"_TimeWindow_H2_Run"+ss.str()).c_str()     ,nBinsX_, binXMin, binXMax+1,totalEvents/EventBinSize,0       ,totalEvents)));
        theH2TimeWindow_norm_  .insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_TimeWindow_norm_H2_Run"+ss.str()).c_str(),(name_+"_TimeWindow_norm_H2_Run"+ss.str()).c_str(),nBinsX_, binXMin, binXMax+1,totalEvents/EventBinSize,0       ,totalEvents)));
        theH1TimeWindow_       .insert(std::pair<int,TH1F*> (runIt->first ,new TH1F ((name_ +"_TimeWindow_Run"+ss.str()).c_str()       ,(name_ +"_TimeWindow_Run"+ss.str()).c_str()       ,nBinsX_, binXMin, binXMax+1                                              )));

        theHWindowClusterSize1_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowClusterSize1_Run"+ss.str()).c_str(),(name_+"_WindowClusterSize1_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));
        theHWindowClusterSize2_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowClusterSize2_Run"+ss.str()).c_str(),(name_+"_WindowClusterSize2_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));
        theHWindowClusterSize3_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowClusterSize3_Run"+ss.str()).c_str(),(name_+"_WindowClusterSize3_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));
        theHWindowClusterSize4_.insert(std::pair<int,TH2F*> (runIt->first ,new TH2F ((name_+"_WindowClusterSize4_Run"+ss.str()).c_str(),(name_+"_WindowClusterSize4_Run"+ss.str()).c_str()            ,nBinsX_, binXMin, binXMax+1,nBinsY_                 , binYMin,binYMax+1  )));

        theStripIndex_             .insert(std::pair<int,TH1F*>   (runIt->first ,new TH1F ((name_+"_StripIndex_Run"                +ss.str()).c_str(),(name_+"_StripIndex_Run"                +ss.str()).c_str() ,120, 500, 620)));
        theRowIndex_               .insert(std::pair<int,TH1F*>   (runIt->first ,new TH1F ((name_+"_RowIndex_Run"                  +ss.str()).c_str(),(name_+"_RowIndex_Run"                  +ss.str()).c_str() ,120, 400, 620)));

        EfficiencyVsEventFull_norm_.insert(std::pair<float,TH1F*> (runIt->first ,new TH1F ((name_+"_EfficiencyVsEventFull_norm_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsEventFull_norm_Run"+ss.str()).c_str(), totalEvents/250, 0, totalEvents)));
        EfficiencyVsEventFull_.insert(std::pair<float,TH1F*> (runIt->first ,new TH1F ((name_+"_EfficiencyVsEventFull_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsEventFull_Run"+ss.str()).c_str(), totalEvents/250, 0, totalEvents)));
        EfficiencyVsEventGood_.insert(std::pair<float,TH1F*> (runIt->first ,new TH1F ((name_+"_EfficiencyVsEventGood_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsEventGood_Run"+ss.str()).c_str(), totalEvents/250, 0, totalEvents)));
        EfficiencyVsEventFull2D_.insert(std::pair<float,TH2F*> (runIt->first ,new TH2F ((name_+"_EfficiencyVsEventFull2D_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsEventFull2D_Run"+ss.str()).c_str(), totalEvents/250, 0, totalEvents, 10000, 0, 1)));
        EfficiencyVsEventGood2D_.insert(std::pair<float,TH2F*> (runIt->first ,new TH2F ((name_+"_EfficiencyVsEventGood2D_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsEventGood2D_Run"+ss.str()).c_str(), totalEvents/250, 0, totalEvents, 10000, 0, 1)));

        EfficiencyVsTriggerLatency_.insert(std::pair<float,TH1F*> (runIt->first ,new TH1F ((name_+"_EfficiencyVsTriggerLatency_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsTriggerLatency_Run"+ss.str()).c_str(), 15, 0, 15)));
        EfficiencyVsTriggerLatency_norm_.insert(std::pair<float,TH1F*> (runIt->first ,new TH1F ((name_+"_EfficiencyVsTriggerLatency_norm_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsTriggerLatency_norm_Run"+ss.str()).c_str(), 15, 0, 15)));

        EfficiencyVsTriggerLatencyFull_.insert(std::pair<float,TH1F*> (runIt->first ,new TH1F ((name_+"_EfficiencyVsTriggerLatencyFull_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsTriggerLatencyFull_Run"+ss.str()).c_str(), 15, 0, 15)));
        EfficiencyVsTriggerLatencyFull_norm_.insert(std::pair<float,TH1F*> (runIt->first ,new TH1F ((name_+"_EfficiencyVsTriggerLatencyFull_norm_Run"+ss.str()).c_str(),(name_+"_EfficiencyVsTriggerLatencyFull_norm_Run"+ss.str()).c_str(), 15, 0, 15)));

    }
    //TH1F *EfficiencyVsTriggerLatency = new TH1F("EfficiencyVsTriggerLatency","EfficiencyVsTriggerLatency",15, 0, 15);
    //TH1F *EfficiencyVsTriggerLatency_norm = new TH1F("EfficiencyVsTriggerLatency_norm","EfficiencyVsTriggerLatency_norm",15, 0, 15);
}
///
///////////////////////////////////////////////////////////////////////////////////////////
HistogramWindow::~HistogramWindow(void)
{
    if (thePlaneMapping_) delete thePlaneMapping_;

    for(std::map<int,TH2F*>::iterator it=theHWindow_.begin(); it != theHWindow_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=theHWindowV1_.begin(); it != theHWindowV1_.end(); ++it){
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

    for(std::map<int,TH1F*>::iterator it=theStripIndex_.begin(); it != theStripIndex_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH1F*>::iterator it=theRowIndex_.begin(); it != theRowIndex_.end(); ++it){
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

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsEventFull_norm_.begin(); it != EfficiencyVsEventFull_norm_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsEventFull_.begin(); it != EfficiencyVsEventFull_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsEventGood_.begin(); it != EfficiencyVsEventGood_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=EfficiencyVsEventFull2D_.begin(); it != EfficiencyVsEventFull2D_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH2F*>::iterator it=EfficiencyVsEventGood2D_.begin(); it != EfficiencyVsEventGood2D_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsTriggerLatency_.begin(); it != EfficiencyVsTriggerLatency_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsTriggerLatency_norm_.begin(); it != EfficiencyVsTriggerLatency_norm_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsTriggerLatencyFull_.begin(); it != EfficiencyVsTriggerLatencyFull_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsTriggerLatencyFull_norm_.begin(); it != EfficiencyVsTriggerLatencyFull_norm_.end(); ++it){
        if(!Window::fDoNotDelete_ && it->second )
            delete it->second;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void HistogramWindow::label(int runNumber) const
{

    theHWindow_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column");
    theHWindow_.find(runNumber)->second->GetYaxis()->SetTitle("Strip Index Row (Projection)");

    theHWindowV1_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column");
    theHWindowV1_.find(runNumber)->second->GetYaxis()->SetTitle("Strip Length (micron)");
    theHWindowV1_.find(runNumber)->second->GetYaxis()->SetNoExponent(1);

    theH2TimeWindow_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column");
    theH2TimeWindow_.find(runNumber)->second->GetYaxis()->SetTitle("Number of events");

    theH2TimeWindow_norm_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column (NORM)");
    theH2TimeWindow_norm_.find(runNumber)->second->GetYaxis()->SetTitle("Number of events");

    theH1TimeWindow_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column");
    theH1TimeWindow_.find(runNumber)->second->GetYaxis()->SetTitle("Number of events");

    theHWindowClusterSize1_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column (Size 1)");
    theHWindowClusterSize1_.find(runNumber)->second->GetYaxis()->SetTitle("Number of events");

    theHWindowClusterSize2_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column (Size 2)");
    theHWindowClusterSize2_.find(runNumber)->second->GetYaxis()->SetTitle("Number of events");

    theHWindowClusterSize3_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column (Size 3)");
    theHWindowClusterSize3_.find(runNumber)->second->GetYaxis()->SetTitle("Number of events");

    theHWindowClusterSize4_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column (Size 4)");
    theHWindowClusterSize4_.find(runNumber)->second->GetYaxis()->SetTitle("Number of events");

    theStripIndex_.find(runNumber)->second->GetXaxis()->SetTitle("Strip Index Column");
    theStripIndex_.find(runNumber)->second->GetYaxis()->SetTitle("Number of events");

    EfficiencyVsEventFull_norm_.find(runNumber)->second->GetXaxis()->SetTitle("Number of Full events");
    EfficiencyVsEventFull_norm_.find(runNumber)->second->GetYaxis()->SetTitle("Efficiency");

    EfficiencyVsEventFull_.find(runNumber)->second->GetXaxis()->SetTitle("Number of Full events");
    EfficiencyVsEventFull_.find(runNumber)->second->GetYaxis()->SetTitle("Efficiency");

    EfficiencyVsEventGood_.find(runNumber)->second->GetXaxis()->SetTitle("Number of Good events");
    EfficiencyVsEventGood_.find(runNumber)->second->GetYaxis()->SetTitle("Efficiency");

    EfficiencyVsEventFull2D_.find(runNumber)->second->GetXaxis()->SetTitle("Number of Full events");
    EfficiencyVsEventFull2D_.find(runNumber)->second->GetYaxis()->SetTitle("Efficiency");

    EfficiencyVsEventGood2D_.find(runNumber)->second->GetXaxis()->SetTitle("Number of Good events");
    EfficiencyVsEventGood2D_.find(runNumber)->second->GetYaxis()->SetTitle("Efficiency");



    //theStripIndex_->GetXaxis()->SetTitle("strip");

    //for(unsigned int p=0; p<50; p++)
    //{
    //    if(data.getIsInDetector(p))
    //       theHWindow_[p]->GetXaxis()->SetTitle("x (um)"          );
    //}
}
///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkGoodRun(int runNumber, int entry) const
{
    if(entry > EfficiencyVsEventGood2D_.find(runNumber)->second->GetXaxis()->GetXmax()) return false;
    else return true;
}
///////////////////////////////////////////////////////////////////////////////////////////
double HistogramWindow::runAverageEfficiency(int runNumber) const
{
    double runMean = EfficiencyVsEventGood2D_.find(runNumber)->second->GetMean(2);
    return runMean;
}
///////////////////////////////////////////////////////////////////////////////////////////
/*Alex's stuff, not in use
 *
 *void HistogramWindow::latencyPlots(int runNumber)
{
    ifstream csvFile("../OuterTrackerExtra/runLatencyData.csv");
    if (csvFile.is_open()){
        std::string lineStr;
        //std::cout << "file is open!" << std::endl;
        while (getline(csvFile, lineStr)){
            unsigned int k = 0;
            std::string toParse="";
            std::string parsed;
            std::vector<std::string> lineVec;
            for(int i=0; i<3; i++){
                //std::cout << 0 << std::endl;
                if(i!=0){
                    k = parsed.length();
                    parsed.clear();
                }
                bool isParsing = true;
                for(; k<lineStr.length(); k++){
                    //while(isParsing){
                    if(lineStr[k]!=','){
                        toParse+=lineStr[k];
                        //k++;
                    }
                    else{
                        lineVec.push_back(toParse);
                        parsed = toParse;
                        toParse.clear();
                        //k++;
                        //++k;
                        isParsing = false;
                    }
                }
            }
            if(atoi(lineVec[0].c_str())==runNumber){
                //std::cout << lineVec[0] << std::endl;
                int triggerLatency = atoi(lineVec[1].c_str());
                if(lineVec[1]!=""){
                    double runMeanEfficiency = runAverageEfficiency(runNumber);
                    //std::cout << "filling plots" << std::endl;
                    EfficiencyVsTriggerLatency_[runNumber]->Fill(triggerLatency, runMeanEfficiency);
                    EfficiencyVsTriggerLatency_norm_[runNumber]->Fill(triggerLatency, 1);
                    //std::cout << "Run " << runNumber << " has efficiency " << runMeanEfficiency << " with trigger latency " << lineVec[1] << std::endl;
                }
            }
            lineVec.clear();
        }
    }
}*/
///////////////////////////////////////////////////////////////////////////////////////////
void HistogramWindow::combineLatencyPlots(void)
{
    double binContent;
    int normContent;
    for(std::map<int,TH1F*>::iterator it=EfficiencyVsTriggerLatency_norm_.begin(); it!=EfficiencyVsTriggerLatency_norm_.end(); ++it){
        for(int bin=0; bin<(it->second->GetXaxis()->GetNbins()); bin++){
            binContent  = EfficiencyVsTriggerLatency_[it->first]->GetBinContent(bin);
            //std::cout << binContent << "," << bin << std::endl;
            normContent = EfficiencyVsTriggerLatency_norm_[it->first]->GetBinContent(bin);
            EfficiencyVsTriggerLatencyFull_[EfficiencyVsTriggerLatency_norm_.begin()->first]->Fill(bin, binContent);
            EfficiencyVsTriggerLatencyFull_norm_[EfficiencyVsTriggerLatency_norm_.begin()->first]->Fill(bin, normContent);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkWindow(float col, float row, int runNumber) const
{
    //std::cout<<"Inside checkWindow, col = "<<col<<std::endl;
    //std::cout<<"Inside checkWindow, row = "<<row<<std::endl;

    TAxis* xAxis = theHWindow_.find(runNumber)->second->GetXaxis();
    TAxis* yAxis = theHWindow_.find(runNumber)->second->GetYaxis();

    // std::cout<<"theHWindow = "<<theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),yAxis->FindBin(row))<<std::endl;

    if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),yAxis->FindBin(row)) != 0) return true;
    return false;
}
///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::byPassCheckWindow() const
{
    return true;
}
bool HistogramWindow::checkWindow_v1(float col, float row, int runNumber, int/*planeID*/) const
{
    //std::cout<<"Inside checkWindow, col = "<<col<<std::endl;
    //std::cout<<"Inside checkWindow, row = "<<row<<std::endl;

    TAxis* xAxis = theHWindow_.find(runNumber)->second->GetXaxis();
    TAxis* yAxis = theHWindow_.find(runNumber)->second->GetYaxis();
/*
    if (planeID==24 || planeID==25){
    std::cout<<"-- checkWindow --"<<std::endl;
    std::cout<<"Looking in "<<thePlaneMapping_->getPlaneName(planeID)<<std::endl;
    std::cout<<"Projected track to column = "<<col<<std::endl;
    std::cout<<"Projected track to row = "<<row<<std::endl;
    std::cout<<"xAxis->FindBin(col) = "<<xAxis->FindBin(col)<<std::endl;
    std::cout<<"yAxis->FindBin(row) = "<<yAxis->FindBin(row)<<std::endl;
    int nBins = theHWindow_.find(runNumber)->second->GetNbinsX();
    int nBinsY = theHWindow_.find(runNumber)->second->GetNbinsY();
    std::cout<<"Number of bin X content = "<<nBins<<std::endl;
    std::cout<<"Number of bin Y content = "<<nBinsY<<std::endl;
*/
//    if(xAxis->GetXmin() <= col && col <= xAxis->GetXmax() && yAxis->GetXmin() <= row && row <= yAxis->GetXmax()) return true;
    if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),yAxis->FindBin(row)) != 0) return true;
    return false;
    //    }

}

///////////////////////////////////////////////////////////////////////////////////////////
bool HistogramWindow::checkWindowAbout(float col, float row, int runNumber, int type) const
{
    TAxis* xAxis = theHWindow_.find(runNumber)->second->GetXaxis() ;
    TAxis* yAxis = theHWindow_.find(runNumber)->second->GetYaxis() ;

    //std::cout<<"theHWindow Xaxis Nbins = "<<theHWindow_.find(runNumber)->second->GetXaxis()->GetNbins()<<std::endl;
    //std::cout<<"theHWindow Xaxis Size = "<<theHWindow_.find(runNumber)->second->GetXaxis()->GetSize()<<std::endl;

    //std::cout<<"theHWindow Yaxis Nbins = "<<theHWindow_.find(runNumber)->second->GetYaxis()->GetNbins()<<std::endl;
    //std::cout<<"theHWindow Yaxis Size = "<<theHWindow_.find(runNumber)->second->GetYaxis()->GetSize()<<std::endl;

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
    else if(type==1) //strip
    {
        //std::cout<<"--cell efficiency window check about--"<<std::endl;
        //std::cout<<"theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),1) = "<<theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),1)<<std::endl;
        //std::cout<<"theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),1) = "<<theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),1)<<std::endl;
        //std::cout<<"theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),1) = "<<theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),1)<<std::endl;
        //std::cout<<"The redundant row = "<<row<<std::endl;
        //std::cout<<"The what we need col = "<<col<<std::endl;

        //only column is concerned
        if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),yAxis->FindBin(row)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),yAxis->FindBin(row)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),yAxis->FindBin(row)) != 0 )
            return true;
        else
            return false;

        //if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(0.)) != 0 &&
        //        theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),yAxis->FindBin(0.)) != 0 &&
        //        theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),yAxis->FindBin(0.)) != 0 )

        //    return true;
        //else
        //    return false;

    }
    else
    {
        std::cout << __PRETTY_FUNCTION__ << "I only know 2 types (0, 1) while this is type: " << type << ". IMPOSSIBLE!" << " row: " << row << " col: " << col << std::endl;
        assert(0);
    }
}
////////////////////////////////////////////////////////////////////////////////////

bool HistogramWindow::AnticheckWindowAbout(float col, float row, int runNumber, int type) const
{
    TAxis* xAxis = theHWindow_.find(runNumber)->second->GetXaxis() ;
    TAxis* yAxis = theHWindow_.find(runNumber)->second->GetYaxis() ;

    //1 col = 90 micron

    if(type==0)
    {
        if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-2),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+2),yAxis->FindBin(row  )) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(row-2)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-2),yAxis->FindBin(row-2)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+2),yAxis->FindBin(row-2)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(row+2)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-2),yAxis->FindBin(row+2)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+2),yAxis->FindBin(row+2)) != 0 )
            return true;
        else
            return false;
    }
    else if(type==1) //strip
    {
        //std::cout<<"--cell efficiency window check about--"<<std::endl;
        //std::cout<<"theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),1) = "<<theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),1)<<std::endl;
        //std::cout<<"theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),1) = "<<theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),1)<<std::endl;
        //std::cout<<"theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),1) = "<<theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),1)<<std::endl;
        //std::cout<<"The redundant row = "<<row<<std::endl;
        //std::cout<<"The what we need col = "<<col<<std::endl;

        //only column is concerned
        if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col),yAxis->FindBin(row)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),yAxis->FindBin(row)) != 0 &&
                theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),yAxis->FindBin(row)) != 0 )
            return true;
        else
            return false;

        //if(theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col  ),yAxis->FindBin(0.)) != 0 &&
        //        theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col-1),yAxis->FindBin(0.)) != 0 &&
        //        theHWindow_.find(runNumber)->second->GetBinContent(xAxis->FindBin(col+1),yAxis->FindBin(0.)) != 0 )

        //    return true;
        //else
        //    return false;

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
    float col          = data.getMeanCol(planeID);
    float row          = data.getMeanRow(planeID);
//    if(planeID == 24)
//        std:: cout << __PRETTY_FUNCTION__ << "Row: " << row << " Col: " << col << std::endl;
    int   rowPredicted = data.getRowPredicted(planeID);
    int   colPredicted = data.getColPredicted(planeID);
    int   nRow         = data.getNumberOfRows(planeID);
    int   nCol         = data.getNumberOfCols(planeID);
    int   run          = data.getRunNumber();
    int   entry        = data.getEventChewieNumber();
    int   size         = data.getClusterSize(planeID);

    //float   localx = data.getXPitchLocal(planeID);
    //float   localy = data.getYPitchLocal(planeID);

    //    if (thePlaneMapping_->getPlaneName(planeID).find("Dut2") != std::string::npos
    //            || thePlaneMapping_->getPlaneName(planeID).find("Strip") != std::string::npos
    //            )
    //    {
    //        std::cout << "-- Calculate Window --" << std::endl;
    //        std::cout << "Looking at "            << thePlaneMapping_->getPlaneName(planeID) << std::endl;
    //        std::cout << "The lowerCol of the plane = " << lowerCol      << std::endl;
    //        std::cout << "data.getMeanCol ("      << planeID<<") = "     << col  << std::endl;
    //        std::cout << "The higherCol of the plane = "<< higherCol     << std::endl;
    //        std::cout << "The lowerRow of the plane = " << lowerRow      << std::endl;
    //        std::cout << "data.getMeanRow ("      << planeID<<") = "     << row  << std::endl;
    //        std::cout << "The higherRow of the plane = "<< higherRow     << std::endl;
    //        std::cout << "data.getNumberOfRows (" << planeID << ") = "   << nRow << std::endl;
    //        std::cout << "data.getNumberOfCols (" << planeID <<") = "    << nCol << std::endl;
    //        std::cout << "data.getRunNumber () = "<< data.getRunNumber() << std::endl;
    //        std::cout << "data.getClusterSize ("  << planeID << ") = "   << data.getClusterSize(planeID) << std::endl;
    //        std::cout << "data.getIsInDetector ("  << planeID << ") = "  << data.getIsInDetector(planeID) << std::endl;
    //     }

    
    if (data.getIsInDetector(planeID) && rowPredicted >= lowerRow && rowPredicted <= higherRow && colPredicted >= lowerCol && colPredicted <= higherCol)
    {
        if(theH2TimeWindow_norm_.find(run)  == theH2TimeWindow_norm_.end()  ||
                theH2TimeWindow_.find(run)	== theH2TimeWindow_.end()	   )
        {
            static bool alreadySeen = false ;
            if(  alreadySeen ) return ;
            if( !alreadySeen ) alreadySeen = true ;
            STDLINE("Time Window or ClusterSize data missing: skipping event (this message will NOT be repeated...)",ACRed) ;
        }
        else
        {
/*
            if ((thePlaneMapping_->getPlaneName(planeID).find("Dut2") != std::string::npos
                 || thePlaneMapping_->getPlaneName(planeID).find("Dut3") != std::string::npos)
                    && !data.getHasHit(planeID) && entry == 695
                    )
            {

                std::cout << "-- Calculate Window --" << std::endl;\
                std::cout << "Chewie entry at "            << entry <<std::endl;
                std::cout << "Looking at "            << thePlaneMapping_->getPlaneName(planeID) << std::endl;
                std::cout << "Monicelli event number = " <<  data.getEventNumber()     << std::endl;
                std::cout << "The lowerCol of the plane = " << lowerCol      << std::endl;
                std::cout << "data.getMeanCol ("      << planeID<<") = "     << col  << std::endl;
                std::cout << "data.getColPredicted ("      << planeID<<") = "     << data.getColPredicted(planeID)  << std::endl;
                std::cout << "The higherCol of the plane = "<< higherCol     << std::endl;
                std::cout << "The lowerRow of the plane = " << lowerRow      << std::endl;
                std::cout << "data.getMeanRow ("      << planeID<<") = "     << row  << std::endl;
                std::cout << "data.getRowPredicted ("      << planeID<<") = "     << data.getRowPredicted(planeID)  << std::endl;
                std::cout << "The higherRow of the plane = "<< higherRow     << std::endl;
                std::cout << "data.getNumberOfRows (" << planeID << ") = "   << nRow << std::endl;
                std::cout << "data.getNumberOfCols (" << planeID <<") = "    << nCol << std::endl;
                std::cout << "data.getRunNumber () = "<< data.getRunNumber() << std::endl;
                std::cout << "data.getClusterSize ("  << planeID << ") = "   << size << std::endl;
                std::cout << "data.getIsInDetector (" << planeID << ") = "   << data.getIsInDetector(planeID) << std::endl;
                std::cout << "data.getHasHit ("       << planeID << ") = "   << data.getHasHit(planeID) << std::endl;

            }
*/
            theH2TimeWindow_.find(run)->second->Fill(col,entry);
            EfficiencyVsEventFull_norm_.find(run)->second->Fill(entry);
            if (data.getHasHit(planeID)) theH2TimeWindow_.find(run)->second->Fill(col,entry);
            if (data.getHasHit(planeID)) EfficiencyVsEventFull_.find(run)->second->Fill(entry);
        }
    }


    if (data.getHasHit(planeID) && data.getIsInDetector(planeID) && row >= lowerRow && row <= higherRow && col >= lowerCol && col <= higherCol)
        {

        //if(planeID == 24)
        //    std:: cout << __PRETTY_FUNCTION__ << " In detector -> Row: " << row << " Col: " << col << std::endl;
        theStripIndex_.find(run)->second->Fill(col);
        theRowIndex_.find(run)->second->Fill(row);

        if (nRow == 1 && nCol == 1)
        {
            theHWindow_.find(run)->second->Fill(col,row);
            theHWindowV1_.find(run)->second->Fill(col,row*50.);

            if (size == 1)
                theHWindowClusterSize1_.find(run)->second->Fill(col,row);
            else
                std::cout << "THIS NEVER HAPPENS!" << std::endl;
//            if (size == 2)
//                theHWindowClusterSize2_.find(run)->second->Fill(col,row);
//            if (size == 3)
//                theHWindowClusterSize3_.find(run)->second->Fill(col,row);
//            if (size == 4)
//                theHWindowClusterSize4_.find(run)->second->Fill(col,row);
        }
        else if (nRow > 1 && nCol == 1)
        {
            if (ceil(nRow/2.) != nRow/2.) // nRow odd
            {


                for (int r = 1; r < nRow; r++)
                {
                    if (ceil(r/2.) == r/2.)
                    {
                        if ((row+r/2.) <= higherRow)
                        {
                            theHWindow_.find(run)->second->Fill(col,row+r/2.);
                            theHWindowV1_.find(run)->second->Fill(col,(row*50.)+r/2.);

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
                            theHWindowV1_.find(run)->second->Fill(col,(row*50.)-r/2.);

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
                            theHWindowV1_.find(run)->second->Fill(col,(row*50.)+r/2.);

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
                            theHWindowV1_.find(run)->second->Fill(col,(row*50.)-r/2.);

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
                theHWindowV1_.find(run)->second->Fill(col,row*50.);

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
                            theHWindowV1_.find(run)->second->Fill(col+c/2.,row*50.);

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
                            theHWindowV1_.find(run)->second->Fill(col-c/2.,row*50.);

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
                            theHWindowV1_.find(run)->second->Fill(col+c/2.,row*50.);


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
                            theHWindowV1_.find(run)->second->Fill(col-c/2.,row*50.);


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
    label(run);
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

    //

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsEventFull_.begin(); it!=EfficiencyVsEventFull_.end(); ++it){
        it->second->Divide(EfficiencyVsEventFull_norm_[it->first]);
    }

    for(std::map<int,TH1F*>::iterator it=EfficiencyVsEventFull_.begin(); it!=EfficiencyVsEventFull_.end(); ++it){
        for(int bin=0; bin<=(it->second->GetXaxis()->GetNbins()); bin++){
            //std::cout << "Bin: " << bin << std::endl;
            EfficiencyVsEventFull2D_[it->first]->Fill(250*bin, it->second->GetBinContent(bin));

        }
    }

    //double efficiencyRMS=EfficiencyVsEventFull2D_.find(EfficiencyVsEventFull2D_.begin()->first)->second->GetRMS(2);
    //double efficiencyMean=EfficiencyVsEventFull2D_.find(EfficiencyVsEventFull2D_.begin()->first)->second->GetMean(2);
    for(std::map<int,TH1F*>::iterator it=EfficiencyVsEventFull_.begin(); it!=EfficiencyVsEventFull_.end(); ++it)
    {
        double dynamicSum = 0;
        double dynamicMean = 1;
        double dynamicRMS  = 1;
        double sigmaSquared;
        double binContent;
        double oldMean = 1;
        double oldRMS  = 1;
        bool   badBin0 = false;
        bool   badBin1 = false;
        bool   isKeep  = false;
        int    badBin  = it->second->GetXaxis()->GetNbins();
        std::cout << __PRETTY_FUNCTION__ << it->first << " : " << it->second->GetXaxis()->GetNbins() << " Name: " << it->second->GetName() << std::endl;
        for(int bin=1; bin<=(it->second->GetXaxis()->GetNbins()); bin++)
        {
            binContent = it->second->GetBinContent(bin);
            dynamicSum += binContent;
            dynamicMean = dynamicSum / bin;
            sigmaSquared = 0;
//            std::cout << "Bin #: "        << bin
//                      << " BinContent: "  << binContent
//                      << " dynamicSum: "  << dynamicSum
//                      << " dynamicMean: " << dynamicMean
//                      << " oldmean: "     << oldMean
//                      << " 3sigma: "      << 3*oldRMS
//                      << std::endl;
            if(bin != 1)
            {
                for(int dynamicBin=1; dynamicBin <= bin; dynamicBin++)
                {
                    sigmaSquared += pow((dynamicMean - it->second->GetBinContent(dynamicBin)), 2);
                }

                sigmaSquared /= bin;
                sigmaSquared = sqrt(sigmaSquared);
                if(sigmaSquared < 0.01) sigmaSquared = 0.01;
                dynamicRMS = sigmaSquared;
                if(binContent < (oldMean - 3*oldRMS))
                {
                    isKeep = true;
                    if(badBin0)
                    {
                        if(badBin1)
                        {
                            badBin = bin - 3;
                            if(badBin <= 0) badBin=1;
                            //std::cout << "Success! Bin: " << bin << std::endl;
                            break;
                        }
                        badBin1 = true;
                    }
                    badBin0 = true;
                }
                else
                {
                    badBin0 = false;
                    badBin1 = false;
                    isKeep  = false;
                }
            }
            if(!isKeep){
                oldMean = dynamicMean;
                oldRMS  = dynamicRMS;
            }
        }
        for(int bin=1; bin<=badBin; bin++)
            EfficiencyVsEventGood_[it->first]->SetBinContent(bin, it->second->GetBinContent(bin));
        EfficiencyVsEventGood_[it->first]->GetXaxis()->SetRange(0, badBin);

        //std::cout << "badBin: " << badBin << std::endl;
        EfficiencyVsEventGood2D_[it->first]->GetXaxis()->Set(badBin, 0, 250*badBin); //
        for(int bin=0; bin<=badBin; bin++){
            //std::cout << "Bin: " << bin << std::endl;
            EfficiencyVsEventGood2D_[it->first]->Fill(250*bin, it->second->GetBinContent(bin));
        }

        //EfficiencyVsEventGood2D_[it->first]->GetXaxis()->SetLimits(0, badBin*250);
        //std::cout << "it->first " <<  it->first << std::endl;
        //std::cout << "Range: " << EfficiencyVsEventGood2D_[it->first]->GetXaxis()->GetXmax() << std::endl;
    }
    //std::cout << __PRETTY_FUNCTION__ << "Done here" << std::endl;
    //for(std::map<int,TH2F*>::iterator it=EfficiencyVsEventGood2D_.begin(); it!=EfficiencyVsEventGood2D_.end(); ++it){
    //    latencyPlots(it->first);
    //}
}

