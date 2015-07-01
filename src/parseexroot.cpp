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

  Takes in an open parameter file and sets up reading for a
space/tab-delimited multi-column read file.

Input File Name   Variable     Values


###################################################################### */

ParseLHCO::ParseLHCO() : LHCOInputs(0), Chain(NULL), Reader(NULL),
                         JetBranch(NULL), METBranch(NULL), 
			 ElecBranch(NULL), MuonBranch(NULL), 
			 TauBranch(NULL), PhotonBranch(NULL) {}

ParseLHCO::ParseLHCO(istream &in): LHCOInputs(0), Chain(NULL), Reader(NULL),
				   JetBranch(NULL), METBranch(NULL), 
				   ElecBranch(NULL), MuonBranch(NULL), 
				   TauBranch(NULL), PhotonBranch(NULL) {
  Initialize(in);
}

void ParseLHCO::LoadDefaults() {
  Parse::LoadDefaults();
  simType = "Unknown";
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
  numEntries = 0;
  entryIndex = 0;  //Start read index at zero (first event)
  stringstream command;

  //Unzip the file if it is currently zipped (default for madevent output)
  //Inserted to fix errors
  command << file << ".gz";
  if(CheckFileExistence(command.str())) {
    command.str("");
    command << "gunzip " << file << ".gz";
    system(command.str().c_str());
    command.str("");
  }

  //Delete current data
  for(int i = 0; i < storedValues.size(); i++) {
    storedValues[i] -> resize(0);
  }

  //Output current settings if in verbose mode
  if(parseVerbosity > 0) this -> PrintSettings(cout);

  //Open the root file and initialize the reader object
  if(simType == "Delphes" || simType == "delphes")
    Chain = new TChain("Analysis");
  else Chain = new TChain("LHCO");
  Chain -> Add(file.c_str());
  Reader = new ExRootTreeReader(Chain);
  numEntries = Reader -> GetEntries();

  //Grab create pointers to the six branches in the root file
  JetBranch = Reader -> UseBranch("Jet");
  if(simType == "Delphes" || simType == "delphes") 
    METBranch = Reader -> UseBranch("ETmis");
  else METBranch = Reader -> UseBranch("MissingET");
  ElecBranch = Reader -> UseBranch("Electron");
  MuonBranch = Reader -> UseBranch("Muon");
  if(simType == "Delphes" || simType == "delphes") 
    TauBranch = Reader -> UseBranch("TauJEt");
  else TauBranch = Reader -> UseBranch("Tau");
  PhotonBranch = Reader -> UseBranch("Photon");
}

//Read a line from the input file
int ParseLHCO::Read(vector<double> &readValues) {

  //Exit if index has exceeded number of events
  if(entryIndex >= numEntries) return 0;

  //Read current event data and interate
  Reader -> ReadEntry(entryIndex);
  entryIndex++;

  for(int i = 0; i < LHCOInputs.size(); i++) {
    if(LHCOInputs[i] -> GetOption() == "PT") {
      double pt;

      // Use only the first identifier for PT
      if(LHCOInputs[i] -> GetNumSubInputs() > 0 &&
	 GrabValue(pt, "PT", LHCOInputs[i] -> GetType(0),
		   LHCOInputs[i] -> GetIndex(0))) {
	readValues.push_back(pt);
      }
      else readValues.push_back(0);
    }
    else if(LHCOInputs[i] -> GetOption() == "MET") {
      double met;

      // Use only the first identifier for MET
      if(LHCOInputs[i] -> GetNumSubInputs() > 0 &&
	 GrabValue(met, "MET", LHCOInputs[i] -> GetType(0), 0)) {
	readValues.push_back(met);
      }
      else readValues.push_back(0);
    }
    else if(LHCOInputs[i] -> GetOption() == "Phi") {
      double phi;

      // Use only the first identifier for PT
      if(LHCOInputs[i] -> GetNumSubInputs() > 0 &&
	 GrabValue(phi, "Phi", LHCOInputs[i] -> GetType(0),
		   LHCOInputs[i] -> GetIndex(0))) {
	readValues.push_back(phi);
      }
      else readValues.push_back(0);
    }
    else if(LHCOInputs[i] -> GetOption() == "Eta") {
      double eta;

      // Use only the first identifier for PT
      if(LHCOInputs[i] -> GetNumSubInputs() > 0 &&
	 GrabValue(eta, "Eta", LHCOInputs[i] -> GetType(0),
		   LHCOInputs[i] -> GetIndex(0))) {
	readValues.push_back(eta);
      }
      else readValues.push_back(0);
    }
    else if(LHCOInputs[i] -> GetOption() == "DeltaPhi") {
      double phi1, phi2, diff;

      // Use only the first identifier for PT
      if(LHCOInputs[i] -> GetNumSubInputs() > 0 &&
	 GrabValue(phi1, "Phi", LHCOInputs[i] -> GetType(0),
		   LHCOInputs[i] -> GetIndex(0)) &&
	 GrabValue(phi2, "Phi", LHCOInputs[i] -> GetType(1),
		   LHCOInputs[i] -> GetIndex(1))) {
	double diff = phi2 - phi1;
	if(diff < 0) diff = - diff;
	if(diff > 3.1415926) diff = 2 * 3.1415926 - diff;
	readValues.push_back(diff);
      }
      else readValues.push_back(0);
    }
    else if(LHCOInputs[i] -> GetOption() == "DeltaEta") {
      double eta1, eta2;

      // Use only the first identifier for PT
      if(LHCOInputs[i] -> GetNumSubInputs() > 0 &&
	 GrabValue(eta1, "Eta", LHCOInputs[i] -> GetType(0),
		   LHCOInputs[i] -> GetIndex(0)) &&
	 GrabValue(eta2, "Eta", LHCOInputs[i] -> GetType(1),
		   LHCOInputs[i] -> GetIndex(1))) {
	readValues.push_back(eta2 - eta1);
      }
      else readValues.push_back(0);
    }
    else if(LHCOInputs[i] -> GetOption() == "DeltaR") {
      double phi1, phi2, eta1, eta2;

      // Use first two identifiers for Eta and Phi
      if(LHCOInputs[i] -> GetNumSubInputs() > 1 && 
	 GrabValue(phi1, "Phi", LHCOInputs[i] -> GetType(0),
		   LHCOInputs[i] -> GetIndex(0)) &&
	 GrabValue(phi2, "Phi", LHCOInputs[i] -> GetType(1),
		   LHCOInputs[i] -> GetIndex(1)) &&
	 GrabValue(eta1, "Eta", LHCOInputs[i] -> GetType(0),
		   LHCOInputs[i] -> GetIndex(0)) &&
	 GrabValue(eta2, "Eta", LHCOInputs[i] -> GetType(1),
		   LHCOInputs[i] -> GetIndex(1))) {
	double diff = phi2 - phi1;
	if(diff < 0) diff = - diff;
	if(diff > 3.1415926) diff = 2 * 3.1415926 - diff;
	double deltaR = sqrt(sqr(diff) + sqr(eta1 - eta2));
	readValues.push_back(deltaR);
      }
      else readValues.push_back(0);
    }
  }

  return 1;
}

