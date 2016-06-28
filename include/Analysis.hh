//------------------------------------------------------------------------------
/* INFORMATION: */
//------------------------------------------------------------------------------
#ifndef ANALYSIS_h
#define ANALYSIS_h

//------------------------------------------------------------------------------
#include <CMSRoot.hh>
#include <WebDcs.hh>
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
struct AData {
    int            TDCNHits;  // Number of hits in event i
    vector<int>    *TDCCh;    // List of channels giving hits per event
    vector<double>  *TDCTS;   // List of the corresponding time stamps
};

//------------------------------------------------------------------------------

class Analysis : public CMSRoot, public WebDcs
{
public:

  /* Constructor, destructor. */ 
  //----------------------------------------------------------------------------
  Analysis();
  virtual ~Analysis();
  //----------------------------------------------------------------------------

  /* Set functions. */ 
  //----------------------------------------------------------------------------
  void setPlotName(char *plotName) {plotName_ = plotName;}
  bool setParam(double *param, int numParam);
  //----------------------------------------------------------------------------

  void clustering(int spaceRange, double timeRange, 
                  AData data, 
                  int *TDCNHits, vector<int> *TDCCh, vector<double> *TDCTS);
  /* Loop function */ 
  //----------------------------------------------------------------------------
   bool loop(char* nameType);
  //----------------------------------------------------------------------------
protected:

  char *plotName_;
  
  double *param_;
  int numParam_;
  bool isParam_;

  /* Analysis functions. */ 
  //----------------------------------------------------------------------------
  int francois();
  int general();
  //----------------------------------------------------------------------------
};
#endif
//------------------------------------------------------------------------------
