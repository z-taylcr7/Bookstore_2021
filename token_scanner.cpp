//
// Created by Cristiano on 2022/1/3.
//
#include "token_scanner.hpp"
using namespace std;
TokenScanner::TokenScanner(string exp) {
    head=new Node;
    cur=head;
    divide(exp);
}
string TokenScanner::firstToken() {
    return head->next->token;
}

bool TokenScanner::hasMoreToken() {
    return (cur->next != nullptr);
}

string TokenScanner::nextToken() {
    cur=cur->next;
    if(cur== nullptr)error("no more tokens!");
    return cur->token;
}

void TokenScanner::divide(const string &s){
    auto p=head;
    int len=s.length();
    int j=0,i=0;
    for(i=0;i<len;i++){
        if(s[i]==' '){
            Node*n=new Node;
            n->token=s.substr(j,i-j);j=i+1;
            while(s[j]==' '&&j!=len-1)j++;i=j;
            p->next=n;
            p=p->next;
        }
    }
    if(s[len-1]!=' ') {
        Node*l=new Node;
        l->token = s.substr(j);
        p->next = l;
        l->next = nullptr;
    }else{
        p->next= nullptr;
    }
    cur=head->next;
}
TokenScanner::~TokenScanner() {
    Node*c=head;
    while(c!= nullptr){
        Node* d=c;
        c=c->next;
        delete d;
    }
}

string TokenScanner::currentToken() {
    return cur->token;
}

