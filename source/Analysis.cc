#include "Analysis.hh"
/*! \fn Analysis
* \brief   constructtor
*/
Analysis::Analysis()
{
}

/*! \fn ~Analysis
* \brief  destructor
*/
Analysis::~Analysis()
{
}

//-------------------------------------------------------
void Analysis::setThreshold(double *thr) 
{
  threshold = thr;
}

void Analysis::setVoltage(double *volt) 
{
  voltage = volt;
}

void  Analysis::setMask(int firstW, int lastW, int *Mask, int nChMask) 
{
  firstCh = firstW;
  lastCh = lastW;
  mask = Mask; 
  numChMask = nChMask;
}
//-------------------------------------------------------


//-------------------------------------------------------
double Analysis::thrEff(char* inputFileName, double lowTSThr, double highTSThr)
{
  //****************** ROOT FILE ***********************************
  // input ROOT data file containing the RAWData TTree that we'll
  // link to our RAWData structure
  
  TFile   dataFile(inputFileName);
  TTree*  dataTree = (TTree*)dataFile.Get("RAWData");
  if(!dataTree)
    return -1; // can't read file
  RAWData data;
  
  data.TDCCh = new vector<int>; //List of hits and their channels
  data.TDCTS = new vector<float>; //List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();
  
  dataTree->SetBranchAddress("EventNumber",    &data.iEvent);
  dataTree->SetBranchAddress("number_of_hits", &data.TDCNHits);
  dataTree->SetBranchAddress("TDC_channel",    &data.TDCCh);
  dataTree->SetBranchAddress("TDC_TimeStamp",  &data.TDCTS);
  
  //****************** MACRO ***************************************
  double numGoodEvents = 0; 
  unsigned int nEntries = dataTree->GetEntries();
  
  for(unsigned int i = 0; i < nEntries; i++) {
    dataTree->GetEntry(i);
  
    for(int h = 0; h < data.TDCNHits; h++) {
      bool isCh = true;
      if(data.TDCTS->at(h) > lowTSThr && data.TDCTS->at(h) < highTSThr && 
         data.TDCCh->at(h) >= firstCh && data.TDCCh->at(h) <= lastCh) {
      for(int i = 0; i < numChMask; i++) {
        if(data.TDCCh->at(h) == mask[i])
          isCh = false;
      }
      if(isCh) {
        numGoodEvents++;
        break;
      }
      }
    }
  }
  dataFile.Close();

  return numGoodEvents/nEntries;
}

double Analysis::thrEffErr(char* inputFileName, double lowTSThr, double highTSThr)
{
  //****************** ROOT FILE ***********************************
  // input ROOT data file containing the RAWData TTree that we'll
  // link to our RAWData structure
  
  TFile   dataFile(inputFileName);
  TTree*  dataTree = (TTree*)dataFile.Get("RAWData");
  if(!dataTree)
    return -1; // can't read file
  RAWData data;
  
  data.TDCCh = new vector<int>; //List of hits and their channels
  data.TDCTS = new vector<float>; //List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();
  
  dataTree->SetBranchAddress("EventNumber",    &data.iEvent);
  dataTree->SetBranchAddress("number_of_hits", &data.TDCNHits);
  dataTree->SetBranchAddress("TDC_channel",    &data.TDCCh);
  dataTree->SetBranchAddress("TDC_TimeStamp",  &data.TDCTS);
  
  //****************** MACRO ***************************************
  double numGoodEvents = 0; 
  unsigned int nEntries = dataTree->GetEntries();
  
  for(unsigned int i = 0; i < nEntries; i++) {
    // You are looping on all the entries (1 trigger = 1 event = 1 entry) 
    dataTree->GetEntry(i);
  
    //Loop over the TDC hits
    for(int h = 0; h < data.TDCNHits; h++) {
      /* You are looping on the hits recorded for entry i
         Here do whatever you need to
         You can for example print out all the hit information */
    
//      printf("Hit %u - Time stamp = %f", data.TDCCh->at(h),data.TDCTS->at(h));
//      cout << endl;
      if(data.TDCTS->at(h) > lowTSThr && data.TDCTS->at(h) < highTSThr) {
       numGoodEvents++;
      break;
      }
    }
  }
  dataFile.Close();

  return sqrt((numGoodEvents*(nEntries-numGoodEvents))/nEntries)/numGoodEvents;
}

