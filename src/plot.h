#ifndef PLOT_H
#define PLOT_H

#include"TLegend.h"
#include"TCanvas.h"
#include"TH2F.h"
#include"TAxis.h"
#include"TStyle.h"
#include"TCanvas.h"
#include"TFrame.h"
#include"TFile.h"
#include"TGraph.h"
#include"THistPainter.h"
#include"colgraph.h"
#include"lhcograph.h"

using namespace std;

class Plot : public virtual Def {
 public:
  //Default constructor
  Plot();

  //Constructor takes as input an initialization stream, with format
  //detailed in function definition.
  Plot(istream &in);

  //Load default plot settings
  virtual void LoadDefaults();

 protected:
  virtual int Define(stringstream &buff);

 public:

  //Print the current plot settings
  virtual void PrintSettings(ostream &out);

  //Add a new graph object.  Called from main program
  void NewGraph(istream &in, string type);

  //Draw canvas and all contained graphs
  virtual int Draw();

  //Save the plot to file.  Save appends to a .root file, Write
  //appends to a different file type
  void Save();
  void Write(string &file);

  static int plotVerbosity;  //Verbosity level of plot class
  static int plotNumber;     //Naming helper variable

 protected:
  //Generate a legend based on current settings.  Called from SetupCanvas()
  int MakeLegend(stringstream &inputLegend);

  TCanvas* Canvas;
  TLegend* Legend;
  vector<Def*> Graphs;
  double xMin, xMax, yMin, yMax, zMin, zMax;
  double topMargin, botMargin, leftMargin, rightMargin;
  int canvasWidth, canvasHeight, xticks, yticks;
  string xname, yname, logX, logY, logZ;
};

#endif
