//IA foi utilizada para gerar parte da lógica de sorteio. toda a utilização dessas funções
//dentro do fluxo do código foi feito por mim, sophy, manualmente.
//seus comentários são sinalizados pelo uso de '/* */'

#ifndef SORTEIO_H
#define SORTEIO_H

/*
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

#endif