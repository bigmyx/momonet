#include "momo_net.h"



int main(int argc, const char * argv[])
{
    struct sockaddr_in   servaddr, clntddr;
    socklen_t len;
    int       sockfd;
    int       connfd;
    char      buff[MAXLINE];
    pid_t     childpid;
     
    // init socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
        die("error creating a socket");
    printf(">>> created socket: %d\n", sockfd);
    
    
    // put the address to listen on
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SOCK_PORT);    /* daytime server port */
    
    
    // bind the listening address to the socket
    int srv = bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (srv == -1)
        die("failed to bind");
    printf(">>> bound to socket: %d\n", srv);
    
    //listen
    if ((listen(sockfd, BACKLOG)) < 0)
        die("failed to listen");
    printf(">>> Listening for connections ...\n");
    
    signal(SIGCHLD, sig_chld);           /* signal handler - must call waitpid() ! */
    
    for (;;)
    {
    
        len = sizeof(clntddr);
        connfd = accept(sockfd, (struct sockaddr *) &clntddr, &len);
        
//        if ((connfd = accept(sockfd, (struct sockaddr *) &clntddr, &len)) < 0)
//        {
//            if (errno == SIGINT)
//                continue;
//            else
//            die ("!!! accept error !!!");
//        }
        
        
        /* log the client connection */
        printf("connection from %s, port %d\n",
               inet_ntop(AF_INET, &clntddr.sin_addr, buff, sizeof(buff)),
               ntohs(clntddr.sin_port));
        
        if ( (childpid = fork()) == 0)
        {
            printf("[PID::%d]",childpid);  /* I'm I child ??? */
            
            close(sockfd);                 /* child closes listening socket */
            str_echo(connfd);
            exit(0);
        }
        
        printf(">>> Closing ...\n");
        
        close(connfd);                     /*parent closes connected socket*/
        
    }
    
    return 0;
}

