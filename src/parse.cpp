#include"parse.h"

/* ######################################################################

                               Parse Constructor

  Takes in an open input file and assigns all appropriate variables.
This is a stub class; a derived class which defines the type of input
file must be used for actual input.  Most are specified directly from
the input file, with default values given in LoadDefaults.

Input File Name   Variable     Values
file              file           Name of file to be parsed

reset_input       inputFormulas  Sets the size of the formula and data
                  storedValues   vectors to zero.  Must be called to 
                                 remove default two-column input

input             inputFormulas  Formulas for values to be read from
                  storedValues   the file.  The size of the stored values 
                                 array is tied to the formula array.
                                   Default: None

sorting           sorting        Sort mode
                  sortCol          Default: nosort - leave as read
		                   increasing #: sort low to high by input #
                                   decreasing # - sort high to low by input #
                                 'sortCol' specifies the (human-readable)
                                 column number to use for sorting

###################################################################### */

Parse::Parse() {}

Parse::Parse(istream &in) {
  if(parseVerbosity > 0) cout << "Parse::Parse : Start" << endl;

  Initialize(in);

  if(parseVerbosity > 0) cout << "Parse::Parse : End" << endl;
}

//Copy constructor
Parse::Parse(Parse &other) {
  sortCol = other.sortCol;
  file = other.file;
  sorting = other.sorting;
  inputFormulas.resize(0);
  headerNames.resize(0);
  storedValues.resize(0);
  for(int i = 0; i < other.inputFormulas.size(); i++) {
    inputFormulas.push_back(new Formula(*other.inputFormulas[i]));
    storedValues.push_back(new vector<double>(0));
  }
  conditions.resize(0);
}

//Loads default values for the Parse class
//Should be called by inherited classes before additional defaults
void Parse::LoadDefaults() {
  file = "";
  sorting = "nosort";
  sortCol = 0;
  inputFormulas.resize(0);
  headerNames.resize(0);
  storedValues.resize(0);
  conditions.resize(0);
}

//Output the current control settings to the given stream
void Parse::PrintSettings(ostream &out) {
  out << "Parse Parameters" << endl
      << "Input file: " << file << endl;
  for(int i = 0; i < inputFormulas.size(); i++) {
    out << "Input value #" << i + 1 << ": " << *inputFormulas[i] << endl;
  }  
  for(int i = 0; i < headerNames.size(); i++) {
    out << "Column header #" << i + 1 << ": " << headerNames[i] << endl;
  }
  for(int i = 0; i < conditions.size(); i++) {
    out << "Condition #" << i + 1 << ": " << *conditions[i] << endl;
  }
  out << "Sorting: " << sorting << " " << sortCol << endl;
}

//Ouput a file header for the given input columns
//Defaults to simply column numbers
int Parse::PrintHeader(ostream &out, int startCol) {
  out << setprecision(4) << scientific;
  stringstream nameNumber;

  nameNumber << "(" << startCol << ")";
  if(headerNames.size() > 0) nameNumber << " " << headerNames[0];
  out << setw(16) << nameNumber.str();
  nameNumber.str("");

  return startCol + 1;
}

//Define a variable taken from the input stringstream
//The first input is the control sequence, subsequent inputs are
//parsed based on that sequence.  Excess inputs in a particular
//sequence are discarded.
int Parse::Define(stringstream &inputLine) {
  string block;

  inputLine >> block;

  if(parseVerbosity > 1) {
    cout << "Full Define stream: " << inputLine.str() << endl;
  }

  if(parseVerbosity > 0) {
    cout << "Parse: Assigning Block: " << block << endl;
  }

  if(block == "file") inputLine >> file;

  //Reset the input, including the 
  else if(block == "reset_input") {
    inputFormulas.resize(0);
    storedValues.resize(0);
  }

  //Add the new input formula to the list of formulas and 
  //create a new vector for input
  else if(block == "input") {  
    char buff[1000];  
    inputLine.getline(buff, 1000);
    string temp = buff;
    temp += " ";
    inputFormulas.push_back(new Formula(temp));
    storedValues.push_back(new vector<double>(0));
  }
  else if(block == "header_name") {
    string temp;
    inputLine >> temp;
    char buff[1000];
    inputLine.getline(buff, 1000);
    temp += buff;
    headerNames.push_back(temp);
  }
  else if(block == "sorting") {
    inputLine >> sorting;
    if(sorting == "increasing" || sorting == "decreasing")
      inputLine >> sortCol;
  }
  else if(block == "condition") {
    char buff[1000];
    inputLine.getline(buff, 1000);
    string temp = buff;
    temp += " ";
    Formula* newCond = new Formula(temp);

    //Only store valid conditions with boolean return values
    if(newCond -> CheckValidity() && newCond -> CheckBoolean()) 
      conditions.push_back(newCond);
    else delete newCond;
  }
  else if(block == "noconditions") conditions.resize(0);
  else {
    if(parseVerbosity > 0)
      cout << "Parse: Invalid option: " << block << endl;
    return 0;
  }

  return 1;
}

