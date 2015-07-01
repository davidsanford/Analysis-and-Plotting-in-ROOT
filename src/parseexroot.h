#ifndef PARSELHCO_H
#define PARSELHCO_H

#include"parse.h"
#include"TChain.h"
#include"TClonesArray.h"
#include"ExRootAnalysis/ExRootTreeReader.h"
#include"ExRootAnalysis/ExRootClasses.h"

using namespace std;

class LHCOInfo {
 public:
  LHCOInfo();
  LHCOInfo(string);

  string GetOption() {return option;}
  int GetNumSubInputs() {return types.size();}
  string GetType(int subInput);
  int GetIndex(int subInput);

  void ChangeOption(string newOption) {option = newOption;}
  void AddSubInput(string newType, int newIndex);

 private:
  string option;
  vector<string> types;
  vector<int> indices;
};

/* ######################################################################

                              ParseLHCO Class

  Extension of Parse class to manage the parsing of an lhco .root
file.  Each initialization is associated with a single input file.

###################################################################### */

class ParseLHCO : public virtual Parse {
 public:
  //Default constructor
  ParseLHCO();

  //Constructor takes as input an initialization stream, with format
  //detailed in function definition.
  ParseLHCO(istream &in);

  virtual void LoadDefaults();  //List of default values
                                //Separated from constructor for
                                //inheritance purposes

  virtual void PrintSettings(ostream &out);  //Print out current settings

  virtual void OpenFile();  //Reset data vectors and open the currently
                            //named input file.

 protected:
  virtual int Define(stringstream &buff);

  //Read a line from the input file
  virtual int Read(vector<double> &readValues);

  int GrabValue(double &, const char*, string, int);

  //Ensure the given branch is non-null and has at least 'index' entries
  int CheckBranch(TClonesArray *Branch, int index);

  ifstream input;      //Input stream for file to be parsed

  long int numEntries, entryIndex;

  TChain *Chain;
  ExRootTreeReader *Reader;        //Reader object
  /*  vector<string> Options;          //Set of input options to use
  vector<vector<string>*> Types;   //Track types
  vector<vector<int>*> Indices;    //Input indices (jet #1, photon #2, etc.)
  vector<vector<TClonesArray*>*> Branches;  //Branches to use*/

  //Pointers to the appropriate particle branch types
  TClonesArray *JetBranch, *METBranch, *ElecBranch, *MuonBranch,
    *TauBranch, *PhotonBranch;

  //Pointers to the data selection options
  vector<LHCOInfo*> LHCOInputs;

  //Selector for detector simulation type
  string simType;
};

#endif
