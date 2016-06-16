//-------------------------------------------------------
// Description: main file for analysis LR-GLASS test beam
// Authors:  Shchablo, Shchablo@gmail.com
//-------------------------------------------------------

// C++ includes
#include <iostream>
#include <fstream>
#include <stdio.h>

// Analysis class
#include "Analysis.hh"

// Configure class
#include "Configure.hh"

using namespace std;

int main(int argc, char* argv[]) 
{
  cout <<"#------------------------------------------------------------" << endl;
  cout <<"#------------------------------------------------------------" << endl;
  cout <<"#       LR-GLASS, Life is Endless Analysis."                   << endl;
  cout <<"#------------------------------------------------------------" << endl;
  if (argc < 3) {
    cout <<"#------------------------------------------------------------" << endl;
    cout <<"To run LR-GLASS:                                           " << endl;
    cout <<"Syntax: ./lrGlass outputFile.root card.txt dirName plotName" << endl;
    cout <<"Syntax for cards files: Use example cards." << endl;

    return 0;
  }
  char outputFileName[128] = "none";
  char outputTreeName[128] = "default";
  char inputTextFile[128] = "default.txt";
  char dirName[128] = "";
  char plotName[128] = "";

  strcpy(outputFileName, argv[1]);
  strcpy(inputTextFile, argv[2]);
  if(argc == 4)
    strcpy(dirName, argv[3]);
  if(argc == 5) {
    strcpy(dirName, argv[3]);
    strcpy(plotName, argv[4]);
  }
  Configure configure;

  /* get type and parameters for type */
  /* BEGIN: */
  int nType = 0;
  char nameType[128];
  int numParam = 0;
  double *param;
  char **nameParam;
  nType = configure.getType(inputTextFile, nameType);
  if(nType == 0)
    return 0;
  numParam = configure.getNumParam(inputTextFile);
  param = new double[numParam];
  nameParam = new char*[numParam];
  for(int i = 0; i < numParam; i++)
    nameParam[i] = new char[128];
  configure.getParam(inputTextFile, param, nameParam);
  /* END. */  
  
  /* get files for read */
  /* BEGIN: */
  int numInFiles = 0;
  char **inputFileNames;
    numInFiles = configure.getNumFiles(inputTextFile);
  if(numInFiles == 0)
    return 0;
  inputFileNames = new char*[numInFiles];
  for(int i = 0; i < numInFiles; i++)
    inputFileNames[i] = new char[256];
  configure.getNamesFiles(inputTextFile, inputFileNames, numInFiles);
  /* END. */  
  
  /* get files for read */
  /* BEGIN: */
  double voltage[numInFiles];
  double threshold[numInFiles];
  int isThrVol = configure.getThrVolt(inputTextFile, threshold, voltage, numInFiles);
  if(!isThrVol) {
    cout << "ERROR: Check numbers of files and numbers of voltage and threshold values. It shond be the same." << endl;
    return 1;
  }
  /* END. */  
  
  /* get mask */
  /* BEGIN: */
int numChMask = configure.getMaskNumParam(inputTextFile);
  if(numChMask < 0) {
    cout << "ERROR: Check Mask syntax." << endl;
    return 1;
  }
  int mask[numChMask];
  int firstCh = 0;
  int lastCh = 0;
  configure.getMask(inputTextFile, mask, &firstCh, &lastCh);
  /* END. */  
  MAP map[32];
  configure.getMap(inputTextFile, map);

  cout <<"# INFORMATION ABOUT RUN"                                       << endl;
  cout <<"#------------------------------------------------------------" << endl;
  cout <<"#RUN TYPE: " << nameType << endl;
  for(int i = 0; i < numParam; i++)
    cout << nameParam[i] << "=" << param[i] << endl;
  cout <<"#INPUT FILES:"                                                << endl;
  for(int i = 0; i < numInFiles; i++)
    cout << "inF[" << i << "]=" << inputFileNames[i] << endl;
  cout <<"#MASK:"                                                       << endl;
  cout << "-firstCh=" << firstCh << endl;
  cout << "-lastCh=" << lastCh << endl;
  for(int i = 0; i < numChMask; i++)
    cout << "-ch[" << i << "]=" << mask[i] << endl;
  cout <<"#OUTPUT FILES:"                                                << endl;
  cout << "outF=" << outputFileName << endl;
  cout <<"#------------------------------------------------------------" << endl;

    Analysis analysis;
    analysis.setThreshold(threshold);
    analysis.setVoltage(voltage);
    analysis.setMask(firstCh, lastCh, mask, numChMask);
    analysis.setMap(map);
    analysis.setOutputFile(outputFileName, outputTreeName);
    int isLoop = analysis.loop(inputFileNames, dirName, plotName, numInFiles, nameType, param, numParam);
    if(isLoop == 1)
      cout << "The End." << endl;
    if(isLoop == -1) { 
      cout << "ERROR: Can't read file." << endl;
      cout << endl;
      cout << "The End." << endl;
    }

  return 1;
}
