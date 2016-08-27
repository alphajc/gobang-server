/*************************************************************************
    > File Name: ../src/list.c
    > Author: gavinchan
    > Mail: gavinchan267@163.com
    > Created Time: 2016年08月27日 星期六 13时14分21秒
 ************************************************************************/

#include "list.h"

bool init_list(list *l) {
  l = NULL;

  return true;
}

bool insert_list(list *l, PlayerInfo playerInfo) {
  list *p;
  if ((p = (list *)malloc(sizeof(list))) < 0) {
    return false;
  }
  p->playerInfo = playerInfo;
  p->next = l;
  l = p;

  return true;
}

bool remove_list(list *l, char *ip) {
  list *p, *q;
  if (!(p = l)) {
    q = p->next;
  }

  if (p->playerInfo->ip == ip) {
    l = p->next;
    free(p);
    p = NULL;
    return true;
  }

  while (q != NULL && q->playerInfo->ip != ip) {
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

bool iterator(list l, bool (*deal)(PlayerInfo, char *), char *ip) {
  list *p = &l;
  while (p != NULL) {
    deal(p->playerInfo, ip);
    p = p->next;
  }

  return true;
}
