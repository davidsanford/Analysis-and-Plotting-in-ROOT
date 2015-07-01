#include"curve.h"

Curve::Curve() : Points(NULL) {}

Curve::Curve(istream &in) : Points(NULL) {
  Initialize(in);
}

void Curve::LoadDefaults() {
  Graph::LoadDefaults();
  legendName = "none";
  close = "no";
  bound = "no";
  drawOptions = "C";
  linewidth = 3;   //3 pt width
  linestyle = 1;   //solid line
  linecolor = 1;   //black
  pointsize = 1;   //1 pt size
  pointstyle = 1;  //dot
  pointcolor = 1;  //black
  fillcolor = 0;   //white
  fillstyle = 1001; //solid fill
  xMin = -1e100;
  xMax = 1e100;
  yMin = -1e100;
  yMax = 1e100;
}

int Curve::Draw() {
  if(graphVerbosity > 0) cout << "Curve::Draw : Start" << endl;

  //Fill the Points object and exit if it fails
  if(!GenerateCurve()) return 0;

  Points -> Draw(drawOptions.c_str());

  if(graphVerbosity > 0) cout << "Curve::Draw : End" << endl;

  return 1;
}

int Curve::GenerateCurve() {
  //Exit if the stored data is inappropriate for curve generation
  if(storedValues[0] -> size() < 1) {
    cout << "Empty data vectors" << endl;
    return 0;
  }
  if(storedValues.size() < 2) { 
    cout << "Too few columns" << endl;
    return 0;
  }

  //Initialize the Points object as a TGraph using the stored variables
  if(Points != NULL) delete Points;
  Points = new TGraph();
  Points -> SetName(legendName.c_str());
  Points -> SetMarkerColor(pointcolor);
  Points -> SetMarkerStyle(pointstyle);
  Points -> SetMarkerSize(pointsize);
  Points -> SetLineColor(linecolor);
  Points -> SetLineStyle(linestyle);
  Points -> SetLineWidth(linewidth);
  Points -> SetFillColor(fillcolor);
  Points -> SetFillStyle(fillstyle);

  int i;
  for(i = 0; i < storedValues[0] -> size(); i++) {
    if((bound == "yes" || bound == "Yes") &&
       ((*storedValues[0])[i] < xMin || (*storedValues[0])[i] > xMax ||
	(*storedValues[1])[i] < yMin || (*storedValues[1])[i] > yMax))
      continue;

    Points -> SetPoint(i, (*storedValues[0])[i], (*storedValues[1])[i]);
  }
  if(close == "yes" || close == "Yes") {
    double x, y;
    Points -> GetPoint(0, x, y);
    Points -> SetPoint(i, x, y);
  }

  return 1;
}
