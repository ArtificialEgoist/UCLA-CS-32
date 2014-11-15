#include "provided.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

class AdHunterImpl
{
public:
    AdHunterImpl(istream& ruleStream);
	~AdHunterImpl();
    void addSeedSite(string site);
    int getBestAdTargets(double minPrice, int pagesPerSite, vector<Match>& matches);
private:
	Matcher* matcher;
	vector<string> seedSites;
};

bool sortComp(const Match& one, const Match& two)
{
	if(one.dollarValue==two.dollarValue)
	{
		if(one.url==two.url)
			return(one.ruleName<two.ruleName);
		else
			return(one.url<two.url);
	}
	else
		return(one.dollarValue>two.dollarValue);
}

AdHunterImpl::AdHunterImpl(istream& ruleStream)
{
	matcher = new Matcher(ruleStream);
}

AdHunterImpl::~AdHunterImpl()
{
	delete matcher;
}

void AdHunterImpl::addSeedSite(string site)
{
	seedSites.push_back(site);
}

int AdHunterImpl::getBestAdTargets(double minPrice, int pagesPerSite, vector<Match>& matches)
{
	matches.clear(); //get rid of all ad matches prior to this method call
	for(int i=0; i<seedSites.size(); i++)
	{
		Crawler* crawler = new Crawler(seedSites.at(i)); //crawl for each seed site
		Document* temp;
		int count=0;

		do
		{
			temp=crawler->crawl();

			if(temp!=NULL) //as long as there are more documents made, keep crawling
				matcher->process(*temp, minPrice, matches);

			count++;
		}
		while(temp!=NULL && count<pagesPerSite);

		delete crawler; //free up the memory of the Crawler
	}

	//-----Sort matches
	sort(matches.begin(), matches.end(), sortComp);

    return matches.size();
}

//******************** AddHunter functions *******************************

// These functions simply delegate to AdHunterImpl's functions.
// You probably don't want to change any of this code.

AdHunter::AdHunter(istream& ruleStream)
{
    m_impl = new AdHunterImpl(ruleStream);
}

AdHunter::~AdHunter()
{
    delete m_impl;
}

void AdHunter::addSeedSite(string site)
{
    return m_impl->addSeedSite(site);
}

int AdHunter::getBestAdTargets(double minPrice, int pagesPerSite, vector<Match>& matches)
{
    return m_impl->getBestAdTargets(minPrice, pagesPerSite, matches);
}
