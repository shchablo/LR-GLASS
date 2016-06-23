#include "Analysis.hh"
/*! \fn Analysis
* \brief   constructtor
*/
Analysis::Analysis()
{
  isInputFileNames_ = false; numInFiles_ = -1;
  isParam_ = false; numParam_ = -1;
  isThreshold_ = false;
  isVoltage_ = false;
  isMask_ = false; firstCh_ = 0; lastCh_ = 0; numChMask_ = 0;
  isMap_ = false; numChMap_ = 0;
}

/*! \fn ~Analysis
* \brief  destructor
*/
Analysis::~Analysis()
{
}
int Analysis::indexMinMax(int length, double* data, int flag)
{
    int index = 0;
    double result = data[0];
    for(int i = 1; i < length; i++) {
        if(flag == -1) {
            if(result > data[i]) {
                result = data[i];
                index = i;
            }
        }
        else {
            if(result < data[i]) {
                result = data[i];
                index = i;
            }
        }
    }
    return index;
}
//-------------------------------------------------------
int Analysis::setInputFileNames(char **inputFileNames, int numInFiles) 
{
  inputFileNames_ = inputFileNames;
  numInFiles_ = numInFiles;
  if(numInFiles_ > 0)
    isInputFileNames_ = true;
  else
    return -10;
  return 1;
}

int Analysis::setParam(double *param, int numParam) 
{
  param_ = param;
  numParam_ = numParam;
  if(numParam > 0)
    isParam_ = true;
  else
    return -20;
  return 1;

}

void Analysis::setThreshold(double *threshold) 
{
  thr_ = threshold;
  isThreshold_ = true;
}

void Analysis::setVoltage(double *voltage) 
{
  volt_ = voltage;
  isVoltage_ = true;
}

int  Analysis::setMask(int firstCh, int lastCh, int *mask, int numChMask) 
{
  if(lastCh - firstCh <= 0)
    return -30;
  for(int i = 0; i < numChMask; i++) {
    if(mask[i] < firstCh || mask[i] > lastCh)
      return -31;
  }
  firstCh_ = firstCh;
  lastCh_ = lastCh;
  mask_ = mask; 
  numChMask_ = numChMask;
  isMask_ = true;
  return 1;
}

int  Analysis::setMap(MAP *map, int numChMap) 
{
  if(!isMask_)
    return -40;
  if(numChMap == lastCh_ - firstCh_ - numChMask_ + 1) {
    map_ = map;
    numChMap_ = numChMap;
    isMap_ = true;
  }
  else
    return -41;
  int chMinMap = std::numeric_limits<int>::max();
  int chMaxMap = 0;
  for(int i = 0; i < numChMap; i++) {
    if(chMinMap >= map_[i].real)
      chMinMap = map_[i].real;
    if(chMaxMap <= map_[i].real)
      chMaxMap = map_[i].real;
  }
  chMinMap_ = chMinMap;
  chMaxMap_ = chMaxMap;
  return 1;
}

void Analysis::setDirName(char *dirName) 
{
  dirName_ = dirName;
}

void Analysis::setPlotName(char *plotName) 
{
  plotName_ = plotName;
}
//-------------------------------------------------------

int Analysis::getEntries(char *inputFileName) 
{
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
  
  int nEntries = dataTree->GetEntries();
  
  return nEntries;
}

RAWData Analysis::getEvent(int nEvent, char *inputFileName) 
{
  RAWData data;
  data.check = true;
  TFile   dataFile(inputFileName);
  TTree*  dataTree = (TTree*)dataFile.Get("RAWData");
  if(!dataTree) {
    data.check = false;
    return data; // can't read file
  }
  
  data.TDCCh = new vector<int>; //List of hits and their channels
  data.TDCTS = new vector<float>; //List of the corresponding time stamps
  data.TDCCh->clear();
  data.TDCTS->clear();
  
  dataTree->SetBranchAddress("EventNumber",    &data.iEvent);
  dataTree->SetBranchAddress("number_of_hits", &data.TDCNHits);
  dataTree->SetBranchAddress("TDC_channel",    &data.TDCCh);
  dataTree->SetBranchAddress("TDC_TimeStamp",  &data.TDCTS);
  
  //****************** MACRO ***************************************
  dataTree->GetEntry(nEvent);
  dataFile.Close();
  return data;
}

