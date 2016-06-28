//------------------------------------------------------------------------------
/* INFORMATION: */
//------------------------------------------------------------------------------
#ifndef FIT_h
#define FIT_h

#include <TH1D.h>
#include <TF1.h>
#include <TObjArray.h>

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooCmdArg.h>
#include <RooAddPdf.h>
#include <RooArgList.h>
#include <RooGaussian.h>
#include <RooDataHist.h>


using namespace RooFit;
using namespace std;

class Fit
{
public:

  /* Constructor, destructor. */ 
  //----------------------------------------------------------------------------
  Fit();
  virtual ~Fit();
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  void min_max(int length, double *xData, double *yData, 
               int flag, double *xResult, double *yResult, int *index);
  int  indexMinMax(int length, double* data, int flag);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  RooFitResult* nGaussFit(int NPeak, TH1D *hist,
                      double *left, double *right, double *center,
                      double **arrMean, double **arrSigma);
  void splitHist(int *NPeak, TH1D *hist, 
                 double *left, double *right, double *center);
  //----------------------------------------------------------------------------
};
#endif
