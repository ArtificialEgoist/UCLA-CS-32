#include "provided.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class MatcherImpl
{
public:
    MatcherImpl(istream& ruleStream);
	~MatcherImpl();
    void process(Document& doc, double minPrice, vector<Match>& matches) const;
private:
	vector<Rule*> m_rules;
};

MatcherImpl::MatcherImpl(istream& ruleStream)
{
	string line;
	while (getline(ruleStream, line)) //while there's a line in ruleStream
	{
	    Rule* tempRule = new Rule(line); //construct its rule and push it into the rule vector
		m_rules.push_back(tempRule);
	}
}

MatcherImpl::~MatcherImpl()
{
	for(int i=0; i<m_rules.size(); i++)
		delete m_rules.at(i);
}

void MatcherImpl::process(Document& doc, double minPrice, vector<Match>& matches) const
{
	vector<Rule*> applicableRules; //vector to hold rules that are applicable

	for(int j=0; j<m_rules.size(); j++) //for every rule, check if it's applicable
	{ 
		Rule* temp = m_rules.at(j);

		//cerr << "Number of elements in this rule: " << temp->getNumElements() << endl;

		for(int k=0; k<temp->getNumElements(); k++)
		{
			if(doc.contains(temp->getElement(k))) //if one of the words is contained, it's applicable
			{
				applicableRules.push_back(temp); //push it into the vector and break out to avoid repeat rules
				break;
			}
		}
	}

	// applicableRules now holds pointers only to rules that have at least
	// one keyword that is also found in doc.

	for(int i=0; i<applicableRules.size(); i++)
	{
		Rule* tempRule = applicableRules.at(i);
		if(tempRule->match(doc) && tempRule->getDollarValue()>=minPrice) //The rule and document match together!
		{
			Match temp; //Construct the match and assign its variables accordingly
			temp.url=doc.getURL();
			temp.dollarValue=tempRule->getDollarValue();
			temp.ruleName=tempRule->getName();
			temp.ad=tempRule->getAd();

			matches.push_back(temp); //Put that object into the vector
		}
	}
}

//******************** Matcher functions *******************************

// These functions simply delegate to MatcherImpl's functions.
// You probably don't want to change any of this code.

Matcher::Matcher(istream& ruleStream)
{
    m_impl = new MatcherImpl(ruleStream);
}

Matcher::~Matcher()
{
    delete m_impl;
}

void Matcher::process(Document& doc, double minPrice, vector<Match>& matches) const
{
    m_impl->process(doc, minPrice, matches);
}
