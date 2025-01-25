#include "Deque.h"
#include <stdio.h>
#include <iostream>

using namespace std;

//Default constructor

Deque::Deque() {
    head = nullptr;
    tail = nullptr;
    noOfItems = 0;
} //end-Deque

// Destructor
Deque::~Deque() {
    while (!IsEmpty()) 
        RemoveFront();
} //end-~Deque


void Deque::AddFront(int item) {	//Addition protocol to the front part
    DequeNode* newNode = new DequeNode(item);
    if (IsEmpty()) {	//Control mechanism for queue is empty or not, if it's empty
        tail = newNode;	//Moving tail part 
        head = newNode;	//Moving head part 
    }
    else {	// if it's not empty
        newNode->next = head;	//new node's next part will be head
        head->prev = newNode;	//head's previous part will be new node 
        head = newNode;			//new node will be head
    }
    noOfItems++;	//for addin' an element, rise the number of item
} //end-AddFront


void Deque::AddRear(int item) {		//Addition protocol to the rear part
    DequeNode* newNode = new DequeNode(item);
    if (IsEmpty()) {	//Control mechanism for queue is empty or not, if it's empty
        head = newNode;	//Moving tail part
        tail = newNode;	//Moving head part 
    }
    else {	// if it's not empty
        newNode->prev = tail;	//new node's previous part will be tail
        tail->next = newNode;	//tail's next part will be new node 
        tail = newNode;			//new node will be tail
    }
    noOfItems++;	//for addin' an element, rise the number of item
} //end-AddRear

// Remove and return the item at the front of the Deque
int Deque::RemoveFront() {
    if (IsEmpty())	//check it for blank or not
        cout << "Deque has not include any element, it's blank\n";	//print an error message
    int item = head->item;
    DequeNode* clone = head;	//create a clone for avoiding the data lose
    if (head == tail) {
        head = nullptr;		//delete with head=NULL
        tail = nullptr;		//delete with tail=NULL
    }
    else {
        head = head->next;	//new head is head's next
        head->prev = nullptr;	//delete with head's prev=NULL
    }
    delete clone;	//for memory efficiency, delete the copy
    noOfItems--;	//for addin' an element, decrease the number of item
    return item;
} //end-RemoveFront

/// Remove and return the item at the rear of the Deque

int Deque::RemoveRear() {
    if (IsEmpty())	//check it for blank or not
        cout << "Deque has not include any element, it's blank\n";	//print an error message
    int item = tail->item;
    DequeNode* clone = tail;	//create a clone for avoiding the data lose
    if (head == tail) {		//detecting the what part will we delete
        head = nullptr;		//delete with head=NULL
        tail = nullptr;		//delete with tail=NULL
    }
    else {
        tail = tail->prev;	//new tail is tail's previous
        tail->next = nullptr;	//delete with tail's next=NULL
    }
    delete clone;	//for memory efficiency, delete the copy
    noOfItems--;	//for addin' an element, decrease the number of item
    return item;
} //end-RemoveRear

/// Return the item at the front of the Deque (do not remove the item)
int Deque::Front() {
    if (IsEmpty())	//check it for blank or not
        cout << "Deque has not include any element, it's blank\n";	//print an error message
    
	return head->item;	//showing what's the head
} //end-Front

/// Return the item at the rear of the Deque (do not remove the item)
int Deque::Rear() {
    if (IsEmpty())	//check it for blank or not
        cout << "Deque has not include any element, it's blank\n";	//print an error message

	return tail->item;	//showing what's the tail
} //end-Rear
