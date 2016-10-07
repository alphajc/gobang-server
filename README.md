gobang-server
===============
---该程序用于汇集玩家信息，组织客户端连接。
---------------

#概要设计：</br>
连接过程：</br>
Server:（Linux C编程）</br>
1.	监听玩家上线 --- 设置监听套接字（TCP连接）</br>
2.	添加玩家列表 --- 将玩家信息（IP->【面向服务器的套接字，状态】）维护到本地玩家列表，初始状态为在线（玩家列表存入玩家队列）</br>
3.	发送列表给上线玩家 --- 发送IP及状态的列表</br>
4.	发送上线玩家给其他玩家</br>
5.	监听玩家是否在线 --- 如果离线，转6</br>
6.	发送离线玩家给其他玩家</br>
7.	转发邀请信息 --- 发送玩家IP到被邀请玩家</br>
8.	确认邀请信息 --- 处理邀请结果，如成功转10；如失败转9</br>
9.	返回失败信息 --- 如果失败，向玩家返回原因（拒绝）</br>
10.	给受邀请玩家发送玩家IP地址和端口</br>
11.	接收受邀玩家连接错误 ---如有，告知另一玩家</br></br>

#数据结构设计：</br>
```C
enum State{ STATE_ONLINE, STATE_GAMING };   //状态分为两种，在线上/游戏中</br>
enum MessageType{ MESSAGE_TYPE_LIST,        //发送玩家列表</br>
                  MESSAGE_TYPE_ADD,         //添加玩家</br>
                  MESSAGE_TYPE_REMOVE,      //移除玩家</br>
                  MESSAGE_TYPE_INVIATION,   // 邀请（端口）</br>
                  MESSAGE_TYPE_REPLY,       //受邀方回复</br>
                  MESSAGE_TYPE_CONNECTION,  //连接信息（发起方IP和端口）</br>
                  MESSAGE_TYPE_CONERROR     //受邀方连接错误</br>
                };</br></br>

typedef struct {</br>
  char * ip;    //ip地址作为玩家信息主键使用，特别记录</br>
  int socket;   //存储套接字，用于遍历网络接口，批量发送信息</br>
  State state;  //记录玩家状态</br>
} * PlayerInfo;</br>
list playerList; //玩家列表</br>
```
函数设计：</br>
<list></br>
```c
bool init_list( list * l );                 //[ head:include<stdbool.h> ]  l <- &playerList</br>
bool insert_list( list * l, PlayerInfo playerInfo );   //上线玩家</br>
bool remove_list( list * l, char * ip );           //下线玩家</br>
bool iterator( list * l, void (* deal)(PlayerInfo, char * ), char * ip );  //遍历玩家,并进行相应处理</br>

void set_monitor(int socketfd); //1.  thread1</br>
void communicate(); //5.服务器与玩家消息维护  thread*n</br>
bool send_list_to_new( list playerList, int newPlayerSocketFd );  //3.发送列表给新上线玩家</br>
bool send_new_ip( PlayerInfo,char * newPlayerIp ); //4.</br>
bool send_remove_ip( PlayerInfo,char * removeIp ); //6.</br>
bool pakage_message( MessageType type, char * msg );  //封装消息</br>
bool resolve_message( char * message );  //解析消息</br>
bool process_message( MessageType type, char * msg);  //处理消息</br>
```
