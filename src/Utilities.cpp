#include "Utilities.h"

#include <iostream>

#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"

////////////////////////////////////////////////////////////////////////////////////
double Utilities::langaus(double *x, double *par)
{
     double invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
     double mpshift  = -0.22278298;       // Landau maximum location

     // Control constants
     double np = 100.0;      // number of convolution steps
     double sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

     // Variables
     double xx;
     double mpc;
     double fland;
     double sum = 0.0;
     double xlow,xupp;
     double step;
     double i;


     // MP shift correction
     mpc = par[1] - mpshift * par[0];

     // Range of convolution integral
     xlow = x[0] - sc * par[3];
     xupp = x[0] + sc * par[3];

     step = (xupp-xlow) / np;

     // Convolution integral of Landau and Gaussian by sum
     for(i=1.0; i<=np/2; i++) {
        xx = xlow + (i-.5) * step;
        fland = TMath::Landau(xx,mpc,par[0]) / par[0];
        sum += fland * TMath::Gaus(x[0],xx,par[3]);

        xx = xupp - (i-.5) * step;
        fland = TMath::Landau(xx,mpc,par[0]) / par[0];
        sum += fland * TMath::Gaus(x[0],xx,par[3]);
     }

     return (par[2] * step * sum * invsq2pi / par[3]);
}

////////////////////////////////////////////////////////////////////////////////////
double Utilities::calibrationFitFunk(double *x, double *par)
{
    return par[0]+par[1]*tanh(par[2]*x[0]+par[3]);
}

////////////////////////////////////////////////////////////////////////////////////
double Utilities::calibrationFitFunkInv(double *x, double *par)
{
    return (atanh((x[0]-par[0])/par[1]) - par[3])/par[2];
}

////////////////////////////////////////////////////////////////////////////////////
double Utilities::timeEtaFitFunc (double y, double *par)
{
    // Comparing with the Belau reference, page 257 equation 4: par[0] = d, par[1] = 2*mu*U_D, par[2] = U_D, par[3] = U + U_D
    // y is the same as the reference

    return (-1)*(par[0]*par[0]/par[1])*log(1 - (2*par[2]*(par[0] - y))/(par[3]*par[0]));
}

double Utilities::timeEtaFitFunc2 (double y, double *par)
{
    // This is the same as in reference, with the fact that we brought U_D to 0 (as a limit)
    // Comparing with the Belau reference, page 257 equation 4: par[0] = d, par[1] = d/2*mu*U
    // y is the same as the reference

    return (par[1])*(par[0] - y);
}
////////////////////////////////////////////////////////////////////////////////////

double Utilities::integrandEtaFitFunc (double x, double *par)
{
    //Comparing with the reference above, equation 5 (we don't consider delta contribution, eq. 6): par[0] = d, par[1] par[2] par[3] used for the time function (above), par[4] = 4*D_h

    double f = 0; //the f(x) defined in reference
    double step = 1;
    for (double y = 0; y < par[0]; y += step)
    {
        f += (1/(sqrt(timeEtaFitFunc(y, par))))*exp((-1)*x*x/(par[4]*timeEtaFitFunc(y, par)))*step;
    }

    f = f/(par[0]*sqrt(3.14159265359*par[4]));
    return f;
}

double Utilities::integrandEtaFitFunc2 (double x, double *par)
{
    //done with timeEtaFitFunc2 (U_D ->0)
    //Comparing with the reference above, equation 5 (we don't consider delta contribution, eq. 6): par[0] = d, par[1] used for the time function (above), par[2] = 4*D_h

    double f = 0; //the f(x) defined in reference
    double step = 1;
    for (double y = 0; y < par[0]; y += step)
    {
        f += (1/(sqrt(timeEtaFitFunc2(y, par))))*exp((-1)*x*x/(par[2]*timeEtaFitFunc2(y, par)))*step;
    }

    f = f/(par[0]*sqrt(3.14159265359*par[2]));
    return f;
}

double Utilities::integrandEtaFitFunc (double *x, double *par)
{
    //Comparing with the reference above, equation 5 (we don't consider delta contribution, eq. 6): par[0] = d, par[1] par[2] par[3] used for the time function (above), par[4] = 4*D_h

    double f = 0; //the f(x) defined in reference
    double step = 1;
    for (double y = 0; y < par[0]; y += step)
    {
        f += (1/(sqrt(timeEtaFitFunc(y, par))))*exp((-1)*x[0]*x[0]/(par[4]*timeEtaFitFunc(y, par)))*step;
    }

    f = f/(par[0]*sqrt(3.14159265359*par[4]));
    return f;
}

