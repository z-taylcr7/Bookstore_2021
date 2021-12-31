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

    void import(TokenScanner &s) const;

    //*********diaries***************//
    void showFinance(const string& times);


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
vector<pair<int,int>>log_stack;
static user currentUser;
Memo<trade> finance;
Memo<book> library;
//***************Book-Memory****************//
BlockList<book> bookList_ISBN;
BlockList<book> bookList_keyword;
BlockList<book> bookList_author;
BlockList<book> bookList_book_name;

book currentBook;
int currentOffset;
bool selected=false;
user::user(){
    pr='0';
    string _id;
    string _password;
    string _name;
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
};
user::user(int x){
    pr='0';
    string _id;
    string _password;
    string _name;
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}
user::user(const string& _id,const string& _password,const string& _name,const string&_pr ){
    if(!isUserName(_name)||!(isID(_id))||!isPassword(_password)||!isPriority(_pr)){
        error("input error!");
    }
    pr=_pr[0];
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
    if(cmd=="su"){su(s);return;}
    if(cmd=="logout"){logout(s);return;}
    if(cmd=="register"){register_account(s);return;}
    if(cmd=="passwd"){passwd(s);return;}
    if(cmd=="useradd"){useradd(s);return;}
    if(cmd=="delete"){Delete(s);return;}
    if(cmd=="show"){show(s);return;}
    if(cmd=="buy"){buy(s);return;}
    if(cmd=="import"){import(s);return;}
    if(cmd=="select"){select(s);return;}
    if(cmd=="modify"){modify(s);return;}
    error("no command found!");
}

void user::su(TokenScanner& s) const{
    user user1;
    if(int x=userList.findOne(s.nextToken(),user1)) {
        if (pr <= user1.pr) {
            if(!s.hasMoreToken())error("password required!");
            string st = s.nextToken();
            if(s.hasMoreToken())error("token too much!");
            if (strcmp(st.c_str(), user1.password)!=0) {
                error("wrong password!");//todo error：password error
            }
        }
        if(!log_stack.empty()){
            log_stack.pop_back();
            int of1=userList.findOne(currentUser.id,currentUser);
            int of2=currentOffset;
            log_stack.push_back(make_pair(of1,of2));
        }
        book book1;
        currentUser = user1;
        currentBook=book1;
        log_stack.push_back(make_pair(x,0));
        selected=false;
    }else{
        error("account not found!");//todo:cant find error
    }
}
void user::logout(TokenScanner& s) {
    if(s.hasMoreToken())error("token too much!");
    if(log_stack.empty())error("no account!");
    if(pr=='0')error("beyond your priority");
    log_stack.pop_back();
    if(log_stack.empty()){
        user user1;book book1;
        currentUser=user1;
        currentBook=book1;
        currentOffset=0;return;
    }
    currentUser=userList.read(log_stack.back().first);
    currentOffset=log_stack.back().second;
    library.read(currentBook,currentOffset);
    selected=(log_stack.back().second!=0);
}
void user::register_account(TokenScanner& s){
    string _id=s.nextToken();
    string _pw=s.nextToken();
    string _name=s.nextToken();
    if(s.hasMoreToken())error("token too much!");
    user user1(_id,_pw,_name,"1");
    if(userList.findOne(_id,user1)){
        error("account has been registered!");//todo:reRegister error
    }else{
        int x=userList.data.write(user1);
        userList.insert(_id,x);
    }
}
void user::passwd(TokenScanner& s) const{
    if(pr=='0')error("please create an account!");
    if(pr=='7'){
        user user1;
        if(int of=userList.findOne(s.nextToken(),user1)){
            string pw=s.nextToken();
            if(!isPassword(pw))error("invalid password");
            strcpy(user1.password,pw.c_str());
            userList.data.update(user1,of);
        }else {
            error("account not found:");//todo:cannot find user error
        }
    }else{
        user user1;
        if(int of=userList.findOne(s.nextToken(),user1)){
            if(strcmp(s.nextToken().c_str(),user1.password)!=0){
                error("wrong password!"); //todo:password error
            }
            string pw=s.nextToken();
            if(!isPassword(pw))error("invalid password");
            strcpy(user1.password,pw.c_str());
            userList.data.update(user1,of);
        }else {
            error("account not found!");//todo:cannot find user error
        }
    }
}
void user::useradd(TokenScanner& s) const {
    if(pr=='0'||pr=='1')error("you can't even do this!");
    string _id=s.nextToken();
    string _pw=s.nextToken();
    string _pr=s.nextToken();
    string _name=s.nextToken();
    if(s.hasMoreToken())error("token too much!");
    user user1(_id,_pw,_name,_pr);
    if(_pr[0]>pr||_pr[0]==pr)error("this operation is beyond your priority.");//todo::priority error;
    if(userList.findOne(_id,user1))error("account has been registered!");//todo:re register ;
    int u=userList.data.write(user1);
    userList.insert(_id,u);
}

