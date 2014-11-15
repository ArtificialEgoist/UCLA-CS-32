#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;


bool isGreaterOrEqualPrecedence(char me, char other);
//check if OTHER operator is at least equal or higher precedence than ME operator
bool isInfix(string expression);
//return true only if the expression string is in infix notation
bool allLettersInMap(string expression, const Map& values);
//return true only if all letters are found in the Map
int calculatePostfix(string postfix, const Map& values);
//return the calculated value of the postfix string (assumes string is in right format)
string infixToPostfix(string infix);
//return the postfix version of the infix input string
bool dividesByZero(string postfix, const Map& values);
//return true if postfix string tries to divide by zero


int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	//cerr << "The infix expression is: " << infix << endl;

	if(!isInfix(infix))
		return 1; //infix string is not in the right format

	postfix=infixToPostfix(infix); //convert the infix string to postfix string
	
	//cerr << "The postfix expression is: " << postfix << endl;

	if(!allLettersInMap(infix, values))
		return 2; //not all letter variables map to a number

	if(dividesByZero(postfix, values))
		return 3; //expression tries to divide by zero

	result=calculatePostfix(postfix, values);

	return 0; //evaluate worked correctly and result was changed

}

bool isGreaterOrEqualPrecedence(char me, char other) //check if OTHER operator is at least equal or higher precedence than ME operator
{
	if(me=='*' || me=='/')
		if(other=='+' || other=='-')
			return false; //only false if you are popping a lower precedence operator when the last one is *  or /
	
	return true;
}



bool isInfix(string expression)
{
	char acceptableOperands[7] = {'-', '+', '*', '/', '(', ')', ' '}; //only characters that can be used apart from lowercase letters

	if(expression=="") //if no chars, return false
		return false;

	for(int i=0; i<expression.size(); i++)
	{
		if(isalpha(expression[i]) && !islower(expression[i])) //if any letter is not lowercase, return false
			return false;
		if(!isalpha(expression[i])) //if character is not a letter
		{
			bool oneMatch=false;

			for(int k=0; k<7; k++)
				if(expression[i]==acceptableOperands[k])
					oneMatch=true; //as long as it matches one char in the acceptableOperands array, don't return false

			if(oneMatch==false)
				return false; //any unacceptable characters will return false
		}
	}

	int alphaCount=0;
	int operatorCount=0;
	
	int leftPar=0;
	int rightPar=0;

	for(int i=0; i<expression.size(); i++)
	{
		if(isalpha(expression[i]) && islower(expression[i]))
			alphaCount++;
		if(expression[i]=='+' || expression[i]=='-' || expression[i]=='*' || expression[i]=='/')
			operatorCount++; //not counting parenthesis '(' and ')'
		if(expression[i]=='(')
			leftPar++;
		if(expression[i]==')')
			rightPar++;
	}

	if(alphaCount-operatorCount!=1) //there should always be one more lowercase letter than operator
		return false;

	if(leftPar!=rightPar) //parenthesis must be even in pairs and match
		return false;

	return true;
}

bool allLettersInMap(string expression, const Map& values)
{
	for(int i=0; i<expression.size(); i++)
	{
		char temp = expression[i];
		if(isalpha(temp) && islower(temp) && !values.contains(temp)) //if for any lowercase letter, it's not in Map, return false
			return false;
	}
	return true;
}

int calculatePostfix(string postfix, const Map& values)
{
	if(postfix.size()==1) //if there are no operators, simply return the value that it maps to
	{
		int tempValue=0;
		values.get(postfix[0], tempValue);
		return tempValue;
	}

	stack<int> cStack; //create an integer stack for piling on operands

	for(int i=0; i<postfix.size(); i++)
	{
		char token = postfix[i];

		if(isalpha(token) && islower(token)) //if the token is a variable
		{

			int tempValue=0;
			values.get(token, tempValue);
			cStack.push(tempValue); //push in the integer that the variable maps to
		}
		else //token is an operator
		{
			int operand2=cStack.top();
			cStack.pop();
			int operand1=cStack.top();
			cStack.pop();

			int tempAns=0;
			
			switch(token)
			{
				case '-':
					tempAns=operand1-operand2;
					break;
				case '+':
					tempAns=operand1+operand2;
					break;
				case '*':
					tempAns=operand1*operand2;
					break;
				case '/':
					tempAns=operand1/operand2;
					break;
			}
			cStack.push(tempAns);
		}
	}

		//cerr << "The integer answer is: " << int(cStack.top()) << endl;

		return int(cStack.top());
}

