#include<stdio.h>
#include<pthread.h>

#define NTHREADS 5

int contador =0;

void * Multithread(void * arg)
{
    int numThread = *(int *)arg;
    if (numThread==5 && contador== 0)
    {
        printf("Seja bem-vindo!\n");
        contador++;
    }
    else if(contador == 4)
    {
        printf("Volte sempre!\n");
        contador++;
    }
    else
    {
        if(numThread == 2 && contador !=0)
        {
            printf("Fique a vontade.\n");
            contador++;
        }
        if(numThread == 3 && contador !=0)
        {
            printf("Sente-se por favor.\n");
            contador++;
        }
        if(numThread == 4 && contador !=0)
        {
            printf("Aceita um copo d'água?\n");
            contador++;
        }
    }
    pthread_exit(NULL);
}


int main()
{
    pthread_t tid[NTHREADS];
    int ident[NTHREADS];
    for(int i=0; i<NTHREADS; i++)
    {
        ident[i] = i+1;
        pthread_create(&tid[i], NULL, Multithread, (void *)&ident[i]);
    }

    for(int i=0; i<NTHREADS; i++)
    {
        if(pthread_join(tid[i], NULL))
            printf("Erro: pthread create - tid[%d] = %lu", i, tid[i]);
    }



    return 0;
}