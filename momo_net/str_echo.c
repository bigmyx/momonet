#include "momo_net.h"

void str_echo(int fd)
{
    ssize_t n;
    char    buff[MAXLINE];
    
    while ( (n = recv(fd, buff, MAXLINE, 0)) > 0)
        send(fd, buff, n, 0);
    
    
    
}