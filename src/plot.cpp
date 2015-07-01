#include"plot.h"

Plot::Plot(istream &in) : Canvas(NULL), Legend(NULL), Graphs(0) {
  if(plotVerbosity > 0) cout << "Plot::Plot : Start" << endl;

  Initialize(in);

  if(plotVerbosity > 0) cout << "Plot::Plot : End" << endl;
}

void Plot::LoadDefaults() {
  canvasWidth = 600;
  canvasHeight = 600;
  logX = "no";
  logY = "no";
  logZ = "no";
  xMin = 0;
  xMax = 1;
  yMin = 0;
  yMax = 1;
  zMin = 0;
  zMax = -1;   //z range left undefined if zMax < zMin
  topMargin = 0.03;
  botMargin = 0.13;
  leftMargin = 0.13;
  rightMargin = 0.03;
  xticks = 0;
  yticks = 0;
}


//Define a variable taken from the input stringstream
//The first input is the control sequence, subsequent inputs are
//parsed based on that sequence.  Excess inputs in a particular
//sequence are discarded. 
int Plot::Define(stringstream &inputLine) {
  char buff[1000];
  string block;

  inputLine >> block;

  if(plotVerbosity > 1) {
    cout << "Full Define stream: " << inputLine.str() << endl;
  }

  if(plotVerbosity > 0) {
    cout << "Plot: Assigning Block: " << block << endl;
  }

  if(block == "legend") {
    inputLine.getline(buff, 1000);
    stringstream legendConvert(buff);
    MakeLegend(legendConvert);
  }
  else if(block == "nolegend" && Legend != NULL) delete Legend;
  else if(block == "xname") {    
    inputLine.getline(buff, 1000);
    xname = buff;
  }
  else if(block == "yname") {    
    inputLine.getline(buff, 1000);
    yname = buff;
  }
  else if(block == "logX") inputLine >> logX;
  else if(block == "logY") inputLine >> logY;
  else if(block == "logZ") inputLine >> logZ;
  else if(block == "xrange") inputLine >> xMin >> xMax;
  else if(block == "yrange") inputLine >> yMin >> yMax;
  else if(block == "zrange") inputLine >> zMin >> zMax;
  else if(block == "topmargin") inputLine >> topMargin;
  else if(block == "botmargin") inputLine >> botMargin;
  else if(block == "leftmargin") inputLine >> leftMargin;
  else if(block == "rightmargin") inputLine >> rightMargin;
  else if(block == "canvaswidth") inputLine >> canvasWidth;
  else if(block == "canvasheight") inputLine >> canvasHeight;
  else if(block == "xticks") inputLine >> xticks;
  else if(block == "yticks") inputLine >> yticks;
  
  else {
    if(plotVerbosity > 0)
      cout << "Plot: Invalid option: " << block << endl;
    return 0;
  }

  return 1;
}

//Print current plot settings
void Plot::PrintSettings(ostream &out) {
  out << "Plot Settings" << endl
      << "Canvas Dimensions: " << endl
      << "Width: " << canvasWidth << ", Height: " << canvasHeight << endl
      << "X axis: " << endl
      << "Name: " << xname << endl;
  if(logX == "yes" || logX == "Yes") out << "log" << endl;
  else out << "linear" << endl;
  out << "x range: " << xMin << " to " << xMax << endl
      << "Y axis: " << endl
      << "Name: " << yname << endl;
  if(logY == "yes" || logY == "Yes") out << "log" << endl;
  else out << "linear" << endl;
  out << "y range: " << yMin << " to " << yMax << endl;
  if(logZ == "yes" || logZ == "Yes") out << "Logarithmic z axis" << endl;
  else out << "Linear z axis" << endl;
  out << "z range: " << zMin << " to " << zMax << endl
      << "Margins (%canvas size): " << endl
      << "Top: " << topMargin << ", Bottom: " << botMargin << endl
      << "Left: " << leftMargin << ", Right: " << rightMargin << endl
      << "Tick Options: x: " << xticks << ", y: " << yticks << endl;

}

//Function to insert a legend into the plot
//Takes in a string stream which should contain four double values
//corresponding to  xMin, xMax, yMin, yMax respectively.
//Each takes a value between 0 and 1 corresponding to spacing
//horizontally or vertically.
//Exits without creating a legend if less than four are present.
//Returns 1 if legend was successfully made and zero if not
int Plot::MakeLegend(stringstream &inputLegend) {
  if(plotVerbosity > 0) cout << "Plot::MakeLegend : Start" << endl;

  vector<double> legendDist(4);

  if(Legend != NULL) delete Legend;

  for(int i = 0; i < 4; i++) {
    if(inputLegend.eof()) return 0;
    inputLegend >> legendDist[i];
  }

  if(plotVerbosity > 0)
    cout << "Legend parameters: "
	 << "X: " << legendDist[0] << " to " << legendDist[1] << "   "
	 << "Y: " << legendDist[2] << " to " << legendDist[3] << endl;

  //Create the legend object
  Legend = new TLegend(legendDist[0], legendDist[2],
		       legendDist[1], legendDist[3]);

  //Define legend parameters
  //Currently unalterable --
  //Black lines, white background, 1 pixel linewidth, no shadow
  Legend -> SetFillColor(0);
  Legend -> SetLineColor(kBlack);
  Legend -> SetLineWidth(1);
  Legend -> SetShadowColor(0);
  Legend -> SetTextSize(0.04);
  Legend -> SetTextFont(42);
  Legend -> SetBorderSize(1);

  if(plotVerbosity > 0) cout << "Plot::MakeLegend : End" << endl;

  return 1;
}

