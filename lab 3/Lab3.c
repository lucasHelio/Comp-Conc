// Lab 3 Comp.conc

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include"timer.h"


//vetor valores aleatórios
    float *vetor;
    

//valores de limite entrada pelo usuário
    float limiteInferior;
    float limiteSuperior;
    
//contadores de valores encontrados
    long int valoresEncontradosSeq=0;
    long int valoresEncontradosConc=0;

//Numero de threads definido na chamada do programa
    int NTHREADS;
//Tamanho do vetor definido na chamada do programa
    long long int TAMANHO;


//cada thread executa sua propria busca e retorna a soma para a funcao contaConcorrente()
void * multiThread(void * arg)
{
    int idThread = *(int *)arg;
    long int somaLocal=0;
    int razao = TAMANHO/NTHREADS;

    if(idThread == 1)
    {
        for(int i=0;i<razao;i++)
        {
            if(vetor[i]>limiteInferior && vetor[i]<limiteSuperior)
            {
                somaLocal++;
            }
        }
        pthread_exit((void *)somaLocal);
    }
    else
    {
        int limite = idThread*razao;
        for(int i=(idThread-1)*razao; i<limite;i++)
        {
            if(vetor[i]>limiteInferior && vetor[i]<limiteSuperior)
            {   
                somaLocal++;
            }
        }
        pthread_exit((void *)somaLocal);
    }
}

//executa a busca de valores de forma sequencial
void contaSequencial()
{
    for(int i=0; i<TAMANHO; i++)
    {
        if(vetor[i]>limiteInferior && vetor[i]<limiteSuperior)
        {   
            valoresEncontradosSeq++;
        }
    }
}

//chama as threads e soma os resultados encontrados
void contaConcorrente()
{
    pthread_t tid[NTHREADS];
    int ident[NTHREADS];
    long int status=0;

    for(int i=0; i<NTHREADS;i++)
    {
        ident[i]=i+1;
        pthread_create(&tid[i], NULL, multiThread, (void *)&ident[i]);
    }
    for(int i=0; i<NTHREADS;i++)
    {
        if(pthread_join(tid[i], (void **) &status))
            printf("Erro: pthread create - tid[%d] = %lu", i, tid[i]);
        valoresEncontradosConc+=status;
    }
    return;
}

//Funcao que compara resultados obtidos em ambos os metodos
void comparaSolucao()
{
    if (valoresEncontradosSeq!=valoresEncontradosConc)
    {
        printf("Existe uma divergencia nos resultados encontrados\n"
        "valores encontrados sequencialmente: %ld\n"
        "valores encontrados concorrentemente: %ld\n\n", 
        valoresEncontradosSeq, valoresEncontradosConc);
    }
    else
        printf("Resultados entre ambos métodos foram iguais\n");
    return;
}

int main(int argc, char *argv[])
{
    //variavel para armazenar o tempo
    double start, finish, tempo_concorrente, tempo_sequencial;

    //seed para ter números "aleatórios" sempre que rodamos o programa
    srand((unsigned)time(NULL));
    
    //ordem de argumentos: ./nomeDoPrograma TAMANHO NTHREADS
    TAMANHO = atoll(argv[1]);
    NTHREADS = atoi(argv[2]);
    

    //inicialização do vetor
    vetor = malloc(TAMANHO * sizeof(float));
    for(int i=0; i<TAMANHO;i++)
    {
        vetor[i]=rand()%100 + (rand()%101)/100.0f;
    }
    

    //entrada do usuário
    printf("Insira o limite Inferior: \n");
    scanf("%f", &limiteInferior);
    printf("Insira o limite Superior: \n");
    scanf("%f", &limiteSuperior);

    GET_TIME(start);
    contaSequencial();
    GET_TIME(finish);
    tempo_sequencial = finish-start;


    GET_TIME(start);
    contaConcorrente();
    GET_TIME(finish);
    tempo_concorrente = finish-start;

    printf("\ntempo de execução sequencial: %e\nvalores encontrados no intervalo: %ld\n\n", tempo_sequencial, valoresEncontradosSeq);
    printf("tempo de execução Concorrente: %e\nvalores encontrados no intervalo: %ld\n\n", tempo_concorrente, valoresEncontradosConc);
    
    comparaSolucao();

    free(vetor);

    return 0;
}