/* hash.c
 * Implementacao do TAD Tabela Hash.
 * INF01203 - Estruturas de Dados - UFRGS
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

int g_hits;
int g_miss;
long g_colisoes;

/* ================================================================
 * Funcoes de hash
 * ================================================================ */

int hashDivisao(int k, int m) {
    return k % m;
}

int hashMultiplicativo(int k, int m) {
    const double A = 0.6180339887;   /* (sqrt(5)-1)/2 - Knuth */
    double prod = (double)k * A;
    double frac = prod - floor(prod);
    return (int)(m * frac);
}

int hashString(const char *s, int m) {
    const int B = 31;
    int h = 0;
    for (int i = 0; s[i] != '\0'; i++)
        h = (h * B + (unsigned char)s[i]) % m;
    return h;
}

/* ================================================================
 * Encadeamento separado
 * ================================================================ */

void encInicializar(TabelaEnc *t, int m) {
    t->m      = m;
    t->n      = 0;
    t->tabela = malloc(m * sizeof(NoHash  *));
    for (int i = 0; i < m; i++)
        t->tabela[i] = NULL;
}

void encInserir(TabelaEnc *t, int chave, char *receita) {
    int i    = hashDivisao(chave, t->m);
    if (t->tabela[i] != NULL) {
        g_colisoes++;
    }

    NoHash  *novo = malloc(sizeof(NoHash ));
    novo->chave  = chave;
    strcpy(novo->receita, receita);
    novo->prox   = t->tabela[i];
    t->tabela[i] = novo;
    t->n++;
}

NoHash*  encBuscar(const TabelaEnc *t, int chave) {
    int i = hashDivisao(chave, t->m);
    NoHash *p = t->tabela[i];
    while (p != NULL) {
        if (p->chave == chave) return p;
        p = p->prox;
    }
    return NULL;
}

int encRemover(TabelaEnc *t, int chave) {
    int i   = hashDivisao(chave, t->m);
    NoHash *p   = t->tabela[i];
    NoHash *ant = NULL;
    while (p != NULL) {
        if (p->chave == chave) {
            if (ant == NULL)
                t->tabela[i] = p->prox;
            else
                ant->prox = p->prox;
            free(p);
            t->n--;
            return 1;
        }
        ant = p;
        p   = p->prox;
    }
    return 0;
}

void encDestruir(TabelaEnc *t) {
    for (int i = 0; i < t->m; i++) {
        NoHash *p = t->tabela[i];
        while (p) {
            NoHash  *prox = p->prox;
            free(p);
            p = prox;
        }
    }
    free(t->tabela);
    t->tabela = NULL;
    t->m = t->n = 0;
}