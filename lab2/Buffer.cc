#include <fstream>
#include <iostream>
#include <stdio.h>
#include "Buffer.h"

//TODO: your code here
//implement the functions in ListBuffer

Buffer::Buffer() {
    currentLineNum=0;
    head=nullptr;
}

Buffer::~Buffer() {
    node *p=head,*q=head;
    while(p->next!=nullptr){
       q=p->next;
       delete p;
       p=q;
    }
    delete p;
    currentLineNum=0;
    head=nullptr;
}

void Buffer::scanLine(int lineNum){
    if(head==nullptr){
        throw "Line number out of range";
    }
    node *p=head;
    for (int i=1;i<=lineNum;i++){
        if(p==nullptr){
            throw "Line number out of range";
        }
        p=p->next;
    }
}

void Buffer::writeToFile(const string &filename) const {
    std::ofstream OutFile(filename);
    node *p=head;
    int size=0;
    while(p->next!=nullptr){
        size+=p->text.size()+1;
        OutFile<<p->text<<"\n";
        p=p->next;
    }
    size+=p->text.size()+1;
    OutFile<<p->text;
    OutFile.close();
    printf("%d byte(s) written\n",size);
}

void Buffer::showLines(int from, int to) {
    node *p=head;
    if(from==1 &&to==-1){
        if(currentLineNum==0){
            throw "Number range error";
        }
        currentLineNum=0;
        while(p!=nullptr){
            currentLineNum++;
            std::cout<<currentLineNum<<"\t"<<p->text<<std::endl;
            p=p->next;
        }
        return;
    }
    if(from<=0){
        throw "Line number out of range";
    }
    scanLine(to);
    for (int i=1;i<from;i++){
        p=p->next;
    }
    for (int i=from;i<=to;i++){
        std::cout<<i<<"\t"<<p->text<<std::endl;
        p=p->next;
    }
    currentLineNum=to;
}

void Buffer::deleteLines(int from, int to){
    if(from==1 &&to==-1){
        if(currentLineNum==0){
            throw "Delete range error";
        }
        node *p=head,*q=head;
        while(p->next!=nullptr){
           q=p->next;
           delete p;
           p=q;
        }
        delete p;
        currentLineNum=0;
        head=nullptr;
        return;
    }
    if(from<=0){
        throw "Line number out of range";
    }
    scanLine(to);
    node *pos=head,*p,*q;
    if(from==1){
        p=pos;
        for(int i=from;i<=to;i++){
            q=p->next;
            delete p;
            p=q;
        }
        if(p==nullptr){
            head=nullptr;
            currentLineNum=0;
        }
        else{
            head=p;
            currentLineNum=1;
        }
        return;
    }
    for (int i=1;i<from-1;i++){
        pos=pos->next;
    }
    p=pos->next;
    for (int i=from;i<=to;i++){
        q=p->next;
        delete p;
        p=q;
    }
    pos->next=p;
    currentLineNum=from;
}

void Buffer::insertLine(const string &text){
    if(head==nullptr){
        head=new node(text);
        currentLineNum=1;
        return;
    }
    if(currentLineNum==1){
        node *p=head;
        head=new node(text);
        head->next=p;
    }
    else{
        node *p=head;
        for (int i=1;i<currentLineNum-1;i++){
            p=p->next;
        }
        node *q=new node(text);
        q->next=p->next;
        p->next=q;
    }
}

void Buffer::appendLine(const string &text){
    if(head==nullptr){
        head=new node(text);
        currentLineNum=1;
    }
    else{
        node *p=head;
        for (int i=1;i<currentLineNum;i++){
            p=p->next;
        }
        node *q=new node(text);
        q->next=p->next;
        p->next=q;
        currentLineNum++;
    }
}

const string &Buffer::moveToLine(int idx) {
    scanLine(idx);
    node *p=head;
    for (int i=1;i<idx;i++){
        p=p->next;
    }
    currentLineNum=idx;
    return p->text;
}
