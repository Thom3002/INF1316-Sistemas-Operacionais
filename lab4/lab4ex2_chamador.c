#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

#define EVER \
    ;        \
    ;

struct timeval t_start, t_end;

void chamadaInicioHandler(int sig)
{
    int tempo_decorrido = 0;
    gettimeofday(&t_start, NULL);
    for (EVER)
    {
        sleep(1);
        printf("Tempo decorrido: %d\n", ++tempo_decorrido);
    }
}

void chamadaDesligaHandler(int sig)
{
    gettimeofday(&t_end, NULL);
    printf("Desligando!\n");
}

int main(void)
{

    double tempo_chamada;
    float preco_chamada;
    int pid_chamador;

    signal(SIGUSR1, chamadaInicioHandler);
    signal(SIGUSR2, chamadaDesligaHandler);

    pid_chamador = getpid();
    printf("PID chamador: %d\n", pid_chamador);

    // Chama
    int pid_atendente;
    printf("Digite o PID do atendente para ligar: ");
    scanf("%d", &pid_atendente);
    kill(pid_atendente, SIGUSR1);
    pause();

    tempo_chamada = ((t_end.tv_sec - t_start.tv_sec) + (t_end.tv_usec - t_start.tv_usec)) / 1000000.0;

    printf("Tempo chamada: %f\n", tempo_chamada);
    if (tempo_chamada < 60.0)
        preco_chamada = tempo_chamada * 2.00 / 60.00;
    else
    {
        tempo_chamada -= 60;
        preco_chamada = 2.00 + tempo_chamada * 1.00 / 60.00;
    }
    printf("Tempo chamada: %f\n", tempo_chamada);
    printf("Preco total da chamada = R$%.2f\n", preco_chamada);

    return 0;
}
