#include <Data.h>

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <math.h>
#include <iostream>

using namespace std;

int main()
{
  gStyle->SetOptStat(111111);
  TFile* inRootFile = TFile::Open("Converted/Run458_Merged_Converted.root", "READ");
  TTree* theEventsTree;
  Data   theEvent;
  
  if ( !(theEventsTree = (TTree*)inRootFile->Get("CaptanTrack")) )
  {
    std::cout << "Couldn't find tree!" << std::endl; 
  }
  
  theEvent.setBranchAddress(theEventsTree);
  
  TFile* outRootFile = TFile::Open("Histos.root", "RECREATE");
  TH1F* hXErrors = new TH1F("xErrors", "Projected X Errors", 200, 0, 20);
  hXErrors->GetXaxis()->SetTitle("X Track Errors on Dut (#mum)");
  //hXErrors->GetYaxis()->SetTitle("# of tracks" );
  hXErrors->GetXaxis()->SetRangeUser(4,13);
  hXErrors->SetBit(TH1::kNoTitle);
  
  TH1F* hYErrors = new TH1F("yErrors", "Projected Y Errors", 200, 0, 20);
  hYErrors->GetXaxis()->SetTitle("Y Track Errors on Dut (#mum)");
  //hYErrors->GetYaxis()->SetTitle("# of tracks" );
  hYErrors->GetXaxis()->SetRangeUser(4,13);
  hYErrors->SetBit(TH1::kNoTitle);
  
  
  unsigned int numberOfPlanes;
  double xTotalError = 0;
  double yTotalError = 0;
  for(unsigned int e=0; e<theEventsTree->GetEntries(); e++)
  {
    theEventsTree->GetEntry(e);
    numberOfPlanes = 0;
    for(unsigned int p=0; p<8; p++)//9 and 10 are the DUTs
    {
      if(theEvent.getHasHit(p) 
      && theEvent.getClusterSize(p) <= 2
      && (theEvent.getNumberOfCols(p) == 1 || theEvent.getNumberOfRows(p) == 1)
      )
        numberOfPlanes++;
    }
    if(numberOfPlanes == 8 && theEvent.getChi2()/theEvent.getNdof() < 10 && theEvent.getNumberOfTracks() == 1)
    {
      hXErrors->Fill(theEvent.getXSigmaIntercept());
      xTotalError += theEvent.getXSigmaIntercept()*theEvent.getXSigmaIntercept();
      hYErrors->Fill(theEvent.getYSigmaIntercept());
      yTotalError += theEvent.getYSigmaIntercept()*theEvent.getYSigmaIntercept();
    }
  }
  xTotalError = sqrt(xTotalError/hXErrors->GetEntries());
  yTotalError = sqrt(yTotalError/hYErrors->GetEntries());
  
  cout << "X error: " << xTotalError << " Y error: " << yTotalError << endl;
  
  TCanvas canvas("Canvas", "Errors", 800, 600);
  hXErrors->Draw();
  canvas.Print("XErrors_selected.jpg");
  hYErrors->Draw();
  canvas.Print("YErrors_selected.jpg");

  outRootFile->Write();
  outRootFile->Close();
  
  inRootFile->Close();
  return 0;
}

