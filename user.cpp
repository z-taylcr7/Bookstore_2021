//
// Created by Cristiano on 2022/1/3.
//

#include "user.hpp"
#include "book.hpp"
#include "memo.hpp"
#include "blocklist.hpp"
#include "error.hpp"
#include "formula.hpp"
#include "global.hpp"
user::user(){
    pr='0';
    string _id;
    string _password;
    string _name;
    string _ans;
    string _pro="null";
    strcpy(secAnswer,_ans.c_str());
    strcpy(secProblem,_pro.c_str());
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}
user::user(int x){
    pr='0';
    string _id;
    string _password;
    string _name;
    string _ans;
    string _pro="null";
    strcpy(secAnswer,_ans.c_str());
    strcpy(secProblem,_pro.c_str());
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}
user::user(const string& _id,const string& _password,const string& _name,const string&_pr ){
    if(!isUserName(_name)||!(isID(_id))||!isPassword(_password)||!isPriority(_pr)){
        error("input error!");
    }
    pr=_pr[0];
    string _ans;
    string _pro="null";
    strcpy(secAnswer,_ans.c_str());
    strcpy(secProblem,_pro.c_str());
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}
user::user(const user&obj){
    strcpy(secAnswer,obj.secAnswer);
    strcpy(secProblem,obj.secProblem);
    strcpy(id,obj.id);
    strcpy(name,obj.name);
    strcpy(password,obj.password);
    pr=obj.pr;
}
void user::allocate(int cmd){
    if(cmd==1){su();return;}
    if(cmd==2){logout();return;}
    if(cmd==3){register_account();return;}
    if(cmd==4){passwd();return;}
    if(cmd==5){useradd();return;}
    if(cmd==6){Delete();return;}
    if(cmd==7){show();return;}
    if(cmd==8){buy();return;}
    if(cmd==9){import();return;}
    if(cmd==10){select();return;}
    if(cmd==11){modify();return;}
    if(cmd==12){Log();return;}
    if(cmd==13){Report();return;}
    error("no command found!");
}
void user::su() const{
    user user1;
    string token;
    cout<<"input id";cin>>token;
    if(int x=userList.findOne(token,user1)) {
        if (pr <= user1.pr) {
            string st;cout<<"input password";cin>>st;
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
    Line x(currentUser,"log in.");
    diary.write(x);
    cout<<"Welcome back, "<<currentUser<<'!'<<endl;
}
void user::logout() const {
    if(log_stack.empty())error("no account!");
    if(pr=='0')error("beyond your priority");
    log_stack.pop_back();
    cout<<"bye-bye, "<<currentUser<<'!'<<endl;
    Line x(currentUser,"log out.");
    diary.write(x);
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
void user::register_account(){
    string _id;
    cout<<"input id";cin>>_id;
    string _pw;
    cout<<"input _pw";cin>>_pw;
    string _name;
    cout<<"input _pw";cin>>_pw;
    user user1(_id,_pw,_name,"1");
    if(userList.findOne(_id,user1)){
        error("account has been registered!");//todo:reRegister error
        Line x(currentUser,"register "+_name+"FAIL-ID repetition.");
        diary.write(x);
    }else{
        int x=userList.data.write(user1);
        userList.insert(_id,x);
        Line y(currentUser,"register "+_name+" SUCCESS.");
        diary.write(y);
    }
    cout<<"SUCCESS."<<endl;
}
void user::passwd() const{
    if(pr=='0')error("please create an account!");
    if(pr=='7'){
        user user1;string _id;
        cout<<"input id";cin>>_id;
        if(int of=userList.findOne(_id,user1)){
            string _pw;
            cout<<"input _password";cin>>_pw;
            if(!isPassword(_pw))error("invalid password");
            strcpy(user1.password,_pw.c_str());
            userList.data.update(user1,of);
            Line x(currentUser,"revise password SUCCESS");
            diary.write(x);
        }else {
            Line x(currentUser,"revise password FAIL-Account not found.");
            diary.write(x);
            error("account not found:");//todo:cannot find user error
        }
    }else{
        user user1;string _id;
        cout<<"input id";cin>>_id;
        if(int of=userList.findOne(_id,user1)){
            string _pw;
            cout<<"input old password";cin>>_pw;
            if(!isPassword(_pw))error("invalid password");
            if(strcmp(_pw.c_str(),user1.password)!=0){
                Line x(currentUser,"revise password FAIL-Wrong password.");
                diary.write(x);
                error("wrong password!"); //todo:password error
            }
            cout<<"input new _password";cin>>_pw;
            if(!isPassword(_pw))error("invalid password");
            strcpy(user1.password,_pw.c_str());
            userList.data.update(user1,of);
            Line x(currentUser,"revise password SUCCESS");
            diary.write(x);
        }else {
            Line x(currentUser,"revise password FAIL-Account not found.");
            diary.write(x);
            error("account not found!");//todo:cannot find user error
        }
    }
}
void user::useradd() const {
    if(pr=='0'||pr=='1'){
        Line st(currentUser,"Useradd FAIL-Priority limited.");
        diary.write(st);
        error("you can't even do this!");
    }
    string _id;
    cout<<"input id";cin>>_id;
    string _pw;
    cout<<"input _pw";cin>>_pw;
    string _name;
    cout<<"input _pw";cin>>_pw;
    string _pr;
    cout<<"input _pr";cin>>_pr;
    user user1(_id,_pw,_name,_pr);
    if(_pr[0]>pr||_pr[0]==pr){
        Line x(currentUser,"Create account FAIL-Priority limited.");
        diary.write(x);
        error("this operation is beyond your priority.");//todo::priority error;
    }
    if(userList.findOne(_id,user1)){
        Line x(currentUser,"Create account FAIL-ID repetition.");
        diary.write(x);
        error("account has been registered!");//todo:re register ;
    }
    Line x(currentUser,"Create account SUCCESS:"+_id);
    diary.write(x);
    int u=userList.data.write(user1);
    userList.insert(_id,u);
}
void user::Delete() const {
    if(pr!='7'){
        Line st(currentUser,"Delete account FAIL-Priority limited.");
        diary.write(st);
        error("only superUser can delete an account.");
    }

    string _id;
    cout<<"input id";cin>>_id;
    if(!isID(_id))error("invalid ID");
    user tmp;
    int of=userList.findOne(_id,tmp);
    if(!of){
        Line x(currentUser,"Delete account FAIL-Account not found.");
        diary.write(x);
        error("account not found!");//todo:cannot find
    }
    for(auto it:log_stack){
        if(strcmp(userList.read(it.first).id,_id.c_str())==0){
            Line x(currentUser,"Delete account FAIL-Already logged in.");
            diary.write(x);
            error("the account has logged in.");//todo:logged in
        }
    }
    Line x(currentUser,"Delete account SUCCESS.id:"+_id);
    diary.write(x);
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

void user::show() {
    if(pr=='0')error("you don't have the priority. Sign in first");
    cout<<"input show type.\n0.show finance\n1.all books\n2.ISBN\n3.name\n4.author\n5.keyword\n";
    int cmd;cin>>cmd;
    if(cmd==1){bookList_ISBN.scroll();return;}
    if(cmd==0) {
        cout << "input times.Please input -1 to show all." << endl;
        int times;
        cin >> times;
        showFinance(times);
        return;
    }
    string token;
    if(cmd==2){
        cout<<"input ISBN";cin>>token;
        if(!isISBN(token))error("wrong ISBN");
        bookList_ISBN.find(token);return;
    }
    if(cmd==3){

        cout<<"input name";cin>>token;
        if(!isBookName(token))error("wrong book name");
        bookList_book_name.find(token);
    }
    if(cmd==4){
        cout<<"input author";cin>>token;
        if(!isBookName(token))error("wrong author");
        bookList_author.find(token);
    }
    if(cmd==5){

        cout<<"input keyword";cin>>token;
        int l=token.length();int j=0;
        if(l>60)error("wrong keyword");
        while(j<l){if(token[j]=='|')error("only one keyword is required.");j++;}
        bookList_keyword.find(token);
    }
}

void user::select() {
    if(pr=='0'||pr=='1'){
        Line st(currentUser,"Select book FAIL-Priority limited.");
        diary.write(st);
        error("you don't have the priority. Sign in first if you wanna select.");
    }
    string Isbn;
    cout<<"input ISBN";cin>>Isbn;
    if(!isISBN(Isbn))error("too long for isbn");
    book book1(Isbn);
    int x=bookList_ISBN.findOne(Isbn,book1);
    if(!x){
        x=library.write(book1);
        bookList_ISBN.insert(Isbn,x);
        Line dx(currentUser,"has created a new book,which ISBN is: "+Isbn);
        diary.write(dx);
    }
    currentOffset=x;
    currentBook=book1;
    int y=log_stack.back().first;
    log_stack.pop_back();
    log_stack.push_back(make_pair(y,x));
    selected=true;
    Line dx(currentUser,"has selected the book: "+Isbn+'('+string(book1.getBookName())+')');
    diary.write(dx);
}

void user::modify() const{
    if(pr=='0'||pr=='1'){
        Line st(currentUser,"Modify book FAIL-Priority limited.");
        diary.write(st);
        error("you don't have the priority. Sign in first if you want to modify.");return;
    }
    if(!selected){
        Line st(currentUser,"Modify book FAIL-No book selected.");
        diary.write(st);
        error("please select a book first!");
    }
    if(log_stack.back().second==0)error(" 00");
    book mod;
    library.read(mod,log_stack.back().second);
    bool a= false,n=false,isbn=false,k=false,p=false;
    cout<<"start modification.";
    while(true){
        cout<<"input type.\n0.completed!\n1.price\n2.ISBN\n3.name\n4.author\n5.keyword\n";
        int cmd;cin>>cmd;
        if(cmd==0){
            cout<<"complete modification!\n";break;
        }
        cout<<"input parameter";
        string token;cin>>token;
        if(cmd==2){
            if(isbn){
                Line st(currentUser,"Modify book FAIL-Additional parameter repeated");
                diary.write(st);
                error("0");
            }
            if(token==""){
                Line st(currentUser,"Modify book FAIL-no Additional parameter");
                diary.write(st);error("0-0");
            }
            if(!isISBN(token))error("wrong isbn");
            string Isbn=token;
            book book1(Isbn);
            if(bookList_ISBN.findOne(Isbn,book1))error("ISBN should be unique.");
            isbn=true;
            mod.changeIsbn(token);
        }
        if(cmd==3) {
            if(n){
                        Line st(currentUser,"Modify book FAIL-Additional parameter repeated");
                        diary.write(st);
                        error("name repeat");
            }
            token=token.substr(1,token.length()-2);
            if(token==""){
                Line st(currentUser,"Modify book FAIL-no Additional parameter");
                diary.write(st);error("0-0");
            }
            if(!isBookName(token))error("wrong name");
            n=true;
            mod.changeBookName(token);
        }
        if(cmd==4) {
            token=token.substr(1,token.length()-2);
            if(a){
                Line st(currentUser,"Modify book FAIL-Additional parameter repeated");
                diary.write(st);error("author repeat");
            }
            if(token==""){
                Line st(currentUser,"Modify book FAIL-no Additional parameter");
                diary.write(st);error("0-0");
            }
            if(!isBookName(token))error("wrong author");
            a=true;
            mod.changeAuthor(token);
        }
        if(cmd==5){
            if(k){
                Line st(currentUser,"Modify book FAIL-Additional parameter repeated");
                diary.write(st);error("no more keyword");
            }
            token=token.substr(1,token.length()-2);
            if(token==""){
                Line st(currentUser,"Modify book FAIL-no Additional parameter");
                diary.write(st);
                error("0-0");
            }
            if(!isBookName(token))error("wrong keyword");
            k=true;
            mod.changeKeyword(token);
        }
        if(cmd==1){
            if(p){
                Line st(currentUser,"Modify book FAIL-Additional parameter repeated");
                diary.write(st);error("no price anymore.");
            }
            if(token==""){
                Line st(currentUser,"Modify book FAIL-no Additional parameter");
                diary.write(st);
                error("0-0");
            }
            if(!isPrice(token))error("wrong price");
            p=true;
            mod.changePrice(token);
        }
    }
    Line dx(currentUser,"modified the book: "+string(currentBook.getIsbn())+'('+string(currentBook.getBookName())+')');
    currentBook.setModification(mod,currentOffset);
    currentBook=mod;
    diary.write(dx);
}

void user::buy() const {
    if(pr=='0')error("beyond your priority");
    string key;
    cout<<"input ISBN";cin>>key;
    if(!isISBN(key))error("invalid isbn");
    book book1;
    if(int ofs=bookList_ISBN.findOne(key,book1)){
        if(finance.getIndexMax()>5+sizeof(trade)*2147483647)error("full");
        long long int quantity;
        cout<<"how much?";cin>>quantity;
        if(quantity>2147483647)error("invalid amount");
        book1.addAmount(-quantity);
        library.update(book1,ofs);
        double price=book1.getPrice()*quantity;
        cout<<fixed<<setprecision(2)<<book1.getPrice()*quantity<<'\n';
        //todo:log data.
        trade lg(price,0);
        finance.write(lg);
        string money= to_string(price);
        string y="has bought "+string(book1.getBookName())+ " * "+to_string(quantity)+" || +  "+money;
        string z="has bought "+ to_string(quantity)+" * "+book1.getBookName()+" in "+money;
        log x(currentUser.pr,currentUser.id,y);
        report.write(x);
        Line temp(currentUser,z);
        diary.write(temp);
    }else{
        Line x(currentUser,"buy FAIL-Book not found.");
        diary.write(x);
        error("book not found!");
    }
}

void user::import() const {
    if(pr=='0'||pr=='1'){
        Line st(currentUser,"import book FAIL-Priority limited.");
        diary.write(st);
        error("you don't have the priority. Sign in first if you want to import.");
    }
    if(!selected){
        Line st(currentUser,"import book FAIL-No book selected.");
        diary.write(st);
        error("select a book first.");
    }
    long long int quantity;
    cout<<"import how much?";cin>>quantity;
    if(quantity>2147483647)error("invalid amount");
    currentBook.addAmount(quantity);
    cout<<"total cost is?";
    double price;cin>>price;
    //todo:log data.
    trade lg(0,price);
    finance.write(lg);
    library.update(currentBook,currentOffset);
    string money= to_string(price);
    string lz="has imported "+string(currentBook.getBookName())+ " * "+to_string(quantity)+" || - "+money;
    log x(currentUser.pr,currentUser.id,lz);
    report.write(x);
    string l="has imported "+ to_string(quantity)+" * "+currentBook.getBookName()+" in "+money;
    Line temp(currentUser,l);
    diary.write(temp);
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

void user::showFinance(int times) {
    if(pr!='7')error("You're not allowed to see this.");
    fstream file;
    if(times==0){
        cout<<'\n';return;
    }
    file.open("trade_data");
    trade sum(0,0);
    int d=finance.getIndexMax();
    if(times==-1) {
        while (d > 5) {
            trade tmp;
            finance.read(tmp, d);
            sum += tmp;
            d -= sizeof(trade);
        }
        cout<<fixed<<setprecision(2)<<sum<<'\n';
        return;
    }
    int t=times;
    if(t>(d-5)/sizeof(trade))error("trades limited.");
    while(t--&&d>5){
        trade tmp;
        finance.read(tmp,d);
        sum+=tmp;
        d-=sizeof(trade);
    }
    cout<<sum<<'\n';
}

void user::Log() const {
    if(pr!='7')error("You're not allowed to see this.");
    fstream file;
    file.open("log_data");
    int d=diary.getIndexMax();
    while(d>5){
        Line tmp;
        diary.read(tmp,d);
        cout<<tmp<<endl;
        d-=sizeof(Line);
    }
}

void user::Report() {
    int cmd;
    cout<<"Report what?\n1.myself 2.employee 3.finance"<<endl;
    cin>>cmd;
    if(cmd==1){reportMyself();return;}
    if(cmd==2){reportEmployee();return;}
    if(cmd==3){reportFinance();return;}
}

void user::reportMyself() {
    if(pr!='7'&&pr!='3')error("You're not allowed to see this.");
    fstream file;
    file.open("log_data");
    bool q=false;
    int d=diary.getIndexMax();
    cout<<currentUser<<"'s recent report:"<<endl;
    while(d>5){
        Line tmp;
        diary.read(tmp,d);
        if(tmp.getUsr()==currentUser){
            q=true;cout<<tmp<<endl;
        }
        d-=sizeof(Line);
    }
    if(!q)cout<<"reports not found!"<<endl;
}

void user::reportFinance() {
    if(pr!='7'&&pr!='3')error("You're not allowed to see this.");
    fstream file;
    file.open("transfer_data");
   cout<<"-----------9cm's Finance Report-----------"<<endl;
    int d=report.getIndexMax();
    while(d>5){
        log tmp;
        report.read(tmp,d);
        cout<<tmp<<endl;
        d-=sizeof(log);
    }
    cout<<"total:";
    d=finance.getIndexMax();
    trade sum(0,0);
        while (d > 5){
            trade tmp;
            finance.read(tmp, d);
            sum += tmp;
            d -= sizeof(trade);
        }
        cout<<fixed<<setprecision(2)<<sum<<'\n';
}

void user::reportEmployee() {
    fstream file;
    file.open("log_data");
    bool q=false;
    int d=diary.getIndexMax();
    while(d>5){
        Line tmp;
        diary.read(tmp,d);
        if(tmp.getUsr().pr=='3'){
            q=true;cout<<tmp<<endl;
        }
        d-=sizeof(Line);
    }
    if(!q)cout<<"reports not found!"<<endl;
}


user::~user()=default;

Line::Line(const user &usr, string script_) : usr(usr) {
    strcpy(script,script_.c_str());
}

user &Line::getUsr(){
    return usr;
}

const char *Line::getScript() const {
    return script;
}

ostream &operator<<(ostream &os, const Line &line) {
    os << "user: " << line.usr << line.script;
    return os;
}

Line::Line() {}
