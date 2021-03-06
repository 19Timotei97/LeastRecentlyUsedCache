// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>

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
	virtual void set(int, int) = 0; //set function
	virtual int get(int) = 0; //get function

};


class LRUCache : public Cache
{
public:
	LRUCache(int capc)
	{
		this->cp = capc;
	}

	void set(int key, int value)
	{
		Node *n;

		if (mp.empty()) // No other nodes
		{
			n = new Node(key, value);
			tail = head = n;
			mp[key] = n;
		}

		auto i = mp.find(key);

		if (i != mp.end())
		{
			i->second->value = value;

			if (head == i->second)
				return;

			i->second->prev->next = i->second->next;

			if (tail == i->second) //Is it at the tail?
				tail = tail->prev;
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
				tail = tail->prev;
				// erase the key
				mp.erase(tail->next->key);
				// free the memory of him
				delete tail->next;
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
	cin >> n >> capacity;
	LRUCache l(capacity);
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

