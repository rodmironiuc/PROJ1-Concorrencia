#define _GNU_SOURCE
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <time.h>

// 64kB stack
#define FIBER_STACK 1024*64
struct c {
    int saldo;
};

typedef struct c conta;
conta from, to;
int valor;

// The child thread will execute this function
int transferencia(void *arg)
{
    int *c1 = &from.saldo;
    int *c2 = &to.saldo;
    int *ptr = (int*)arg;

    while(1){
        if(*ptr == 0){
            break;
        }
    }
    *ptr = 1;

    int aleatorio = rand() % 2;

    if(aleatorio == 1){
        c1 = &to.saldo;
        c2 = &from.saldo;
        printf("\nTransferindo 10 da conta 2 para a conta 1!\n");
    }
    else{
        printf("\nTransferindo 10 da conta 1 para a conta 2!\n");
    }

    if(*c1 == 0){
        printf("\nTransferencia nao pode ser realizada!\n");
        *ptr = 0;
        return 0;
    }
    if (*c1 >= valor){ // 2
        *c1 -= valor;
        *c2 += valor;
        *ptr = 0;
    }

    printf("Transferência concluída com sucesso!\n");
    printf("Saldo de c1: %d\n", from.saldo);
    printf("Saldo de c2: %d\n", to.saldo);
    *ptr = 0;
    return 0;

}
int main()
{
    srand(time(NULL));
    void* stack;
    pid_t pid;
    int i;
// Allocate the stack
    stack = malloc( FIBER_STACK );
    if ( stack == 0 )
    {
        perror("malloc: could not allocate stack");
        exit(1);
    }

    printf("Insira o saldo da conta 1: ");
    scanf("%d",&from.saldo);
    printf("Insira o saldo da conta 2: ");
    scanf("%d",&to.saldo);
    valor = 10;

    int semaforo = 0;

    for (i = 0; i < 100; i++){
// Call the clone system call to create the child thread

        pid = clone( &transferencia, (char*) stack + FIBER_STACK,
                     SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &semaforo);
        if ( pid == -1 )
        {
            perror( "clone" );
            exit(2);
        }

        int status;
        while ((pid = waitpid(-1, &status, 0)) != -1) {
        }
    }
// Free the stack
    free( stack );
    printf("Transferências concluídas e memória liberada.\n");
    return 0;
}