RAWData Analysis::getData(RAWData *read) 
{
  RAWData data;
  data.TDCCh = new vector<int>; data.TDCCh->clear();
  data.TDCTS = new vector<float>; data.TDCTS->clear();
  data.check = false;
  data.TDCNHits = 0;
  for(int i = 0; i < read->TDCNHits; i++) {
    bool isHit = true;  
    for(int j = 0; j < numChMask_; j++) {
      if(read->TDCCh->at(i) == mask_[j] 
         || (read->TDCCh->at(i) <= firstCh_ || read->TDCCh->at(i) >= lastCh_))
        isHit = false;
    }
    if(isHit) {
      data.TDCNHits += 1;
      data.TDCTS->push_back(read->TDCTS->at(i));
      int mapCh = 0; 
      for(int j = 0; j < numChMap_; j++) {
        if(read->TDCCh->at(i) == map_[j].TDC) {
          mapCh =  map_[j].real;
          break;
        }
      }
      data.TDCCh->push_back(mapCh);
    }
  }
  data.check = true;
  return data; 
}

RAWData Analysis::Filter(RAWData *data) 
{
  RAWData filter;
  filter.TDCCh = new vector<int>; filter.TDCCh->clear();
  filter.TDCTS = new vector<float>; filter.TDCTS->clear();
  filter.check = false;
  filter.TDCNHits = 0;
  for(int i = 0; i < data->TDCNHits; i++) {
    int multiplicity = 0;
    for(int h2 = 0; h2 < data->TDCNHits; h2++) {                                                                                   
    if(i != h2 && data->TDCTS->at(i) + 6 >= data->TDCTS->at(h2) && data->TDCTS->at(i) <= data->TDCTS->at(h2) 
            && (data->TDCCh->at(h2) <= data->TDCCh->at(i)-1 || data->TDCCh->at(h2) >= data->TDCCh->at(i)+1))
        multiplicity += 1;
}    
    if(multiplicity <= 1) {
      filter.TDCNHits += 1;
      filter.TDCCh->push_back(data->TDCCh->at(i));
      filter.TDCTS->push_back(data->TDCTS->at(i));
    }
  }
  filter.check = true;
  return filter; 
}

double Analysis::getCurrent(int index) 
{
   TFile dataFile(inputFileNames_[index]);
   TH1F *h = (TH1F*)dataFile.Get("Imon_LYON-RE1-1-LRGRPC-00;1"); 

  return double(h->GetMean());
}

int Analysis::getMinMaxTime(double binTime, double *minTime, double *maxTime, int *numBins) 
{
  int numEntries = getEntries(inputFileNames_[0]);
  if(!numEntries)
    return -1;

  *minTime = std::numeric_limits<double>::max();
  *maxTime = 0;
  for(int i = 0; i < numEntries; i++) {
    RAWData read = getEvent(i, inputFileNames_[0]);
    if(!read.check)
      return -100;
     RAWData data = getData(&read);
    if(!data.check)
      return -101;

    for(int j = 0; j < data.TDCNHits; j++) {
      if(*minTime >= data.TDCTS->at(j))
        *minTime = data.TDCTS->at(j);
      if(*maxTime <= data.TDCTS->at(j))
        *maxTime = data.TDCTS->at(j);
    }
  }
  *numBins = int((*maxTime - *minTime)/binTime);
  return 1;
}

double Analysis::getMeanMoreTime(int hitIndex, RAWData *data)
{
  double meanMoreTime = 0;
  double count = 0;
  for(int i = 0; i < data->TDCNHits; i++) {
    if(data->TDCTS->at(i) > data->TDCTS->at(hitIndex)) {
      meanMoreTime += data->TDCTS->at(i); 
      count += 1;
    }
  }
  return meanMoreTime/count;
}

double Analysis::getMeanLessTime(int hitIndex, RAWData *data)
{
  double meanMoreTime = 0;
  double count = 0;
  for(int i = 0; i < data->TDCNHits; i++) {
    if(data->TDCTS->at(i) < data->TDCTS->at(hitIndex)) {
      meanMoreTime += data->TDCTS->at(i); 
      count += 1;
    }
  }
  return meanMoreTime/count;
}

