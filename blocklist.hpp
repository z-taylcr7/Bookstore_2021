//
// Created by Cristiano on 2021/12/18.
//

#ifndef BOOKSTORE_CBP_BLOCKLIST_HPP
#define BOOKSTORE_CBP_BLOCKLIST_HPP
#include <iostream>
#include <cstring>
//#include<time.h>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include "memo.hpp"
using namespace std;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;


//BOOKs
const int maxElements=500;
extern string data_file;
template<class T>
class Node{
private:
    char key[65];
    int offset=0;
public:
    Node(){
        memset(&key,0,sizeof(key));
        offset=0;
    };
    Node(const string &key_, const int &offset_) :  offset(offset_) {
        memset(&key,0,sizeof(key));
        strcpy(key, key_.c_str());
    };
    string Key() const { return key; };
    void Value( T &t) const {
        if(offset==0)return;
        fstream file;
        file.open(data_file);
        file.seekp(offset);
        file.read(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
    } ;
    int Offset ()const{return offset;};
    bool operator<(const Node &rhs) const{
        if (!strcmp(key, rhs.key)) {
            if(offset==0&&rhs.offset!=0)return true;
            if(rhs.offset==0)return false;
            T vl;T vr;
            fstream file;
            file.open(data_file);
            file.seekp(offset);
            file.read(reinterpret_cast<char *>(&vl), sizeof(T));
            file.seekp(rhs.offset,ios::beg);
            file.read(reinterpret_cast<char *>(&vr), sizeof(T));
            file.close();
            return vl < vr;
        }
        return strcmp(key, rhs.key) < 0;
    } ;
    bool operator==(const Node &rhs) const {
        if (strcmp(key, rhs.key)==0) {
            T vl;T vr;
            fstream file;
            file.open(data_file);
            file.seekp(offset);
            file.read(reinterpret_cast<char *>(&vl), sizeof(T));
            file.seekp(rhs.offset,ios::beg);
            file.read(reinterpret_cast<char *>(&vr), sizeof(T));
            file.close();
            return vl == vr;
        }
        return false;
    } ;
    bool operator!=(const Node &rhs) const {
        return !(*this == rhs);
    };
};

//insert(k,v),delete(k,v),find(k)
template <class T>
class BlockList{
private:
    string data_name;
    string map_name;
    class blockNode{
    public:
        Node<T> elements[maxElements+1];//key-value's pos in memory
        int numElements=0;
        int nextBlock=-1;
        blockNode(){
            memset(&elements,0,sizeof (elements));
        }
    };
    int headIndex=5;
    Memo<blockNode> list;
public:
    Memo<T>data;
    BlockList() {
        blockNode t;
        t.numElements=0;
        t.nextBlock=-1;
    };
    ~BlockList()=default;
    bool initialize(const string& m_name,const string& d_name){
        map_name=m_name;
        data_name=d_name;
        data.initialise(data_name);
        bool res=list.initialise(map_name);
        return res;
    };
    void swap(blockNode& lhs,int r,int j){
        data_file=data_name;
        Node<T>tmp;
        tmp=lhs.elements[r];
        lhs.elements[r]=lhs.elements[j];
        lhs.elements[j]=tmp;
    };
    int insert(const string& key,int num){
        data_file=data_name;
        Node<T> node(key,num);
        blockNode it;
        int pos=headIndex,tmp=headIndex;
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
        //todo:裂块处理
        if(it.numElements>maxElements){
            apart(it);
        }
        list.update(it,pos);
        return num;
    };
    void apart(blockNode& it){
        data_file=data_name;
        blockNode crack;
        for(int i=it.numElements/2, j=0;i<it.numElements;j++,i++){
            crack.elements[j]=it.elements[i];
        }
        crack.numElements=it.numElements-it.numElements/2;
        it.numElements=it.numElements/2;
        crack.nextBlock=it.nextBlock;
        it.nextBlock=list.write(crack);
    };
    bool Delete(const string& key,int ofs){
        bool deleted=false;
        data_file=data_name;
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
                deleted=true;
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
        return deleted;

    }
    void merge(blockNode &lhs){
        data_file=data_name;
        if(lhs.nextBlock==-1)return;
        int rpos=lhs.nextBlock;
        blockNode rhs;
        if(rpos==-1)return;
        list.read(rhs,rpos);
        if(lhs.numElements+rhs.numElements>maxElements)return;
        for (int i = lhs.numElements, j = 0; j<rhs.numElements; j++, i++)lhs.elements[i] = rhs.elements[j];
        lhs.numElements+=rhs.numElements;lhs.nextBlock = rhs.nextBlock;list.Delete(rpos);
    };
    void find(const string& key){
        data_file=data_name;
        T queue[1000];int cnt=0;
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
        if(!found)cout<<"\n";
        else{
            for(int i=0;i<cnt;i++){
                cout<<queue[i]<<endl;
            }
        }
    };
    void scroll(){
        data_file=data_name;
        blockNode it;int pos=headIndex;
        list.read(it,headIndex);
        for(it;pos!=-1;pos=it.nextBlock,list.read(it,pos)){
            for(int i=0;i<it.numElements;i++){
                T t;it.elements[i].Value(t);
                cout<<t<<'\n';
            }
        }
    };
    T read(int ofs){
        T t;if(ofs==0)return t;
        data.read(t,ofs);return t;
    };
    int findOne(const string& key,T& ans){
        data_file = data_name;
        Node<T> node(key, 0);
        blockNode it;
        int pos = headIndex, tmp = headIndex;
        list.read(it, headIndex);
        for (it; pos != -1; tmp = pos, pos = it.nextBlock, list.read(it, pos)) {
            if (strcmp(it.elements[0].Key().c_str(), key.c_str()) < 0)continue;
            else { break; }
        }
        pos = tmp;
        list.read(it, pos);
        while (true) {
            int i = std::lower_bound(it.elements, it.elements + it.numElements, node) - it.elements;
            for (; i < it.numElements; i++) {
                if (strcmp(key.c_str(), it.elements[i].Key().c_str()) == 0) {
                    it.elements[i].Value(ans);
                    return it.elements[i].Offset();
                }
            }
            if (it.nextBlock == -1)break;
            pos = it.nextBlock;
            list.read(it, pos);
            if (strcmp(it.elements[0].Key().c_str(), key.c_str()) > 0)break;
        }
        return 0;
    }
};
#endif //BOOKSTORE_CBP_BLOCKLIST_HPP
