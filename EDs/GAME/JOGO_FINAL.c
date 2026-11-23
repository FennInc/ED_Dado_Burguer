#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "config.h"
#include "tipos.h"
#include "timer.h"
#include "sorteio.h"
#include "avl.h"
#include "hash.h"
#include "heap.h"
#include "estoque.h"
#include "partida.h"
#include "pedidos.h"
#include "historico.h"
#include "fila.h"
#include "montagem.h"
#include "ui.h"
#include "relatorio.h"

#define ARQUIVO_LOG "medicoes.log"

//relatorio
static void emitirRelatorioDesempenho(TabelaEnc *cache) {
    double mediaComparacoes = (g_buscas == 0) ? 0.0 : (double)g_comparacoes / g_buscas;
    double taxa = (g_hits + g_miss == 0) ? 0.0 : (double)g_hits / (g_hits + g_miss) * 100.0;
    double lambda = (cache->m == 0) ? 0.0 : (double)cache->n / cache->m;

    relatorioTitulo();
    relatorioHash(cache->m, cache->n, lambda, g_hits, g_miss, taxa, g_colisoes);
    relatorioBuscasAvl(g_buscas, g_comparacoes, mediaComparacoes, g_tempo_buscas);
    relatorioFim();
}

static void executar_partida(NodoAvl *raiz,
                             TabelaEnc *cache,
                             int menorCod,
                             int maiorCod,
                             ListaHistorico *hist,
                             const char *caminhoHist,
                             unsigned int semente) {
    if (raiz == NULL || menorCod > maiorCod) {
        printf("Nao ha pedidos carregados para iniciar a partida.\n");
        return;
    } //verifica pedidos

    int numClientes = sortearIntervalo(CFG_CLIENTES_MIN, CFG_CLIENTES_MAX); //sorteia quantidade de clientes
    relatorioParametros(semente, numClientes);

    Partida partida;
    IniciarPartida(&partida, numClientes);

    FilaClientes fila;
    inicializa_FilaClientes(&fila);
    fila.tamanho = 0;
    for (int i = 1; i <= numClientes; i++) {
        Cliente c = {0};
        c.id_cliente = i;
        snprintf(c.nome, sizeof(c.nome), "Cliente #%d", i);
        c.paciencia_atual = CFG_PACIENCIA_MAX;
        c.paciencia_maxima = CFG_PACIENCIA_MAX;
        enfileira_FilaClientes(&fila, c);
    }

    int pacienciaAtual = CFG_PACIENCIA_MAX;
    int codPedido = 0;
    char receitaAtual[CFG_RECEITA_TAM];

    if (!pedidosObterReceita(raiz, cache, menorCod, maiorCod,
                             receitaAtual, &codPedido)) {
        printf("Falha ao sortear o pedido do primeiro cliente.\n");
        return;
    }

    Pedido pedidoVis = {0};
    pedidoVis.codigo = codPedido;
    snprintf(pedidoVis.nome_lanche, sizeof(pedidoVis.nome_lanche),"Receita: %s", receitaAtual);

    PilhaBurger lanche = {0};
    b1_inicializar_montagem_e_receita(&lanche, &pedidoVis, receitaAtual);

    while (!partidaTerminou(&partida)) {
        int descartados = IniciarTurno(&partida);
        if (descartados > 0) {
            printf("Descartados por validade neste turno: %d\n", descartados);
        }

        EstoqueVisual estVis;
        b1_preencher_estoque_visual(&partida.estoque, &estVis);

        b1_renderizar_partida(&partida, &fila, &pedidoVis, &lanche, &estVis,
                              pacienciaAtual, codPedido, receitaAtual);

        int acao = ler_opcao_inteira("\nEscolha uma acao: ", 0, 4);

        switch(acao){ //quitar
        case 0 :
            partidaAbandonar(&partida);
            break;

        case 1: //preparar ingrediente
            char ing = '\0';
            printf("Preparar ingrediente (n=carne, o=ovo, b=bacon, s=queijo suico, q=cheddar, c=cebola, t=tomate): ");
            if (scanf(" %c", &ing) == 1) {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                int venc = -1;
                int res = estoquePreparar(&partida.estoque, ing, partida.turno, &venc);
                if (res == 1) {
                    if (estoqueEhPerecivel(ing)) {
                        printf("Preparou %s (vence no turno %d).\n", estoqueNomeIngrediente(ing), venc);
                    } else {
                        printf("Preparou %s.\n", estoqueNomeIngrediente(ing));
                    }
                    turnosAcaoRealizada(&partida, &pacienciaAtual, 0);
                } else if (res == 0) {
                    printf("Estoque cheio para %s.\n", estoqueNomeIngrediente(ing));
                } else {
                    if(estoqueEhIlimitado(ing)) printf("%s não precisa de preparo\n", estoqueNomeIngrediente(ing));
                    else printf("Ingrediente invalido.\n");
                }
            }
        break;
        
        case 2: //adicionar ao lanche
            ing = '\0';
            printf("Adicionar ingrediente (n, o, b, s, q, c, t): ");
            if (scanf(" %c", &ing) == 1) {
                if(estoqueEhIlimitado(ing)){
                    if (b1_adicionar_na_pilha(&lanche, ing)) {
                        printf("Adicionou %s ao lanche.\n", estoqueNomeIngrediente(ing));
                        turnosAcaoRealizada(&partida, &pacienciaAtual, 0);
                    break;
                }
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                int vencConsumido = -1;
                int ok = estoqueConsumir(&partida.estoque, ing, &vencConsumido);
                if (ok == 1) {
                    if (b1_adicionar_na_pilha(&lanche, ing)) {
                        printf("Adicionou %s ao lanche.\n", estoqueNomeIngrediente(ing));
                            // se a receita terminou
                           if (b1_validar_lanche(&lanche, &pedidoVis, receitaAtual)) {
                                printf("Pedido entregue com sucesso!\n");

                                Cliente atendido;
                                desenfileira_Clientes(&fila, &atendido);
                                turnosAcaoRealizada(&partida, &pacienciaAtual, 1);

                                if (!partidaTerminou(&partida)) {
                                    pacienciaAtual = CFG_PACIENCIA_MAX;
                                    if (!pedidosObterReceita(raiz, cache, menorCod, maiorCod,
                                                            receitaAtual, &codPedido)) {
                                        receitaAtual[0] = '\0';
                                    }
                                    pedidoVis.codigo = codPedido;
                                    snprintf(pedidoVis.nome_lanche, sizeof(pedidoVis.nome_lanche),"Receita: %s", receitaAtual);
                                    b1_inicializar_montagem_e_receita(&lanche, &pedidoVis, receitaAtual);
                                } else {
                                    receitaAtual[0] = '\0';
                                }
                            } else if (ing == 'z'){
                                printf("Lanche incorreto! Turno gasto.\n");
                                turnosAcaoRealizada(&partida, &pacienciaAtual, 0);
                                break;
                            }
                        turnosAcaoRealizada(&partida, &pacienciaAtual, 0);
                    }
                } else if (ok == 0) {
                    printf("Sem estoque de %s.\n", estoqueNomeIngrediente(ing));
                } else {
                    printf("Ingrediente invalido.\n");
                }
            }
        break;

        case 3: //remover do lanche
            char rem = '\0';
            if (b1_remover_do_topo(&lanche, &rem)) {
                printf("Removeu %s do topo.\n", estoqueNomeIngrediente(rem));
                turnosAcaoRealizada(&partida, &pacienciaAtual, 0);
            } else {
                printf("Lanche ja esta vazio.\n");
            }
        break;
        }
    }

    printf("\n=== FIM DA PARTIDA ===\n");
    printf("Atendidos: %d/%d | Turnos: %d | Pontuacao: %d\n",
           partida.clientesAtendidos,
           partida.clientesTotal,
           partida.turno,
           partidaPontuacao(&partida));

    b1_registrar_e_salvar_historico(hist, caminhoHist, &partida);

    Cliente remCli;
    while (desenfileira_Clientes(&fila, &remCli)) {
    }
    emitirRelatorioDesempenho(cache);
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

    if (relatorioIniciar(ARQUIVO_LOG) != 0) {
        fprintf(stderr, "Aviso: o log de medicoes nao sera gravado.\n");
    }

    unsigned int semente = CFG_USAR_SEMENTE_FIXA ? (unsigned int)CFG_SEMENTE
                                                 : (unsigned int)time(NULL);
    sorteioIniciar(semente);

    NodoAvl *raiz = NULL;
    TabelaEnc *tabelaCache = malloc(sizeof(TabelaEnc));
    if (tabelaCache == NULL) {
        fclose(fp);
        return 1;
    }
    encInicializar(tabelaCache, CFG_HASH_M);

    g_rotacoes = 0;
    g_tempo_buscas = 0.0;
    g_buscas = 0;
    g_comparacoes = 0;
    g_hits = 0;
    g_miss = 0;
    g_colisoes = 0;

    Timer tCarga;
    timer_iniciar(&tCarga);

    PedidoAVL val;
    while (fscanf(fp, "%d %7s", &val.cod, val.receita) == 2) {
        raiz = avlInserir(raiz, val);
    }

    double tempoCarga = timer_parar(&tCarga);
    fclose(fp);

    relatorioCarregamentoAvl(raiz, tempoCarga);

    int menorCod = 0;
    int maiorCod = 0;
    if (raiz != NULL) {
        menorCod = avlMinimo(raiz)->dado.cod;
        maiorCod = avlMaximo(raiz)->dado.cod;
    }

    // TODO [Bloco 1 - Colega]: carregar historico inicial em lista
    ListaHistorico historico;
    inicializar_historico(&historico, 16);
    carregar_historico(&historico, argv[2]);
    relatorioHistoricoCarregado(historico.tamanho);

    int rodando = 1;
    while (rodando) {
        impressao_menu_principal();
        int opc = ler_opcao_inteira("\n> ", 1, 3);

        if (opc == 1) {
            executar_partida(raiz, tabelaCache, menorCod, maiorCod,
                             &historico, argv[2], semente);
        } else if (opc == 2) {
            como_jogar();
        } else if (opc == 3) {
            int crit = ler_opcao_inteira("Ordenar por (1=Pontuacao, 2=Nome): ", 1, 2);
            MetricasOrdenacao met;
            exibir_historico_ordenado(&historico,
                                      (crit == 2) ? ORDENAR_POR_NOME : ORDENAR_POR_PONTUACAO,
                                      &met);
            relatorioOrdenacao(met.elementos_ordenados,
                               (crit == 2) ? "Mergesort (Nome)" : "Mergesort (Pontuacao)",
                               met.tempo_segundos);
        }

        char resp = '\0';
        printf("\nVoltar ao menu? (s/n): ");
        if (scanf(" %c", &resp) == 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            if (tolower((unsigned char)resp) == 'n') {
                rodando = 0;
            }
        } else {
            rodando = 0;
        }
    }

    liberar_historico(&historico);
    raiz = avlDestruir(raiz);
    encDestruir(tabelaCache);
    free(tabelaCache);

    return 0;
}