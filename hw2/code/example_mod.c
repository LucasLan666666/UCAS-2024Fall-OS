#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>

#define LOOP 2

int main(int argc,char *argv[])
{
    pid_t pid;
    int loop;

    for (loop=0; loop<LOOP; loop++) {
        if((pid=fork()) < 0)
            fprintf(stderr, "fork failed\n");
        else if(pid == 0) {
            printf(" I am child process\n");
            /* 第二种方法：阻止子进程创建新的子进程 */
            // wait(NULL);
            // return 0;
        }
        else {
            sleep(5);
            wait(NULL); /* 助教补充：安全起见最好等待子进程结束 */
            return 0; /* add this line */
        }
    }
    return 0;
}
