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

void Analysis::clustering(int spaceRange, double timeRange, 
                          AData inData, int *outTDCNHits, vector<int> *outTDCCh, vector<double> *outTDCTS) 
{
  *outTDCNHits = 0;
  outTDCCh->clear();
  outTDCTS->clear();
  int tempCh = 0;
  double tempTime = 0;
  bool exit = false;
  while(!exit) {
   exit = true;
   for(int i = 0; i < inData.TDCNHits - 1; i++)
     if(inData.TDCCh->at(i) > inData.TDCCh->at(i+1)) {
      tempCh = inData.TDCCh->at(i);
      inData.TDCCh->at(i) = inData.TDCCh->at(i+1);
      inData.TDCCh->at(i+1) = tempCh;
      tempTime = inData.TDCTS->at(i);
      inData.TDCTS->at(i) = inData.TDCTS->at(i+1);
      inData.TDCTS->at(i+1) = tempCh;
      exit = false;
     }
  }
  int hBegin = 0;
  bool cluster[inData.TDCNHits];
  for(int h = 0; h < inData.TDCNHits; h++)
    cluster[h] = true;
  inData.TDCCh->push_back(std::numeric_limits<int>::max());
  inData.TDCTS->push_back(0);
  
  int TDCNHits = 0;
  vector<int> TDCCh; TDCCh.clear();
  vector<double> TDCTS; TDCTS.clear();
  vector<int> index; index.clear();
  exit = false;
  while(!exit) {
    exit = true;
    for(int h = hBegin; (h < inData.TDCNHits) && cluster[h]; h++) {
      int hIndex = h+1;
      for(int hh = hIndex; hh < inData.TDCNHits; hh++) {
        if(cluster[hh] == true) {
          hIndex = hh;
          break;
        }
      }
      if(inData.TDCCh->at(h) + spaceRange < inData.TDCCh->at(hIndex)) {
        if(TDCNHits != 0) {
          exit = false;
          while(!exit) {
            double minTDCTS = std::numeric_limits<double>::max();
            double maxTDCTS = std::numeric_limits<double>::min();
            int indexMinTDCTS = 0;
            int indexMaxTDCTS = 0;
            double meanTDCTS = 0;
            double meanTDCCh = 0;
            if(TDCNHits != 0) {
              for(int hh = 0; hh < TDCNHits; hh++) {
                meanTDCTS += TDCTS.at(hh);
                meanTDCCh += TDCCh.at(hh);
                if(minTDCTS > TDCTS.at(hh)) {
                  minTDCTS = TDCTS.at(hh);
                  indexMinTDCTS = hh;
                }
                if(maxTDCTS < TDCTS.at(hh)) {
                  maxTDCTS = TDCTS.at(hh);
                  indexMaxTDCTS = hh;
                }
              }
              meanTDCTS = meanTDCTS/TDCNHits;
              meanTDCCh = meanTDCCh/TDCNHits;
              if(timeRange > (maxTDCTS - minTDCTS) || true) {
                outTDCCh->push_back(meanTDCCh);
                outTDCTS->push_back(meanTDCTS);
                *outTDCNHits += 1;
                exit = true;
              }
              else {
                if(abs(maxTDCTS - meanTDCCh) > abs(meanTDCCh - minTDCTS)) {
                  TDCNHits -= 1;
                  cluster[index[indexMaxTDCTS]] = false;
                  TDCCh.erase(TDCCh.begin() + indexMaxTDCTS);
                  vector<int>(TDCCh).swap(TDCCh);
                  TDCTS.erase(TDCTS.begin() + indexMaxTDCTS);
                  vector<double>(TDCTS).swap(TDCTS);
                  index.erase(index.begin() + indexMaxTDCTS);
                  vector<int>(index).swap(index);
                }
                else {
                  TDCNHits -= 1;
                  cluster[index[indexMinTDCTS]] = false;
                  TDCCh.erase(TDCCh.begin() + indexMinTDCTS);
                  vector<int>(TDCCh).swap(TDCCh);
                  TDCTS.erase(TDCTS.begin() + indexMinTDCTS);
                  vector<double>(TDCTS).swap(TDCTS);
                  index.erase(index.begin() + indexMinTDCTS);
                  vector<int>(index).swap(index);
                }
              }
            }
            else {
              cluster[index[0]] = false;
              outTDCCh->push_back(inData.TDCCh->at(index[0]));
              outTDCTS->push_back(inData.TDCTS->at(index[0]));
              *outTDCNHits += 1;
              exit = true;
            }
          }
        }
        else {
          cluster[h] = false;
          outTDCCh->push_back(inData.TDCCh->at(h));
          outTDCTS->push_back(inData.TDCTS->at(h));
          *outTDCNHits += 1;
        }
      }
      else {
//        cout << inData.TDCCh->at(h) << "  " << inData.TDCCh->at(h+1) << endl;
        cluster[h] = false;
        TDCNHits += 1;
        TDCCh.push_back(inData.TDCCh->at(h));
        TDCTS.push_back(inData.TDCTS->at(h));
        index.push_back(h);
      }
    }
    for(int h = 0; h < inData.TDCNHits; h++) {
      if(cluster[h] == true) {
        hBegin = h;
        TDCCh.clear();
        TDCTS.clear();
        index.clear();
        TDCNHits = 0;
        exit = false;
        break;
      }
    }
  }
} 

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

