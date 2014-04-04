#include "momo_net.h"

Sigfunc * signal (int signo, Sigfunc *func)
{
    struct sigaction act, oact;
    
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    if (signo == SIGALRM)
    {
        #ifdef SA_RESTART
            act.sa_flags |= SA_RESTART;
        #endif
    }
    
    if (sigaction (signo, &act, &oact) < 0)
        return (SIG_ERR);
    
    return (oact.sa_handler);
}

void sig_chld(int signo)
{
    
    pid_t pid;
    int   stat;
    
    // This one is WRONG :
    // It will only receive one of few SIGCHLD signals,
    // leaving zombie processes
    
    //pid = wait(&stat);
    
    // This one is CORRECT :
    
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf(">>> child %d terminated\n", pid);
    return;
    
}
