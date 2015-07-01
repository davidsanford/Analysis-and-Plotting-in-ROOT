#ifndef DEF_H
#define DEF_H

#include<fstream>
#include<iostream>
#include<iomanip>
#include<sstream>
#include<math.h>
#include<string>
#include<vector>
#include<sys/stat.h>

/* ######################################################################

                               Def Class

  This is a base-level class for reading in object definition
parameters.  It reads in the row from an parameter file and assigns
appropriate variables within a derived class.

###################################################################### */

using namespace std;

class Def {
 public:
  //Default constructor
  Def();

  //Constructor takes as input an initialization stream, with format
  //detailed in function definition.
  Def(istream &in);

  //Copy constructor.  Does not copy stored values; performs a reset
  //based on the given input.  Stub function.
  Def(Def &other);

  void Initialize(istream &in);  //Initialize function used for virtualization

  //List of default values, separated from constructor for inheritance
  //purposes.  Stub virtual function.
  virtual void LoadDefaults() {}

  //Print out current settings.  Derived classes print setting from
  //all classes recursively.  Stub virtual function.
  virtual void PrintSettings(ostream &out) {}

  //Redefine variables in the class - returns 1 if variable was found
  //and 0 if now.  Handles definitions for constructor as well.
  virtual int Define(const char *buff);
  virtual int Define(const string buff);

  //Stub functions for inherited classes
  virtual int Legend(string &s) {return 0;};
  virtual int Draw() {return 0;};
  virtual void Fill() {};

 protected:
  //Common "true" define function with virtualization.  Stub virtual function
  virtual int Define(stringstream &buff) {return 1;};

  //Common vectors for input of data, included here for compatibility
  //of separate "branches" in class structure
  vector<vector<double>*> storedValues;
};



/* ######################################################################

                   Additional Universal Global Functions

  This is a base-level class for reading in object definition
parameters.  It reads in the row from an parameter file and assigns
appropriate variables within a derived class.

###################################################################### */

double sqr(double x);  //Simple squaring functions
int CheckFileExistence(string filename);

#endif
