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
void  Analysis::setMap(MAP *map) 
{
  map_ = map;
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
    hHit->GetYaxis()->SetTitle("hits/sec/strip");
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
  hHit->SetTitle(Form("From left to right: %s", plotName));
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

int Analysis::stripHist(char* inputFileName, char*dirName, char* plotName, double lowTSThr, double highTSThr, double range,
    int lowTDCNHits, int highTDCNHits, int maxCount)
{
    double min = 1000000; 
    double max = 0;
    for(int i = 0; i < 32; i++) {
      if(min >= map_[i].real)
        min = map_[i].real;
      if(max <= map_[i].real)
        max = map_[i].real;
    }
    TH1F *multiplicity = new TH1F("multiplicity", "multiplicity", 100, 0, 100);
    multiplicity->GetXaxis()->SetTitle(Form("multiplicity, range %2.fns", range));
    multiplicity->GetYaxis()->SetTitle("number of events");

    TH1F *profileStrip = new TH1F("profileStrip", "profileStrip", 32, min, max);
    profileStrip->GetXaxis()->SetTitle("strip");
    profileStrip->GetYaxis()->SetTitle("number of events");

    TH1F *NHits = new TH1F("NHits", "NHits", 100, 0, 100);
    NHits->GetXaxis()->SetTitle("NHits");
    NHits->GetYaxis()->SetTitle("number of events");
    
    TH1F *profileStripAfterFilter = new TH1F("profileStripAfterFilter", "profileStripAfterFilter", 32, min, max);
    profileStripAfterFilter->GetXaxis()->SetTitle("strip");
    profileStripAfterFilter->GetYaxis()->SetTitle("number of events");
    
    TH1F *NHitsAfterFilter = new TH1F("NHitsAfterFilter", "NHitsAfterFilter", 100, 0, 100);
    NHitsAfterFilter->GetXaxis()->SetTitle("NHits");
    NHitsAfterFilter->GetYaxis()->SetTitle("number of events");

  double minTime = 10000000; 
  double maxTime = 0;
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
          for(int j = 0; j < 32; j++) {
            if(data.TDCCh->at(h) == map_[j].TDC) {
              if(minTime >= data.TDCTS->at(h))
                minTime = data.TDCTS->at(h);
              if(maxTime <= data.TDCTS->at(h))
                maxTime = data.TDCTS->at(h);
              break;
            }
          }
        }
      }
    }
    if(i%((nEntries)/80*1) == 0) {                                                                       
      cout << "\r";                                                            
      for(int j = 0; j < i/((nEntries)/80*1); j++)                                                       
        cout << "#";                                                                                                                
      for(int j = i/((nEntries)/80*1); j < 80; j++)                                                      
        cout << "-";      
    }
  }

  int numBin = (int)((maxTime-minTime)/4);
  TH1F *profileTime = new TH1F("profieTime", "profileTime", numBin, minTime, maxTime);
  profileTime->GetXaxis()->SetTitle("time stamp (ns), (binSize 4ns)");
  profileTime->GetZaxis()->SetTitle("number of events");
  TH1F *profileTimeAfterFilter = new TH1F("profileTimeAfterFilter", "profileTimeAfterFilter", numBin, minTime, maxTime);
  profileTimeAfterFilter->GetXaxis()->SetTitle("time stamp (ns), (binSize 4ns)");
  profileTimeAfterFilter->GetYaxis()->SetTitle("number of events");

  for(unsigned int i = 0; i < nEntries; i++) {
    dataTree->GetEntry(i);

    int count = 0;
    for(int h = 0; h < data.TDCNHits; h++) {
      bool isCh = true;
      for(int i = 0; i < numChMask; i++) {
        if(data.TDCCh->at(h) == mask[i])
        isCh = false;
      }
      if(data.TDCCh->at(h) >= firstCh && data.TDCCh->at(h) <= lastCh && isCh) {
        for(int h2 = 0; h2 < data.TDCNHits; h2++) {
          if(h != h2 && data.TDCTS->at(h) + range >= data.TDCTS->at(h2) && data.TDCTS->at(h) <= data.TDCTS->at(h2) && isCh
              && (data.TDCCh->at(h2) <= data.TDCCh->at(h)-1 || data.TDCCh->at(h2) >= data.TDCCh->at(h)+1))
            count += 1;
        }
        multiplicity->Fill(count);
        if(isCh) {
          for(int j = 0; j < 32; j++) {
            if(data.TDCCh->at(h) == map_[j].TDC) {
              profileTime->Fill(data.TDCTS->at(h));
              profileStrip->Fill(map_[j].real);
              if(count <= maxCount && data.TDCTS->at(h) > lowTSThr && data.TDCTS->at(h) < highTSThr
                 && data.TDCNHits > lowTDCNHits && data.TDCNHits < highTDCNHits) {
                profileTimeAfterFilter->Fill(data.TDCTS->at(h));
                profileStripAfterFilter->Fill(map_[j].real);
              }
              break;
            }
          }
        }
      }
    }
    NHits->Fill(data.TDCNHits);
    if(count <= maxCount)
      NHitsAfterFilter->Fill(data.TDCNHits);
    if(i%((nEntries)/80*1) == 0) {                                                                       
      cout << "\r";                                                            
      for(int j = 0; j < i/((nEntries)/80*1); j++)                                                       
        cout << "#";                                                                                                                
      for(int j = i/((nEntries)/80*1); j < 80; j++)                                                      
        cout << "-";      
    }
  }
  multiplicity->SetTitle(Form("Multiplicity profile. T3S2, %s", plotName));
  multiplicity->SetStats(10);

  profileTime->SetTitle(Form("Time profile. T3S2, %s", plotName));
  profileTime->SetStats(1);
  profileStrip->SetTitle(Form("Strip profile. T3S2, %s", plotName));
  profileStrip->SetStats(10);
  NHits->SetTitle(Form("NHits profile. T3S2, %s", plotName));
  NHits->SetStats(10);
  
  profileTimeAfterFilter->SetTitle(Form("Time profile after filter. T3S2, %s", plotName));
  profileTimeAfterFilter->SetStats(1);
  profileStripAfterFilter->SetTitle(Form("Strip profile after filter. T3S2, %s", plotName));
  profileStripAfterFilter->SetStats(10);
  NHitsAfterFilter->SetTitle(Form("NHits profile after filter. T3S2, %s", plotName));
  NHitsAfterFilter->SetStats(10);

  TString dir(dirName);

  dir+="_"; dir+="param_";
  dir+="lowTsThr-"; dir+=lowTSThr;
  dir+="_highTSThr-"; dir+=highTSThr;
  dir+="_lowNHit-"; dir+=lowTDCNHits;
  dir+="_highNHit-"; dir+=highTDCNHits;
  dir+="_range-"; dir+=range;
  dir+="_maxMultiplicity-"; dir+=maxCount;
  const char* dirc=dir.Data();

  writeObject(dirc, multiplicity);

  writeObject(dirc, profileTime);
  writeObject(dirc, profileStrip);
  writeObject(dirc, NHits);

  writeObject(dirc, profileTimeAfterFilter);
  writeObject(dirc, profileStripAfterFilter);
  writeObject(dirc, NHitsAfterFilter);

  dataFile.Close();
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
  grNoise->SetName(Form("%s, voltage = %.2fV", plotName, vol));
  grNoise->SetTitle(Form("%s, voltage = %.2fV",plotName, vol));
  grNoise->GetXaxis()->SetTitle("Threshold, mV");
  grNoise->GetYaxis()->SetTitle("hits/sec/strip");
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
  grNoise->GetYaxis()->SetTitle("hits/sec/strip");
  grNoise->SetMarkerStyle(8);
  grNoise->SetLineStyle(9);
  grNoise->SetFillColor(0);
  grNoise->SetLineWidth(1);
  writeObject(dirName, grNoise);
  grNoise->Delete();
  return 1;
}

