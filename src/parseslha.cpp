#include"parseslha.h"

SLHAItem::SLHAItem()
  : block(""), code(-1), code2(-1), value(0), varname("NULL"), state(-1) {}

SLHAItem::SLHAItem(string &n, string &b, long int c)
  : block(b), varname(n), code(c), code2(0), value(0), state(0) {}

SLHAItem::SLHAItem(string &n, string &b, long int c, long int c2)
  : block(b), varname(n), code(c), code2(c2), value(0), state(0) {}

SLHAItem & SLHAItem::operator=(SLHAItem &slha) {
  block = slha.block;
  code = slha.code;
  code2 = slha.code2;
  value = slha.value;
  state = slha.state;
  varname = slha.varname;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GeneralItem::GeneralItem() {}

GeneralItem::GeneralItem(string type) {
  ruleType = type;
  value = 0;
  decayParent = 0;
}

GeneralItem::GeneralItem(string type, int parent) {
  ruleType = type;
  value = -1;
  decayParent = parent;
}

void GeneralItem::CheckRule(istream &in) {
  string input;
  int compare;
  double temp;

  if(ruleType == "Xsection") {
    in >> input;
    if(input == "Integrated") {
      in >> input >> input >> input >> value;
    }
  }
  else if(ruleType == "numevents") {
    in >> input;
    if(input == "Number") {
      in >> input;
      in >> input;
      if(input == "Events") {
	in >> input >> value;
      }
    }
  }
  else if(ruleType == "DECAY") {
    in >> compare;
    if(compare == decayParent) in >> value;
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

CheckLine::CheckLine()
  : line(""), existence(0) {}

CheckLine::CheckLine(string nameIn, string lineIn)
  : name(nameIn), line(lineIn), existence(0) {}

void CheckLine::Compare(string lineIn) {
  Compare(lineIn.c_str());
}

void CheckLine::Compare(const char *lineIn) {
  if(line == lineIn) existence = 1;
}

void CheckLine::Reset() {
  existence = 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ParseSLHA::ParseSLHA() : slhaData(0) {}

ParseSLHA::ParseSLHA(istream &in) : slhaData(0), generalData(0),
				    lineData(0) {
  Initialize(in);
}

int ParseSLHA::PrintHeader(ostream &out, int startCol) {
  out << setprecision(4) << scientific;
  stringstream nameNumber;

  if(correlatedInput > 0) {
    for(int i = 0; i < slhaData.size(); i++) {
      nameNumber << "(" << startCol + i << ") "
		 << slhaData[i] -> GetName();
      out << setw(16) << nameNumber.str();
      nameNumber.str("");
    }
    for(int i = 0; i < generalData.size(); i++) {
      nameNumber << "(" << startCol + slhaData.size() + i << ") ";
      if(generalData[i] -> GetRule() == "DECAY")
	nameNumber << "DCY " << generalData[i] -> GetParent();
      else
	nameNumber << generalData[i] -> GetRule();

      out << setw(16) << nameNumber.str();
      nameNumber.str("");
    }
    for(int i = 0; i < lineData.size(); i++) {
      nameNumber << "(" << startCol + slhaData.size() + i << ") "
		 << lineData[i] -> GetName();
      out << setw(16) << nameNumber.str();
      nameNumber.str("");
    }

    return startCol + slhaData.size() + generalData.size() + lineData.size();
  }

  for(int i = 0; i < inputFormulas.size(); i++) {
    nameNumber << "(" << startCol << ")";
    if(headerNames.size() > i) nameNumber << " " << headerNames[i];
    out << setw(16) << nameNumber.str();
    nameNumber.str("");
  }

  return startCol + inputFormulas.size();
}

void ParseSLHA::LoadDefaults() {
  Parse::LoadDefaults();
  madOutput = 0;
  reZip = 0;
  correlatedInput = 0;
}

void ParseSLHA::PrintSettings(ostream &out) {
  Parse::PrintSettings(out);
  out << "ParseSLHA Parameters" << endl;
  for(int i = 0; i < slhaData.size(); i++) {
    out << "Block: " << slhaData[i] -> GetBlock() 
	<< ", Code: " << slhaData[i] -> GetCode();
    if(DoubleBlock(slhaData[i] -> GetBlock())) 
      out << ", Code 2: " << slhaData[i] -> GetCode2();

    out << endl;
  }
  for(int i = 0; i < generalData.size(); i++) {
    out << "General Rule: " << generalData[i] -> GetRule();
    if(generalData[i] -> GetRule() == "DECAY")
      out << " " << generalData[i] -> GetParent();
    out << endl;
  }
  for(int i = 0; i < lineData.size(); i++) {
    out << "Line Comparison: " << lineData[i] -> GetName()
	<< "   \"" << lineData[i] -> GetLine() << "\"" << endl;
  }
  out << "Use madgraph output format: " << madOutput << endl;
  out << "Zip file when finished: " << reZip << endl;
  out << "Tie SLHA input to normal input: " << correlatedInput << endl;
}

int ParseSLHA::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(Parse::Define(copy)) return 1;

  string block;
  inputLine >> block;

  if(parseVerbosity > 1) {
    cout << "Full Define stream: " << inputLine.str() << endl;
  }

  if(parseVerbosity > 0) {
    cout << "ParseSLHA: Assigning Block: " << block << endl;
  }

  if(block == "slha_input") {
    string varname;
    long int code, code2;
    SLHAItem* slha;
  
    inputLine >> varname >> block >> code;
    if(DoubleBlock(block)) {
      inputLine >> code2;
      slha = new SLHAItem(varname, block, code, code2);
    }
    else 
      slha = new SLHAItem(varname, block, code);

    slhaData.push_back(slha);
  }
  else if(block == "general_input") {
    string type;
    inputLine >> type;
    if(type == "DECAY") {
      int parent;
      inputLine >> parent;
      generalData.push_back(new GeneralItem(type, parent));
    }
    else
      generalData.push_back(new GeneralItem(type));
  }
  else if(block == "check_line") {
    string name;
    char buff[1000];
    inputLine >> name;
    inputLine.getline(buff, 1000);
    string rawline = buff;
    string line = "";

    int index;

    for(index = 0; index < rawline.size(); index++) {
      if(rawline[index] == '\"') break;
    }

    index++;

    for(index; index < rawline.size(); index++) {
      if(rawline[index] == '\"') break;
      line.push_back(rawline[index]);
    }

    lineData.push_back(new CheckLine(name, line));
  }
  else if(block == "madoutput") inputLine >> madOutput;
  else if(block == "rezip") inputLine >> reZip;
  else if(block == "correlateinput") inputLine >> correlatedInput;
  else {
    if(parseVerbosity > 0)
      cout << "ParseSLHA: Invalid option: " << block << endl;
    return 0;
  }
}

void ParseSLHA::OpenFile() {
  readFlag = 1;
  char buff[1000];
  stringstream command;

  //Delete current data
  for(int i = 0; i < storedValues.size(); i++) {
    storedValues[i] -> resize(0);
  }

  //Reset line check data
  for(int i = 0; i < lineData.size(); i++) {
    lineData[i] -> Reset();
  }

  //Enforce 1-to-1 correspondence between slha inputs and parse inputs
  //if correlation flag is set 
  if(correlatedInput > 0) {
    stringstream definition;
    definition << "reset_input";
    Define(definition);
    definition.str("");

    int index;
    for(int i = 0; i < slhaData.size() + generalData.size()
	  + lineData.size(); i++) {
      definition << "input   #" << i + 1;
      Define(definition);
      definition.str("");
    }
  }

  //Output current settings if in verbose mode
  if(parseVerbosity > 0) this -> PrintSettings(cout);

  //Reset stream if it is already open
  if(input.is_open()) {
    input.close();
    input.clear();
  }

  //Unzip the file if it is currently zipped (default for madevent output)
  //No effect if the file is already unzipped
  command << file << ".gz";
  if(CheckFileExistence(command.str())) {
    if(parseVerbosity > 0)
      cout << "Unzipping " << file << ".gz" << endl;
    command.str("");
    command << "gunzip " << file << ".gz";
    system(command.str().c_str());
    command.str("");
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
}

int ParseSLHA::Read(vector<double> &readValues) {
  stringstream convert;
  string currentBlock, test, temp;
  long int code, code2;
  double val;
  char buff[1000];
  stringstream command;

  int n = 0;

  while(!input.eof()) {

    convert.str("");
    convert.seekp(0, ios_base::beg);
    convert.clear();
    input.getline(buff, 1000);

    if(madOutput) {
      if(buff[0] == '#') buff[0] = ' ';
      else {
	if(parseVerbosity > 0)
	  cout << "Reached event listing.  Exiting this file." << endl;
	break;
      }
    }

    for(int i = 0; i < lineData.size(); i++) {
      lineData[i] -> Compare(buff);
    }

    convert << buff;

    convert >> test;

    if(test == "<event>" || test == "</LesHouchesEvents>") {
      if(parseVerbosity > 0)
	cout << "Reached event listing.  Exiting this file." << endl;
      break;
    }
    else if(test == "" || test == " ") continue;
    else if(test == "#") {
      for(int i = 0; i < generalData.size(); i++) {
	generalData[i] -> CheckRule(convert);
	convert.str("");
	convert.seekp(0, ios_base::beg);
	convert.clear();
	convert << buff;
	convert >> test;
      }
    }
    else if(test == "Block" || test == "BLOCK") {
      convert >> currentBlock;
      continue;
    }
    else if(test == "DECAY") {
      currentBlock = "";
      for(int i = 0; i < generalData.size(); i++) {
	if(generalData[i] -> GetRule() == "DECAY") {
	  generalData[i] -> CheckRule(convert);
	  convert.str("");
	  convert.seekp(0, ios_base::beg);
	  convert.clear();
	  convert << buff;
	  convert >> test;
	}
      }
    }
    else {
      convert.str("");
      convert.seekp(0, ios_base::beg);
      convert.clear();
      convert << buff;

      if(!DoubleBlock(currentBlock)) {
	convert >> code >> val;

	for(int i = 0; i < slhaData.size(); i++) {
	  if(currentBlock == slhaData[i] -> GetBlock()
	     && code == slhaData[i] -> GetCode()) {
	    slhaData[i] -> SetVal(val);
	    slhaData[i] -> SetState(1);
	  }
	}
      }
      else {
	convert >> code >> code2 >> val;
	for(int i = 0; i < slhaData.size(); i++) {
	  if(currentBlock == slhaData[i] -> GetBlock()
	     && code == slhaData[i] -> GetCode() 
	     && code2 == slhaData[i] -> GetCode2()) {
	    slhaData[i] -> SetVal(val);
	    slhaData[i] -> SetState(1);
	  }
	}
      }
    }
  }

  for(int i = 0; i < slhaData.size(); i++) {
    if(slhaData[i] -> GetBlock() == "EXTERNAL")
      readValues.push_back(slhaData[i] -> GetCode());
    else
      readValues.push_back(slhaData[i] -> GetVal());
  }
  for(int i = 0; i < generalData.size(); i++) {
    readValues.push_back(generalData[i] -> GetVal());
  }
  for(int i = 0; i < lineData.size(); i++) {
    readValues.push_back(lineData[i] -> GetExistence());
  }

  //Re-zip the file if it originated from madevent output
  if(reZip > 0) { 
    command << "gzip " << file;
    system(command.str().c_str());
    command.str("");
  }

  readFlag = 0;

    //Reset stream if it is already open
  if(input.is_open()) {
    input.close();
    input.clear();
  }

  return 1;
}

int ParseSLHA::DoubleBlock(string b) {
  if(b == "nmix" || b == "Umix" || b == "Vmix" || b == "stopmix"
     || b == "sbotmix" || b == "staumix" || b == "yu" || b == "yd"
     || b == "ye" || b == "au" || b == "ad" || b == "ae" 
     || b == "DMMIX" || b == "NMHMIX" || b == "NMAMIX"
     || b == "NMNMIX") return 1;
  else return 0;
}
