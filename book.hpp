//
// Created by Cristiano on 2021/12/22.
//

#ifndef MAIN_CPP_BOOK_HPP
#define MAIN_CPP_BOOK_HPP
#include <string>
#include <iomanip>


extern int currentOffset;
class book{

public:
    book();

    void setModification(book& mod) ;

    void changeIsbn(string s) ;

    void changeBookName(string s) ;

    void changeAuthor(string s) ;

    void changeKeyword(string s);

    void changePrice(string s);

    void addAmount(int x);

    float getPrice();

private:
    char ISBN[22] ;
    char book_name[62];
    char author[62];
    char keyword[62][62];
    int amount;
    int keys;
    char price[14] ;
public:
//构造，拷贝构造，=，<,==,!=

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
            strcpy(keyword[cnt++],t.substr(j,i-j).c_str());j=i+1;
        }
    }
    strcpy(keyword[cnt++],t.substr(j).c_str());
    keys=cnt;
}

book::book(const book &ob) {

    memset(&keyword,0,sizeof(keyword));
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

book::book(const string &i, const string &n="", const string &a="", const string &k="", int q=0, const string &p="0.00") {
    memset(&keyword,0,sizeof(keyword));
    strcpy(ISBN,i.c_str());
    strcpy(book_name,n.c_str());
    strcpy(author,a.c_str());
    strcpy(price,p.c_str());
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
    os<<rhs.ISBN<<'\t'<<rhs.book_name<<'\t'<<rhs.author<<'\t';
    for(int i=0;i<rhs.keys-1;i++){
        os<<rhs.keyword[i]<<'|';
    }
    if(rhs.keys>0)os<<rhs.keyword[rhs.keys-1]<<'\t';
    os<<fixed<<setprecision(2)<<toFloat(rhs.price)<<'\t'<<rhs.amount;
    return os;
}

float book::getPrice(){
    return toFloat(price);
}
void book::setModification( book& mod) {
    bookList_ISBN.Delete(ISBN,*this);
    bookList_book_name.Delete(book_name,*this);
    bookList_author.Delete(author, *this);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],*this);
    *this=mod;
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],*this);
    bookList_author.insert(author,*this);
    bookList_book_name.insert(book_name,*this);
    bookList_ISBN.insert(ISBN,*this);
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
    strcpy(price,s.c_str());
}
void book::addAmount(int x) {
    if(amount+x<0)error("sold out!");
    bookList_ISBN.Delete(ISBN,*this);
    bookList_book_name.Delete(book_name,*this);
    bookList_author.Delete(author, *this);
    for(int i=0;i<keys;i++)bookList_keyword.Delete(keyword[i],*this);
    amount+=x;
    for(int i=0;i<keys;i++)bookList_keyword.insert(keyword[i],*this);
    bookList_author.insert(author,*this);
    bookList_book_name.insert(book_name,*this);
    bookList_ISBN.insert(ISBN,*this);
}



#endif //MAIN_CPP_BOOK_HPP
