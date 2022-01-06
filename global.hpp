//
// Created by Cristiano on 2022/1/6.
//

#ifndef MAIN_CPP_GLOBAL_HPP
#define MAIN_CPP_GLOBAL_HPP

#include "log.hpp"
#include "user.hpp"
#include "book.hpp"
#include "blocklist.hpp"
#include "memo.hpp"

extern Memo<trade> finance;
extern Memo<log> report;
extern Memo<Line> diary;
//********USER-Memory***********//
extern BlockList<user> userList;
extern vector<pair<int,int>>log_stack;
extern user currentUser;
//***************Book-Memory****************//
extern Memo<book> library;
extern BlockList<book> bookList_ISBN;
extern BlockList<book> bookList_keyword;
extern BlockList<book> bookList_author;
extern BlockList<book> bookList_book_name;
extern book currentBook;
extern int currentOffset;
extern bool selected;

#endif //MAIN_CPP_GLOBAL_HPP
