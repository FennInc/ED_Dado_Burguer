#ifndef SORTEIO_H
#define SORTEIO_H

/*
 * sorteio.h - sorteio de inteiros em um intervalo.
 *
 * O rand() do C garante apenas valores ate 32767 (RAND_MAX >= 32767), e no
 * Windows e exatamente esse o maximo. Para cobrir intervalos maiores, como
 * os codigos de pedido (centenas de milhares), este modulo combina duas
 * chamadas do rand() em um valor de 30 bits (0 a 1.073.741.823).
 */

/* Define a semente do gerador. Chamar uma vez, no inicio do programa. */
void sorteioIniciar(unsigned int semente);

/* Devolve um inteiro no intervalo [min, max], inclusive.
   Se min > max, os valores sao trocados. O tamanho do intervalo
   (max - min + 1) deve ser de ate 1.073.741.824. */
int sortearIntervalo(int min, int max);

#endif /* SORTEIO_H */