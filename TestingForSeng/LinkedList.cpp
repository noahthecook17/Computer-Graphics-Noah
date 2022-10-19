#include<iostream>
#include<cstdlib>
#include "LinkedList.h"
#include "Student.h"

using namespace std;

    LinkedList::LinkedList()
    {
        head_ptr_ = NULL;
        tail_ptr_ = NULL;
        list_length;
    }

    void LinkedList::addToHead(data_type& data)
    {
        if(list_length == 0)
        {
            head_ptr_ = new Node(data);
            tail_ptr_ = head_ptr_;
            list_length = 1;
        }
        else
        {
            Node* head_insert = new Node(data);

            head_ptr_->setPrev(head_insert);
            head_insert->setNext(head_ptr_);

            head_ptr_ = head_insert;

            list_length++;

            head_insert = NULL;

        }
      
    }

    void LinkedList::addToTail(data_type& data)
    {
        if(list_length == 0)
        {
            head_ptr_ = new Node(data);
            tail_ptr_ = head_ptr_;
            list_length = 1;
        }
        else
        {
            Node* tail_insert = new Node(data);
            tail_ptr_->setNext(tail_insert);
            tail_insert->setPrev(tail_ptr_);

            tail_ptr_ = tail_insert;

            list_length++;
            
            tail_insert = NULL;

        }


    }