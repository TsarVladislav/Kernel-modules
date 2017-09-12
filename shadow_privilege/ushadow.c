/*
 * ushadow.c - программа пытается открыть /etc/shadow.
 */
#define _POSIX_SOURCE
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define PROGRAMM_NAME "kshadow"
#define PATH "/etc/shadow"
void my_handler(int signum)
{
    FILE *fp;
    if (signum == SIGUSR1) {
        fp = fopen(PATH, "r");
        if (fp != NULL) {
            fclose(fp);
            printf("Открыл!\n");
            exit(1);
        } else {
            printf("Не могу открыть\n");
        }
    }
}

int main(int argc, const char *argv[])
{
    int pid;
    FILE *procf;
    pid = getpid();
    procf = fopen("/proc/" PROGRAMM_NAME, "w");

    signal(SIGUSR1, my_handler);

    /* Мы хотим сделать так, чтобы модуль ядра повысил привелегии этой
     * программы. По умолчанию, обычный пользователь не может */
    if (procf == NULL) {
        printf("Не могу открыть файл\n");
        return 0;
    }
    fprintf(procf,"%d", pid);
    fclose(procf);

    printf("Отправил pid(%d)...\n", pid);
    while(1){
        printf("czekamy\n");
        kill(pid, SIGUSR1);
    }
    return 0;
}


