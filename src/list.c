/*************************************************************************
    > File Name: ../src/list.c
    > Author: gavinchan
    > Mail: gavinchan267@163.com
    > Created Time: 2016年08月27日 星期六 13时14分21秒
 ************************************************************************/

#include "list.h"

bool init_list(list *l) {
  *l = NULL;

  return true;
}

bool insert_list(list *l, PlayerInfo playerInfo) {
  list p;
  if ((p = (list)malloc(sizeof(struct PlayerList))) < 0) {
    perror("allocate playerList failed.");
    return false;
  }
  p->playerInfo = playerInfo;
  p->next = *l;
  *l = p;

  return true;
}

bool remove_list(list *l, int socketfd) {
  list p, q;
  if (!(p = *l)) {
    printf("The list is empty.\n");
    return false;
  }

  q = p->next;

  if (p->playerInfo->socketfd == socketfd) {
    *l = p->next;
    free(p);
    p = NULL;
    return true;
  }

  while (q != NULL && q->playerInfo->socketfd != socketfd) {
    q = q->next;
    p = p->next;
  }

  if (NULL == q) {
    return false;
  }

  p->next = q->next;
  free(q);
  q = NULL;
  return true;
}

bool iterator(list l, bool (*deal)(PlayerInfo, const void *),
              const void *data) {
  list p = l;
  while (p != NULL) {
    deal(p->playerInfo, data);
    p = p->next;
  }

  return true;
}
