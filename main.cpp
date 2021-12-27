#include <iostream>
#include <cstring>
//#include<time.h>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
#include "memory.hpp"
#include "MemoryTemplate.hpp"
#include "token_scanner.hpp"
#include "formula.hpp"
#include "error.hpp"
#include "book.hpp"
#include "user.hpp"
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


    while(true){
        try {
            string exp;
            getline(cin, exp);
            if (exp == "exit" || exp == "quit")break;
            TokenScanner token_scanner(exp);
            currentUser.allocate(token_scanner);
        }catch(errorException e){
            cerr<<"Invalid"<<e.getMessage()<<endl;
        }
    }
    end=clock();
    //cout<<(float)((end-start)*1000/CLOCKS_PER_SEC);
}


