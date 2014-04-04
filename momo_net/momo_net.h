

#ifndef momo_net_h
#define momo_net_h

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BACKLOG   10
#define MAXLINE   4096
#define SOCK_PORT 3333

void die(const char * str);
void str_echo(int fd);

typedef    void     Sigfunc(int);
Sigfunc    *signal  (int signo, Sigfunc *func);
void       sig_chld (int signo);


#endif
