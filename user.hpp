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
#include "formula.hpp"
#include "book.hpp"
#include "error.hpp"
#include "memory.hpp"
#include "log.hpp"
using namespace std;

class user{
private:
char id[30]={' '};
char password[30]={' '};
char name[30]={' '} ;
char pr='0';
public:
    user();

    user(const user&obj);

    explicit user(int x);

    user(const string& _id,const string& _password,const string& _name,const char&_pr);

    void allocate(TokenScanner& s);

    void su(TokenScanner& s) const;

    void logout(TokenScanner& s);

    void register_account(TokenScanner& s);

    void passwd(TokenScanner& s) const;

    void useradd(TokenScanner& s) const;

    void Delete(TokenScanner& s);

    //******************BOOKS***************//

    void show(TokenScanner& s);

    void select(TokenScanner& s);

    void modify(TokenScanner& s);

    void buy(TokenScanner& s);

    void import(TokenScanner &s);

    void express(const string& s,string &type,string &token);

    //*********diaries***************//
    void showFinance(const string& times);

    static void addIncome(float x);

    static void addExpense(float x);

    //**********operators*****************//
    friend ostream &operator<<(ostream &os, const user &user);

    friend istream &operator>>(istream &is,  user &user);

    bool operator<(const user&obj);

    bool operator==(const user&obj);

    user& operator=(const user&obj);

    ~user();
};

//********USER-Memory***********//
BlockList<user> userList;
vector<user>log_stack;
static user currentUser;
Memo<trade> finance;
//***************Book-Memory****************//
BlockList<book> bookList_ISBN;
BlockList<book> bookList_keyword;
BlockList<book> bookList_author;
BlockList<book> bookList_book_name;

book currentBook;
bool selected=false;
user::user()=default;
user::user(int x){
    pr='0';
    string _id;
    string _password;
    string _name;
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}
user::user(const string& _id,const string& _password,const string& _name,const char&_pr ){
    if(!isUserName(_name)||!(isID(_id))||!isPassword(_password)){
        error("input error!");
    }
    pr=_pr;
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}
user::user(const user&obj){
    strcpy(id,obj.id);
    strcpy(name,obj.name);
    strcpy(password,obj.password);
    pr=obj.pr;
}
void user::allocate(TokenScanner& s){
    string cmd;
    cmd=s.firstToken();
    if(cmd=="su")su(s);
    if(cmd=="logout")logout(s);
    if(cmd=="register")register_account(s);
    if(cmd=="passwd")passwd(s);
    if(cmd=="useradd")useradd(s);
    if(cmd=="delete")Delete(s);
    if(cmd=="show")show(s);
    if(cmd=="buy")buy(s);
    if(cmd=="import")import(s);
    if(cmd=="select")select(s);
    if(cmd=="modify")modify(s);
}

void user::su(TokenScanner& s) const{
    user user1;
    if(userList.findOne(s.nextToken(),user1)) {
        if (pr <= user1.pr) {
            if(!s.hasMoreToken())error("password required!");
            string st = s.nextToken();
            char *tmp = new char[30];
            strcpy(tmp, st.c_str());
            if (strcmp(tmp, user1.password)!=0) {
                error("wrong password!");//todo error：password error
            }
            delete[]tmp;
        }
        log_stack.push_back(user1);
        currentUser = user1;
        book book1;
        currentBook=book1;selected=false;
    }else{
        error("account not found!");//todo:cant find error
    }
}
void user::logout(TokenScanner& s) {
        log_stack.pop_back();
        if(log_stack.empty())log_stack.push_back(user(0));
        currentUser=log_stack.back();
        book book1;
        currentBook=book1;selected=false;
}
void user::register_account(TokenScanner& s){
    string _id=s.nextToken();
    string _pw=s.nextToken();
    string _name=s.nextToken();
    if(!isUserName(_name)||!(isID(_id))||!isPassword(_pw)){
        error("input error!");//todo:input error
    }
    user user1(_id,_pw,_name,'1');
    if(userList.findOne(_id,user1)){
        error("account has been registered!");//todo:reRegister error
    }else{
        userList.insert(_id,user1);
    }
}
void user::passwd(TokenScanner& s) const{
    if(pr=='7'){
        user user1;
        if(userList.findOne(s.nextToken(),user1)){
            userList.Delete(user1.id,user1);
            strcpy(user1.password,s.nextToken().c_str());
            userList.insert(user1.id,user1);
         }else {
            error("account not found!");//todo:cannot find user error
        }
    }else{
        user user1;
        if(userList.findOne(s.nextToken(),user1)){
            if(strcmp(s.nextToken().c_str(),user1.password)!=0){
               error("wrong password!"); //todo:password error
            }
            userList.Delete(user1.id,user1);
            strcpy(user1.password,s.nextToken().c_str());
            userList.insert(user1.id,user1);
        }else {
            error("account not found!");//todo:cannot find user error
        }
    }
}
void user::useradd(TokenScanner& s) const {
    string _id=s.nextToken();
    string _pw=s.nextToken();
    string _pr=s.nextToken();
    string _name=s.nextToken();
    user user1(_id,_pw,_name,_pr[0]);
    if(!isUserName(_name)||!(isID(_id))||!isPassword(_pw)||!isPriority(_pr)){
        cout<<"input error"<<endl;
        error("input error!");//todo:input error
    }else {
        if(_pr[0]>pr)error("this operation is beyond your priority.");//todo::priority error;
        if(userList.findOne(_id,user1)){
            error("account has been registered!");//todo:re register ;
        }
        userList.insert(_id,user1);
    }
}
void user::Delete(TokenScanner& s) {
    string _id=s.nextToken();
    user tmp;
    if(!userList.findOne(_id,tmp)){
        error("account not found!");//todo:cannot find
    }
    for(auto it:log_stack){
        if(strcmp(it.id,_id.c_str())==0){
            error("the account has logged in.");//todo:logged in
        }
    }
    userList.Delete(_id,tmp);
}
bool user::operator<(const user&obj){
    return (strcmp(obj.id,id)>0);
}
bool user::operator==(const user &obj) {
    return(strcmp(id,obj.id)==0);
}
user &user::operator=(const user &obj) {
    if(this==&obj)return *this;
    strcpy(id,obj.id);
    strcpy(name,obj.name);
    strcpy(password,obj.password);
    pr=obj.pr;
    return *this;
}

