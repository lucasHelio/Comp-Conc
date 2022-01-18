/*class Compartilha{
    private int vetor[];
    public S(){
        this.vetor=0;
    }

}*/
//class compartilhada
class S{
    private int[] vetor;
    private int somaTotal;
    //private int Threads;
    private int tamVetor;

    public S(int tamanho){
        this.vetor = new int[tamanho];
        this.tamVetor = tamanho;
        this.somaTotal=0;

        for(int i =0; i<tamanho;i++){
            this.vetor[i]=i+1;
        }
    }

    public synchronized int sum(int i){
        return this.vetor[i];
    }

    public synchronized void incSoma(int i){ this.somaTotal+= this.vetor[i];}

    public synchronized int getSomaTotal(){
        return this.somaTotal;
    }

    //public int getThreads(){return this.Threads;}

    public int getTamVetor(){return this.tamVetor;}
}

class Soma extends Thread{
    private int id;
    //o construtor receberia o vetor de inteiros?
    S s;
    int Thread;


    public  Soma(int tid, S s, int Thread){
        this.id = tid;
        this.s=s;
        this.Thread= Thread;

    }

    public void run(){

        if(id == 0) {
            for (int i = 0; i < s.getTamVetor()/this.Thread; i++) {
                System.out.println("valor a ser somado: " + s.sum(i));
                s.incSoma(i);
            }
        }
        else{
            for(int i=(id-1)*(s.getTamVetor()/this.Thread); i<id*(s.getTamVetor()/this.Thread); i++){
                System.out.println("valor a ser somado: " + s.sum(i));
                s.incSoma(i);
            }
        }
    }
    
}

class Principal{
    static final int N = 10;
    static int tamanhoVetor = 10;
    public static void main(String[] args){

        //int vetor[]= new int[10];
        S s = new S(tamanhoVetor);


        //reserva espaçio para um vetor de threads
        Thread[] threads = new Thread[N];

        
        //runnable em thread
        for(int i=0;i<N;i++){
            threads[i] = new Soma(i+1, s, N);
        }

        //iniciar a thread
        for(int i=0; i<N;i++){
            threads[i].start();
        }

        //esperar o termino das threads
        for(int i=0; i<N;i++){
            try{ threads[i].join();}
            catch (InterruptedException e) {return; }
        }
        System.out.println("Soma: "+ s.getSomaTotal());

        System.out.println("Terminado");
    }
}