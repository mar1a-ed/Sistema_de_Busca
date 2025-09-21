#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define T_CHAR 150 //constante para definir o numero maximo de caracteres de uma string qualquer
#define VEZES 100 //quantidade de ordenacoes

//variaveis globais para: vetor original, vetor auxiliar, tamanho do vetor, tempos de execucoes e indicadores de buscas e ordenacoes
int *v_alocar = NULL, carregou, *v_auxiliar = NULL, encontrou_binario, encontrou_linear, fez_linear, fez_binaria;
int tam_alocado = 0;
char nome_arquivo[T_CHAR];
double t_bubble, t_insertion, t_selection, t_merge, t_quick, t_radix, t_busca_linear, t_busca_binaria;
int valor_linear, valor_binaria, indice_linear, indice_binario;

//funcao para o menu principal
void menu();

//funcao para carregar arquivos de dados
void carregar_arquivo();

//funcao que conta a quantidade de valores que há no arquivo antes de alocar o vetor
int contando_linhas(FILE *arquivo);

//funcao para copiar o vetor original para um auxiliar
void vetor_copia(int *primeiro, int *segundo, int tamanho);

//funcao para escolher entre busca linear e binaria
void submenu_busca();

//funcao para realização da busca linear
int busca_linear(int *vetor, int valor, int tamanho, int *posicao);

//funcao para realizacao da busca binaria
int busca_binaria(int *vetor, int valor, int tamanho, int *posicao);

//funcao para escolher qual algoritmo de ordenacao quer realizar
void submenu_ordenar();

//funcao para gerar o relatorio txt
void gerar_relatorio(char *arquivo);

//funcao para verificar se existe vetor para as outras operacoes
int verificar_alocacao(int *vetor, int tamanho);

//funcao para verificar se o vetor esta ordenado antes da busca binaria
int verificar_ordenacao(int *vetor, int tamanho);

//funcao para pausar e limpar o terminal
void limpar_pausar();

//funcao do algoritmo bubble sort
void bubble_sort(int *vetor, int tamanho);

//funcao do algoritmo insertion sort
void insertion_sort(int *vetor, int tamanho);

//funcao do algoritmo selection sort
void selection_sort(int *vetor, int tamanho);

//funcao de mesclação do algoritmo merge sort
void merge(int *vetor, int l, int m, int r);

//funcao do algoritmo merge sort
void merge_sort(int *vetor, int l, int r);

//funcao do algoritmo quick sort
void quick_sort(int *v, int inicio, int fim);

int particiona(int *v, int inicio, int fim);

//funcao para encontrar maior valor
int maior_valor(int *vetor, int tamanho);

//funcao que realiza o counting sort dentro do radix
void counting_sort(int *vetor, int tamanho, int pos_digito);

//funcao do algoritmo radix sort
void radix_sort(int *vetor, int tamanho);