void user::Delete(TokenScanner& s) {
    if(pr!='7')error("only superUser can delete an account.");
    string _id=s.nextToken();
    if(!isID(_id))error("invalid ID");
    if(s.hasMoreToken())error("token too much!");
    user tmp;
    int of=userList.findOne(_id,tmp);
    if(!of){
        error("account not found!");//todo:cannot find
    }
    for(auto it:log_stack){
        if(strcmp(userList.read(it.first).id,_id.c_str())==0){
            error("the account has logged in.");//todo:logged in
        }
    }
    userList.Delete(_id,of);
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
    if(token.length()==0)error("no token");
    if(i==len)error("token required.");
    if(type[0]!='-')error("formula error'-'");
    type=type.substr(1);
    if(type=="ISBN"){
        if(!isISBN(token))error("wrong ISBN");
        bookList_ISBN.find(token);return;
    }
    if(token[0]!='"'||token[token.length()-1]!='"')error("formula error");
    token=token.substr(1,token.length()-2);
    if(token.length()==0)error("no token");
    if(type=="name"){
        if(!isBookName(token))error("wrong book name");
        bookList_book_name.find(token);
    }
    if(type=="author"){
        if(!isBookName(token))error("wrong author");
        bookList_author.find(token);
    }
    if(type=="keyword"){
        int len=token.length();int i=0;
        if(len>60)error("wrong keyword");
        while(i<len){if(t[i]=='|')error("only one keyword is required.");i++;}
        bookList_keyword.find(token);
    }
}

void user::select(TokenScanner &s) {
    if(pr=='0'||pr=='1'){
        error("you don't have the priority. Sign in first if you wanna select.");
    }
    string Isbn=s.nextToken();
    if(!isISBN(Isbn))error("too long for isbn");
    book book1(Isbn);
    int x=bookList_ISBN.findOne(Isbn,book1);
    if(!x){
        x=library.write(book1);
        bookList_ISBN.insert(Isbn,x);
    }
    currentOffset=x;
    currentBook=book1;
    int y=log_stack.back().first;
    log_stack.pop_back();
    log_stack.push_back(make_pair(y,x));
    selected=true;
}

void user::modify(TokenScanner &s){
    if(pr=='0'||pr=='1'){error("you don't have the priority. Sign in first if you want to modify.");return;}
    if(!selected)error("please select a book first!");
    if(log_stack.back().second==0)error(" 00");
    book mod;
    library.read(mod,log_stack.back().second);
    bool a= false,n=false,isbn=false,k=false,p=false;
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
            if(isbn)error("0");
            if(token=="")error("0-0");
            if(!isISBN(token))error("wrong isbn");
            string Isbn=token;
            book book1(Isbn);
            if(bookList_ISBN.findOne(Isbn,book1))error("ISBN should be unique.");
            isbn=true;
            mod.changeIsbn(token);
        }
        if(type=="name") {
            if(n)error("name repeat");
            token=token.substr(1,token.length()-2);
            if(token=="")error("0-0");
            if(!isBookName(token))error("wrong name");
            n=true;
            mod.changeBookName(token);
        }
        if(type=="author") {
            token=token.substr(1,token.length()-2);
            if(a)error("author repeat");
            if(token=="")error("0-0");
            if(!isBookName(token))error("wrong author");
            a=true;
            mod.changeAuthor(token);
        }
        if(type=="keyword"){
            if(k)error("no more keyword");
            token=token.substr(1,token.length()-2);
            if(token=="")error("0-0");
            if(!isBookName(token))error("wrong keyword");
            k=true;
            mod.changeKeyword(token);
        }
        if(type=="price"){
            if(p)error("no price anymore.");
            if(token=="")error("0-0");
            if(!isPrice(token))error("wrong price");
            p=true;
            mod.changePrice(token);
        }
        if(type!="ISBN"&&type!="keyword"&&type!="author"&&type!="price"&&type!="name")error("type error");
    }
    currentBook.setModification(mod,currentOffset);
    currentBook=mod;
}

void user::buy(TokenScanner &s) {
    if(pr=='0')error("beyond your priority");
    string key=s.nextToken();
    if(!isISBN(key))error("invalid isbn");
    book book1;
    if(int ofs=bookList_ISBN.findOne(key,book1)){
        if(finance.getIndexMax()>5+sizeof(trade)*2147483647)error("full");
        long long int quantity=stoll(s.nextToken());
        book1.addAmount(-quantity);
        library.update(book1,ofs);
        double price=book1.getPrice()*quantity;
        cout<<fixed<<setprecision(2)<<book1.getPrice()*quantity<<'\n';
        //todo:log data.
        trade log(price,0);
        finance.write(log);
    }else{
        error("book not found!");
    }
}

void user::import(TokenScanner &s) const {
    if(pr=='0'||pr=='1')error("you don't have the priority. Sign in first if you want to import.");
    if(!selected)error("select a book first.");
    int quantity= toNumber(s.nextToken());
    currentBook.addAmount(quantity);
    double price= toFloat(s.nextToken());
    //todo:log data.
    trade log(0,price);
    finance.write(log);
    library.update(currentBook,currentOffset);
}

ostream &operator<<(ostream &os, const user &user) {
    os <<user.pr<< ')' << user.id<<"    ";
    return os;
}

istream &operator>>(istream &is, user &user1) {
    string id,pw,name;
    is>>id>>pw>>name;
    user user2(id,pw,name,"1");
    user1=user2;
    return is;
}

void user::showFinance(const string& times) {
    if(pr!='7')error("You're not allowed to see this.");
    fstream file;
    if(times=="0"){
        cout<<'\n';return;
    }
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
        cout<<fixed<<setprecision(2)<<sum<<'\n';
        return;
    }
    int t=toNumber(times);
    if(t>(d-5)/sizeof(trade))error("trades limited.");
    while(t--&&d>5){
        trade tmp;
        finance.read(tmp,d);
        sum+=tmp;
        d-=sizeof(trade);
    }
    cout<<sum<<'\n';
}



user::~user()=default;


#endif //MAIN_CPP_USER_HPP
