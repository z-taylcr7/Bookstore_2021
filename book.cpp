//
// Created by Cristiano on 2022/1/3.
//
#include "book.hpp"
#include "memo.hpp"
#include "error.hpp"
#include "global.hpp"
#include "blocklist.hpp"
book::book() {
    amount=0;keys=0;
    memset(&ISBN,0,sizeof(ISBN));
    memset(&price,0,sizeof(price));
    memset(&book_name,0,sizeof(book_name));
    memset(&author,0,sizeof(author));
    memset(&keyword,0,sizeof(keyword));
}
void book::toKeyword(const string &t) {
    int l=t.length();int j=0;int cnt=0;
    if(l>60)error("too long");
    for(int i=0;i<l;i++){
        if(t[i]=='|'){
            if(i-j==0)error("");
            strcpy(keyword[cnt++],t.substr(j,i-j).c_str());j=i+1;
        }
        for(int i=0;i<cnt-1;i++){
            if(strcmp(keyword[cnt-1],keyword[i])==0){
                Line st(currentUser,"Edit book FAIL-repeated keyword");
                diary.write(st);error("repeat keyword");
            }
        }
    }
    strcpy(keyword[cnt++],t.substr(j).c_str());
    for(int i=0;i<cnt-1;i++){
        if(strcmp(keyword[cnt-1],keyword[i])==0){
            Line st(currentUser,"Edit book FAIL-repeated keyword");
            diary.write(st);error("repeat keyword");
        }
    }
    keys=cnt;
}

book::book(const book &ob) {

    memset(&keyword,0,sizeof(keyword));
    strcpy(ISBN,ob.ISBN);
    strcpy(author,ob.author);
    strcpy(book_name,ob.book_name);
    price=ob.price;
    amount=ob.amount;
    keys=ob.keys;
    for(int i=0;i<keys;i++){
        strcpy(keyword[i],ob.keyword[i]);
    }
}

book::book(const string &i, const string &n, const string &a, const string &k, int q, const string &p) {
    memset(&keyword,0,sizeof(keyword));
    strcpy(ISBN,i.c_str());
    strcpy(book_name,n.c_str());
    strcpy(author,a.c_str());
    price=stod(p);
    if(!k.empty())toKeyword(k);
    else keys=0;
    amount=q;
}

book &book::operator=(const book &ob) {
    if(this==&ob)return *this;
    memset(&ISBN,0,sizeof(ISBN));
    memset(&price,0,sizeof(price));
    memset(&book_name,0,sizeof(book_name));
    memset(&author,0,sizeof(author));
    memset(&keyword,0,sizeof(keyword));
    strcpy(ISBN,ob.ISBN);
    strcpy(author,ob.author);
    strcpy(book_name,ob.book_name);
    price=ob.price;
    amount=ob.amount;
    keys=ob.keys;
    for(int i=0;i<keys;i++){
        strcpy(keyword[i],ob.keyword[i]);
    }
    return *this;
}

bool book::operator==(const book &rhs) {
    return (strcmp(ISBN,rhs.ISBN)==0);
}

bool book::operator!=(const book &rhs) {
    return (strcmp(ISBN,rhs.ISBN)!=0);
}

bool book::operator<(const book &rhs) {
    return (strcmp(ISBN,rhs.ISBN)<0);
}
ostream &operator<<(ostream &os, const book &rhs) {
    os<<rhs.ISBN<<'\t'<<rhs.book_name<<'\t'<<rhs.author<<'\t';
    for(int i=0;i<rhs.keys-1;i++){
        os<<rhs.keyword[i]<<'|';
    }
    if(rhs.keys>0)os<<rhs.keyword[rhs.keys-1]<<'\t';
    else os<<'\t';
    os<<fixed<<setprecision(2)<<rhs.price<<'\t'<<rhs.amount;
    return os;
}

double book::getPrice(){
    return price;
}
void book::setModification(book& mod, int &ofs) {
    bookList_ISBN.Delete(ISBN,ofs);
    bookList_book_name.Delete(book_name,ofs);
    bookList_author.Delete(author, ofs);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],ofs);
    library.update(mod,ofs);
    *this=mod;
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],ofs);
    bookList_author.insert(author,ofs);
    bookList_book_name.insert(book_name,ofs);
    bookList_ISBN.insert(ISBN,ofs);
}
void book::changeIsbn(string s) {
    strcpy(ISBN,s.c_str());
}
void book::changeBookName(string s) {
    strcpy(book_name,s.c_str());
}

void book::changeAuthor(string s) {
    strcpy(author,s.c_str());
}

void book::changeKeyword(string s) {
    toKeyword(s);
}

void book::changePrice(string s) {
    price=stod(s);
}
void book::addAmount(int x) {
    if(amount+x<0)error("sold out!");
    amount+=x;

}
const char *book::getIsbn() const {
    return ISBN;
}

const char *book::getBookName() const {
    return book_name;
}
