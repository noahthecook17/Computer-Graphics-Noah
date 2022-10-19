#ifndef Linkedlist_h
#define Linkedlist_h

#include <cstdlib>
#include "Node.h"
#include "Student.h"

using namespace std;

class LinkedList
{
public:
	typedef Student data_type;

	LinkedList();
	~LinkedList();

	//mem functions
	void addToHead(data_type& item);
	void addToTail(const data_type& item);
	void removeFromHead();
	void removeFromTail();
	void remove(string s);
	double average();
	
	std::size_t size();

	void operator +=(LinkedList& n);

	void operator -=(LinkedList& n);




private:
	Node* head_ptr_;
	Node* tail_ptr_;
	std::size_t list_length;

};

ostream& operator <<(ostream&, LinkedList&);


#endif