#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS  5

// Variaveis globais
int x = 0;    //variavel compartilhada entre as threads
sem_t sg1, sg2;     // semaforo para exclusao mutua entre as threads


void printaMensagem(int id)
{
    if(id==1)
        printf("Volte sempre!\n");
    else if(id==2)
        printf("Fique a vontade\n");
    else if(id==3)
        printf("Sente-se por favor\n");
    else if(id==4)
        printf("Aceita um copo d'agua?\n");
    else if(id==5)
        printf("Seja bem-vindo!\n");
    return;
}



//funcao executada pelas threads
void *ExecutaTarefa (void *threadid) {
  int *tid = (int*) threadid;
  //printf("Thread : %d esta executando...\n", *tid);
  if(*tid!=5 && *tid!=1) 
  {
    sem_wait(&sg1); //entrada na secao critica
    printaMensagem(*tid);  
    
    x++; //secao critica

    if(x==3)
        sem_post(&sg2);//libera thread 1

    sem_post(&sg1); //saida da secao critica
  }
  else if(*tid ==5)//thread 5 printa seua mensagem
  {
    printaMensagem(*tid);
    sem_post(&sg1);
  }
  else{  
    sem_wait(&sg2);
    printaMensagem(*tid);
    sem_post(&sg2);
  }
  //printf("Thread : %d terminou!\n", *tid);
  free(threadid);
  pthread_exit(NULL);
}



//funcao principal
int main(int argc, char *argv[]) {
  pthread_t tid[NTHREADS];
  int t, *id; 
  
  //inicia o semaforo binario
  sem_init(&sg1, 0, 0);
  sem_init(&sg2, 0, 0);

  for(t=0; t<NTHREADS; t++) {
    //aloca espaco para o identificar da thread
    if ((id = malloc(sizeof(int))) == NULL) 
    {
       pthread_exit(NULL); 
       return 1;
    }
    
    //id da thread
    *id=t+1; 
    if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)id)) 
    {
      printf("--ERRO: pthread_create()\n"); 
      exit(-1);
    }
  }
  //--espera todas as threads terminarem
  for (t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) 
    {
        printf("--ERRO: pthread_join() \n"); 
        exit(-1); 
    } 
  } 
  
  return 0;
}
