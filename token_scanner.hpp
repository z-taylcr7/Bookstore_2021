//
// Created by Cristiano on 2021/12/21.
//

#ifndef BOOKSTORE_CBP_TOKEN_SCANNER_HPP
#define BOOKSTORE_CBP_TOKEN_SCANNER_HPP
#include"error.hpp"
class TokenScanner{
private:
    struct Node{
        string token;
        Node*next;
    };
    Node* head;
    Node* cur;
public:
    TokenScanner(string exp);

    string firstToken();

    bool hasMoreToken();

    string currentToken();

    string nextToken();

    void divide(const string& s);

    ~TokenScanner();
};

#endif //BOOKSTORE_CBP_TOKEN_SCANNER_HPP
