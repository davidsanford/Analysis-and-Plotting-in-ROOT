#include"aggregate.h"

Aggregate::Aggregate() : low(0), high(0), iterate(0), 
			 current(0), Sources(0), leadingAffix(0),
			 followingAffix(0), SourceTypes(0) {}

Aggregate::Aggregate(istream &in) : low(0), high(0), iterate(0), 
				    current(0), Sources(0), leadingAffix(0),
				    followingAffix(0), SourceTypes(0) {
  if(aggregateVerbosity > 0) cout << "Aggregate::Aggregate : Start" << endl;

  Initialize(in);

  if(aggregateVerbosity > 0) cout << "Aggregate::Aggregate : End" << endl;
}

void Aggregate::LoadDefaults() {
  headerFlag = 1;
}

//Define a variable taken from the input stringstream
//The first input is the control sequence, subsequent inputs are
//parsed based on that sequence.  Excess inputs in a particular
//sequence are discarded. 
int Aggregate::Define(stringstream &inputLine) {
  char buff[1000];
  string block;

  inputLine >> block;

  if(aggregateVerbosity > 1) {
    cout << "Full Define stream: " << inputLine.str() << endl;
  }

  if(aggregateVerbosity > 0) {
    cout << "Aggregate: Assigning Block: " << block << endl;
  }

  if(block == "affix") {
    inputLine.getline(buff, 1000);
    stringstream affixconvert(buff);
    leadingAffix.push_back("");
    followingAffix.push_back("");
    affixconvert >> leadingAffix.back() >> followingAffix.back();
  }
  else if(block == "fragment") {
    nameFragments.push_back("");
    low.push_back(0);
    high.push_back(0);
    iterate.push_back(0);
    inputLine >> nameFragments.back() >> low.back()
	      >> high.back() >> iterate.back();

    current.push_back(low.back());
  }
  else if(block == "print_header") {
    inputLine >> headerFlag;
  }
  else {
    if(aggregateVerbosity > 0)
      cout << "Aggregate: Invalid option: " << block << endl;
    return 0;
  }

  return 1;
}

//Print current aggregate settings
void Aggregate::PrintSettings(ostream &out) {
  out << "Aggregate Settings" << endl
      << "Number of Fragments: " << nameFragments.size() << endl;
  for(int i = 0; i < nameFragments.size(); i++) {
    out << nameFragments[i] << " from " << low[i] << " to "
	<< high[i] << " in increments of " << iterate[i] << endl;
  }
  for(int i = 0; i < leadingAffix.size(); i++) {
    out << "Source files from " << leadingAffix[i] << "[_____]"
	<< followingAffix[i] << endl;
  }
}

//Define a new parse object to aggregate
void Aggregate::NewSource(istream &in, string type) {

  //Create a new graph based on the input type
  if(type == "COLUMN") Sources.push_back(new ParseCol(in));
  else if(type == "LHCO") Sources.push_back(new ParseLHCO(in));
  else if(type == "SLHA") Sources.push_back(new ParseSLHA(in));
  else return;

  SourceTypes.push_back(type);
}

int Aggregate::Write(ostream &out) {
  current[0] -= iterate[0];
  string base;
  stringstream filename, zippedfilename, definition;
  int writeFlag = 0;

  //Output current settings if in verbose mode
  if(aggregateVerbosity > 0) this -> PrintSettings(cout);

  while(ContinueIteration()) {
    base = NextFile();

    if(aggregateVerbosity > 0)
      cout << "Current central fragment: " << base << endl;

    for(int i = 0; i < Sources.size() && i < leadingAffix.size(); i++) {
      filename << leadingAffix[i] << base << followingAffix[i];
      zippedfilename << leadingAffix[i] << base
		     << followingAffix[i] << ".gz";

      if(aggregateVerbosity > 0)
	cout << "Current file: " << filename.str() << endl;

      if(CheckFileExistence(filename.str())
	 || CheckFileExistence(zippedfilename.str())) {

	writeFlag = 1;

	definition << "file  " << filename.str();
	Sources[i] -> Def::Define(definition.str());
	Sources[i] -> Fill();

	if(SourceTypes[i] == "SLHA") {
	  vector<double> slhadata;
	  Sources[i] -> GetStoredRow(0, slhadata);
	  for(int j = 0; j < slhadata.size(); j++) {
	    out << setw(16) << slhadata[j];
	  }
	}
	else {
	  out << setw(16) << Sources[i] -> GetNumRows();
	}
      }
      else {
	if(aggregateVerbosity > 1)
	  cout << "Cannot find file: " << filename.str() << endl;

	writeFlag = 0;
	break;
      }

      filename.str("");
      definition.str("");
      zippedfilename.str("");
    }

    filename.str("");
    definition.str("");
    zippedfilename.str("");

    if(writeFlag) out << endl;
  }
}

void Aggregate::PrintHeader(ostream &out) {
  if(headerFlag) {
    int startCol = 1;

    for(int i = 0; i < Sources.size() && i < leadingAffix.size(); i++) {
      startCol = Sources[i] -> PrintHeader(out, startCol);
    }
    out << endl;
  }
  else out << setprecision(4) << scientific;
}

string Aggregate::NextFile() {
  stringstream filename;

  for(long int i = 0; i < low.size(); i++) {
    if(current[i] < high[i]) {
      current[i] += iterate[i];
      for(long int j = 0; j < i; j++) {
	current[j] = low[j];
      }
      break;
    }
  }

  for(long int i = 0; i < low.size(); i++) {
    filename << nameFragments[i] << current[i];
  }

  return filename.str();
}

int Aggregate::ContinueIteration() {
  for(long int i = 0; i < low.size(); i++) {
    if(current[i] < high[i]) return 1;
  }

  return 0;
}
