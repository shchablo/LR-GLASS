//------------------------------------------------------------------------------
/* INFORMATION: */
//------------------------------------------------------------------------------
#ifndef CONFIGURE_h
#define CONFIGURE_h

// C++ includes
#include <iostream>
#include <fstream>
#include <TFile.h>

#include "Analysis.hh"

using namespace std;

class Configure
{
public:

  /* Constructor, destructor. */ 
  //----------------------------------------------------------------------------
  Configure();
  virtual ~Configure();
  //----------------------------------------------------------------------------

  /* Get functions. */ 
  //----------------------------------------------------------------------------
  void getParam(char* inputTextFile, double* param, char** nameParam);
  int getType(char* inputTextFile, char* nameType);
  int getNumParam(char* inputTextFile);
  int getDaqNumFiles(char* inputTextFile);
  void getDaqNamesFiles(char* inputTextFile, 
                        char** inputFileNames, int numInFiles); 
  int getCaenNumFiles(char* inputTextFile);
  void getCaenNamesFiles(char* inputTextFile, 
                         char** inputFileNames, int numInFiles); 
  int getThrVolt(char* inputTextFile, 
                 double* thr, double* voltage, int numInFiles);
  int getMaskNumParam(char* inputTextFile);
  void getMask(char* inputTextFile, int* mask, int* firstCh, int* lastCh);
  void getMap(char* inputTextFile, MAP* map);
  //----------------------------------------------------------------------------
};

#endif
//------------------------------------------------------------------------------
