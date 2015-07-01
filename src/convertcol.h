#ifndef CONVERTCOL_H
#define CONVERTCOL_H

#include"parsecol.h"

/* ######################################################################

                              ConvertCol Class

  Extension of Parse class to manage the parsing of a file with data
in columns which are space- or tab-delimited.  Each initialization is
associated with a single input file, which is expected to be formatted
in columns of values with the possibility of a header.

###################################################################### */

using namespace std;

class ConvertCol : public virtual ParseCol {
 public:
  //Default constructor
  ConvertCol();

  //Constructor takes as input an initialization stream, with format
  //detailed in function definition.
  ConvertCol(istream &in);

  //Copy constructor.  Does not copy stored data; performs a reset
  //based on the given input.
  ConvertCol(ConvertCol &other);

  virtual void LoadDefaults();  //List of default values
                                //Separated from constructor for
                                //inheritance purposes

  virtual void PrintSettings(ostream &out);  //Print out current settings

  //virtual void OpenFile();  //Reset data vectors and open the currently
                            //named input file.

  virtual void PrintOutput();

  virtual int PrintHeader(ostream &, int startCol);

 protected:
  virtual int Define(stringstream &buff);

  //Read a line from the input file
  //virtual int Read(vector<double> &readValues);

  string outFile;
};

#endif
