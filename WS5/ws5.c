#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    fork();
    fork();
    printf("hello world from PID %d!\n", getpid());
    return 0;
}
