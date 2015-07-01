#include"plot.h"
#include"aggregate.h"
#include"convertcol.h"

using namespace std;

int Parse::parseVerbosity = 0;
int Plot::plotVerbosity = 0;
int Graph::graphVerbosity = 0;
int Aggregate::aggregateVerbosity = 0;
int Plot::plotNumber = 1;
int Contour::contourNumber = 1;
int Histogram::histNumber = 1;
int Parse::numOpen = 1;

string fileType(string &file) {
  int i;

  for(i = 0; i < file.size(); i++) {
    if(file[i] == '.') break;
  }

  string extension = "";

  for(i += 1; i < file.size(); i++)
    extension.push_back(file[i]);

  return extension;
}

int main(int argc, char* argv[]) {
  if(argc < 2) {
    cout << "Error, need an input file" << endl;
    return 0;
  }

  for(int filenum = 1; filenum < argc; filenum++) {

    ifstream fin(argv[filenum]);

    if(!fin.is_open()) {
      cout << "Error, invalid file: " << argv[filenum] << endl;
      return 0;
    }

    char buff[1000];
    string directory, option, outputFiles, lastPrimary = "";
    stringstream command, readLine;

    vector<Plot*> Plots(0);
    vector<Aggregate*> Aggregates(0);
    vector<ConvertCol*> Convert(0);

    cout << "Starting up file #" << filenum << ".  Hello." << endl;

    while(!fin.eof()) {
      readLine.seekp(0, ios_base::beg);
      readLine.clear();
      readLine.str("");
      fin.getline(buff, 1000);
      readLine.str(buff);
      if(readLine.str() == "" || (readLine.str())[0] == '#') {
	continue;  //Skip empty lines or lines starting in '#' (commend code)
      }
      readLine >> option;
      if(option == "PARSEVERBOSITY") {
	int temp;
	readLine >> temp;
	Parse::parseVerbosity = temp;
      }
      else if(option == "PLOTVERBOSITY") {
	int temp;
	readLine >> temp;
	Plot::plotVerbosity = temp;
      }
      else if(option == "GRAPHVERBOSITY") {
	int temp;
	readLine >> temp;
	Graph::graphVerbosity = temp;
      }
      else if(option == "AGGREGATEVERBOSITY") {
	int temp;
	readLine >> temp;
	Aggregate::aggregateVerbosity = temp;
      }
      else if(option == "NEWPLOT") {
	Plots.push_back(new Plot(fin));
	lastPrimary = "plot";
      }
      else if(option == "NEWAGGREGATE") {
	Aggregates.push_back(new Aggregate(fin));
	lastPrimary = "aggregate";
      }
      else if(option == "NEWCONVERT") {
	Convert.push_back(new ConvertCol(fin));
      }
      else if(option == "CURVE" || option == "CONTOUR" ||
	      option == "HISTOGRAM" || option == "LHCOHIST" ||
	      option == "LHCOCURVE" || option == "LHCOCONTOUR") {
	if(Plots.size() > 0) {
	  if(lastPrimary == "plot") {
	    Plots.back() -> NewGraph(fin, option);
	  }
	  else
	    cout << "Error: No plot to which " << option
		 << "can be added" << endl;
	}
	else 
	  cout << "Error: No plots exist" << endl;
      }
      else if(option == "COLUMN" || option == "LHCO"
	      || option == "SLHA") {
	if(Aggregates.size() > 0) {
	  if(lastPrimary == "aggregate") {
	    Aggregates.back() -> NewSource(fin, option);
	  }
	  else
	    cout << "Error: No aggregate to which " << option
		 << "can be added" << endl;
	}
	else 
	  cout << "Error: No aggregate exist" << endl;
      }
      else if(option == "PLOT") {
	readLine.getline(buff, 1000);
	outputFiles = buff;

	istringstream sin(outputFiles);
	vector<string> saveFiles, extension;
	string temp;
	while(!sin.eof()) {
	  sin >> temp;
	  saveFiles.push_back(temp);
	  extension.push_back(fileType(temp));
	}

	TCanvas *blank = new TCanvas("blank", "", 0, 0, 600, 600);

	ostringstream sout;

	//Note: put in a type parsing algorithm

	TFile *rootFile = NULL;

	for(int i = 0; i < saveFiles.size(); i++) {
	  if(extension[i] == "root") {
	    if(rootFile != NULL) delete rootFile;
	    rootFile = new TFile(temp.c_str(), "recreate");
	  }
	}

	for(int i = 0; i < saveFiles.size(); i++) {
	  if(extension[i] == "pdf" || extension[i] == "ps") {
	    sout << saveFiles[i] << "[";
	    cout << sout.str() << endl;
	    blank -> Print(sout.str().c_str(), extension[i].c_str());
	    sout.str("");
	  }
	}

	for(int i = 0; i < Plots.size(); i++) {
	  Plots[i] -> Draw();
	  for(int j = 0; j < saveFiles.size(); j++) {
	    if(extension[j] == "pdf" || extension[j] == "ps") {
	      cout << saveFiles[j] << endl;
	      Plots[i] -> Write(saveFiles[j]);
	    }
	    else if(extension[i] == "root")
	      Plots[i] -> Save();
	  }
	}

	for(int i = 0; i < saveFiles.size(); i++) {
	  if(extension[i] == "pdf" || extension[i] == "ps") {
	    sout << saveFiles[i] << "]";
	    cout << sout.str() << endl;
	    blank -> Print(sout.str().c_str(),"pdf");
	    sout.str("");
	  }
	}

	if(rootFile != NULL) rootFile -> Close();

	Plots.resize(0);
	lastPrimary = "";
      }
      else if(option == "AGGREGATE") {
	char buff[1000];
	readLine.getline(buff, 1000);
	outputFiles = buff;

	istringstream sin(outputFiles);
	string saveFile;
	sin >> saveFile;

	if(Aggregates.size() > 0) {
	  ofstream fout(saveFile.c_str());
	  Aggregates[0] -> PrintHeader(fout);

	  for(int i = 0; i < Aggregates.size(); i++) {
	    Aggregates[i] -> Write(fout);
	  }
	}

	Aggregates.resize(0);
	lastPrimary = "";
      }
      else if(option == "CONVERT") {
	for(int i = 0; i < Convert.size(); i++) {
	  Convert[i] -> Fill();
	  Convert[i] -> PrintOutput();
	  delete Convert[i];
	}
	Convert.resize(0);
      }
      else if(option == "EXTERNALCOMMAND") {
	fin.getline(buff, 1000);
	cout << buff << endl;
	system(buff);
      }
      else if(option == "ENDFILE") {
	cout << "Finishing up input #" << filenum <<".  Goodbye." << endl;
	break;
      }
      else {
	cout << "Error: Invalid option: " << option << "; Exiting." << endl;
	break;
      }
    }
  }
}
