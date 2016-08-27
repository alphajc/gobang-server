/*************************************************************************
    > File Name: communication.h
    > Author: gavinchan
    > Mail: gavinchan267@163.com
    > Created Time: 2016年08月27日 星期六 14时03分38秒
 ************************************************************************/
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "list.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

enum MessageType {
  MESSAGE_TYPE_LIST,       //发送玩家列表
  MESSAGE_TYPE_ADD,        //添加玩家
  MESSAGE_TYPE_REMOVE,     //移除玩家
  MESSAGE_TYPE_INVIATION,  // 邀请（端口）
  MESSAGE_TYPE_REPLY,      //受邀方回复
  MESSAGE_TYPE_CONNECTION, //连接信息（发起方IP和端口）
  MESSAGE_TYPE_CONERROR    //受邀方连接错误
};

typedef struct {
  int socketfd;
  struct sockaddr_in client_addr;
} Arg;

list playerList;            //玩家列表
pthread_mutex_t list_mutex; //给列表加互斥锁

void set_monitor(); // 1.  thread1
void communicate(); // 5.服务器与玩家消息维护  thread*n
bool send_list_to_new(int newPlayerSocket); // 3.发送列表给新上线玩家
bool send_new_ip(PlayerInfo playerInfo, char *newPlayerIp); // 4.
bool send_remove_ip(PlayerInfo playerInfo, char *removeIp); // 6.
char *pakage_message(enum MessageType type, void *data);    //封装消息
bool resolve_message(char *message);                        //解析消息
bool process_message(enum MessageType type, char *msg);     //处理消息

#endif
