#include"convertcol.h"

/* ######################################################################

                        ConvertCol Constructor

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

ConvertCol::ConvertCol() {}

ConvertCol::ConvertCol(istream &in) {
  Initialize(in);
}

ConvertCol::ConvertCol(ConvertCol &other) {
  outFile = other.outFile;
}

void ConvertCol::LoadDefaults() {
  ParseCol::LoadDefaults();
  skip = 0;
  nCol = -1;
}

//Output the current control settings to the given stream
void ConvertCol::PrintSettings(ostream &out) {
  ParseCol::PrintSettings(out);
  out << "ConvertCol Parameters" << endl
      << "# converted output file: " << outFile << endl;
}

int ConvertCol::Define(stringstream &inputLine) {
  stringstream copy(inputLine.str());
  if(ParseCol::Define(copy)) return 1;

  string block;
  inputLine >> block;

  if(parseVerbosity > 1) {
    cout << "Full Define stream: " << inputLine.str() << endl;
  }

  if(parseVerbosity > 0) {
    cout << "ConvertCol: Assigning Block: " << block << endl;
  }

  if(block == "outfile") inputLine >> outFile;
  else {
    if(parseVerbosity > 0)
      cout << "ConvertCol: Invalid option: " << block << endl;
    return 0;
  }

  return 1;
}

//Output converted file
void ConvertCol::PrintOutput() {
  ofstream fout(outFile.c_str());
  fout << setprecision(4) << scientific;

  PrintHeader(fout, 1);

  fout << endl;

  for(int i = 0; i < storedValues[0] -> size(); i++) {
    for(int j = 0; j < storedValues.size(); j++) {
      fout << setw(16) << (*storedValues[j])[i];
    }
    fout << endl;
  }
}

//Ouput a file header for the given input columns
//Defaults to simply column numbers
int ConvertCol::PrintHeader(ostream &out, int startCol) {
  out << setprecision(4) << scientific;
  stringstream nameNumber;

  for(int i = 0; i < headerNames.size(); i++) {
    nameNumber << "(" << startCol + i << ")";
    if(headerNames.size() > 0) nameNumber << " " << headerNames[i];
    out << setw(16) << nameNumber.str();
    nameNumber.str("");
  }

  return startCol + headerNames.size();
}

/*
//Setup (or reset) for reading.  Opens the current input file, deletes
//current data, and performs line skipping.
void ConvertCol::OpenFile() {
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
int ConvertCol::Read(vector<double> &readValues) {
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
    cout << "ConvertCol::Read : ";
    for(int i = 0; i < readValues.size(); i++)
      cout << readValues[i] << "  ";
    cout << endl;
  }

  return 1;
}

*/
