//IA foi utilizada para gerar parte da lógica de manipulação de arquivos e formatação no terminal.
//seus comentários são sinalizados pelo uso de '/* */'

#ifndef RELATORIO_H
#define RELATORIO_H

#include "avl.h" /* NodoAvl, usado em relatorioCarregamentoAvl */

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