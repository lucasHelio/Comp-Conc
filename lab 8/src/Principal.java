class LE{
    private int leit, escr;
    public int variavel;

    //construtor
    LE(){//aqui talvez
        this.leit=0;//leitores lendo (0 ou mais)
        this.escr=0;//escritor escrevendo (0 ou 1)
        //this.leitescr=0;
        this.variavel=0;//temos q passar o valor pra ela
    }

    //entrada para leitores
    public synchronized void EntraLeitor(int id){
        try{
            while(this.escr >0){
                System.out.println("le.leitorBloqueado("+id+")");
                wait();//bloqueia pela condição logica da aplicação
            }
            this.leit++;//registra mais um leitor lendo
            System.out.println("le.leitorLendo("+id+")");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    //saida para leitores
    public synchronized void SaiLeitor(int id){
        this.leit--; // registra q um leitor saiu
        if(this.leit == 0)
            this.notify();//libera escritor (caso exista escritor bloqueado)
        System.out.println("le.leitorSaindo("+id+")");
    }


    //Entrada para escritores
    public synchronized void EntraEscritor(int id){
        try{
            while((this.leit>0)|| (this.escr>0)){
                //if((this.leit > 0)|| (this.escr>0)){
                System.out.println("le.escritorBloqueado("+id+")");
                wait();//bloqueia condição lógica da aplicação
            }
            this.escr++;//registra que há um escritor escrevendo
            System.out.println("le.escritorEscrevendo("+id+")");

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }


    //saida para escritores
    public synchronized void SaiEscritor(int id){
        this.escr--;//registra que o escritor saiu
        notifyAll();//libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        System.out.println("le.escritorSaindo("+id+")");
    }
}

class Leitor extends Thread{
    int id;//identificador da Thread
    int delay;//atraso bobo
    LE monitor;//objeto monitor para coordenar a lógica de execução das threads

    //Construtor
    Leitor(int id, int delayTime, LE m){
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    public void run(){
        try{
            for(;;){
                this.monitor.EntraLeitor(this.id);
                /*for(i =0; i< 100000000; i++){//loop bobo para simbolizar tempo de leitura
                    j=j/2;
                }*/

                System.out.println("Variável: "+monitor.variavel);
                if(monitor.variavel%2==0)System.out.println("Valor Par\n");
                else System.out.println("Valor Impar\n");

                this.monitor.SaiLeitor(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}


// ESCRITOR
class Escritor extends Thread{
    int id;//identificador da Thread
    int delay;//atraso bobo
    LE monitor;//objeto monitor para coordenar a lógica de execução das threads

    //Construtor
    Escritor(int id, int delayTime, LE m){
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    public void run(){
        try{
            for(;;){
                this.monitor.EntraEscritor(this.id);
                /*for(i =0; i< 100000000; i++){//loop bobo para simbolizar tempo de leitura
                    j=j/2;
                }*/
                monitor.variavel=id;//modificando o valor da variavel com o id da thread
                this.monitor.SaiEscritor(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

//leitor e escritor
class LEESCREVE extends Thread{
    int id;//identificador da Thread
    int delay;//atraso bobo
    LE monitor;//objeto monitor para coordenar a lógica de execução das threads

    public LEESCREVE(int id, int delayTime, LE m){
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    public void run(){ // thread que le e escreve
        double j=777777777.7, i;
        try{
            for(;;){
                this.monitor.EntraEscritor(this.id);//trava o fluxo do programa para somente esse run ser executado
                //this.monitor.EntraLeitor(this.id);
                System.out.println("Variável: "+monitor.variavel);

                for(i =0; i< 100000000; i++){//processamento bobo qualquer
                    j=j/2;
                }
                //this.monitor.SaiLeitor(this.id);

                //this.monitor.EntraEscritor(this.id);
                monitor.variavel++;//modificando o valor da variavel com o id da thread

                this.monitor.SaiEscritor(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}


public class Principal {
    //numero de threads de executando cada classe
    static final int L = 1;
    static final int E = 3;
    static final int LE = 1;

    public static void main(String[] args){
        int i;

        LE monitor = new LE();
        Leitor[] l = new Leitor[L];
        Escritor[] e = new Escritor[E];
        LEESCREVE[] leitescr = new LEESCREVE[LE];

        //inicia o log de saida
        System.out.println("import verificaLE");
        System.out.println("le = verificaLE.LE()");

        for(i =0; i<L; i++){
            l[i]= new Leitor(i+1, (i+1)*500, monitor);
            l[i].start();
        }
        for(i=0; i<E; i++){
            e[i]= new Escritor(i+1, (i+1)*500, monitor);
            e[i].start();
        }
        for(i=0; i<LE;i++){
            leitescr[i]= new LEESCREVE(i+1, (i+1)*500, monitor);
            leitescr[i].start();
        }
    }
}