int main(){
    int op1; //opcao do menu principal
    char op2; //opcao para submenu de busca
    char op3; //opcao para decidir o algoritmo de ordenacao
    int buscar; //valor procurado do vetor
    int indice = 0; //referencia a posicao do valor encontrado
    int esta_ordenado; //variavel para verificar se o vetor esta ordenado
    double soma = 0; //soma dos tempos de ordenacao
    LARGE_INTEGER inicio, fim, frequencia; //inicio, fim e frequencia do tempo de ordenacao 

    do{
        menu();
        scanf("%d",&op1);

        switch(op1){
            case 1:
                carregar_arquivo(); //carregando arquivo

                vetor_copia(v_alocar,v_auxiliar,tam_alocado); //copiando o vetor original para o auxiliar para manter o vetor original desordenado

                //indica se fez busca ou nao
                fez_linear = 0; 
                fez_binaria = 0;

                limpar_pausar();
                break;
            
            case 2:
                carregou = verificar_alocacao(v_alocar,tam_alocado); //verificando se tem vetor ou nao

                if(carregou == 2){ //nao há vetor carregado no programa
                    printf("\nNao ha nenhum vetor alocado na memoria. Carregue um arquivo de valores.\n");
                    limpar_pausar();
                    break;
                }

                submenu_busca();
                scanf(" %c",&op2);

                switch(op2){
                    case 'a': //busca linear
                        printf("\nInforme o valor que deseja procurar no vetor: ");
                        scanf("%d",&buscar);
                        valor_linear = buscar;

                        QueryPerformanceFrequency(&frequencia);
                        QueryPerformanceCounter(&inicio);

                        encontrou_linear = busca_linear(v_alocar,buscar,tam_alocado,&indice);
                        fez_linear = 1;

                        QueryPerformanceCounter(&fim);

                        t_busca_linear = ((double)(fim.QuadPart - inicio.QuadPart) * 1000) / (double)(frequencia.QuadPart);

                        if(encontrou_linear == 1){
                            indice_linear = indice;
                        }

                        printf("\nBusca linear feita com sucesso!\n");
                        limpar_pausar();
                        break;
                    
                    case 'b'://busca binaria
                        esta_ordenado = verificar_ordenacao(v_auxiliar,tam_alocado);

                        if(esta_ordenado == 2){
                            printf("\nNao eh possivel realizar busca binaria. Por favor ordene o vetor.\n");
                            limpar_pausar();
                            break;
                        }
                        
                        printf("\nInforme o valor que deseja procurar no vetor: ");
                        scanf("%d",&buscar);
                        valor_binaria = buscar;

                        QueryPerformanceFrequency(&frequencia);
                        QueryPerformanceCounter(&inicio);

                        encontrou_binario = busca_binaria(v_auxiliar,buscar,tam_alocado,&indice);
                        fez_binaria = 1;

                        QueryPerformanceCounter(&fim);

                        t_busca_binaria = ((double)(fim.QuadPart - inicio.QuadPart) * 1000) / (double)(frequencia.QuadPart);

                        if(encontrou_binario == 1){
                            indice_binario = indice;
                        }

                        printf("\nBusca binaria feita com sucesso!\n");
                        limpar_pausar();
                        break;

                    case 'c':
                        limpar_pausar();
                        break;

                }

                break;
            
            case 3:
                carregou = verificar_alocacao(v_alocar,tam_alocado);

                if(carregou == 2){
                    printf("\nNao ha nenhum vetor alocado na memoria. Carregue um arquivo de valores.\n");
                    limpar_pausar();
                    break;
                }

                submenu_ordenar();
                scanf(" %c",&op3);

                switch(op3){
                    case 'a':
                        soma = 0; //inicia a soma das ordenacoes com 0

                        for(int i=0;i<VEZES;i++){ //realiza as ordenacoes 100 vezes para realizar a media de tempo

                            vetor_copia(v_alocar,v_auxiliar,tam_alocado); //copia o vetor para ordenar

                            QueryPerformanceFrequency(&frequencia);
                            QueryPerformanceCounter(&inicio);

                            bubble_sort(v_auxiliar,tam_alocado);

                            QueryPerformanceCounter(&fim);

                            soma += ((double)(fim.QuadPart - inicio.QuadPart) * 1000) / (double)(frequencia.QuadPart); //soma dos tempos de ordenacao
                        }

                        t_bubble = soma / VEZES; //tempo medio de ordenacao

                        printf("\nOrdenacao concluida com sucesso!\n");
                        limpar_pausar();
                        break;

                    case 'b'://insertion sort
                        soma = 0;    

                        for(int i=0;i<VEZES;i++){

                            vetor_copia(v_alocar,v_auxiliar,tam_alocado);

                            QueryPerformanceFrequency(&frequencia);
                            QueryPerformanceCounter(&inicio);

                            insertion_sort(v_auxiliar,tam_alocado);

                            QueryPerformanceCounter(&fim);

                            soma += ((double)(fim.QuadPart - inicio.QuadPart) * 1000) / (double)(frequencia.QuadPart);

                        }

                        t_insertion = soma / VEZES;

                        printf("\nOrdenacao concluida com sucesso!\n");
                        limpar_pausar();
                        break;

                    case 'c'://selection sort
                        soma = 0;
                        
                        for(int i=0;i<VEZES;i++){

                            vetor_copia(v_alocar,v_auxiliar,tam_alocado);

                            QueryPerformanceFrequency(&frequencia);
                            QueryPerformanceCounter(&inicio);

                            selection_sort(v_auxiliar,tam_alocado);

                            QueryPerformanceCounter(&fim);

                            soma += ((double)(fim.QuadPart - inicio.QuadPart) * 1000) / (double)(frequencia.QuadPart);

                        }

                        t_selection = soma / VEZES;

                        printf("\nOrdenacao concluida com sucesso!\n");
                        limpar_pausar();
                        break;

                    case 'd'://merge sort
                        soma = 0;

                        for(int i=0;i<VEZES;i++){

                            vetor_copia(v_alocar,v_auxiliar,tam_alocado);

                            QueryPerformanceFrequency(&frequencia);
                            QueryPerformanceCounter(&inicio);

                            merge_sort(v_auxiliar,0,tam_alocado-1);

                            QueryPerformanceCounter(&fim);

                            soma += ((double)(fim.QuadPart - inicio.QuadPart) * 1000) / (double)(frequencia.QuadPart);

                        }

                        t_merge = soma / VEZES;

                        printf("\nOrdenacao concluida com sucesso!\n");
                        limpar_pausar();
                        break;

                    case 'e'://quick sort
                        soma = 0;    

                        for(int i=0;i<VEZES;i++){

                            vetor_copia(v_alocar,v_auxiliar,tam_alocado);

                            QueryPerformanceFrequency(&frequencia);
                            QueryPerformanceCounter(&inicio);
                            quick_sort(v_auxiliar,0,tam_alocado-1);

                            QueryPerformanceCounter(&fim);

                            soma += ((double)(fim.QuadPart - inicio.QuadPart) * 1000) / (double)(frequencia.QuadPart);

                        }

                        t_quick = soma / VEZES;

                        printf("\nOrdenacao concluida com sucesso!\n");
                        limpar_pausar();
                        break;

                    case 'f'://radix sort
                        soma = 0;

                        for(int i=0;i<VEZES;i++){
                            
                            vetor_copia(v_alocar,v_auxiliar,tam_alocado);

                            QueryPerformanceFrequency(&frequencia);
                            QueryPerformanceCounter(&inicio);
                            radix_sort(v_auxiliar,tam_alocado);

                            QueryPerformanceCounter(&fim);

                            soma += ((double)(fim.QuadPart - inicio.QuadPart) * 1000) / (double)(frequencia.QuadPart);

                        }

                        t_radix = soma / VEZES;

                        printf("\nOrdenacao concluida com sucesso!\n");
                        limpar_pausar();
                        break;

                    case 'g': //saindo do submenu para o principal
                        limpar_pausar();
                        break;

                }
                
                break;

            case 4:
                carregou = verificar_alocacao(v_alocar,tam_alocado);

                if(carregou == 2){
                    printf("\nNao ha nenhum vetor alocado na memoria. Carregue um arquivo de valores.\n");
                    limpar_pausar();
                    break;
                }

                gerar_relatorio(nome_arquivo); //gera o relatorio
                limpar_pausar();
                break;

            case 5:
                printf("\nBYE BYE\n");
                printf("Encerrando programa....\n");
                free(v_alocar);
                free(v_auxiliar);
                break;

            default:
                printf("\nOpcao Invalida!\n");
                limpar_pausar();
                break;
        }

    }while(op1 != 5); //o programa vai continuar funcionando enquanto o usuario nao digitar o valor para encerrar o programa


    return 0;
}

