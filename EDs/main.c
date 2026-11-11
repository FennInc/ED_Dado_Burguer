#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "avl.h"

//negocio de manipular arquivos fiz com IA mesmo.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <pedidos> <historico>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'\n", argv[1]);
        return 1;
    }

    //Historico pode nao existir na primeira execucao: NULL = historico vazio.
    FILE *fh = fopen(argv[2], "r");

    NodoAvl *raiz = NULL; // arvore de pedidos

    // Zera o contador global pra poder reutilizar o programa omagah wow
    g_rotacoes = 0;

    Timer t;
    timer_iniciar(&t);

    Pedido val;

    while (fscanf(fp, "%d %7s", &val.cod, val.receita) == 2) {
        raiz = avlInserir(raiz, val); // insere os baguiu nos trem né
    }

    double tempo = timer_parar(&t);
    fclose(fp);

    //gravando 
    avlGravarMedicoes(raiz);

    printf("Tempo de carregamento: %.6f s\n", tempo);

    if (fh != NULL) fclose(fh);
    
    //mata, n sei se é necessario proq vamos fazer dps
    raiz = avlDestruir(raiz);

    return 0;
}