double Analysis::thrCorr(char* inputFileName, double lowTSThr, double highTSThr, double lowTSThr2, double highTSThr2, int ch1, int ch2)
{
  //****************** ROOT FILE ***********************************
  // input ROOT data file containing the RAWData TTree that we'll
  // link to our RAWData structure
  
  TFile   dataFile(inputFileName);
  TTree*  dataTree = (TTree*)dataFile.Get("RAWData");
  if(!dataTree)
    return -1; // can't read file
  RAWData data;
  
  data.TDCCh = new vector<int>; //List of hits and their channels
  data.TDCTS = new vector<float>; //List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();
  
  dataTree->SetBranchAddress("EventNumber",    &data.iEvent);
  dataTree->SetBranchAddress("number_of_hits", &data.TDCNHits);
  dataTree->SetBranchAddress("TDC_channel",    &data.TDCCh);
  dataTree->SetBranchAddress("TDC_TimeStamp",  &data.TDCTS);
  
  //****************** MACRO ***************************************
  double corr = 0; 
  double d1 = 0; 
  double d2 = 0; 
  double Hit1 = 0; 
  double sumHit1 = 0; 
  double numHit1 = 0; 
  double middleHit1 = 0; 
  double Hit2 = 0; 
  double sumHit2 = 0; 
  double numHit2 = 0; 
  double middleHit2 = 0; 
  unsigned int nEntries = dataTree->GetEntries();
  
  for(unsigned int i = 0; i < nEntries; i++) {
    // You are looping on all the entries (1 trigger = 1 event = 1 entry) 
    dataTree->GetEntry(i);
  
    //Loop over the TDC hits
    for(int h = 0; h < data.TDCNHits; h++) {
      /* You are looping on the hits recorded for entry i
         Here do whatever you need to
         You can for example print out all the hit information */
    
//      printf("Hit %u - Time stamp = %f", data.TDCCh->at(h),data.TDCTS->at(h));
//      cout << endl;
      if(data.TDCTS->at(h) > lowTSThr && data.TDCTS->at(h) < highTSThr 
         && data.TDCCh->at(h) >= ch1 && data.TDCCh->at(h) <= ch2) {
       sumHit1 = sumHit1 + 1; 
      }
      if(data.TDCTS->at(h) > lowTSThr2 && data.TDCTS->at(h) < highTSThr2 
         && data.TDCCh->at(h) >= ch1 && data.TDCCh->at(h) <= ch2) {
       sumHit2 = sumHit2 + 1; 
      }
    }
  }
  middleHit1 = sumHit1/nEntries;
  middleHit2 = sumHit2/nEntries;
  for(unsigned int i = 0; i < nEntries; i++) {
    sumHit1 = 0; 
    sumHit2 = 0; 
    // You are looping on all the entries (1 trigger = 1 event = 1 entry) 
    dataTree->GetEntry(i);
  
    //Loop over the TDC hits
    for(int h = 0; h < data.TDCNHits; h++) {
      /* You are looping on the hits recorded for entry i
         Here do whatever you need to
         You can for example print out all the hit information */
    
//      printf("Hit %u - Time stamp = %f", data.TDCCh->at(h),data.TDCTS->at(h));
//      cout << endl;
      if(data.TDCTS->at(h) > lowTSThr && data.TDCTS->at(h) < highTSThr
         && data.TDCCh->at(h) >= ch1 && data.TDCCh->at(h) <= ch2) {
       sumHit1 = sumHit1 +  1; 
      }
      if(data.TDCTS->at(h) > lowTSThr2 && data.TDCTS->at(h) < highTSThr2
         && data.TDCCh->at(h) >= ch1 && data.TDCCh->at(h) <= ch2) {
       sumHit2 = sumHit2 + 1; 
      }
    }
      corr += (sumHit1-middleHit1)*(sumHit2-middleHit2);
      d1 += (sumHit1-middleHit1)*(sumHit1-middleHit1);
      d2 += (sumHit2-middleHit2)*(sumHit2-middleHit2);
  }
  dataFile.Close();

  return corr/sqrt(d1*d2);
}

