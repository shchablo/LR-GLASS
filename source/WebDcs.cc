#include "WebDcs.hh"

/* Constructor, destructor. */ 
//------------------------------------------------------------------------------
/*! \fn WebDcs
* \brief   constructtor
*/
WebDcs::WebDcs()
{
  isDaqInputFileNames_ = false; daqNumInFiles_ = -1;
  isCaenInputFileNames_ = false; caenNumInFiles_ = -1;
  isMask_ = false; firstCh_ = 0; lastCh_ = 0; numChMask_ = 0;
  isMap_ = false; numChMap_ = 0;

  data_.TDCCh = new vector<int>; //List of hits and their channels
  data_.TDCTS = new vector<float>; //List of the corresponding time stamps
}

/*! \fn ~WebDcs
* \brief  destructor
*/
WebDcs::~WebDcs()
{
    dataDaq_->Close();
    dataCaen_->Close();
}
//------------------------------------------------------------------------------

/* Set functions. */ 
//------------------------------------------------------------------------------
bool WebDcs::setDaqInputFileNames(char **inputFileNames, int numInFiles) 
{
  daqInputFileNames_ = inputFileNames;
  daqNumInFiles_ = numInFiles;
  if(daqNumInFiles_ > 0)
    isDaqInputFileNames_ = true;
  else
    return false;
  return true;
}

bool WebDcs::setCaenInputFileNames(char **inputFileNames, int numInFiles) 
{
  caenInputFileNames_ = inputFileNames;
  caenNumInFiles_ = numInFiles;
  if(caenNumInFiles_ > 0)
    isCaenInputFileNames_ = true;
  else
    return false;
  return true;
}

bool  WebDcs::setMask(int firstCh, int lastCh, int *mask, int numChMask) 
{
  if(lastCh - firstCh <= 0)
    return false;
  for(int i = 0; i < numChMask; i++) {
    if(mask[i] < firstCh || mask[i] > lastCh)
      return false;
  }
  firstCh_ = firstCh;
  lastCh_ = lastCh;
  mask_ = mask; 
  numChMask_ = numChMask;
  isMask_ = true;
  return true;
}

bool  WebDcs::setMap(MAP *map, int numChMap) 
{
  if(!isMask_)
    return false;
  if(numChMap == lastCh_ - firstCh_ - numChMask_ + 1) {
    map_ = map;
    numChMap_ = numChMap;
    isMap_ = true;
  }
  else
    return false;
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
  return true;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool WebDcs::setRunFile(int indexFile) 
{
  if(indexFile > 0) {
    dataCaen_->Close();
    dataDaq_->Close();
  }
  dataCaen_ = new TFile(caenInputFileNames_[indexFile]);
  dataDaq_ = new TFile(daqInputFileNames_[indexFile]);
  dataTree_ = (TTree*)dataDaq_->Get("RAWData");
  if(!dataTree_)
    return false; // can't read file
  data_.TDCCh->clear();
  data_.TDCTS->clear();
  dataTree_->SetBranchAddress("EventNumber",    &data_.iEvent);
  dataTree_->SetBranchAddress("number_of_hits", &data_.TDCNHits);
  dataTree_->SetBranchAddress("TDC_channel",    &data_.TDCCh);
  dataTree_->SetBranchAddress("TDC_TimeStamp",  &data_.TDCTS);
  return true;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double WebDcs::getCurrent() 
{
  TH1F *h = (TH1F*)dataCaen_->Get("Imon_LYON-RE1-1-LRGRPC-00;1"); 
  return double(h->GetMean());
}

double WebDcs::getVoltage() 
{
  TH1F *h = (TH1F*)dataCaen_->Get("HVeff_LYON-RE1-1-LRGRPC-00;1"); 
  return double(h->GetMean());
}

int WebDcs::getEntries() 
{
  return dataTree_->GetEntries();
}

bool WebDcs::getEvent(int nEvent,
              int *TDCNHits, vector<int> *TDCCh, vector<double> *TDCTS)
{
   data_.TDCCh->clear();
   data_.TDCTS->clear();
   *TDCNHits = 0;
   TDCCh->clear();
   TDCTS->clear();
   dataTree_->GetEntry(nEvent);

  for(int i = 0; i < data_.TDCNHits; i++) {
    bool isHit = true;  
    for(int j = 0; j < numChMask_; j++) {
      if(data_.TDCCh->at(i) == mask_[j] 
         || (data_.TDCCh->at(i) <= firstCh_ || data_.TDCCh->at(i) >= lastCh_))
        isHit = false;
    }
    if(isHit) {
      *TDCNHits += 1;
      TDCTS->push_back((double)data_.TDCTS->at(i));
      int mapCh = 0; 
      for(int j = 0; j < numChMap_; j++) {
        if(data_.TDCCh->at(i) == map_[j].TDC) {
          mapCh =  map_[j].real;
          break;
        }
      }
      TDCCh->push_back(mapCh);
    }
  }
  return true;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
string WebDcs::getDaqName(int index)
{
  char result[128];
  string str(daqInputFileNames_[index]);
  char * cstr = new char [str.length()+1];
  strcpy (cstr, str.c_str());
  char* pch;
  pch = strtok (cstr,"/");
  while (pch != NULL) {
    pch = strtok (NULL, "/");
    if(pch == NULL)
      break;
    strcpy (result, pch);
  }
  pch = strtok (result, ".");
  strcpy (result, pch);
  return result;
}
//------------------------------------------------------------------------------

