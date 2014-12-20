#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int flag=0;

void show_handler(int sig)
{
    printf("I got signal %d\n", sig);
    int i;
    for(i = 0; i < 5; i++) {
        printf("i = %d\n", i);
        sleep(1);
    }
    flag=1;
}

int main(void)
{
    int i = 0;
    struct sigaction act, oldact;
    act.sa_handler = show_handler;
    sigaddset(&act.sa_mask, 30); //(1)
    act.sa_flags = SA_RESETHAND | SA_NODEFER; //(2)
    //act.sa_flags = 0; //(3)

    sigaction(30, &act, &oldact);
    while(!flag) {
        sleep(1);
        printf("sleeping %d\n", i);
        i++;
    }
}


