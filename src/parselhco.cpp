#include"parselhco.h"

/* ######################################################################

                        Data Class BranchInfo

Stores the option for input data, along with paired branch type and
branch index values.  Also stores the actual branch pointers.


###################################################################### */

LHCOInfo::LHCOInfo() : types(0), indices(0) {
  option = "";
}

LHCOInfo::LHCOInfo(string opt) : types(0), indices(0) {
  option = opt;
}

string LHCOInfo::GetType(int subInput) {
  if(subInput < 0 || subInput > types.size()) return "";
  else return types[subInput];
}

int LHCOInfo::GetIndex(int subInput) {
  if(subInput < 0 || subInput > indices.size()) return -1;
  else return indices[subInput];
}

void LHCOInfo::AddSubInput(string newType, int newIndex) {
  types.push_back(newType);
  indices.push_back(newIndex);
}

/* ######################################################################

                        ParseLHCO Constructor

  Takes in an open parameter file and sets up reading for an
LHCO formatted file

Input File Name   Variable     Values


###################################################################### */

ParseLHCO::ParseLHCO() : LHCOInputs(0) {}

ParseLHCO::ParseLHCO(istream &in) : LHCOInputs(0) {
  Initialize(in);
}

void ParseLHCO::LoadDefaults() {
  Parse::LoadDefaults();
}

//Output the current control settings to the given stream
void ParseLHCO::PrintSettings(ostream &out) {
  Parse::PrintSettings(out);
  out << "ParseLHCO Parameters" << endl;
  for(int i = 0; i < LHCOInputs.size(); i++) {
    out << "LHCO Option: " << LHCOInputs[i] -> GetOption() << " ";
    for(int j = 0; j < LHCOInputs[i] -> GetNumSubInputs(); j++) {
      out << LHCOInputs[i] -> GetType(j) << " "
	  << LHCOInputs[i] -> GetIndex(j) << " ";
    }
    out << endl;
  }
}

int ParseLHCO::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(Parse::Define(copy)) return 1;

  string block;
  inputLine >> block;

  if(block == "lhco_input") {
    string type, option;
    int val;
    inputLine >> option;
    LHCOInputs.push_back(new LHCOInfo(option));

    while(!inputLine.eof()) {
      inputLine >> type;
      if(type != "MET") inputLine >> val;
      else val = -1;
      LHCOInputs.back() -> AddSubInput(type, val);
    }
  }
  else {
    if(parseVerbosity > 0)
      cout << "ParseLHCO: Invalid option: " << block << endl;
    return 0;
  }

  return 1;
}

//Setup (or reset) for reading.  Opens the current input file, deletes
//current data, and performs line skipping.
void ParseLHCO::OpenFile() {
  readFlag = 1;
  stringstream command;

  //Unzip the file if it is currently zipped (default for madevent output)
  //Inserted to fix errors
  command << file << ".gz";
  if(CheckFileExistence(command.str())) {
    if(parseVerbosity > 0)
      cout << "Unzipping " << file << ".gz" << endl;
    command.str("");
    command << "gunzip " << file << ".gz";
    system(command.str().c_str());
  }
  command.str("");
  

  //Delete current data
  for(int i = 0; i < storedValues.size(); i++) {
    storedValues[i] -> resize(0);
  }

  //Reset stream if it is already open
  if(input.is_open()) {
    input.close();
    input.clear();
  }

  //Open file to be parsed
  input.open(file.c_str());

  //Check file existence
  if(!input.is_open()) {
    cout << "Error, could not find file " << file << endl;
    readFlag = 0;
  }
  else if(parseVerbosity > 0) {
    cout << file << " opened successfully" << endl;
  }

  //Skip lines starting in "#"
  if(readFlag) {
    char buff[1000];
    input.getline(buff, 1000);
    while(buff[0] == '#' || buff[1] == '#' || buff[2] == '#') {
      input.getline(buff, 1000);
    }
  }

  //Output current settings if in verbose mode
  if(parseVerbosity > 0) this -> PrintSettings(cout);
}