int ParseLHCO::GrabValue(double &val, const char* opt,
			 string type, int index) {
  string option = opt;

  if(type == "Jet" && CheckBranch(JetBranch, index)) {
    TRootJet *jet = (TRootJet*) JetBranch -> At(index - 1);
    if(option == "PT") {
      val = jet -> PT;
    }
    else if(option == "Phi") {
      val = jet -> Phi;
    }
    else if(option == "Eta") {
      val = jet -> Eta;
    }
    /*else if(option == "Mass") {
      val = jet -> Mass;
      }*/
    else return 0;
  }

  //The consistent photon variables are Eta, Phi, PT, an Charge
  else if(type == "Electron" && CheckBranch(ElecBranch, index)) {
    TRootElectron *elec = (TRootElectron*) ElecBranch -> At(index - 1);
    if(option == "PT") {
      val = elec -> PT;
    }
    else if(option == "Phi") {
      val = elec -> Phi;
    }
    else if(option == "Eta") {
      val = elec -> Eta;
    }
    else if(option == "Charge") {
      val = elec -> Charge;
    }
    else return 0;
  }
  else if(type == "Muon" && CheckBranch(MuonBranch, index)) {
    TRootMuon *muon = (TRootMuon*) MuonBranch -> At(index - 1);
    if(option == "PT") {
      val = muon -> PT;
    }
    else if(option == "Phi") {
      val = muon -> Phi;
    }
    else if(option == "Eta") {
      val = muon -> Eta;
    }
    else if(option == "Charge") {
      val = muon -> Charge;
    }
    else return 0;
  }
  else if(type == "Tau" && CheckBranch(TauBranch, index)) {
    TRootTau *tau = (TRootTau*) TauBranch -> At(index - 1);
    if(option == "PT") {
      val = tau -> PT;
    }
    else if(option == "Phi") {
      val = tau -> Phi;
    }
    else if(option == "Eta") {
      val = tau -> Eta;
    }
    else if(option == "Charge") {
      val = tau -> Charge;
    }
    else return 0;
  }

  //The consistent photon variables are Eta, Phi, and PT
  else if(type == "Photon" && CheckBranch(PhotonBranch, index)) {
    TRootPhoton *tau = (TRootPhoton*) PhotonBranch -> At(index - 1);
    if(option == "PT") {
      val = tau -> PT;
    }
    else if(option == "Phi") {
      val = tau -> Phi;
    }
    else if(option == "Eta") {
      val = tau -> Eta;
    }
    else return 0;
  }
  else if(type == "MET" && CheckBranch(METBranch, index)) {

    TRootMissingET *met = (TRootMissingET*) METBranch -> At(0);
    if(option == "MET") {
      val = met -> MET;
    }
    else if(option == "Phi") {
      val = met -> Phi;
    }
    else return 0;
  }
  else return 0;

  //If val has been assigned any non-zero value return success
  return 1;
}

int ParseLHCO::CheckBranch(TClonesArray *Branch, int index) {
  if(Branch == NULL) {
    return 0;
  }
  if(index < 0 || index >= Branch -> GetEntries()) {
    return 0;
  }

  return 1;
}
