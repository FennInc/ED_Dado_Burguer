/* hash.h
 * TAD Tabela Hash - funcoes de hash e tratamento de colisao.
 * Cobre o conteudo das aulas 19 e 20 de INF01203 - UFRGS.
 *
 * Compilar:
 *   gcc -Wall -o hash_demo hash_demo.c hash.c -lm
 */

#ifndef HASH_H
#define HASH_H

extern int g_hits;
extern int g_miss;
extern long g_colisoes;

/* ================================================================
 * Funcoes de hash (aula 19)
 * ================================================================ */

/* Hash por divisao: h(k) = k mod m
 * Recomendacao: m primo, nao proximo de potencia de 2 ou 10. */
int hashDivisao(int k, int m);

/* Hash multiplicativo de Knuth: h(k) = floor(m * {k*A}), A ~ 0,618.
 * O carater irracional de A espalha os indices uniformemente. */
int hashMultiplicativo(int k, int m);

/* Hash polinomial para strings via Regra de Horner, base B = 31.
 * h(0) = 0,  h(i) = h(i-1)*B + s[i]  (mod m),  h(s) = h_L mod m */
int hashString(const char *s, int m);


/* ================================================================
 * Encadeamento separado (aula 20)
 * ================================================================
 * Cada slot e a cabeca de uma lista encadeada.
 * Suporta fator de carga lambda > 1.
 * Custo medio de busca: O(1 + lambda).
 */

typedef struct NoHash {
    int chave;
    char receita[8];
    struct NoHash *prox;
} NoHash;

typedef struct {
    NoHash **tabela;   /* vetor de ponteiros para listas */
    int m;         /* numero de slots */
    int n;         /* numero de elementos */
} TabelaEnc;

void encInicializar(TabelaEnc *t, int m);
void encInserir(TabelaEnc *t, int chave, char *receita);
NoHash*  encBuscar(const TabelaEnc *t, int chave);
int  encRemover(TabelaEnc *t, int chave);
void encDestruir(TabelaEnc *t);


/* ================================================================
 * Enderecamento aberto - tipo de slot compartilhado (aula 20)
 * ================================================================
 * Remocao usa marca lapide (DELETADO) para nao interromper sondagens.
 */

typedef enum { VAZIO, OCUPADO, DELETADO } EstadoSlot;

typedef struct {
    int chave;
    int valor;
    EstadoSlot estado;
} Slot;

#endif /* HASH_H */