//Define a new curve object
void Plot::NewGraph(istream &in, string type) {

  //Create a new graph based on the input type
  if(type == "CURVE") Graphs.push_back(new ColCurve(in));
  else if(type == "CONTOUR") Graphs.push_back(new ColCont(in));
  else if(type == "HISTOGRAM") Graphs.push_back(new ColHist(in));
  else if(type == "LHCOCURVE") Graphs.push_back(new LHCOCurve(in));
  else if(type == "LHCOHIST") Graphs.push_back(new LHCOHist(in));
  else if(type == "LHCOCONTOUR") Graphs.push_back(new LHCOCont(in));

  //Pass plot ranges to the graph object
  stringstream rangeDef;

  rangeDef << "xrange " << xMin << " " << xMax;
  Graphs.back() -> Define(rangeDef.str());
  rangeDef.str("");

  rangeDef << "yrange " << yMin << " " << yMax;
  Graphs.back() -> Define(rangeDef.str());
  rangeDef.str("");

  if(zMin < zMax) {
    rangeDef << "zrange " << zMin << " " << zMax;
    Graphs.back() -> Define(rangeDef.str());
    rangeDef.str("");
  }
}

int Plot::Draw() {
  //If a canvas already exists, delete it
  if(Canvas != NULL) delete Canvas;

  ostringstream naming;

  //Output plot settings in verbose mode
  if(plotVerbosity > 0) {
    PrintSettings(cout);
  }

  gStyle -> SetPadTopMargin(topMargin);
  gStyle -> SetPadBottomMargin(botMargin);
  gStyle -> SetPadLeftMargin(leftMargin);
  gStyle -> SetPadRightMargin(rightMargin);

  //Declare new canvas object
  //Define a dummy name for the plot for use by root
  //Uses numbering to avoid overlapping names
  naming << "Plot" << plotNumber;
  Canvas = new TCanvas(naming.str().c_str(), naming.str().c_str(),
		       0, 0, canvasWidth, canvasHeight);
  naming.str("");

  if(logX == "yes" || logX == "Yes") Canvas -> SetLogx();
  if(logY == "yes" || logY == "Yes") Canvas -> SetLogy();
  if(logZ == "yes" || logZ == "Yes") Canvas -> SetLogz();
  Canvas -> SetFillColor(0);
  Canvas -> GetFrame()->SetFillColor(0);
  Canvas -> GetFrame()->SetBorderSize(0);
  Canvas -> SetBorderMode(0);
  Canvas -> ToggleEventStatus(); //Do not show the event status bar
  Canvas -> SetTicks(xticks, yticks);

  Canvas -> cd();   //Create axes within the canvas

  //Define axes object, again with a dummy name
  naming << "axes" << plotNumber;
  TH2F* Axes = new TH2F(naming.str().c_str(), naming.str().c_str(), 100, xMin,
			xMax, 100, yMin, yMax);
  naming.str("");

  Axes -> Sumw2();

  Axes -> SetStats(kFALSE);

  Axes -> SetTitle("");  //No title

  //Set x axis parameters
  Axes -> GetXaxis() -> SetTitle(xname.c_str());
  Axes -> GetXaxis() -> CenterTitle(kTRUE);
  Axes -> GetXaxis() -> SetTitleSize(0.05);
  Axes -> GetXaxis() -> SetLabelSize(0.04);
  Axes -> GetXaxis() -> SetTitleFont(42);
  Axes -> GetXaxis() -> SetLabelFont(42);
  Axes -> GetXaxis() -> SetTitleOffset(1.1);
  Axes -> GetXaxis() -> SetMoreLogLabels();

  //Set y axis parameters
  Axes -> GetYaxis() -> SetTitle(yname.c_str());
  Axes -> GetYaxis() -> CenterTitle(kTRUE);
  Axes -> GetYaxis() -> SetTitleSize(0.05);
  Axes -> GetYaxis() -> SetLabelSize(0.04);
  Axes -> GetYaxis() -> SetTitleFont(42);
  Axes -> GetYaxis() -> SetLabelFont(42);
  Axes -> GetYaxis() -> SetTitleOffset(1.1);

  Axes -> Draw();   //Draw axes onto canvas

  //Axis limits are re-defined after being drawn
  Axes -> GetXaxis() -> SetRangeUser(xMin, xMax);
  Axes -> GetYaxis() -> SetRangeUser(yMin, yMax);
  if(zMax > zMin) Axes -> GetZaxis() -> SetRangeUser(zMin, zMax);

  //Iterate plot number for next plot
  plotNumber++;


  Canvas -> cd();

  for(int i = 0; i < Graphs.size(); i++)
    Graphs[i] -> Fill();

  //Generate the legend
  string legendEntry;

  for(int i = 0; i < Graphs.size(); i++) {
    Graphs[i] -> Draw();
    if(Legend != NULL && Graphs[i] -> Legend(legendEntry))
      Legend -> AddEntry(legendEntry.c_str(), legendEntry.c_str(), "l");
  }

  if(Legend != NULL) Legend -> Draw();

  return 1;
}

void Plot::Write(string &file) {
  Canvas -> SaveAs(file.c_str());
}

void Plot::Save() {
  Canvas -> Write();
}