string infixToPostfix(string infix)
{
	string postfix=""; //clear postfix string just in case

	stack<char> cStack;

	for(int i=0; i<infix.size(); i++) //for each character in infix string
	{
		if(isalpha(infix[i]) && islower(infix[i]))
			postfix += infix[i];
		else if(infix[i]=='(')
			cStack.push(infix[i]); //push it into stack
		else if(infix[i]==')')
		{
			while(!cStack.empty() && cStack.top()!='(') //as long as stack isn't empty and isn't '('
			{
				postfix+=cStack.top();
				cStack.pop(); //pop all operators between parenthesis
			}
			cStack.pop(); //remove the '('
		}
		else if(infix[i]=='*' || infix[i]=='+' || infix[i]=='-' || infix[i]=='/')
		{
			while(!cStack.empty() && cStack.top()!='(' && isGreaterOrEqualPrecedence(infix[i], cStack.top()))
			{
				postfix+=cStack.top(); //pop off operators when conditions are met
				cStack.pop();
			}
			cStack.push(infix[i]); //push in new operator
		}
	}

	while(!cStack.empty()) //pop the remaining operators into the postfix string
	{
		postfix+=cStack.top();
		cStack.pop();
	}

	return postfix;
}

bool dividesByZero(string postfix, const Map& values) //does the same as calculatePostfix without producing an int result
{
	if(postfix.size()==1) //if there are no operators, simply return the value that it maps to
		return false;

	stack<int> cStack; //create an integer stack for piling on operands

	for(int i=0; i<postfix.size(); i++)
	{
		char token = postfix[i];

		if(isalpha(token) && islower(token))
		{
			int tempValue=0;
			values.get(token, tempValue);
			cStack.push(tempValue); //push in the integer that the variable maps to
		}
		else //char is an operator
		{
			int operand2=cStack.top();
			cStack.pop();
			int operand1=cStack.top();
			cStack.pop();

			int tempAns=0;
			
			switch(token)
			{
				case '-':
					tempAns=operand1-operand2;
					break;
				case '+':
					tempAns=operand1+operand2;
					break;
				case '*':
					tempAns=operand1*operand2;
					break;
				case '/':
					if(operand2==0)
						return true;
					else
						tempAns=operand1/operand2;
					break;
			}
			cStack.push(tempAns);
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////

//According to the specs, "each source file you turn in may or may not contain a main routine; we'd prefer that it doesn't."
//Thus the tester main routine has been commented out


/*
int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = {  3,  -9,   6,   2,   4,   1  };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
	m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0  &&
	                        pf == "ae+"  &&  answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
	    // unary minus not allowed:
	assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2  &&
	                        pf == "ab*"  &&  answer == 999);
	assert(evaluate("y+o*(a-u) ", m, pf, answer) == 0  &&
	                        pf == "yoau-*+"  &&  answer == -1);
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                                pf == "oyy-/"  &&  answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0  &&
	                        pf == "a"  &&  answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0  &&
	                        pf == "a"  &&  answer == 3);
	cout << "Passed all tests" << endl;

	//My tests are below

	string pf2;
	int answer2=-123;

	assert(evaluate("a+i+u", m, pf2, answer2)==0 && pf2=="ai+u+" && answer2==13);

	assert(evaluate("a+(i+u)", m, pf2, answer2)==0 && answer2==13);
	assert(evaluate("(a+(i+u))", m, pf2, answer2)==0 && answer2==13);
	assert(evaluate("((a)+(i+u))", m, pf2, answer2)==0 && answer2==13);

	assert(evaluate("a+(i+u)", m, pf2, answer2)==0 && pf2=="aiu++" && answer2==13);

	assert(evaluate("a/e+y+o+i", m, pf2, answer2)==0 && answer2==9 && pf2=="ae/y+o+i+");

	assert(evaluate("a/(e+y+o+i)", m, pf2, answer2)==3 && answer2==9 && pf2=="aey+o+i+/"); //divide by zero
	assert(evaluate("a /  ( e + y + o+ i  )   ", m, pf2, answer2)==3 && answer2==9 && pf2=="aey+o+i+/"); //divide by zero


	assert(evaluate("asdf", m, pf2, answer2)==1);
	assert(evaluate("a", m, pf2, answer2)==0 && pf2=="a" && answer2==3);

	assert(evaluate("(((((a)))))", m, pf2, answer2)==0 && pf2=="a" && answer2==3);

	assert(evaluate("f", m, pf2, answer2)==2);
	assert(evaluate("(a)+z", m, pf2, answer2)==2);
	assert(evaluate("D", m, pf2, answer2)==1);
	assert(evaluate("a+D", m, pf2, answer2)==1);
	assert(evaluate("a+d", m, pf2, answer2)==2);

	cout << "No problems thus far." << endl;
}
*/