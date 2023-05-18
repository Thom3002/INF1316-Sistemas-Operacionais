#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

#define MAX_TIME 66
struct timeval t_start, t_end;
int tempo_decorrido = 0;
double tempo_chamada;
float preco_chamada;

void handler(int signal);

int main(){

  pid_t pid;
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);
  printf("Chamador: Iniciando chamada...\n");

  pid = fork();
  if (pid < 0)
  {
    perror("Erro ao criar processo atendente");
    exit(1);
  }
  else if (pid == 0)
  { // Processo atendente
    execl("./atendente", "atendente", NULL);
    perror("Erro ao executar atendente");
    exit(1);
  }
  else
  { // Processo chamador

    kill(pid, SIGUSR1);
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

    kill(pid, SIGTERM);
  }
  return 0;
}

void handler(int signal)
{
  if (signal == SIGUSR1)
  {
    printf("Atendente: Chamada recebida.\n");
    gettimeofday(&t_start, NULL);
  }

  else if (signal == SIGUSR2)
  {
    gettimeofday(&t_end, NULL);
    printf("Atendente: Chamada encerrada pelo cliente.\n");
    exit(0);
  }
}
