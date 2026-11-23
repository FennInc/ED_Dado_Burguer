#include <stdio.h>
#include <stdlib.h>
#include "pilhaEnc.h"
#include "tipos.h"
void inicializaPilha(PilhaBurger *p) {
    p->topo = NULL;
    p->quantidade = 0;
}

int estaVaziaPilha(PilhaBurger *p) {
    return p->topo == NULL;
}

int empilha_ingrediente(PilhaBurger *p, IdIngrediente id) {
    if (p->topo== NULL && id == ing_pao_inferior) {
        printf("Erro: o primeiro ingrediente deve ser o pão inferior.\n");
        return 0; 
    }
    NoIngrediente *novo = (NoIngrediente*) malloc(sizeof(NoIngrediente));
    if (novo == NULL)
        return 0;

    novo->id = id;
    novo->prox = p->topo; // aponta para o antigo topo
    p->topo = novo;        // o novo nodo passa a ser o topo
    p->quantidade++;
    return 1;
}
bool desempilha_ingrediente(PilhaBurger *p, IdIngrediente *id_removido) {
    if (estaVaziaPilha(p)) // underflow
        return false;
    NoIngrediente *aux = p->topo;
    *id_removido = aux->id;
    p->topo = aux->prox;
    free(aux);
    p->quantidade--;
    return true;
}

int topo(PilhaBurger *p, IdIngrediente *id_topo) {
    if (estaVaziaPilha(p))
        return 0;

    *id_topo = p->topo->id;
    return 1;
}

int tamanhoPilha(PilhaBurger *p) {
    NoIngrediente *aux = p->topo;
    int contador = 0;

    while (aux != NULL) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

static bool comparar_recursivo(NoIngrediente *no_pilha, NoIngrediente **no_receita) {
    if (no_pilha == NULL) {
        return true;
    }
    /* Vai até o fundo da pilha (pão inferior) primeiro */
    if (!comparar_recursivo(no_pilha->prox, no_receita)) {
        return false;
    }
    /* Na volta da recursão, compara de baixo para cima com a receita */
    if (*no_receita == NULL || no_pilha->id != (*no_receita)->id) {
        return false;
    }
    *no_receita = (*no_receita)->prox;
    return true;
}
// A função  abaixo valida o lanche inteiro em O(n) sem precisar clonar a pilha com malloc e sem risco de vazar memória.
bool validar_hamburguer(const PilhaBurger *pilha, const Pedido *pedido) {
    if (pilha->quantidade != pedido->qtd_ingredientes) {
        return false;
    }
    NoIngrediente *cursor_receita = pedido->receita_inicio;
    return comparar_recursivo(pilha->topo, &cursor_receita) && (cursor_receita == NULL);
}


//Limpa a pilha do lanche e monta a lista encadeada da receita atual.
//Empilha um ingrediente no lanche (LIFO) e retorna 1 se deu certo ou 0 se bloqueado/falhou.
//Desempilha o topo do lanche (LIFO), devolve o simbolo em outSimbolo e retorna 1 se removeu ou 0 se vazio.
// Compara a pilha montada com a receita do pedido e retorna 1 se estiver igual ou 0 se errado.