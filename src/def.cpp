#include"def.h"

Def::Def() : storedValues(0) {}

Def::Def(istream &in) : storedValues(0) {
  Initialize(in);
}

//Copy constructor
Def::Def(Def &other) {}

void Def::Initialize(istream &in) {
  char buff[1000];

  //Load Default Values
  this -> LoadDefaults();

  //Parse input stream for variable definitions.  Reads in input until
  //the end of the stream or a '!' character is encountered at the
  //start of a string.  Skips lines starting in '#'
  do {
    in.getline(buff, 1000);
    if(buff[0] == '#') continue;   //Skip lines starting in #
    else if(buff[0] == '!') break; // ! serves as a break character 
    else this -> Define(buff);     //Define function of inherited class
  }
  while(!in.eof());
}

//Given a converter functions for the real Define functions
int Def::Define(const char *buff) {
  stringstream inputLine;
  inputLine << buff;
  return this -> Define(inputLine);
}

int Def::Define(const string buff) {
  stringstream inputLine;
  inputLine << buff;
  return this -> Define(inputLine);
}


// ######################################################################

double sqr(double x) {
  return x * x;
}

int CheckFileExistence(string filename) {
  struct stat buffer;
  if(stat(filename.c_str(), &buffer) == 0) return 1;
  return 0;
}
