#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>

//Numero de threads(global)
int nThreads;

//Numero de iteraçoes(global)
int nIteracoes;

//declaracao do vetor de inteiros(global)
int *vetor;

long int *valoresEncontrados;

//contador de threads bloqueadas(global)
int bloqueadas=0;

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void sincronizaThread()
{
    pthread_mutex_lock(&x_mutex);
    if(bloqueadas==nThreads-1)
    {
        pthread_cond_broadcast(&x_cond);
        bloqueadas=0;
        pthread_mutex_unlock(&x_mutex);
    }
    else
    {
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);
        pthread_mutex_unlock(&x_mutex);
    }
    return;
}

void * funcaoMultithread(void * arg)
{
    int ident = *(int *) arg;
    long int somatorio=0;
    
    //as N iterações
    for(int k=0; k<nIteracoes;k++)
    {   
        printf("vetor[%d]: %d\n", ident-1, vetor[ident-1]);
        sincronizaThread();
        //somatorio de valores no vetor
        for(int i=0; i<nThreads;i++)
        {
            somatorio+=vetor[i];
            printf("Thread numero %d somatorio = %ld\n", ident, somatorio);
        }
        //espera as outras threads terminarem
        sincronizaThread();
        
        //atribui novo valor ao id no vetor
        vetor[ident-1]= rand()%10;
        
        //espera todas as threads atribuirem no vetor
        sincronizaThread();
    }
    
    pthread_exit((void *)somatorio);
}

int main(int argc, char *argv[])
{
    //int nThreads;
    
    if(argc != 3)
    {
        printf("Rode o programa da seguinte maneira:\n\n./nomedoPrograma numThreads numIteracoes\n\n");
        return 0;
    }

    srand((unsigned)time(NULL));
    
    //entrada do usuario de quantos threads serao criados e quantas iteraçoes existirao
    nThreads = atoi(argv[1]);
    nIteracoes = atoi(argv[2]);

    pthread_t tid[nThreads];
    int id[nThreads];
    long int status=0;

    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    //inicializacao do vetor com o msm tamanho de threads
    vetor=malloc(nThreads*sizeof(int));
    valoresEncontrados=malloc(nThreads*sizeof(long int));
    //inicializa o vetor de nThreads posicoes com valores aleatorios de 0-9
    for(int i=0; i<nThreads;i++)
    {
        vetor[i]=rand()%10;
    }
    
    //cria as threads
    for(int i=0; i<nThreads;i++)
    {
        
        id[i]=i+1;
        pthread_create(&tid[i], NULL, funcaoMultithread, (void *)&id[i]);
        //printf("%d\n", vetor[i]);
    }
    for(int i=0; i<nThreads; i++)
    {
        if(pthread_join(tid[i], (void **) &status))
            printf("Erro: pthread create - tid[%d] = %lu", i, tid[i]);
        valoresEncontrados[i]=status;
    }
    for(int i=0; i<nThreads;i++)
    {
        printf("%d - Thread: %ld\n", i+1, valoresEncontrados[i]);
        if(valoresEncontrados[i+1]!=valoresEncontrados[i] && i>0 && i<3)
        {
            printf("Os valores encontrados nao sao iguais\n");
            return 0;
        }
    }
    printf("Os valores sao iguais\n");
    return 0;
}
