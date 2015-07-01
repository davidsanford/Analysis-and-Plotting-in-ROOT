#ifndef COLGRAPH_H
#define COLGRAPH_H

#include"parsecol.h"
#include"curve.h"
#include"histogram.h"
#include"contour.h"

using namespace std;

class ColCurve : public virtual Curve, public virtual ParseCol {
 public:
  ColCurve();

  ColCurve(istream &in);

  virtual void LoadDefaults();

  virtual void PrintSettings(ostream &out);

 protected:
  virtual int Define(stringstream &buff);
};

///////////////////////////////////////////////////////////////////////////////

class ColHist : public virtual Histogram, public virtual ParseCol {
 public:
  ColHist();

  ColHist(istream &in);

  virtual void LoadDefaults();

  virtual void PrintSettings(ostream &out);

 protected:
  virtual int Define(stringstream &buff);
};

///////////////////////////////////////////////////////////////////////////////

class ColCont : public virtual Contour, public virtual ParseCol {
 public:
  ColCont();

  ColCont(istream &in);

  virtual void LoadDefaults();

  virtual void PrintSettings(ostream &out);

 protected:
  virtual int Define(stringstream &buff);
};

#endif
