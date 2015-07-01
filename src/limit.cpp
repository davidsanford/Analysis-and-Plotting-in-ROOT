#include"limit.h"

//Reads in the contents of the limit file
//The first input should be a scaling factor for output values, which
//is discarded, and the rest columns of (x, y) values.  The values
//must be sorted in ascending order in x values for sensible output.
Limit::Limit(string &filename) : valIn(0), valOut(0) {
  ifstream fin(filename.c_str());

  double x, y;

  fin >> x;
  fin >> x >> y;

  while(!fin.eof()) {
    valIn.push_back(x);
    valOut.push_back(y);
    fin >> x >> y;
  }
}

//Copy another Limit object
Limit::Limit(Limit &other) {
  valIn = other.valIn;
  valOut = other.valOut;
}

//Find the index for the largest value less than x in the array
//Returns -1 for a value of x outside of the array.
int Limit::FindIndex(double x) {
  if(x < valIn[0] || x > valIn.back()) return -1;

  for(int i = 1; i < valIn.size(); i++) {
    if(x < valIn[i]) return i - 1;
  }

  //This code should not execute for a properly sorted input file
  cout << "Something is very wrong with limit input file" << endl;
  return -1;
}

//Linearly interpolates between the points bounding x to determine the
//corresponding output value.  Returns 0 if the point is outside of
//the limit range of the file.
double Limit::Evaluate(double x) {
  int index = FindIndex(x);
  if(index != -1) {
    return valOut[index] + (valOut[index + 1] - valOut[index])
      * (x - valIn[index]) / (valIn[index + 1] - valIn[index]);
  }
  else return 0;
}