int Analysis::hitsProfile() 
{

  TH1F *profileStrip = new TH1F("hitsProfile", "hitsProfile", numChMap_, chMinMap_, chMaxMap_);
  profileStrip->GetXaxis()->SetTitle("strip");
  profileStrip->GetYaxis()->SetTitle("number of hits");
  
  TH1F *profileStripFilter = new TH1F("hitsProfileFilter", "hitsProfileFilter", numChMap_, chMinMap_, chMaxMap_);
  profileStripFilter->GetXaxis()->SetTitle("strip");
  profileStripFilter->GetYaxis()->SetTitle("number of hits");

  
//  int numEntries = getEntries(inputFileNames_[0]);
  int numEntries = 4000;
  if(!numEntries)
    return -1;

  for(int i = 0; i < numEntries; i++) {
    RAWData read = getEvent(i, inputFileNames_[0]);
    if(!read.check)
      return -100;
     RAWData data = getData(&read);
    if(!data.check)
      return -101;
     RAWData filter = Filter(&data);
    if(!filter.check)
      return -101;

    for(int j = 0; j < data.TDCNHits; j++)
      profileStrip->Fill(data.TDCCh->at(j));
    for(int j = 0; j < filter.TDCNHits; j++)
      profileStripFilter->Fill(filter.TDCCh->at(j));
    
  }

  TString dir(dirName_);
  dir+="_"; dir+="param_";
  const char* dirc=dir.Data();

  writeObject(dirc, profileStrip);
  writeObject(dirc, profileStripFilter);

  return 1;
}

int Analysis::timeProfile() 
{

  double binTime = 1;
  double minTime = 0;
  double maxTime = 0;
  int numBins = 0;
  getMinMaxTime(binTime, &minTime, &maxTime, &numBins);
  TH1F *profileTime = new TH1F("timeProfile", "timeProfile", numBins, minTime, maxTime);
  profileTime->GetXaxis()->SetTitle("time");
  profileTime->GetYaxis()->SetTitle("number of hits");
  TH1F *profileTimeFilter = new TH1F("timeProfileFilter", "timeProfileFilter", numBins, minTime, maxTime);
  profileTimeFilter->GetXaxis()->SetTitle("time");
  profileTimeFilter->GetYaxis()->SetTitle("number of hits");

  
  int numEntries = getEntries(inputFileNames_[0]);
  if(!numEntries)
    return -1;

  for(int i = 0; i < numEntries; i++) {
    RAWData read = getEvent(i, inputFileNames_[0]);
    if(!read.check)
      return -100;
     RAWData data = getData(&read);
    if(!data.check)
      return -101;
     RAWData filter = Filter(&data);
    if(!filter.check)
      return -101;

    for(int j = 0; j < data.TDCNHits; j++) {
      profileTime->Fill(data.TDCTS->at(j));
    }
      profileTimeFilter->Fill(data.TDCNHits);

    for(int j = 0; j < filter.TDCNHits; j++) {
//      profileTimeFilter->Fill(filter.TDCTS->at(j));
    }
  }

  TString dir(dirName_);
  dir+="_"; dir+="param_";
  const char* dirc=dir.Data();

  writeObject(dirc, profileTime);
  writeObject(dirc, profileTimeFilter);

  return 1;
}

int Analysis::diffTimeProfile() 
{
}

