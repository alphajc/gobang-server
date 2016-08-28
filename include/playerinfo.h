/*************************************************************************
    > File Name: playerinfo.h
    > Author: gavinchan
    > Mail: gavinchan267@163.com
    > Created Time: 2016年08月27日 星期六 13时58分17秒
 ************************************************************************/
#ifndef _PLAYERINFO_H_
#define _PLAYERINFO_H_

enum State { STATE_ONLINE, STATE_GAMING }; //状态分为两种，在线上/游戏中

typedef struct information {
  char ip[16];  // ip地址作为玩家信息主键使用，特别记录
  int socketfd; //存储套接字，用于遍历网络接口，批量发送信息
  enum State state; //记录玩家状态
} * PlayerInfo;

#endif
