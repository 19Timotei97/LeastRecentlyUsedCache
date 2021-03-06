#include "stdafx.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

struct Node
{
	Node* next;
	Node* prev;
	int value;
	int key;
	Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
	Node(int k, int val) :prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache
{

protected:
	map<int, Node*> mp; //map the key to the node in the linked list
	int cp;  //capacity
	Node* tail; // double linked list tail pointer
	Node* head; // double linked list head pointer
	/*
	set() - Set / insert the value of the key, if present, otherwise add the key as the most recently used key.
			If the cache has reached its capacity, it should replace the least recently used key with a new key.
	*/
	virtual void set(int, int) = 0;
	/// get() - Get the value(will always be positive) of the key if the key exists in the cache, otherwise return -1.
	virtual int get(int) = 0; 

};


class LRUCache : public Cache
{
public:
	LRUCache(int capc){ this->cp = capc;}

	void set(int key, int value)
	{
		Node *n;

		if (mp.empty()) // If there are no other nodes mapped
		{
			n = new Node(key, value); // set the new values
			tail = head = n; // create it as the only node
			mp[key] = n; // map it
		}

		auto i = mp.find(key); // Try to find the node

		if (i != mp.end()) // If it found it
		{
			i->second->value = value; // set the new value

			if (head == i->second) // If is the head, skip
				return;

			// If this next step is skipped, the whole list "collapses"
			i->second->prev->next = i->second->next; // Get the node out of his position

			if (tail == i->second) //Is it at the tail?
				tail = tail->prev; // cut one from the tail
			else                   //Or in between?
				i->second->next->prev = i->second->prev;

			// Set the node as the new head:
			// the next one is the head;
			i->second->next = head;
			// no more nodes behind;
			i->second->prev = NULL;
			// head's prev node is him;
			head->prev = i->second;
			// he is the node.
			head = i->second;
		}
		else               // Not in hash table
		{
			n = new Node(head->prev, head, key, value);
			// Set it as the head
			head->prev = n;
			head = n;
			mp[key] = n;

			if (mp.size() > cp) // Is capacity overloaded?
			{
				// delete the last element
				tail = tail->prev;
				// erase the key
				mp.erase(tail->next->key);
				// free the memory of it
				delete tail->next;
				// do not let it become a dangling pointer
				tail->next = NULL;
			}
		}
	}

	int get(int x)
	{
		auto i = mp.find(x);
		if (i != mp.end())
			return i->second->value;

		return -1;
	}
};

int main()
{
	int n, capacity, i;
	cout << "Enter the number of queries and the capacity of the Cache: \n";
	cin >> n >> capacity;
	LRUCache l(capacity);
	cout << "Please enter your queries: \n";
	cout << "Type \'set\' to setup a new Node, giving it the key and the value: \n";
	cout << "E.g.: set x y.\n";
	cout << "Or type \'get\' followed up the key, to return the value associated" << endl << " with that key or -1 if it doesn\'t exist.\n";
	cout << "Your turn: \n\n";
	for (i = 0; i<n; i++)
	{
		string command;
		cin >> command;
		if (command == "get")
		{
			int key;
			cin >> key;
			cout << l.get(key) << endl;
		}
		else if (command == "set")
		{
			int key, value;
			cin >> key >> value;
			l.set(key, value);
		}
	}
	return 0;
}