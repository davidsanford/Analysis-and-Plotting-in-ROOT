#include"contour.h"

Contour::Contour() : Binning(NULL) {}

Contour::Contour(istream &in) : Binning(NULL) {
  Initialize(in);
}

void Contour::LoadDefaults() {
  Graph::LoadDefaults();
  drawOptions = "col,z";
  xBins = 100;
  yBins = 100;
}

int Contour::Draw() {
  if(graphVerbosity > 0) cout << "Contour::Draw : Start" << endl;

  //Fill the Points object and exit if it fails
  if(!GenerateContour()) return 0;

  string options = drawOptions + "same";

  Binning -> Draw(options.c_str());

  if(graphVerbosity > 0) cout << "Contour::Draw : End" << endl;

  return 1;
}

int Contour::GenerateContour() {
  if(storedValues.size() < 2) { 
    cout << "Too few columns" << endl;
    return 0;
  }
  if(storedValues[0] -> size() < 1) {
    cout << "Empty data vectors" << endl;
    return 0;
  }

  stringstream contourName;
  contourName << "contour" << contourNumber;
  contourNumber++;

  if(Binning != NULL) delete Binning;
  Binning = new TH2F(contourName.str().c_str(), contourName.str().c_str(),
		     xBins, xMin, xMax, yBins, yMin, yMax);
  Binning -> SetName(legendName.c_str());

  if(zMin < zMax) {
    Binning -> GetZaxis() -> SetRangeUser(zMin, zMax);
  }

  if(contourLevels.size() > 0) {
    Binning -> SetContour(contourLevels.size());
    for(int i = 0; i < contourLevels.size(); i++) {
      Binning -> SetContourLevel(i, contourLevels[i]);
    }
  }

  for(int i = 0; i < storedValues[0] -> size(); i++) {
    if(storedValues.size() < 3)
      Binning -> Fill((*storedValues[0])[i], (*storedValues[1])[i]);
    else
      Binning -> Fill((*storedValues[0])[i], (*storedValues[1])[i],
		      (*storedValues[2])[i]);
  }

  return 1;
}
