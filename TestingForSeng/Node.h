#ifndef Node_h
#define Node_h
#include <cstdlib>
#include <iostream>
#include "Student.h"

using namespace std;
	class Node
	{
	public:
		typedef Student data_type;
		//constructor & deconstructor
		Node(const data_type& init_data = data_type(), Node* init_next = NULL, Node* init_prev = NULL) {
			data = init_data;
			next = init_next;
			prev = init_prev;
		}
		~Node();

		void setData(const data_type& new_data);
		void setNext(Node* next_link);
		void setPrev(Node* prev_link);

		const Node* getNext() const;
		const Node* getPrev() const;
		data_type* getData() const;

	private:
		data_type data;
		Node* next;
		Node* prev;
	};



#endif
