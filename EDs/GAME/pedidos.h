#ifndef PEDIDOS_H
#define PEDIDOS_H

#include "avl.h"
#include "hash.h"

/*
 * pedidos.h - obtencao do pedido de cada cliente (spec, secoes 3.3 e 4.3).
 *
 * Fluxo de uma chamada:
 *   1) sorteia um codigo de pedido no intervalo [menor, maior];
 *   2) consulta a hash (cache);
 *   3) em caso de miss, busca na AVL e insere o resultado na hash;
 *   4) entrega a receita como string (por exemplo "nkmpz").
 *
 * As metricas de hits, misses, buscas, comparacoes e tempo da AVL sao
 * atualizadas pelo proprio fluxo, como na main de teste.
 *
 * Quem recebe a receita (string) converte para a lista encadeada do
 * pedido, se precisar.
 */

#define PEDIDOS_RECEITA_TAM 8 /* 7 caracteres da receita + '\0' */

/* Obtem o pedido do proximo cliente.
 *   raiz, cache      : AVL com os pedidos e tabela hash usada como cache
 *   menor, maior     : intervalo dos codigos existentes na AVL
 *   receita          : vetor de PEDIDOS_RECEITA_TAM posicoes que recebe a receita
 *   codigoSorteado   : recebe o codigo sorteado (pode ser NULL)
 * Retorna 1 em caso de sucesso e 0 se o codigo sorteado nao foi encontrado
 * (a receita fica vazia). */
int pedidosObterReceita(NodoAvl *raiz, TabelaEnc *cache, int menor, int maior,
                        char receita[PEDIDOS_RECEITA_TAM], int *codigoSorteado);

#endif /* PEDIDOS_H */