void menu(){

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //api que permite modificar os recursos do windows no programa

    SetConsoleTextAttribute(hConsole,4); //usado para mudar a saida do terminal
    printf("\n================================\n");
    printf("||            MENU            ||");
    printf("\n================================\n");
    SetConsoleTextAttribute(hConsole,14);
    printf("[1] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Carregar Arquivo de Dados.\n");
    SetConsoleTextAttribute(hConsole,5);
    printf("[2] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Buscar Elemento.\n");
    SetConsoleTextAttribute(hConsole,3);
    printf("[3] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Ordenar Vetor.\n");
    SetConsoleTextAttribute(hConsole,4);
    printf("[4] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Gerar Relatorio.\n");
    SetConsoleTextAttribute(hConsole,11);
    printf("[5] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Sair.\n");
    printf("Escolha uma opcao: ");
}

void submenu_busca(){

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole,8);
    printf("\n[a] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Linear.\n");
    SetConsoleTextAttribute(hConsole,9);
    printf("[b] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Binaria.\n");
    SetConsoleTextAttribute(hConsole,10);
    printf("[c] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Voltar ao menu principal.\n");
    printf("Escolha uma opcao: ");
}

void submenu_ordenar(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole,1);
    printf("\n[a] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Bubble Sort.\n");
    SetConsoleTextAttribute(hConsole,2);
    printf("[b] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Insertion Sort.\n");
    SetConsoleTextAttribute(hConsole,3);
    printf("[c] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Selection Sort.\n");
    SetConsoleTextAttribute(hConsole,4);
    printf("[d] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Merge Sort.\n");
    SetConsoleTextAttribute(hConsole,5);
    printf("[e] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Quick Sort.\n");
    SetConsoleTextAttribute(hConsole,6);
    printf("[f] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Radix Sort.\n");
    SetConsoleTextAttribute(hConsole,7);
    printf("[g] - ");
    SetConsoleTextAttribute(hConsole,15);
    printf("Voltar ao menu principal.\n");
    printf("Escolha uma opcao: ");
}

