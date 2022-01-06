//
// Created by Cristiano on 2022/1/6.
//
#include "global.hpp"
#include "log.hpp"
#include "user.hpp"
#include "book.hpp"
#include "blocklist.hpp"
#include "memo.hpp"
Memo<trade> finance;
Memo<log> report;
Memo<Line> diary;
//********USER-Memory***********//
BlockList<user> userList;
vector<pair<int,int>>log_stack;
user currentUser;
//***************Book-Memory****************//
BlockList<book> bookList_ISBN;
Memo<book> library;
BlockList<book> bookList_keyword;
BlockList<book> bookList_author;
BlockList<book> bookList_book_name;
book currentBook;
int currentOffset;
bool selected=false;
string data_file;