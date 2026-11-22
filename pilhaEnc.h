#ifndef PILHAENC_H
#define PILHAENC_H

#include "tipos.h"
#include <stdbool.h>

typedef struct {
    int cod;
    char nome[50];
    float preco;
} Produto;

typedef struct {
    NoIngrediente *topo;
    int quantidade;
} PilhaBurger;

void inicializaPilha(PilhaBurger *p);
int estaVaziaPilha(PilhaBurger *p);
int empilha_ingrediente(PilhaBurger *p, IdIngrediente id);
bool desempilha_ingrediente(PilhaBurger *p, IdIngrediente *id_removido);
int topo(PilhaBurger *p, IdIngrediente *id_topo);
int tamanhoPilha(PilhaBurger *p);
bool validar_hamburguer(const PilhaBurger *pilha, const Pedido *pedido);

#endif