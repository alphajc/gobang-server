/*************************************************************************
    > File Name: communication.c
    > Author: gavinchan
    > Mail: gavinchan267@163.com
    > Created Time: 2016年08月27日 星期六 14时11分00秒
 ************************************************************************/
#include "communication.h"

#define LISTEN_PORT 5267
#define MAX_BUFFER 1024

void communication() {
  int sockfd;
  struct sockaddr_in server_addr; //描述服务器地址

  //服务器建立套接字描述符
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    /* code */
    fprintf(stderr, "socket error:%s\n", strerror(errno));
    exit(1);
  }

  int opt = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  //设置服务端sockaddr
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(LISTEN_PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  //将sockfd与套接字绑定
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("bind error:\n");
    exit(0);
  }

  //设置允许连接的最大客户数
  if (listen(sockfd, 20) == -1) {
    /* code */
    perror("listen error:");
  }

  init_list(&playerList); //初始化playerList

  //监听
  while (1) {
    set_monitor(sockfd);
  }
}

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
  fd_set rset;

  PlayerInfo playerInfo = (PlayerInfo)malloc(sizeof(struct information));
  strcpy(playerInfo->ip, inet_ntoa(msg.client_addr.sin_addr));
  playerInfo->socketfd = msg.socketfd;
  playerInfo->state = STATE_ONLINE;

  printf("%s\n", playerInfo->ip);

  if (!send_list_to_new(msg.socketfd)) {
    printf("It is failed to send the list to new player.\n");
  }

  if (!iterator(playerList, send_new_ip, playerInfo->ip)) {
    /* code */
    printf("iterator failed.\n");
  }

  pthread_mutex_lock(&list_mutex);
  if (!insert_list(&playerList, playerInfo)) {
    /* code */
    printf("It is failed to insert \"%s\"\'s information. \n", playerInfo->ip);
  }
  pthread_mutex_unlock(&list_mutex);

  char message[MAX_BUFFER];
  if (0 == recv(msg.socketfd, message, MAX_BUFFER,
                MSG_WAITALL)) { //用recv解决客户端断开连接，立即捕捉的问题
    pthread_mutex_lock(&list_mutex);
    if (!remove_list(&playerList, msg.socketfd)) {
      printf("It is failed to remove \"%s\"\'s information. \n",
             playerInfo->ip);
    }
    pthread_mutex_unlock(&list_mutex);

    if (!iterator(playerList, send_remove_ip, playerInfo->ip)) {
      /* code */
      printf("iterator failed.\n");
    }

    close(msg.socketfd);
    pthread_exit(0);
  }
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
