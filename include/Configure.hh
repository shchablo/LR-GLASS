//-------------------------------------------------------------
#ifndef CONFIGURE_h
#define CONFIGURE_h

// C++ includes
#include <iostream>
#include <fstream>
#include <TFile.h>

using namespace std;

class Configure
{
public:

  //! constructor
  Configure();

  //! destructor
  virtual ~Configure();

  void getParam(char* inputTextFile, double* param, char** nameParam);
  int getType(char* inputTextFile, char* nameType);
  int getNumParam(char* inputTextFile);
  int getNumFiles(char* inputTextFile);
  void getNamesFiles(char* inputTextFile, char** inputFileNames, int numInFiles); 
  int getThrVolt(char* inputTextFile, double* thr, double* voltage, int numInFiles);
  int getMaskNumParam(char* inputTextFile);
  void getMask(char* inputTextFile, int* mask, int* firstCh, int* lastCh);
};

#endif
//-------------------------------------------------------------