//Read data in from the input file
void Parse::Fill() {
  if(parseVerbosity > 0) cout << "Parse::Fill : Start" << endl;

  if(parseVerbosity > 1) this -> PrintSettings(cout);

  //Open the input file and prepare for read
  this -> OpenFile();

  numOpen++;
  if(parseVerbosity > 2)
    cout << "Total number of files opened: " << numOpen << endl;

  if(!readFlag) {
    if(parseVerbosity > 0) { 
      cout << "File: "<< file << " not read" << endl
           << "Parse::Fill : End" << endl;
      return;
    }
  }

  long int index;
  int test;
  vector<double> readValues, saveValues(storedValues.size());

  while(readFlag) {

    //Reset input array
    readValues.resize(0);

    //Produce an input line from the file
    test = this -> Read(readValues);

    //Apply equations and conditions or end loop
    if(test) test = Convert(readValues, saveValues); 
    else break;

    //Only record points which pass conditional statements
    if(test) {
      for(int i = 0; i < storedValues.size(); i++) {
	storedValues[i] -> push_back(saveValues[i]);
      }
    }
  }

  Sort();  //Sort stored values.  Sort condition is applied within

  if(parseVerbosity > 0) cout << "Parse::Fill : End" << endl;
}

//Sort data by the defined sorting order
//Sorting is performed by a bubble sort algorithm
void Parse::Sort() {

  //Do nothing if 'nosort' is specified
  if(sorting == "nosort" || sortCol <= 0
     || sortCol > storedValues.size()) return;

  //Sort in increasing order for data in column 'sortCol'
  else if(sorting == "increasing") {
    for(int i = 0; i < storedValues[0] -> size(); i++) {
      for(int j = i + 1; j < storedValues[0] -> size(); j++) {
	if((*storedValues[sortCol - 1])[j] <
	   (*storedValues[sortCol - 1])[i])
	  Swap(i, j);
      }
    }
  }

  //Sort in decreasing order for data in column 'sortCol'
  else if(sorting == "decreasing") {
    for(int i = 0; i < (*storedValues[0]).size(); i++) {
      for(int j = i + 1; j < (*storedValues[0]).size(); j++) {
	if((*storedValues[sortCol - 1])[j] > 
	   (*storedValues[sortCol - 1])[i])
	  Swap(i, j);
      }
    }
  }
  else cout << "Error: invalid sort option: " << sorting << endl;
}

//Swap the data in all arrays for two given indices
//Contains error checking for equal indices, empty data array, and
//index bounds.
void Parse::Swap(int index1, int index2) {
  if(index1 == index2 || storedValues.size() < 1) return;
  if(index1 < 0 || index2 < 0 || index1 >= storedValues[0] -> size()
     || index2 >= storedValues[0] -> size()) return;

  double temp;

  for(int i = 0; i < storedValues.size(); i++) {
    temp = (*storedValues[i])[index1];
    (*storedValues[i])[index1] = (*storedValues[i])[index2];
    (*storedValues[i])[index2] = temp;
  }
}

//Convert an input line into data values to save
//Applies equations on the input line and checks conditions
int Parse::Convert(vector<double> &readValues,
		   vector<double> &saveValues) {

  //Check conditional statements based on the inputs
  for(int i = 0; i < conditions.size(); i++) {
    if(conditions[i] -> Evaluate(readValues) == 0) {
      if(parseVerbosity > 1)
	cout << "Point failed condition #" << i << endl;
      return 0;
    }
  }

  //If all conditions are passed, evaluate the line using each formula
  for(int i = 0; i < saveValues.size(); i++) {
    saveValues[i] = inputFormulas[i] -> Evaluate(readValues);
  }

  if(parseVerbosity > 1) {
    cout << "Parse::Convert : ";
    for(int i = 0; i < saveValues.size(); i++)
      cout << saveValues[i] << "  ";
    cout << endl;
  }

  return 1;
}

//Get the number of rows of stored data.  Returns null if no columns exist
int Parse::GetNumRows() {
  if(storedValues.size() < 1) {
    return 0;
  }
  else {
    return storedValues[0] -> size();
  }
}

//Get a single column of stored data, indexed by 'index'
//Returns 1 if the column exists, 0 if it does not.
//If 0 is output an empty vector is generated for output
//The expected index is human readable (starts at 1).
int Parse::GetStoredColumn(int index, vector<double> &data) {
  if(index < 0 || index >= storedValues.size()) {
    data.resize(0);
    return 0;
  }
  else {
    data = *storedValues[index];
    return 1;
  }
}

//Get a single column of stored data, indexed by 'index'
//Returns 1 if the column exists, 0 if it does not.
//If 0 is output an empty vector is generated for output
//The expected index is human readable (starts at 1).
int Parse::GetStoredRow(int index, vector<double> &data) {
  if(index < 0 || index >= GetNumRows()) {
    data.resize(0);
    return 0;
  }
  else {
    data.resize(0);
    for(int i = 0; i < storedValues.size(); i++) {
      data.push_back((*storedValues[i])[index]);
    }
    return 1;
  }
}
