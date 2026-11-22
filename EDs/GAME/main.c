#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "timer.h"
#include "avl.h"
#include "relatorio.h"
#include "hash.h"
#include "heap.h"
#include "sorteio.h"
#include "pedidos.h"

#define ARQUIVO_LOG "medicoes.log"

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
    sorteioIniciar(semente);

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

    /* Intervalo dos codigos de pedido, obtido da AVL depois da carga. */
    int menor = 0;
    int maior = 0;

    if (raiz != NULL){
        menor = avlMinimo(raiz)->dado.cod;
        maior = avlMaximo(raiz)->dado.cod;
    }

    /* Um pedido sorteado por cliente. Para um teste de carga, troque
       numPedidos por uma constante maior (e evite imprimir cada pedido). */
    int numPedidos = numClientes;

    for (int i = 0; i < numPedidos; i++) {
        if(raiz == NULL) break;

        char receita[PEDIDOS_RECEITA_TAM];
        int cod = 0;

        if (pedidosObterReceita(raiz, tabelaCache, menor, maior, receita, &cod)) {
            printf("Pedido %d: codigo %d -> Receita: %s\n", i + 1, cod, receita);
        } else {
            printf("Pedido %d: codigo %d -> nao encontrado.\n", i + 1, cod);
        }
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