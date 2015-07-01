#include"lhcograph.h"

LHCOCurve::LHCOCurve() {}

LHCOCurve::LHCOCurve(istream &in) {
  Initialize(in);
}

void LHCOCurve::LoadDefaults() {
  Curve::LoadDefaults();
  ParseLHCO::LoadDefaults();
}

int LHCOCurve::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(ParseLHCO::Define(copy)) return 1;
  stringstream copy2(inputLine.str());
  if(Graph::Define(copy2)) return 1;
  
  string block;
  inputLine >> block;

  
  //else {
    if(graphVerbosity > 0)
      cout << "LHCOCurve: Invalid option: " << block << endl;
    return 0;
    //}

  return 1;
}

void LHCOCurve::PrintSettings(ostream &out) {
  ParseLHCO::PrintSettings(out);
  Graph::PrintSettings(out);
}

///////////////////////////////////////////////////////////////////////////////

LHCOHist::LHCOHist() {}

LHCOHist::LHCOHist(istream &in) {
  Initialize(in);
}

void LHCOHist::LoadDefaults() {
  Histogram::LoadDefaults();
  ParseLHCO::LoadDefaults();
}

int LHCOHist::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(ParseLHCO::Define(copy)) return 1;
  stringstream copy2(inputLine.str());
  if(Graph::Define(copy2)) return 1;
  
  string block;
  inputLine >> block;

  
  //else {
    if(graphVerbosity > 0)
      cout << "LHCOHist: Invalid option: " << block << endl;
    return 0;
    //}

  return 1;
}

void LHCOHist::PrintSettings(ostream &out) {
  ParseLHCO::PrintSettings(out);
  Graph::PrintSettings(out);
}

///////////////////////////////////////////////////////////////////////////////

LHCOCont::LHCOCont() {}

LHCOCont::LHCOCont(istream &in) {
  Initialize(in);
}

void LHCOCont::LoadDefaults() {
  Contour::LoadDefaults();
  ParseLHCO::LoadDefaults();
}

int LHCOCont::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(ParseLHCO::Define(copy)) return 1;
  stringstream copy2(inputLine.str());
  if(Graph::Define(copy2)) return 1;
  
  string block;
  inputLine >> block;

  
  //else {
    if(graphVerbosity > 0)
      cout << "LHCOCont: Invalid option: " << block << endl;
    return 0;
    //}

  return 1;
}

void LHCOCont::PrintSettings(ostream &out) {
  ParseLHCO::PrintSettings(out);
  Graph::PrintSettings(out);
}
