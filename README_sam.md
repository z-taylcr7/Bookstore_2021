### **开发文档**

#### 项目名称：bookstore

#### 文档作者：章轩畅

##### 程序功能概述：

* 账户的登录，注册，修改信息
* 书本的条件查找，出售，进货和信息修改
* 对于错误的信息抛出异常（对应的要求readme写的很清楚了，错误名称自己取吧，实现也不难，下面就不写了）

##### 主体逻辑说明

* 通过固定每个信息的长度储存各种信息
* 登录用户信息存内存，其他信息存外存
* user_data文档存账户信息，user_id_map索引文档（ID->A中ID对应信息位置（一个指针））
* book_data文档存书本信息，book_ISBN_map文档作为索引文件（ISBN->书本序号（1-base，乘信息长度后可直接访问书本信息）），book_name/au.hppor/keywrod_map索引文件（Book-Name/Au.hppor/Keyword->书本序号）
* trade_data文档存交易信息
* log 文档存所有系统操作信息（用于生成日志）

##### 代码文件结构(仿照basic解释器)

* main.cpp
* error.hpp
* token_scanner.hpp
* token_scanner.cpp
* user.hpp
* user.cpp
* book.hpp
* book.cpp
* log.hpp
* log.cpp

##### 各个类的接口和成员说明

* error类：用来报错

* token_scanner类：处理每一行输入，返回账户/图书/日志类（类似Basic对输入的处理，但是可以不用预先编译，账户/图书/日志类直接解释编译输入指令）

* user类：

  * private可直接存输入指令，类里再解释编译

  * 成员函数对应每个账户系统指令，可自行添加辅助函数（如文件读取），下同

* book类：同上

* log类(也可以根据不同的日志格式写几个不同的类)：同上

##### 具体算法说明

###### 程序

* 按登录顺序存登录账户的各种信息（包括账户的信息，该账户交易的开始和结束（即他人登录或自己登出）时的time，自己的操作在日志里的始末位置，选中的图书的ISBN）

######         账户系统

* user_data文档信息存储

  * 按照顺序存User-ID（30），Password（30），Priority（1），User-Name(30)
  * 括号里是预设长度，不够加‘\0'/’ ‘（每一位都是二进制读写，1是sizeof（int）或sizeof（.hppar））
  
* user_id_map索引文档信息存储

  * 类似下面book_data（具体写了方法）
  * 数组元素是ID+到user_data对应信息的位置

* 登录指令

  * user_data文件里遍历找User-ID,先判断等级，等级不够再判断对应的Password和输入的一不一样

  * 之后的指令都要先判断权限等级是否匹配
  * 日志格式：ID log in

* 注销指令

  * 记登录账户trade_data和log的结束位置

  * 删除最后一个登录账户的信息

  * 记录新登录账户交易和日志的起始位置

  * 日志格式：成功：ID log out SUCCESS

    ​                   失败：ID log out FAIL-No login Account 

* 注册指令

  * 遍历user_data文件里ID，不重复就在文件末尾加上一个新账户信息

  * 日志格式：成功：ID register name SUCCESS 

    ​                   失败：ID register name FAIL-ID repitition

* 修改密码指令

  * 找ID，先判断等级是不是7，不是的话还要验证old-password，然后对的话改password
  
  * 日志格式：成功：ID revise password SUCCESS 
  
    ​                   账户不存在：ID revise password FAIL-Account not found
  
    ​                   密码错误：ID revise password FAIL-Wrong password
  
* 创建账户

  * 先判断账户等级，够的话遍历ID，不重的话文件末尾加一个账户信息

  * 日志格式：成功：Create account SUCCESS

    ​                   重名：Create account FAIL-Name repetition

    ​                   权限不够：Create account FAIL-Priority limited

* 删除账户

  * 遍历user_data文件ID，不存在则操作失败，再遍历登录的账户ID，不存在就删除，否则操作失败
  
  * 日志格式：成功：Delete account SUCCESS
  
    ​                   账户不存在：Delete account FAIL-Account notfind
  
    ​                   已登录：Delete account FAIL-Already logged in
######     图书系统

* book_data文档信息存储

  * 按照顺序储存：图书编号（每次加一），ISBN（20）,Book-Name（60）,Au.hppor（60）,Keyword（60）,Quantity（10）,Price（13）,int store(库存)
  
  * 其中pre，next，store固定8位（应该够的，不够再加）
  
* book_ISBN_map索引文档信息存储

  * 作为索引文件存ISBN和图书编号
  * 块状链表：pre，next，sign，数组（单链表如果可以也行）
  * 保证数组内元素个数在（sqrt（n），2sqrt（n））之间，n是书本数，超过了就把数组分成两段并在之后增加一个节点
  * 数组：ISBN->图书编号
  * 查找ISBN时先与数组最小元素（开头）比较确定范围，再遍历数组得到图书编号，即可访问B中图书信息，复杂度应该是sqrt（n）
  
* book_name/author/keyword_map索引文档信息储存
  * 类似book_ISBN_map
  * 数组元素：Book-Name/Author/Keyword->图书编号
  
* 检索图书指令

* 按要求遍历文件，输出结果即可

* 购买图书指令

  * 遍历查找ISBN，得到price，修改store
  
  * 在trade_data文档末记录此次交易的信息
  
  * time加一
  
  * 日志格式：成功：Sell 数量 书名 in 总价
  
    ​                   失败：Sell 数量 书名 FAIL-Book not found
  
* 选择图书指令

  * 修改选中的图书的编号（避免修改图书信息导致选中的书没了，因为图书编号永远不变）
  
  * 日志格式：登录ID select 书名
  
    ​                    若创建新图书：登录ID create new book，ISBN is 【ISBN】
  
* 修改图书信息指令

  * 按要求修改文件中对应书本的信息，并修改选中的图书
  
  * 日志格式：成功：登录ID change （书名‘s type/书名） from a to b（若未修改书名用前者）
  
    ​                   失败：登录ID change FAIL-No book selected(未选中图书)/Aadditional parameter repeated（重复附加参数）/No additional parameter（附加参数为空）/Keyword repeated(keyword包含重复信息段)
  
* 图书进货指令

  * 对选中的图书的Quantity修改，数目为Total-Cost/price
  
  * 在trade_data文档末记录此次交易的信息
  
  * time加一
  
  * 日志格式：成功：Import 数量 书名 in 总价
  
    ​                   失败：Import FAIL-No book selected

###### 日志系统

* trade_data文档文件存储：

  * 交易账户ID，位数与user_data中相同
  * 交易图书名字，单价，本数，位数与book_data中相同
  * 每次交易金额：+/-代表收入或支出，固定位数就行，具体看数据范围

* 生成员工操作记录指令

  * 按照该员工账户的交易始末信息依次输出交易信息，格式为：

    import/buy 图书名字 单价 本数 交易总金额

* 财务记录查询指令

  * 输出book_data文档中time后每次交易的交易金额

* 生成财务记录报表指令

  * 输出所有book_data文档所有交易，格式为：

    import/buy 图书名字 单价 本数 交易总金额 此次交易后总盈亏（从0开始，buy盈import亏）

* 生成全体员工工作情况报表指令

  * 输出所有交易，格式如下：

    交易账户ID import/buy 图书名字 单价 本数 交易总金额 

* 生成日志指令

  * 输出log文件所有内容

##### 其他补充说明

