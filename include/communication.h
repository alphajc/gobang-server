/*************************************************************************
    > File Name: communication.h
    > Author: gavinchan
    > Mail: gavinchan267@163.com
    > Created Time: 2016年08月27日 星期六 14时03分38秒
 ************************************************************************/
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "list.h"
#include "message.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  int socketfd;
  struct sockaddr_in client_addr;
} Arg;

// extern list playerList;
pthread_mutex_t list_mutex; //给列表加互斥锁

void communication();
void set_monitor(); // 1.  thread1
void communicate(); // 5.服务器与玩家消息维护  thread*n
bool send_list_to_new(int newPlayerSocket); // 3.发送列表给新上线玩家
bool send_new_ip(PlayerInfo playerInfo, void *info);        // 4.
bool send_remove_ip(PlayerInfo playerInfo, void *removeIp); // 6.
int find_socket_fd(Messages msg);   //返回套接字
bool process_message(Messages msg); //处理消息

#endif
