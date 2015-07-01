#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include<fstream>
#include<iostream>
#include<sstream>
#include<math.h>
#include<string>
#include<vector>
#include"TGraph.h"
#include"TStyle.h"
#include"formula.h"
#include"parse.h"
#include"curve.h"
#include"TH1F.h"

using namespace std;

class Histogram : public virtual Graph {
 public:
  Histogram();
  Histogram(istream &in);

  virtual void LoadDefaults();  //New default values for curves

  virtual int Draw();   //Draw curve

  static int histNumber;

 protected:
  int GenerateHistogram();

  TH1F* Hist;  //Histogram
};

#endif
