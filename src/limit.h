#ifndef LIMIT_H
#define LIMIT_H

#include<fstream>
#include<string>
#include<vector>
#include<iostream>

//Class to create a limit condition from a data file
//The file must have two columns, corresponding to input variable  and
//output variable respectively.
using namespace std;

class Limit {
public:
  //Constructor taking in the limit file's filename
  Limit(string &input);
  Limit(Limit &other);

  //Get the output value for a given input value
  double Evaluate(double x);

private:
  //Get the array index associated with data value.  This corresponds
  //to the index of the greatest value in the array less than the input value
  int FindIndex(double m);

  //Interplolates linearly between two given limit points to produce
  //an output value
  double Interpolate(int index, double x);

  //Arrays containing input and output data points
  vector<double> valIn;
  vector<double> valOut;
};

#endif
