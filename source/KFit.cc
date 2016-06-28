#include "KFit.hh"

/* Constructor, destructor. */ 
//------------------------------------------------------------------------------
/*! \fn KFit
* \brief   constructtor
*/
KFit::KFit()
{
}

/*! \fn ~KFit
* \brief  destructor
*/
KFit::~KFit()
{
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/*! \brief   Find min-max value
*
*   \param   int length (length array)
*   \param   int flag (-1 -> find min
*                      1 -> find max)
*
*   \return  double xData
*   \return  double yData
*   \return  double xResult
*   \return  double yResult
*   \return  double index
*
*/
void KFit::min_max(int length, double *xData, double *yData, int flag, double *xResult, double *yResult, int *index)
{
    *xResult = xData[0];
    *yResult = yData[0];
    for(int i = 1; i < length; i++) {
        if(flag == -1) {
            if(*yResult > yData[i]) {
                *xResult = xData[i];
                *yResult = yData[i];
                *index = i;
            }
        }
        else {
            if(*yResult < yData[i]) {
                *xResult = xData[i];
                *yResult = yData[i];
                *index = i;
            }
        }
    }
}

/*! \brief   Find index min-max value
*   \param   int length (length array)
*   \param   double* data (array data)
*   \param   int flag (-1 -> find min
*                      1 -> find max)
*   \return  int index
*/
int KFit::indexMinMax(int length, double* data, int flag)
{
    int index = 0;
    double result = data[0];
    for(int i = 1; i < length; i++) {
        if(flag == -1) {
            if(result > data[i]) {
                result = data[i];
                index = i;
            }
        }
        else {
            if(result < data[i]) {
                result = data[i];
                index = i;
            }
        }
    }
    return index;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/*! \brief   split Hist by windows
*
*   \param   int NPeak number of gauss function
*   \param   TH1D *hist (array data)

*
*   \return   double *left  array for lefts sides of windows
    \return   double *right  array for rights sides of windows
    \return   double *center  array for centers sides of windows
*/
void KFit::splitHist(int *NPeak, TH1D *hist, double *left, double *right, double *center)
{
  double xData[hist->GetNbinsX()];
  double yData[hist->GetNbinsX()];
  double yMaxBin;
  int index;
  for(unsigned short i = 0; i < hist->GetNbinsX(); i++) {
    xData[i] = hist->GetBinCenter(i);
    yData[i] = hist->GetBinContent(i);
  }
  double leftIndex;
  double rightIndex;
    int xmin = 1;
    int xmax = hist->GetNbinsX()-1;
  for(unsigned short nPeak = 0; nPeak < *NPeak; nPeak++) {
    min_max(hist->GetNbinsX(), xData, yData, 1, &center[nPeak], &yMaxBin, &index);
    if(yMaxBin == -1 || index < xmin || index > xmax) {
      *NPeak = nPeak;
      break;
    }
    for(unsigned short i = index; i >= xmin; i--) {
      if((yData[i] < yData[i-1]) || (yData[i] == 0) || i == xmin) {
        left[nPeak] = xData[i];
        leftIndex = i;
        break;
      }
    }
    for(unsigned short i = index; i <= xmax; i++) {
      if((yData[i] < yData[i+1]) || (yData[i] == 0) || i == xmax) {
        right[nPeak] = xData[i];
        rightIndex = i;
        break;
      }
    }

    for(unsigned short i = leftIndex; i <= rightIndex; i++)
      yData[i] = -1;
  }

    //delete

}

/*! \brief   Fit data with function: g1+g2+...+gN
*
*   \param   int NPeak number of gauss function
*   \param   RooAbsData* data (array data)

*   \return   RooAddPdf *sig - fit function
*   \return   double **arrMean [0] - value
*                              [1] - error
*   \return   double **arrSigma [0] - value
*                               [1] - error
*
*   \return   double *left  array for left side of window
    \return   double *right  array for right side of window
*/
RooFitResult* KFit::nGaussFit(int NPeak, TH1D *hist,
                    double *left, double *right, double *center,
                    double **arrMean, double **arrSigma)
{
    RooFitResult *result = 0;
    int indexLeft = indexMinMax(NPeak, left, -1);
    int indexRight = indexMinMax(NPeak, right, 1);
    RooRealVar x(hist->GetName(), hist->GetName(), left[indexLeft], right[indexRight]);
    RooAbsData *data = new RooDataHist("data","data", x, hist);

    TObjArray *objListPdf = new TObjArray();
    TObjArray *objListCoef = new TObjArray();

    RooRealVar *mean[NPeak];
    RooRealVar *sigma[NPeak];
    RooGaussian *gauss[NPeak];
    RooRealVar *coef[NPeak];
    for(unsigned short i = 0; i < NPeak; i++) {
      mean[i] = new RooRealVar(Form("mean_%d", i),
                               Form("mean_%d", i),
                               center[i], left[i], right[i]);
      sigma[i] = new RooRealVar(Form("sigma_%d", i),
                                Form("sigma_%d", i),
                                (right[i]-left[i])/3, 0, 100);
      gauss[i] = new RooGaussian(Form("gauss_%d", i),
                                 Form("gauss_%d", i),
                                 x, *mean[i], *sigma[i]);
      coef[i] = new RooRealVar(Form("coef_%d", i),
                               Form("coef_%d", i),
                               50000, 0, 1000000);

      objListPdf->Add(gauss[i]);
      objListCoef->Add(coef[i]);
    }

    RooArgList pdfList(*objListPdf, "pdfList");
    RooArgList coefList(*objListCoef, "coefList");
    RooAddPdf fun("fun","g1+g2+...+gN", pdfList, coefList);

    result = fun.fitTo(*data);

    // Print final value of parameters
    for(unsigned short nPeak = 0; nPeak < NPeak; nPeak++) {
      mean[nPeak]->Print();
      arrMean[0][nPeak] = mean[nPeak]->getValV();
      arrMean[1][nPeak] = mean[nPeak]->getError();
      sigma[nPeak]->Print();
      arrSigma[0][nPeak] = sigma[nPeak]->getValV();
      arrSigma[1][nPeak] = sigma[nPeak]->getError();
      coef[nPeak]->Print();
    }
    return result;
    //delete

}
//------------------------------------------------------------------------------