double Utilities::integrandEtaFitFunc2 (double *x, double *par)
{
    //done with timeEtaFitFunc2 (U_D ->0)
    //Comparing with the reference above, equation 5 (we don't consider delta contribution, eq. 6): par[0] = d, par[1] used for the time function (above), par[2] = 4*D_h

    double f = 0; //the f(x) defined in reference
    double step = 1;
    for (double y = 0; y < par[0]; y += step)
    {
        f += (1/(sqrt(timeEtaFitFunc2(y, par))))*exp((-1)*x[0]*x[0]/(par[2]*timeEtaFitFunc2(y, par)))*step;
    }

    f = f/(par[0]*sqrt(3.14159265359*par[2]));
    return f;
}

double Utilities::integrandEtaFitFuncScale (double *x, double *par)
{
    //Comparing with the reference above, equation 5 (we don't consider delta contribution, eq. 6): par[0] = d, par[1] par[2] par[3] used for the time function (above), par[4] = 4*D_h, par[5] a scale factor

    double f = 0; //the f(x) defined in reference
    double step = 1;
    for (double y = 0; y < par[0]; y += step)
    {
        f += (1/(sqrt(timeEtaFitFunc(y, par))))*exp((-1)*x[0]*x[0]/(par[4]*timeEtaFitFunc(y, par)))*step;
    }

    f = f/(par[0]*sqrt(3.14159265359*par[4]));
    return par[5]*f;
}

