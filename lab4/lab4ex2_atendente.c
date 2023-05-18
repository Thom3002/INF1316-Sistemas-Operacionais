#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define EVER \
    ;        \
    ;

void chamadaInicioHandler(int sig)
{
    printf("Chamada atendida!\n");
}

void chamadaDesligaHandler(int sig)
{
    int pid_chamador;
    printf("Digite o PID do chamador para desligar: ");
    scanf("%d", &pid_chamador);
    kill(pid_chamador, SIGUSR2);
}

int main(void)
{

    signal(SIGUSR1, chamadaInicioHandler);
    signal(SIGUSR2, chamadaDesligaHandler);
    printf("PID atendente: %d\n", getpid());
    pause();

    return 0;
}
