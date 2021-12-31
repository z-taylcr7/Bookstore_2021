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

using namespace std;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
extern user currentUser;

int main()
{
    std::ios::sync_with_stdio(0);
    finance.initialise("trade_data");
    library.initialise("book_data");
    //initialise:
    user superUser("root","sjtu","yyu","7");fstream file;
    if(userList.initialize("user_id_map","user_data")) {
        int s=userList.data.write(superUser);
        userList.insert("root", s);
    }
    bookList_ISBN.initialize("book_ISBN_map","book_data");
    bookList_author.initialize("book_author_map","book_data");
    bookList_book_name.initialize("book_book_name_map","book_data");
    bookList_keyword.initialize("book_keyword_map","book_data");
    user user1;
    currentUser=user1;
    string exp;
    while(getline(cin, exp)){
        try {
            if(exp=="")continue;
            if (exp == "exit" || exp == "quit")break;
            TokenScanner token_scanner(exp);
            currentUser.allocate(token_scanner);
        }catch(errorException e){
            cout<<"Invalid\n";
        }
    }
}


