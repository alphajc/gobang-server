# gobang-server
该程序用于汇集玩家信息，组织客户端连接。


概要设计：

连接过程：

Server:（Linux C编程）

1.	监听玩家上线 --- 设置监听套接字（TCP连接）

2.	添加玩家列表 --- 将玩家信息（IP->【面向服务器的套接字，状态】）维护到本地玩家列表，初始状态为在线（玩家列表存入玩家队列）

3.	发送列表给上线玩家 --- 发送IP及状态的列表

4.	发送上线玩家给其他玩家

5.	监听玩家是否在线 --- 如果离线，转6

6.	发送离线玩家给其他玩家

7.	转发邀请信息 --- 发送玩家IP到被邀请玩家

8.	确认邀请信息 --- 处理邀请结果，如成功转10；如失败转9

9.	返回失败信息 --- 如果失败，向玩家返回原因（拒绝）

10.	给受邀请玩家发送玩家IP地址和端口

11.	接收受邀玩家连接错误 ---如有，告知另一玩家


数据结构设计：

enum State{ STATE_ONLINE, STATE_GAMING };   //状态分为两种，在线上/游戏中

enum MessageType{ MESSAGE_TYPE_LIST,        //发送玩家列表

                  MESSAGE_TYPE_ADD,         //添加玩家

                  MESSAGE_TYPE_REMOVE,      //移除玩家

                  MESSAGE_TYPE_INVIATION,   // 邀请（端口）

                  MESSAGE_TYPE_REPLY,       //受邀方回复

                  MESSAGE_TYPE_CONNECTION,  //连接信息（发起方IP和端口）

                  MESSAGE_TYPE_CONERROR     //受邀方连接错误

                };


typedef struct {

  char * ip;    //ip地址作为玩家信息主键使用，特别记录

  int socket;   //存储套接字，用于遍历网络接口，批量发送信息

  State state;  //记录玩家状态

} *PlayerInfo;

list playerList; //玩家列表


函数设计：

<list>

bool init_list( list * l );                 //[ head:include<stdbool.h> ]  l <- &playerList

bool insert_list( list * l, PlayerInfo playerInfo );   //上线玩家

bool remove_list( list * l, char * ip );           //下线玩家

bool iterator( list * l, void (* deal)(PlayerInfo, char *),char * ip );  //遍历玩家,并进行相应处理


void set_monitor(int socketfd); //1.  thread1

void communicate(); //5.服务器与玩家消息维护  thread*n

bool send_list_to_new( list playerList, int newPlayerSocketFd );  //3.发送列表给新上线玩家

bool send_new_ip( PlayerInfo,char * newPlayerIp ); //4.

bool send_remove_ip( PlayerInfo,char * removeIp ); //6.

bool pakage_message( MessageType type, char * msg );  //封装消息

bool resolve_message( char * message );  //解析消息

bool process_message( MessageType type, char * msg);  //处理消息
