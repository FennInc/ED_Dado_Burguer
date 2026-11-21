#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "heap.h"

/* Indices dos ingredientes perenes no vetor interno */
enum {
    PERENE_QUEIJO_SUICO = 0,
    PERENE_QUEIJO_CHEDDAR = 1,
    PERENE_CEBOLA = 2,
    PERENE_TOMATE = 3,
    PERENE_QTD = 4
};

typedef struct {
    FilaPrioridade carne;
    FilaPrioridade ovo;
    FilaPrioridade bacon;
    int perenes[PERENE_QTD]; /* s, q, c, t */
} Estoque;

void estoqueInicializar(Estoque *est);

int estoqueEhPerecivel(char ing);
int estoqueIndicePerene(char ing);
const char *estoqueNomeIngrediente(char ing);
int estoqueValidade(char ing);

/* Retorna o heap do ingrediente perecivel (n, o, b) ou NULL se nao for perecivel */
FilaPrioridade *estoqueHeapPerecivel(Estoque *est, char ing);

/* Descarta vencidos nos tres heaps para o turno atual; retorna total descartado */
int estoqueLimparVencidos(Estoque *est, int turnoAtual);

/* Retornos: 1 = ok, 0 = sem espaco / sem estoque, -1 = ingrediente invalido */
int estoquePreparar(Estoque *est, char ing, int turnoAtual, int *outVencimento);
int estoqueConsumir(Estoque *est, char ing, int *outVencimento);
int estoqueInserirVencimento(Estoque *est, char ing, int vencimento);
/* Retorna a quantidade atual em estoque do ingrediente, ou -1 se invalido */
int estoqueQuantidade(const Estoque *est, char ing);

#endif /* ESTOQUE_H */