#include "KCluster.hh"
/* Constructor, destructor. */ 
//------------------------------------------------------------------------------
/*! \fn KCluster
* \brief   constructtor
*/
KCluster::KCluster()
{
}

/*! \fn ~KCluster
* \brief  destructor
*/
KCluster::~KCluster()
{
}
//------------------------------------------------------------------------------

/* Clustering. */ 
//------------------------------------------------------------------------------
void KCluster::clustering(int spaceRange, double timeRange, 
            int inTDCNHits, vector<int> inTDCCh, vector<double> inTDCTS,
            int *outTDCNHits, vector<int> *outTDCCh, vector<double> *outTDCTS,
            vector<int> *outCluster)
{
  outCluster->clear();
  *outTDCNHits = 0;
  outTDCCh->clear();
  outTDCTS->clear();
  int tempCh = 0;
  double tempTime = 0;
  bool exit = false;
  while(!exit) {
   exit = true;
   for(int i = 0; i < inTDCNHits - 1; i++)
     if(inTDCCh.at(i) > inTDCCh.at(i+1)) {
      tempCh = inTDCCh.at(i);
      inTDCCh.at(i) = inTDCCh.at(i+1);
      inTDCCh.at(i+1) = tempCh;
      tempTime = inTDCTS.at(i);
      inTDCTS.at(i) = inTDCTS.at(i+1);
      inTDCTS.at(i+1) = tempCh;
      exit = false;
     }
  }
  int hBegin = 0;
  bool cluster[inTDCNHits];
  for(int h = 0; h < inTDCNHits; h++)
    cluster[h] = true;
  inTDCCh.push_back(std::numeric_limits<int>::max());
  inTDCTS.push_back(0);
  
  int TDCNHits = 0;
  vector<int> TDCCh; TDCCh.clear();
  vector<double> TDCTS; TDCTS.clear();
  vector<int> index; index.clear();
  exit = false;
  while(!exit) {
    exit = true;
    for(int h = hBegin; (h < inTDCNHits) && cluster[h]; h++) {
      int hIndex = h+1;
      for(int hh = hIndex; hh < inTDCNHits; hh++) {
        if(cluster[hh] == true) {
          hIndex = hh;
          break;
        }
      }
      if(inTDCCh.at(h) + spaceRange < inTDCCh.at(hIndex)) {
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
              if(timeRange > abs(maxTDCTS - minTDCTS)) {
                outTDCCh->push_back(meanTDCCh);
                outTDCTS->push_back(minTDCTS);
                *outTDCNHits += 1;
                outCluster->push_back(TDCNHits);
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
              outTDCCh->push_back(inTDCCh.at(index[0]));
              outTDCTS->push_back(inTDCTS.at(index[0]));
              *outTDCNHits += 1;
              outCluster->push_back(1);
              exit = true;
            }
          }
        }
        else {
          cluster[h] = false;
          outTDCCh->push_back(inTDCCh.at(h));
          outTDCTS->push_back(inTDCTS.at(h));
          *outTDCNHits += 1;
          outCluster->push_back(1);
        }
      }
      else {
        cluster[h] = false;
        TDCNHits += 1;
        TDCCh.push_back(inTDCCh.at(h));
        TDCTS.push_back(inTDCTS.at(h));
        index.push_back(h);
      }
    }
    for(int h = 0; h < inTDCNHits; h++) {
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
//------------------------------------------------------------------------------
