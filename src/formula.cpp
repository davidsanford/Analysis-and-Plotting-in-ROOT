#include"formula.h"

Formula::Formula() : precision(1e-12) {
  Validate("");
}

Formula::Formula(const char* expression) : precision(1e-12) {
  Validate(expression);
}

Formula::Formula(string expression) : precision(1e-12) {
  Validate(expression.c_str());
}

Formula::Formula(Formula &other) : precision(1e-12) {
  exp = other.exp;
  parsedExp = other.parsedExp;
}

//Parses the stored expression and saves it as a vector of individual
//strings.  Validate() returns 1 for a valid formula and 0 for an
//invalid one.  A valid expression contains one more total operand than
//operator, and always contains less operators than previous operants.
int Formula::Validate(const char* expression) {
  validity = 0;
  boolean = 0;
  exp = expression;
  parsedExp.resize(0);
  limits.resize(0);
  factors.resize(0);
  int numVal = 0, numOp = 0;
  string s;
  istringstream expIn(exp);

  expIn >> s;

  while(!expIn.eof()) {

    if(isOperator(s)) {

      //Push the operator onto the back of the stack
      parsedExp.push_back(s);

      //Count the number of boolean operators, and subtract off the
      //number of logic operators
      if(isBoolean(s)) boolean++;
      if(isLogic(s)) boolean--;

      numOp += OperatorOrder(s) - 1;

      //String is invalid if there are more operators than
      //operands at any point in the string
      if(numOp >= numVal) return 0;
    }
    else {
      parsedExp.push_back(InitializeOperand(s));
      numVal++;
    }

    expIn >> s;
  }

  //Check to ensure that the total of boolean order (zero or one)
  //makes sense
  if(boolean < 0 || boolean > 1) return 0;

  //String is valid if there is exactly one more operand than operator
  if(numVal == numOp + 1) {
    validity = 1;
    return 1;
  }
  else return 0;
}

//Evaluate the formula in postfix notation
//If any error is found, return the boolean flag (0 for non-boolean
//expression or 1 for any boolean expression).
double Formula::Evaluate(vector<double> &line) {
  if(validity) {
    limits.resize(0);
    factors.resize(0);
    vector<double> operands(0);

    for(int i = 0; i < parsedExp.size(); i++) {

      //For operator input, grab the last two elements of the array
      //act upon them with the operator, then add the result back onto
      //the end of the array.
      if(isOperator(parsedExp[i])) {
	if(EvaluateOperator(parsedExp[i], operands)) continue;
	else return boolean;
      }

      //For operand input, add it to the end of the vector
      else {
	double temp;
	if(ReadLine(parsedExp[i], line, temp))
	  operands.push_back(temp);
	else return boolean;
      }
    }

    //Return the first (and only) element of the operand array
    return operands[0];
  }
  else return boolean;
}

//Reproduce the formula from the stored vector
string Formula::ReproduceExp() {
  stringstream expression;
  for(int i = 0; i < parsedExp.size(); i++) {
    expression << parsedExp[i] << " ";
  }

  return expression.str();
}

//Initializes control strings 
string Formula::InitializeOperand(string &s) {
  if(s[0] != '%') return s;  //Ignore any operand not flagged as a
			     //control string

  stringstream code;

  //Add another limit and return a control code corresponding to
  //the location in the limit array
  if(s[1] == 'L') {
    s.erase(0, 3);
    limits.push_back(new Limit(s));
    code << limits.size() - 1;
  }

  //Add another isospin rescaling factor and return a control code
  //corresponding to the location in the factor array
  else if(s[1] == 'F') {
    s.erase(0, 3);
    factors.push_back(new IsospinFactor(s));
    code << factors.size() - 1;
  }

  //Convert operators intended to be passed as operands (%>, %<, %=) 
  //to integer input codes
  //else if(s[1] == '>') code << "1";
  //else if(s[1] == '<') code << "-1";
  //else if(s[1] == '=') code << "0";

  else return "0";    //Default to zero for unknown string

  return code.str();
}

//Produce numerical operands
//Numerical operand is output in the 'value' reference variable; the
//return value is 1 if a valid value was found and 0 if not
int Formula::ReadLine(string s, vector<double> &line, double &value) {
  istringstream numIn;

  //If the number is preceded by '#', grab the value in that column
  if(s[0] == '#') {
    int colIndex;
    s.erase(0, 1);
    numIn.str(s);
    numIn >> colIndex;
    colIndex--;      //Convert human-readable index to c++ notation
    if(colIndex >= 0 && colIndex < line.size()) {
      value = line[colIndex];
      return 1;
    }
    else {
      value = 0;
      //cout << "Column #" << colIndex + 1 << " : Out of range" << endl;
      return 0;  //Return false if column index was out of bounds
    }
  }

  //When a number is really just a number
  else {
    numIn.str(s);
    numIn >> value;
    return 1;
  }
}

