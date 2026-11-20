#ifndef RELATORIO_H
#define RELATORIO_H

#include "avl.h" /* NodoAvl, usado em relatorioCarregamentoAvl */

/*
 * relatorio.h - medicoes.log e Relatorio de Desempenho (spec, secao 5).
 *
 * Cada funcao escreve na tela e no arquivo, no formato do apendice B.
 * As funcoes apenas formatam e gravam. Os valores das metricas (media de
 * comparacoes, taxa de acerto, fator de carga, etc.) devem vir calculados
 * por quem os mede.
 *
 * Fluxo tipico:
 *   relatorioIniciar("medicoes.log");   -- uma vez por execucao
 *   relatorioParametros(semente, n);
 *   relatorioCarregamentoAvl(raiz, tempo);
 *   ...
 *   relatorioTitulo();                  -- ao encerrar a partida
 *   relatorioBuscasAvl(...);
 *   relatorioHash(...);
 *   relatorioFim();
 *
 * Cada chamada acrescenta ao arquivo (modo "a"). O arquivo so e
 * sobrescrito em relatorioIniciar.
 */

/* Cria ou sobrescreve o arquivo. Retorna 0 em sucesso e -1 em erro. */
int relatorioIniciar(const char *caminho);

/* Liga (1) ou desliga (0) a exibicao na tela. O arquivo e sempre gravado. */
void relatorioMostrarNaTela(int ligado);

/* Parametros da partida (semente e numero de clientes sorteado). */
void relatorioParametros(unsigned int semente, int numClientes);

/* "=== RELATORIO DE DESEMPENHO ===" */
void relatorioTitulo(void);

/* Secao "AVL: carregamento": nos, altura (convencao da spec), rotacoes e tempo. */
void relatorioCarregamentoAvl(const NodoAvl *raiz, double tempo);

/* Secao "Historico: carregamento". */
void relatorioHistoricoCarregado(long entradas);

/* Secao "AVL: buscas durante a partida". */
void relatorioBuscasAvl(long buscas, long comparacoes,
                        double mediaComparacoes, double tempoTotal);

/* Secao "Tabela hash". */
void relatorioHash(int m, long n, double fatorCarga,
                   long hits, long misses, double taxaAcerto, long colisoes);

/* Secao "Ordenacao do historico". Pode ser chamada a cada acesso ao Historico. */
void relatorioOrdenacao(long tamanho, const char *algoritmo, double tempo);

/* Linha de fim de relatorio. */
void relatorioFim(void);

#endif /* RELATORIO_H */