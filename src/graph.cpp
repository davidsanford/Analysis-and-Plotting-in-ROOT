#include"graph.h"

Graph::Graph() : contourLevels(0) {}

Graph::Graph(istream &in) : contourLevels(0) {
  Initialize(in);
}

void Graph::LoadDefaults() {
  legendName = "none";
  close = "";
  drawOptions = "";
  linewidth = -1;
  linestyle = -1;
  linecolor = -1;
  pointsize = -1;
  pointstyle = -1;
  pointcolor = -1;
  fillcolor = -1;
  fillstyle = 1001;
  xBins = -1;
  yBins = -1;
  xMin = -1e100;
  xMax = 1e100;
  yMin = -1e100;
  yMax = 1e100;
  zMin = 0;
  zMax = -1;  //No constraint applied if zMin > zMax
}

void Graph::PrintSettings(ostream &out) {
  out << "Graph Settings" << endl;
  out << "Draw options: " <<drawOptions << endl;
  if(legendName != "None" && legendName != "none")
    out << "Legend entry name: " << legendName << endl;
  if(linewidth > -1) out << "Line width: " << linewidth << endl;
  if(linestyle > -1) out << "Line style: " << linestyle << endl;
  if(linecolor > -1) out << "Line color: " << linecolor << endl;
  if(pointsize > -1) out << "Point size: " << pointsize << endl;
  if(pointstyle > -1) out << "Point style: " << pointstyle << endl;
  if(pointcolor > -1) out << "Point color: " << pointcolor << endl;
  if(fillcolor > -1) out << "Fill color: " << fillcolor << endl;
  if(fillstyle > -1) out << "Fill style: " << fillstyle << endl;
  if(close == "yes" || close == "Yes" || close == "no" || close == "No")
    out << "Close curve? " << close << endl;
  if(bound == "yes" || bound == "Yes" || bound == "no" || bound == "No")
    out << "Use canvas bounds? " << bound << endl;
  if(xBins > -1)
    out << "x bins: " << xBins << endl;
  if(xBins > -1)
    out <<"y bins: " << yBins << endl;
  out << "Canvas Bounds" << endl;
  out << xMin << " < x < " << xMax << endl;
  out << yMin << " < x < " << yMax << endl;
  if(zMin < zMax)
    out << zMin << " < z < " << zMax << endl;
  if(contourLevels.size() > 0) {
    out << "Contour levels: " << endl;
    for(int i = 0; i < contourLevels.size(); i++)
      out << contourLevels[i] << "   ";
    out << endl;
  };
}

int Graph::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());

  string block;
  inputLine >> block;

  if(graphVerbosity > 0) {
    cout << "Graph: Assigning Block: " << block << endl;
  }

  if(block == "legend_entry") {
    char buff[1000];
    inputLine.getline(buff, 1000);
    legendName = buff;
    //Remove whitespace
    while(legendName[0] == ' ') legendName.erase(0, 1);
  }
  else if(block == "draw_options") inputLine >> drawOptions;
  else if(block == "xrange") inputLine >> xMin >> xMax;
  else if(block == "yrange") inputLine >> yMin >> yMax;
  else if(block == "zrange") inputLine >> zMin >> zMax;
  else if(block == "line_style") inputLine >> linestyle;
  else if(block == "point_style") inputLine >> pointstyle;
  else if(block == "point/line_style") {
    inputLine >> pointstyle;
    linestyle = pointstyle;
  }
  else if(block == "line_color") inputLine >> linecolor;
  else if(block == "point_color") inputLine >> pointcolor;
  else if(block == "point/line_color") {
    inputLine >> pointcolor;
    linecolor = pointcolor;
  }
  else if(block == "fill_color") inputLine >> fillcolor;
  else if(block == "fill_style") inputLine >> fillstyle;
  else if(block == "line_width") inputLine >> linewidth;
  else if(block == "point_size") inputLine >> pointsize;
  else if(block == "close_curve") inputLine >> close;
  else if(block == "bound_points") inputLine >> bound;
  else if(block == "bins") inputLine >> xBins;
  else if(block == "xbins") inputLine >> xBins;
  else if(block == "ybins") inputLine >> yBins;
  else if(block == "contour_levels") {
    double tempLevel;
    contourLevels.resize(0);
    while(!inputLine.eof()) {
      inputLine >> tempLevel;
      contourLevels.push_back(tempLevel);
    }
  }
  else if(block == "autocontour") contourLevels.resize(0);
  else {
    if(graphVerbosity > 0)
      cout << "Graph: Invalid option: " << block << endl;
    return 0;
  }

  return 1;
}

//Return the existence of a legend (0 or 1) and the legend name if any
int Graph::Legend(string &lname) {
  if(legendName != "None" && legendName != "none") {
    lname = legendName;
    return 1;
  }

  lname = "";
  return 0;
}
