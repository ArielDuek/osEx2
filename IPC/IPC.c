#include <stdio.h>
#include <signal.h>
#include <unistd.h> //for pipe() and fork()
#include <stdlib.h>
#include <sys/wait.h>

//In order for the processes to be updated and exchanged information we created a common arr where one cell will be used
// for reading and another cell will be used for writing.rw[0] for read rw[1] for write.
int rw[2];
int flagToStop=1;
int value=0;
int temp=0;
pid_t pFather;

void signalHandleFather(int signal) {
    read(rw[0], &temp, sizeof(int));
    if (temp < 6) {
        printf("%d\n", temp);
        value = temp + 1;
        write(rw[1], &value, sizeof(int));
        kill(pFather, SIGUSR1);
    }else flagToStop=0;
}

void signalHandleSon(int signal)
{
    read(rw[0],&temp, sizeof(int));
    printf("%d\n",temp);
    value=temp+1;
    write(rw[1],&value, sizeof(int));
    kill(getppid(), SIGUSR1);
    if(value==6) exit(0);
}

int main()
{
    //In order for both processes to have a common memory, the pipe must be created before the fork
    if(pipe(rw)<0)
    {
        printf("Pipe did not succeed");
        return -1;
    }
    //make another process like the parent process,this will be child process .Father see pFather as the son pid.
    // Son see pFather as 0.
    pFather=fork();

    if(pFather<0)
    {
        printf("Fork did not succeed");
        return -1;
    }
    if(pFather>0) //its the father processes because it is bigger then 0
    {
        signal(SIGUSR1, signalHandleFather);
    }
    else if(pFather==0)//son processes
    {
        signal(SIGUSR1, signalHandleSon);
        write(rw[1], &value, sizeof(int));
        kill(getppid(),SIGUSR1);
    }
    while(flagToStop){}
    wait(NULL);
    printf("Child is going to be terminated\n");
    printf("Parent is going to be terminated\n");
    close(rw[0]);
    close(rw[1]);

    return 0;
}