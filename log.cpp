//
// Created by Cristiano on 2022/1/6.
//

#include "log.hpp"
#include <iostream>
using  namespace std;
const char *log::getOperatorID() const {
    return operatorID;
}

const char *log::getLine() const {
    return line;
}

log::log(char p,const string &user, const string &lines) {
    pr=p;
    strcpy(operatorID,user.c_str());
    strcpy(line,lines.c_str());
}

ostream &operator<<(ostream &os, const log &log) {
    os << "User: "<<log.getPr()<<')'<< log.operatorID << " " << log.line;
    return os;
}

char log::getPr() const {
    return pr;
}

ostream &operator<<(ostream &os, const trade &rhs) {
    os<<fixed<<setprecision(2)<<"+ "<<rhs.in<<" - "<<rhs.out;
    return os;
}
trade::trade() {
    in=0;out=0;
}
trade &trade::operator+=(const trade &rhs) {
    in+=rhs.in;
    out+=rhs.out;
    return *this;
}