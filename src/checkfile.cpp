#include"slharead.h"

using namespace std;

int main(int argc, char* argv[]) {
  if(argc < 3) {
    cout << "Expected: ./checkfile [param file] [parse file]";
    return 0;
  }

  ifstream fin(argv[1]);
  string filename = argv[2];
  SLHARead* reader = new SLHARead(fin, filename);

  cout << "Got Here" << endl;

  reader -> ReadAll();

  reader -> PrintHeader(cout);
  reader -> PrintAll(cout);

  return 1;
}
