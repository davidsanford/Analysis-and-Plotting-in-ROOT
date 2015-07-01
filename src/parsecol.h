#ifndef PARSECOL_H
#define PARSECOL_H

#include"parse.h"

/* ######################################################################

                              ParseCol Class

  Extension of Parse class to manage the parsing of a file with data
in columns which are space- or tab-delimited.  Each initialization is
associated with a single input file, which is expected to be formatted
in columns of values with the possibility of a header.

###################################################################### */

using namespace std;

class ParseCol : public virtual Parse {
 public:
  //Default constructor
  ParseCol();

  //Constructor takes as input an initialization stream, with format
  //detailed in function definition.
  ParseCol(istream &in);

  //Copy constructor.  Does not copy stored data; performs a reset
  //based on the given input.
  ParseCol(ParseCol &other);

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

  int nCol;            //Number of columns
  int skip;            //Number of header lines to skip when parsing
};

#endif
