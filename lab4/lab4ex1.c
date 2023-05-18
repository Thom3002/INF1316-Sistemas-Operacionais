#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h> // Para o tipo pid_t

int main(void)
{

    pid_t pid;

    pid = fork(); // cria processo hierarquico

    if (pid == 0)
    { // processo filho
        printf("Processo filho pid = %d...\n", getpid());
    }

    else
    { // processo pai
        for (int i = 0; i < 5; i++)
        {
            sleep(2); // filho dorme por 2s
            kill(pid, SIGSTOP);
            sleep(3);
            printf("Processo filho %d interrompido por 3 segundos...\n", pid);
            kill(pid, SIGCONT);
        }

        kill(pid, SIGKILL); // mata o filho
    }
    return 0;
}