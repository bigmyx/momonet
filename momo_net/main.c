#include "momo_net.h"

int main(int argc, char *argv[])
{
    
    fd_set master;                                          /* master file descriptor list */
    fd_set read_fds;                                        /* temp file descriptor list for select() */
    struct sockaddr_in serveraddr;                          /* server address */
    struct sockaddr_in clientaddr;                          /* client address */
    int fdmax;                                              /* maximum file descriptor number */
    int listener;                                           /* listening socket descriptor */
    int newfd;                                              /* newly accept()ed socket descriptor */
    char buf[1024];                                         /* buffer for client data */
    size_t nbytes;
    int yes = 1;                                            /* for setsockopt() SO_REUSEADDR, below */
    socklen_t addrlen;
    int i, j;
    
    FD_ZERO(&master);                                       /* clear the master and temp sets */
    FD_ZERO(&read_fds);
    
    
    if((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)  /* get the listener */
    {
        perror("Server-socket() error lol!");
        /*just exit lol!*/
        exit(1);
    }
    printf("Server-socket() is OK...\n");
    /*"address already in use" error message */
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("Server-setsockopt() error lol!");
        exit(1);
    }
    printf("Server-setsockopt() is OK...\n");
    
    /* bind */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
    memset(&(serveraddr.sin_zero), '\0', 8);
    
    if(bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Server-bind() error lol!");
        exit(1);
    }
    printf("Server-bind() is OK...\n");
    
    /* listen */
    if(listen(listener, 10) == -1)
    {
        perror("Server-listen() error lol!");
        exit(1);
    }
    printf("Server-listen() is OK...\n");
    
    /* add the listener to the master set */
    FD_SET(listener, &master);
    /* keep track of the biggest file descriptor */
    fdmax = listener; /* so far, it's this one*/
    
    /* loop */
    for(;;)
    {
        /* copy it */
        read_fds = master;
        
        if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("Server-select() error lol!");
            exit(1);
        }
        printf("New data input...\n");
        
        /*run through the existing connections looking for data to be read*/
        for(i = 0; i <= fdmax; i++)
        {
            if(FD_ISSET(i, &read_fds))
            { /* we got one... */
                if(i == listener)
                {
                    /* handle new connections */
                    addrlen = sizeof(clientaddr);
                    if((newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen)) == -1)
                    {
                        perror("Server-accept() error lol!");
                    }
                    else
                    {
                        printf("Server-accept() is OK...\n");
                        
                        FD_SET(newfd, &master); /* add to master set */
                        if(newfd > fdmax)
                        { /* keep track of the maximum */
                            fdmax = newfd;
                        }
                        printf("New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), newfd);
                    }
                }
                else
                {
                    /* handle data from a client */
                    if((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0)
                    {
                        /* got error or connection closed by client */
                        if(nbytes == 0)
                        /* connection closed */
                            printf("socket %d hung up\n", i);
                        
                        else
                            perror("recv() error lol!");
                        
                        /* close it... */
                        close(i);
                        /* remove from master set */
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        /* we got some data from a client*/
                        for(j = 0; j <= fdmax; j++)
                        {
                            /* send to everyone! */
                            if(FD_ISSET(j, &master))
                            {
                                /* except the listener and ourselves */
                                if(j != listener && j != i)
                                {
                                    if(send(j, buf, nbytes, 0) == -1)
                                        perror("send() error lol!");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
