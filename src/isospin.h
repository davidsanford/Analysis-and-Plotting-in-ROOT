#ifndef ISOSPIN_H
#define ISOSPIN_H

#include<fstream>
#include<string>
#include<vector>
#include<iostream>

//Class to create a limit condition from a data file
//The file must have two columns, corresponding to input variable  and
//output variable respectively.
using namespace std;

class IsospinFactor {
public:
  //Constructor taking in the limit file's filename
  IsospinFactor(string &input);
  IsospinFactor(IsospinFactor &other);

  //Get the output value for a given input value
  double RescaleFactor(double mass, double ratio);

  //Get the rescaling factor for neutrons
  double NeutronRescaleFactor(double mass, double ratio);

private:
  //Arrays containing input and output data points
  double mp, mn;
  int Z;
  vector<double> A;
  vector<double> abundance;
};

#endif
