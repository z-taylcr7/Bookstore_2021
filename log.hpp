//
// Created by Cristiano on 2021/12/26.
//

#ifndef BOOKSTORAGE_CBP_LOG_HPP
#define BOOKSTORAGE_CBP_LOG_HPP
class log{
    char line[100];
public:


};
class trade{
    double in;
    double out;
public:
    trade()=default;
    trade(double x,double y):in(x),out(y){};
    trade& operator+=(const trade& rhs){
        in+=rhs.in;
        out+=rhs.out;
        return *this;
    };
    friend ostream& operator<<(ostream&os,const trade& rhs);
};

ostream &operator<<(ostream &os, const trade &rhs) {
    os<<fixed<<setprecision(2)<<"+ "<<rhs.in<<" - "<<rhs.out;
    return os;
}

#endif //BOOKSTORAGE_CBP_LOG_HPP
