# Bookstore_2021 开发文档

##### 		written by z_taylcr7



## 程序功能概述

本项目旨在实现一个书店管理系统。

#### 账户系统

##### 实现注册账户、登陆账户、登出账户、注销账户、创建账户、更改账户密码的操作。**不同权限的账户能执行不同的操作**。

要求实现登录栈的结构，即：

###### 可以同时登陆多个账户，所有的操作为最近一次登陆的账户的操作；登出账户即视为登出最近一次登陆的账户；退出系统为登出所有账户。

###### 权限等级说明

- **店主 `{7}`**：使用 `root` 账户，可以访问书店系统所有功能，包括日志和账户管理功能
- **员工 `{3}`**：可以访问修改图书数据相关功能等
- **顾客 `{1}`**：可以注册账户，查询 / 购买图书
- **游客 `{0}`**：可以注册账户

#### 图书系统

##### 要求实现检索图书、购买图书、选择图书、修改图书信息、图书进货的功能

#### 日志系统

##### 用于记录运营过程中的各种数据，提供各类日志查询服务

**需要实现的功能有：**

* ######  员工操作记录 只含有某一个员工的log_data

- ##### **财务记录查询** ：transfer_data


- ##### 财务记录报告生成： finance_data


- #####  全体员工工作报告生成： 只含有员工的log_data


- #####  日志生成 log_data

**需要实现的有**

- log_data

- finance_data

- transfer_data

  

## 主体逻辑说明

###### 主体结构：输入--解释输入-执行函数（-抛出异常--回到输入）--存储&更新数据--回到输入

###### 所有的数据诸如【书本数据、用户信息、财务数据】等使用写入文件的方式存储。仅登陆栈使用内存，直接以一个vector（存用户ID）的形式存于main函数里。



## 代码文件结构

##### 外存文件

- **file1：accounts_data，**存储用户数据，**也许可以考虑将游客{0}，顾客{1}，管理员{3}，老板{7}分块存储**

- **file2：books_data，**存储书本数据，**这里应按照ISBN的字典序存储**。

- **file3：log_data，**存储操作记录

- **file4：finance_data，**存储交易记录

- **file5：transfer_data，**存储收支记录（+x-y……）

（若复杂度太高，则继续开文件，将需要访问的数据直接存入文件，每次O(1)访问，）

##### 头文件

- **h1:TokenScanner，**解释输入

- **h2：MemoryRiver，**将外存数据链接到文件，也可以通过文件读数据

- **h3：Accounts，**封装所有账户操作的执行函数

- **h4：Books，**封装所有图书操作的执行函数

- **h5：Diary，**封装所有日志操作的执行函数，

- **h6：Error，**用来承包所有的错误抛出处理。

- **h7：Express，**用于链接被TokenScanner解释后的语句和执行函数



## 各个类的接口及成员说明

###### 输入经过TokenScanner解释，进入express函数，链接至Accounts、Books、Diary中的操作函数，操作完成后将记录写入Diary中对应文件。所有的读写文件操作需要通过MemoryRiver读写文件数据。所有的异常通过error.h抛出。



#### class1: TokenScanner

接入：输入的string

以链表的形式临时地存储整个string中的每一个token

- `firstToken()`：返回第一个token的地址

- `hasMoreToken()`: 返回一个bool 判断是否为最后一个token

- `nextToken()`返回下一个token的地址

#### class2:MemoryRiver

##### 文件读写模块

这个类的一个元素和一个文件的文件名绑定。这个类的函数实现指定数据和对应文件之间的读写。文件中采用顺序读写。可以参考OJ1337.

##### 块状链表

应该包括一个文件读写模块类。一个文件可以对应多个块状链表（books可以通过不同参数查找），每个块状链表能且仅能对应一个文件。

对于这个块状链表，链表中存的元素为pair<key，value>其中key为键值，value为这个键值对应的数据信息的存储地址（如果不确定类型可以写成模板类）。块块之间通过链表链接，一个块里面要维护一个数组elements。

int maxelements//数组元素数量上限, 

`struct blocknode{`

`int numelements，pair<key,value> elements[maxelements];blocknode next;`

`}`

每个块的头节点之间**必须**保证有序。可以为key的字典序。

每个块节点最多可容纳某个最大数量的元素，通常刚好足够大，以便节点填充单个其中的一小部分。列表中的位置由对节点的引用和元素数组中的位置指示。还可以为块状链表包含前一个指针。

要插入一个新元素，我们只需找到元素应该在的节点并将元素插入到`elements`数组中，增加`numElements`。如果数组已经满了，我们首先在当前节点之前或之后插入一个新节点，并将当前节点中的一半元素移动到其中。

要删除一个元素，我们只需找到它所在的节点并将其从`elements`数组中删除，递减`numElements`。如果这将节点减少到小于半满，那么我们从下一个节点移动元素以将其填满一半以上。如果这使得下一个节点少于半满，那么我们将其所有剩余元素移动到当前节点中，然后绕过并删除它。

