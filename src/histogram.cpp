#include"histogram.h"

Histogram::Histogram() : Hist(NULL) {}

Histogram::Histogram(istream &in) : Hist(NULL) {
  Initialize(in);
}

void Histogram::LoadDefaults() {
  Graph::LoadDefaults();
  legendName = "none";
  drawOptions = "";
  linewidth = 3;   //3 pt width
  linestyle = 1;   //solid line
  linecolor = 1;   //black
  pointsize = 1;   //1 pt size
  pointstyle = 1;  //dot
  pointcolor = 1;  //black
  fillcolor = 0;   //white
  fillstyle = 1001; //solid fill
  xBins = 100;
  xMin = -1e100;
  xMax = 1e100;
  yMin = -1e100;
  yMax = 1e100;
}

int Histogram::Draw() {
  if(graphVerbosity > 0) cout << "Histogram::Draw : Start" << endl;

  //Fill the Hist object and exit if it fails
  if(!this -> GenerateHistogram()) return 0;

  string options = drawOptions + "same";

  Hist -> Draw(options.c_str());

  if(graphVerbosity > 0) cout << "Histogram::Draw : End" << endl;

  return 1;
}

int Histogram::GenerateHistogram() {
  if(storedValues[0] -> size() < 1) {
    cout << "Empty data vectors" << endl;
    return 0;
  }

  stringstream histName;
  histName << "hist" << histNumber;
  histNumber++;

  if(Hist != NULL) delete Hist;
  Hist = new TH1F(histName.str().c_str(),  histName.str().c_str(),
		  xBins, xMin, xMax);
  Hist -> SetName(legendName.c_str());
  Hist -> SetMarkerColor(pointcolor);
  Hist -> SetMarkerStyle(pointstyle);
  Hist -> SetMarkerSize(pointsize);
  Hist -> SetLineColor(linecolor);
  Hist -> SetLineStyle(linestyle);
  Hist -> SetLineWidth(linewidth);
  Hist -> SetFillColor(fillcolor);
  Hist -> SetFillStyle(fillstyle);

  for(int i = 0; i < storedValues[0] -> size(); i++) {
    if(storedValues.size() < 2)
      Hist -> Fill((*storedValues[0])[i]);
    else
      Hist -> Fill((*storedValues[0])[i], (*storedValues[1])[i]);
  }

  return 1;
}
