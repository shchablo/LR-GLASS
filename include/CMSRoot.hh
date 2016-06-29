//------------------------------------------------------------------------------
/* INFORMATION: */
//------------------------------------------------------------------------------
#ifndef CMSROOT_h
#define CMSROOT_h

/* ROOT includes */
//------------------------------------------------------------------------------
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TString.h>
#include <TGraphErrors.h>
//------------------------------------------------------------------------------

/* C++ includes */
//------------------------------------------------------------------------------
#include <string>
#include <iostream>
using namespace std;
//------------------------------------------------------------------------------

class CMSRoot
{
public:

  /* Constructor, destructor. */ 
  //----------------------------------------------------------------------------
  CMSRoot();
  virtual ~CMSRoot();
  //----------------------------------------------------------------------------

  /* Set functions. */ 
  //----------------------------------------------------------------------------
  bool setOutputFile(char *outputFileName, char *outputTreeName);
  bool setBranch(double *data, string branchName);
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  bool writeObject(string dirName, TObject *object);
  //----------------------------------------------------------------------------

protected:

  bool isOutFile_;
  TFile *outFile_;
  TTree *outTree_;

//char  *dirName_;
//char  *plotName_;
};
#endif
//------------------------------------------------------------------------------
