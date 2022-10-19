#include<iostream>
#include<cstdlib>
#include "Node.h"
using namespace std;

void Node::setData(const data_type& i) {
	data = i;
}
void Node::setNext(Node* j) {
	next = j;
}
void Node::setPrev(Node* j) {
	prev = j;
}

Node::getNext(Node) const {
	return next;
}
Node::getPrev(Node) const {
	return previous;
}
Node::data_type Node::getData() const{
	return data;
}