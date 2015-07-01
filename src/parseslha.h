#ifndef PARSESLHA_h
#define PARSESLHA_h

#include"parse.h"
#include<stdio.h>
#include<stdlib.h>

/* ######################################################################

                              ParseSLHA Class

  Extension of Parse class to manage the parsing of a .slha file.
Each initialization is associated with a single input file.

###################################################################### */


using namespace std;

class SLHAItem {
 public:
  SLHAItem();
  SLHAItem(string &, string &, long int);
  SLHAItem(string &, string &, long int, long int);

  int CheckSLHA() {return state;}
  string GetBlock() {return block;}
  string GetName() {return varname;}
  long int GetCode() {return code;}
  long int GetCode2() {return code2;}
  double GetVal() {return value;}

  void SetState(int s) {state = s;}
  //void SetBlock(string &b) {block = b;}
  //void SetCode(long int c) {code = c;}
  //void SetCode2(long int c2) {code2 = c2;}
  void SetVal(double val) {value = val;}

  SLHAItem & operator=(SLHAItem &);

 private:
  string block, varname;
  long int code, code2;
  int state;
  double value;
};

class GeneralItem {
 public:
  GeneralItem();
  GeneralItem(string type);
  GeneralItem(string type, int parent);

  void CheckRule(istream &in);
  double GetVal() {return value;}
  string GetRule() {return ruleType;}
  int GetParent() {return decayParent;}

 private:
  string ruleType;
  double value;
  int decayParent;
};

class CheckLine {
 public:
  CheckLine();
  CheckLine(string nameIn, string lineIn);

  void Compare(string lineIn);
  void Compare(const char *lineIn);
  void Reset();

  string GetName() {return name;}
  string GetLine() {return line;}
  int GetExistence() {return existence;}

 private:
  string name;
  string line;
  int existence;
};

class ParseSLHA : public virtual Parse {
 public:
  //Default constructor
  ParseSLHA();

  //Constructor takes as input an initialization stream, with format
  //detailed in function definition.
  ParseSLHA(istream &in);

  virtual void LoadDefaults();  //List of default values
                                //Separated from constructor for
                                //inheritance purposes

  virtual void PrintSettings(ostream &out);  //Print out current settings

  virtual void OpenFile();  //Reset data vectors and open the currently
                            //named input file.

  virtual int PrintHeader(ostream &, int startCol);  //Print SLHA Header

 protected:
  virtual int Define(stringstream &buff);

  //Read a line from the input file
  virtual int Read(vector<double> &readValues);

  int DoubleBlock(string);  //Returns 1 for blocks with 2 identifiers

  vector<SLHAItem*> slhaData;
  vector<GeneralItem*> generalData;
  vector<CheckLine*> lineData;

  int madOutput, correlatedInput, reZip;
};

#endif
