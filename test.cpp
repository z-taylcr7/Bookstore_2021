//
// Created by Cristiano on 2021/12/26.
//

#include <iostream>
#include <cstring>
//#include<time.h>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include "error.hpp"
using namespace std;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T>
class Memo{
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int indexmax=5;
    std::vector<int>deleted;
public:
    Memo()=default;
    ~Memo(){
        file.open(file_name);
        file.write(reinterpret_cast<char*>(&indexmax),sizeof(int));
        file.close();
    }
    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name);//todo:delete out
        if(!file){
            file.open(file_name,std::ios::out);indexmax=5;
            file.write(reinterpret_cast<char*>(&indexmax),sizeof(int));
        }else{
            file.seekp(0,std::ios::beg);
            file.read(reinterpret_cast<char*>(&indexmax),sizeof(int));//indexmax赋值回到上一次的最后位置
        }
        file.close();
    }
    int write(T &t) {//write down ,return index
        file.open(file_name);
        if(!deleted.empty()){
            int dex=deleted.back();
            file.seekp(dex,std::ios::beg);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
            deleted.pop_back();
            return dex;
        }
        indexmax+=sizeofT;
        file.seekp(indexmax,std::ios::beg);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return indexmax;
    }
    void update(T &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    void read(T &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    int find(const T &t){
        file.open(file_name);
        int d=indexmax;
        while(d>5){
            T tmp;
            read(tmp,d);
            if(tmp==t)return d;
            d-=sizeofT;
        }
        return 0;
    }
    void Delete(int index) {
        deleted.push_back(index);
    }
};

//BOOKs


const int maxElements=1830;
string data_name;
string map_name;

template<class T>
class Node{
private:
    char key[65]="";
    int offset;
public:
    Node()=default;
    Node(const string &key_, const int &offset_)
            :  offset(offset_) {
        strcpy(key, key_.c_str());
    }
    string Key() const { return key; }
    void Value( T &t) const {
        fstream file;
        file.open(data_name);
        file.seekp(offset);
        file.read(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
        return ;

    }
    const int&Offset ()const{return offset;};
    bool operator<(const Node &rhs) const {
        if (!strcmp(key, rhs.key)) {
            if(offset==0&&rhs.offset!=0)return true;
            if(rhs.offset==0)return false;
            T vl;T vr;
            fstream file;
            file.open(data_name);
            file.seekp(offset);
            file.read(reinterpret_cast<char *>(&vl), sizeof(T));
            file.seekp(rhs.offset,ios::beg);
            file.read(reinterpret_cast<char *>(&vr), sizeof(T));
            file.close();
            return vl < vr;
        }
        return strcmp(key, rhs.key) < 0;
    }
    bool operator==(const Node &rhs) const {
        if (strcmp(key, rhs.key)==0) {
            T vl;T vr;
            fstream file;
            file.open(data_name);
            file.seekp(offset);
            file.read(reinterpret_cast<char *>(&vl), sizeof(T));
            file.seekp(rhs.offset,ios::beg);
            file.read(reinterpret_cast<char *>(&vr), sizeof(T));
            file.close();
            return vl == vr;
        }
        return false;
    }
    bool operator!=(const Node &rhs) const {
        return !(*this == rhs);
    }
};
template <class T>
class BlockList{
private:
    struct blockNode{
        Node<T> elements[maxElements+1];//key-value's pos in memory
        int numElements=0;
        int nextBlock=-1;
    };
    int headIndex=5;
    Memo<blockNode> list;
    Memo<T>data;

public:
    BlockList() {
        blockNode t;
        t.numElements=0;
        t.nextBlock=-1;
    }
    ~BlockList(){}
    void initialize(const string& m_name,const string& d_name){
        map_name=m_name;
        data_name=d_name;
        list.initialise(map_name);
        data.initialise(data_name);
    }
    void swap(blockNode& lhs,int r,int j){
        Node<T>tmp;
        tmp=lhs.elements[r];
        lhs.elements[r]=lhs.elements[j];
        lhs.elements[j]=tmp;
    }
    void insert(const string& key,T& value){
        int num=data.write(value);
        Node<T> node(key,num);
        blockNode it;int pos=headIndex,tmp=headIndex;
        list.read(it,headIndex);
        for(it;pos!=-1;tmp=pos,pos=it.nextBlock,list.read(it,pos)){
            if(it.elements[0]<node)continue;
            else{break;}
        }
        pos=tmp;
        list.read(it,pos);
        int p=std::lower_bound(it.elements,it.elements+it.numElements,node)-it.elements;
        int i=it.numElements;
        while(i>p){
            it.elements[i]=it.elements[i-1];i--;
        }
        it.elements[p]=node;
        it.numElements++;//find the block to be set;
        //if(key<it.elements[0].first)swap(it.elements[0],it.elements[it.numElements-1]);//ensure that elements[0]is the smallest
        //todo:裂块处理
        if(it.numElements>maxElements){
            apart(it);
        }
        list.update(it,pos);
    }

    void apart(blockNode& it){
        blockNode crack;
        for(int i=it.numElements/2, j=0;i<it.numElements;j++,i++){
            crack.elements[j]=it.elements[i];
        }
        crack.numElements=it.numElements-it.numElements/2;
        it.numElements=it.numElements/2;
        crack.nextBlock=it.nextBlock;
        it.nextBlock=list.write(crack);
    }
    void Delete(const string& key, T &value){
        int ofs=data.write(value);
        Node<T> node(key,ofs);
        blockNode it;int pos=headIndex,tmp=headIndex;
        list.read(it,headIndex);
        for(it;pos!=-1;tmp=pos,pos=it.nextBlock,list.read(it,pos)){
            if(it.elements[0]<node||it.elements[0]==node)continue;
            else{break;}
        }
        pos=tmp;
        list.read(it,pos);
        while(true) {
            int i=std::lower_bound(it.elements,it.elements+it.numElements,node)-it.elements;
            if (it.elements[i]==node) {
                it.numElements--;
                while(i<it.numElements){
                    swap(it,i,i+1);
                    i++;
                }
                if (it.numElements < maxElements / 2&&it.nextBlock!=-1)merge(it);
                list.update(it,pos);
            }
            if(it.nextBlock==-1)break;
            pos=it.nextBlock;
            list.read(it,pos);
            if(node<it.elements[0])break;
        }
        data.Delete(ofs);
    }
    void merge(blockNode &lhs){
        if(lhs.nextBlock==-1)return;
        int rpos=lhs.nextBlock;
        blockNode rhs;
        if(rpos==-1)return;
        list.read(rhs,rpos);
        if(lhs.numElements+rhs.numElements>maxElements)return;
        for (int i = lhs.numElements, j = 0; j<rhs.numElements; j++, i++)lhs.elements[i] = rhs.elements[j];
        lhs.numElements+=rhs.numElements;lhs.nextBlock = rhs.nextBlock;list.Delete(rpos);
    }
    void find(const string& key){
        T queue[10000];int cnt=0;
        Node<T> node(key,0);
        bool found=false;
        blockNode it;int pos=headIndex,tmp=headIndex;
        list.read(it,headIndex);
        for(it;pos!=-1;tmp=pos,pos=it.nextBlock,list.read(it,pos)){
            if(strcmp(it.elements[0].Key().c_str(),key.c_str())<0)continue;
            else{break;}
        }
        pos=tmp;
        list.read(it,pos);
        while(true) {
            int i=std::lower_bound(it.elements,it.elements+it.numElements,node)-it.elements;
            for (; i < it.numElements; i++) {
                if (strcmp(key.c_str(),it.elements[i].Key().c_str())==0) {
                    it.elements[i].Value(queue[cnt++]);found=true;
                }
            }
            if(it.nextBlock==-1)break;
            pos=it.nextBlock;
            list.read(it,pos);
            if(strcmp(it.elements[0].Key().c_str(),key.c_str())>0)break;
        }
        if(!found)cout<<"null";
        else{
            for(int i=0;i<cnt;i++){
                cout<<queue[i]<<' ';
            }
        }
        cout<<'\n';
    }
};
//
// Created by Cristiano on 2021/12/21.
//
//visitor account:user account(0);
//default account:user account;

#include <ostream>
#include<cstring>
#include "token_scanner.hpp"
#include "formula.hpp"
#include "book.hpp"
#include "error.hpp"
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
string rootID="root";

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

user::~user()=default;

int main()
{
    BlockList<user> chunkList;
    chunkList.initialize("_map","_data");
    int n;cin>>n;
    while(n--){
        string command;
        cin>>command;
        if(command=="insert"){
            string key;
            user value;
            cin>>key>>value;
            chunkList.insert(key,value);
        }
        if(command=="delete"){
            string key;
            user value;
            cin>>key>>value;
            chunkList.Delete(key,value);
        }
        if(command=="find"){
            string key;
            cin>>key;
            chunkList.find(key);
        }
    }

}