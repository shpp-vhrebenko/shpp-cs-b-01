#pragma once
#include <conio.h>

using namespace std;
// Implementation of a stack-based linked list(LIFO).
template<class T>
class MyStack{
    class Node{
    public:
        T value;
        Node *next;
    };
    Node *head;
    int count;
public:

    // Default constructor
    MyStack(){
        head = nullptr;
        count = 0;
    }

    // Adding an element to the top of the stack
    void push(T elem){
        Node *tmp = new Node;
        tmp->value = elem;
        if(head == nullptr){
            tmp->next = nullptr;
            head = tmp;
        }
        else{
            tmp->next = head;
            head = tmp;
        }
        count++;
    }


    // Removing an item from the top of the stack.
    void pop(){
       Node *temp = head->next;
        if(count > 0) {
          if(temp != nullptr){
              delete head;
              head = temp;
              count--;
          }
          else{
              delete head;
              head = nullptr;
              count = 0;
          }
        } else {
          cout<<"The stack is empty!\n";
        }
    }


    // Getting the value of the top element of the stack.
    T top(){
        return head->value;
    }


    //Stack size.
    int size(){
        return count;
    }


    // Destructor
    ~MyStack(){
        while(head != nullptr){
            Node *temp = head->next;
            delete head;
            head = temp;
        }       
    }


    // Is the stack empty.
    bool empty(){
        if(count == 0){
            return true;
        }
        else{
            return false;
        }
    }


    // Display stack.
    void print(){
        Node *temp = head;
        while(temp != nullptr){
            cout<<temp -> value<<" ";
            temp = temp->next;
        }
        cout<<endl;

    }
};


