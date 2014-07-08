#include <TH1F.h>
#include <TF1.h>
#include <TDirectory.h>

void DefaultChargeFitter(TFile* inFile, std::string detectorName)
{
  
  TH1F* h1 = (TH1F*)inFile->Get(("Charge/" + detectorName + "/Landau/hLandauClusterSize1_" + detectorName).c_str());
  TF1* fGaus = new TF1("fGaus","gaus",0,50000);
  h1->Fit(fGaus);

  TH1F* h2 = (TH1F*)inFile->Get(("Charge/" + detectorName + "/Landau/hCellLandau_" + detectorName).c_str());
  h2->Fit("gaus");
  
}
