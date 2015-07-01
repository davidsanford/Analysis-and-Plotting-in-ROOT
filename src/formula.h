#ifndef FORMULA_H
#define FORMULA_H

#include<iostream>
#include<sstream>
#include<math.h>
#include<string>
#include<vector>
#include"limit.h"
#include"isospin.h"

using namespace std;

//Evaluates a formula (in postfix notation)
//There are four types of notation
class Formula {
 public:
  //The expression to be evaluated is set at time of declaration.
  //Formula() is null, but used for inheritance purposes.
  Formula();
  Formula(const char* expression);
  Formula(string expression);
  Formula(Formula &other);

  //Evaluate a single line
  virtual double Evaluate(vector<double> &line);
  int Validate(const char *);
  int CheckValidity() {return validity;}

  //Check whether a valid expression is boolean in nature
  int CheckBoolean() {return boolean && validity;}  

  //Get a string for the expression
  string GetExp() {return exp;}
  string ReproduceExp();

  //Output stream operator
  friend ostream &operator <<(ostream &out, Formula &form) {
    out << form.exp;
  }

 protected:
  string exp;                //Original expression stored for reference
  vector<string> parsedExp;  //Vector containing the parsed expression
  double precision;

  int validity;                 //Validity flag for the formula
  int boolean;                  //Flag indicating that the formula is
                                //a boolean expression

  //Initializes control strings 
  //This function leaves pure numbers and column references unaffected
  string InitializeOperand(string &s);

  //Grabs input vales from the passed vector
  //This function leaves pure numbers and previously converted control
  //codes unchanged
  int ReadLine(string s, vector<double> &line, double &value);

  vector<Limit*> limits;      //Vector containing limits for use by
                              //the LimitCheck functions

  vector<IsospinFactor*> factors;   //Vector containing isospin violation
                              //rescaling factors for  limits

  int isOperator(string &s);  //Check whether a string is an operator
  int isBinary(string &s);    //Check whether a string is a binary operator
  int isFunction(string &s);  //Check whether a string is a function
  int isBoolean(string &s);   //Check whether an operator is boolean
  int isLogic(string &s);     //Check whether an operator is logic function

  int OperatorOrder(string &s);   //Get the number of operands
                                  //operated on by an operator

  inline int isEqual(double v1, double v2);  //Equals operators for doubles
  inline int isNotEqual(double v1, double v2); //Not equal operators for doubles

  //Perform a binary operation given two values and an operator
  int EvaluateOperator(string &op, vector<double> &operands);

  double GetLimit(int index, double inputVal);
  double GetFactor(int index, double mass, double ratio);
  double DDFactor(double fnfp, double nUp, double nDown,
		  double Bup, double Bun);
  double chxx_SDFermion(double Mchi, double MS, double MD,
			double y, double tantheta);
  double CdCu(double fnfp, double Bup, double Bun);
};

#endif
