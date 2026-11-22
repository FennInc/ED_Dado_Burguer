#include <stdio.h>
#include "sorteio.h"

/* Teste do sorteio: cobertura do intervalo e reprodutibilidade com a semente. */

#define MIN       1
#define MAX       837253
#define SORTEIOS  100000
#define SEMENTE   42
#define AMOSTRA   5

int main(void) {
    int menor = MAX, maior = MIN;
    int acimaDe32767 = 0, foraDoIntervalo = 0;
    int primeiros[AMOSTRA], repetidos[AMOSTRA];

    sorteioIniciar(SEMENTE);
    for (int i = 0; i < SORTEIOS; i++) {
        int v = sortearIntervalo(MIN, MAX);
        if (v < menor) menor = v;
        if (v > maior) maior = v;
        if (v > 32767) acimaDe32767++;
        if (v < MIN || v > MAX) foraDoIntervalo++;
        if (i < AMOSTRA) primeiros[i] = v;
    }

    printf("Sorteios: %d no intervalo [%d, %d]\n", SORTEIOS, MIN, MAX);
    printf("Menor valor sorteado: %d\n", menor);
    printf("Maior valor sorteado: %d\n", maior);
    printf("Acima de 32767: %d\n", acimaDe32767);
    printf("Fora do intervalo: %d\n", foraDoIntervalo);

    /* Mesma semente: a sequencia deve se repetir. */
    sorteioIniciar(SEMENTE);
    int iguais = 1;
    for (int i = 0; i < AMOSTRA; i++) {
        repetidos[i] = sortearIntervalo(MIN, MAX);
        if (repetidos[i] != primeiros[i]) iguais = 0;
    }
    printf("Primeiros %d valores: ", AMOSTRA);
    for (int i = 0; i < AMOSTRA; i++) printf("%d ", primeiros[i]);
    printf("\nMesma semente reproduz a sequencia: %s\n", iguais ? "sim" : "nao");

    /* Intervalos pequenos (numero de clientes). */
    int vistos[8] = {0};
    for (int i = 0; i < 1000; i++) {
        int c = sortearIntervalo(3, 7);
        if (c >= 3 && c <= 7) vistos[c]++;
    }
    printf("Clientes de 3 a 7 (1000 sorteios): ");
    for (int c = 3; c <= 7; c++) printf("%d:%d ", c, vistos[c]);
    printf("\n");

    return 0;
}