int Analysis::corrMatrix(char* inputFileName, char* dirName, char* plotName,  double range)
{
    double min = 1000000; 
    double max = 0;
    for(int i = 0; i < 32; i++) {
      if(min >= map_[i].real)
        min = map_[i].real;
      if(max <= map_[i].real)
        max = map_[i].real;
    }
  //****************** ROOT FILE ***********************************
  // input ROOT data file containing the RAWData TTree that we'll
  // link to our RAWData structure
 TH2D *h2e = new TH2D(Form("Correlation %s", plotName), Form("Correlation %s", plotName), 32, min, max, 32, min, max);
 h2e->GetXaxis()->SetTitle("strip");
 h2e->GetYaxis()->SetTitle("strip");
 TH1D *h1e = new TH1D(Form("CorrelationSum %s", plotName), Form("CorrelationSum %s", plotName), 32, min, max);
 h1e->GetXaxis()->SetTitle("strip");
  
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
  double M = 255;
  unsigned int nEntries = dataTree->GetEntries();
  for(unsigned int i = 0; i < nEntries; i++) {
  int resultH = 0;
  int resultCh1 = 0;
    double time = 0;
    bool read  = false;
    dataTree->GetEntry(i);
    for(int h = 0; h < data.TDCNHits; h++) {
      if(data.TDCNHits < 1000) {
        for(int h2 = 0; h2 < data.TDCNHits; h2++) {
          bool isCh = true;
          if((data.TDCCh->at(h) >= firstCh && data.TDCCh->at(h) <= lastCh) ||
            (data.TDCCh->at(h2) >= firstCh && data.TDCCh->at(h2) <= lastCh)) {
            for(int i = 0; i < numChMask; i++) {
              if(data.TDCCh->at(h) == mask[i] || data.TDCCh->at(h2) == mask[i])
                isCh = false;
              }
            if(data.TDCTS->at(h) + range >= data.TDCTS->at(h2) && data.TDCTS->at(h) <= data.TDCTS->at(h2) && isCh
              && data.TDCCh->at(h2) >= data.TDCCh->at(h)-1 && data.TDCCh->at(h2) <= data.TDCCh->at(h)+1) {
            int ch1 = 0;
            for(int j = 0; j < 32; j++) {
              if(data.TDCCh->at(h) == map_[j].TDC) {
                ch1 = map_[j].real;
                break;
              }
            }
            int ch2 = 0;
            for(int j = 0; j < 32; j++) {
              if(data.TDCCh->at(h2) == map_[j].TDC) {
                ch2 = map_[j].real;
                break;
              }
            }
            h2e->Fill(ch1, ch2);
            }
            if(data.TDCCh->at(h) == data.TDCCh->at(h2)) {
              if(abs(M-time) >= abs(M-data.TDCTS->at(h))) {
                time = data.TDCTS->at(h);
                resultH = data.TDCCh->at(h);
                read  = true;
              }
            }
          }
        }
        for(int j = 0; j < 32; j++) {
          if(resultH == map_[j].TDC) {
            resultCh1 = map_[j].real;
            break;
          }
        }
      }
    }
    if(read) {
//      h2e->Fill(resultCh1, resultCh1);
      h1e->Fill(resultCh1);
    }
  }

  dataFile.Close();

  TString dir(plotName);
  dir+="_"; dir+="param_";
  dir+="ch1-"; dir+=firstCh; dir+="_"; 
  dir+="ch2-";dir+=lastCh;  dir+="_"; 
  dir+="range-"; dir+=Form("%.2f", range);
  const char* dirc=dir.Data();

  writeObject(dirName, h2e);
  h2e->Delete();
  writeObject(dirName, h1e);
  h1e->Delete();
  return 1;
}

