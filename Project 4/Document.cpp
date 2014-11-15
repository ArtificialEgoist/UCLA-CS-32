#include "provided.h"
#include "MyHashMap.h"
#include <string>
using namespace std;

class DocumentImpl 
{
public:
    DocumentImpl(string url, const string& text);
    string getURL() const;
    bool contains(string word) const;
    bool getFirstWord(string& word);
    bool getNextWord(string& word);
private:
	string m_url;
	string m_text;
	//MyHashMap<bool>* wordHash;
	MyHashMap<bool> wordHash;
};

DocumentImpl::DocumentImpl(string url, const string& text)
	: m_url(url), m_text(text)
{
	//wordHash = new MyHashMap<bool>;

	for(size_t i=0; i<m_text.length(); i++)
	{
		if(!isascii(m_text[i])) //if a character is not ascii, set it to space so that it can be removed by tokenizer later
			m_text[i]=' ';
		else
			m_text[i]=tolower(m_text[i]); //make the rest of m_text all lowercase only
	}

	Tokenizer temp(m_text, " ,.!?/<>@#$%^&*()_-+=\\:;'{}[]\""); //splits up words by nearly all symbols

	string token;
	while (temp.getNextToken(token))
		wordHash.associate(token, true); //put all unique separated words (tokens) into the hash map and assign an arbitrary integer
		//wordHash->associate(token, true); //put all unique separated words (tokens) into the hash map and assign an arbitrary integer

	//cerr << "Document's constructor has finished adding words to hash map." << endl;
}

string DocumentImpl::getURL() const
{
    return m_url;
}

bool DocumentImpl::contains(string word) const
{
    return(wordHash.find(word)!=NULL); //return true if the word is found in the hash table
	//return(wordHash->find(word)!=NULL); //return true if the word is found in the hash table
}

bool DocumentImpl::getFirstWord(string& word)
{
	return(wordHash.getFirst(word)!=NULL); //return true if the first word is found in the hash table
	//return(wordHash->getFirst(word)!=NULL); //return true if the first word is found in the hash table
}

bool DocumentImpl::getNextWord(string& word)
{
	return(wordHash.getNext(word)!=NULL); //return true if the first word is found in the hash table
	//return(wordHash->getNext(word)!=NULL); //return true if the first word is found in the hash table
}

//******************** Document functions *******************************

// These functions simply delegate to DocumentImpl's functions.
// You probably don't want to change any of this code.

Document::Document(string url, const string& text)
{
    m_impl = new DocumentImpl(url, text);
}

Document::~Document()
{
    delete m_impl;
}

string Document::getURL() const
{
    return m_impl->getURL();
}

bool Document::contains(string word) const
{
    return m_impl->contains(word);
}

bool Document::getFirstWord(string& word)
{
    return m_impl->getFirstWord(word);
}

bool Document::getNextWord(string& word)
{
    return m_impl->getNextWord(word);
}
