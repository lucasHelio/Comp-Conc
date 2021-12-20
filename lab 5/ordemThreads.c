#include<stdio.h>
#include<pthread.h>

#define NTHREADS 5

//variavel global de estado das condições lógicas
int contador =0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;



void * Multithread(void * arg)
{
    int numThread = *(int *)arg;
    
    
    if (numThread==5)
    {
         
        printf("Seja bem-vindo!\n");
        contador++;
        pthread_cond_broadcast(&x_cond);
        
    }
    
    
    else
    {
        pthread_mutex_lock(&x_mutex);
        if(contador ==0)
        {
            pthread_cond_wait(&x_cond, &x_mutex);
            //agr vamos continuar
            pthread_mutex_unlock(&x_mutex);
        }
        pthread_mutex_unlock(&x_mutex);
        
        pthread_mutex_lock(&x_mutex);
        
        if(numThread == 2)
        {
            //pthread_mutex_lock(&x_mutex);
            printf("Fique a vontade.\n");
            contador++;
            //pthread_mutex_unlock(&x_mutex);
            //pthread_exit(NULL);
        }
        pthread_mutex_unlock(&x_mutex);

        pthread_mutex_lock(&x_mutex);
        if(numThread == 3)
        {
            //pthread_mutex_lock(&x_mutex);
            printf("Sente-se por favor.\n");
            contador++;
            //pthread_mutex_unlock(&x_mutex);
            //pthread_exit(NULL);
        }
        pthread_mutex_unlock(&x_mutex);


        pthread_mutex_lock(&x_mutex);
        if(numThread == 4)
        {
            //pthread_mutex_lock(&x_mutex);
            printf("Aceita um copo d'água?\n");
            contador++;
            //pthread_mutex_unlock(&x_mutex);
            //pthread_exit(NULL);
        }
        pthread_mutex_unlock(&x_mutex);
        
        pthread_mutex_lock(&x_mutex);    
        if (contador<4){
            //pthread_mutex_lock(&x_mutex);
            pthread_cond_wait(&x_cond, &x_mutex);
            pthread_mutex_unlock(&x_mutex);
        }
        pthread_mutex_unlock(&x_mutex);

        pthread_mutex_lock(&x_mutex);
        if(contador == 4)
        {
            //pthread_cond_signal(&x_cond);
            pthread_cond_broadcast(&x_cond);
            pthread_mutex_unlock(&x_mutex);
            //ativa a thread 1 para ser ultima a printar
        }
        pthread_mutex_unlock(&x_mutex);
        
        

        if(numThread==1 && contador == 4)
        {
            printf("Volte sempre!\n");
            contador++;
        }
        
    }
    
    pthread_exit(NULL);
}


int main()
{
    pthread_t tid[NTHREADS];
    int ident[NTHREADS];

    //inicializa o mutex e a condição
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);
    

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
