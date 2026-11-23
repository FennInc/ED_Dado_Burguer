//lista encadeada da receita
#include <stdio.h>
#include <stdlib.h>
#include "montagem.h"
#include "tipos.h"

void inicializar(ListaDuplaEnc *lista) {
    lista->ini = NULL;
    lista->fim = NULL;
}

void imprimir(const ListaDuplaEnc *lista) {
    NodoD *aux = lista->ini;

    while (aux != NULL) {
        printf("%d\n", (int)aux->id);
        aux = aux->prox;
    }
}

void imprimirInverso(const ListaDuplaEnc *lista) {
    NodoD *aux = lista->fim;

    while (aux != NULL) {
        printf("%d\n", (int)aux->id);
        aux = aux->ant;
    }
}

IdIngrediente buscar(const ListaDuplaEnc *lista, int cod) {
    NodoD *aux = lista->ini;

    while (aux != NULL) {
        if ((int)aux->id == cod)
            return aux->id;
        aux = aux->prox;
    }

    return (IdIngrediente)0;
}

int inserirInicio(ListaDuplaEnc *lista, IdIngrediente ing) {
    NodoD *novo = malloc(sizeof *novo);
    if (novo == NULL)
        return 0;

    novo->id = ing;
    novo->ant = NULL;
    novo->prox = lista->ini;

    if (lista->ini != NULL)
        lista->ini->ant = novo;
    else
        lista->fim = novo;

    lista->ini = novo;
    return 1;
}

int inserirFim(ListaDuplaEnc *lista, IdIngrediente ing) {
    NodoD *novo = malloc(sizeof *novo);
    if (novo == NULL)
        return 0;

    novo->id = ing;
    novo->prox = NULL;
    novo->ant = lista->fim;

    if (lista->fim != NULL)
        lista->fim->prox = novo;
    else
        lista->ini = novo;

    lista->fim = novo;
    return 1;
}

int removerPorCod(ListaDuplaEnc *lista, int cod) {
    NodoD *aux = lista->ini;

    while (aux != NULL && (int)aux->id != cod)
        aux = aux->prox;

    if (aux == NULL)
        return 0;

    if (aux->ant != NULL)
        aux->ant->prox = aux->prox;
    else
        lista->ini = aux->prox;

    if (aux->prox != NULL)
        aux->prox->ant = aux->ant;
    else
        lista->fim = aux->ant;

    free(aux);
    return 1;
}

void destruir(ListaDuplaEnc *lista) {
    NodoD *aux = lista->ini;

    while (aux != NULL) {
        NodoD *prox = aux->prox;
        free(aux);
        aux = prox;
    }

    lista->ini = NULL;
    lista->fim = NULL;
}

int tamanho(ListaDuplaEnc *lista) {
    int contador = 0;
    NodoD *aux = lista->ini;

    while (aux != NULL) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}