int contando_linhas(FILE *arquivo){
    int linhas = 0; //inicializa com 0 linhas
    char l; //caracter usado como referenciador de parada

    while((l = fgetc(arquivo)) != EOF){
        if(l == '\n'){ 
            linhas++;
        }
    }

    rewind(arquivo);
    return linhas;

}

void carregar_arquivo(){

    printf("\nInforme o nome do arquivo(.txt): ");
    scanf("%s",nome_arquivo);

    FILE *arquivo_carregado; //arquivo

    arquivo_carregado = fopen(nome_arquivo,"r"); //abrindo o arquivo para ler os numeros

    if(arquivo_carregado == NULL){
        printf("\nVetor nao encontrado na pasta corrente. Favor inserir arquivo na pasta!\n");
        return;
    }

    free(v_alocar); //libera a memoria dos vetores original e auxiliar
    free(v_auxiliar);

    tam_alocado = contando_linhas(arquivo_carregado); //quantidade de valores inteiros do arquivo de dados

    if(tam_alocado == 0){
        printf("\nArquivo nao contem nenhum dado. Favor inserir dados no arquivo!\n");
        fclose(arquivo_carregado);
        return;
    }

    //alocando os vetores na memoria
    v_alocar = (int *) calloc (tam_alocado, sizeof(int));
    v_auxiliar = (int *) calloc (tam_alocado, sizeof(int));

    if(v_alocar == NULL){
        printf("\nNao foi possivel alocar memoria.\n");
        fclose(arquivo_carregado);
        return;
    }

    //le os dados do arquivo
    for(int i=0;i<tam_alocado;i++){
        if(fscanf(arquivo_carregado,"%d",&v_alocar[i]) != 1){
            printf("\nNao foi possivel ler o numero do arquivo.\n");
            fclose(arquivo_carregado);
            free(v_alocar);
            v_alocar = NULL;
            tam_alocado = 0;
            return;
        }
    }

    //fecha o arquivo
    fclose(arquivo_carregado);

}

int verificar_alocacao(int *vetor, int tamanho){

    //significa que nao tem nenhum vetor alocado no programa
    if(vetor == NULL && tamanho ==  0){
        return 2;
    }

    return 1;
}

int verificar_ordenacao(int *vetor, int tamanho){
    int i;

    //significa que nao esta ordenado
    for(i=0;i<tamanho-1;i++){
        if(*(vetor + i) > *(vetor + (i + 1))){
            return 2;
        }
    }

    return 1;

}

void vetor_copia(int *primeiro, int *segundo, int tamanho){
    int i;

    //copia um vetor para outro
    for(i=0;i<tamanho;i++){
        *(segundo + i) = *(primeiro + i);
    }

}

int busca_linear(int *vetor, int valor, int tamanho, int *posicao){
    int i;

    //realiza a busca linear
    //percorre o vetor inteiro
    for(i=0;i<tamanho;i++){
        //verifica se o valor procurado é igual a algum valor do vetor
        if(valor == *(vetor + i)){ 
            *posicao = i; //retorna a posicao
            return 1;
        }
    }

    return 2;

}

int busca_binaria(int *vetor, int valor, int tamanho, int *posicao){
    int lim_esq, lim_dir, pivo;

    //primeiro indice inicial do vetor
    lim_esq = 0;

    //ultimo indice inicial do vetor
    lim_dir = tamanho - 1;

    //enquanto o primeiro indice for menor ou igual ao ultimo indice
    while(lim_esq<=lim_dir){

        //o cursor onde o vetor foi dividido, ou seja, onde o cursor fica
        pivo = (lim_esq + lim_dir) / 2;

        //se o valor for igual ao valor do indice do pivo
        if(valor == *(vetor + pivo)){
            *posicao = pivo; //retorna a posicao
            return 1;
        }

        //se o valor procurado for maior que o valor onde o cursor esta
        if(valor > *(vetor + pivo)){
            lim_esq = pivo + 1; //significa que o valor está a frente do cursor, entao o indice inicial deve ser um indice apos o cursor
        }else{
            lim_dir = pivo - 1; //significa que o valor está abaixo do cursor, entao o indice final deve ser um indice antes do cursor
        }

    }

    return 2;

}

