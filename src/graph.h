#ifndef GRAPH_H
#define GRAPH_H

#include"def.h"

using namespace std;

/* ######################################################################

                                 Graph Class

  Stub class to contain common graph functions.  Includes stub virtual
functions for legend and plotting, as well as variable definitions.
Not all variables defined in Graph are used by all inherited classes.

###################################################################### */

class Graph : public virtual Def {
 public:
  //Default constructor
  Graph();

  //Constructor with initialization stream
  Graph(istream &in);

  virtual void LoadDefaults();  //Default values.  See def.h

  virtual void PrintSettings(ostream &out);  //Print settings.  See def.h

  //Stub function for drawing the graph on the canvas.  Defined in
  //inherited classes based on 
  //Return boolean expressions indicating success; defaults to zero
  //Call a different helper function GenerateXXXXXX in each inherited class
  virtual int Draw() {return 0;}

  //Return existence of legend entry directly, and legend entry string
  //by reference string.  Drawing of legend is handled by "plot" class.
  virtual int Legend(string &s);

  //Control flag for verbosity of Graph class and appropriate
  // behaviors of derived classes
  // 0 -- Minimal
  // 1 -- Debug
  static int graphVerbosity;

 protected:
  virtual int Define(stringstream &buff);

  // #################### Universal Options ####################
  double xMin, xMax, yMin, yMax;  //Boundary values in (x, y) plane
  double zMin, zMax;              //Z-axis boundaries for contour

  string legendName;       //Legend entry for this curve
  string drawOptions;      //Plotting options

  // #################### Line/Histogram Options ####################
  int linewidth, linecolor, linestyle;   //Control variables for curves
  int pointsize, pointcolor, pointstyle; //Control variables for points
  int fillcolor, fillstyle;              //Fill color and style

  // #################### Line Only Options ####################
  string close;            //Close curve (for plotting regions)
  string bound;            //Apply edge-of-plot boundaries

  // #################### Histogram Options ####################
  int xBins, yBins;
  vector<double> contourLevels;
};

#endif
