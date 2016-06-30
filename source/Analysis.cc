#include "Analysis.hh"

/* Constructor, destructor. */ 
//------------------------------------------------------------------------------
/*! \fn Analysis
* \brief   constructtor
*/
Analysis::Analysis()
{
  isParam_ = false; numParam_ = -1;
}

/*! \fn ~Analysis
* \brief  destructor
*/
Analysis::~Analysis()
{
}
//------------------------------------------------------------------------------

  /* Set functions. */ 
//------------------------------------------------------------------------------
bool Analysis::setParam(double *param, int numParam) 
{
  param_ = param;
  numParam_ = numParam;
  if(numParam > 0)
    isParam_ = true;
  else
    return false;
  return true;

}
//------------------------------------------------------------------------------

/* Analysis functions. */ 
//------------------------------------------------------------------------------

int Analysis::example()
{
  AData data; 
  data.TDCCh = new vector<int>;
  data.TDCTS = new vector<double>;
  data.TDCCh->clear();
  data.TDCTS->clear();

  int numFiles = getNumFiles();
  for(int i = 0; i < numFiles; i++) {

    int isRunFile = setRunFile(i);
    if(!isRunFile)
      return -1;
    string nameFile = getDaqName(i);
    
    // In case if exist CAEN - get value from CAEN file
    // another way - card file with .csv
    double voltage = getVoltage();
//    double current = getCurrent();
//    double thershold = getThreshold();
//    double source = getSource();


    int numEntries = getEntries();
    if(!numEntries)
      return -1;
    
    for(int e = 0; e < numEntries; e++) {
      bool isGetEvent = getEvent(e, &data.TDCNHits, data.TDCCh, data.TDCTS);
      if(!isGetEvent)
        return 0;
      for(int h = 0; h < data.TDCNHits; h++) {
        // loop for hits
        cout << "TDCTS=" << data.TDCTS->at(h) 
            << " TDCCh=" << data.TDCCh->at(h) << endl;
      }
    }
  }
  return 1;
}

int Analysis::francois()
{
  
  AData data; 
  data.TDCCh = new vector<int>;
  data.TDCTS = new vector<double>;
  data.TDCCh->clear();
  data.TDCTS->clear();

  int numFiles = getNumFiles();
  for(int i = 0; i < numFiles; i++) {

    int isRunFile = setRunFile(i);
    if(!isRunFile)
      return -1;
    string nameFile = getDaqName(i);
    double voltage = getVoltage();

    int numEntries = getEntries();
    if(!numEntries)
      return -1;
    
    for(int e = 0; e < numEntries; e++) {
      bool isGetEvent = getEvent(e, &data.TDCNHits, data.TDCCh, data.TDCTS);
      if(!isGetEvent)
        return 0;
      for(int h = 0; h < data.TDCNHits; h++) {
        // loop for hits
        cout << "TDCTS=" << data.TDCTS->at(h) 
            << " TDCCh=" << data.TDCCh->at(h) << endl;
      }
    }
  }
  return 1;
}

