//
// Created by Cristiano on 2021/12/21.
//
//visitor account:user account(0);
//default account:user account;

#ifndef MAIN_CPP_USER_HPP
#define MAIN_CPP_USER_HPP

#include <ostream>
#include<cstring>
#include <iomanip>
#include "token_scanner.hpp"
#include "book.hpp"
#include "error.hpp"
#include "memo.hpp"
#include "log.hpp"
#include"blocklist.hpp"
using namespace std;

class user{
private:
    char id[32]={' '};
    char password[32]={' '};
    char name[32]={' '} ;
    char pr='0';
public:
    user();

    user(const user&obj);

    explicit user(int x);

    user(const string& _id,const string& _password,const string& _name,const string&_pr);

    void allocate(TokenScanner& s);

    void su(TokenScanner& s) const;

    void logout(TokenScanner& s) const;

    void register_account(TokenScanner& s);

    void passwd(TokenScanner& s) const;

    void useradd(TokenScanner& s) const;

    void Delete(TokenScanner& s) const;

    //******************BOOKS***************//

    void show(TokenScanner& s);

    void select(TokenScanner& s);

    void modify(TokenScanner& s) const;

    void buy(TokenScanner& s) const;

    void import(TokenScanner &s) const;

    //*********diaries***************//
    void showFinance(const string& times);

    void Report(TokenScanner& s);

    void reportMyself();

    void reportFinance();

    void reportEmployee();

    void Log(TokenScanner &s)const ;

    //**********operators*****************//
    friend ostream &operator<<(ostream &os, const user &user);

    friend istream &operator>>(istream &is,  user &user);

    bool operator<(const user&obj);

    bool operator==(const user&obj);

    user& operator=(const user&obj);

    ~user();
};
class Line{
private:
    user usr;
    char script[200];
public:
    Line(const user &usr, string script);

    Line();

    user &getUsr();

    const char *getScript() const;

    friend ostream &operator<<(ostream &os, const Line &line);
};



#endif //MAIN_CPP_USER_HPP
