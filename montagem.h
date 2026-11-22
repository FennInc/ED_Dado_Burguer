#ifndef MONTAGEM_H
#define MONTAGEM_H

#include "tipos.h"

typedef struct str_NodoD NodoD;
struct str_NodoD {
    NodoD *ant;
    NodoD *prox;
    IdIngrediente id;
};

typedef struct {
    NodoD *ini;
    NodoD *fim;
} ListaDuplaEnc;

void inicializar(ListaDuplaEnc *lista);
void imprimir(const ListaDuplaEnc *lista);
void imprimirInverso(const ListaDuplaEnc *lista);
IdIngrediente buscar(const ListaDuplaEnc *lista, int cod);
int inserirInicio(ListaDuplaEnc *lista, IdIngrediente ing);
int inserirFim(ListaDuplaEnc *lista, IdIngrediente ing);
int removerPorCod(ListaDuplaEnc *lista, int cod);
void destruir(ListaDuplaEnc *lista);
int tamanho(ListaDuplaEnc *lista);

#endif
