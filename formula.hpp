//
// Created by Cristiano on 2021/12/21.
//

#ifndef MAIN_CPP_FORMULA_HPP
#define MAIN_CPP_FORMULA_HPP

bool isID(const string& id){
    int l=id.length();
    if(l>30)return false;
    for(int i=0;i<l;i++){
        if((id[i]>='0'&&id[i]<='9')||(id[i]>='a'&&id[i]<='z')||(id[i]>='A'&&id[i]<='Z')||(id[i]=='_'))continue;
        else{
            return false;
        }
    }
    return true;
};
bool isPassword(const string& password){
    return isID(password);
};
bool isUserName(const string& id){
    int l=id.length();
    if(l>30)return false;
    for(int i=0;i<l;i++){
        if(id[i]<=126&&id[i]>32)continue;
        else{
            return false;
        }
    }
    return true;
}
bool isISBN(const string&ISBN){
    int l=ISBN.length();
    if(l>20)return false;
    for(int i=0;i<l;i++){
        if(ISBN[i]<=126&&ISBN[i]>=32)continue;
        else{
            return false;
        }
    }
    return true;
};
bool isPriority(const string&pr){
    if(pr.length()!=1)return false;
    if(pr[0]=='0'||pr[0]=='1'||pr[0]=='3'||pr[0]=='7')return true;
    else return false;
};
bool isBookName(const string &book_name){
    int l=book_name.length();
    if(l>60)return false;
    for(int i=0;i<l;i++){
        if(book_name[i]<=126&&book_name[i]>32&&book_name[i]!='"')continue;
        else{
            return false;
        }
    }
    return true;
};
bool isNumber(const string&number){
    int l=number.length();
    if(l>10)return false;
    for(int i=0;i<l;i++){
        if(number[i]<='9'&&number[i]>='0')continue;
        else{
            return false;
        }
    }
    return true;
};
bool isPrice(const string&price){
    int l=price.length();
    if(l>13)return false;
    for(int i=0;i<l;i++){
        if(price[i]<='9'&&price[i]>='0'||price[i]=='.')continue;
        else{
            return false;
        }
    }
    return true;
};
long long int toNumber(const string& s){
    int l=s.length();
    if(l>10)error("longlong");
    long long int num=0;
    for(int i=0;i<l;i++){
        num=10*num+s[i]-'0';
    }
    return num;
};
double toFloat(const string& s){
    int l=s.length();int i;
    double num=0;
    if(l>13)error("too long");
    num=stod(s);
    return double(num);
}
#endif //MAIN_CPP_FORMULA_HPP
