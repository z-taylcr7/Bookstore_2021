//
// Created by Cristiano on 2021/12/13.
//

#ifndef BOOKSTORM_STRUCTURES_HPP
#define BOOKSTORM_STRUCTURES_HPP
#include <iostream>
#include <cstring>
#include<time.h>
using namespace std;
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include "memo.hpp"
const int maxElements=380;
template<class T>
class Node{
private:
    char key[65]="";
    int offset;T value;
public:
    Node()=default;
    Node(const string &key_, const T &value_, const int &offset_)
            : offset(offset_), value(value_) {
        strcpy(key, key_.c_str());
    }
    string Key() const { return key; }
    const T &Value() const { return value; }
    const int& Offset()const{return offset;}
    bool operator<(const Node &rhs) const {
        if (!strcmp(key, rhs.key)) return value < rhs.value;
        return strcmp(key, rhs.key) < 0;
    }
    bool operator==(const Node &rhs) const {
        return !strcmp(key, rhs.key) && value == rhs.value;
    }
    bool operator!=(const Node &rhs) const {
        return !(*this == rhs);
    }
};
template <class T>
class BlockList{
private:
    Memo<T> memory;
    struct blockNode{
        Node<T> elements[maxElements+1];//key-value's pos in memory
        int numElements=0;
        int nextBlock=-1;
    };
    int headIndex=5;
    Memo<blockNode> list;
public:
    BlockList() {
        blockNode t;
        t.nextBlock=-1;
    }
    ~BlockList(){}
    void initialize(string s,string t){
        memory.initialise(s);
        list.initialise(t);

    }
    void swap(blockNode& lhs,int r,int j){
        Node<T>tmp;
        tmp=lhs.elements[r];
        lhs.elements[r]=lhs.elements[j];
        lhs.elements[j]=tmp;
    }
    void insert(const string& key,T& value){
        int index=memory.write(value);
        Node<T> node(key,value,index);
        blockNode it;int pos=headIndex,tmp=headIndex;
        list.read(it,headIndex);
        for(it;pos!=-1;tmp=pos,pos=it.nextBlock,list.read(it,pos)){
            if(it.elements[0]<node)continue;
            else{break;}
        }
        pos=tmp;
        list.read(it,pos);
        int p=it.numElements;
        it.elements[it.numElements++]=node;//find the block to be set;
        while(p>0&&it.elements[p]<it.elements[p-1]){
            swap(it,p,p-1);p--;
        }
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
    void Delete(const string& key,const T& value){
        Node<T> node(key,value,0);
        blockNode it;int pos=headIndex,tmp=headIndex;
        list.read(it,headIndex);
        for(it;pos!=-1;tmp=pos,pos=it.nextBlock,list.read(it,pos)){
            if(it.elements[0]<node)continue;
            else{break;}
        }
        pos=tmp;
        list.read(it,pos);
        while(true) {
            int i=std::lower_bound(it.elements,it.elements+it.numElements,node)-it.elements;
            if (it.elements[i]==node) {
                memory.Delete(it.elements[i].Offset());
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
        T queue[50000];int cnt=0;
        Node<T> node(key,0,0);
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
                    queue[cnt++]=it.elements[i].Value();found=true;
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

#endif //BOOKSTORM_STRUCTURES_HPP
