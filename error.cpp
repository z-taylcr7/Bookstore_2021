//
// Created by Cristiano on 2021/12/24.
//

#include "error.hpp"

using namespace std;
errorException::errorException(const string& message){
    this->message=message;
}

const char *errorException::what() const throw() {
    return ("Error: "+message).c_str();
}

errorException::~errorException() throw() {

}

const string &errorException::getMessage() const {
    return message;
}

void error(string msg) {
    throw errorException(msg);
}
