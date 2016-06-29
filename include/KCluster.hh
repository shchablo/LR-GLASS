//------------------------------------------------------------------------------
/* INFORMATION: */
//------------------------------------------------------------------------------
#ifndef KCLUSTER_h
#define KClUSTER_h

#include <TROOT.h>
/* C++ includes */
//------------------------------------------------------------------------------
#include <math.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
//------------------------------------------------------------------------------

class KCluster
{
public:

  /* Constructor, destructor. */ 
  //----------------------------------------------------------------------------
  KCluster();
  virtual ~KCluster();
  //----------------------------------------------------------------------------
  
  //----------------------------------------------------------------------------
  void clustering(int spaceRange, double timeRange, 
              int inTDCNHits, vector<int> inTDCCh, vector<double> inTDCTS,
              int *outTDCNHits, vector<int> *outTDCCh, vector<double> *outTDCTS,
              vector<int> *outCluster);
  //----------------------------------------------------------------------------

};
#endif
//------------------------------------------------------------------------------
