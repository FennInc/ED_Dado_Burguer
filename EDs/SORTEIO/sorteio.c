//IA foi utilizada para gerar parte da lógica de sorteio. toda a utilização dessas funções
//dentro do fluxo do código foi feito por mim, sophy, manualmente.
//seus comentários são sinalizados pelo uso de '/* */'

#include <stdlib.h>
#include "sorteio.h"

#define SORTEIO_BITS_POR_CHAMADA 15
#define SORTEIO_MASCARA          0x7FFF /* 15 bits: sempre cabe no rand() */

/* Valor de 30 bits formado por duas chamadas do rand(). */
static unsigned long sorteioBase(void) {
    unsigned long alto  = (unsigned long)(rand() & SORTEIO_MASCARA);
    unsigned long baixo = (unsigned long)(rand() & SORTEIO_MASCARA);
    return (alto << SORTEIO_BITS_POR_CHAMADA) | baixo;
}

void sorteioIniciar(unsigned int semente) {
    srand(semente);
}

int sortearIntervalo(int min, int max) {
    if (min > max) {
        int tmp = min;
        min = max;
        max = tmp;
    }

    unsigned long tamanho = (unsigned long)((long)max - (long)min + 1);
    return (int)((long)min + (long)(sorteioBase() % tamanho));
}