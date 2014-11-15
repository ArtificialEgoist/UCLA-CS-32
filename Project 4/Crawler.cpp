#include "provided.h"
#include "http.h"
#include <string>
#include <queue>

using namespace std;

class CrawlerImpl
{
public:
    CrawlerImpl(string seedSite);
	~CrawlerImpl();
    Document* crawl();
private:
	string m_seedSite;
	queue<string> allSites;
	bool sameDomain(string seed, string other);
};

CrawlerImpl::CrawlerImpl(string seedSite)
	: m_seedSite(seedSite)
{
	//for(size_t i=0; i<m_seedSite.length(); i++)
		//m_seedSite[i]=tolower(m_seedSite[i]);
	
	//Don't make the seed site all lowercase only, as it should be case-sensitive
	allSites.push(m_seedSite);
}

CrawlerImpl::~CrawlerImpl()
{
	//No dynamically-allocated memory to remove
}

Document* CrawlerImpl::crawl()
{
	if(allSites.empty()) //no more sites to crawl!
		return NULL;

	string tempSite = allSites.front(); //get the next site to crawl, then remove it from queue
	allSites.pop();

	string text;
	if(HTTP().get(tempSite, text)) //the site's text was fetched successfully
	{
		ExtractLinks* extractor = new ExtractLinks(text);
		string newLink;
		while(extractor->getNextLink(newLink)) //extract all the links from text
		{
			if(sameDomain(m_seedSite, newLink)) //if it is from the same domain, add it into the vector
				allSites.push(newLink);
		}

		delete extractor; //free up memory of the ExtractLinks
		
		Document* ans = new Document(tempSite, text); //generate a Document pointer for the current site just removed from the vector

		return ans;
	}
	else if(!allSites.empty()) //if the last page could not be accessed but there are still other sites in the vector
		return crawl(); //call the method again until something is returned (whether it's another Document or a NULL)

	return NULL; //if the last page was broken or inaccessible and the vector is empty, return NULL
}

bool CrawlerImpl::sameDomain(string seed, string other)
{
	//-----Determine whether the two sites have protocol:// in the front
	size_t seedProtocol = seed.find("://");
	size_t otherProtocol = other.find("://");
	string seedDomain, otherDomain;

	//-----Discard the protocol:// from the front of both sites if applicable
	int seedFront=0;
	if(seedProtocol!=string::npos) //the seed site has a front protocol with slashes
		seedFront=seedProtocol+3;
	seedDomain=seed.substr(seedFront); //if no :// was found, the string doesn't change

	int otherFront=0;
	if(otherProtocol!=string::npos) //the other site has a front protocol with slashes
		otherFront=otherProtocol+3;
	otherDomain=other.substr(otherFront); //if no :// was found, the string doesn't change

	//-----Determine whether the two sites have another / after the protocol
	size_t seedSlash = seedDomain.find("/");
	size_t otherSlash = otherDomain.find("/");

	//-----Discard everything from and after the / from the end of both sites if applicable
	int seedEnd=seedDomain.size();
	if(seedSlash!=string::npos) //the seed site has another slash
		seedEnd=seedSlash;
	seedDomain=seedDomain.substr(0, seedEnd);

	int otherEnd=otherDomain.size();
	if(otherSlash!=string::npos) //the other site has another slash
		otherEnd=otherSlash;
	otherDomain=otherDomain.substr(0, otherEnd);

	//-----Compare seedDomain and otherDomain
	//Since other has to be a subdomain of seed, the entire contents of seed must be contained in other
	size_t subDomain=other.find(seed);

	return(subDomain!=string::npos); //if the index is found, then we return true
}

//******************** Crawler functions *******************************

// These functions simply delegate to CrawlerImpl's functions.
// You probably don't want to change any of this code.

Crawler::Crawler(string seedSite)
{
    m_impl = new CrawlerImpl(seedSite);
}

Crawler::~Crawler()
{
    delete m_impl;
}

Document* Crawler::crawl()
{
    return m_impl->crawl();
}