int Analysis::timeCorrMatrix(char* inputFileName, char* dirName, char* plotName,  double range)
{
 TH2D *hStrip = new TH2D(Form("Strip %s", plotName), Form("Strip %s", plotName), 127, 1, -1, 200, 1, -1);
 hStrip->GetXaxis()->SetTitle("strip");
 hStrip->GetYaxis()->SetTitle("multiplicity");
 TH2D *hTime = new TH2D(Form("Time %s", plotName), Form("Time %s", plotName), 300, 1, -1, 200, 1, -1);
 hTime->GetXaxis()->SetTitle("time");
 hTime->GetYaxis()->SetTitle("multiplicity");
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
  double count = 0; 
  unsigned int nEntries = dataTree->GetEntries();
  
  for(unsigned int i = 0; i < nEntries; i++) {
    dataTree->GetEntry(i);
  
    for(int h = 0; h < data.TDCNHits; h++) {
      if(data.TDCCh->at(h) >= firstCh && data.TDCCh->at(h) <= lastCh) {
        for(int h2 = 0; h2 < data.TDCNHits; h2++) {
          if(data.TDCCh->at(h) == data.TDCCh->at(h2))
            count += 1;
        }
      }
    }
    for(int h = 0; h < data.TDCNHits; h++) {
      if(data.TDCCh->at(h) >= firstCh && data.TDCCh->at(h) <= lastCh) {
        hStrip->Fill(data.TDCCh->at(h), count);
        hTime->Fill(data.TDCTS->at(h), count);
      }
    }
  }
  dataFile.Close();

  TString dir(plotName);
  dir+="_"; dir+="param_";
  dir+="ch1-"; dir+=firstCh; dir+="_"; 
  dir+="ch2-";dir+=lastCh;  dir+="_"; 
  dir+="range-"; dir+=Form("%.2f", range);
  const char* dirc=dir.Data();

  writeObject(dirName, hTime);
  hTime->Delete();
  writeObject(dirName, hStrip);
  hStrip->Delete();
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
  
  if(strncmp(nameType, "stripHist", 9) == 0) { // stripHist
    
    if(numParam != 6) {
      cout << "ERROR: incorrect number for parameters!" << endl;
      cout << "For Type: " << nameType << " Need two parametors." << endl;
      return 1;
    }
    if(numInFiles > 1) {
      cout << "ERROR: Set only one input file." << endl;
      return 1;
    }

    int isStripHist = stripHist(inputFileNames[0], dirName, plotName, param[0], param[1], param[4], param[2], param[3], param[5]);
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
  if(strncmp(nameType, "corrMatrix", 10) == 0) { // noise
    
    if(numParam != 1) {
      cout << "ERROR: incorrect number for parameters!" << endl;
      cout << "For Type: " << nameType << " Need one parametor." << endl;
      return 1;
    }
    if(numInFiles > 1) {
      cout << "ERROR: Set only one input file." << endl;
      return 1;
    }
    int isCorrMatrix = corrMatrix(inputFileNames[0], dirName, plotName, param[0]);
    int isTimeCorrMatrix = timeCorrMatrix(inputFileNames[0], dirName, plotName, param[0]);
    if(isCorrMatrix == 0)
      cout << "ERROR: Can't plot." << endl;
      return 1;
  }
  else
    cout << "Can't find type" << endl;
  return 1;
  }
//-------------------------------------------------------
