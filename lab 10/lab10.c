#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

#define N 5 //tamanho do buffer
//#define P 2 //qtde de threads produtoras
//#define C 4 //qtde de threads consumidoras

int P; //qtde de threads produtoras
int C; //qtde de threads consumidoras

//variaveis do problema
int Buffer[N]; //espaco de dados compartilhados
int out=0; //variaveis de estado

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_cons, cond_prod;

//variaveis globais
sem_t slotCheio, slotVazio; //condição
sem_t mutexProd, mutexCons; //exclusao mutua

//inicializa o buffer
void IniciaBuffer(int n) {
  int i;
  for(i=0; i<n; i++)
    Buffer[i] = 0;
}

//imprime o buffer
void ImprimeBuffer(int n) {
  int i;
  for(i=0; i<n; i++)
    printf("%d ", Buffer[i]);
  printf("\n");
}

//insere um elemento no Buffer ou bloqueia a thread caso o Buffer esteja cheio
void Insere (int item, int id, int tid) {  
    
    sem_wait(&mutexProd);
    Buffer[id] = item;
    
    sem_post(&slotCheio);
    sem_post(&mutexProd);
    

    printf("P[%d] inseriu\n", tid);
    ImprimeBuffer(N);
    
    
}

//retira um elemento no Buffer ou bloqueia a thread caso o Buffer esteja vazio
int Retira (int id) {
    
    int item;

    sem_wait(&mutexCons);
    sem_wait(&slotCheio);
    

    printf("C[%d] quer consumir\n", id);
    
    
    item = Buffer[out];
    Buffer[out] = 0;
        
    printf("C[%d] consumiu %d\n", id, item);
    ImprimeBuffer(N);
    
    out++;
    if(out!=N)
        sem_post(&mutexCons);

    return item;
}

//thread produtora
void * produtor(void * arg) {
    int *id = (int *) arg;
    printf("Sou a thread produtora %d\n", *id);
    while(1) {
        

        sem_wait(&slotVazio);
        for(int i=0; i<N; i++){
            printf("P[%d] quer inserir\n", *id);

            Insere(*id, i, *id);
        }
        printf("Slot cheio\n");
        sem_post(&mutexCons);
        
        sleep(1);
    } 
    free(arg);
    pthread_exit(NULL);
}

//thread consumidora
void * consumidor(void * arg) {
    int *id = (int *) arg;
    int item;



    printf("Sou a thread consumidora %d\n", *id);
    while(1) {
        
        item = Retira(*id);
        if(out == N){ //Estou enviando um sinal para produtora
            out=0;
            sem_post(&slotVazio);
        }
        sleep(1); //faz o processamento do item 
        
    } 
    free(arg);
    pthread_exit(NULL);
}


//funcao principal
int main(int argc, char *argv[]) {

    if(argc!=3)
    {
        printf("digite:\n\n");
        printf("./nomeDoPrograma nProdutores nConsumidores\n\n");
        return 0;
    }

    P = atoi(argv[1]);
    C = atoi(argv[2]);

    sem_init(&mutexCons, 0, 0);
    sem_init(&mutexProd, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, 1);

    //variaveis auxiliares
    int i;
 
    //identificadores das threads
    pthread_t tid[P+C];
    int *id[P+C];

    //aloca espaco para os IDs das threads
    for(i=0; i<P+C;i++) {
        id[i] = malloc(sizeof(int));
        if(id[i] == NULL) exit(-1);
        *id[i] = i+1;
    }

    //inicializa o Buffer
    IniciaBuffer(N);  


    //cria as threads produtoras
    for(i=0; i<P; i++) {
        if(pthread_create(&tid[i], NULL, produtor, (void *) id[i])) 
            exit(-1);
    } 
  
    //cria as threads consumidoras
    for(i=0; i<C; i++) {
        if(pthread_create(&tid[i+P], NULL, consumidor, (void *) id[i+P])) 
            exit(-1);
    } 

    pthread_exit(NULL);
    return 1;
}
