//lista encadeada da receita
#include <stdio.h>
#include <stdlib.h>
#include "montagem.h"
#include "tipos.h"

void inicializar(ListaDuplaEnc *lista) {
    lista->ini = ing_pao_inferior;
    lista->fim = ing_pao_superior;
}

void imprimir(ListaDuplaEnc *lista) {
    NodoD *aux = lista->ini;

    while (aux != ing_pao_superior) {
        printf("%d - %s - %.2f\n", aux->dado.cod, aux->dado.nome, aux->dado.preco);
        aux = aux->prox;
    }
}

void imprimirInverso(ListaDuplaEnc *lista) {
    NodoD *aux = lista->fim;

    while (aux != ing_pao_inferior) {
        printf("%d - %s - %.2f\n", aux->dado.cod, aux->dado.nome, aux->dado.preco);
        aux = aux->ant;
    }
}

Produto acessar(ListaDuplaEnc *lista, int cod) {
    NodoD *aux = lista->ini;
    Produto prod = {-1, "", 0.0f};

    while (aux != ing_pao_superior) {
        if (aux->dado.cod == cod)
            return aux->dado;
        aux = aux->prox;
    }

    return prod;
}

int inserirInicio(ListaDuplaEnc *lista, IdIngrediente id) {
    NodoD *novo = (NodoD*) malloc(sizeof(NodoD));
    if (novo == NULL)
        return 0;

    novo->dado = id;
    novo->ant = NULL;
    novo->prox = lista->ini;

    if (lista->ini != NULL)
        lista->ini->ant = novo; // o antigo início passa a ter um antecessor
    else
        lista->fim = novo;      // lista estava vazia: novo nodo também é o fim

    lista->ini = novo;
    return 1;
}

int inserirFim(ListaDuplaEnc *lista, IdIngrediente id) {
    NodoD *novo = (NodoD*) malloc(sizeof(NodoD));
    if (novo == NULL)
        return 0;

    novo->dado = id;
    novo->prox = NULL;
    novo->ant = lista->fim;

    if (lista->fim != NULL)
        lista->fim->prox = novo; // o antigo fim passa a apontar para o novo nodo
    else
        lista->ini = novo;       // lista estava vazia: novo nodo também é o início

    lista->fim = novo;
    return 1;
}

int removerPorCod(ListaDuplaEnc *lista, int cod) {
    NodoD *aux = lista->ini;

    while (aux != NULL && aux->dado.cod != cod)
        aux = aux->prox;

    if (aux == NULL) // não encontrado
        return 0;

    if (aux->ant != NULL) // existe antecessor: religa por ele
        aux->ant->prox = aux->prox;
    else                  // removendo o próprio início
        lista->ini = aux->prox;

    if (aux->prox != NULL) // existe sucessor: religa por ele
        aux->prox->ant = aux->ant;
    else                   // removendo o próprio fim
        lista->fim = aux->ant;

    free(aux);
    return 1;
}

void destruir(ListaDuplaEnc *lista) {
    NodoD *ant;
    NodoD *aux = lista->ini;

    while (aux != NULL) {
        ant = aux;
        aux = aux->prox;
        free(ant);
    }

    lista->ini = NULL;
    lista->fim = NULL;
}

int tamanho(ListaDuplaEnc *lista) {
    NodoD *aux = lista->ini;
    int contador = 0;

    while (aux != NULL) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

// Funções a implementar
void imprimirPelasPontas(ListaDuplaEnc *lista) {
    NodoD *ini, *fim;
    ini = lista->ini;
    fim = lista->fim;

    if(ini == NULL) {
        return;
    }

    while(ini->ant != fim) {
        printf("%d - %s - %.2f\n", ini->dado.cod, ini->dado.nome, ini->dado.preco);

        // if abaixo para o bônus: impede de imprimir duplicado.
        if(ini == fim) {
            return;
        }

        printf("%d - %s - %.2f\n", fim->dado.cod, fim->dado.nome, fim->dado.preco);

        ini = ini->prox;
        fim = fim->ant;
    }
}

void inverter(ListaDuplaEnc *lista) {
    NodoD *aux = lista->ini;
    NodoD *temp;

    if(aux == NULL) {
        return;
    }

    while(aux != NULL) {
        temp = aux->ant;
        aux->ant = aux->prox;
        aux->prox = temp;
        aux = aux->ant;
    }

    temp = lista->ini;
    lista->ini = lista->fim;
    lista->fim = temp;
}
