#ifndef STACK_H
#define STACK_H
#include <iostream>
using namespace std;

template <class elemType>
class Stack{
private:
    struct node{
        elemType data;
        node *next;
        node(const elemType &x,node *N=NULL){
            data=x;
            next=N;
        }
        node():next(NULL){};
        ~node(){}
    };
    node *elem;
public:
    Stack(){
        elem=NULL;
    }
    ~Stack(){}
    bool isEmpty()const{
        return elem==NULL;
    }
    void push(const elemType &x){
        node *tmp=new node(x,elem);
        elem=tmp;
    }
    elemType pop(){
        node*tmp=elem;
        elemType x=tmp->data;
        elem=elem->next;
        delete tmp;
        return x;
    }

    elemType top()const{
        return elem->data;
    }

};
#endif // STACK_H
