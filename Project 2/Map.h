//#pragma once

#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
public:
    Map();
	~Map();
	Map(const Map& other);
	Map& operator=(const Map& rhs);

    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
private:
	struct dataPair
	{
		KeyType key;
		ValueType value;
	};

	struct mapNode
	{
		dataPair pair;
		mapNode* next;
		mapNode* prev;
	};

	int m_size;
	mapNode* head;
	mapNode* tail;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif