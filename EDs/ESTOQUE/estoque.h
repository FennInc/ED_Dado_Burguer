#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "heap.h"

//indices dos vetores perenes pra ficar bonitinho
enum {
    PERENE_QUEIJO_SUICO = 0,
    PERENE_QUEIJO_CHEDDAR = 1,
    PERENE_CEBOLA = 2,
    PERENE_TOMATE = 3,
    PERENE_QTD = 4
};

//estoque, basicamente 4 vetores juntos
typedef struct {
    FilaPrioridade carne;
    FilaPrioridade ovo;
    FilaPrioridade bacon;
    int perenes[PERENE_QTD]; // s, q, c, t
} Estoque;

void estoqueInicializar(Estoque *est);

int estoqueEhPerecivel(char ing);
int estoqueIndicePerene(char ing);
const char *estoqueNomeIngrediente(char ing);
int estoqueValidade(char ing);

FilaPrioridade *estoqueHeapPerecivel(Estoque *est, char ing);

int estoqueLimparVencidos(Estoque *est, int turnoAtual);

int estoquePreparar(Estoque *est, char ing, int turnoAtual, int *outVencimento);
int estoqueConsumir(Estoque *est, char ing, int *outVencimento);
int estoqueInserirVencimento(Estoque *est, char ing, int vencimento);
int estoqueQuantidade(const Estoque *est, char ing);
int estoqueEhIlimitado(char ing);

#endif