//
// Created by Cristiano on 2021/12/22.
//

#ifndef MAIN_CPP_BOOK_HPP
#define MAIN_CPP_BOOK_HPP
#include <string>
#include <iomanip>
using namespace std;

class book{
private:
    char ISBN[22] ;
    char book_name[62];
    char author[62];
    char keyword[62][62];
    int amount;
    int keys;
    double price ;
public:
    book();

    void setModification(book& mod,int& ofs) ;

    void changeIsbn(string s) ;

    void changeBookName(string s) ;

    void changeAuthor(string s) ;

    void changeKeyword(string s);

    void changePrice(string s);

    void addAmount(int x);

    double getPrice();

//构造，拷贝构造，=，<,==,!=

    book(const book &ob);

    book(const string&i,const string&n="",const string&a="",const string&k="",int q=0,const string&p="0.00");

    void toKeyword(const string&t);

    book &operator=(const book& obj);

    bool operator==(const book& rhs);

    bool operator!=(const book& rhs);

    bool operator<(const book& rhs);

    friend ostream& operator<<(ostream&os,const book& rhs);

    const char *getBookName() const;

    const char *getIsbn() const;

};



#endif //MAIN_CPP_BOOK_HPP