int Analysis::meanMoreTimeProfile() 
{

  double binTime = 2;
  double minTime = 0;
  double maxTime = 0;
  int numBins = 0;
  getMinMaxTime(binTime, &minTime, &maxTime, &numBins);
  TH1F *profileTime = new TH1F("TimeProfile", "TimeProfile", numBins, minTime, maxTime);
  profileTime->GetXaxis()->SetTitle("time, ns");
  profileTime->GetYaxis()->SetTitle("hits");
  TH1F *profileStrip = new TH1F("StripProfile", "StripProfile", numChMap_, chMinMap_, chMaxMap_);
  profileStrip->GetXaxis()->SetTitle("strip");
  profileStrip->GetYaxis()->SetTitle("hits");
  
  TH1F *profileTimeFilter = new TH1F("TimeProfile_1", "TimeProfile_1", numBins, minTime, maxTime);
  profileTimeFilter->GetXaxis()->SetTitle("time, ns");
  profileTimeFilter->GetYaxis()->SetTitle("hits");
  TH1F *profileStripFilter = new TH1F("StripProfile_1", "StripProfile_1", numChMap_, chMinMap_, chMaxMap_);
  profileStripFilter->GetXaxis()->SetTitle("strip");
  profileStripFilter->GetYaxis()->SetTitle("hits");

  TH1F *profileClaster = new TH1F("ProfileClaster", "ProfileClaster", numChMap_, 0, numChMap_);
  profileClaster->GetXaxis()->SetTitle("count hits");
  profileClaster->GetYaxis()->SetTitle("hits");
  
  TH1F *profileTimeFilter2 = new TH1F("TimeProfile_2", "TimeProfile_2", numBins, minTime, maxTime);
  profileTimeFilter2->GetXaxis()->SetTitle("time, ns");
  profileTimeFilter2->GetYaxis()->SetTitle("hits");
  TH1F *profileStripFilter2 = new TH1F("StripProfile_2", "StripProfile_2", numChMap_, chMinMap_, chMaxMap_);
  profileStripFilter2->GetXaxis()->SetTitle("strip");
  profileStripFilter2->GetYaxis()->SetTitle("hits");
  TH1F *profileClasterFilter = new TH1F("ProfileClaster_1", "ProfileClaster_1", numChMap_, 0, numChMap_);
  profileClasterFilter->GetXaxis()->SetTitle("count hits");
  profileClasterFilter->GetYaxis()->SetTitle("hits");
  
  TH1F *profileTimeFilter3 = new TH1F("TimeProfile_3", "TimeProfile_3", numBins, minTime, maxTime);
  profileTimeFilter3->GetXaxis()->SetTitle("time, ns");
  profileTimeFilter3->GetYaxis()->SetTitle("hits");
  TH1F *profileStripFilter3 = new TH1F("StripProfile_3", "StripProfile_3", numChMap_, chMinMap_, chMaxMap_);
  profileStripFilter3->GetXaxis()->SetTitle("strip");
  profileStripFilter3->GetYaxis()->SetTitle("hits");
  
  TH1F *profileTimeCh[numChMap_];
  TH1F *profileTimeCh2[numChMap_];
  for(int i = 0; i < numChMap_; i++) {
    profileTimeCh[i] = new TH1F(Form("profileTime_ch%d",i), Form("profileTime_ch%d",i), numBins, minTime, maxTime);
    profileTimeCh[i]->GetXaxis()->SetTitle("time");
    profileTimeCh[i]->GetYaxis()->SetTitle("hits");
    profileTimeCh2[i] = new TH1F(Form("profileTimeFilter_ch%d",i), Form("profileTimeFilter_ch%d",i), numBins, minTime, maxTime);
    profileTimeCh2[i]->GetXaxis()->SetTitle("time");
    profileTimeCh2[i]->GetYaxis()->SetTitle("hits");
  }
  
  
  int numEntries = getEntries(inputFileNames_[0]);
  if(!numEntries)
    return -1;

  for(int i = 0; i < numEntries; i++) {
    RAWData read = getEvent(i, inputFileNames_[0]);
    if(!read.check)
      return -100;
     RAWData data = getData(&read);
    if(!data.check)
      return -101;

    for(int j = 0; j < data.TDCNHits; j++) {
    
      profileStrip->Fill(data.TDCCh->at(j));
      profileTime->Fill(data.TDCTS->at(j));
      for(int jj = 0; jj < numChMap_; jj++) {
        if(data.TDCCh->at(j) == map_[jj].real)
          profileTimeCh[jj]->Fill(data.TDCTS->at(j));
      }

      double time = data.TDCTS->at(j);
      int ch = data.TDCCh->at(j);
      int count = 0;
      int count2 = 0;
      ch = data.TDCCh->at(j);
      count = 0;
      time = data.TDCTS->at(j);
      for(int jj = 0; jj < data.TDCNHits; jj++) {
          if(((ch == data.TDCCh->at(jj) +1)) 
             && (time > data.TDCTS->at(jj))) {
           count +=1;
           ch = data.TDCCh->at(jj);
           time = data.TDCTS->at(jj);
          }
      }
      count2 = 0;
      time = data.TDCTS->at(j);
      ch = data.TDCCh->at(j);
      for(int jj = 0; jj < data.TDCNHits; jj++) {
          if((ch == data.TDCCh->at(jj) -1) 
             && (time > data.TDCTS->at(jj))) {
           count2 -= 1;
           ch = data.TDCCh->at(jj);
           time = data.TDCTS->at(jj);
          }
      }
      int mult = 0;
      for(int jj = 0; jj < data.TDCNHits; jj++) {
        if(data.TDCCh->at(j) != data.TDCCh->at(jj)
           && (data.TDCTS->at(j) + 6 > data.TDCTS->at(jj)) && (data.TDCTS->at(j) - 6 < data.TDCTS->at(jj)))
          mult+=1;
      }
      profileClaster->Fill(mult);
      if(mult <= 2) {
        profileStripFilter->Fill(data.TDCCh->at(j));
        profileTimeFilter->Fill(data.TDCTS->at(j));
      }
      if(count == 0 && count2 == 0) {
        profileStripFilter2->Fill(data.TDCCh->at(j));
        profileTimeFilter2->Fill(data.TDCTS->at(j));
        double multTime[mult];
        double multCh[mult];
        mult = 0;
        for(int jj = 0; jj < data.TDCNHits; jj++) {
          if(data.TDCCh->at(j) != data.TDCCh->at(jj)
            && (data.TDCTS->at(j) + 6 > data.TDCTS->at(jj)) && (data.TDCTS->at(j) - 6 < data.TDCTS->at(jj))) {
            multTime[mult] = data.TDCTS->at(jj);
            multCh[mult] = data.TDCCh->at(jj);
            mult+=1;
          }
        }
         profileClasterFilter->Fill(mult);
        if(mult <= 2) {
         profileStripFilter3->Fill(data.TDCCh->at(j));
         profileTimeFilter3->Fill(data.TDCTS->at(j));
          for(int jj = 0; jj < numChMap_; jj++) {
            if(data.TDCCh->at(j) == map_[jj].real)
              profileTimeCh2[jj]->Fill(data.TDCTS->at(j));
          }
        }
//        profileClasterFilter->Fill(mult);
//        int indexMin = indexMinMax(mult, multTime, -1);
//        profileStripFilter3->Fill(multCh[indexMin]);
//        profileTimeFilter3->Fill(multTime[indexMin]);
//        for(int jj = 0; jj < numChMap_; jj++) {
//          if(data.TDCCh->at(j) == map_[jj].real)
//            profileTimeCh2[jj]->Fill(multTime[indexMin]);
//        }
      }
    }
  }

  TString dir(dirName_);
  dir+="_"; dir+="param_";
  const char* dirc=dir.Data();

  writeObject(dirc, profileClaster);
  writeObject(dirc, profileClasterFilter);
  writeObject(dirc, profileStrip);
  writeObject(dirc, profileStripFilter);
  writeObject(dirc, profileStripFilter2);
  writeObject(dirc, profileStripFilter3);
  writeObject(dirc, profileTime);
  writeObject(dirc, profileTimeFilter);
  writeObject(dirc, profileTimeFilter2);
  writeObject(dirc, profileTimeFilter3);
  TString dirCh("time");
  const char* cDirCh=dirCh.Data();
  for(int jj = 0; jj < numChMap_; jj++)
    writeObject(cDirCh, profileTimeCh[jj]);
  TString dirCh2("timeFilter");
  const char* cDirCh2=dirCh2.Data();
  for(int jj = 0; jj < numChMap_; jj++)
    writeObject(cDirCh2, profileTimeCh2[jj]);

  return 1;
}

