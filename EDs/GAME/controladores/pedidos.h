#ifndef PEDIDOS_H
#define PEDIDOS_H

#include "avl.h"
#include "hash.h"
#include "config.h"

int pedidosObterReceita(NodoAvl *raiz, TabelaEnc *cache, int menor, int maior,
                        char receita[CFG_RECEITA_TAM], int *codigoSorteado);

#endif /* PEDIDOS_H */