void user::show(TokenScanner &s) {
    if(pr=='0')error("you don't have the priority. Sign in first");
    if(!s.hasMoreToken()){bookList_ISBN.scroll();return;}
    string t=s.nextToken();
    if(t=="finance"){
        if(s.hasMoreToken())showFinance(s.nextToken());
        else showFinance("all");
        return;
    }
    t=t.substr(1);
    string type;
    string token;
    int len=t.length();int i=0;
    while(i<len){
        if(t[i]=='='){
            type=t.substr(0,i);
            token=t.substr(i+1);
            break;
        }
        i++;
    }
    if(i==len)error("token required.");
    if(type=="ISBN")bookList_ISBN.find(token);
    token=token.substr(1,token.length()-2);
    if(type=="name")bookList_book_name.find(token);
    if(type=="author")bookList_author.find(token);
    if(type=="keyword"){
        int len=token.length();int i=0;
        while(i<len){if(t[i]=='|')error("only one keyword is required.");i++;}
        bookList_keyword.find(token);
    }
}

void user::select(TokenScanner &s) {
    string Isbn=s.nextToken();
    book book1(Isbn);
    if(!bookList_ISBN.findOne(Isbn,book1)){
        bookList_ISBN.insert(Isbn,book1);
    }
    currentBook=book1;
    selected=true;
}

void user::modify(TokenScanner &s) {
    if(!selected)error("please select a book first!");
    while(s.hasMoreToken()){
        string t=s.nextToken().substr(1);
        string type;
        string token;
        int len=t.length();int i=0;
        while(i<len){
            if(t[i]=='='){
                type=t.substr(0,i);
                token=t.substr(i+1);
                break;
            }
            i++;
        }
        if(i==len)error("token required.");
        if(type=="ISBN"){
            token=token.substr(1,token.length()-2);
            string Isbn=token;
            book book1(Isbn);
            if(bookList_ISBN.findOne(Isbn,book1))error("ISBN should be unique.");
            currentBook.changeIsbn(token);
        }
        if(type=="name") {
            token=token.substr(1,token.length()-2);
            currentBook.changeBookName(token);
        }
        if(type=="author") {
            token=token.substr(1,token.length()-2);
            currentBook.changeAuthor(token);
        }
        if(type=="keyword"){
            token=token.substr(1,token.length()-2);
            currentBook.changeKeyword(token);
        }
        if(type=="price"){
            currentBook.changePrice(token);
        }
    }
}

void user::buy(TokenScanner &s) {
    book book1;
    if(bookList_ISBN.findOne(s.nextToken(),book1)){
        int quantity= toNumber(s.nextToken());
        book1.addAmount(-quantity);
        float price=quantity*book1.getPrice();
        cout<<fixed<<setprecision(2)<<price<<endl;
        //todo:log data.
        trade log(price,0);
        finance.write(log);
    }else{
        error("book not found!");
    }
}

void user::import(TokenScanner &s) {
    if(!selected)error("select a book first.");
        int quantity= toNumber(s.nextToken());
        currentBook.addAmount(quantity);
        float price= toFloat(s.nextToken());
        //todo:log data.
        trade log(0,price);
        finance.write(log);
}

void user::express(const string &s, string &type, string &token) {

}

ostream &operator<<(ostream &os, const user &user) {
    os << "name is: " << user.name;
    return os;
}

istream &operator>>(istream &is, user &user1) {
    string id,pw,name;
    is>>id>>pw>>name;
    user user2(id,pw,name,'1');
    user1=user2;
    return is;
}

void user::showFinance(const string& times) {
    if(pr!='7')error("You're not allowed to see this.");
    fstream file;
    file.open("trade_data");
    trade sum(0,0);
    int d=finance.getIndexMax();
    if(times=="all") {
        while (d > 5) {
            trade tmp;
            finance.read(tmp, d);
            sum += tmp;
            d -= sizeof(trade);
        }
        cout<<sum<<endl;
        return;
    }
    int t=toNumber(times);
    while(t--&&d>5){

        trade tmp;
        finance.read(tmp,d);
        sum+=tmp;
        d-=sizeof(trade);
    }
    cout<<sum<<endl;
}



user::~user()=default;


#endif //MAIN_CPP_USER_HPP