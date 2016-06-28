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
  char plotName[128] = "default";

  strcpy(outputFileName, argv[1]);
  strcpy(inputTextFile, argv[2]);
  if(argc == 4)
    strcpy(plotName, argv[3]);
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
    return 1;
  numParam = configure.getNumParam(inputTextFile);
  param = new double[numParam];
  nameParam = new char*[numParam];
  for(int i = 0; i < numParam; i++)
    nameParam[i] = new char[128];
  configure.getParam(inputTextFile, param, nameParam);
  /* END. */  
  
  /* get DAQ files for read */
  /* BEGIN: */
  int daqNumInFiles = 0;
  char **daqInputFileNames;
    daqNumInFiles = configure.getDaqNumFiles(inputTextFile);
  if(daqNumInFiles == 0)
    return 1;
  daqInputFileNames = new char*[daqNumInFiles];
  for(int i = 0; i < daqNumInFiles; i++)
    daqInputFileNames[i] = new char[256];
  configure.getDaqNamesFiles(inputTextFile, daqInputFileNames, daqNumInFiles);
  /* END. */  
  
  /* get CAEN files for read */
  /* BEGIN: */
  int caenNumInFiles = 0;
  char **caenInputFileNames;
    caenNumInFiles = configure.getCaenNumFiles(inputTextFile);
  if(caenNumInFiles == 0)
    return 1;
  caenInputFileNames = new char*[caenNumInFiles];
  for(int i = 0; i < caenNumInFiles; i++)
    caenInputFileNames[i] = new char[256];
  configure.getCaenNamesFiles(inputTextFile, caenInputFileNames, caenNumInFiles);
  /* END. */  
 
  if(daqNumInFiles != caenNumInFiles) {
    cout << "ERROR: Check files." << endl;
    return 1; 
  }
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
  int numChMap = lastCh - firstCh - numChMask + 1;
  MAP map[numChMap];
  configure.getMap(inputTextFile, map);

  cout <<"# INFORMATION ABOUT RUN"                                       << endl;
  cout <<"#------------------------------------------------------------" << endl;
  cout <<"#RUN TYPE: " << nameType << endl;
  for(int i = 0; i < numParam; i++)
    cout << nameParam[i] << "=" << param[i] << endl;
  cout <<"#DAQ INPUT FILES:"                                                << endl;
  for(int i = 0; i < daqNumInFiles; i++)
    cout << "inF[" << i << "]=" << daqInputFileNames[i] << endl;
  cout <<"#CAEN INPUT FILES:"                                                << endl;
  for(int i = 0; i < caenNumInFiles; i++)
    cout << "inF[" << i << "]=" << caenInputFileNames[i] << endl;
  cout <<"#MASK:"                                                       << endl;
  cout << "-firstCh=" << firstCh << endl;
  cout << "-lastCh=" << lastCh << endl;
  for(int i = 0; i < numChMask; i++)
    cout << "-ch[" << i << "]=" << mask[i] << endl;
  cout <<"#MAP:"                                                       << endl;
  for(int i = 0; i < numChMap; i++)
    cout << "-TDC=" << map[i].TDC << " " << "-NEW=" << map[i].real << endl;
  cout <<"#OUTPUT FILES:"                                                << endl;
  cout << "outF=" << outputFileName << endl;
  cout <<"#------------------------------------------------------------" << endl;

    Analysis analysis;

    analysis.setDaqInputFileNames(daqInputFileNames, daqNumInFiles);
    analysis.setCaenInputFileNames(caenInputFileNames, caenNumInFiles);
    analysis.setPlotName(plotName);
    analysis.setMask(firstCh, lastCh, mask, numChMask);
    analysis.setMap(map, numChMap);
    analysis.setParam(param, numParam);
    
    analysis.setOutputFile(outputFileName, outputTreeName);

    int isLoop = analysis.loop(nameType);
    if(isLoop == 1)
      cout << "The End." << endl;
    if(isLoop == -1) { 
      cout << "ERROR: Can't read file." << endl;
      cout << endl;
      cout << "The End." << endl;
    }

  return 1;
}
