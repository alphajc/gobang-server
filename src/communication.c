/*************************************************************************
    > File Name: communication.c
    > Author: gavinchan
    > Mail: gavinchan267@163.com
    > Created Time: 2016年08月27日 星期六 14时11分00秒
 ************************************************************************/
#include "communication.h"

extern list playerList;
extern pthread_mutex_t list_mutex;

void set_monitor(int socketfd) {
  struct sockaddr_in client_addr;
  int sin_size = sizeof(struct sockaddr_in);
  int new_fd;
  if ((new_fd = accept(socketfd, (struct sockaddr *)(&client_addr),
                       &sin_size)) == -1) {
    fprintf(stderr, "Accept error:%s\n\a", strerror(errno));
    exit(1);
  }

  //创建线程
  Arg arg;
  arg.socketfd = new_fd;
  arg.client_addr = client_addr;
  pthread_t communicate_thread;
  pthread_create(&communicate_thread, NULL, (void *)communicate, (void *)&arg);
}
void communicate(void *arg) {
  Arg msg = *((Arg *)arg);

  PlayerInfo playerInfo;
  strcpy(playerInfo->ip, inet_ntoa(msg.client_addr.sin_addr));
  playerInfo->socketfd = msg.socketfd;
  playerInfo->state = STATE_ONLINE;

  pthread_mutex_lock(&list_mutex);
  if (!send_list_to_new(msg.socketfd)) {
    printf("It is failed to send the list to new player.\n");
  }

  if (!iterator(playerList, send_new_ip, playerInfo->ip)) {
    /* code */
    printf("iterator failed.\n");
  }

  if (!insert_list(&playerList, playerInfo)) {
    /* code */
    printf("It is failed to insert \"%s\"\'s information. \n", playerInfo->ip);
  }
  pthread_mutex_unlock(&list_mutex);
}

bool send_list_to_new(int newPlayerSocketFd) {
  char *message = pakage_message(MESSAGE_TYPE_LIST, NULL);
  if (send(newPlayerSocketFd, message, strlen(message) + 1, MSG_DONTWAIT) < 0) {
    /* code */
    return false;
  }
  return true;
}

bool send_new_ip(PlayerInfo playerInfo, char *newPlayerIp) {
  char *message = pakage_message(MESSAGE_TYPE_ADD, (void *)newPlayerIp);
  if (send(playerInfo->socketfd, message, strlen(message) + 1, MSG_DONTWAIT) <
      0) {
    /* code */
    return false;
  }
  return true;
}

bool send_remove_ip(PlayerInfo playerInfo, char *removeIp) {
  char *message = pakage_message(MESSAGE_TYPE_REMOVE, (void *)removeIp);
  if (send(playerInfo->socketfd, message, strlen(message) + 1, MSG_DONTWAIT) <
      0) {
    /* code */
    return false;
  }
  return true;
}

char *pakage_message(enum MessageType type, void *data) {
  char *message;
  switch (type) {
  case MESSAGE_TYPE_LIST: {
    list *p = &playerList;
    message = (char *)malloc(sizeof(char[16]) + 3);
    strcpy(message, "0\n");
    while (p) {
      message = (char *)realloc(message, strlen(message) + 17);
      strcat(message, p->playerInfo->ip);
      strcat(message, "\n");
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
