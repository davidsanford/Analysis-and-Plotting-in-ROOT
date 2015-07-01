#include"parsecol.h"

/* ######################################################################

                        ParseCol Constructor

  Takes in an open parameter file and sets up reading for a
space/tab-delimited multi-column read file.

Input File Name   Variable     Values
skip_lines        skip           Number of lines to skip.
                                    Default: 0

num_columns       nCol           Total number of columns to input
                                 This is an upper limit on the number
                                 of columns to be read in
                                    Default: -1 (read entire line)

###################################################################### */

ParseCol::ParseCol() {}

ParseCol::ParseCol(istream &in) {
  Initialize(in);
}

ParseCol::ParseCol(ParseCol &other) {
  nCol = other.nCol;
  skip = other.skip;
}

void ParseCol::LoadDefaults() {
  Parse::LoadDefaults();
  skip = 0;
  nCol = -1;
}

//Output the current control settings to the given stream
void ParseCol::PrintSettings(ostream &out) {
  Parse::PrintSettings(out);
  out << "ParseCol Parameters" << endl
      << "# lines to skip at the beginning of the file: " << skip << endl
      << "# total columns to read: " << nCol << endl;
}

int ParseCol::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(Parse::Define(copy)) return 1;

  string block;
  inputLine >> block;

  if(parseVerbosity > 1) {
    cout << "Full Define stream: " << inputLine.str() << endl;
  }

  if(parseVerbosity > 0) {
    cout << "ParseCol: Assigning Block: " << block << endl;
  }

  if(block == "skip_lines") inputLine >> skip;
  else if(block == "num_columns") inputLine >> nCol;
  else {
    if(parseVerbosity > 0)
      cout << "ParseCol: Invalid option: " << block << endl;
    return 0;
  }

  return 1;
}

//Setup (or reset) for reading.  Opens the current input file, deletes
//current data, and performs line skipping.
void ParseCol::OpenFile() {
  readFlag = 1;
  char buff[1000];

  //Delete current data
  for(int i = 0; i < storedValues.size(); i++) {
    storedValues[i] -> resize(0);
  }

  //Output current settings if in verbose mode
  if(parseVerbosity > 0) this -> PrintSettings(cout);

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

  //Discard lines of the file based on the value of 'skip'
  if(readFlag) {
    for(int i = 0; i < skip; i++) {
      input.getline(buff, 1000);
      if(parseVerbosity > 0) {
	cout << "Skipping line: " << buff << endl;
      }
    }
  }
}

//Read a line from the input file
int ParseCol::Read(vector<double> &readValues) {
  char buff[1000];
  input.getline(buff, 1000);

  //Return 0 (exit code) if end-of-file has been reached
  if(input.eof()) return 0;

  string temp = buff;
  temp += " ";
  double current;

  stringstream lineParse(temp);

  lineParse >> current;

  //Loop over all columns
  while(!lineParse.eof() && (nCol < 0 || readValues.size() < nCol)) {
    readValues.push_back(current);
    lineParse >> current;
  }

  if(parseVerbosity > 1) {
    cout << "ParseCol::Read : ";
    for(int i = 0; i < readValues.size(); i++)
      cout << readValues[i] << "  ";
    cout << endl;
  }

  return 1;
}
