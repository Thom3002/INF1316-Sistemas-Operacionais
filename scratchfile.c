#include <stdio.h>
typedef struct process
{
    char name[10]; // Nome do Programa
    int index;     // Numero do processo
    int init;      // Inicio (tempo)
    int duration;  // tempo de duracao
    int last;      // é o último (0 = false | 1 = true)
} Process;