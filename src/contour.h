#ifndef CONTOUR_H
#define CONTOUR_H

#include"graph.h"
#include"TH2F.h"

class Contour : public virtual Graph {
 public:
  Contour();

  Contour(istream &in);

  virtual void LoadDefaults();  //New default values for contours

  virtual int Draw();   //Draw curve

  static int contourNumber;

 protected:
  int GenerateContour();

  TH2F *Binning;
};

#endif