double Analysis::noise(char* inputFileName, double acqTime)
{
  //****************** ROOT FILE ***********************************
  // input ROOT data file containing the RAWData TTree that we'll
  // link to our RAWData structure
  
  TFile   dataFile(inputFileName);
  TTree*  dataTree = (TTree*)dataFile.Get("RAWData");
  if(!dataTree)
    return -1; // can't read file
  RAWData data;
  
  data.TDCCh = new vector<int>; //List of hits and their channels
  data.TDCTS = new vector<float>; //List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();
  
  dataTree->SetBranchAddress("EventNumber",    &data.iEvent);
  dataTree->SetBranchAddress("number_of_hits", &data.TDCNHits);
  dataTree->SetBranchAddress("TDC_channel",    &data.TDCCh);
  dataTree->SetBranchAddress("TDC_TimeStamp",  &data.TDCTS);
  
  //****************** MACRO ***************************************
  double hits = 0; 
  unsigned int nEntries = dataTree->GetEntries();
  
  bool isCh = true;
  for(unsigned int i = 0; i < nEntries; i++) {
    dataTree->GetEntry(i);
  
    for(int h = 0; h < data.TDCNHits; h++) {
      isCh = true;
      if(data.TDCCh->at(h) >= firstCh && data.TDCCh->at(h) <= lastCh) {
      for(int i = 0; i < numChMask; i++) {
        if(data.TDCCh->at(h) == mask[i])
          isCh = false;
          break;
      }
      if(isCh)
        hits++;
      }
    }
  }
  dataFile.Close();

  int maskCh = 0;
  for(int i = 0; i < numChMask; i++) {
    if(mask[i] >= firstCh && mask[i] <= lastCh)
      maskCh++;

  }
  return (hits/(lastCh-firstCh-maskCh))/acqTime;
}
//-------------------------------------------------------


//-------------------------------------------------------
int Analysis::thrEffScan(char** inputFileNames, char* dirName, char* plotName,  int numInFiles,
                          double lowTimeStampThr, double highTimeStampThr)
{
  double eff[numInFiles];
  double eEff[numInFiles];
  double thr[numInFiles];
  double eThr[numInFiles];
  for(int i = 0; i < numInFiles; i ++) {
    eff[i] = thrEff(inputFileNames[i], lowTimeStampThr, highTimeStampThr);
    if(eff[i] == -1)
      return 0;
    eEff[i] = thrEffErr(inputFileNames[i], lowTimeStampThr, highTimeStampThr);
  
    thr[i] = threshold[i];
    eThr[i] = 0;
  }
  
  double vol = voltage[0];
  TGraphErrors *thrEff = new TGraphErrors(numInFiles, thr, eff, eThr, eEff);
  thrEff->SetName(Form("%s Efficiency, voltage = %.2fV", plotName, vol));
  thrEff->SetTitle(Form("%s Efficiency, voltage = %.2fV", plotName, vol));
  thrEff->GetXaxis()->SetTitle("Threshold, mV");
  thrEff->GetYaxis()->SetTitle("Efficiency");
  thrEff->SetMarkerStyle(8);
  thrEff->SetLineStyle(9);
  thrEff->SetFillColor(0);
  thrEff->SetLineWidth(1);
   TString dir(dirName);
   dir+="_"; dir+="param_";
   dir+="lowTSThr-"; dir+=lowTimeStampThr; dir+="_"; 
   dir+="highTSThr-";dir+=highTimeStampThr;  dir+="_"; 
   const char* dirc=dir.Data();
   writeObject(dirc, thrEff);
   thrEff->Delete();
  
  return 1;
}

int Analysis::volEffScan(char** inputFileNames, char* dirName, char* plotName, int numInFiles,
                          double lowTimeStampThr, double highTimeStampThr)
{
  double eff[numInFiles];
  double eEff[numInFiles];
  double vol[numInFiles];
  double eVol[numInFiles];
  for(int i = 0; i < numInFiles; i ++) {
    eff[i] = thrEff(inputFileNames[i], lowTimeStampThr, highTimeStampThr);
    if(eff[i] == -1)
      return 0;
    eEff[i] = thrEffErr(inputFileNames[i], lowTimeStampThr, highTimeStampThr);
  
    vol[i] = voltage[i];
    eVol[i] = 0;
  }
  
  double thr = threshold[0];
  TGraphErrors *volEff = new TGraphErrors(numInFiles,vol,eff,eVol,eEff);
  volEff->SetName(Form("%s Efficiency, threshold = %.2fmV", plotName, thr));
  volEff->SetTitle(Form("%s Efficiency, threshold = %.2fmV", plotName, thr));
  volEff->GetXaxis()->SetTitle("Voltage, V");
  volEff->GetYaxis()->SetTitle("Efficiency");
  volEff->GetYaxis()->SetRange(0.3, 0);
  volEff->SetMarkerStyle(8);
  volEff->SetLineStyle(9);
  volEff->SetFillColor(0);
  volEff->SetLineWidth(1);
   TString dir(dirName);
   dir+="_"; dir+="param_";
   dir+="lowTSThr-"; dir+=lowTimeStampThr; dir+="_"; 
   dir+="highTSThr-";dir+=highTimeStampThr;  dir+="_"; 
   const char* dirc=dir.Data();
   writeObject(dirc, volEff);
   volEff->Delete();
  
  return 1;
}

