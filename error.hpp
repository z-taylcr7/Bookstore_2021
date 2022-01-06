 //
// Created by Cristiano on 2021/12/23.
//

#ifndef BOOKSTORE_CBP_ERROR_HPP
#define BOOKSTORE_CBP_ERROR_HPP

#include <string>

 using namespace std;
class errorException{
private:
    string message;
public:
    errorException()=default;

    errorException(const string&);

    const string &getMessage() const;

    const char* what()const throw();

    ~errorException()throw();


};

 void error(string msg);
#endif //BOOKSTORE_CBP_ERROR_HPP
