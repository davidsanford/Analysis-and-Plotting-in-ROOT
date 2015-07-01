#ifndef AGGREGATE_H
#define AGGREGATE_H

#include"parsecol.h"
#include"parselhco.h"
#include"parseslha.h"

using namespace std;

class Aggregate : public virtual Def {
 public:
  //Default constructor
  Aggregate();

  //Constructor takes as input an initialization stream, with format
  //detailed in function definition.
  Aggregate(istream &in);

  //Load default aggregate settings
  virtual void LoadDefaults();

 protected:
  virtual int Define(stringstream &buff);

 public:

  //Print the current aggregate settings
  virtual void PrintSettings(ostream &out);

  //Add a new graph object.  Called from main program
  void NewSource(istream &in, string type);

  //Write aggregated values to file
  virtual int Write(ostream &out);

  virtual void PrintHeader(ostream &out);

  static int aggregateVerbosity;  //Verbosity level of aggregate class

 protected:
  string NextFile();        //Get next base file string
  int ContinueIteration();  //Check if iteration should continue

  vector<Parse*> Sources;
  vector<string> SourceTypes;

  vector<string> leadingAffix, followingAffix;
  vector<string> nameFragments;
  vector<int> low, high, iterate, current;

  int headerFlag;
};

#endif
