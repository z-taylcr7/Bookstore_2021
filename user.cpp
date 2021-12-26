//
// Created by Cristiano on 2021/12/23.
//
#include"user.hpp"
#include "token_scanner.hpp"
#include "formula.hpp"
#include "memory.hpp"
#include "MemoryTemplate.hpp"
BlockList<user> userList;
vector<user>log_stack;
static user currentUser;
string rootID="yyu";


user::user(){}
user::user(int x){
    pr='0';
    string _id="";
    string _password="";
    string _name="";
    id=new char[30];
    name=new char[30];
    password=new char[30];
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}
user::user(const string& _id,const string& _password,const string& _name,const char&_pr ){
    if(!isUserName(_name)||!(isID(_id))||!isPassword(_password)){
        //todo:input error
    }
    pr=_pr;
    id=new char[30];
    name=new char[30];
    password=new char[30];
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}

user::user(const user&obj){
    id=new char[30];
    name=new char[30];
    password=new char[30];
    strcpy(id,obj.id);
    strcpy(name,obj.name);
    strcpy(password,obj.password);
    pr=obj.pr;
};

void user::su(TokenScanner& s){
    user user1;
    if(userList.findOne(s.nextToken(),user1)) {
        if (pr <= user1.pr) {
            string st = s.nextToken();
            char *tmp = new char[30];
            strcpy(tmp, st.c_str());
            if (strcmp(tmp, user1.password)) {
                cout << "error1";//todo error：password error
            }
            delete[]tmp;
        }
        log_stack.push_back(user1);
        currentUser = user1;
    }else{
        //todo:cant find error
    }
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
}

void user::logout(TokenScanner& s) {
    log_stack.pop_back();
    if(log_stack.empty())log_stack.push_back(user(0));
    currentUser=log_stack.back();

}
void user::register_account(TokenScanner& s){
    string _id=s.nextToken();
    string _pw=s.nextToken();
    string _name=s.nextToken();
    if(!isUserName(_name)||!(isID(_id))||!isPassword(_pw)){
        //todo:input error
    }
    user user1(_id,_pw,_name,'1');
    if(userList.findOne(_id,user1)){
        //todo:reRegister error
    }else{
        userList.insert(_id,user1);
    }
}
void user::passwd(TokenScanner& s){
    if(pr=='7'){
        user user1;
        if(userList.findOne(s.nextToken(),user1)){
            user1.password=const_cast<char*>(s.nextToken().c_str());
            userList.update(user1.id,user1);
        }else {
            //todo:cannot find user error
        }
    }else{
        user user1;
        if(userList.findOne(s.nextToken(),user1)){
            if(strcmp(s.nextToken().c_str(),user1.password)!=0){
                //todo:password error
            }
            user1.password=const_cast<char*>(s.nextToken().c_str());
            userList.update(user1.id,user1);
        }else {
            //todo:cannot find user error
        }
    }
}

void user::useradd(TokenScanner& s){
    string _id=s.nextToken();
    string _pw=s.nextToken();
    string _pr=s.nextToken();
    string _name=s.nextToken();
    user user1(_id,_pw,_name,_pr[0]);
    if(!isUserName(_name)||!(isID(_id))||!isPassword(_pw)||!isPriority(_pr)){
        //todo:input error
    }else {
        if(_pr[0]>pr){}//todo::priority error;
        if(userList.findOne(s.nextToken(),user1)){}//todo:cannot find;
        userList.insert(_id,user1);
    }
}

void user::Delete(TokenScanner& s) {
    string _id=s.nextToken();
    user tmp;
    if(!userList.findOne(_id,tmp)){
        //todo:cannot find
    }
    for(auto it:log_stack){
        if(strcmp(it.id,_id.c_str())==0){
            //todo:logged in
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
    delete[] id;
    delete[] name;
    delete[] password;
    id=new char[30];
    name=new char[30];
    password=new char[30];
    strcpy(id,obj.id);
    strcpy(name,obj.name);
    strcpy(password,obj.password);
    pr=obj.pr;
    return *this;
}

user::~user(){
    delete id;
    delete name;
    delete password;
}
