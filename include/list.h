/*************************************************************************
    > File Name: list.h
    > Author: gavinchan
    > Mail: gavinchan267@163.com
    > Created Time: 2016年08月27日 星期六 13时06分21秒
 ************************************************************************/
#ifndef _LIST_H_
#define _LIST_H_

#include "playerinfo.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct PlayerList {
  PlayerInfo playerInfo;
  struct PlayerList *next;
} list;

bool init_list(list *l); //[ head:include<stdbool.h> ]  l <- &playerList
bool insert_list(list *l, PlayerInfo playerInfo); //上线玩家
bool remove_list(list *l, char *ip);              //下线玩家
bool iterator(list l, bool (*deal)(PlayerInfo, char *),
              char *ip); //遍历玩家,并进行相应处理

#endif