int Analysis::noiseHist(char* inputFileName, char*dirName, char* plotName, double acqTime)
{
    TH1F *hHit = new TH1F("Noise", "Noise", lastCh-firstCh, firstCh - 0.5, lastCh - 0.5);
    hHit->GetXaxis()->SetTitle("channel");
    hHit->GetYaxis()->SetTitle("hits/sec, Hz");
  //****************** ROOT FILE ***********************************
  // input ROOT data file containing the RAWData TTree that we'll
  // link to our RAWData structure 

  TFile   dataFile(inputFileName);
  TTree*  dataTree = (TTree*)dataFile.Get("RAWData");
  if(!dataTree)
    return -1; // can't read file
  RAWData data;
  
  data.TDCCh = new vector<int>; //List of hits and their channels
  data.TDCTS = new vector<float>; //List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();
  
  dataTree->SetBranchAddress("EventNumber",    &data.iEvent);
  dataTree->SetBranchAddress("number_of_hits", &data.TDCNHits);
  dataTree->SetBranchAddress("TDC_channel",    &data.TDCCh);
  dataTree->SetBranchAddress("TDC_TimeStamp",  &data.TDCTS);
  
  //****************** MACRO ***************************************
  int ch = 0;
  double sum = 0;
  double sumL[4];
  sumL[0] = 0;
  sumL[1] = 0;
  sumL[2] = 0;
  sumL[3] = 0;
  unsigned int nEntries = dataTree->GetEntries();
  
  for(unsigned int i = 0; i < nEntries; i++) {
    // You are looping on all the entries (1 trigger = 1 event = 1 entry) 
    dataTree->GetEntry(i);
    //Loop over the TDC hits
    for(int h = 0; h < data.TDCNHits; h++) {
      ch = data.TDCCh->at(h);
      if(numChMask > 0) {
        bool isCh = true;
        for(int i = 0; i < numChMask; i++) {
          if(ch == mask[i])
            isCh = false;
        }
          if(isCh && ch >= firstCh && ch <= lastCh) {
            hHit->Fill(ch);
            sum ++;
            if(firstCh <= ch && firstCh + 31 >= ch)
              sumL[0]++;
            if(firstCh+ 32 <= ch && firstCh + 63 >= ch)
              sumL[1]++;
            if(firstCh+ 64 <= ch && firstCh + 95 >= ch)
              sumL[2]++;
            if(firstCh + 96 <= ch && firstCh + 127 >= ch)
              sumL[3]++;
          }
      } 
      else {
        if(ch >= firstCh && ch <= lastCh) {
          hHit->Fill(ch);
          sum ++;
        }
      }
    }
  }
  dataFile.Close();
  hHit->SetTitle(Form("From left to right: A,B,C,D. %s", plotName));
  hHit->Scale(1/acqTime);
  hHit->SetStats(0);
  TCanvas *c1 = new TCanvas(plotName);
  hHit->Draw();
  Float_t ymax = hHit->GetMaximum();
  TLine *line = new TLine(firstCh - 0.5, ymax, firstCh - 0.5, 0);
  TLine *line2 = new TLine(firstCh+1*31 + 0.5, ymax, firstCh+1*31+0.5, 0);
  TLine *line3 = new TLine(firstCh+63+0.5, ymax, firstCh+63+0.5, 0);
  TLine *line4 = new TLine(firstCh+95+0.5, ymax, firstCh+95+0.5, 0);
  line->SetLineColor(kRed);
  line2->SetLineColor(kRed);
  line3->SetLineColor(kRed);
  line4->SetLineColor(kRed);
  line->Draw(); 
  line2->Draw(); 
  line3->Draw(); 
  line4->Draw(); 

   TString dir(dirName);
   dir+="_"; dir+="param_";
   dir+=acqTime; 
   const char* dirc=dir.Data();
  writeObject(dirc, c1);
  return 1;
}

