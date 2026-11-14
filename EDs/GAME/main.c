#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "timer.h"
#include "avl.h"
#include "relatorio.h"
/* TODO: #include "hash.h" quando o fluxo de cache for ligado */

#define ARQUIVO_LOG "medicoes.log"

/* Sorteia um inteiro no intervalo [min, max], inclusive. */
static int sortearIntervalo(int min, int max) {
    return min + rand() % (max - min + 1);
}

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

    /* Historico pode nao existir na primeira execucao: NULL = historico vazio.
       A leitura do historico fica com o Bloco I. */
    FILE *fh = fopen(argv[2], "r");

    /* Cria (ou sobrescreve) o medicoes.log. Uma vez por execucao. */
    if (relatorioIniciar(ARQUIVO_LOG) != 0) {
        fprintf(stderr, "Aviso: o log de medicoes nao sera gravado.\n");
    }

    /* Semente do sorteio: fixa (medicoes repetiveis) ou variavel (jogo normal). */
    unsigned int semente = CFG_USAR_SEMENTE_FIXA ? (unsigned int)CFG_SEMENTE
                                                 : (unsigned int)time(NULL);
    srand(semente);

    /* Numero de clientes da partida, sorteado entre o minimo e o maximo. */
    int numClientes = sortearIntervalo(CFG_CLIENTES_MIN, CFG_CLIENTES_MAX);
    relatorioParametros(semente, numClientes);

    NodoAvl *raiz = NULL; /* arvore de pedidos */

    /* Zera os contadores globais antes de cada execucao. */
    g_rotacoes = 0;
    g_tempo_buscas = 0.0;
    g_buscas = 0;
    g_comparacoes = 0;
    /* TODO: zerar tambem os contadores da hash (hits, misses, colisoes) */

    /* Carregamento dos pedidos na AVL, cronometrado. */
    Timer t;
    timer_iniciar(&t);

    Pedido val;
    while (fscanf(fp, "%d %7s", &val.cod, val.receita) == 2) {
        raiz = avlInserir(raiz, val);
    }

    double tempo = timer_parar(&t);
    fclose(fp);

    relatorioCarregamentoAvl(raiz, tempo);
    /* TODO: relatorioHistoricoCarregado(entradas) quando o Bloco I entregar
       a quantidade de entradas lidas do historico. */

    /* Modo de teste: busca manual de codigos pelo teclado.
       TODO: no jogo, o codigo vem do sorteio e a hash e consultada antes
       da AVL (hit: devolve a receita; miss: busca na AVL e insere na hash). */
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

    /* Relatorio de desempenho ao encerrar. */
    double mediaComparacoes = 0.0; /* TODO: calcular (0 se nao houve busca) */

    relatorioTitulo();
    relatorioBuscasAvl(g_buscas, g_comparacoes, mediaComparacoes, g_tempo_buscas);
    /* TODO: relatorioHash(m, n, lambda, hits, misses, taxa, colisoes) */
    relatorioFim();

    if (fh != NULL) fclose(fh);

    /* Libera a memoria da arvore. */
    raiz = avlDestruir(raiz);

    return 0;
}