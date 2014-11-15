#include "provided.h"
#include <string>
#include <vector>
#include <stack>

using namespace std;

class RuleImpl
{
public:
    RuleImpl(string ruleText);
    string getName() const;
    double getDollarValue() const;
    int getNumElements() const;
    string getElement(int elementNum) const;
    string getAd() const;
    bool match(const Document& doc) const;
private:
	string m_rule;
	vector<string> m_element;
	int m_count;
	string m_ad;
	string m_val;
	bool improperForm;
};

RuleImpl::RuleImpl(string ruleText)
	: m_rule(ruleText), m_count(0), improperForm(false)
{
	//-----Break up rule elements into tokens and put them into the vector

	Tokenizer temp(m_rule, " "); //splits up words by spaces
	int count=0; 
	string token;
	while (temp.getNextToken(token))
	{
		if(token==">") //this is the dividing line between the elements and ad string; don't count it
			break;
		else
			count++;

		if(count==2)
			m_val=token; //store the dollar value as a string

		if(count>2) //only the third token and on are actually rule elements
		{
			for(size_t i=0; i<token.size(); i++)
				token[i]=tolower(token[i]); //make all tokens lowercase so they can match

			m_element.push_back(token); //if it's an element, push it back into the vector
		}
	}

	//-----Determine the count of elements and the ad message

	count-=2; //decrement 2 to account for rule name and value
	m_count=count;
	size_t adInd= m_rule.find(">"); //should never be npos
	adInd+=2; //increment by 2 to account for space
	m_ad=m_rule.substr(adInd, m_rule.length()-adInd);

	//-----Determine whether the rule elements are in proper postfix form
	
	int operandCount=0, operatorCount=0;
	for(int i=0; i<m_count; i++) //determine the number of operators and operands
	{
		if(m_element.at(i)=="|" || m_element.at(i)=="&") //if it's an operator, increment operatorCount
			operatorCount++;
		else
			operandCount++; //otherwise, increment operandCount
	}
	if(operandCount!=operatorCount+1) //if the operators are not exactly one less than the operands, it is not in postfix form
		improperForm=true;

	stack<string> s;

	for(int i=0; i<m_count; i++) //if at anytime the stack is empty where an operand should be, it is not in postfix form
	{
		string temp = getElement(i);

		if(temp!="|" && temp!="&") //if that element is not an operator
			s.push(temp);
		else //if that element is an operator; the stack should have two operands to apply the operator to
		{
			if(s.empty()) //missing the first operand, so improper!
			{
				improperForm=true;
				break;
			}
			string oper2=s.top(); 
			s.pop();

			if(s.empty()) //missing the second operand, so improper!
			{
				improperForm=true;
				break;
			}
			string oper1=s.top();
			s.pop();

			if(temp=="|" || temp=="&")
				s.push("test"); //arbitrary test string that will later be the result for true or false
		}
	}

	if(s.empty()) //there should be one answer left; if not, it's improper!
		improperForm=true;
	else
	{
		s.pop(); //after popping the answer, if there's yet another answer, it's improper!
		if(!s.empty())
			improperForm=true;
	}
}

string RuleImpl::getName() const
{
	//The format of the rule name is in: rule-########
	return m_rule.substr(0, 13);
}

double RuleImpl::getDollarValue() const
{
	/*
	int ind=14; //this is the index where the dollar value substring begins
	int count=0;
	while(m_rule[ind]!=' ')
	{
		ind++;
		count++;
	}

	string value=m_rule.substr(ind, count); //find the dollar value substring
	*/

    return atof(m_val.c_str()); //convert the string to a c string, then finally to a double
}

int RuleImpl::getNumElements() const
{
	if(improperForm) //improper rules have no elements according to the spec
		return 0;
    return m_count;
}

string RuleImpl::getElement(int elementNum) const
{
    if(elementNum<0 || elementNum>getNumElements()-1) //if out of bounds, return empty string
		return "";
	else
		return m_element.at(elementNum); //return the corresponding element
}

string RuleImpl::getAd() const
{
    return m_ad;
}

bool RuleImpl::match(const Document& doc) const
{
	if(improperForm) //a rule that is improper cannot match
		return false;

	stack<string> s;

	string sTrue = "nathantungtrue"; //this denotes true
	string sFalse = "nathantungfalse"; //this denotes false

	for(int i=0; i<getNumElements(); i++)
	{
		string temp = getElement(i);
	
		if(temp!="|" && temp!="&") //if that element is not an operator
			s.push(temp);
		else //if that element is an operator
		{
			string oper2=s.top();
			s.pop();
			string oper1=s.top();
			s.pop();

			if(temp=="|") //operator OR
			{
				if(oper1==sTrue || doc.contains(oper1)) //if either of these is true, push sTrue
					s.push(sTrue);
				else if(oper2==sTrue || doc.contains(oper2)) //if either of these is true, push sTrue
					s.push(sTrue);
				else
					s.push(sFalse); //none contained, so false
			}
			else if(temp=="&")
			{
				bool oper1True=(oper1==sTrue || doc.contains(oper1));
				bool oper2True=(oper2==sTrue || doc.contains(oper2));

				if(oper1True && oper2True) //if both of these are true, push sTrue
					s.push(sTrue);
				else
					s.push(sFalse); //not both contained, so false
			}
		}
	}

	return(s.top()==sTrue); //return whether the top of the stack is equal to true
}

//******************** Rule functions *******************************

// These functions simply delegate to RuleImpl's functions.
// You probably don't want to change any of this code.

Rule::Rule(string ruleText)
{
    m_impl = new RuleImpl(ruleText);
}

Rule::~Rule()
{
    delete m_impl;
}

Rule::Rule(const Rule& other)
 : m_impl(new RuleImpl(*other.m_impl))
{}

Rule& Rule::operator=(const Rule& other)
{
    if (this != &other)
    {
        RuleImpl* copyOfOther = new RuleImpl(*other.m_impl);
        delete m_impl;
        m_impl = copyOfOther;
    }
    return *this;
}

string Rule::getName() const
{
    return m_impl->getName();
}

double Rule::getDollarValue() const
{
    return m_impl->getDollarValue();
}

int Rule::getNumElements() const
{
    return m_impl->getNumElements();
}

string Rule::getElement(int elementNum) const
{
    return m_impl->getElement(elementNum);
}

string Rule::getAd() const
{
    return m_impl->getAd();
}

bool Rule::match(const Document& doc) const
{
    return m_impl->match(doc);
}
