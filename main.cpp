#include <iostream>
#include <cstring>
//#include<time.h>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include "memory.hpp"
#include "MemoryTemplate.hpp"
#include "token_scanner.hpp"
#include "formula.hpp"
#include "error.hpp"
#include "book.hpp"
#include "user.hpp"
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
extern user currentUser;
int main()
{

    clock_t start,end;
    start=clock();
    std::ios::sync_with_stdio(0);
    finance.initialise("trade_data");
    //initialise:
    user superUser("root","sjtu","yyu",'7');fstream file;
    if(userList.initialize("user_id_map","user_data")) {
        userList.insert("root", superUser);
    }
    bookList_ISBN.initialize("book_ISBN_map","book_data_1");
    bookList_author.initialize("book_author_map","book_data_2");
    bookList_book_name.initialize("book_book_name_map","book_data_3");
    bookList_keyword.initialize("book_keyword_map","book_data_4");
    string exp;
    while(getline(cin, exp)){
        try {
            if (exp == "exit" || exp == "quit")break;
            TokenScanner token_scanner(exp);
            currentUser.allocate(token_scanner);
        }catch(errorException e){
            cout<<"Invalid\n";
        }
    }
    end=clock();
    //
    // cout<<(float)((end-start)*1000/CLOCKS_PER_SEC);
}


