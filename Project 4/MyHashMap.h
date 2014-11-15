#ifndef MYHASHMAP_INCLUDED
#define MYHASHMAP_INCLUDED

const int DEFAULT_NUM_BUCKETS = 1000000;

#include <string>

// In accordance with the spec, YOU MUST RE-IMPLEMENT THIS CLASS, since
// you are not allowed to use an STL map or unordered_map or hash_map.

template <typename ValueType>
class MyHashMap
{
public:
	MyHashMap()
		: m_items(0), m_buckets(DEFAULT_NUM_BUCKETS), tail(NULL), head(NULL)
	{
		hashTable = new Node*[DEFAULT_NUM_BUCKETS];
		
		for(int i=0; i<m_buckets; i++)
			hashTable[i]=NULL;
	}

    MyHashMap(int numBuckets)
		: m_items(0), m_buckets(numBuckets), tail(NULL), head(NULL)
    {
		hashTable = new Node*[numBuckets];
		
		for(int i=0; i<m_buckets; i++)
			hashTable[i]=NULL;
	}

    ~MyHashMap()
    {
		Node* temp=head;
		Node* tempNext=head->next;

		for(int i=0; i<m_items; i++) //delete all items pointed to in our second linked-list
		{
			delete temp;
			temp=tempNext;
			tempNext=tempNext->next;
		}

		delete hashTable;
    }

    void associate(std::string key, const ValueType& value)
    {
		int code = hashCode(key); //get hash code for string key

		if(hashTable[code]==NULL) //if there's no item there, add a new linked list
		{
			Node* newNode = new Node(); //create a new node and add it into the list
			newNode->m_key=key;
			newNode->m_value=value;
			newNode->next=NULL;

			hashTable[code]=newNode;
			
			if(head!=NULL) //that means this is NOT our first item added
				tail->next=newNode; //add a copy of that pointer into the back of our separate linked list
			else //this is the first item added
				head=newNode; //head now points to the new node
			tail=newNode; //update tail pointer (if the linked list was empty, this sets it to the first key)
			
			m_items++; //increment item count

			//cerr << "Added new item (unique key) to hash map!" << endl;

		}
		else //the slot is occupied, so we "probe" for the last spot
		{
			Node* temp = hashTable[code];

			while(temp->m_key!=key && temp->next!=NULL) //check if the key is already there
				temp=temp->next; //temp will either be the last node or the node with the matching key

			if(temp->next==NULL && temp->m_key!=key) //if its the last node without a matching key
			{		
				Node* newNode = new Node(); //create a new node
				newNode->m_key=key;
				newNode->m_value=value;
				newNode->next=NULL;

				temp->next=newNode; //add the new node to the end of the list
				tail->next=newNode; //add a copy of that pointer into our own separate linked list
				tail=newNode;

				m_items++; //increment item count
				//cerr << "Added new item (colliding key) to hash map!" << endl;
			}
			else //this means that temp's m_key matches key
			{
				temp->m_value=value; //if the key is still there, we just need to update it
				//cerr << "Updated an item in hash map!" << endl;
			}
		}
    }

    const ValueType* find(std::string key) const
    {
		int code = hashCode(key);
		Node* temp = hashTable[code];

		if(temp==NULL) //if the bucket is empty, return NULL
			return NULL;
		else
		{
			
			while(temp->m_key!=key)
			{
				if(temp->next==NULL)
					return NULL;
				temp=temp->next;
			}

			//while(temp!=NULL && temp->m_key!=key) //find the matching item or first NULL
				//temp=temp->next;

			//if(temp==NULL)
				//return NULL;

			return &(temp->m_value); //return the pointer to the constant-casted value

			//ValueType returnTemp = (const ValueType)(temp->m_value);
			//return &returnTemp //return that value
		}
    }

    ValueType* find(std::string key)
    {
		// Do not change the implementation of this overload of find
        const MyHashMap<ValueType>* constThis = this;
        return const_cast<ValueType*>(constThis->find(key));
    }

    ValueType* getFirst(std::string& key)
    {
		curr=head; //without calling getFirst, getNext is supposed to be undefined, so we initialize the curr pointer here only

		if(curr==NULL) //curr is initiated as a head pointer in constructor
			return NULL; //if there are no items, return NULL

		key=curr->m_key; //update the key parameter
		return &(const ValueType)(curr->m_value); //return the pointer to the constant-casted value
    }

    ValueType* getNext(std::string& key)
    {
		if(curr->next==NULL) //if there are no items, return NULL
			return NULL;

		curr=curr->next; //set curr pointer to the next node

		key=curr->m_key; //update the key parameter
		return &(const ValueType)(curr->m_value); //return the pointer to the constant-casted value
    }

    int numItems() const
    {
		return m_items;
    }

private:
	//No STL structures (like maps) are used
	struct Node
    {
        std::string m_key;
        ValueType m_value;
        Node* next;
    };

	int m_items;
	int m_buckets;

	Node** hashTable;
	Node* head;
	Node* tail;
	Node* curr;

	int hashCode(const std::string &temp) const
	{
		int total=0;
		for (size_t i=0; i<temp.length(); i++)
			total = total + (i+1)*temp[i];

		total = total % m_buckets;
		
		//cerr << "the key is: " << total << endl;

		return total;
	}
};

#endif // MYHASHMAP_INCLUDED





/*
	typedef std::map<std::string, ValueType> HelperMapType;
    HelperMapType                    m_map;
    typename HelperMapType::iterator m_curr;
*/




/*
MyHashMap(int numBuckets = DEFAULT_NUM_BUCKETS)
     : m_curr(m_map.begin())
    {}

    ~MyHashMap()
    {
    }

    void associate(std::string key, const ValueType& value)
    {
        m_map[key] = value;
    }

    const ValueType* find(std::string key) const
    {
        typename HelperMapType::const_iterator it = m_map.find(key);
        if (it == m_map.end())
            return NULL;
        else
            return &it->second;
    }

    ValueType* find(std::string key)
    {
          // Do not change the implementation of this overload of find
        const MyHashMap<ValueType>* constThis = this;
        return const_cast<ValueType*>(constThis->find(key));
    }

    ValueType* getFirst(std::string& key)
    {
        m_curr = m_map.begin();
        if (m_curr == m_map.end())
            return NULL;

        key = m_curr->first;
        return &m_curr->second;
    }

    ValueType* getNext(std::string& key)
    {
        m_curr++;
        if (m_curr == m_map.end())
            return NULL;

        key = m_curr->first;
        return &m_curr->second;
    }

    int numItems() const
    {
        return m_map.size();
    }
*/