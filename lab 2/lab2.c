#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"
//#include <conio.h>

//declaração de matrizes

int dimensao;
int threads;
int **matriz_1;
int **matriz_2;
int **matriz_saida_sequencial;
int **matriz_saida_concorrente;


void * multiThread(void * arg)
{
    int idThread = *(int *)arg;

    //resolver isso aqui
    int razao = dimensao/threads;

    //erro
    if(idThread==1)
    {
        for(int i = 0;i<razao;i++)
        {
            for(int j=0; j<dimensao;j++)
            {
                matriz_saida_concorrente[i][j]=matriz_1[i][j]*matriz_2[i][j];
            }

        }
    }

    else
    {
        int limite = idThread*razao;
        for(int i = (idThread-1)*razao;i<limite;i++)
        {
            for(int j=0; j<dimensao;j++)
            {
                matriz_saida_concorrente[i][j]=matriz_1[i][j]*matriz_2[i][j];
            }

        }
    }
    

    pthread_exit(NULL);
}


void comparaResultados(int **mat1, int **mat2)
{
    for(int i=0; i< dimensao;i++)
    {
        for(int j=0; j<dimensao;j++)
        {
            if(mat1[i][j]!=mat2[i][j])
            {
                printf("Os resultados nao batem... \nmatriz1[%d][%d]=%d  matriz2[%d][%d]=%d\n", i, j, mat1[i][j], i, j, mat2[i][j]);
                return;
            }
        }
    }
}


//funcao que as threads executarão
void multiplicaConcorrente(int **matriz1, int **matriz2)
{
    pthread_t tid[threads];
    int ident[threads];

    for(int i=0;i<threads;i++)
    {
        ident[i]= i+1;
        pthread_create(&tid[i], NULL, multiThread, (void *)&ident[i]);
    }
    for(int i =0; i<threads;i++)
    {
        if(pthread_join(tid[i], NULL))
        printf("Erro:pthread create - tid[%d] = %lu", i, tid[i]);
    }
    return;
}

//funcao que faz a multiplicação sequencial
void multiplicaSequencial(int **matriz1, int **matriz2)
{
    for(int i=0;i<dimensao;i++)
    {
        for(int j=0;j<dimensao;j++)
        {
            matriz_saida_sequencial[i][j] = matriz1[i][j]*matriz2[i][j];
        }
    }
}



//espera os threads como primeiro argumento e a dimensão como segundo
int main(int argc, char *argv[])
{
    //variavel para armazenar o tempo
    double start, finish, tempo_concorrente, tempo_sequencial;


    //seed para ter números aleatórios sempre que rodamos o programa
    srand((unsigned)time(NULL));

    dimensao = atoi(argv[1]);
    threads = atoi(argv[2]);

    //inicialização de matrizes
    matriz_1 = malloc(dimensao * sizeof(int *));
    matriz_2 = malloc(dimensao * sizeof(int *));
    matriz_saida_sequencial = malloc(dimensao * sizeof(int *));
    matriz_saida_concorrente = malloc(dimensao * sizeof(int *));
    
    for(int i=0;i<dimensao;i++)
    {
        matriz_1[i]=malloc(dimensao*sizeof(int));
        matriz_2[i]=malloc(dimensao*sizeof(int));
        matriz_saida_concorrente[i]=malloc(dimensao*sizeof(int));
        matriz_saida_sequencial[i]=malloc(dimensao*sizeof(int));
    }

    //Atribuição aleatória de valores para a matriz de entrada
    for(int i=0;i<dimensao;i++)
    {
        for(int j=0;j<dimensao;j++)
        {
            matriz_1[i][j]= rand()%100;
            matriz_2[i][j]= rand()%100;
        }
    }


    //chamando a funcao multi. sequenc.
    GET_TIME(start);
    multiplicaSequencial(matriz_1, matriz_2);
    GET_TIME(finish);
    tempo_sequencial = finish - start;
    printf("tempo de execução sequencial: %e\n", tempo_sequencial);

    //chamando a funcao multi. conc.
    GET_TIME(start);
    multiplicaConcorrente(matriz_1, matriz_2);
    GET_TIME(finish);
    tempo_concorrente = finish - start;
    printf("tempo de execução concorrente: %e\n", tempo_concorrente);
    
    comparaResultados(matriz_saida_concorrente, matriz_saida_sequencial);

    printf("diferença de tempo de execução (tempo_sequencial/tempo_concorrente): %e\n", tempo_sequencial/tempo_concorrente);


    free(matriz_1);
    free(matriz_2);
    free(matriz_saida_concorrente);
    free(matriz_saida_sequencial);

    return 0;
}
