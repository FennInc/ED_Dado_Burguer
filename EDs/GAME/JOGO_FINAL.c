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

static char ler_ingrediente(const char *prompt);
static void ler_nome_jogador(char *destino, size_t tamanho);

//relatorio de desempenho
static void emitirRelatorioDesempenho(TabelaEnc *cache) {
    double mediaComparacoes = (g_buscas == 0) ? 0.0 : (double)g_comparacoes / g_buscas;
    double taxa = (g_hits + g_miss == 0) ? 0.0 : (double)g_hits / (g_hits + g_miss) * 100.0;
    double lambda = (cache->m == 0) ? 0.0 : (double)cache->n / cache->m;

    relatorioTitulo();
    relatorioHash(cache->m, cache->n, lambda, g_hits, g_miss, taxa, g_colisoes);
    relatorioBuscasAvl(g_buscas, g_comparacoes, mediaComparacoes, g_tempo_buscas);
    relatorioFim();
}

//limpar caractere da entrada soh pra facilitar a vida
static void limparEntrada(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void limparPedido(Pedido *pedido) {
    if (pedido == NULL) return;
    NoIngrediente *atual = pedido->receita_inicio;
    while (atual != NULL) {
        NoIngrediente *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    pedido->receita_inicio = NULL;
    pedido->qtd_ingredientes = 0;
}

static void configurarPedido(Pedido *pedido, int codigo, const char *receita) {
    if (pedido == NULL) return;

    limparPedido(pedido);
    pedido->codigo = codigo;
    snprintf(pedido->nome_lanche, sizeof(pedido->nome_lanche),
             "Receita: %s", receita != NULL ? receita : "");

    NoIngrediente **fim = &pedido->receita_inicio;
    if (receita == NULL) return;

    for (const char *p = receita; *p != '\0'; p++) {
        int id = converteIngId(*p);
        if (id < 0) continue;

        NoIngrediente *novo = malloc(sizeof(NoIngrediente));
        if (novo == NULL) return;
        novo->id = (IdIngrediente)id;
        novo->prox = NULL;
        *fim = novo;
        fim = &novo->prox;
        pedido->qtd_ingredientes++;
    }
}

static void executar_partida(NodoAvl *raiz, TabelaEnc *cache, int menorCod, int maiorCod, ListaHistorico *hist, const char *caminhoHist, unsigned int semente) {
    if (raiz == NULL || menorCod > maiorCod) {
        printf("Nao ha pedidos carregados para iniciar a partida.\n");
        return;
    }  //verifica pedidos

    int numClientes = sortearIntervalo(CFG_CLIENTES_MIN, CFG_CLIENTES_MAX);
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

    int codPedido = 0;
    char receitaAtual[CFG_RECEITA_TAM];

    if (!pedidosObterReceita(raiz, cache, menorCod, maiorCod, receitaAtual, &codPedido)) {
        printf("Falha ao sortear o pedido do primeiro cliente.\n");
        return;
    }

    Pedido pedidoVis = {0};
    configurarPedido(&pedidoVis, codPedido, receitaAtual);

    PilhaBurger lanche = {0 };
    inicializaPilha(&lanche);

    while (!partidaTerminou(&partida)) {
        int descartados = IniciarTurno(&partida);
        if (descartados > 0) {
            printf("Descartados por validade neste turno: %d\n", descartados);
        }

        renderizar_tela_jogo(partida.turno, partida.clientesAtendidos, partida.clientesTotal - partida.clientesAtendidos, &(fila.frente->dados), &pedidoVis, &lanche, &partida.estoque);

        //Acoes: 0 = abandonar, 1 = preparar, 2 = adicionar, 3 = remover
        int acao = ler_opcao_inteira("\nEscolha uma acao: ", 0, 3);

        if (acao == 0) {
            partidaAbandonar(&partida);

        } else if (acao == 1) {
           //preparacao
            char ing = ler_ingrediente("Preparar ingrediente (n=carne, o=ovo, b=bacon, s=queijo suico, q=cheddar, c=cebola, t=tomate): ");
            if (ing != '\0') {

                int venc = -1;
                int res = estoquePreparar(&partida.estoque, ing, partida.turno, &venc);
                if (res == 1) {
                    if (estoqueEhPerecivel(ing)) {
                        printf("Preparou %s (vence no turno %d).\n", estoqueNomeIngrediente(ing), venc);
                    } else {
                        printf("Preparou %s.\n", estoqueNomeIngrediente(ing));
                    }
                    turnosAcaoRealizada(&partida, &fila.frente->dados.paciencia_atual, 0);
                } else if (res == 0) {
                    printf("Estoque cheio para %s.\n", estoqueNomeIngrediente(ing));
                } else if (estoqueEhIlimitado(ing)) {
                    printf("%s nao precisa de preparo.\n", estoqueNomeIngrediente(ing));
                } else {
                    printf("Ingrediente invalido.\n");
                }
            }

        } else if (acao == 2) {
            char ing = ler_ingrediente("Adicionar ingrediente (n=carne, o=ovo, b=bacon, s=queijo suico, q=cheddar, c=cebola, t=tomate, z=pao de cima, k=ketchup, m=mostarda, p=picles, a=alface): ");
            if (ing != '\0') {
                int vencConsumido = -1;
                int ok = estoqueConsumir(&partida.estoque, ing, &vencConsumido);
                if (ok == 1) {
                    int empilhado = empilha_ingrediente(&lanche, (IdIngrediente)converteIngId(ing));
                    if (empilhado < 0) {
                        printf("Não pode adicionar ingrediente encima do pão!\n");
                    } else if (empilhado == 1) {
                        printf("Adicionou %s ao lanche.\n", estoqueNomeIngrediente(ing));
                        if (ing == 'z' && validar_hamburguer(&lanche, &pedidoVis)) {
                            printf("Pedido concluido com sucesso!\n");
                            Cliente atendido;
                            turnosAcaoRealizada(&partida, &fila.frente->dados.paciencia_atual, 1);
                            desenfileira_FilaClientes(&fila, &atendido);

                            if (!partidaTerminou(&partida)) {
                                if (!pedidosObterReceita(raiz, cache, menorCod, maiorCod, receitaAtual, &codPedido)) {
                                    receitaAtual[0] = '\0';
                                }
                                configurarPedido(&pedidoVis, codPedido, receitaAtual);
                                inicializaPilha(&lanche);
                            } else {
                                receitaAtual[0] = '\0';
                            }
                        } else if (ing == 'z') {
                            printf("Lanche incorreto! Turno gasto.\n");
                            turnosAcaoRealizada(&partida, &fila.frente->dados.paciencia_atual, 0);
                        } else {
                            turnosAcaoRealizada(&partida, &fila.frente->dados.paciencia_atual, 0);
                        }
                    } else {
                        printf("Nao foi possivel adicionar o ingrediente ao lanche.\n");
                    }
                } else if (ok == 0) {
                    printf("Sem estoque de %s.\n", estoqueNomeIngrediente(ing));
                } else {
                    printf("Ingrediente invalido.\n");
                }
            }

        } else if (acao == 3) {
            IdIngrediente rem;
            if (topo(&lanche, &rem) && desempilha_ingrediente(&lanche)) {
                printf("Removeu %s do topo.\n", nome_ingrediente(rem));
                turnosAcaoRealizada(&partida, &fila.frente->dados.paciencia_atual, 0);
            } else {
                printf("Lanche ja esta vazio.\n");
            }
        }

        if (!partidaTerminou(&partida)) {
            renderizar_tela_jogo(partida.turno, partida.clientesAtendidos,
                                 partida.clientesTotal - partida.clientesAtendidos,
                                 cliente_atual(&fila), &pedidoVis, &lanche,
                                 &partida.estoque);
        }
    }

    printf("\n=== FIM DA PARTIDA ===\n");
    printf("Atendidos: %d/%d | Turnos: %d | Pontuacao: %d\n", partida.clientesAtendidos, partida.clientesTotal, partida.turno, partidaPontuacao(&partida));

    char nome[MAX_NOME];
    ler_nome_jogador(nome, sizeof(nome));

    RegistroHistorico registro = {0};
    snprintf(registro.nome_jogador, sizeof(registro.nome_jogador), "%s", nome);
    registro.pontuacao = partidaPontuacao(&partida);
    registro.clientes_atendidos = partida.clientesAtendidos;
    registro.turnos_jogados = partida.turno > 1 ? partida.turno - 1 : 0;

    if (!inserir_historico(hist, registro)) {
        printf("Erro: nao foi possivel inserir o registro no historico.\n");
    } else if (!salvar_historico(hist, caminhoHist)) {
        printf("Erro: nao foi possivel salvar o historico em '%s'.\n", caminhoHist);
    } else {
        printf("Partida salva no historico de pontuacoes!\n");
    }

    salvar_historico(hist, caminhoHist);

    Cliente remCli;
    while (desenfileira_FilaClientes(&fila, &remCli)) {
    }

    //Relatorio de desempenho ao encerrar a partida antes de voltar ao menu.
    emitirRelatorioDesempenho(cache);
}

static int ingredienteValido(char ing) {
    return ing == 'n' || ing == 'o' || ing == 'b' || ing == 's' ||
           ing == 'q' || ing == 'c' || ing == 't' || ing == 'z' ||
           ing == 'k' || ing == 'm' || ing == 'p' || ing == 'a';
}

static char ler_ingrediente(const char *prompt) {
    char linha[64];
    printf("%s", prompt);
    if (fgets(linha, sizeof(linha), stdin) == NULL) return '\0';

    for (char *p = linha; *p != '\0'; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') continue;
        char ing = (char)tolower((unsigned char)*p);
        return ingredienteValido(ing) ? ing : ing;
    }
    return '\0';
}

static void ler_nome_jogador(char *destino, size_t tamanho) {
    if (destino == NULL || tamanho == 0) return;

    printf("Digite seu nome: ");
    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        snprintf(destino, tamanho, "Jogador");
        return;
    }

    size_t fim = strcspn(destino, "\r\n");
    destino[fim] = '\0';

    for (char *p = destino; *p != '\0'; p++) {
        if (*p == ';') *p = ' ';
    }

    if (destino[0] == '\0') {
        snprintf(destino, tamanho, "Jogador");
    }
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
            limparEntrada();
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