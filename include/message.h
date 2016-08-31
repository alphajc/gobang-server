#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "list.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
  enum MessageType type;
  char src_ip[16];
  char det_ip[16];
  char port[8];
  char data[10];
} Messages;

char *pakage_message(enum MessageType type, void *data); //封装消息
Messages resolve_message(char *message);                       //解析消息

#endif
