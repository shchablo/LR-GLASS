#include "Configure.hh"

/*! \fn Configure
* \brief   constructtor
*/
Configure::Configure()
{
}

/*! \fn ~Configure
* \brief  destructor
*/
Configure::~Configure()
{
}

int Configure::getType(char* inputTextFile, char* nameType) 
{
  int numType = 0;
  bool read = false;
  string line;
  ifstream myfile (inputTextFile);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      if(strncmp(line.c_str(),"#TYPE END.", 10) == 0)
        read = false;
      if(read) {
        if(strncmp(line.c_str(),"-type=", 6) == 0)
          sscanf(line.c_str(), "-type=%s", nameType);
      }
      if(strncmp(line.c_str(), "#TYPE:", 6) == 0)
        read = true;
    }

    myfile.close();
  }
  else { 
    cout << "#ERROR: Unable to open card file" << endl;
    return 0;
  }
  return 1;
}

int Configure::getNumParam(char* inputTextFile) 
{
  int numInFiles = 0;
  bool read = false;
  string line;
  ifstream myfile (inputTextFile);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      if(strncmp(line.c_str(),"#TYPE END.", 10) == 0)
        read = false;
      if(read)
      numInFiles++;
      if(strncmp(line.c_str(), "#TYPE:", 6) == 0)
        read = true;
    }
    myfile.close();
  }
  else { 
    cout << "#ERROR: Unable to open card file" << endl;
    return 0;
  }
  return numInFiles-1;
}

void Configure::getParam(char* inputTextFile, double* param, char** nameParam) 
{
  int i = 0;
  bool read = false;
  string line;
  ifstream myfile (inputTextFile);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      if(strncmp(line.c_str(),"#TYPE END.", 10) == 0)
        read = false;
      if(read) {
        if(strncmp(line.c_str(),"-type=", 6) != 0) {
          char tmp[128];
          strcpy(tmp, line.c_str());
          strtok(tmp, "=");
          strcpy(nameParam[i], tmp);
          param[i] = atof(strtok(NULL, "="));
          i++;
        }
      }
      if(strncmp(line.c_str(), "#TYPE:", 6) == 0)
        read = true;
    }
    myfile.close();
  }
  else cout << "#ERROR: Unable to open card file" << endl;
}

int Configure::getNumFiles(char* inputTextFile) 
{
  int numInFiles = 0;
  bool read = false;
  string line;
  ifstream myfile (inputTextFile);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      if(strncmp(line.c_str(),"#FILES END.", 10) == 0)
        read = false;
      if(read)
      numInFiles++;
      if(strncmp(line.c_str(), "#FILES:", 7) == 0)
        read = true;
    }
    myfile.close();
  }
  else cout << "#ERROR: Unable to open card file or file has't pathes for data files" << endl;
  return numInFiles;
}

void Configure::getNamesFiles(char* inputTextFile, char** inputFileNames, int numInFiles) 
{
 int i = 0;
  bool read = false;
  string line;
  ifstream myfile (inputTextFile);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      if(strncmp(line.c_str(),"#FILES END.", 10) == 0)
        read = false;
      if(read) {
        if(i < numInFiles) {
          strcpy(inputFileNames[i], line.c_str());
          i++;
        }
      }
    if(strncmp(line.c_str(), "#FILES:", 7) == 0)
      read = true;
    }
    myfile.close();
  }
  else cout << "#ERROR: Unable to open card file" << endl;
}

int Configure::getThrVolt(char* inputTextFile, double* threshold, double* voltage, int numInFiles)
{
  int i = 0;
  char str[128];
  char *pch;
  char cVoltage[128];
  char cThreshold[128];
  bool read = false;
  string line;
  ifstream myfile (inputTextFile);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      if(numInFiles == i)
        return -1;
      if(strncmp(line.c_str(), "#THR-HV END.", 12) == 0)
        read = false;
        if(read) {
          strcpy(str, line.c_str());
          pch = strtok (str,"_");
          strcpy(cThreshold, pch);
          pch = strtok (NULL, "_");
          strcpy(cVoltage, pch);
          threshold[i] = atof(strtok(cThreshold,"mV"));
          voltage[i] = atof(strtok(cVoltage,"V"));
          i++;
        }
      if(strncmp(line.c_str(), "#THR-HV:", 8) == 0)
        read = true;
    }
    myfile.close();
  }
  else cout << "#ERROR: Unable to open card file." << endl;
  if(numInFiles != i)
    return 0;
  return 1;
}
  
int Configure::getMaskNumParam(char* inputTextFile)
{
  int numChMask = 0;
  bool read = false;
  string line;
  ifstream myfile (inputTextFile);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      if(strncmp(line.c_str(),"#MASK END.", 10) == 0)
        read = false;
      if(read)
      numChMask++;
      if(strncmp(line.c_str(), "#MASK:", 6) == 0)
        read = true;
    }
    myfile.close();
  }
  else { 
    cout << "#ERROR: Unable to open card file" << endl;
    return 0;
  }
  return numChMask-2;

}

void Configure::getMask(char* inputTextFile, int* mask, int *firstCh, int *lastCh)
{
 int i = 0;
  bool read = false;
  string line;
  ifstream myfile (inputTextFile);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      if(strncmp(line.c_str(),"#MASK END.", 9) == 0)
        read = false;
      if(read) {
        char tmp[128];
        strcpy(tmp, line.c_str());
        strtok(tmp, "=");
          if(i == 0)
            *firstCh = atoi(strtok(NULL, "="));
          if(i == 1)
            *lastCh = atoi(strtok(NULL, "="));
          if(i >= 2)
            mask[i-2] = atoi(strtok(NULL, "="));
        i++;
      }
    if(strncmp(line.c_str(), "#MASK:", 6) == 0)
      read = true;
    }
    myfile.close();
  }
  else cout << "#ERROR: Unable to open card file" << endl;
}
