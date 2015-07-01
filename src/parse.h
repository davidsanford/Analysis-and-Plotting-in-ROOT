#ifndef PARSE_H
#define PARSE_H

#include"def.h"
#include"formula.h"

/* ######################################################################

                                 Parse Class

  Stub class to manage the parsing of a file.  Each initialization is
associated with a single input file, with input values stored in
multi-column arrays.  Supports input with arbitrary formula
expressions defining the stored values and supporst sorting in
increasing or decreasing order for any column.

###################################################################### */

using namespace std;

//Parse class declaration
class Parse : public virtual Def {
 public:
  //Default constructor
  Parse();

  //Constructor takes as input an initialization stream
  Parse(istream &in);

  //Copy constructor.  Does not copy stored data; performs a reset
  //based on the given input.
  Parse(Parse &other);

  virtual void LoadDefaults();  //List of default values
                                //Separated from constructor for
                                //inheritance purposes

  virtual void PrintSettings(ostream &out);  //Print out current settings

  virtual void OpenFile() {} //Reset data vectors and open the currently
                             //named input file.  Stub virtual function

  //Print a header for aggregate file, starting with column startCol
  //Returns value of final column number + 1
  //Does not include line break
  virtual int PrintHeader(ostream &, int startCol);

 protected:
  virtual int Define(stringstream &buff);

 public:
  virtual void Fill();  //Open and read the input file
                        //Uses virtual member function Read to handle
                        //file-type dependent input

  //Get a single line of stored data
  int GetNumRows();
  int GetNumColumns() {return storedValues.size();}
  int GetStoredColumn(int index, vector<double> &data);
  int GetStoredRow(int index, vector<double> &data);

  //Control flag for verbosity of Parse class and derived classes output
  // 0 -- Minimal
  // 1 -- Debug
  // 2 -- All input data
  static int parseVerbosity;
  static int numOpen;

 protected:

  void Sort();     // Sort previously input data.  Called within Fill
  void Swap(int, int);  //Swap the data in all arrays for two given indices

  //Function to read a single line from the input file
  //Stub virtual function defined in derived classes
  virtual int Read(vector<double> &) {return 0;}
  
  //Function to convert a read line into stored data
  //Call equations and applies conditions
  int Convert(vector<double> &, vector<double> &);

  int readFlag;   //Flag to control the read state.  Zero skips/ends
                  //parsing.  Set to zero when end state reached
  int sortCol;    //Number of column to use for sorting (if any)

  //Control Strings
  string file;             //Name of input file
  string sorting;          //Sorting method

  ifstream input;      //Input stream for file to be parsed

  vector<Formula*> inputFormulas;  //Expressions for input values

  vector<string> headerNames;       //Column header names for aggregate

  vector<Formula*> conditions;     //Conditionals applied to 
};

#endif
