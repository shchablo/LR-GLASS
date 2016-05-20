//-------------------------------------------------------------
#ifndef OUTFILEROOT_h
#define OUTFILEROOT_h

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TH1F.h>
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"

// C++ includes
#include <string>
#include <iostream>
using namespace std;

class OutFileRoot
{
public:

  //! constructor
  OutFileRoot();

  //! destructor
  virtual ~OutFileRoot();

  bool setOutputFile(char *outputFileName, char *outputTreeName);
  bool setBranch(double *data, string branchName);

  bool writeObject(string dirName, TObject *object);

protected:
  
  bool isOutFile_;
  TFile *outFile_;
  TTree *outTree_;
   
};

#endif