void gerar_relatorio(char *arquivo){

    FILE *relatorio; //declara um arquivo

    relatorio = fopen("relatorio_ti_ti.txt","a"); //abre o arquivo

    if(relatorio == NULL){
        printf("\nNao foi possivel gerar relatorio. Tente novamente mais tarde!\n");
        return;
    }

    fprintf(relatorio,"\n======RELATORIO TCD CLAUDINEY======\n");
    fprintf(relatorio,"Nome do arquivo: %s\n",arquivo); //nome do arquivo
    fprintf(relatorio,"--------------------------------\n");
    fprintf(relatorio,"Tempos de Ordenacao:\n");
    fprintf(relatorio,"Bubble Sort: %.10f\n",t_bubble); //tempo de ordenacao bubble
    fprintf(relatorio,"--------------------------------\n");
    fprintf(relatorio,"Insertion Sort: %.10f\n",t_insertion); //tempo de ordenacao insertion
    fprintf(relatorio,"--------------------------------\n");
    fprintf(relatorio,"Selection Sort: %.10f\n",t_selection); //tempo de ordenacao selection
    fprintf(relatorio,"--------------------------------\n");
    fprintf(relatorio,"Merge Sort: %.10f\n",t_merge); //tempo de ordenacao merge
    fprintf(relatorio,"--------------------------------\n");
    fprintf(relatorio,"Quick Sort: %.10f\n",t_quick); //tempo de ordenacao quick
    fprintf(relatorio,"--------------------------------\n");
    fprintf(relatorio,"Radix Sort: %.10f\n",t_radix); //tempo de ordenacao radix
    fprintf(relatorio,"--------------------------------\n");
    fprintf(relatorio,"Buscas Feitas:\n");
    
    if(fez_linear == 1){ //se a busca linear foi feita
            if(encontrou_linear == 1){ //se o valor foi encontrado
                fprintf(relatorio,"Valor %d encontrado na posicao %d em %.10f ms pela busca linear.\n",valor_linear,indice_linear,t_busca_linear);
            }else{
                fprintf(relatorio,"Valor nao encontrado pela busca linear. Tempo de execucao: %.10f\n",t_busca_linear);
            }
    
    }
    
    if(fez_binaria == 1){ //se a busca binaria foi feita
        if(encontrou_binario == 1){ //se o valor foi encontrado
            fprintf(relatorio,"Valor %d encontrado na posicao %d em %.10f ms pela busca binaria.\n",valor_binaria,indice_binario,t_busca_binaria);
        }else{
            fprintf(relatorio,"Valor nao encontrado pela busca binaria. Tempo de execucao: %.10f\n",t_busca_binaria);
        }
    }

    if(fez_linear == 0 && fez_binaria == 0){
        fprintf(relatorio,"Nao foram feitas nenhum tipo de buscar.\n");
    }
    

    fprintf(relatorio,"======RELATORIO TCD CLAUDINEY======\n");
    fprintf(relatorio,"\n");
    fclose(relatorio); //fecha o arquivo

    //indica que o arquivo foi gerado com sucesso e o nome
    printf("\nRelatorio gerado com sucesso. Verifique na pasta corrente em 'output': relatorio_ti_ti.txt\n");

}

void limpar_pausar(){
    //pausa o terminal enquanto o usuario nao apertar enter para continuar
    system("pause"); 

    //limpa o terminal
    system("cls");
}

void bubble_sort(int *vetor, int tamanho){
    int i, aux, temp;

    aux = 1; //flag para verificar se precisa continuar a ordenacao

    while(aux){
        aux = 0;

        //percorre até o penultimo indice do vetor
        for(i=0;i<tamanho-1;i++){
            if(*(vetor + i) > *(vetor + (i + 1))){ //se o valor do indice anterior for maior que o indice posterior fz a troca
                temp = *(vetor + i);
                *(vetor + i) = *(vetor + (i + 1));
                *(vetor + (i + 1)) = temp;
                aux = 1;
            }
        }
    }

}

void insertion_sort(int *vetor, int tamanho){
    int temp, j; 
    for(int i= 1; i < tamanho; i++){
        temp= *(vetor + i);
        j= i - 1;
        while((j >= 0) && (temp < *(vetor + j))){
            *(vetor + (j + 1))= *(vetor + j);
            j--;
        }
        *(vetor + (j + 1))= temp;
    }

}

