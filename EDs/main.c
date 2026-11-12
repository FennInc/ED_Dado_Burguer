#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "avl.h"

//negocio de manipular arquivos fiz com IA mesmo.
int main(int argc, char *argv[]) {
    if (argc < 3) {
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
    g_tempo_buscas = 0.0;
    g_buscas = 0;
    g_comparacoes = 0;


    Timer t;
    timer_iniciar(&t);

    Pedido val;

    while (fscanf(fp, "%d %7s", &val.cod, val.receita) == 2) {
        raiz = avlInserir(raiz, val); // insere os baguiu nos trem né
    }

    double tempo = timer_parar(&t);
    fclose(fp);

    Timer tb;
    int cod;
    printf("Digite um codigo para buscar (-1 para sair): ");
    while (scanf("%d", &cod) == 1 && cod != -1) {
        timer_iniciar(&tb);
        NodoAvl *r = avlBuscar(raiz, cod);
        g_tempo_buscas += timer_parar(&tb);
        g_buscas++;

        if (r != NULL) printf("Receita: %s\n", r->dado.receita);
        else           printf("Codigo %d nao encontrado.\n", cod);

        printf("Digite um codigo para buscar (-1 para sair): ");
    }

    printf("Buscas: %d\n", g_buscas);
    printf("Comparacoes: %d\n", g_comparacoes);
    printf("Tempo acumulado: %.6f s\n", g_tempo_buscas);

    //gravando 
    avlGravarMedicoes(raiz, tempo);

    printf("Tempo de carregamento: %.2f s\n", tempo);

    if (fh != NULL) fclose(fh);
    
    //mata, n sei se é necessario proq vamos fazer dps
    raiz = avlDestruir(raiz);

    return 0;
}