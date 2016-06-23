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

struct MAP {
    int             TDC;
    int             real;
};

struct RAWData {
    int             iEvent;     //Event i
    int             TDCNHits;   //Number of hits in event i
    vector<int>    *TDCCh;      //List of channels giving hits per event
    vector<float>  *TDCTS;      //List of the corresponding time stamps
    bool check;
};

class Analysis : public OutFileRoot
{
public:

  //! constructor
  Analysis();

  //! destructor
  virtual ~Analysis();

  int indexMinMax(int length, double* data, int flag);
  int setInputFileNames(char **inputFileNames, int numInFiles);
  int setParam(double *param, int numParam); 
  void setThreshold(double *threshold); 
  void setVoltage(double *voltage);
  int  setMask(int firstCh, int lastCh, int *mask, int numChMask);
  int  setMap(MAP *map, int numChMap); 
  void setDirName(char *dirName);
  void setPlotName(char *plotName);
  
  int getEntries(char *inputFileName);
  RAWData Filter(RAWData *data);
  RAWData getEvent(int nEvent, char *inputFileName);
  RAWData getData(RAWData *read);
  int getMinMaxTime(double binTime, double *minTime, double *maxTime, int *numBins);
  double getMeanMoreTime(int hitIndex, RAWData *data);
  double getMeanLessTime(int hitIndex, RAWData *data);
  double getCurrent(int index);
  
  int hitsProfile(); 
  int timeProfile();
  int meanMoreTimeProfile();
  int currentSourceProfile();
  int diffTimeProfile();
  int effProfile();
  
  int loop(char* nameType);
  
  string getError(int index);

protected:

  char **inputFileNames_;
  int numInFiles_;
  bool isInputFileNames_;
  
  double *param_;
  int numParam_;
  bool isParam_;

  double *thr_;
  bool isThreshold_;
  
  double *volt_;
  bool isVoltage_;

  int firstCh_;
  int lastCh_;
  int *mask_;
  int numChMask_;
  bool isMask_;

  MAP *map_;
  int numChMap_;
  bool isMap_;
  int chMinMap_;
  int chMaxMap_;

  char* dirName_;
  
  char* plotName_;
};
#endif