int Analysis::francois()
{
  
  AData data; 
  data.TDCCh = new vector<int>; // List of hits and their channels
  data.TDCTS = new vector<double>; // List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();

  int numFiles = getNumFiles();
  for(int i = 0; i < numFiles; i++) {

    int isRunFile = setRunFile(i);
    if(!isRunFile)
      return -1;
    string nameFile = getDaqName(i); // Don´t use the dots in file name only .root 
    double voltage = getVoltage();

    int numEntries = getEntries();
    if(!numEntries)
      return -1;
    
    for(int e = 0; e < numEntries; e++) {
      bool isGetEvent = getEvent(e, &data.TDCNHits, data.TDCCh, data.TDCTS);
      if(!isGetEvent)
        return 0;

      for(int h = 0; h < data.TDCNHits; h++) {

      }
    }
  }
  return 1;
}

int Analysis::general() 
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

  TH2D *profileTimeStripCorrectTime;
  
  TH1D *profileNHitsCluster;
  TH1D *profileStripCluster;
  TH1D *profileTimeCluster;
  TH2D *clusteringStrip;
  
  AData data; 
  data.TDCCh = new vector<int>; //List of hits and their channels
  data.TDCTS = new vector<double>; //List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();

  int numFiles = getNumFiles();
  for(int i = 0; i < numFiles; i++) {

    int isRunFile = setRunFile(i);
    if(!isRunFile)
      return -1;
//------------------------------------------------------------------------------
    string nameFile = getDaqName(i);
    double voltage = getVoltage();

    correlation = new TH2D(Form("Correlation-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            numCh, minCh, maxCh, numCh, minCh, maxCh);
    correlation->GetXaxis()->SetTitle("strip");
    correlation->GetYaxis()->SetTitle("strip");
    correlation->GetZaxis()->SetTitle("correlation");
    profileNHits = new TH1D(Form("NHits-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            numCh, 0, numCh);
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
    profileTimeStripCorrectTime = new TH2D(Form("timeAndStripCorrectTime-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            200, 200, 400, numCh, minCh, maxCh);
    profileTimeStripCorrectTime->GetXaxis()->SetTitle("time, ns (bin size 2ns)");
    profileTimeStripCorrectTime->GetYaxis()->SetTitle("strip");
    profileTimeStripCorrectTime->GetZaxis()->SetTitle("number of hits");
    
    /* clustering */
    profileNHitsCluster = new TH1D(Form("cluster-NHits-%s-HV%2.fV", plotName_, voltage), 
                            Form("%s-HV%2.fV", plotName_, voltage), 
                            numCh, 0, numCh);
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

//------------------------------------------------------------------------------
    
    int numEntries = getEntries();
    if(!numEntries)
      return -1;
    
    // plot profiles and fit data
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

    // time correct
    double timeCorrect[numCh];
    double meanTimeCorrect = 0;
      for(int ch = 0; ch < numCh; ch++)
       meanTimeCorrect += profileTimes[ch]->GetMean();
      meanTimeCorrect = meanTimeCorrect/numCh;
      for(int ch = 0; ch < numCh; ch++)
       timeCorrect[ch] = meanTimeCorrect - profileTimes[ch]->GetMean();

    // clustering
    AData out; 
    out.TDCCh = new vector<int>; // List of hits and their channels
    out.TDCTS = new vector<double>; // List of the corresponding time stamps
    out.TDCCh->clear();
    out.TDCTS->clear();
    for(int e = 0; e < numEntries; e++) {
      bool isGetEvent = getEvent(e, &data.TDCNHits, data.TDCCh, data.TDCTS);
      if(!isGetEvent)
        return 0;
     
      // time correct переделай все в функцию
      for(int h = 0; h < data.TDCNHits; h++) {
        for(int ch = 0; ch < numCh; ch++) {
          if(data.TDCCh->at(h) == ch+minCh)
            data.TDCTS->at(h) = data.TDCTS->at(h)+timeCorrect[ch];
        }
      }
      for(int h = 0; h < data.TDCNHits; h++) {
        if(data.TDCTS->at(h) <= 400 && data.TDCTS->at(h) >= 200) {
          profileTimeStripCorrectTime->Fill(data.TDCTS->at(h), data.TDCCh->at(h));
        }
      }
      
      clustering(param_[2], param_[3], data, &out.TDCNHits, out.TDCCh, out.TDCTS);
      profileNHitsCluster->Fill(out.TDCNHits); 
      for(int h = 0; h < out.TDCNHits; h++) {
        profileStripCluster->Fill(out.TDCCh->at(h));
        if(out.TDCTS->at(h) <= 400 && out.TDCTS->at(h) >= 200) {
          clusteringStrip->Fill(out.TDCTS->at(h), out.TDCCh->at(h));
          profileTimeCluster->Fill(out.TDCTS->at(h));
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
    
    TString profileCorrectTimeDir(nameFile.c_str());
    profileCorrectTimeDir+="/profiles/timeCorrect";
    const char* profileCorrectTimeDirc=profileCorrectTimeDir.Data();
    writeObject(profileCorrectTimeDirc, profileTimeStripCorrectTime);
    profileTimeStripCorrectTime->Delete();

    TString profileClusterDir(nameFile.c_str());
    profileClusterDir+="/profiles/cluster";
    const char* profileClusterDirc=profileClusterDir.Data();
    writeObject(profileClusterDirc, profileNHitsCluster);
    profileNHitsCluster->Delete();
    writeObject(profileClusterDirc, profileStripCluster);
    profileStripCluster->Delete();
    writeObject(profileClusterDirc, profileTimeCluster);
    profileTimeCluster->Delete();
    writeObject(profileClusterDirc, clusteringStrip);
    clusteringStrip->Delete();

    TString chTimeDir(nameFile.c_str());
    chTimeDir+="/profiles/chTimeProfiles";
    const char* chTimeDirc=chTimeDir.Data();
    for(int ch = 0; ch < numCh; ch++) {
      writeObject(chTimeDirc, profileTimes[ch]);
    }
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
  }
  return 1;
}
//------------------------------------------------------------------------------

/* Loop function. */ 
//------------------------------------------------------------------------------
/*! \fn loop
* \brief Loop function for choose the type of analysis and run it.
*
* \inputs: char* nameType
* Types:
* 1. General analysis: general
* 2. Plot hits profile: hPro
* 3. Plot time profile: tPro 
* 4. Plot efficincy: eff
* \return: bool: true - good run; false - run with errors.   
*/
bool Analysis::loop(char* nameType)
{
 if(strncmp(nameType, "general", 7) == 0) {
   int isGeneral = general();
 }
 if(strncmp(nameType, "francois", 8) == 0) {
   int isGeneral = general();
  }
  return true;
}
//------------------------------------------------------------------------------
