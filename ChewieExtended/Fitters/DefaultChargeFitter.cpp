#include <TH1F.h>
#include <TDirectory.h>

void DefaultChargeFitter(TFile* inFile, std::string detectorName)
{
  
  TH1F* h1 = (TH1F*)inFile->Get(("Charge/"+detectorName+"/Landau/hLandauClusterSize1_"+detectorName).c_str());
  h1->Fit("gaus");

  TH1F* h2 = (TH1F*)inFile->Get("Charge/Dut0/Landau/hCellLandau_Dut0");
  h2->Fit("gaus","Q");
  
}
