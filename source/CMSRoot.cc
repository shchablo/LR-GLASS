#include "CMSRoot.hh"

/* Constructor, destructor. */ 
//------------------------------------------------------------------------------
/*! \fn CMSRoot
* \brief   constructtor
*/
CMSRoot::CMSRoot()
{
    isOutFile_ = false;
}

/*! \fn ~CMSRoot
* \brief  destructor
*/
CMSRoot::~CMSRoot()
{
  if(isOutFile_) {
    outTree_->Delete();
    outFile_->Close();
    outFile_->Delete();
  }
}
//------------------------------------------------------------------------------

/* Set functions. */ 
//------------------------------------------------------------------------------
/*! \fn setOutputFile
* \brief Function for set output file.
*
* \param  char* outputFileName - path to the file and name of file
* \param  char* outputTreeName - name of tree
*
* \return bool - check tag
*/
bool CMSRoot::setOutputFile(char *outputFileName, char *outputTreeName)
{
  outFile_ = new TFile(outputFileName, "UPDATE");
  if(!outFile_)
    return false;
  outTree_ = new TTree(outputTreeName, outputTreeName);
  if(!outTree_)
    return false;
  isOutFile_ = true;
  return true;
}

/*! \fn setBranch
* \brief Function for ser output branch for tree.
*
* \param  double *data - pointer of variable of data
* \param  string branchName - neame of branch
* \return bool - check tag
*/
bool CMSRoot::setBranch(double *data, string branchName)
{
  if(!isOutFile_)
    return false;
  outTree_->Branch(branchName.c_str(), &data, branchName.c_str());
  return true;
}
//------------------------------------------------------------------------------

/* Functions */ 
//------------------------------------------------------------------------------
/*! \fn writeObject
* \brief Function for write TGraph in folder.
*
* \param  string dirName - name of folder
* \param  TObject *object
*
* \return bool - check tag
*/
bool CMSRoot::writeObject(string dirName, TObject *object)
{
  if(!outFile_->GetDirectory(dirName.c_str())) {
    outFile_->mkdir(dirName.c_str());
    outFile_->cd(dirName.c_str());
  }
  else
    outFile_->cd(dirName.c_str());

    if(outFile_->FindObject(object->GetName()))
     return false;

  object->Write();
  return true;
}
//------------------------------------------------------------------------------
