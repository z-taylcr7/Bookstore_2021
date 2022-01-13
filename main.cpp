#include <iostream>
#include <cstring>
//#include<time.h>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include "memo.hpp"
#include "blocklist.hpp"
#include "token_scanner.hpp"
#include "book.hpp"
#include "user.hpp"
#include "global.hpp"

using namespace std;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;


int main()
{
    finance.initialise("trade_data");
    library.initialise("book_data");
    diary.initialise("log_data");
    report.initialise("transfer_data");
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
    int exp;
    cout<<"Welcome to z_taylcr7's Bookstore!"<<endl;

    while(true){
        try {
            cout<<"0---->exit\n1---->su\n2---->logout\n3---->register\n4---->passwd\n5----->useradd\n6----->delete\n7---->show\n8---->buy\n9---->import\n10---->select\n11---->modify\n"
                <<"12---->log\n13---->report"<<endl;
            cin>>exp;
            if (exp == 0)break;
            currentUser.allocate(exp);
        }catch(errorException e){
            cout<<currentUser<<':'<<"Invalid:"<<e.getMessage()<<endl;
        }
    }
}


