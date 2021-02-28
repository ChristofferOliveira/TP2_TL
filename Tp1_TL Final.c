#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pilha.c"

typedef struct transicoes{
    char caracter[10];
    char estado_seguinte[10];
    char empilha[10];
    char desempilha[10];
}Transicoes;

typedef struct estado{
    char nome[10];
    int id;
    int caracteristica;
    Transicoes transicoes[5];
    int posicao;
}Estado;


int main(int argc, char **argv){
    Estado conj_estados[50];
    int i, j, k, l, m;
    char dados_transicao[5][10];
    int estado_existe = 0, estados_criados = 0,aresta_existe = 0, caracteristica;
    char palavra[50];

    node *PILHA = (node *) malloc(sizeof(node));
    if(!PILHA){
        printf("Sem memoria disponivel!\n");
        exit(1);
    }else{

    //Iniciando a  pilha
    inicia(PILHA);

    }
    //configura��o atual do automato.
    int conf_atual[50];

    //Abrindo arquivos e verificando se foram abertos corretamente.
    FILE *transicoes = fopen("transicoes.txt", "r");
    FILE *estados = fopen("estados.txt", "r");

    if(estados == NULL){
        printf("Erro na leitura do arquivo de estados\n");
        return 0;
    }

    if(transicoes == NULL){
        printf("Erro na leitura do arquivo de transicoes\n");
        return 0;
    }

    //Criando estados.
    for(i = 0; !feof(estados); i++){

        fscanf(estados, "%s %d", conj_estados[i].nome, &conj_estados[i].caracteristica);
        conj_estados[i].id = i;
        conj_estados[i].posicao = 0;
        estados_criados++;

        //Verificando a caracteristicqa do estado, 1 são iniciais.
        if((conj_estados[i].caracteristica == 0) || (conj_estados[i].caracteristica == 3)){
            conf_atual[i] = 1;
        }else{
            conf_atual[i] = 0;
        }

    }

    //fazendo leitura de arquivo e armazenando na estrutura
    for(i = 0; !feof(transicoes); i++){

        fscanf(transicoes, "%s%s%s%s%s", dados_transicao[0], dados_transicao[1], dados_transicao[2], dados_transicao[3], dados_transicao[4]);

        //Procurando o estado que j� foi criado e adicionando 'aresta'.
        for(j = 0; j < estados_criados; j++){
            if(!strcmp(dados_transicao[0], conj_estados[j].nome)){

                strcpy(conj_estados[j].transicoes[conj_estados[j].posicao].caracter, dados_transicao[1]);
                strcpy(conj_estados[j].transicoes[conj_estados[j].posicao].estado_seguinte, dados_transicao[2]);
                strcpy(conj_estados[j].transicoes[conj_estados[j].posicao].desempilha, dados_transicao[3]);
                strcpy(conj_estados[j].transicoes[conj_estados[j].posicao].empilha, dados_transicao[4]);
                conj_estados[j].posicao++;

            }
        }
    }

    //pegando palavra que �suario quer testar
    printf("Digite a palavra a ser aceita pelo automato\n");
    scanf("%s", palavra);

    if(!palavra, "$"){

        //Automato em funcionamento
        for(i = 0; i < strlen(palavra)+ 1; i++){

            //Bucando em quais estados o automato est�
            for(j = 0; j < estados_criados; j++){

                if(conf_atual[j] == 1){

                    //Verificando se alguma das arestas, tem a letra da palavra.
                    for(k = 0; k < 5; k++){

                        if(conj_estados[j].transicoes[k].caracter[0] == palavra[i]){
                            aresta_existe = 1;
                            printf("%c\n",palavra[i]);
                            //Porocurando o pr�ximo estado que est� do outro lado da aresta.
                            for(l = 0; l < estados_criados; l++){
                                if(!strcmp(conj_estados[j].transicoes[k].estado_seguinte, conj_estados[l].nome)){

                                    //Configurando automato.
                                    conf_atual[j] = 5;
                                    conf_atual[conj_estados[l].id] = 6;

                                    //Empilhando e desempilhando  enquanto n ter lido toda a palavra
                                   if(i != strlen(palavra) - 1){
                                        if(strcmp(conj_estados[j].transicoes[k].empilha, "$")){
                                            push(PILHA, conj_estados[j].transicoes[k].empilha);
                                            printf("Empilhou %s\n", conj_estados[j].transicoes[k].empilha);
                                        }
                                        if(strcmp(conj_estados[j].transicoes[k].desempilha, "$")){
                                            if(vazia(PILHA)){
                                                printf("Palavra invalida\n");
                                                return;
                                            }
                                            pop(PILHA);
                                            printf("Desempilhou %s\n", conj_estados[j].transicoes[k].desempilha);
                                        }
                                    }
                                }
                            }

                        //Caso n tenha aresta e seja feita a leitura de um caracter invalido
                        }else if(aresta_existe == 0){
                          conf_atual[j] = 5;
                        }
                    }
                }
            }
            aresta_existe = 0;

            //Estabelecendo nosvas configura��es para o automato
            for(k = 0; k < estados_criados;k++){
                if(conf_atual[k] == 6){
                    conf_atual[k] = 1;
                }else
                    if(conf_atual[k] == 5){
                        conf_atual[k] = 0;
                    }
            }
        }
    }
    //Terminada a palavra e pilha vazia
    if (vazia(PILHA)){
        //Verificando se a palavra foi aceita
        for(i = 0; i <= estados_criados; i++){
            if((conf_atual[i] == 1) && ((conj_estados[i].caracteristica == 2) || (conj_estados[i].caracteristica == 3))){
                printf("Palavra aceita\n");
                return 0;
            }
        }
    }

    printf("Palavra nao aceita\n");
    return 0;
}
