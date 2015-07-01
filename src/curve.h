#ifndef CURVE_H
#define CURVE_H

#include"TGraph.h"
#include"graph.h"

using namespace std;

class Curve : public virtual Graph {
 public:
  Curve();
  Curve(istream &in);

  virtual void LoadDefaults();  //New default values for curves

  virtual int Draw();   //Draw curve

 protected:
  //Function to fill the Points object with data
  //Non-virtualized -- uses the function GrabNextLine to get stored values
  int GenerateCurve();

  TGraph *Points;      //TGraph used to contain the plot
};

#endif
