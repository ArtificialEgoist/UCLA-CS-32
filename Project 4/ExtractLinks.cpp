#include "provided.h"
#include <string>

using namespace std;

class ExtractLinksImpl
{
public:
    ExtractLinksImpl(const string& pageContents);
    bool getNextLink(string& link);
private:
	string m_pageContents;
	string m_pageContentsLowercase;
};

ExtractLinksImpl::ExtractLinksImpl(const string& pageContents)
	: m_pageContents(pageContents), m_pageContentsLowercase(pageContents)
{
	for(int i=0; i<m_pageContentsLowercase.length(); i++)
		m_pageContentsLowercase[i]=tolower(m_pageContentsLowercase[i]);
	//Don't make the pageContents all lowercase only, since links are case-sensitive
}

bool ExtractLinksImpl::getNextLink(string& link)
{
	size_t ind=m_pageContentsLowercase.find("<a href=\""); //check regardless of capitalization, so we use the lowercase page contents
	if(ind==string::npos) //the URL tag could not be found, so there's none; return false
		return false;

	ind+=9; //this updates ind to be the first index of the URL

	size_t endInd=m_pageContents.find("\"", ind); //finds the end quotation of the URL after the opening tag
	if(endInd==string::npos)
		return false;

	int urlSize=endInd-ind;
	link=m_pageContents.substr(ind, urlSize); //link is set to the first link substring found
	m_pageContents=m_pageContents.substr(endInd); //update page contents to eliminate the link already found
	m_pageContentsLowercase=m_pageContentsLowercase.substr(endInd); //lowercase page contents holds the same without capitalization

	return true;
}

//******************** ExtractLink functions *******************************

// These functions simply delegate to ExtractLinksImpl's functions.
// You probably don't want to change any of this code.

ExtractLinks::ExtractLinks(const string& pageContents)
{
    m_impl = new ExtractLinksImpl(pageContents);
}

ExtractLinks::~ExtractLinks()
{
    delete m_impl;
}

bool ExtractLinks::getNextLink(string& link)
{
    return m_impl->getNextLink(link);
}
