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
    char secProblem[32]={' '};
    char secAnswer[32]={' '};
    char name[32]={' '} ;
    char pr='0';
public:
    user();

    user(const user&obj);

    explicit user(int x);

    user(const string& _id,const string& _password,const string& _name,const string&_pr);

    void allocate(int cmd);

    void su() const;

    void logout() const;

    void register_account();

    void passwd() const;

    void useradd() const;

    void Delete() const;

    //******************BOOKS***************//

    void show();

    void select();

    void modify() const;

    void buy() const;

    void import() const;

    //*********diaries***************//
    void showFinance(int times);

    void Report();

    void reportMyself();

    void reportFinance();

    void reportEmployee();

    void Log()const ;

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
