#ifndef PARSELHCO_H
#define PARSELHCO_H

#include"parse.h"
#include"event.h"
#include<stdio.h>
#include<stdlib.h>

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

  //Pointers to the data selection options
  vector<LHCOInfo*> LHCOInputs;
};

#endif
