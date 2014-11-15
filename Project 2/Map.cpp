#include "Map.h"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

Map::Map()
{
	m_size=0;	//there are no nodes on construction of Map
	head=NULL;	//there is no head node
	tail=NULL;	//there is no tail node
}

Map::~Map()
{
	if(!empty()) //we only call the destructor if there are nodes in Map
	{
		if(head==tail) //if there is only one node, just delete that one node
			delete head;
		else //otherwise...
		{
			mapNode* temp = head; //make a node pointer that starts out at head
			while(temp->next!=NULL) //as long as temp's next isn't NULL
			{
				temp=temp->next; //advance temp pointer to next node
				delete temp->prev; //delete the previous node
			}
			delete tail; //when temp's next is finally NULL, we don't delete the last node, so we must call it here
		}
	}
}

Map::Map(const Map& other)
{
	m_size=0; //initialize the size counter
	head=NULL;
	tail=NULL;

	for(int i=0; i<other.size(); i++) //for each node in other, make a copy of it in this map
	{			
		KeyType k;
		ValueType v;
		other.get(i, k, v); //get the node in other and save its key and value

		insert(k, v);
	}
}

Map& Map::operator=(const Map& rhs)
{
	if(this!=&rhs)
    {
        Map temp(rhs);
        swap(temp);
    }
    return *this;
}

bool Map::empty() const
{
	return head==NULL; //return m_size==0;
}

int Map::size() const
{
	return m_size; //returns number of nodes in Map
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if(contains(key)) //if Map already contains key, return false and do nothing
		return false;

	mapNode* newNode=new mapNode; //create a new node
	newNode->pair.key=key; //assign the node's key and value
	newNode->pair.value=value;
	newNode->next=NULL; //since this node is at the end, no node follows it (set it to NULL)
	newNode->prev=NULL; //if this node is the head, there would be no previous; change later if it has a previous node

	if(empty()) //if there is no head, set that node to head AND tail
	{
		head=newNode;
		tail=newNode;
	}	
	else	//otherwise, set the new node to tail's next
	{
		newNode->prev=tail;
		tail->next=newNode;
		tail=newNode;
	}

	m_size++;
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	if(!contains(key)) //if Map doesn't contain key, there is nothing to update! Return false
		return false;

	mapNode* temp = head;

	while(temp->pair.key!=key) //set temp pointer to the node where keys match
		temp=temp->next;

	temp->pair.value=value;

	return true;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if(contains(key)) //if Map has key already, update it
		update(key, value);
	else //otherwise, insert it
		insert(key, value);

	return true; //this function should always return true
}

bool Map::erase(const KeyType& key)
{
	if(!contains(key)) //if the map doesn't have a node with the key, there is nothing to erase; return false
		return false;

	if(size()==1) //if there's only one node and its key matches, delete the node; then delete the head and tail pointers
	{
		delete head;
		head=NULL;
		tail=NULL;
		m_size--;
		return true;
	}

	mapNode* temp = head;

	while(temp->pair.key!=key) //set temp pointer to the node where keys match
		temp=temp->next;

	if(temp==head) //if the node to delete is the head, reassign head pointer; then delete the node
	{
		head=temp->next; //move the head pointer to the following node
		delete temp; //delete specified node
		head->prev=NULL; //set the prev value of the new head to NULL
	}
	else if(temp==tail) //if the node to delete is the tail, reassign tail pointer; then delete the node
	{
		tail=temp->prev; //move the tail pointer to the previous node
		delete temp; //delete specified node
		tail->next=NULL; //set the next value of the new tail to NULL
	}
	else //if the node is somewhere in between the head and tail nodes
	{
		mapNode* before = temp->prev;
		mapNode* after = temp->next;

		delete temp; //delete specified node 

		before->next=after; //set the next of the node before temp to the node after it
		after->prev=before; //set the prev of the node after temp to the node before it
	}
	m_size--;
	return true;
}

bool Map::contains(const KeyType& key) const
{
	mapNode* temp = head; //create a temporary pointer to the first node

	if(head==NULL) //if Map is empty, return false
		return false;

	while(temp!=NULL) //as long as temp isn't a NULL, check if its key matches
	{
		if(temp->pair.key==key)
			return true; //if the keys match, return true
		temp=temp->next; //otherwise, update temp
	}
	return false; //after no keys are found to be matching, return false
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	if(!contains(key)) //if there is no node that has key, there is nothing to get; return false
		return false;

	mapNode* temp = head;

	while(temp->pair.key!=key) //set temp pointer to the node where keys match
		temp=temp->next;

	value=temp->pair.value; //set value to the value of the selected node

	return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if(i<0 || i>=size()) //if index i is out of bounds, return false
		return false;

	mapNode* temp = head;

	for(int k=0; k<i; k++) //find the ith node in the Map (i.e. i=0 would simply give what head points to)
		temp=temp->next;

	key=temp->pair.key; //set key and value to the node's key and value
	value=temp->pair.value;

	return true;
}

void Map::swap(Map& other)
{
	mapNode* tempHead = head; //set a pointer to link to this Map's head
	mapNode* tempTail = tail; //set a pointer to link to this Map's tail

	head=other.head; //set this Map's head and tail pointers to those of other
	tail=other.tail;

	other.head=tempHead; //set the other Map's head and tail pointers to those originally of this Map
	other.tail=tempTail;

	int currentSize=size(); //save the size of the original Map
	m_size=other.size(); //update the size of this Map
	other.m_size=currentSize; //update the size of the other Map
}

bool combine(const Map& m1, const Map& m2, Map& result)
{	
	Map m1Temp=m1; //make temporary Maps for m1 and m2 to prevent aliasing
	Map m2Temp=m2;

	bool noMismatches=true; //checks if values in m1 and m2 differ for any key k

	while(!result.empty()) //if result starts with anything, erase the nodes
	{
		KeyType k;
		ValueType v;

		result.get(0, k, v);
		result.erase(k);
	}

	for(int i=0; i<m1Temp.size(); i++)
	{
		KeyType k;
		ValueType v;
		m1Temp.get(i, k, v); //get each node from m1Temp

		if(!m2Temp.contains(k)) //if m2Temp doesn't have that node, insert it into result
			result.insert(k, v);
		else //if m2Temp does have that node, check for its value
		{
			ValueType v2;
			m2Temp.get(k, v2);
			if(v==v2) //if the values are the same, insert it into result
				result.insert(k, v);
			else //otherwise, make sure the function returns false (at the end)
				noMismatches=false;
		}
	}

	for(int i=0; i<m2Temp.size(); i++) //for the nodes that are ONLY in m2
	{
		KeyType k;
		ValueType v;
		m2Temp.get(i, k, v); //get each node from m2Temp

		if(!m1Temp.contains(k)) //if m1Temp doesn't have that node, insert it into result
			result.insert(k, v);
	}

	return noMismatches;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	Map m1Temp=m1; //make temporary Maps for m1 and m2 to prevent aliasing
	Map m2Temp=m2;

	while(!result.empty()) //if result starts with anything, erase the nodes
	{
		KeyType k;
		ValueType v;

		result.get(0, k, v);
		result.erase(k);
	}

	for(int i=0; i<m1Temp.size(); i++)
	{
		KeyType k;
		ValueType v;

		m1Temp.get(i, k, v); //get each node from m1Temp
		if(!m2Temp.contains(k)) //if that node's key is not found in m2Temp, insert it into result
			result.insert(k, v);		
	}
}