//Check whether a string is any operator, either a binary operator or
//a hard-coded function
int Formula::isOperator(string &s) {
  return (isBinary(s) || isFunction(s));
}

//Check if the string is a binary operator
//Arithmetic and exponentiation operators are supported
int Formula::isBinary(string &s) {
  if(s == "+" || s == "-" || s == "*" || s == "/" || s == "^" || 
     s == "=" || s == ">" || s == "<" || s == "X" || s == "and" ||
     s == "or" || s == "!=") return 1;
  else return 0;
}

//Checks whether a string corresponds to a hard-coded function
int Formula::isFunction(string &s) {
  if(s == "isospinfactor" || s == "limit" || s == "max" || s == "min" ||
     s == "abs" || s == "tan" || s == "arctan" || s == "cos" ||
     s == "arccos" || s == "sin" || s == "arcsin" ||
     s == "ddfactor" || s == "redmass" || s == "cdcu" ||
     s == "chxx_sdfermion") return 1;
  else return 0;
}

//Check if an operator is a boolean operator
int Formula::isBoolean(string &s) {
  if(!isOperator(s)) return 0;  //Double-check operator status
  if(s == "=" || s == "!=" || s == ">" || s == "<") return 1;
  else return 0;
}

//Check if an operator is a boolean operator
int Formula::isLogic(string &s) {
  if(!isOperator(s)) return 0;  //Double-check operator status
  if(s == "and" || s == "or") return 1;
  else return 0;
}

//Get the number of operands operated on by an operator
int Formula::OperatorOrder(string &s) {
  if(isBinary(s)) return 2;
  else if(s == "isospinfactor") return 3;
  else if(s == "chxx_sdfermion") return 5;
  else if(s == "ddfactor") return 5;
  else if(s == "limit") return 2;
  else if(s == "max") return 2;
  else if(s == "min") return 2;
  else if(s == "abs") return 1;
  else if(s == "redmass") return 2;
  else if(s == "cdcu") return 3;
  else if(s == "tan" || s == "arctan" || s == "cos" ||
	  s == "arccos"|| s == "sin" || s == "arcsin") return 1;
  else return -1;
}

//Evaluate a binary operation with the given operator and two values
//Returns 1 for successfully located operator and zero for invalid input
int Formula::EvaluateOperator(string &op, vector<double> &operands) {

  //Binary operations
  if(isBinary(op)) {
    double v1, v2;
    v2 = operands.back();
    operands.pop_back();
    v1 = operands.back();
    operands.pop_back();

    //Arithmetic operations
    if(op == "+") operands.push_back(v1 + v2);
    else if(op == "-") operands.push_back(v1 - v2);
    else if(op == "*" || op == "X") operands.push_back(v1 * v2);
    else if(op == "/") operands.push_back(v1 / v2);
    else if(op == "^") operands.push_back(pow(v1, v2));

    //Boolean operations
    else if(op == "=") {
      if(isEqual(v1, v2)) operands.push_back(1);
      else operands.push_back(0);
    }
    else if(op == "!=") {
      if(isNotEqual(v1, v2)) operands.push_back(1);
      else operands.push_back(0);
    }
    else if(op == ">") {
      if(v1 > v2) operands.push_back(1);
      else operands.push_back(0);
    }
    else if(op == "<") {
      if(v1 < v2) operands.push_back(1);
      else operands.push_back(0);
    }
    else if(op == "and") {
      if(v1 > 0 && v2 > 0) operands.push_back(1);
      else operands.push_back(0);
    }
    else if(op == "or") {
      if(v1 > 0 || v2 > 0) operands.push_back(1);
      else operands.push_back(0);
    }

    return 1;
  }

  //Get the value of a limit
  else if(op == "limit") {
    double val = operands.back();
    operands.pop_back();
    int index = int(operands.back());
    operands.pop_back();
    operands.push_back(GetLimit(index, val));

    return 1;
  }

  //Calculate the value of the Higgs coupling for sdfermion case
  else if(op == "chxx_sdfermion") {
    double tantheta = operands.back();
    operands.pop_back();
    double y = operands.back();
    operands.pop_back();
    double MD = operands.back();
    operands.pop_back();
    double MS = operands.back();
    operands.pop_back();
    double Mchi = operands.back();
    operands.pop_back();
    operands.push_back(chxx_SDFermion(Mchi, MS, MD, y, tantheta));

    return 1;
  }

  else if(op == "isospinfactor") {
    double ratio = operands.back();
    operands.pop_back();
    double mass = operands.back();
    operands.pop_back();
    int index = int(operands.back());
    operands.pop_back();
    operands.push_back(GetFactor(index, mass, ratio));

    return 1;
  }

  else if(op == "max") {
    double v1, v2;
    v2 = operands.back();
    operands.pop_back();
    v1 = operands.back();
    operands.pop_back();

    if(v1 > v2) operands.push_back(v1);
    else operands.push_back(v2);

    return 1;
  }

  else if(op == "min") {
    double v1, v2;
    v2 = operands.back();
    operands.pop_back();
    v1 = operands.back();
    operands.pop_back();

    if(v1 < v2) operands.push_back(v1);
    else operands.push_back(v2);

    return 1;
  }

  else if(op == "abs") {
    double v1 = operands.back();
    operands.pop_back();
    operands.push_back(fabs(v1));

    return 1;
  }

  else if(op == "sin") {
    double v1 = operands.back();
    operands.pop_back();
    operands.push_back(sin(v1));

    return 1;
  }

  else if(op == "cos") {
    double v1 = operands.back();
    operands.pop_back();
    operands.push_back(cos(v1));

    return 1;
  }

  else if(op == "tan") {
    double v1 = operands.back();
    operands.pop_back();
    operands.push_back(tan(v1));

    return 1;
  }

  else if(op == "arcsin") {
    double v1 = operands.back();
    operands.pop_back();
    operands.push_back(asin(v1));

    return 1;
  }

  else if(op == "arccos") {
    double v1 = operands.back();
    operands.pop_back();
    operands.push_back(acos(v1));

    return 1;
  }

  else if(op == "arctan") {
    double v1 = operands.back();
    operands.pop_back();
    operands.push_back(atan(v1));

    return 1;
  }

  else if(op == "ddfactor") {
    vector<double> v(5);
    for(int i = 4; i >= 0; i--) {
      v[i] = operands.back();
      operands.pop_back();
    }
    double factor = DDFactor(v[0], v[1], v[2], v[3], v[4]);
    operands.push_back(factor);
    return 1;
  }

  else if(op == "redmass") {
    double v1, v2;
    v2 = operands.back();
    operands.pop_back();
    v1 = operands.back();
    operands.pop_back();
    operands.push_back(v1 * v2 / (v1 + v2));

    return 1;
  }

  else if(op == "redmass") {
    double v1, v2, v3;
    v2 = operands.back();
    operands.pop_back();
    v1 = operands.back();
    operands.pop_back();
    v3 = operands.back();
    operands.pop_back();
    operands.push_back(CdCu(v1, v2, v3));

    return 1;
  }

  return 0;
}

