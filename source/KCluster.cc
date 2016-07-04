#include "KCluster.hh"

#include <limits>
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
     if(inTDCCh[i] > inTDCCh[i+1]) {
      tempCh = inTDCCh[i];
      inTDCCh[i] = inTDCCh[i+1];
      inTDCCh[i+1] = tempCh;
      tempTime = inTDCTS[i];
      inTDCTS[i] = inTDCTS[i+1];
      inTDCTS[i+1] = tempTime;
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
    for(int h = 0; (h < inTDCNHits); h++) {
      int hIndex = h;
      for(int hh = h; hh < inTDCNHits; hh++) {
        if(cluster[hh] == true) {
          hIndex = hh;
          break;
        }
      }
      if(inTDCCh.at(hIndex) + spaceRange < inTDCCh.at(hIndex+1)) {
        cluster[hIndex] = false;
        TDCNHits += 1;
        TDCCh.push_back(inTDCCh.at(hIndex));
        TDCTS.push_back(inTDCTS.at(hIndex));
        index.push_back(hIndex);
        if(TDCNHits != 0) {
          bool exit2 = false;
          while(!exit2) {
            double minTDCTS = 10000;
            double maxTDCTS = -10000;
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
              if(timeRange >= fabs(maxTDCTS - minTDCTS)) {
                outTDCCh->push_back(meanTDCCh);
                outTDCTS->push_back(meanTDCTS);
                *outTDCNHits += 1;
                outCluster->push_back(TDCNHits);
                exit2 = true;
              }
              else {
                if(fabs(maxTDCTS - meanTDCCh) > fabs(meanTDCCh - minTDCTS)) {
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
              outTDCCh->push_back(inTDCCh.at(0));
              outTDCTS->push_back(inTDCTS.at(0));
              *outTDCNHits += 1;
              outCluster->push_back(1);
              exit2 = true;
            }
          }
        }
        else {
          cluster[hIndex] = false;
          outTDCCh->push_back(inTDCCh.at(hIndex));
          outTDCTS->push_back(inTDCTS.at(hIndex));
          *outTDCNHits += 1;
          outCluster->push_back(1);
        }
      }
      else {
        cluster[hIndex] = false;
        TDCNHits += 1;
        TDCCh.push_back(inTDCCh.at(hIndex));
        TDCTS.push_back(inTDCTS.at(hIndex));
        index.push_back(hIndex);
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
