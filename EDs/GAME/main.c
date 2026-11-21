#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "timer.h"
#include "avl.h"
#include "relatorio.h"
#include "hash.h"
#include "heap.h"

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
    TabelaEnc *tabelaCache = malloc(sizeof(TabelaEnc));
    encInicializar(tabelaCache, CFG_HASH_M);

    /* Zera os contadores globais antes de cada execucao. */
    g_rotacoes = 0;
    g_tempo_buscas = 0.0;
    g_buscas = 0;
    g_comparacoes = 0;
    g_hits = 0;
    g_miss = 0;
    g_colisoes = 0;

    Timer t;
    timer_iniciar(&t);

    Pedido val;
    while (fscanf(fp, "%d %7s", &val.cod, val.receita) == 2) {
        raiz = avlInserir(raiz, val);
    }

    double tempo = timer_parar(&t);
    fclose(fp);

    relatorioCarregamentoAvl(raiz, tempo);

    Timer tb;
    int cod;
    printf("Digite um codigo para buscar (-1 para sair): ");
    while (scanf("%d", &cod) == 1 && cod != -1) {
        timer_iniciar(&tb);
        NoHash *n = encBuscar(tabelaCache, cod);
        if(n != NULL) {
            g_hits++;
            printf("Receita: %s\n", n->receita);
        } else {
        NodoAvl *r = avlBuscar(raiz, cod);
        g_tempo_buscas += timer_parar(&tb);
        g_buscas++;
        g_miss++;

        if (r != NULL) { 
            printf("Receita: %s\n", r->dado.receita);
            encInserir(tabelaCache, r->dado.cod, r->dado.receita);
        }
        else printf("Codigo %d nao encontrado.\n", cod);
        }
        printf("Digite um codigo para buscar (-1 para sair): ");
    }

    /* Relatorio de desempenho ao encerrar. */
    double mediaComparacoes = (g_buscas == 0) ? 0.0 : (double)g_comparacoes/g_buscas;
    double taxa = (g_hits + g_miss == 0) ? 0.0 : (double)g_hits/(g_hits+g_miss) * 100;
    double lambda = (double)tabelaCache->n/tabelaCache->m;
    
    relatorioTitulo();
    relatorioHash(tabelaCache->m, tabelaCache->n, lambda, g_hits, g_miss, taxa, g_colisoes);
    relatorioBuscasAvl(g_buscas, g_comparacoes, mediaComparacoes, g_tempo_buscas);
    relatorioFim();

    if (fh != NULL) fclose(fh);

    /* Libera a memoria da arvore. */
    raiz = avlDestruir(raiz);

    encDestruir(tabelaCache);
    free(tabelaCache);

    return 0;
}