int Analysis::stripHist(char* inputFileName, char*dirName, char* plotName, double lowTSThr, double highTSThr)
{
    TH1F *hHit = new TH1F("beam", "beam", lastCh-firstCh, firstCh - 0.5, lastCh - 0.5);
    hHit->GetXaxis()->SetTitle("channel");
    hHit->GetYaxis()->SetTitle("hits");
  //****************** ROOT FILE ***********************************
  // input ROOT data file containing the RAWData TTree that we'll
  // link to our RAWData structure 

  TFile   dataFile(inputFileName);
  TTree*  dataTree = (TTree*)dataFile.Get("RAWData");
  if(!dataTree)
    return -1; // can't read file
  RAWData data;
  
  data.TDCCh = new vector<int>; //List of hits and their channels
  data.TDCTS = new vector<float>; //List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();
  
  dataTree->SetBranchAddress("EventNumber",    &data.iEvent);
  dataTree->SetBranchAddress("number_of_hits", &data.TDCNHits);
  dataTree->SetBranchAddress("TDC_channel",    &data.TDCCh);
  dataTree->SetBranchAddress("TDC_TimeStamp",  &data.TDCTS);
  
  //****************** MACRO ***************************************
  double numGoodEvents = 0; 
  unsigned int nEntries = dataTree->GetEntries();
  
  for(unsigned int i = 0; i < nEntries; i++) {
    dataTree->GetEntry(i);
  
    for(int h = 0; h < data.TDCNHits; h++) {
      bool isCh = true;
      if(data.TDCTS->at(h) > lowTSThr && data.TDCTS->at(h) < highTSThr && 
         data.TDCCh->at(h) >= firstCh && data.TDCCh->at(h) <= lastCh) {
        for(int i = 0; i < numChMask; i++) {
          if(data.TDCCh->at(h) == mask[i])
            isCh = false;
        }
        if(isCh) {
          numGoodEvents++;
          hHit->Fill(data.TDCCh->at(h));
  //        break;
        }
      }
    }
  }
  dataFile.Close();
  hHit->SetTitle(Form("%s", plotName));
  hHit->SetStats(0);
  TCanvas *c1 = new TCanvas(plotName);
  hHit->Draw();

   TString dir(dirName);
   dir+="_"; dir+="param_";
   dir+="lowTsThr-"; dir+=lowTSThr;
   dir+="_highTSThr-"; dir+=highTSThr;
   const char* dirc=dir.Data();
  writeObject(dirc, c1);
  return 1;
}

int Analysis::noiseThrScan(char** inputFileNames, char* dirName, char* plotName, int numInFiles,  double acqTime)
{
  double valueNoise[numInFiles];
  double thr[numInFiles];
  for(int i = 0; i < numInFiles; i++) {
    valueNoise[i] = noise(inputFileNames[i], acqTime);
    if(valueNoise[i] == -1)
      return 0;
    thr[i] = threshold[i];
  }
  double vol = voltage[0];
  TGraph *grNoise = new TGraph(numInFiles, thr, valueNoise);
  grNoise->SetName(Form("Hits/sec/spill, voltage = %.2fV", vol));
  grNoise->SetTitle(Form("Hits/sec/spill, voltage = %.2fV", vol));
  grNoise->GetXaxis()->SetTitle("Threshold, mV");
  grNoise->GetYaxis()->SetTitle("hits/acqTime, Hz");
  grNoise->SetMarkerStyle(8);
  grNoise->SetLineStyle(9);
  grNoise->SetFillColor(0);
  grNoise->SetLineWidth(1);
   TString dir(dirName);
   dir+="_"; dir+="param_";
   dir+="ch1-"; dir+=firstCh; dir+="_"; 
   dir+="ch2-";dir+=lastCh;  dir+="_";
   dir+="acqTime-"; dir+=Form("%.2f", acqTime);
   const char* dirc=dir.Data();
  writeObject(dirc, grNoise);
  grNoise->Delete();
  return 1;
}

int Analysis::noiseVolScan(char** inputFileNames, char* dirName, char* plotName, int numInFiles,  double acqTime)
{
  double valueNoise[numInFiles];
  double vol[numInFiles];
  for(int i = 0; i < numInFiles; i++) {
    valueNoise[i] = noise(inputFileNames[i], acqTime);
    if(valueNoise[i] == -1)
      return 0;
    vol[i] = voltage[i];
  }
  TString dir(plotName);
  dir+="_"; dir+="param_";
  dir+="ch1-"; dir+=firstCh; dir+="_"; 
  dir+="ch2-";dir+=lastCh;  dir+="_"; 
  dir+="acqTime-"; dir+=Form("%.2f", acqTime);
  const char* dirc=dir.Data();
  double thr = threshold[0];
  TGraph *grNoise = new TGraph(numInFiles, voltage, valueNoise);
  grNoise->SetName(Form("%s, threshold = %.2fmV", dirc, thr));
  grNoise->SetTitle(Form("%s, threshold = %.2fmV", plotName, thr));
  grNoise->GetXaxis()->SetTitle("Voltage, V");
  grNoise->GetYaxis()->SetTitle("hits/sec/spill, Hz");
  grNoise->SetMarkerStyle(8);
  grNoise->SetLineStyle(9);
  grNoise->SetFillColor(0);
  grNoise->SetLineWidth(1);
  writeObject(dirName, grNoise);
  grNoise->Delete();
  return 1;
}
//-------------------------------------------------------


