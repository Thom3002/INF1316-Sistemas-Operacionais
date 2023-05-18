#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define MAX_TIME 15

void handler(int signal);

int main()
{
    srand(time(NULL));
    signal(SIGUSR1, handler);
    printf("Atendente: Esperando chamada...\n");
    while (1)
    {
        pause();
    }
    return 0;
}

void handler(int signal)
{
    int time = 0;
    printf("Atendente: Atendendo chamada...\n");
    while (time < MAX_TIME)
    {
        sleep(1);
        time++;
        if (rand() % 10 == 0)
        {
            printf("Atendente: Desculpe, estou com problemas técnicos. Posso retornar a ligação em alguns minutos?\n");
            return;
        }
    }
    printf("Atendente: Chamada encerrada após %d segundos.\n", MAX_TIME);
    kill(getppid(), SIGUSR2);
}
