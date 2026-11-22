#include <stddef.h>
#include <string.h>
#include "pedidos.h"
#include "sorteio.h"
#include "timer.h"

int pedidosObterReceita(NodoAvl *raiz, TabelaEnc *cache, int menor, int maior,
                        char receita[PEDIDOS_RECEITA_TAM], int *codigoSorteado) {
    if (receita != NULL) {
        receita[0] = '\0';
    }

    if (raiz == NULL || cache == NULL || receita == NULL || menor > maior) {
        return 0;
    }

    int cod = sortearIntervalo(menor, maior);
    if (codigoSorteado != NULL) {
        *codigoSorteado = cod;
    }

    Timer tb;
    timer_iniciar(&tb);

    NoHash *n = encBuscar(cache, cod);
    if (n != NULL) {
        g_hits++;
        strncpy(receita, n->receita, PEDIDOS_RECEITA_TAM - 1);
        receita[PEDIDOS_RECEITA_TAM - 1] = '\0';
        return 1;
    }

    NodoAvl *r = avlBuscar(raiz, cod);
    g_tempo_buscas += timer_parar(&tb);
    g_buscas++;
    g_miss++;

    if (r != NULL) {
        strncpy(receita, r->dado.receita, PEDIDOS_RECEITA_TAM - 1);
        receita[PEDIDOS_RECEITA_TAM - 1] = '\0';
        encInserir(cache, r->dado.cod, r->dado.receita);
        return 1;
    }

    receita[0] = '\0';
    return 0;
}