//-------------------------------------------------------
int Analysis::loop(char** inputFileNames, char* dirName, char* plotName, int numInFiles, char* nameType, double *param, int numParam)
{
  if(strncmp(nameType, "thrEff", 6) == 0) { // reshold scan
    if(numParam != 2) {
      cout << "ERROR: incorrect number for parameters!" << endl;
      cout << "For Type: " << nameType << " Need two parametors." << endl;
      return 1;
    }
    double lowTimeStampThr = param[0];
    double highTimeStampThr = param[1]; 

    int isThrEff = thrEffScan(inputFileNames, dirName, plotName,  numInFiles, lowTimeStampThr, highTimeStampThr);
    if(isThrEff == 0)
      cout << "ERROR: Can't calculate efficiency for threshold scan." << endl;
      return 1;
  }
  
  if(strncmp(nameType, "volEff", 6) == 0) { // voltage scan
    
    if(numParam != 2) {
      cout << "ERROR: incorrect number for parameters!" << endl;
      cout << "For Type: " << nameType << " Need two parametors." << endl;
      return 1;
    }
    double lowTimeStampThr = param[0];
    double highTimeStampThr = param[1]; 

    int isVolEff = volEffScan(inputFileNames, dirName, plotName, numInFiles, lowTimeStampThr, highTimeStampThr);
    if(isVolEff == 0)
      cout << "ERROR: Can't calculate efficiency for HV scan." << endl;
      return 1;
  }
  
  if(strncmp(nameType, "noiseHist", 9) == 0) { // noise
    
    if(numParam != 1) {
      cout << "ERROR: incorrect number for parameters!" << endl;
      cout << "For Type: " << nameType << " Need one parametor." << endl;
      return 1;
    }
    if(numInFiles > 1) {
      cout << "ERROR: Set only one input file." << endl;
      return 1;
    }
    int isNoise = noiseHist(inputFileNames[0], dirName, plotName, param[0]);
    if(isNoise == 0)
      cout << "ERROR: Can't plot." << endl;
      return 1;
  }
  
  if(strncmp(nameType, "stripHist", 9) == 0) { // voltage scan
    
    if(numParam != 2) {
      cout << "ERROR: incorrect number for parameters!" << endl;
      cout << "For Type: " << nameType << " Need two parametors." << endl;
      return 1;
    }
    if(numInFiles > 1) {
      cout << "ERROR: Set only one input file." << endl;
      return 1;
    }
    double lowTimeStampThr = param[0];
    double highTimeStampThr = param[1]; 

    int isStripHist = stripHist(inputFileNames[0], dirName, plotName, lowTimeStampThr, highTimeStampThr);
    if(isStripHist == 0)
      cout << "ERROR: Can't plot" << endl;
      return 1;
  }
  
  if(strncmp(nameType, "noiseVolScan", 12) == 0) { // noise
    
    if(numParam != 1) {
      cout << "ERROR: incorrect number for parameters!" << endl;
      cout << "For Type: " << nameType << " Need one parametor." << endl;
      return 1;
    }
    int isNoiseVolScan = noiseVolScan(inputFileNames, dirName, plotName, numInFiles, param[0]);
    if(isNoiseVolScan == 0)
      cout << "ERROR: Can't plot." << endl;
      return 1;
  }

  if(strncmp(nameType, "noiseThrScan", 12) == 0) { // noise
    
    if(numParam != 1) {
      cout << "ERROR: incorrect number for parameters!" << endl;
      cout << "For Type: " << nameType << "Need one parametor." << endl;
      return 1;
    }
    int isNoiseThrScan = noiseThrScan(inputFileNames, dirName, plotName, numInFiles, param[0]);
    if(isNoiseThrScan == 0)
      cout << "ERROR: Can't plot." << endl;
      return 1;
  }
  else
    cout << "Can't find type" << endl;
  return 1;
  }
//-------------------------------------------------------