void selection_sort(int *vetor, int tamanho){

    int temp;
    for(int i = 0; i < tamanho-1; i++){
        int menor= i;
        for(int j= i+1; j < tamanho; j++){
            if(*(vetor+ j) < *(vetor+ menor))
                menor= j;
        }
        if(i != menor){
            temp= *(vetor+ i);
            *(vetor+ i)= *(vetor + menor);
            *(vetor+ menor)= temp;
        }
    }

}

void merge(int *vetor, int l, int m, int r){
    int aux[r - l + 1]; //vetor auxiliar
    int i = l; //apontador para o inicio do vetor
    int j = m + 1; //apontador para o inicio da segunda metade
    int k = 0; //posicao que vamos colocar os valores no vetor auxiliar

    while(i<=m && j<=r){ //enquanto tiver elementos em todas as metades
        if(vetor[i] < vetor[j]){
            aux[k++] = vetor[i++]; //copia o menor elemento pro auxiliar
        }else{
            aux[k++] = vetor[j++];
        }
    }

    //copia o restante dos valores da primeira metade para o vetor auxiliar
    while(i<=m){ //se tiver sobrado valores preenche o restante
        aux[k++] = vetor[i++];
    }

    while(j<=r){
        aux[k++] = vetor[j++];
    }

    //copia o vetor auxiliar para o vetor original
    for(i=l,k=0;i<=r;i++,k++){
        vetor[i] = aux[k];
    }
}

void merge_sort(int *vetor, int l, int r){
    //enquanto o indice da esquerda for menor do que o da direita (ultimo), continua dividindo em metades
    if(l < r){
        int m = (l + r) / 2; //encontra o meio 

        merge_sort(vetor,l,m); //ordena a primeira parte
        merge_sort(vetor,m+1,r); //ordena a segunda parte

        merge(vetor,l,m,r); //mescla as duas partes ordenadas

    }

}

void quick_sort(int *v, int inicio, int fim){
     if(inicio < fim){ //enquanto houver mais de um elemento
        int pos = particiona(v, inicio, fim); //parte o vetor
        quick_sort(v, inicio, pos - 1); //ordena a parte esquerda
        quick_sort(v, pos + 1, fim); //ordena a parte direita
    }
}

int particiona(int *v, int inicio, int fim){
    int pivo = v[(inicio + fim) / 2]; //escolhe o elemento do meio o pivô
    while(inicio <= fim){ 
        while (v[inicio] < pivo) //move o indice da esquerda pra direita enquanto o elemento for menor que o elemento do pivô
            inicio++; 
        while (v[fim] > pivo) //move o indice da direita pra esquerda enquanto o elemento for maio que o elemento do pivo
            fim--;
        if(inicio <= fim){ //se os indices nao se cruzaram, troca os elementos
            int temp = v[inicio];
            v[inicio] = v[fim];
            v[fim] = temp;
            inicio++;
            fim--;
        }
    }
    return inicio; //retorna a posicao pra poder dividir o vetor
}

int maior_valor(int *vetor, int tamanho){
    int i, maior = *(vetor + 0);

    for(i=0;i<tamanho;i++){ //encontra o maior valor do vetor pra ver quantos digitos vai ter que processar
        if(*(vetor + i) > maior){
            maior = *(vetor + i);
        }
    }

    return maior;

}

void counting_sort(int *vetor, int tamanho, int pos_digito){

    int i, num, array_saida[tamanho], contador[10] = {0};

    for(i=0;i<tamanho;i++){
        contador[(*(vetor + i) / pos_digito) % 10]++; //conta a frequencia do digito na posição atual
    }

    for(i=1;i<10;i++){
        contador[i] += contador[i - 1]; //junta as frequencias para contar as posicoes
    }

    for(i=tamanho-1;i>=0;i--){//percorre o vetor de tras pra frente
        num = (*(vetor + i) / pos_digito) % 10; 
        array_saida[contador[num] - 1] = *(vetor + i); //coloca o elemento na posicao correta
        contador[num]--;
    }

    for(i=0;i<tamanho;i++){
        *(vetor + i) = array_saida[i]; //copia o vetor ordenado de volta pro atual
    }

}

void radix_sort(int *vetor, int tamanho){
    int aleat, maior_num;

    maior_num = maior_valor(vetor,tamanho);

    for(aleat=1;maior_num/aleat > 0;aleat*=10){
        counting_sort(vetor,tamanho,aleat); //ordena o vetor pelo digito atual do numero
    }

}