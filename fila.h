#ifndef FILAENC_H
#define FILAENC_H
#include "tipos.h"

typedef struct str_NoCliente NoCliente;
struct str_NoCliente {
    Cliente dados;
    NoCliente *prox;
};

typedef struct {
    NoCliente *frente;
    NoCliente *final;
    int tamanho;
} FilaClientes;

void inicializa_FilaClientes(FilaClientes *f);
int  estaVazia_FilaClientes(FilaClientes *f);
int  enfileira_FilaClientes(FilaClientes *f, Cliente valor);
int  desenfileira_FilaClientes(FilaClientes *f, Cliente *valorRemovido);
Cliente* cliente_atual(FilaClientes *f);
#endif