double Utilities::integrandEtaFitFunc2Scale (double *x, double *par)
{
    //done with timeEtaFitFunc2 (U_D ->0)
    //Comparing with the reference above, equation 5 (we don't consider delta contribution, eq. 6): par[0] = d, par[1] used for the time function (above), par[2] = 4*D_h

    double f = 0; //the f(x) defined in reference
    double step = 1;
    for (double y = 0; y < par[0]; y += step)
    {
        f += (1/(sqrt(timeEtaFitFunc2(y, par))))*exp((-1)*x[0]*x[0]/(par[2]*timeEtaFitFunc2(y, par)))*step;
    }

    f = f/(par[0]*sqrt(3.14159265359*par[2]));
    return par[3]*f;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Utilities::integrandEtaFitFunc2Angle (double x, double *par)
{
    //done with timeEtaFitFunc2 (U_D ->0) and an angle of entrance
    //Comparing with the reference above, equation 5 (we don't consider delta contribution, eq. 6): par[0] = d, par[1] used for the time function 2), par[2] = 4*D_h, par[3] = scale factor, par[4] = tan(theta)

    double f = 0; //the f(x) defined in reference
    double step = 1;
    for (double y = 0; y < par[0]; y += step)
    {
        f += (1/(sqrt(timeEtaFitFunc2(y, par))))*exp((-1)*(x-y*par[4])*(x-y*par[4])/(par[2]*timeEtaFitFunc2(y, par)))*step;
    }

    f = par[3]*f/(par[0]*sqrt(3.14159265359*par[2]));
    return f;
}

double Utilities::integrandEtaFitFunc2Angle (double *x, double *par)
{
    //done with timeEtaFitFunc2 (U_D ->0) and an angle of entrance
    //Comparing with the reference above, equation 5 (we don't consider delta contribution, eq. 6): par[0] = d, par[1] used for the time function 2), par[2] = 4*D_h, par[3] = scale factor, par[4] = tan(theta)

    double f = 0; //the f(x) defined in reference
    double step = 1;
    for (double y = 0; y < par[0]; y += step)
    {
        f += (1/(sqrt(timeEtaFitFunc2(y, par))))*exp((-1)*(x[0]-y*par[4])*(x[0]-y*par[4])/(par[2]*timeEtaFitFunc2(y, par)))*step;
    }

    f = par[3]*f/(par[0]*sqrt(3.14159265359*par[2]));
    return f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Utilities::etaFitFunc (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc plus par[5] a scale factor
    //we have only x[0] = x to take in account as variable

    double step = 1;
    double inf = -500; //should be -infinity, hopefully it's enough compared with the lenght of a cell (100-150 um)
    double sup = 500; //should be +infinity, same thing as above

    double etaSx = 0;
    for (double p = inf; p < x[0]; p += step)
    {
        etaSx += integrandEtaFitFunc(p, par)*step;
    }

    double etaDx = 0;
    for (double q = x[0]; q < sup; q += step)
    {
        etaDx += integrandEtaFitFunc(q, par)*step;
    }

    double eta = (etaDx - etaSx)/(etaDx + etaSx);
    return par[5]*eta;
}

double Utilities::etaFitFunc2 (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc2 plus par[3] a scale factor
    //we have only x[0] = x to take in account as variable
/*
    double step = 1;
    double inf = -500; //should be -infinity, hopefully it's enough compared with the lenght of a cell (100-150 um)
    double sup = 500; //should be +infinity, same thing as above

    double etaSx = 0;
    for (double p = inf; p < x[0]; p += step)
    {
        etaSx += integrandEtaFitFunc2(p, par)*step;
    }

    double etaDx = 0;
    for (double q = x[0]; q < sup; q += step)
    {
        etaDx += integrandEtaFitFunc2(q, par)*step;
    }

    double eta = (etaDx - etaSx)/(etaDx + etaSx);
    return par[3]*eta;
*/
    double step = 0.1;
//    double inf = -1;
//    double sup = +1;

    double etaValue = 0;

    double lim;
    if (x[0] >= 0) lim =  x[0];
    else           lim = -x[0];

    for (double e = 0; e < lim; e += step)
    {
        etaValue += integrandEtaFitFunc2(e, par)*step;
    }

    if (x[0] >= 0) return (-2)*par[3]*etaValue;
    else           return    2*par[3]*etaValue;
}

double Utilities::etaFitFunc_cut (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc plus par[5] a scale factor and par[6] as the threshold under which the charge is not collected
    //we have only x[0] = x to take in account as variable

    double step = 1;
    double inf = -500; //should be -infinity, hopefully it's enough compared with the lenght of a cell (100-150 um)
    double sup = 500; //should be +infinity, same thing as above

    double etaSx = 0;
    for (double p = inf; p < x[0]; p += step)
    {
        etaSx += integrandEtaFitFunc(p, par)*step;
    }

    double etaDx = 0;
    for (double q = x[0]; q < sup; q += step)
    {
        etaDx += integrandEtaFitFunc(q, par)*step;
    }

    double eta = (etaDx - etaSx)/(etaDx + etaSx);
    if (etaDx < par[6]) return -1;
    else if (etaSx < par[6]) return +1;
    else return par[5]*eta;
}

double Utilities::etaFitFunc_cut2 (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc2 plus par[3] a scale factor and par[4] as the threshold under which the charge is not collected
    //we have only x[0] = x to take in account as variable

    double step = 1;
    double inf = -500; //should be -infinity, hopefully it's enough compared with the lenght of a cell (100-150 um)
    double sup = 500; //should be +infinity, same thing as above

    double etaSx = 0;
    for (double p = inf; p < x[0]; p += step)
    {
        etaSx += integrandEtaFitFunc2(p, par)*step;
    }

    double etaDx = 0;
    for (double q = x[0]; q < sup; q += step)
    {
        etaDx += integrandEtaFitFunc2(q, par)*step;
    }

    double eta = (etaDx - etaSx)/(etaDx + etaSx);
    if (etaDx < par[4]) return -1;
    else if (etaSx < par[4]) return +1;
    else return par[3]*eta;
}

double Utilities::etaInverseFitFunc (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc plus par[5] a scale factor
    //we have only x[0] = x to take in account as variable

    double step = 0.005;
//    double inf = -1;
//    double sup = +1;

    double etaValue = 0;
    double etaValuePrev = 0;

    double lim;
    if (x[0] >= 0) lim =  x[0];
    else           lim = -x[0];

    double deriv;

    for (double e = 0; e < lim; e += step)
    {
        deriv = 2*integrandEtaFitFunc(etaValuePrev, par);
        if (deriv != 0)
            etaValue += -(1/deriv)*step;
        etaValuePrev = etaValue;
//        std::cout << x[0] << " " << etaValue << "!!!\n";
    }

    if (x[0] >= 0) return      par[5]*etaValue;
    else           return (-1)*par[5]*etaValue;
}

double Utilities::etaInverseFitFunc2 (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc2 plus par[3] a scale factor
    //we have only x[0] = x to take in account as variable

    double step = 0.01;
//    double inf = -1;
//    double sup = +1;

    double etaValue = 0;
    double etaValuePrev = 0;

    for (double e = 0; e < fabs(x[0]); e += step)
    {
        etaValue += -(1/integrandEtaFitFunc2(etaValuePrev, par))*step;
        etaValuePrev = etaValue;
    }

    if (x[0] >= 0) return      par[3]*etaValue;
    else           return (-1)*par[3]*etaValue;
}

double Utilities::etaInverseFitFunc2Angle (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc2 plus par[3] a scale factor, par[4] tan(theta)
    //we have only x[0] = x to take in account as variable

    double step = 0.01;
//    double inf = -1;
//    double sup = +1;

    double etaValue = 0;
    double etaValuePrev = 0;
    double der = 0;

    for (double e = 0; e < fabs(x[0]); e += step)
    {
        der = 2*integrandEtaFitFunc2Angle(etaValuePrev, par);
        if (der != 0)
            etaValue += -(1/der)*step;
        etaValuePrev = etaValue;
    }

    if (x[0] >= 0) return      par[3]*etaValue;
    else           return (-1)*par[3]*etaValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Utilities::chi2Eta (double *par, TH1F* data)
{
    double chi2 = 0;
    double * x0 = new double;
    for (int i =1; i < data->GetXaxis()->GetNbins()  +1; ++i)
    {
        *x0 = data->GetBinCenter(i);
        chi2 += (etaFitFunc_cut(x0, par) - data->GetBinContent(i))*(etaFitFunc_cut(x0, par) - data->GetBinContent(i));
    }

    return chi2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Utilities::gaussCutByConstant (double *x, double *par)
{
    //par[0] = lim sup fo integration (the extreme of the pixel, should always be constant; par[1] = lim inf, it depends on the cut applied on the charge; par[2] = gauss sigma, par[3] = scale factor
    //only x[0] = coordinate along pitch

    double step = 0.05;
    int   steps = (int)(fabs(par[1] - par[0])/step);
    double g = 0;

    for (double y = -par[0]; y <= par[0]; y += step)
    {
        g += TMath::Gaus(x[0], y, par[2], true)*step;
    }
/*
    if (par[0] > par[1])
    {
//        for (int n=0; n<steps; n++)
//        {
//            g += TMath::Gaus(x[0], par[1]+n*step, par[2], true)*step;
//        }
        for (double y = par[1]; y <= fabs(10*par[0]); y += step)
        {
            g += TMath::Gaus(x[0], y, par[2], true)*step;
        }
    }
    else
    {
//        for (int n=0; n<steps; n++)
//        {
//            g += TMath::Gaus(x[0], -par[0]+n*step, par[2], true)*step;
//        }
        for (double y = -fabs(10*par[0]); y <= par[1]; y += step)
        {
            g += TMath::Gaus(x[0], y, par[2], true)*step;
        }
    }
*/
    return par[3]*g;
}

double Utilities::gaussCutByConstantNorm (double *x, double *par)
{
    //par[0] = lim sup fo integration (the extreme of the pixel, should always be constant; par[1] = lim inf, it depends on the cut applied on the charge; par[2] = gauss sigma, par[3] = scale factor
    //only x[0] = coordinate along pitch

    double step = 1;
    double g = 0;
    if (par[0] >= 0)
    {
        for (double y = 0; y <= par[0]; y += step)
        {
            g += gaussCutByConstant(&y, par)*step;
        }
    }
    else
    {
        for (double y = par[0]; y <= 0; y += step)
        {
            g += gaussCutByConstant(&y, par)*step;
        }
    }

    return gaussCutByConstant(x, par)/g;
}

//======================================================================================================================================================
double Utilities::uniformSideSmeared (double *x, double *par)
{
    //par[0] = lim sup fo integration (the extreme of the pixel, should always be constant; par[1] = lim inf, it depends on the cut applied on the charge; par[2] = gauss sigma, par[3] = scale factor
    //only x[0] = coordinate along pitch

    double parOtherSide[4];
    parOtherSide[0] = -par[0];
    parOtherSide[1] = -par[1];
    parOtherSide[2] =  par[2];
    parOtherSide[3] =  par[3];

    return gaussCutByConstant(x, par);// + gaussCutByConstant(x, parOtherSide);
}

double Utilities::uniformSideSmearedNorm (double *x, double *par)
{
    //par[0] = lim sup fo integration (the extreme of the pixel, should always be constant; par[1] = lim inf, it depends on the cut applied on the charge; par[2] = gauss sigma, par[3] = scale factor
    //only x[0] = coordinate along pitch

/*    double step = 1;
    double g = 0;
    for (double y = -par[0]; y <= par[0]; y += step)
    {
        g += uniformSideSmeared(&y, par)*step;
    }
*/
    return uniformSideSmeared(x, par)/(par[3]*sqrt(2*3.14159265359)*par[2]*TMath::Erfc(par[1]/par[2]));
}

//======================================================================================================================================================
double Utilities::uniformCenterSmeared (double *x, double *par)
{
    //par[0] = lim of extension of the constant function; par[1] = gauss sigma; par[2] = scale factor
    //only x[0] = coordinate along pitch

    double step = 0.05;
    double g = 0;
    for (double y = -par[0] + par[3]; y <= par[0] + par[3]; y += step)
    {
        g += TMath::Gaus(x[0], y, par[1], true)*step;
    }

    return par[2]*g;
}

double Utilities::uniformCenterSmearedNorm (double *x, double *par)
{
    //par[0] = lim of extension of the constant function; par[1] = gauss sigma; par[2] = scale factor
    //only x[0] = coordinate along pitch
/*
    double step = 1;
    double g = 0;
    for (double y = -par[0] - 4*par[1]; y <= par[0] + 4*par[1]; y += step)
    {
        g += uniformCenterSmeared(&y, par)*step;
    }
*/
    return uniformCenterSmeared(x, par)/(par[2]*sqrt(2*3.14159265359)*par[1]*TMath::Erf(par[0]/par[1]));
}

//======================================================================================================================================================
double Utilities::uniformCenterSmearedPlusConstant (double *x, double *par)
{
    //par[0] = lim of extension of the constant function; par[1] = gauss sigma; par[2] = scale factor, par[3] = horizontal translation constant, par[4] = vertical translation constant
    //only x[0] = coordinate along pitch

    return uniformCenterSmeared(x, par) + par[4];
}

double Utilities::uniformCenterSmearedPlusConstantNorm (double *x, double *par)
{
/*    double step = 1;
    double g = 0;
    for (double y = -50; y <= 50; y += step)
    {
        g += uniformCenterSmearedPlusConstant(&y, par)*step;
    }

    return uniformCenterSmearedPlusConstant(x, par)/g; */

    return uniformCenterSmearedPlusConstant(x, par)/(par[2]*sqrt(2*3.14159265359)*par[1]*TMath::Erf(par[0]/par[1]) + 100*par[4]);
}

//======================================================================================================================================================
double Utilities::etaGaussConvoluted (double *x, double *par)
{
    //the first 5 params are the same as in etaFitFunc_cut2, par[5] is the gaussian sigma
    //only x[0] as usual

    double step = 0.5;
    double inf = -3*par[5]; //should be -infinity, hopefully it's enough compared with the lenght of a cell (100-150 um)
    double sup = 3*par[5]; //should be +infinity, same thing as above

    double g = 0;
    double * pos = new double;
    for (double y = inf; y <= sup; y += step)
    {
        *pos = x[0] - y;
        g += etaFitFunc_cut2(pos, par)*TMath::Gaus(y, 0, par[5], true)*step;
    }
    return g;
}

//======================================================================================================================================================
double Utilities::etaSmeared (double *x, double *par)
{
    //the first 4 params are the same as in etaFitFunc2, the other 5 the same as uniforCenterSmearedPlusConstant
    //only x[0] as usual

    double step = 5;
    double inf = -10.2*4; //should be -infinity, hopefully it's enough compared with the lenght of a cell (100-150 um)
    double sup = 10.2*4; //should be +infinity, same thing as above
    double parD[5];
    parD[0] = par[4];
    parD[1] = par[5];
    parD[2] = par[6];
    parD[3] = par[7];
    parD[4] = par[8];

    double g = 0;
//    double sum = 0;
    double * pos = new double;
//    double * pos2 = new double;
    for (double y = inf; y <= sup; y += step)
    {
        *pos = x[0] - y;
//        *pos2 = y;
        g += etaFitFunc2(pos, par)*TMath::Gaus(y, 0, 10.2, true)*step;
//        sum += TMath::Gaus(y, 0, 10.2, true)*step;
//        std::cout << g << "!!!\n";
    }
//    std::cout << "Calculation done!!!\n";
//    std::cout << "Total area (should be 1) = " << sum << "!!!\n";
//    std::cout << x[0] << ": " << (2./13.)*98./(1300.*50.)*fabs(x[0])*log(fabs(50./(fabs(x[0])+0.0001) -1.)) << "!!!\n";
    return g*(uniformCenterSmeared(x, parD)/(uniformCenterSmeared(x, parD) + 98.)) + (2.*98./(uniformCenterSmeared(x, parD) + 98.))*98./(1300.*50.)*(-x[0])*log(fabs(100./(fabs(x[0])) +1.));
}

//======================================================================================================================================================
double Utilities::gaussPlusConstant (double *x, double *par)
{
    return par[2]*TMath::Gaus(x[0], par[0], par[1], false) + par[3];
}

double Utilities::gaussPlusConstantNorm (double *x, double *par)
{
    double g = par[2]*TMath::Gaus(x[0], par[0], par[1], false) + par[3];
    return g/(par[2]*par[1]*sqrt(2*3.14159265359) + par[3]*100);
}

//======================================================================================================================================================
double Utilities::etaReproduction (double *x, double *par)
{
    //first 4 pars taken from etaFitFunc2, par[4] = lim of clustersize 1 distribution, par[5] gaussian sigma (side), par[6] scale factor (side), par[7] gaussian sigma (center), par[8] scale factor for gaussian of size 2, par[9] the center constant horizontal translation, par[10] constant vertical translation for cluster size 2
    //only x[0] as usual

    double parEta1[4];
    double parEta2[9];
    double parGauss[5];
    parGauss[0] = par[4];
    parGauss[1] = par[7];
    parGauss[2] = par[8];
    parGauss[3] = par[9];
    parGauss[4] = par[10];
    parEta1[0] = 50;
    parEta1[1] = par[4];
    parEta1[2] = par[5];
    parEta1[3] = par[6];
    parEta2[0] = par[0];
    parEta2[1] = par[1];
    parEta2[2] = par[2];
    parEta2[3] = par[3];
    parEta2[4] = parGauss[0];
    parEta2[5] = parGauss[1];
    parEta2[6] = parGauss[2];
    parEta2[7] = parGauss[3];
    parEta2[8] = parGauss[4];
    double probSize1, probSize2, probDelta;
/*    if (x[0] > 0.) {
        parEta1[0] = 50;
        parEta1[1] = par[4];
        parEta1[2] = par[5];
        parEta1[3] = 1.;
        probSize1 = gaussCutByConstantNorm(x, parEta1)               /(gaussCutByConstantNorm(x, parEta1)+ uniformCenterSmearedPlusConstantNorm(x, parGauss));
        probSize2 = uniformCenterSmearedPlusConstantNorm(x, parGauss)/(gaussCutByConstantNorm(x, parEta1)+ uniformCenterSmearedPlusConstantNorm(x, parGauss));
        return -probSize1 + etaSmeared(x, parEta2)*probSize2;
    }
    else if (x[0] < 0.) {
        parEta1[0] = -50;
        parEta1[1] = -par[4];
        parEta1[2] = par[5];
        parEta1[3] = 1.;
        probSize1 = gaussCutByConstantNorm(x, parEta1)/(gaussCutByConstantNorm(x, parEta1)+uniformCenterSmearedPlusConstantNorm(x, parGauss));
        probSize2 = uniformCenterSmearedPlusConstantNorm(x, parGauss)/(gaussCutByConstantNorm(x, parEta1)+uniformCenterSmearedPlusConstantNorm(x, parGauss));
        return probSize1 + etaSmeared(x, parEta2)*probSize2;
    }
    else return 0; */
        probSize1 = uniformSideSmeared  (x, parEta1 )/(uniformSideSmeared(x, parEta1)+ uniformCenterSmearedPlusConstant(x, parGauss));
        probSize2 = uniformCenterSmeared(x, parGauss)/(uniformSideSmeared(x, parEta1)+ uniformCenterSmearedPlusConstant(x, parGauss)); //plus constant at numerator if we don't count deltas
        probDelta = par[10]                          /(uniformSideSmeared(x, parEta1)+ uniformCenterSmearedPlusConstant(x, parGauss));
//        std::cout << "Prob size 1: " << probSize1 << ", prob size 2: " << probSize2 << "// Size 1 value: " << uniformSideSmeared(x, parEta1) << ", size 2 " << uniformCenterSmearedPlusConstant(x, parGauss) << "!!!\n";
        if      (x[0] > 0) return -probSize1 + etaSmeared(x, parEta2)*probSize2 + 2*par[10]/(par[6]*par[0])*( x[0])*log(fabs(par[0]/( x[0]) -1));
        else if (x[0] < 0) return  probSize1 + etaSmeared(x, parEta2)*probSize2 + 2*par[10]/(par[6]*par[0])*(-x[0])*log(fabs(par[0]/(-x[0]) -1));
        else return 0;
}

//======================================================================================================================================================
double Utilities::etaDistribution (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc2 plus par[3] a scale factor, par[4] a final scale factor
    //we have only x[0] = x to take in account as variable

//    double step = 0.000001;
//    double inf = -1;
//    double sup = +1;

/*    double etaValue = 0;
    double etaValuePrev = 0;

    for (double e = 0; e < x[0]; e += step)
    {
        etaValue += (1/integrandEtaFitFunc2(etaValuePrev, par))*step;
        etaValuePrev = etaValue;
    }

    if (x[0] >= 0) return      par[3]*etaValue;
    else           return par[3]*etaValue;   */

    double * e = new double;
    *e = etaInverseFitFunc2(x, par);
    double c = par[3]/integrandEtaFitFunc2(e, par);
    std::cout << x[0] << ": etaInverse =  " << *e << ", value= " << c << "!!\n";
    delete e;
    return par[4]*c;
}

double Utilities::etaDistributionAngle (double *x, double *par)
{
    //the assignment of the parameters is the same as in integrandEtaFitFunc2 plus par[3] a scale factor, par[4] tan(theta), par[5] a final scale factor
    //we have only x[0] = x to take in account as variable

//    double step = 0.000001;
//    double inf = -1;
//    double sup = +1;

/*    double etaValue = 0;
    double etaValuePrev = 0;

    for (double e = 0; e < x[0]; e += step)
    {
        etaValue += (1/integrandEtaFitFunc2(etaValuePrev, par))*step;
        etaValuePrev = etaValue;
    }

    if (x[0] >= 0) return      par[3]*etaValue;
    else           return par[3]*etaValue;   */

    double * e = new double;
    *e = etaInverseFitFunc2Angle(x, par);
    double c = par[3]/integrandEtaFitFunc2Angle(e, par);
    std::cout << x[0] << ": etaInverse =  " << *e << ", value= " << c << "!!\n";
    delete e;
    return par[5]*c;
}

//======================================================================================================================================================
double Utilities::doubleUniformSmeared (double *x, double *par)
{
    //par0 pixel extension, par1 center of distribution, par2 relative center of bumbs, par3 width of bumbs, par4 max heigh, par5 min height, par6 sigma
    /* ____    _____    ____
           |__|     |__|
    */
    double step = 0.05;
    double g = 0;
//    for (double y = -par[0] - 2*par[3]; y <= par[0] + 2*par[3]; y += step)
    for (double y = -par[0] - 3*par[6] + par[1]; y < par[1] - par[2] -par[3]/2; y += step)
    {
        g += par[4]*TMath::Gaus(x[0], y, par[6], true)*step;
    }
    for (double y = -par[1] - par[2] - par[3]/2; y < par[1] - par[2] + par[3]/2; y += step)
    {
//        g += par[5]*TMath::Gaus(x[0], y, par[6], true)*2*sqrt((par[3]*par[3]/4) - ((y+par[2]-par[1])*(y+par[2]-par[1])))*step/(par[3]);
        g += TMath::Gaus(x[0], y, par[6], true)*(par[4] -(2*(par[5])*(sqrt((par[3]*par[3]/4) - ((y+par[2]-par[1])*(y+par[2]-par[1])))/60)))*step;
    }
    for (double y = par[1] - par[2] + par[3]/2; y < par[1] + par[2] - par[3]/2; y += step)
    {
        g += par[4]*TMath::Gaus(x[0], y, par[6], true)*step;
    }
    for (double y = par[1] + par[2] - par[3]/2; y < par[1] + par[2] + par[3]/2; y += step)
    {
//        g += par[5]*TMath::Gaus(x[0], y, par[6], true)*2*sqrt((par[3]*par[3]/4) - ((y-par[2]-par[1])*(y-par[2]-par[1])))*step/(par[3]);
        g += TMath::Gaus(x[0], y, par[6], true)*(par[4] -(2*(par[5])*(sqrt((par[3]*par[3]/4) - ((y-par[2]-par[1])*(y-par[2]-par[1])))/60)))*step;
    }
    for (double y = par[1] + par[2] + par[3]/2; y <= par[0] + 3*par[6] + par[1]; y += step)
    {
        g += par[4]*TMath::Gaus(x[0], y, par[6], true)*step;
    }

    return g;
}

//======================================================================================================================================================
double Utilities::singleUniformSmeared (double *x, double *par)
{
    //par0 pixel extension, par1 center of distribution, par2 width of bumbs, par3 max heigh, par4 min height, par5 sigma
    /* ____    ___
           |__|
    */
    double step = 0.05;
    double g = 0;
//    for (double y = -par[0] - 2*par[3]; y <= par[0] + 2*par[3]; y += step)
    for (double y = -par[0] - 3*par[5] + par[1]; y < par[1] -par[2]/2; y += step)
    {
        g += par[3]*TMath::Gaus(x[0], y, par[5], true)*step;
    }
    for (double y = par[1] - par[2]/2; y < par[1] + par[2]/2; y += step)
    {
//        g += par[5]*TMath::Gaus(x[0], y, par[6], true)*2*sqrt((par[3]*par[3]/4) - ((y+par[2]-par[1])*(y+par[2]-par[1])))*step/(par[3]);
        g += TMath::Gaus(x[0], y, par[5], true)*(par[3] -(2*(par[4])*(sqrt((par[2]*par[2]/4) - ((y-par[1])*(y-par[1])))/40)))*step;
    }
    for (double y = par[1] + par[2]/2; y <= par[0] + 3*par[5] + par[1]; y += step)
    {
        g += par[3]*TMath::Gaus(x[0], y, par[5], true)*step;
    }

    return g;
}
//=======================================================================================================================================================
double Utilities::smearedSquare(double *xx, double *par){
    //smearedSquare: square function convoluted with a gaussian
    //par[0] = normalization
    //par[1] = center of the function, look for misalignment
    //par[2] = sigma of the gaussian, contains telescope and DUT resolutions
    //par[3] = width of the square function, shuold be the width within no charge sharing occurs

    double step=0.03125;//1/2^5
    double func=0;

    for(double y=par[1]-par[3]/2.; y<=par[1]+par[3]/2.; y+=step){
      func += par[0] * TMath::Gaus(xx[0],y,par[2],kTRUE) * step;
              //par[0]*1/TMath::Sqrt(2*TMath::Pi()*par[2])*TMath::Exp(-0.5*(xx[0]-y)*(xx[0]-y)/(par[2]*par[2]))*step;
    }

    return func;

}

//=======================================================================================================================================================
//void Utilities::customProfileX(TH2F *h2DHist, vector<TH1F *>&prof, const char* outputName, const char* outputTitle, int yBinMin, int yBinMax, int minNumberOfEntries){
void Utilities::customProfileX(TH2F *h2DHist, TH1F *profile, const char* outputName, const char* outputTitle, int yBinMin, int yBinMax, int minNumberOfEntries){

    if(yBinMax<=0) yBinMax = h2DHist->GetNbinsY();
    if(yBinMin<=0) yBinMin = 1;

    int nXBin = h2DHist->GetNbinsX();

    //profile->SetNameTitle(outputName,outputTitle);
    //profile->SetBins(nXBin,h2DHist->GetBinLowEdge(1),h2DHist->GetBinWidth(1)*nXBin+h2DHist->GetBinLowEdge(1));
    //prof.push_back(new TH1F(outputName,outputTitle,nXBin,h2DHist->GetBinLowEdge(1),h2DHist->GetBinWidth(1)*nXBin+h2DHist->GetBinLowEdge(1)));
    //TH1F * &profile = prof.back();


    for(int i=1; i<=nXBin; i++){

        float mean=0.;
        float mean_std;
        float error=0.;
        float sigma=0.;
        float weight=0.;
        int   entries=0.;

        for(int j=yBinMin; j<=yBinMax ; j++){

            float z      = h2DHist->GetBinContent(i,j);
            float center = h2DHist->GetYaxis()->GetBinCenter(j);
            if(z==0) continue;
            weight = 1./z;
            mean+=center*weight;
            mean_std += center*z;
            error+=weight;
            sigma+=center*center*z;

            entries+= z;

        }

        if(entries<=minNumberOfEntries) continue;

        mean=mean/error;
        mean_std=mean_std/entries;
        error=TMath::Sqrt(1./error);
        sigma = TMath::Sqrt(sigma - mean_std*mean_std)/entries;

        profile->SetBinContent(i,mean_std);
        profile->SetBinError(i,sigma);

    }

    //profile->Fit("pol1","Q");

    return;

}
