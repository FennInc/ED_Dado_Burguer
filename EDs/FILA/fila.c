#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "tipos.h"
#include "fila.h"

void inicializa_FilaClientes(FilaClientes *f) {
    f->frente = NULL;
    f->final = NULL;
}

int estaVazia_FilaClientes(FilaClientes *f) {
    return f->frente == NULL;
}

int enfileira_FilaClientes(FilaClientes *f, Cliente cliente) {
    NoCliente *novo = (NoCliente*) malloc(sizeof(NoCliente));
    if (novo == NULL)
        return 0;

    novo->dados = cliente;
    novo->prox = NULL;

    if (f->final != NULL)
        f->final->prox = novo; // o antigo final passa a apontar para o novo nodo
    else
        f->frente = novo;      // fila estava vazia: novo nodo também é a frente

    f->final = novo;
    f->tamanho++;
    return 1;
}

int desenfileira_FilaClientes(FilaClientes *f, Cliente *Removido) {
    NoCliente *removido;

    if (estaVazia_FilaClientes(f)) // underflow
        return 0;

    removido = f->frente;
    *Removido = removido->dados;
    f->frente = removido->prox;

    if (f->frente == NULL) // a fila ficou vazia
        f->final = NULL;

    free(removido);
    return 1;
}

int frente(FilaClientes *f, Cliente *cliente) {
    if(estaVazia_FilaClientes(f))
        return 0;
    
    *cliente = f->frente->dados;
    return 1;
}

Cliente* cliente_atual(FilaClientes *f){

return f->frente != NULL ? &(f->frente->dados) : NULL; //ponteiro para decrementar paciencia_atual a cada turno sem precisar remover o client da fila.
}