int Analysis::currentSourceProfile() 
{

  double current[numInFiles_];
  double eCurrent[numInFiles_];
  double source[numInFiles_];
  double eSource[numInFiles_];
  for(int i = 0; i < numInFiles_; i++) {
    source[i] = 1/thr_[i];
    eSource[i] = 0;
    current[i] = getCurrent(i);
    eCurrent[i] = 0.2;
    cout << thr_[i] << " " << source[i] << " " << current[i] << endl;
  }
   TGraphErrors *profile = new TGraphErrors(numInFiles_, source, current, eSource, eCurrent); 
   profile->GetXaxis()->SetTitle("attenuator, U");
   profile->GetYaxis()->SetTitle("current, mA");

  profile->SetName(Form("%s, HV%2.f", plotName_, volt_[0]));
  profile->SetTitle(Form("%s, HV%2.f", plotName_, volt_[0]));
  TString dir(dirName_);
  dir+="_"; dir+="param_";
  const char* dirc=dir.Data();

  writeObject(dirc, profile);

  return 1;
}

int Analysis::effProfile() 
{
  double eff[numInFiles_];
  double eEff[numInFiles_];
  double HV[numInFiles_];
  double eHV[numInFiles_];
  for(int e = 0; e < numInFiles_; e++) {
    
//    int numEntries = getEntries(inputFileNames_[e]);
    int numEntries = 4000;
    if(!numEntries)
      return -1;
    double numGoodEvents = 0;
    for(int i = 0; i < numEntries; i++) {
      RAWData read = getEvent(i, inputFileNames_[e]);
      if(!read.check)
        return -100;
       RAWData data = getData(&read);
      if(!data.check)
        return -101;

      for(int j = 0; j < data.TDCNHits; j++) {
        if(data.TDCTS->at(j) > param_[0] && data.TDCTS->at(j) < param_[1]) {
          numGoodEvents += 1;
          break;
        }
      }
    }
    eff[e] = numGoodEvents/numEntries;
    eEff[e] = sqrt((numGoodEvents*(numEntries-numGoodEvents))/numEntries)/numGoodEvents;
    HV[e] = volt_[e];
    eHV[e] = 0;
  }

  TGraphErrors *gEff = new TGraphErrors(numInFiles_, HV, eff, eHV, eEff);
  gEff->SetName(Form("%s Efficiency, threshold = %.2fmV", plotName_, param_[2]));
  gEff->SetTitle(Form("%s Efficiency, threshold = %.2fmV", plotName_, param_[2]));
  gEff->GetXaxis()->SetTitle("Threshold, mV");
  gEff->GetYaxis()->SetTitle("Efficiency");
  gEff->SetMarkerStyle(8);
  gEff->SetLineStyle(9);
  gEff->SetFillColor(0);
  gEff->SetLineWidth(1);
   TString dir(dirName_);
   dir+="_"; dir+="param_";
   dir+="lowTSThr-"; dir+=param_[0]; dir+="_"; 
   dir+="highTSThr-";dir+=param_[1];  dir+="_"; 
   dir+="Thr-";dir+=param_[2];  dir+="_"; 
   const char* dirc=dir.Data();
   writeObject(dirc, gEff);
   gEff->Delete();

  return 1;
}

