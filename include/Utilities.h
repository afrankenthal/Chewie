#ifndef UTILITIES_H
#define UTILITIES_H
#include <vector>
using std::vector;
class TF1;
class TH1F;
class TH2F;

namespace Utilities
{
   double langaus                              (double *x, double *par);
   double calibrationFitFunk                   (double *x, double *par);
   double calibrationFitFunkInv                (double *x, double *par);

   double timeEtaFitFunc                       (double y,  double *par);
   double timeEtaFitFunc2                      (double y,  double *par);
   double integrandEtaFitFunc                  (double x,  double *par);
   double integrandEtaFitFunc2                 (double x,  double *par);
   double integrandEtaFitFunc                  (double *x, double *par);
   double integrandEtaFitFunc2                 (double *x, double *par);
   double integrandEtaFitFunc2Angle            (double  x, double *par);
   double integrandEtaFitFunc2Angle            (double *x, double *par);
   double integrandEtaFitFuncScale             (double *x, double *par);
   double integrandEtaFitFunc2Scale            (double *x, double *par);
   double etaFitFunc                           (double *x, double *par);
   double etaFitFunc2                          (double *x, double *par);
   double etaFitFunc_cut                       (double *x, double *par);
   double etaFitFunc_cut2                      (double *x, double *par);
   double etaInverseFitFunc                    (double *x, double *par);
   double etaInverseFitFunc2                   (double *x, double *par);
   double etaInverseFitFunc2Angle              (double *x, double *par);
   double gaussCutByConstant                   (double *x, double *par);
   double gaussCutByConstantNorm               (double *x, double *par);
   double uniformSideSmeared                   (double *x, double *par);
   double uniformSideSmearedNorm               (double *x, double *par);
   double uniformCenterSmeared                 (double *x, double *par);
   double uniformCenterSmearedNorm             (double *x, double *par);
   double uniformCenterSmearedPlusConstant     (double *x, double *par);
   double uniformCenterSmearedPlusConstantNorm (double *x, double *par);
   double etaGaussConvoluted                   (double *x, double *par);
   double etaSmeared                           (double *x, double *par);
   double gaussPlusConstant                    (double *x, double *par);
   double gaussPlusConstantNorm                (double *x, double *par);
   double etaReproduction                      (double *x, double *par);
   double etaDistribution                      (double *x, double *par);
   double etaDistributionAngle                 (double *x, double *par);
   double doubleUniformSmeared                 (double *x, double *par);
   double singleUniformSmeared                 (double *x, double *par);
   double smearedSquare                        (double *x, double *par);
   //void   customProfileX                       (TH2F *h2DHist, vector<TH1F*>&profile, const char* outputName, const char* outputTitle, int yBinMin=1, int yBinMax=-1, int minNumberOfEntries=1);
   void   customProfileX                       (TH2F *h2DHist, TH1F *profile, const char* outputName, const char* outputTitle, int yBinMin=1, int yBinMax=-1, int minNumberOfEntries=1);

   double chi2Eta                              (double *par, TH1F *data);
}

#endif // UTILITIES_H