该类需要配备至少如下函数：

- `find()//基于某一段键值的查找`

- `insert()`

- `erase()`



#### class3:Accounts

成员：ID，username，password，priority，（bool）online

链接至文件：accounts_data;

读取用户数据并执行函数

`class Accounts{`

` void su();` //要检查权限等级

`void logout()`

` void passwd() `//注意先检查权限等级

`void useradd()`

`void register()`//要检查权限等级

`void delete()`

}

#### class4:Books

成员：ISBN，bookname，author，keyword，price，amount

实现下列函数：

` show()` 

`modify()`

`select()`

`import()`

`buy()`  

#### class5：Diary

需要包括成员：long long int time

实现下列函数

` addTime()//添加交易记录`

` addLine()//添加日志记录`

` selfReport()仅包括某账户的日志` 

`financeReport()调出简易版的交易记录`

`latestFinance()//调出一定量的交易记录`

`employeeReport()仅包括员工（priority=3）的日志`

`log()//日志` 

#### class6:Error

###### 抛出错误消息，这里要使用what（）方法。除此之外只需要写构造、拷贝构造、析构函数。

#### headfile：Express.h:

`express（）`

`usage：express（TokenScanner*，Accounts current_account,Diary myDiary ）`

###### 然后根据firstToken判断需要执行的函数，调用Books，Accounts，Diary中的各个操作函数。 

###### 函数完成后（包括error结束）将本次操作格式化成一个字符串，进入Diary类，进行必要的日志记录，最后通过MemoryRiver存入对应的文件。



## 具体算法说明

##### 账户系统

- **su**:先遍历找到ID对应账户，没有直接报错。再检查priority，如果可以省密码直接过。不能省则检验密码，不过就报错

- **logout**：主要问题是实现登陆栈。直接在main里面使用queue，vector等等结构都可实现，看自己喜好。

- **passwd** ：先遍历，找到id对应账户，不存在立马报错；检查priority是否为7，为7就不检查旧密码，直接更新密码；如果不为7就要核对旧密码，错误就报错，正确再更新密码

- **useradd** ：先暂存新的userid与password，先让tokenscanner到priority的位置，比较当前账户和待创建的权限，当前账户较小则报错；若可行，则创建一个新的账户。

- **delete** ：现在登陆栈里面遍历找ID，看是否已经登陆，若已经登陆则报错；再在account_data里面遍历id，若不存在则失败

- **register**：创建一个新的账户，priority=1

##### 图书系统

- **show** :先检查有没有多个keyword，有直接报错。确定参数类型，直接遍历对应的块状链表，有符合条件的就输出

- **buy** ：find（），amount-=quantity，记录入transfer_data和finance_data

- **select** ：find（）

- **modify** ：find（），修改信息

- **import： **find（），如果找不到，insert（），amount+=quantity，记录入transfer_data和finance_data

##### 日志系统

- **addTime** 向finance_data和transfer_data添加记录。（这两个文件总是同时添加记录）

- **addLine** 向log_data添加记录。参数可以是日志需要的格式化的字符串。

- **selfReport** ：遍历log_data，输出其中满足【User-ID==current_account.ID】的记录

- **financeReport** ：调出transfer_data

- **latestFinance[time]:**从尾部开始遍历[time]个finance_data并输出

- **employeeReport**:遍历log_data，输出其中满足【priority==3】的记录

- **log**：调出log_data

以上所有函数执行完毕后均要记录入log_data。

## 其他

#### 数据存储的格式/日志输出格式

- ##### 用户数据格式（account_data）


###### ID（30）+username（30）+password（30）+priority(1)+online（1）



- ##### 书本数据格式（books_data）

###### ISBN（60）+name（60）+keyword（60）+author（60）+amount（10）+price(13)



- ##### 日志格式(log_data)


###### 一次操作格式化为一个字符串:     User-ID（30）+’{‘+priority（1）+’}‘+op（~100）

括号内数为预设长度，**如果不够则用’#'补**

op=：

su——“login.”

logout——“logout.”

passwd——“has modified his password：“+Oldpassword+” to “+Newpassword

useradd——“has added an account:”++ID+ '{' + priority+ '}' 

delete——"has deleted an account:"+ID+'{' + priority+ '}'

register——"has registered a new account:"+ID

show——“has shown books under these conditions：”+输入的那一串

modify——“has modified：“+ISBN

select——"has selected:"+ISBN

buy——"has bought "+ISBN+’*‘+Quantity

import——"has imported "+ISBN+'*'+Quantity



- ##### 交易记录格式(finance_data)


仅涉及到 buy和import

格式：ISBN+“ in *”+Quantity+",+"+total-price或者 ISBN+“out *”+Quantity+",-"+total-cost

total-price为quantity和price 的乘积



- ##### 收支记录格式（transfer_data）

同样只涉及到buy and import

题目已给出：+（收入）/ -（支出）

一个char（‘+’或‘-’）+一个int为一个块



