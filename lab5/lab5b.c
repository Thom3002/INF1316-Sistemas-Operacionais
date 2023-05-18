#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/ipc.h>

#define BUFFER_SIZE 8
#define NUM_MSG 32

void imprime(int *vet, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        printf("Mensagem %d", vet[i]);
    }
}

int main(void)
{

    int segmento_pai, segmento_filho;
    pid_t pid;

    // aloca a memoria compartilhada
    segmento_pai = shmget(IPC_PRIVATE, BUFFER_SIZE * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    // semaphore mutex[] = {1, 0};

    sem_t *semaforo1, *semaforo2;
    // cria semaforos
    semaforo1 = sem_open("semaforo1_sem", O_CREAT, 0666, 8);
    semaforo2 = sem_open("semaforo2_sem", O_CREAT, 0666, 0);

    // Cria vetor com numero maximo de mensagens
    int vetMensagens[NUM_MSG];
    int *mensagensBuffer;

    mensagensBuffer = (int *)malloc(sizeof(int) * BUFFER_SIZE);
    for (int i = 0; i < NUM_MSG; i++)
    {
        vetMensagens[i] = i;
    }
    pid = fork();

    if (pid == 1) // processo pai
    {
        segmento_pai = shmget(IPC_PRIVATE, BUFFER_SIZE * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); // cria memoria compartilhada
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            mensagensBuffer[i] = vetMensagens[i]; // Escreve 8 mensagens no vetor mensagensBuffer
        }

        while (1)
        {
            {
                for (int i = 0; i < BUFFER_SIZE; i++)
                {

                    // sem_wait(semaforo1);
                    mensagensBuffer[i] = (int *)shmat(segmento_pai, 0, 0);
                }
                // sem_post(semaforo2);
            }
        }
    }
    else if (pid == 0)
    {
        int *mensagensBufferFilho;
        mensagensBufferFilho = (int *)malloc(sizeof(int) * BUFFER_SIZE);

        segmento_filho = shmget(IPC_PRIVATE, BUFFER_SIZE * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        { // processo filho
            while (1)
            {
                for (int i = 0; i < BUFFER_SIZE; i++)
                {
                    mensagensBufferFilho[i] = (int *)shmat(segmento_filho, 0, 0);
                }

                // sem_wait(semaforo2);
                imprime(mensagensBuffer, BUFFER_SIZE);
            }
        }
    }
    else
    {
        puts("Erro na criacao do novo processo");
        exit(-2);
    }

    // shmctl(segmento, IPC_RMID, 0);
    sem_unlink(semaforo1);
    sem_unlink(semaforo2);

    return 0;
}