//Read a line from the input file
int ParseLHCO::Read(vector<double> &readValues) {
  //Return 0 (exit code) if end-of-file has been reached
  if(input.eof()) return 0;

  char buff[1000];
  stringstream line;
  int trackNum = 0;

  Event *currentEvent = new Event();

  while(!input.eof()) {
    line.clear();
    line.seekp(0, ios_base::beg);
    input.getline(buff, 1000);
    //cout << buff << endl;
    //return 0;
    line << buff;
    line >> trackNum;
    if(trackNum == 0 || input.eof()) break;
    else {
      line.str("");
      line.clear();
      line.seekp(0, ios_base::beg);
      line << buff;
      currentEvent -> AddTrack(new Track(line));
    }
    line.str("");
  }

  currentEvent -> SortTracks();

  Track *track1 = NULL, *track2 = NULL;

  for(int i = 0; i < LHCOInputs.size(); i++) {
    if(LHCOInputs[i] -> GetOption() == "PT") {
      // Use only the first identifier for PT
      if(LHCOInputs[i] -> GetNumSubInputs() > 0) {
	track1 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(0),
					  LHCOInputs[i] -> GetIndex(0) - 1);
	if(track1 != NULL) {
	  /*cout << "Got PT: " << LHCOInputs[i] -> GetType(0) << " #"
	       << LHCOInputs[i] -> GetIndex(0) << "  Value: " 
	       << track1 -> GetPT() << endl;*/
	  readValues.push_back(track1 -> GetPT());
	  continue;
	}
	/*	else {
	  cout << "Failed to get PT: " << LHCOInputs[i] -> GetType(0) << " #"
	       << LHCOInputs[i] -> GetIndex(0) << endl;
	       }*/
      }
    }
    else if(LHCOInputs[i] -> GetOption() == "Phi") {
      // Use only the first identifier for PT
      if(LHCOInputs[i] -> GetNumSubInputs() > 0) {
	track1 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(0),
					  LHCOInputs[i] -> GetIndex(0) - 1);
	if(track1 != NULL) {
	  readValues.push_back(track1 -> GetPhi());
	  continue;
	}
      }
    }
    else if(LHCOInputs[i] -> GetOption() == "Eta") {
      // Use only the first identifier for Eta
      if(LHCOInputs[i] -> GetNumSubInputs() > 0) {
	track1 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(0),
					  LHCOInputs[i] -> GetIndex(0) - 1);
	if(track1 != NULL) {
	  readValues.push_back(track1 -> GetEta());
	  continue;
	}
      }
    }
    else if(LHCOInputs[i] -> GetOption() == "DeltaPhi") {
      // Use only the first two identifiers for Delta Phi
      if(LHCOInputs[i] -> GetNumSubInputs() > 0) {
	track1 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(0),
					  LHCOInputs[i] -> GetIndex(0) - 1);
	track2 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(1),
					  LHCOInputs[i] -> GetIndex(1) - 1);
	if(track1 != NULL && track2 != NULL) {
	  double diff = track2 -> GetPhi() - track1 -> GetPhi();
	  if(diff < 0) diff = - diff;
	  if(diff > 3.1415926) diff = 2 * 3.1415926 - diff;
	  readValues.push_back(diff);
	  continue;
	}
      }
    }
    else if(LHCOInputs[i] -> GetOption() == "DeltaEta") {
      // Use only the first two identifiers for Delta Eta
      if(LHCOInputs[i] -> GetNumSubInputs() > 0) {
	track1 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(0),
					  LHCOInputs[i] -> GetIndex(0) - 1);
	track2 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(1),
					  LHCOInputs[i] -> GetIndex(1) - 1);
	if(track1 != NULL && track2 != NULL) {
	  readValues.push_back(track2 -> GetEta() - track1 -> GetEta());
	  continue;
	}
      }
    }
    else if(LHCOInputs[i] -> GetOption() == "DeltaR") {
      // Use first two identifiers for Eta and Phi
      if(LHCOInputs[i] -> GetNumSubInputs() > 1) {
	track1 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(0),
					  LHCOInputs[i] -> GetIndex(0) - 1);
	track2 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(1),
					  LHCOInputs[i] -> GetIndex(1) - 1);
	if(track1 != NULL && track2 != NULL) {
	  double diff = track2 -> GetPhi() - track1 -> GetPhi();
	  if(diff < 0) diff = - diff;
	  if(diff > 3.1415926) diff = 2 * 3.1415926 - diff;
	  double deltaR = sqrt(sqr(diff) + sqr(track2 -> GetEta() 
					       - track1 -> GetEta()));
	  readValues.push_back(deltaR);
	  continue;
	}
      }
    }
    else if(LHCOInputs[i] -> GetOption() == "BTag") {
      // Use first two identifiers for Eta and Phi
      if(LHCOInputs[i] -> GetNumSubInputs() > 1) {
	track1 = currentEvent -> GetTrack(LHCOInputs[i] -> GetType(0),
					  LHCOInputs[i] -> GetIndex(0) - 1);
	if(track1 != NULL) {
	  readValues.push_back(track1 -> GetBTag());
	  continue;
	}
      }
    }

    //Only executes if no other value inserted
    readValues.push_back(0);
  }

  if(parseVerbosity > 1) {
    cout << "ParseLHCO::Read : ";
    for(int i = 0; i < readValues.size(); i++)
      cout << readValues[i] << "  ";
    cout << endl;
  }

  return 1;
}

