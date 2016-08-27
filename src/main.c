#include "communication.h"

#define LISTEN_PORT 5267

int main(int argc, char const *argv[]) {
  /* code */
  int sockfd;
  struct sockaddr_in server_addr; //描述服务器地址

  //服务器建立套接字描述符
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    /* code */
    fprintf(stderr, "socket error:%s\n", strerror(errno));
    exit(1);
  }

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

  //监听
  while (1) {
    /* code */
    set_monitor(sockfd);
  }

  return 0;
}
