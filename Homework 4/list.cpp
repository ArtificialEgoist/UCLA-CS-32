/*
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Domain
{
  public:
    Domain(string lbl) : m_label(lbl) {}
    string label() const { return m_label; }
    const vector<Domain*>& subdomains() const { return m_subdomains; }
    void add(Domain* d) { m_subdomains.push_back(d); }
    ~Domain();
  private:
    string m_label;
    vector<Domain*> m_subdomains;
};

Domain::~Domain()
{
    for (size_t k = 0; k < m_subdomains.size(); k++)
        delete m_subdomains[k];
}
*/

void listAllAuxiliary(string path, const Domain* d)
{
	for(int i=0; i<d->subdomains().size(); i++)
	{
		Domain* temp=d->subdomains().at(i);

		if(path=="")
			listAllAuxiliary(temp->label(), temp);
		else
			listAllAuxiliary(temp->label()+"."+path, temp);
	}

	if(d->subdomains().empty()) //d is a leaf
		cout << path << endl;
}

/*
void listAll(const Domain* d)
{
    listAllAuxiliary(d->label(), d);
}

int main()
{
    Domain* d1 = new Domain("ucla");
    d1->add(new Domain("cs"));
    d1->add(new Domain("ee"));
    d1->add(new Domain("math"));
    Domain* d2 = new Domain("caltech");
    d2->add(new Domain("math"));
    d2->add(new Domain("cs"));
    Domain* d3 = new Domain("edu");
    d3->add(d1);
    d3->add(d2);
    Domain* d4 = new Domain("com");
    d4->add(new Domain("microsoft"));
    d4->add(new Domain("apple"));
    Domain* d5 = new Domain("biz");
    Domain* root = new Domain("");
    root->add(d3);
    root->add(d4);
    root->add(d5);
    listAll(root);
    cout << "====" << endl;
    listAll(d2);
    cout << "====" << endl;
    listAll(d5);
    delete root;
}
*/