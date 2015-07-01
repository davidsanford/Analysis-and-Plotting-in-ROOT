#include"isospin.h"

//Reads the elemental abundances file
//Of the first eight inputs, the fourth is the 'Z' of the element and
//the sixth is the number of isotopes, and the remainder are
//discarded.  The remaining lines contain A in "x" and abundance in
//"y"
IsospinFactor::IsospinFactor(string &filename) : abundance(0), A(0),
						 mp(0.938272), mn(0.939565) {

  string trash;
  ifstream fin(filename.c_str());
  int numIso;

  fin >> trash >> trash >> trash;
  fin >> Z;
  fin >> trash >> numIso  >> trash >> trash;

  A.resize(numIso);
  abundance.resize(numIso);

  for(int i = 0; i < numIso; i++) {
    fin >> A[i] >> abundance[i];
  }
}

//Copy another IsospinFactor object
IsospinFactor::IsospinFactor(IsospinFactor &other) {
  A = other.A;
  abundance = other.abundance;
}

//Calculates the rescaling factor to scale per-nucleon cross-section
//into proton cross-section
double IsospinFactor::RescaleFactor(double mass, double ratio) {
  double numerator = 0, denominator = 0, mred;
  for(int i = 0; i < A.size(); i++) {
    mred = mass * (Z * mp + A[i] * mn) / (mass + Z * mp + A[i] * mn);
    numerator += abundance[i] * mred * mred * A[i] * A[i];
    denominator += abundance[i] * mred * mred
      * (Z + (A[i] - Z) * ratio) * (Z + (A[i] - Z) * ratio);
  }

  return numerator / denominator;
}

//Calculates the rescaling factor to scale per-nucleon cross-section
//into proton cross-section
/*double IsospinFactor::NeutronRescaleFactor(double mass, double ratio) {
  double numerator = 0, denominator = 0, mred;
  for(int i = 0; i < A.size(); i++) {
    mred = mass * (Z * mp + A[i] * mn) / (mass + Z * mp + A[i] * mn);
    numerator += abundance[i] * mred * mred * A[i] * A[i];
    denominator += abundance[i] * mred * mred
      * (Z + (A[i] - Z) * ratio) * (Z + (A[i] - Z) * ratio);
  }

  return numerator / denominator;
  }*/
