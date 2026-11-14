/* hash.c
 * Implementacao do TAD Tabela Hash.
 * INF01203 - Estruturas de Dados - UFRGS
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

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
    t->tabela = malloc(m * sizeof(No *));
    for (int i = 0; i < m; i++)
        t->tabela[i] = NULL;
}

void encInserir(TabelaEnc *t, int chave, int valor) {
    int i    = hashDivisao(chave, t->m);
    No *novo = malloc(sizeof(No));
    novo->chave  = chave;
    novo->valor  = valor;
    novo->prox   = t->tabela[i];
    t->tabela[i] = novo;
    t->n++;
}

int encBuscar(const TabelaEnc *t, int chave) {
    int i = hashDivisao(chave, t->m);
    No *p = t->tabela[i];
    while (p != NULL) {
        if (p->chave == chave) return p->valor;
        p = p->prox;
    }
    return -1;
}

int encRemover(TabelaEnc *t, int chave) {
    int i   = hashDivisao(chave, t->m);
    No *p   = t->tabela[i];
    No *ant = NULL;
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
        No *p = t->tabela[i];
        while (p) {
            No *prox = p->prox;
            free(p);
            p = prox;
        }
    }
    free(t->tabela);
    t->tabela = NULL;
    t->m = t->n = 0;
}

/* ================================================================
 * Sondagem linear
 * ================================================================ */

void linInicializar(TabelaLinear *t, int m) {
    t->m      = m;
    t->n      = 0;
    t->tabela = malloc(m * sizeof(Slot));
    for (int i = 0; i < m; i++)
        t->tabela[i].estado = VAZIO;
}

int linInserir(TabelaLinear *t, int chave, int valor) {
    int base    = chave % t->m;
    int pri_del = -1;
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i) % t->m;
        if (t->tabela[pos].estado == VAZIO) {
            int dest;
            if (pri_del != -1)
                dest = pri_del;
            else
                dest = pos;
            t->tabela[dest].chave  = chave;
            t->tabela[dest].valor  = valor;
            t->tabela[dest].estado = OCUPADO;
            t->n++;
            return 1;
        }
        if (t->tabela[pos].estado == DELETADO && pri_del == -1)
            pri_del = pos;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave)
            return 0;   /* chave duplicada */
    }
    if (pri_del != -1) {
        t->tabela[pri_del].chave  = chave;
        t->tabela[pri_del].valor  = valor;
        t->tabela[pri_del].estado = OCUPADO;
        t->n++;
        return 1;
    }
    return 0;   /* tabela cheia */
}

int linBuscar(const TabelaLinear *t, int chave) {
    int base = chave % t->m;
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i) % t->m;
        if (t->tabela[pos].estado == VAZIO)
            return -1;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave)
            return t->tabela[pos].valor;
    }
    return -1;
}

int linRemover(TabelaLinear *t, int chave) {
    int base = chave % t->m;
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i) % t->m;
        if (t->tabela[pos].estado == VAZIO)
            return 0;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave) {
            t->tabela[pos].estado = DELETADO;
            t->n--;
            return 1;
        }
    }
    return 0;
}

void linDestruir(TabelaLinear *t) {
    free(t->tabela);
    t->tabela = NULL;
    t->m = t->n = 0;
}

/* ================================================================
 * Sondagem quadratica
 * ================================================================ */

void quadInicializar(TabelaQuad *t, int m) {
    t->m      = m;
    t->n      = 0;
    t->tabela = malloc(m * sizeof(Slot));
    for (int i = 0; i < m; i++)
        t->tabela[i].estado = VAZIO;
}

int quadInserir(TabelaQuad *t, int chave, int valor) {
    int base    = chave % t->m;
    int pri_del = -1;
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i * i) % t->m;
        if (t->tabela[pos].estado == VAZIO) {
            int dest;
            if (pri_del != -1)
                dest = pri_del;
            else
                dest = pos;
            t->tabela[dest].chave  = chave;
            t->tabela[dest].valor  = valor;
            t->tabela[dest].estado = OCUPADO;
            t->n++;
            return 1;
        }
        if (t->tabela[pos].estado == DELETADO && pri_del == -1)
            pri_del = pos;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave)
            return 0;
    }
    if (pri_del != -1) {
        t->tabela[pri_del].chave  = chave;
        t->tabela[pri_del].valor  = valor;
        t->tabela[pri_del].estado = OCUPADO;
        t->n++;
        return 1;
    }
    return 0;
}

int quadBuscar(const TabelaQuad *t, int chave) {
    int base = chave % t->m;
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i * i) % t->m;
        if (t->tabela[pos].estado == VAZIO)
            return -1;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave)
            return t->tabela[pos].valor;
    }
    return -1;
}

int quadRemover(TabelaQuad *t, int chave) {
    int base = chave % t->m;
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i * i) % t->m;
        if (t->tabela[pos].estado == VAZIO)
            return 0;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave) {
            t->tabela[pos].estado = DELETADO;
            t->n--;
            return 1;
        }
    }
    return 0;
}

void quadDestruir(TabelaQuad *t) {
    free(t->tabela);
    t->tabela = NULL;
    t->m = t->n = 0;
}

/* ================================================================
 * Hash duplo
 * ================================================================ */

void dblInicializar(TabelaDouble *t, int m) {
    t->m      = m;
    t->n      = 0;
    t->tabela = malloc(m * sizeof(Slot));
    for (int i = 0; i < m; i++)
        t->tabela[i].estado = VAZIO;
}

int dblInserir(TabelaDouble *t, int chave, int valor) {
    int base    = chave % t->m;
    int passo   = 1 + (chave % (t->m - 2));
    int pri_del = -1;
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i * passo) % t->m;
        if (t->tabela[pos].estado == VAZIO) {
            int dest;
            if (pri_del != -1)
                dest = pri_del;
            else
                dest = pos;
            t->tabela[dest].chave  = chave;
            t->tabela[dest].valor  = valor;
            t->tabela[dest].estado = OCUPADO;
            t->n++;
            return 1;
        }
        if (t->tabela[pos].estado == DELETADO && pri_del == -1)
            pri_del = pos;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave)
            return 0;
    }
    if (pri_del != -1) {
        t->tabela[pri_del].chave  = chave;
        t->tabela[pri_del].valor  = valor;
        t->tabela[pri_del].estado = OCUPADO;
        t->n++;
        return 1;
    }
    return 0;
}

int dblBuscar(const TabelaDouble *t, int chave) {
    int base  = chave % t->m;
    int passo = 1 + (chave % (t->m - 2));
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i * passo) % t->m;
        if (t->tabela[pos].estado == VAZIO)
            return -1;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave)
            return t->tabela[pos].valor;
    }
    return -1;
}

int dblRemover(TabelaDouble *t, int chave) {
    int base  = chave % t->m;
    int passo = 1 + (chave % (t->m - 2));
    for (int i = 0; i < t->m; i++) {
        int pos = (base + i * passo) % t->m;
        if (t->tabela[pos].estado == VAZIO)
            return 0;
        if (t->tabela[pos].estado == OCUPADO && t->tabela[pos].chave == chave) {
            t->tabela[pos].estado = DELETADO;
            t->n--;
            return 1;
        }
    }
    return 0;
}

void dblDestruir(TabelaDouble *t) {
    free(t->tabela);
    t->tabela = NULL;
    t->m = t->n = 0;
}