//-------------------------------------------------------
int Analysis::loop(char* nameType)
{

 if(strncmp(nameType, "hitsProfile", 11) == 0) {
   int isHitsProfile = hitsProfile();
  }
 if(strncmp(nameType, "timeProfile", 11) == 0) {
   int isTimeProfile = timeProfile();
  }
 if(strncmp(nameType, "meanMoreTimeProfile", 19) == 0) {
   int isMeanMoreTimeProfile = meanMoreTimeProfile();
  }
 if(strncmp(nameType, "currentSourceProfile", 20) == 0) {
   int isCurrentSourceProfile = currentSourceProfile();
  }
 if(strncmp(nameType, "eff", 3) == 0) {
   int isEff = effProfile();
  }
    
}

string Analysis::getError(int index)
{
  if(index == -10) // set names of files
    
    return "Number of input files should be more 0.";
  
  if(index == -20) // set param
    return "Number of parametors should be more or equally 0.";
  
  if(index == -30) // set param
    return "Range for strips isn't correct.";
  if(index == -31) // set param
    return "Mask for strips isn't correct. ";
  
  if(index == -40) // set param
    return "If You want put map for strips. You should set the range for strips.";
  if(index == -41) // set param
    return "Map isn't corret. Should be with same range (numStrips - numChMask)";
  
  if(index == 1)
    return "No errors";
  
  return "Not correct index for error.";
}
//-------------------------------------------------------
