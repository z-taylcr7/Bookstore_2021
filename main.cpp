#include <iostream>
#include <cstring>
#include<time.h>
using namespace std;

#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>

#include"memo.hpp"
#include"structures.hpp"
int main()
{
    //clock_t start,end;
    //start=clock();

    BlockList<int> chunkList;
    chunkList.initialize("myF","myL");
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
    //end=clock();
    //cout<<(float)((end-start)*1000/CLOCKS_PER_SEC);
}