#include <iostream>
#include <cstring>
//#include<time.h>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#define fastcall __attribute__((optimize("-O3")))
#pragma GCC optimize(2)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")
using namespace std;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

const int maxElements=1830;


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
    void Delete(int index) {
        deleted.push_back(index);
    }
};
template<class T>
class Node{
private:
    char key[65]="";
    T value;
public:
    Node()=default;
    Node(const string &key_, const T &value_)
            :  value(value_) {
        strcpy(key, key_.c_str());
    }
    string Key() const { return key; }
    const T &Value() const { return value; }
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
        t.numElements=0;
        t.nextBlock=-1;
    }
    ~BlockList(){}
    void initialize(string t){
        list.initialise(t);

    }
    void swap(blockNode& lhs,int r,int j){
        Node<T>tmp;
        tmp=lhs.elements[r];
        lhs.elements[r]=lhs.elements[j];
        lhs.elements[j]=tmp;
    }
    void insert(const string& key,T& value){
        Node<T> node(key,value);
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
    void Delete(const string& key,const T& value){
        Node<T> node(key,value);
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
int main()
{
    clock_t start,end;
    start=clock();
    std::ios::sync_with_stdio(0);
    BlockList<int> chunkList;
    chunkList.initialize("myL");
    int n;cin>>n;
    while(n--){
        string command;
        cin>>command;
        if(command=="insert"){
            string key;
            int value;
            cin>>key>>value;
            chunkList.insert(key,value);
        }
        if(command=="delete"){
            string key;
            int value;
            cin>>key>>value;
            chunkList.Delete(key,value);
        }
        if(command=="find"){
            string key;
            cin>>key;
            chunkList.find(key);
        }
    }
    end=clock();
    //cout<<(float)((end-start)*1000/CLOCKS_PER_SEC);
}