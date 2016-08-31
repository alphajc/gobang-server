#include "message.h"

char *pakage_message(enum MessageType type, void *data) {
  char *message;
  switch (type) {
  case MESSAGE_TYPE_LIST: {
    list p = playerList;
    PlayerInfo info = (PlayerInfo)data;
    message = (char *)malloc(2);
    sprintf(message, "%d", MESSAGE_TYPE_LIST);
    while (p) {
      message =
          (char *)realloc(message, strlen(message) + sizeof(info->ip) + 3);

      strcat(message, "\n");
      strcat(message, p->playerInfo->ip);
      strcat(message, "\t");
      char trans[3];
      sprintf(trans, "%d", p->playerInfo->state);
      strcat(message, trans);
      p = p->next;
    }
    break;
  }
  case MESSAGE_TYPE_ADD: {
    PlayerInfo info = (PlayerInfo)data;
    message = (char *)malloc(strlen(message) + sizeof(info->ip) + 3);
    sprintf(message, "%d\n", MESSAGE_TYPE_ADD);
    strcat(message, info->ip);
    strcat(message, "\t");
    char trans[3];
    sprintf(trans, "%d", info->state);
    strcat(message, trans);
    break;
  }
  case MESSAGE_TYPE_REMOVE: {
    char *ip = (char *)data;
    message = (char *)malloc(strlen(ip) + 3);
    sprintf(message, "%d\n", MESSAGE_TYPE_REMOVE);
    strcat(message, ip);
    break;
  }
  case MESSAGE_TYPE_INVIATION: {
    Messages *msg = (Messages *)data;
    message = (char *)malloc(sizeof(msg->src_ip) + sizeof(msg->port) + 3);
    sprintf(message, "%d\n", MESSAGE_TYPE_INVIATION);
    strcat(message, msg->src_ip);
    strcat(message, "\n");
    strcat(message, msg->port);
    break;
  }
  case MESSAGE_TYPE_REPLY: {
    char *reply = (char *)data;
    printf("%s\n", reply);
    message = (char *)malloc(strlen(data) + 3);
    sprintf(message, "%d\n", MESSAGE_TYPE_REPLY);
    strcat(message, reply);
    break;
  }
  case MESSAGE_TYPE_CONNECTION:

    break;
  case MESSAGE_TYPE_CONERROR:

    break;
  default:
    printf("Wrong type!!!\n");
  }
  return message;
}

Messages resolve_message(char *message) {
  char *temp = NULL;
  Messages msg;
  printf("message.c-75-message:%send\n", message);
  temp = strtok(message, "\n");
  msg.type = (enum MessageType)(atoi(temp));
  switch (msg.type) {
  case MESSAGE_TYPE_INVIATION:
    temp = strtok(NULL, "\n");
    strcpy(msg.src_ip, temp);
    temp = strtok(NULL, "\n");
    strcpy(msg.det_ip, temp);
    temp = strtok(NULL, "\n");
    strcpy(msg.port, temp);
    break;
  case MESSAGE_TYPE_REPLY:
    temp = strtok(NULL, "\n");
    strcpy(msg.det_ip, temp);
    temp = strtok(NULL, "\n");
    printf("temp:%s\n", temp);
    strcpy(msg.data, temp);
    break;
  case MESSAGE_TYPE_CONNECTION:
  case MESSAGE_TYPE_CONERROR:
  default:
    printf("Wrong type!!!\n");
  }

  return msg;
}
