#include "../FittersLibrary/LangausFit.cpp"
#include <TH1F.h>
#include <TF1.h>
#include <TDirectory.h>

void DefaultChargeFitter(TFile* inFile, std::string detectorName)
{
  
  TH1F* h1 = (TH1F*)inFile->Get(("Charge/" + detectorName + "/Landau/hLandauClusterSize1_" + detectorName).c_str());
  double fitParameters[4];
  LangausFit(h1,fitParameters);
  cout << "Width: "  << fitParameters[0] << endl;
  cout << "MPV: "    << fitParameters[1] << endl;
  cout << "Area: "   << fitParameters[2] << endl;
  cout << "GSigma: " << fitParameters[3] << endl;

  //TH1F* h2 = (TH1F*)inFile->Get(("Charge/" + detectorName + "/Landau/hCellLandau_" + detectorName).c_str());
  //h2->Fit("gaus");
  
}
