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
    strcpy(id,_id.c_str());
    strcpy(password,_password.c_str());
    strcpy(name,_name.c_str());
}
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
    if(cmd=="log"){Log(s);return;}
    if(cmd=="report"){Report(s);return;}
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
    Line x(currentUser,"log in.");
    diary.write(x);
}
void user::logout(TokenScanner& s) const {
    if(s.hasMoreToken())error("token too much!");
    if(log_stack.empty())error("no account!");
    if(pr=='0')error("beyond your priority");
    log_stack.pop_back();
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
void user::register_account(TokenScanner& s){
    string _id=s.nextToken();
    string _pw=s.nextToken();
    string _name=s.nextToken();
    if(s.hasMoreToken())error("token too much!");
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
            Line x(currentUser,"revise password SUCCESS");
            diary.write(x);
        }else {
            Line x(currentUser,"revise password FAIL-Account not found.");
            diary.write(x);
            error("account not found:");//todo:cannot find user error
        }
    }else{
        user user1;
        if(int of=userList.findOne(s.nextToken(),user1)){
            if(strcmp(s.nextToken().c_str(),user1.password)!=0){
                Line x(currentUser,"revise password FAIL-Wrong password.");
                diary.write(x);
                error("wrong password!"); //todo:password error
            }
            string pw=s.nextToken();
            if(!isPassword(pw))error("invalid password");
            strcpy(user1.password,pw.c_str());
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
void user::useradd(TokenScanner& s) const {
    if(pr=='0'||pr=='1')error("you can't even do this!");
    string _id=s.nextToken();
    string _pw=s.nextToken();
    string _pr=s.nextToken();
    string _name=s.nextToken();
    if(s.hasMoreToken())error("token too much!");
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
    Line x(currentUser,"Create account SUCCESS:"+user1.pr+')'+_id);
    diary.write(x);
    int u=userList.data.write(user1);
    userList.insert(_id,u);
}
void user::Delete(TokenScanner& s) const {
    if(pr!='7'){
        Line x(currentUser,"Delete account FAIL-Priority limited.");
        diary.write(x);
        error("only superUser can delete an account.");
    }
    string _id=s.nextToken();
    if(!isID(_id))error("invalid ID");
    if(s.hasMoreToken())error("token too much!");
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
        int l=token.length();int j=0;
        if(l>60)error("wrong keyword");
        while(j<l){if(t[j]=='|')error("only one keyword is required.");j++;}
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

void user::modify(TokenScanner &s) const{
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
    Line dx(currentUser,"modified the book: "+string(currentBook.getIsbn())+'('+string(currentBook.getBookName())+')');
    currentBook.setModification(mod,currentOffset);
    currentBook=mod;
    diary.write(dx);
}

void user::buy(TokenScanner &s) const {
    if(pr=='0')error("beyond your priority");
    string key=s.nextToken();
    if(!isISBN(key))error("invalid isbn");
    book book1;
    if(int ofs=bookList_ISBN.findOne(key,book1)){
        if(finance.getIndexMax()>5+sizeof(trade)*2147483647)error("full");
        long long int quantity=stoll(s.nextToken());
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
        Line zxc(currentUser,z);
        diary.write(zxc);
    }else{
        Line x(currentUser,"buy FAIL-Book not found.");
        diary.write(x);
        error("book not found!");
    }
}

void user::import(TokenScanner &s) const {
    if(pr=='0'||pr=='1')error("you don't have the priority. Sign in first if you want to import.");
    if(!selected)error("select a book first.");
    int quantity= toNumber(s.nextToken());
    if(quantity>2147483647)error("invalid amount");
    currentBook.addAmount(quantity);
    double price= toFloat(s.nextToken());
    //todo:log data.
    trade lg(0,price);
    finance.write(lg);
    library.update(currentBook,currentOffset);
    string money= to_string(price);
    string lz="has imported "+string(currentBook.getBookName())+ " * "+to_string(quantity)+" || - "+money;
    log x(currentUser.pr,currentUser.id,lz);
    report.write(x);
    string l="has imported "+ to_string(quantity)+" * "+currentBook.getBookName()+" in "+money;
    Line zxc(currentUser,l);
    diary.write(zxc);
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

void user::Log(TokenScanner &s) const {
    if(s.hasMoreToken())error("zxc is too fat!");
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

void user::Report(TokenScanner& s) {
    string cmd=s.nextToken();
    if(cmd=="myself"){reportMyself();return;}
    if(cmd=="employee"){reportEmployee();return;}
    if(cmd=="finance"){reportFinance();return;}
    error("token Error!");
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
    showFinance("all");
    cout<<endl;
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
