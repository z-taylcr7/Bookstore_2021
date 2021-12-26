//
// Created by Cristiano on 2021/12/22.
//

#ifndef MAIN_CPP_BOOK_HPP
#define MAIN_CPP_BOOK_HPP
#include <string>

class book{
char ISBN[20] ;
char book_name[60];
public:
    void changeIsbn(string s) ;

    void changeBookName(string s) ;

    void changeAuthor(string s) ;

    void changeKeyword(string s);

    void changePrice(string s);

    void addAmount(int x);

    float getPrice();

private:
    char author[60];
char keyword[20][20];
int amount;
int keys;
char price[13] ;
public:
//构造，拷贝构造，=，<,==,!=
book();

book(const book &ob);

book(const string&i,const string&n,const string&a,const string&k,int q,const string&p);

void toKeyword(const string&t);

book &operator=(const book& obj);

bool operator==(const book& rhs);

bool operator!=(const book& rhs);

bool operator<(const book& rhs);

friend ostream& operator<<(ostream&os,const book& rhs);

};
extern BlockList<book> bookList_keyword;
extern BlockList<book> bookList_book_name;
extern BlockList<book> bookList_ISBN;
extern BlockList<book> bookList_author;
book::book() {

}
void book::toKeyword(const string &t) {
    int l=t.length();int j=0;int cnt=0;
    for(int i=0;i<l;i++){
        if(t[i]=='|'){
           strcpy(keyword[cnt++],t.substr(j,i-j).c_str());j=i+1;
        }
    }
    strcpy(keyword[cnt++],t.substr(j).c_str());
    keys=cnt;
}

book::book(const book &ob) {
    strcpy(ISBN,ob.ISBN);
    strcpy(author,ob.author);
    strcpy(book_name,ob.book_name);
    strcpy(price,ob.price);
    amount=ob.amount;
    keys=ob.keys;
    for(int i=0;i<keys;i++){
        strcpy(keyword[i],ob.keyword[i]);
    }
}

book::book(const string &i, const string &n="", const string &a="", const string &k="", int q=0, const string &p="0") {
    strcpy(ISBN,i.c_str());
    strcpy(book_name,n.c_str());
    strcpy(author,a.c_str());
    strcpy(price,p.c_str());
    toKeyword(k);
    amount=q;
}

book &book::operator=(const book &ob) {
    if(this==&ob)return *this;
    strcpy(ISBN,ob.ISBN);
    strcpy(author,ob.author);
    strcpy(book_name,ob.book_name);
    strcpy(price,ob.price);
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
   os<<rhs.ISBN<<'\t'<<rhs.book_name<<'\t';
   for(int i=0;i<rhs.keys-1;i++){
       os<<rhs.keyword[i]<<'|';
   }os<<rhs.keyword[rhs.keys-1]<<'\t';
   os<<rhs.price<<'\t'<<rhs.amount<<'\t'<<'\n';

   return os;
}

float book::getPrice(){
    return toFloat(price);
}

void book::changeIsbn(string s) {
    bookList_ISBN.Delete(ISBN,*this);
    bookList_book_name.Delete(book_name,*this);
    bookList_author.Delete(author,*this);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],*this);
    strcpy(ISBN,s.c_str());
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],*this);
    bookList_author.insert(author,*this);
    bookList_book_name.insert(book_name,*this);
    bookList_ISBN.insert(ISBN,*this);
}

void book::changeBookName(string s) {

    bookList_ISBN.Delete(ISBN,*this);
    bookList_book_name.Delete(book_name,*this);
    bookList_author.Delete(author,*this);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],*this);
    strcpy(book_name,s.c_str());
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],*this);
    bookList_author.insert(author,*this);
    bookList_book_name.insert(book_name,*this);
    bookList_ISBN.insert(ISBN,*this);
}

void book::changeAuthor(string s) {
    bookList_ISBN.Delete(ISBN,*this);
    bookList_book_name.Delete(book_name,*this);
    bookList_author.Delete(author,*this);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],*this);
    strcpy(author,s.c_str());
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],*this);
    bookList_author.insert(author,*this);
    bookList_book_name.insert(book_name,*this);
    bookList_ISBN.insert(ISBN,*this);
}

void book::changeKeyword(string s) {
    bookList_ISBN.Delete(ISBN,*this);
    bookList_book_name.Delete(book_name,*this);
    bookList_author.Delete(author,*this);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],*this);
    toKeyword(s);
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],*this);
    bookList_author.insert(author,*this);
    bookList_book_name.insert(book_name,*this);
    bookList_ISBN.insert(ISBN,*this);
}

void book::addAmount(int x) {
    if(amount+x<0)error("sold out!");
    bookList_ISBN.Delete(ISBN,*this);
    bookList_book_name.Delete(book_name,*this);
    bookList_author.Delete(author,*this);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],*this);
    amount+=x;
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],*this);
    bookList_author.insert(author,*this);
    bookList_book_name.insert(book_name,*this);
    bookList_ISBN.insert(ISBN,*this);
}

void book::changePrice(string s) {
    bookList_ISBN.Delete(ISBN,*this);
    bookList_book_name.Delete(book_name,*this);
    bookList_author.Delete(author,*this);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],*this);
    strcpy(price,s.c_str());
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],*this);
    bookList_author.insert(author,*this);
    bookList_book_name.insert(book_name,*this);
    bookList_ISBN.insert(ISBN,*this);
}

#endif //MAIN_CPP_BOOK_HPP
