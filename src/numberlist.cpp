#include<iostream>
#include<sstream>

using namespace std;

double pi = 3.14159265358979323;

int main(int argc, char * argv[]) {
  if(argc < 4) {
    cout << "Expected ./numberlist [start] [end] [numsteps]" << endl;
    return 0;
  }

  double start, end;
  long int steps;

  stringstream convert;
  for(int i = 1; i < argc; i++) {
    convert << argv[i] << " ";
  }
  convert >> start >> end >> steps;

  if(end < start) {
    double temp = start;
    start = end;
    end = temp;
  }

  double current = start;
  double delta = (end - start) / steps;

  for(long int i = 0; i <= steps; i++) {
    cout << current << endl;
    current += delta;
  }

  return 1;
}
