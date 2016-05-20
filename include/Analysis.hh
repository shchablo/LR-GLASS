//-------------------------------------------------------------
#ifndef ANALYSIS_h
#define ANALYSIS_h

#include <OutFileRoot.hh>

#include "TGraphErrors.h"

// C++ includes
#include <iostream>
#include <math.h>

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

struct RAWData {
    int             iEvent;     //Event i
    int             TDCNHits;   //Number of hits in event i
    vector<int>    *TDCCh;      //List of channels giving hits per event
    vector<float>  *TDCTS;      //List of the corresponding time stamps
};

class Analysis : public OutFileRoot
{
public:

  //! constructor
  Analysis();

  //! destructor
  virtual ~Analysis();

  void setThreshold(double *threshold);
  void setVoltage(double *voltage);
  void setMask(int firstW, int lastW, int *Mask, int nChMask);
  int loop(char** inputFileNames, char* dirName, char* plotName, int numInFiles, char* nameType, double *param, int numParam);

protected:

  double *threshold;
  double *voltage;
  
  int firstCh;
  int lastCh;
  int numChMask;
  int *mask;

  double thrEff(char* inputFileName, double lowTSThr, double highTSThr);
  double thrEffErr(char* inputFileName, double lowTSThr, double highTSThr);
  double thrCorr(char* inputFileName, double lowTSThr, double highTSThr, double lowTSThr2, double highTSThr2, int ch1, int ch2);
  double noise(char* inputFileName, double acqTime);
 
  int thrEffScan(char** inputFileNames, char* dirName, char* plotName,  int numInFiles,
                 double lowTimeStapThr, double highTimeStapThr);
  int volEffScan(char** inputFileNames, char* dirName, char* plotName, int numInFiles,
                 double lowTimeStampThr, double highTimeStampThr);
  int noiseHist(char* inputFileName, char* dirName, char* plotName, double acqTime);
  int stripHist(char* inputFileName, char*dirName, char* plotName, double lowTSThr, double highTSThr);
  int noiseThrScan(char** inputFileNames, char* dirName, char* plotName, int numInFiles, double acqTime);
  int noiseVolScan(char** inputFileNames, char* dirName, char* plotName, int numInFiles, double acqTime);

};
#endif
