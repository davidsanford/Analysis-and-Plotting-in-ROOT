#ifndef LHCOGRAPH_H
#define LHCOGRAPH_H

#include"parselhco.h"
#include"curve.h"
#include"histogram.h"
#include"contour.h"

using namespace std;

class LHCOCurve : public virtual Curve, public virtual ParseLHCO {
 public:
  LHCOCurve();

  LHCOCurve(istream &in);

  virtual void LoadDefaults();

  virtual void PrintSettings(ostream &out);

 protected:
  virtual int Define(stringstream &buff);
};

///////////////////////////////////////////////////////////////////////////////

class LHCOHist : public virtual Histogram, public virtual ParseLHCO {
 public:
  LHCOHist();

  LHCOHist(istream &in);

  virtual void LoadDefaults();

  virtual void PrintSettings(ostream &out);

 protected:
  virtual int Define(stringstream &buff);
};

///////////////////////////////////////////////////////////////////////////////

class LHCOCont : public virtual Contour, public virtual ParseLHCO {
 public:
  LHCOCont();

  LHCOCont(istream &in);

  virtual void LoadDefaults();

  virtual void PrintSettings(ostream &out);

 protected:
  virtual int Define(stringstream &buff);
};

#endif
