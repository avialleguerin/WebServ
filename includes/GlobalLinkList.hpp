#ifndef GLOBALLINKLIST_HPP
#define GLOBALLINKLIST_HPP

#include "Header.hpp"

struct Node {
	t_serverData *data;
	Node* next;
	
	Node(t_serverData *val) : data(val), next(NULL) {}
};

class GlobalLinkedList {
private:
	static Node* head;
	static Node* tail;

public:
	static void insert(t_serverData *data) 
	{
		Node* newNode = new Node(data);
		if (head == NULL) {
			head = tail = newNode;
		} else {
			tail->next = newNode;
			tail = newNode;
		}
	}

	static void update_data(t_serverData *data) 
	{
		Node *copy = head;
		if(!head)
			return;
		while(head != NULL)
		{
			if(head->data == data)
			{
				head->data = NULL;
			}
			head = head->next;
		}
		head = copy;
	}

	static void cleanup() 
	{
		while (head != NULL) {
			Node* temp = head;
			head = head->next;
			if(temp->data)
			{
				if(temp->data->cgi)
				{
					delete temp->data->cgi;
					temp->data->cgi = NULL;
				}
				delete temp->data;
				temp->data = NULL;
			}
			delete temp;
		}
		tail = NULL;
	}
};

#endif