int Analysis::kGenStudy() 
{
  int numCh = getNumCh();
  double minCh = getMinCh();
  double maxCh = getMaxCh();

  TH2D *correlation;
  TH1D *profileNHits;
  TH2D *profileTimeStrip;
  TH1D *profileStrip;
  TH1D *profileTime;

  TH1D *profileTimes[numCh];
  TF1 *fitTimes[numCh];

  TH1D *profileTimeCorrect;
  TF1 *fitTimeCorrect;
  TH2D *profileTimeStripCorrectTime;
  
  TH1D *profileNHitsCluster;
  TH1D *profileStripCluster;
  TH1D *profileTimeCluster;
  TF1 *fitTimeCluster;
  TH2D *clusteringStrip;
  
  TH1D *multiplicity;
  TH1D *multiplicityCluster;
  
  AData data; 
  data.TDCCh = new vector<int>;
  data.TDCTS = new vector<double>;
  data.TDCCh->clear();
  data.TDCTS->clear();

  int numFiles = getNumFiles();
  double effCluster[numFiles];
  double eff[numFiles];
  double effVoltage[numFiles];
  double eEff[numFiles];
  double eEffCluster[numFiles];
  double eEffVoltage[numFiles];
  for(int i = 0; i < numFiles; i++) {

    int isRunFile = setRunFile(i);
    if(!isRunFile)
      return -1;
    string nameFile = getDaqName(i);
    double voltage = getVoltage();

    /* profile study */
    correlation = new TH2D(Form("Correlation-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            numCh, minCh, maxCh, numCh, minCh, maxCh);
    correlation->GetXaxis()->SetTitle("strip");
    correlation->GetYaxis()->SetTitle("strip");
    correlation->GetZaxis()->SetTitle("correlation");
    profileNHits = new TH1D(Form("NHits-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            3*numCh, 0, 3*numCh);
    profileNHits->GetXaxis()->SetTitle("number of hits");
    profileNHits->GetYaxis()->SetTitle("number of events");
    profileTimeStrip = new TH2D(Form("timeAndStrip-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            200, 200, 400, numCh, minCh, maxCh);
    profileTimeStrip->GetXaxis()->SetTitle("time, ns (bin size 2ns)");
    profileTimeStrip->GetYaxis()->SetTitle("strip");
    profileTimeStrip->GetZaxis()->SetTitle("number of hits");
    profileStrip = new TH1D(Form("strip-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            numCh, minCh, maxCh);

    profileTime = new TH1D(Form("time-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            200, 200, 400);
    profileTime->GetXaxis()->SetTitle("time, ns (bin size 2ns)");
    profileTime->GetYaxis()->SetTitle("number of hits");

    for(int ch = 0; ch < numCh; ch++) {
      profileTimes[ch] = new TH1D(Form("ch%1.f-time-%s-HV%2.fV", ch+minCh, plotName_, voltage), 
                             Form("ch%1.f-%s-HV%2.fV", ch+minCh, plotName_, voltage), 
                             200, 200, 400);
      profileTimes[ch]->GetXaxis()->SetTitle("time, ns (bin size 2ns)");
      profileTimes[ch]->GetYaxis()->SetTitle("number of hits");
    }

    /* time correct */
    profileTimeCorrect = new TH1D(Form("timeCorrect-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            200, 200, 400);
    profileTimeCorrect->GetXaxis()->SetTitle("time, ns (bin size 2ns)");
    profileTimeCorrect->GetYaxis()->SetTitle("number of hits");
    profileTimeStripCorrectTime = new TH2D(Form("timeAndStripCorrectTime-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            200, 200, 400, numCh, minCh, maxCh);
    profileTimeStripCorrectTime->GetXaxis()->SetTitle("time, ns (bin size 2ns)");
    profileTimeStripCorrectTime->GetYaxis()->SetTitle("strip");
    profileTimeStripCorrectTime->GetZaxis()->SetTitle("number of hits");
    
    /* clustering */
    profileNHitsCluster = new TH1D(Form("cluster-NHits-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            3*numCh, 0, 3*numCh);
    profileNHitsCluster->GetXaxis()->SetTitle("number of clusters");
    profileNHitsCluster->GetYaxis()->SetTitle("number of events");
    profileStripCluster = new TH1D(Form("cluster-strip-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            numCh, minCh, maxCh);
    profileStripCluster->GetXaxis()->SetTitle("cluster");
    profileStripCluster->GetYaxis()->SetTitle("number of clusters");
    profileTimeCluster = new TH1D(Form("cluster-time-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            200, 200, 400);
    profileTimeCluster->GetXaxis()->SetTitle("time, ns (bin size 2ns)");
    profileTimeCluster->GetYaxis()->SetTitle("number of clusters");
    clusteringStrip = new TH2D(Form("cluster-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            200, 200, 400, numCh, minCh, maxCh);
    clusteringStrip->GetXaxis()->SetTitle("time, ns (bin size 2ns)");
    clusteringStrip->GetYaxis()->SetTitle("cluster");
    clusteringStrip->GetZaxis()->SetTitle("number of clusters");
    
    /* clustering */
    multiplicity = new TH1D(Form("multiplicity-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            3*numCh, 0, 3*numCh);
    multiplicity->GetXaxis()->SetTitle("number of hits");
    multiplicity->GetYaxis()->SetTitle("number of events");
    
    multiplicityCluster = new TH1D(Form("multiplicityCluster-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            3*numCh, 0, 3*numCh);
    multiplicityCluster->GetXaxis()->SetTitle("number of hits");
    multiplicityCluster->GetYaxis()->SetTitle("number of events");

//------------------------------------------------------------------------------
    
    int numEntries = getEntries();
    if(!numEntries)
      return -1;
    
    /*  profiles study and fit data */
    for(int e = 0; e < numEntries; e++) {
      bool isGetEvent = getEvent(e, &data.TDCNHits, data.TDCCh, data.TDCTS);
      if(!isGetEvent)
        return 0;

      profileNHits->Fill(data.TDCNHits); 
      for(int h = 0; h < data.TDCNHits; h++) {

        for(int hh = 0; hh < data.TDCNHits; hh++) {
          if(data.TDCTS->at(h) - param_[0] < data.TDCTS->at(hh) &&
             data.TDCTS->at(h) + param_[1] > data.TDCTS->at(hh))
            correlation->Fill(data.TDCCh->at(h), data.TDCCh->at(hh));
        }
        profileStrip->Fill(data.TDCCh->at(h));

        if(data.TDCTS->at(h) <= 400 && data.TDCTS->at(h) >= 200) {
          profileTime->Fill(data.TDCTS->at(h));
          profileTimeStrip->Fill(data.TDCTS->at(h),data.TDCCh->at(h));
        }

        for(int ch = 0; ch < numCh; ch++) {
          if(data.TDCTS->at(h) <= 400 && data.TDCTS->at(h) >= 200 && 
             data.TDCCh->at(h) == ch+minCh)
            profileTimes[ch]->Fill(data.TDCTS->at(h));
        }
      }
    }

    /* time correct */
    //--------------------------------------------------------------------------
    double timeCorrect[numCh];
    double meanTimeCorrect = 0;
      for(int ch = 0; ch < numCh; ch++)
       meanTimeCorrect += profileTimes[ch]->GetMean();
      meanTimeCorrect = meanTimeCorrect/numCh;
      for(int ch = 0; ch < numCh; ch++)
       timeCorrect[ch] = meanTimeCorrect - profileTimes[ch]->GetMean();
    //--------------------------------------------------------------------------

    /* clustering study and eff + multiplicity */
    AData out; 
    out.TDCCh = new vector<int>;
    out.TDCTS = new vector<double>;
    out.TDCCh->clear();
    out.TDCTS->clear();
    vector<int> outCluster;
    outCluster.clear();
    for(int e = 0; e < numEntries; e++) {
      bool isGetEvent = getEvent(e, &data.TDCNHits, data.TDCCh, data.TDCTS);
      if(!isGetEvent)
        return 0;
     
      /* time correct */
    //--------------------------------------------------------------------------
      for(int h = 0; h < data.TDCNHits; h++) {
        for(int ch = 0; ch < numCh; ch++) {
          if(data.TDCCh->at(h) == ch+minCh)
            data.TDCTS->at(h) = data.TDCTS->at(h)+timeCorrect[ch];
        }
      }
      for(int h = 0; h < data.TDCNHits; h++) {
        if(data.TDCTS->at(h) <= 400 && data.TDCTS->at(h) >= 200) {
          profileTimeStripCorrectTime->Fill(data.TDCTS->at(h), data.TDCCh->at(h));
          profileTimeCorrect->Fill(data.TDCTS->at(h));
        }
      }
    //--------------------------------------------------------------------------
      
     KCluster kCluster;
     kCluster.clustering(param_[2], param_[3], 
                         data.TDCNHits, *data.TDCCh, *data.TDCTS, 
                         &out.TDCNHits, out.TDCCh, out.TDCTS, &outCluster);
      profileNHitsCluster->Fill(out.TDCNHits); 
      for(int h = 0; h < out.TDCNHits; h++) {
        profileStripCluster->Fill(out.TDCCh->at(h));
        if(out.TDCTS->at(h) <= 400 && out.TDCTS->at(h) >= 200) {
          clusteringStrip->Fill(out.TDCTS->at(h), out.TDCCh->at(h));
          profileTimeCluster->Fill(out.TDCTS->at(h));
        }
      }
    }

    fitTimeCorrect = new TF1(Form("fit-timeCorrect-%s-HV%2.fV", plotName_, voltage),
                             "gaus", 
                       profileTimeCorrect->GetMean()-profileTimeCorrect->GetRMS(),
                       profileTimeCorrect->GetMean()+profileTimeCorrect->GetRMS());
    profileTimeCorrect->Fit(fitTimeCorrect);
    
    fitTimeCluster = new TF1(Form("fit-timeCluster-%s-HV%2.fV", plotName_, voltage),
                             "gaus", 
                       profileTimeCluster->GetMean()-profileTimeCluster->GetRMS(),
                       profileTimeCluster->GetMean()+profileTimeCluster->GetRMS());
    profileTimeCluster->Fit(fitTimeCluster);


     eff[i] = 0;
     effCluster[i] = 0;
    for(int e = 0; e < numEntries; e++) {
      bool isGetEvent = getEvent(e, &data.TDCNHits, data.TDCCh, data.TDCTS);
      if(!isGetEvent)
        return 0;
      double countMultiplicity = 0;
      double lowTime = fitTimeCorrect->GetParameter(1) - 3*fitTimeCorrect->GetParameter(2);
      double highTime = fitTimeCorrect->GetParameter(1) + 3*fitTimeCorrect->GetParameter(2);
      for(int h = 0; h < data.TDCNHits; h++) {
        if(data.TDCTS->at(h) >= lowTime && data.TDCTS->at(h) <= highTime) {
          eff[i] += 1;
          for(int hh = 0; hh < data.TDCNHits; hh++) {
            if(data.TDCTS->at(hh) >= lowTime && data.TDCTS->at(hh) <= highTime)
              countMultiplicity += 1;
          }
          multiplicity->Fill(countMultiplicity);
          break;
        }
      }
      KCluster kCluster;
      kCluster.clustering(param_[2], param_[3], 
                          data.TDCNHits, *data.TDCCh, *data.TDCTS, 
                          &out.TDCNHits, out.TDCCh, out.TDCTS, &outCluster);
      countMultiplicity = 0;
      lowTime = fitTimeCluster->GetParameter(1) - 3*fitTimeCluster->GetParameter(2);
      highTime = fitTimeCluster->GetParameter(1) + 3*fitTimeCluster->GetParameter(2);
      for(int h = 0; h < out.TDCNHits; h++) {
        if(out.TDCTS->at(h) >= lowTime && out.TDCTS->at(h) <= highTime) {
          effCluster[i] += 1;
          multiplicityCluster->Fill(outCluster.at(h));
          break;
        }
      }
    }

    TString profileDir(nameFile.c_str());
    profileDir+="/profiles";
    const char* profileDirc=profileDir.Data();
    writeObject(profileDirc, correlation);
    correlation->Delete();
    writeObject(profileDirc, profileNHits);
    profileNHits->Delete();
    writeObject(profileDirc, profileStrip);
    profileStrip->Delete();
    writeObject(profileDirc, profileTime);
    profileTime->Delete();
    writeObject(profileDirc, profileTimeStrip);
    profileTimeStrip->Delete();
    writeObject(profileDirc, multiplicity);
    multiplicity->Delete();
    
    TString profileCorrectTimeDir(nameFile.c_str());
    profileCorrectTimeDir+="/profiles/timeCorrect";
    const char* profileCorrectTimeDirc=profileCorrectTimeDir.Data();
    writeObject(profileCorrectTimeDirc, profileTimeStripCorrectTime);
    profileTimeStripCorrectTime->Delete();
    writeObject(profileCorrectTimeDirc, profileTimeCorrect);
    profileTimeCorrect->Delete();
    fitTimeCorrect->Delete();

    TString profileClusterDir(nameFile.c_str());
    profileClusterDir+="/profiles/cluster";
    const char* profileClusterDirc=profileClusterDir.Data();
    writeObject(profileClusterDirc, profileNHitsCluster);
    profileNHitsCluster->Delete();
    writeObject(profileClusterDirc, profileStripCluster);
    profileStripCluster->Delete();
    writeObject(profileClusterDirc, profileTimeCluster);
    profileTimeCluster->Delete();
    fitTimeCluster->Delete();
    writeObject(profileClusterDirc, clusteringStrip);
    clusteringStrip->Delete();
    writeObject(profileClusterDirc, multiplicityCluster);
    multiplicityCluster->Delete();

    TString chTimeDir(nameFile.c_str());
    chTimeDir+="/profiles/chTimeProfiles";
    const char* chTimeDirc=chTimeDir.Data();
    for(int ch = 0; ch < numCh; ch++) {
      writeObject(chTimeDirc, profileTimes[ch]);
    }

    /* fit data and write */
    //--------------------------------------------------------------------------
    TString chFitTimeDir(nameFile.c_str());
    chFitTimeDir+="/profiles/chFitTimeProfiles";
    const char* chFitTimeDirc=chFitTimeDir.Data();
    for(int ch = 0; ch < numCh; ch++) {
      double lowFitTime = profileTimes[ch]->GetMean()-profileTimes[ch]->GetRMS();
      double highFitTime = profileTimes[ch]->GetMean()+profileTimes[ch]->GetRMS();
      fitTimes[ch] = new TF1(Form("fit-ch%1.f-time-%s-HV%2.fV", 
                                ch+minCh, plotName_, voltage),
                             "gaus", lowFitTime, highFitTime);
      profileTimes[ch]->Fit(fitTimes[ch]);
      writeObject(chFitTimeDirc, profileTimes[ch]);
      fitTimes[ch]->Delete();
      profileTimes[ch]->Delete();
    }
  eEffVoltage[i] = 0;
  effVoltage[i] = getVoltage();
  eEff[i] = sqrt((eff[i]*(numEntries-eff[i]))/numEntries)/eff[i];
  eff[i] = eff[i]/numEntries;
  eEffCluster[i] = sqrt((effCluster[i]*(numEntries-effCluster[i]))/numEntries)/effCluster[i];
  effCluster[i] = effCluster[i]/numEntries;
  }
  TGraphErrors *Eff = new TGraphErrors(numFiles, effVoltage, eff, eEffVoltage, eEff);
  Eff->SetName(Form("%s Efficiency", plotName_));
  Eff->SetTitle(Form("%s Efficiency", plotName_));
  Eff->GetXaxis()->SetTitle("Voltage, V");
  Eff->GetYaxis()->SetTitle("Efficiency");
  Eff->GetYaxis()->SetRange(0.3, 0);
  Eff->SetMarkerStyle(8);
  Eff->SetLineStyle(9);
  Eff->SetFillColor(0);
  Eff->SetLineWidth(1); 
  writeObject("", Eff);
  Eff->Delete();
  TGraphErrors *EffCluster = new TGraphErrors(numFiles, effVoltage, effCluster, eEffVoltage, eEffCluster);
  EffCluster->SetName(Form("%s Efficiency-cluster", plotName_));
  EffCluster->SetTitle(Form("%s Efficiency-cluster", plotName_));
  EffCluster->GetXaxis()->SetTitle("Voltage, V");
  EffCluster->GetYaxis()->SetTitle("Efficiency");
  EffCluster->GetYaxis()->SetRange(0.3, 0);
  EffCluster->SetMarkerStyle(8);
  EffCluster->SetLineStyle(9);
  EffCluster->SetFillColor(0);
  EffCluster->SetLineWidth(1); 
  writeObject("", EffCluster);
  EffCluster->Delete();
  return 1;
}
//------------------------------------------------------------------------------

/* Loop function. */ 
//------------------------------------------------------------------------------
/*! \fn loop
* \brief Loop function for choose the type of analysis and run it.
*
* \inputs: char* nameType
* \return: int: true(1) - good run; false(0) - run with errors.   
*/
int Analysis::loop(char* nameType)
{
   int isRun = 0;;
 if(strncmp(nameType, "kGenStudy", 9) == 0)
   isRun = kGenStudy();
 if(strncmp(nameType, "francois", 8) == 0)
   isRun = francois();
 if(strncmp(nameType, "example", 7) == 0)
   isRun = example();
  return isRun;
}
//------------------------------------------------------------------------------
