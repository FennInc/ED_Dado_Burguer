/* hash.h
 * TAD Tabela Hash - funcoes de hash e tratamento de colisao.
 * Cobre o conteudo das aulas 19 e 20 de INF01203 - UFRGS.
 *
 * Compilar:
 *   gcc -Wall -o hash_demo hash_demo.c hash.c -lm
 */

#ifndef HASH_H
#define HASH_H

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

typedef struct No {
    int chave;
    int valor;
    struct No *prox;
} No;

typedef struct {
    No **tabela;   /* vetor de ponteiros para listas */
    int m;         /* numero de slots */
    int n;         /* numero de elementos */
} TabelaEnc;

void encInicializar(TabelaEnc *t, int m);
void encInserir(TabelaEnc *t, int chave, int valor);
int  encBuscar(const TabelaEnc *t, int chave);
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


/* ================================================================
 * Sondagem linear (aula 20)
 * ================================================================
 * pos(i) = (h(k) + i) mod m,  i = 0, 1, 2, ...
 * Vantagem: boa localidade de cache.
 * Desvantagem: agrupamento primario (clusters longos).
 * Requer lambda < 1; m primo recomendado.
 */

typedef struct {
    Slot *tabela;
    int m;   /* numero de slots (primo recomendado) */
    int n;   /* numero de elementos */
} TabelaLinear;

void linInicializar(TabelaLinear *t, int m);
int  linInserir(TabelaLinear *t, int chave, int valor);
int  linBuscar(const TabelaLinear *t, int chave);
int  linRemover(TabelaLinear *t, int chave);
void linDestruir(TabelaLinear *t);


/* ================================================================
 * Sondagem quadratica (aula 20)
 * ================================================================
 * pos(i) = (h(k) + i^2) mod m,  i = 0, 1, 2, ...
 * Elimina agrupamento primario; pode nao visitar todos os slots
 * (garantido visitar pelo menos m/2 slots distintos se m e primo).
 * Requer lambda < 0,5 para garantia de insercao; m primo recomendado.
 */

typedef struct {
    Slot *tabela;
    int m;   /* numero de slots (primo recomendado) */
    int n;   /* numero de elementos */
} TabelaQuad;

void quadInicializar(TabelaQuad *t, int m);
int  quadInserir(TabelaQuad *t, int chave, int valor);
int  quadBuscar(const TabelaQuad *t, int chave);
int  quadRemover(TabelaQuad *t, int chave);
void quadDestruir(TabelaQuad *t);


/* ================================================================
 * Hash duplo (aula 20)
 * ================================================================
 * pos(i) = (h1(k) + i*h2(k)) mod m,  i = 0, 1, 2, ...
 * h1(k) = k mod m
 * h2(k) = 1 + (k mod (m-2))   - garante passo >= 1
 * Elimina agrupamento secundario; melhor distribuicao entre os tres.
 * Requer m primo (e m-2 primo para maxima qualidade).
 */

typedef struct {
    Slot *tabela;
    int m;   /* numero de slots (primo recomendado, e m-2 primo) */
    int n;   /* numero de elementos */
} TabelaDouble;

void dblInicializar(TabelaDouble *t, int m);
int  dblInserir(TabelaDouble *t, int chave, int valor);
int  dblBuscar(const TabelaDouble *t, int chave);
int  dblRemover(TabelaDouble *t, int chave);
void dblDestruir(TabelaDouble *t);

#endif /* HASH_H */
