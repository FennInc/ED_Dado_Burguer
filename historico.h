#ifndef HISTORICO_H
#define HISTORICO_H

#include <stdbool.h>
#include "tipos.h"

typedef struct {
    RegistroHistorico *dados;
    int tamanho;
    int capacidade;
} ListaHistorico;

/* Critérios exigidos para visualização do histórico */
typedef enum {
    ORDENAR_POR_PONTUACAO = 1, /* Decrescente (maior pontuação primeiro) */
    ORDENAR_POR_NOME = 2       /* Crescente (ordem alfabética A-Z) */
} CriterioOrdenacao;

/* Métricas coletadas durante a ordenação para análise de desempenho */
typedef struct {
    int elementos_ordenados;
    long comparacoes;
    double tempo_segundos;
} MetricasOrdenacao;

/* Funções do Passo 2.1 */
void inicializar_historico(ListaHistorico *lista, int cap_inicial);
bool inserir_historico(ListaHistorico *lista, RegistroHistorico reg);
void liberar_historico(ListaHistorico *lista);
bool carregar_historico(ListaHistorico *lista, const char *caminho);
bool salvar_historico(const ListaHistorico *lista, const char *caminho);

/* Funções do Passo 2.2 */
RegistroHistorico* gerar_copia_ordenada(
    const ListaHistorico *lista,
    CriterioOrdenacao criterio,
    MetricasOrdenacao *metricas
);

bool exibir_historico_ordenado(
    const ListaHistorico *lista,
    CriterioOrdenacao criterio,
    MetricasOrdenacao *metricas_out
);

#endif