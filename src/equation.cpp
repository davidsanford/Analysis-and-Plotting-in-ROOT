#include"formula.h"

using namespace std;

int main(int argc, char* argv[]) {
  stringstream equation;
  for(int i = 1; i < argc; i++) {
    equation << argv[i] << " ";
  }

  Formula *form = new Formula(equation.str());

  vector<double> v(5);
  for(int i = 0; i < 5; i++) {
    v[i] = i + 1;
  }

  cout << form -> Evaluate(v) << "    ";

  return 1;
}
