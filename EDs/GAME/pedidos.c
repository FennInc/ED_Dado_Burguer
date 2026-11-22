#include <stddef.h>
#include <string.h>
#include "pedidos.h"
#include "sorteio.h"
#include "timer.h"

//pedidos sendo sorteados aqui, integração entre a avl e a hash q eu bati a cabeça tentando fazer no arq original delas
int pedidosObterReceita(NodoAvl *raiz, TabelaEnc *cache, int menor, int maior, char receita[CFG_RECEITA_TAM], int *codigoSorteado) {
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

    //busca na hash
    NoHash *n = encBuscar(cache, cod);
    if (n != NULL) {
        g_hits++;
        strncpy(receita, n->receita, CFG_RECEITA_TAM - 1);
        receita[CFG_RECEITA_TAM - 1] = '\0';
        return 1; //se achou yay
    }

    //se nao achou, vai para a avl
    NodoAvl *r = avlBuscar(raiz, cod);
    g_tempo_buscas += timer_parar(&tb);
    g_buscas++;
    g_miss++;

    if (r != NULL) {
        strncpy(receita, r->dado.receita, CFG_RECEITA_TAM - 1);
        receita[CFG_RECEITA_TAM - 1] = '\0';
        encInserir(cache, r->dado.cod, r->dado.receita);
        return 1;
    }

    receita[0] = '\0';
    return 0;
}