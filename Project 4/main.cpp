#include "MyHashMap.h"
#include "provided.h"
#include "http.h"
#include "string"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc!=5)
	{
		cout << "Usage: proj4.exe rulesFile seedSiteFile maxPages minPrice" << endl;
		exit(1);
	}

	string rules=argv[1];
	string sites=argv[2];
	int maxNum=atoi(argv[3]);
	double price=atof(argv[4]);

	ifstream infileRules(rules);
	if (!infileRules)
	{
	    cout << "Error: cannot open advertisement rules file!" << endl;
		exit(1);
	}

	ifstream infileSites(sites);
	if (!infileSites)
	{
	    cout << "Error: cannot open seed sites file!" << endl;
		exit(1);
	}

	AdHunter* adh = new AdHunter(infileRules);

	string s;
	while (getline(infileSites, s))
	    adh->addSeedSite(s);

	cout << "Crawling..." << endl << endl;

	vector<Match> match;
	int numMatches=adh->getBestAdTargets(price, maxNum, match);

	cout.setf(ios::fixed);
	cout.precision(2);

	cout << "There were " << numMatches << " matches found that met the minimum price of $" << price << " per ad:" << endl << endl;

	for(int i=0; i<match.size(); i++)
	{
		Match temp = match.at(i);
		cout << "Match " << i << ": " << temp.ruleName << " triggered on " << temp.url << " for a value of $" << temp.dollarValue << endl;
		cout << "\t" << "Advertisement: " << temp.ad << endl;
	}

	
	//TEST CASES BELOW:

	/*
	MyHashMap<bool> map;
	assert(map.find("stupid")==NULL); //assert(*map.find("stupid")==NULL); will result in error due to NULL dereferencing
	map.associate("blah", true); //try inserting an arbitrary key
	map.associate("asldjfkla", true); //try inserting a second arbitrary key
	map.associate("asdf", true); //try inserting a third arbitrary key

	assert(*map.find("asdf")==true); //fetch and verify the value of a key
	map.associate("asdf", false); //try changing the value of a key
	assert(*map.find("asdf")==false); //fetch and verify the value changed

	map.associate("zzzz", true); //insert key with same code as below, 1220
	map.associate("dPPPP", false); //insert key with same code as above, 1220
	assert(*map.find("zzzz")==true);
	assert(*map.find("dPPPP")==false); // verify their values

	map.associate("zzzz", false); 
	map.associate("dPPPP", true); //change the values of the two keys
	assert(*map.find("zzzz")==false);
	assert(*map.find("dPPPP")==true); //see if their corresponding values changed

	map.associate("PZPPP", true); //insert key with same code as zzzz and dPPPP (1220)
	assert(map.numItems()==6); //verify number of items

	MyHashMap<int> map2;
	map2.associate("one", 1);
	string tKey;
	assert(*map2.getFirst(tKey)==1 && tKey=="one"); //check if getFirst and key correspond
	assert(map2.getNext(tKey)==NULL); //make sure a NULL is returned if no more elements
	assert(map2.numItems()==1); //verify number of items
	*/

	/*
	string text;
	int count=0;
	if(HTTP().get("www.google.com", text))
	{
		ExtractLinks* e = new ExtractLinks(text);
		string newLink;
		while(e->getNextLink(newLink)) //extract all the links from text
		{
			cerr << newLink << endl; //manually check if the links match
			count++; //verify number of links found
		}
		delete e;
		cerr << count << " links were extracted!" << endl;
	}

	if(HTTP().get("www.yahoo.com", text)) //yahoo has several URLs that are literally #
	{
		ExtractLinks* e = new ExtractLinks(text);
		string newLink;
		while(e->getNextLink(newLink)) //extract all the links from text
		{
			cerr << newLink << endl; //manually check if the links match
			count++; //verify number of links found
		}
		delete e;
		cerr << count << " links were extracted!" << endl;
	}
	*/

	/*
	string text;
	int count=0;

	if(HTTP().get("www.google.com", text)) //check if a text is fetched
	{
		Document* d = new Document("www.google.com", text); //try to construct Document
		assert(d->getURL()=="www.google.com"); //verify that the saved URLs match
		
		string aWord;
		if(d->getFirstWord(aWord)) //try to get Document's first word
		{
			count++;
			cerr << aWord << endl; //output that word and manually check it
		}

		while(d->getNextWord(aWord)) //get the rest of Document's words
		{
			count++;
			cerr << aWord << endl; //output all of the words to check
		}
		cerr << count << " words were looked over!" << endl; //show number of words displayed
	}
	*/

	/*
	int count=1;
	Crawler* c = new Crawler("www.google.com"); //construct a Crawler on Google
	Document* d = c->crawl(); //Get the first Document from Crawler
		
	string aWord; //display all words for the FIRST DOCUMENT ONLY (make sure get word function works)
		if(d->getFirstWord(aWord)) //try to get Document's first word
			cerr << aWord << endl; //output that word and manually check it
		while(d->getNextWord(aWord)) //get the rest of Document's words
			cerr << aWord << endl; //output all of the words to check

	while(d!=NULL)
	{
		count++;
		cerr << d->getURL() << endl; //output all the crawled URLs via Document order
		d=c->crawl(); //keep crawling until all links have been displayed
	}
	cerr << "We crawled " << count << " times!" << endl;
	*/

	/*
	Rule* r1 = new Rule("rule-00000000 0.50 BLAH HeY tHeRE & & > Do I work?");
	assert(r1->getAd()=="Do I work?"); //Check if the ad is exactly equal
	assert(r1->getDollarValue()==0.50); //Check for dollar value
	assert(r1->getNumElements()==5); //Check if counter is correct; includes words and operators
	assert(r1->getName()=="rule-00000000"); //verify that the name is exact
	assert(r1->getElement(0)=="blah"); //test for case; everything should become lowercase
	assert(r1->getElement(1)=="hey");
	assert(r1->getElement(2)!="thEre");
	assert(r1->getElement(r1->getNumElements()-1)=="&"); //verify both functions work

	Rule* r2 = new Rule("rule-00000000  0.50 BLAH HeY | tHeRE & & > Do I work?"); //improper rule!
	Rule* r3 = new Rule("rule-00000000  0.50 BLAH & HeY  tHeRE & > Do I work?"); //improper rule!

	assert(r2->getNumElements()==0 && r3->getNumElements()==0); //must show that they are improper!
	
	Document* d = new Document("www.nonexistentwebsite123abc.com", "blah hey there this should be true");
	Document* d2 = new Document("www.thisonedoesntexisteither.com", "blah hey this is false"); 

	assert(r1->match(*d)==true); //the first document matches!
	assert(r1->match(*d2)==false); //the second one NEARLY matches, but doesn't
	*/

	/*
	Rule* r1 = new Rule("rule-00000000 0.50 BLAH HeY tHeRE & & > Do I work?");
	Document* d = new Document("www.nonexistentwebsite123abc.com", "blah hey there this should be true");
	Document* d2 = new Document("www.thisonedoesntexisteither.com", "blah hey this is false"); 
	string r1Text="r1Text.txt"; //holds r1

	ifstream infile(r1Text);
	if (!infile)
	{
		cerr << "Problem!" << endl;
		exit(1);
	}

	Matcher* m = new Matcher(infile);
	vector<Match> matchV;
	m->process(*d, 0.10, matchV); //should match
	m->process(*d2, 0.10, matchV); //should NOT match%

	assert(matchV.size()==1); //make sure there's only one match
	cerr << matchV.at(0).url << endl; //see if the URL matches
	*/

	/*
	string cText="cnnText.txt";
	ifstream infile(cText);
	if (!infile)
	{
		cerr << "Problem!" << endl;
		exit(1);
	}
	AdHunter* adKiller = new AdHunter(infile);
	adKiller->addSeedSite("www.cnn.com");
	adKiller->addSeedSite("www.msn.com");

	vector<Match> matchV;
	adKiller->getBestAdTargets(0.10, 1, matchV); //there should be one match!
	assert(matchV.size()==1); //verify the single match
	cerr << matchV.at(0).url << endl; //see if the URL matches
	cerr << matchV.at(0).ad << endl; //see if the ad matches

	system("pause");
	*/


}