//Get the value of a limit for a given value.
//Index is a code used to select the proper limit which is assigned by
//the ConvertOperators function.
double Formula::GetLimit(int index, double inputVal) {
  return limits[index] -> Evaluate(inputVal);
}

//Get the rescaling factor for a given value.
//Index is a code used to select the proper factor which is assigned
//by the ConvertOperators function.
double Formula::GetFactor(int index, double mass, double ratio) {
  return factors[index] -> RescaleFactor(mass, ratio);
}

//Equals operator for floats and double; applies the given precision
//and error-checking for zero values.  Note this may give erroneous
//output for one identically zero value paired with a value smaller
//than precision.
inline int Formula::isEqual(double v1, double v2) {
  if(v1 == v2) return 1;
  else if(v1 == 0 && fabs(v2) < precision) return 1;
  else if(v2 == 0 && fabs(v1) < precision) return 1;
  else if(fabs((v1 - v2) / v1) < precision) return 1;
  else return 0;
}

//Not equal operator for floats and double; applies the given precision
//and error-checking for zero values.  Note this may give erroneous
//output for one identically zero value paired with a value smaller
//than precision.
inline int Formula::isNotEqual(double v1, double v2) {
  if(v1 == v2) return 0;
  else if(v1 == 0 && fabs(v2) < precision) return 0;
  else if(v2 == 0 && fabs(v1) < precision) return 0;
  else if(fabs((v1 - v2) / v1) < precision) return 0;
  else return 1;
}

double Formula::DDFactor(double fnfp, double nUp, double nDown,
			       double Bup, double Bun) {
  double factor = pow( Bup + Bun * CdCu(fnfp, Bup, Bun) , 2);
  factor /= nUp + pow( CdCu(fnfp, Bup, Bun), 2) * nDown;
  return factor;
}

double Formula::chxx_SDFermion(double Mchi, double MS, double MD,
			       double y, double tantheta) {
  double vev = 246;
  double chxx = - y * y * vev * (Mchi + MD * sin(2 * atan(tantheta)));
  chxx /= (MD * MD + 2 * MS * Mchi - 3 * Mchi * Mchi
	   + y * y * vev * vev / 2);
  return chxx;
}

double Formula::CdCu(double fnfp, double Bup, double Bun) {
  return (Bup * fnfp - Bun) / (Bup - Bun * fnfp);
}
