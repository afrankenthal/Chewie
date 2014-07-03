#include <TH1F.h>
#include <TDirectory.h>

void DefaultChargeFitter(TFile* inFile)
{
  TH1F* h1 = (TH1F*)inFile->Get("Charge/Dut0/Landau/hLandauClusterSize1_Dut0");
  h1->Fit("gaus","Q");

  TH1F* h2 = (TH1F*)inFile->Get("Charge/Dut0/Landau/hCellLandau_Dut0");
  h2->Fit("gaus","Q");  

}
