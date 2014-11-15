class Animal
{
public:
	Animal(string n);
	string name() const;
	virtual string speak() const=0;
	virtual string move() const;
	virtual ~Animal();
private:
	string m_name;
};

Animal::Animal(string n)
{
	m_name=n;
}

Animal::~Animal()
{}

string Animal::name() const
{
	return m_name;
}

string Animal::move() const
{
	return "walk";
}

///////////////////////////////////////////////////////

class Cat : public Animal
{
public:
	Cat(string n);
	virtual string speak() const;
	virtual ~Cat();
private:
};

Cat::Cat(string n)
	: Animal(n)
{}

Cat::~Cat()
{
	cout << "Destroying " << name() << " the cat" << endl;
}

string Cat::speak() const
{
	return "Meow";
}

///////////////////////////////////////////////////////

class Pig : public Animal
{
public:
	Pig(string n, int w);
	virtual string speak() const;
	virtual Pig::~Pig();
private:
	int m_weight;
};

Pig::Pig(string n, int w)
	: Animal(n), m_weight(w)
{}

Pig::~Pig()
{
	cout << "Destroying " << name() << " the pig" << endl;
}

string Pig::speak() const
{
	if(m_weight>=180)
		return "Grunt";
	return "Oink";
}

///////////////////////////////////////////////////////

class Duck : public Animal
{
public:
	Duck(string n);
	virtual string speak() const;
	virtual string move() const;
	virtual ~Duck();
private:
};

Duck::Duck(string n)
	: Animal(n)
{}

Duck::~Duck()
{
	cout << "Destroying " << name() << " the duck" << endl;
}

string Duck::speak() const
{
	return "Quack";
}

string Duck::move() const
{
	return "swim";
}