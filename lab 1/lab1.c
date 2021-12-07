
//atividade 5
/**
 * Aluno: Lucas Hélio Silva Ramos
 * 
 * 
 * Disciplina: Computação concorrente
 * Prof: Silvana Rossetto
 * 
 * 
 * Objetivo: Implementar um programa concorrente, com 
 * duas threads (alem da thread principal), para elevar 
 * ao quadrado cada elemento de um vetor de 10000 
 * elementos. (Para cada elemento (ai) do vetor, 
 * calcular (ai)^2 e escrever o resultado na mesma 
 * posição do elemento.)
 */

#include<stdio.h>
#include <pthread.h>
#include <math.h>

#define NTHREADS 2
#define TAMANHOVETOR 10000

int vetor[TAMANHOVETOR];

//função que eleva ao quadrado
void * expoente2(void * arg){
    int ident = *(int *)arg;

    if(ident == 1)
    {
        for(int i =0; i<TAMANHOVETOR/2;i++)
            vetor[i]=(i+1)*(i+1);
    }
    else
    {
        for(int i= TAMANHOVETOR/2;i<TAMANHOVETOR;i++)
            vetor[i]=(i+1)*(i+1);
    }
    
    pthread_exit(NULL);

}

int main()
{
    pthread_t tid[NTHREADS];
    int ident [NTHREADS]; //identificador local da thread
    
    

    //temos dois threads que fariam a função de elevar 
    for(int i =0;i<NTHREADS;i++)
    {
        ident[i]= i+1;
        pthread_create(&tid[i], NULL, expoente2, (void *)&ident[i]);  
    }

    for(int i =0;i<NTHREADS;i++)
    {
        if(pthread_join(tid[i], NULL))
            printf("Erro: pthread create - tid[%d] = %lu", i, tid[i]);

    }


    

    //imprime o vetor elevado ao quadrado
    for(int i=0; i<TAMANHOVETOR;i++){
        if (vetor[i]!= pow(i+1, 2))
        {
            printf("o numero ta errado\n"); break;
        }
        else if(i== TAMANHOVETOR-1){
            printf("Ta tudo certo\n");
        }
    }


    //pthread_exit(NULL);
    return 0;
}
