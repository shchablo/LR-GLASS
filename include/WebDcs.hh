//------------------------------------------------------------------------------
/* INFORMATION: */
//------------------------------------------------------------------------------
#ifndef WEBDCS_h
#define WEBDCS_h

// C++ includes
//------------------------------------------------------------------------------
#include <math.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
//------------------------------------------------------------------------------

/* ROOT includes */
//------------------------------------------------------------------------------
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
struct MAP {
    int             TDC;
    int             real;
};
struct RAWData {
    int             iEvent;     //Event i
    int             TDCNHits;   //Number of hits in event i
    vector<int>    *TDCCh;      //List of channels giving hits per event
    vector<float>  *TDCTS;      //List of the corresponding time stamps
};
//------------------------------------------------------------------------------

class WebDcs
{
public:

  /* Constructor, destructor. */ 
  //----------------------------------------------------------------------------
  WebDcs();
  virtual ~WebDcs();
  //----------------------------------------------------------------------------

  /* Set functions. */ 
  //----------------------------------------------------------------------------
  bool setDaqInputFileNames(char **inputFileNames, int numInFiles);
  bool setCaenInputFileNames(char **inputFileNames, int numInFiles);
  bool setMask(int firstCh, int lastCh, int *mask, int numChMask);
  bool setMap(MAP *map, int numChMap); 
  //----------------------------------------------------------------------------
  bool setRunFile(int indexFile);
  //----------------------------------------------------------------------------

  /* Get functions. */ 
  //----------------------------------------------------------------------------
  double getCurrent();
  double getVoltage();
  int getEntries();
  bool getEvent(int nEvent,
                int *TDCNHits, vector<int> *TDCCh, vector<double> *TDCTS);
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  int getNumFiles() {if(daqNumInFiles_== caenNumInFiles_) return daqNumInFiles_;
                     else return 0;}
  int getNumCh() {return numChMap_;}
  double getMinCh() {return (double)chMinMap_;}
  double getMaxCh() {return (double)chMaxMap_;}
  string getDaqName(int index);
  //----------------------------------------------------------------------------

protected:

  TFile *dataDaq_;
  TFile *dataCaen_;
  
  RAWData data_;
  TTree *dataTree_;

  /* Set functions. */ 
  //----------------------------------------------------------------------------
  char **daqInputFileNames_;
  int daqNumInFiles_;
  bool isDaqInputFileNames_;
  
  char **caenInputFileNames_;
  int caenNumInFiles_;
  bool isCaenInputFileNames_;
  
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
  //----------------------------------------------------------------------------
};
#endif
//------------------------------------------------------------------------------
