//
// Created by Cristiano on 2021/12/26.
//

#ifndef BOOKSTORE_CBP_LOG_HPP
#define BOOKSTORE_CBP_LOG_HPP

#include <ostream>
#include <cstring>
#include <iomanip>
using namespace std;
class log{
    char operatorID[32];
    char pr;
    char line[200];
public:
    log(){
        pr='0';
        memset(&operatorID,0,sizeof(operatorID));
        memset(&line,0,sizeof(line));
    };

    const char *getOperatorID() const;

    char getPr() const;

    const char *getLine() const;

    log(char pr,const string& user,const string& lines);

    friend ostream &operator<<(ostream &os, const log &log);
};

class trade{
    double in;
    double out;
public:
    trade();
    trade(double x,double y):in(x),out(y){}
    trade& operator+=(const trade& rhs);
    friend ostream& operator<<(ostream&os,const trade& rhs);
};



#endif //BOOKSTORE_CBP_LOG_HPP
