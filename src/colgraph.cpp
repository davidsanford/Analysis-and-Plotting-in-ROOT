#include"colgraph.h"

ColCurve::ColCurve() {}

ColCurve::ColCurve(istream &in) {
  Initialize(in);
}

void ColCurve::LoadDefaults() {
  Curve::LoadDefaults();
  ParseCol::LoadDefaults();
}

int ColCurve::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(ParseCol::Define(copy)) return 1;
  stringstream copy2(inputLine.str());
  if(Graph::Define(copy2)) return 1;
  
  string block;
  inputLine >> block;

  
  //else {
    if(graphVerbosity > 0)
      cout << "ColCurve: Invalid option: " << block << endl;
    return 0;
    //}

  return 1;
}

void ColCurve::PrintSettings(ostream &out) {
  ParseCol::PrintSettings(out);
  Graph::PrintSettings(out);
}

///////////////////////////////////////////////////////////////////////////////

ColHist::ColHist() {}

ColHist::ColHist(istream &in) {
  Initialize(in);
}

void ColHist::LoadDefaults() {
  Histogram::LoadDefaults();
  ParseCol::LoadDefaults();
}

int ColHist::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(ParseCol::Define(copy)) return 1;
  stringstream copy2(inputLine.str());
  if(Graph::Define(copy2)) return 1;
  
  string block;
  inputLine >> block;

  
  //else {
    if(graphVerbosity > 0)
      cout << "ColHist: Invalid option: " << block << endl;
    return 0;
    //}

  return 1;
}

void ColHist::PrintSettings(ostream &out) {
  ParseCol::PrintSettings(out);
  Graph::PrintSettings(out);
}

///////////////////////////////////////////////////////////////////////////////

ColCont::ColCont() {}

ColCont::ColCont(istream &in) {
  Initialize(in);
}

void ColCont::LoadDefaults() {
  Contour::LoadDefaults();
  ParseCol::LoadDefaults();
}

int ColCont::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(ParseCol::Define(copy)) return 1;
  stringstream copy2(inputLine.str());
  if(Graph::Define(copy2)) return 1;
  
  string block;
  inputLine >> block;

  
  //else {
    if(graphVerbosity > 0)
      cout << "ColCont: Invalid option: " << block << endl;
    return 0;
    //}

  return 1;
}

void ColCont::PrintSettings(ostream &out) {
  ParseCol::PrintSettings(out);
  Graph::PrintSettings(out);
}
