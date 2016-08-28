#include "message.h"

char *pakage_message(enum MessageType type, void *data) {
  char *message;
  switch (type) {
  case MESSAGE_TYPE_LIST: {
    list p = playerList;
    message = (char *)malloc(sizeof(char[16]) + 3);
    strcpy(message, "0\n");
    while (p) {
      message = (char *)realloc(message, strlen(message) + 17);
      strcat(message, p->playerInfo->ip);
      strcat(message, "\n");
      p = p->next;
    }
    break;
  }
  case MESSAGE_TYPE_ADD: {
    char *ip = (char *)data;
    message = (char *)malloc(strlen(ip) + 3);
    strcpy(message, "1\n");
    strcat(message, ip);
    break;
  }
  case MESSAGE_TYPE_REMOVE: {
    char *ip = (char *)data;
    message = (char *)malloc(strlen(ip) + 3);
    strcpy(message, "2\n");
    strcat(message, ip);
    break;
  }
  case MESSAGE_TYPE_INVIATION:

    break;
  case MESSAGE_TYPE_REPLY:

    break;
  case MESSAGE_TYPE_CONNECTION:

    break;
  case MESSAGE_TYPE_CONERROR:

    break;
  default:
    printf("Wrong type!!!\n");
  }
  return message;
}

bool resolve_message(char *message) {}

bool process_message(enum MessageType type, char *msg) {}
