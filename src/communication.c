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

void handler(int signo) {
  if (signo == SIGPIPE) {
    printf("SIGPIPE\n");
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

  struct sigaction action;
  action.sa_flags = 0;
  action.sa_handler = handler;

  sigaction(SIGPIPE, &action, NULL);

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

  if (!iterator(playerList, send_new_ip, (void *)playerInfo)) {
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
  int recvSize;
  while (1) {
    recvSize = recv(msg.socketfd, message, MAX_BUFFER, 0);
    if (recvSize > 1) {
      Messages msg = resolve_message(message);
      process_message(msg); //处理消息
    } else if (1 == recvSize ||
               0 == recvSize) { //用recv解决客户端断开连接，立即捕捉的问题
      pthread_mutex_lock(&list_mutex);
      if (!remove_list(&playerList, msg.socketfd)) {
        printf("It is failed to remove \"%s\"\'s information. \n",
               playerInfo->ip);
      }
      pthread_mutex_unlock(&list_mutex);

      if (!iterator(playerList, send_remove_ip, (void *)playerInfo->ip)) {
        /* code */
        printf("iterator failed.\n");
      }

      close(msg.socketfd);
      pthread_exit(0);
    } else {
      perror("recv error:");
      close(msg.socketfd);
      pthread_exit(0);
    }
  }
}

bool send_list_to_new(int newPlayerSocketFd) {
  char *message = pakage_message(MESSAGE_TYPE_LIST, NULL);
  printf("%s\n", message);
  if (send(newPlayerSocketFd, message, strlen(message) + 1, MSG_DONTWAIT) < 0) {
    return false;
  }
  return true;
}

bool send_new_ip(PlayerInfo playerInfo, const void *info) {
  char *message = pakage_message(MESSAGE_TYPE_ADD, playerInfo);
  if (send(playerInfo->socketfd, message, strlen(message) + 1, MSG_DONTWAIT) <
      0) {
    return false;
  }
  return true;
}

bool send_remove_ip(PlayerInfo playerInfo, const void *removeIp) {
  char *message = pakage_message(MESSAGE_TYPE_REMOVE, removeIp);
  if (send(playerInfo->socketfd, message, strlen(message) + 1, MSG_DONTWAIT) <
      0) {
    return false;
  }
  return true;
}

int find_socket_fd(Messages msg) {
  list p = playerList;
  while (p) {
    if (strcmp((p->playerInfo->ip), msg.det_ip) == 0)
      return p->playerInfo->socketfd;
    p = p->next;
  }
  return -1;
}

bool process_message(Messages msg) {
  int socketfd;
  char *message;
  switch (msg.type) {
  case MESSAGE_TYPE_INVIATION:
    if ((socketfd = find_socket_fd(msg)) < 3) {
      printf("find socket file description failed:%d\n", socketfd);
      return false;
    }
    message = pakage_message(MESSAGE_TYPE_INVIATION, (void *)&msg);
    if (send(socketfd, message, strlen(message) + 1, MSG_DONTWAIT) < 0) {
      return false;
    }
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
  return true;
}
