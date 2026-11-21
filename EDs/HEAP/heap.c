#include <stddef.h>
#include "heap.h"

/* Restaura a propriedade do min-heap descendo o no i. */
void heapify(int a[], int n, int i) {
    int menor = i;
    int esq   = 2 * i + 1;
    int dir   = 2 * i + 2;

    if (esq < n && a[esq] < a[menor]) menor = esq;
    if (dir < n && a[dir] < a[menor]) menor = dir;

    if (menor != i) {
        int tmp = a[i];
        a[i] = a[menor];
        a[menor] = tmp;
        heapify(a, n, menor);
    }
}

/* Restaura a propriedade do min-heap subindo o no i. */
void siftUp(int a[], int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (a[i] < a[pai]) {
            int tmp = a[i];
            a[i] = a[pai];
            a[pai] = tmp;
            i = pai;
        } else {
            break;
        }
    }
}

void fpInserir(FilaPrioridade *fp, int valor) {
    if (fp == NULL || fp-> tam >= CAP) return;
    fp->dados[fp->tam] = valor;
    siftUp(fp->dados, fp->tam);
    fp->tam++;
}

int fpMinimo(FilaPrioridade *fp) {
    if (fp == NULL || fp->tam <= 0) return -1;
    return fp->dados[0];
}

int fpRemoverMinimo(FilaPrioridade *fp) {
    if (fp == NULL || fp->tam <= 0) return -1;
    int min = fp->dados[0];
    fp->tam--;
    if (fp->tam > 0) {
        fp->dados[0] = fp->dados[fp->tam];
        heapify(fp->dados, fp->tam, 0);
    }
    return min;
}

int estoqueDescartarVencidos(FilaPrioridade *fp, int turnoAtual) {
    int descartados = 0;

    if (fp == NULL) return 0;

    while (fp->tam > 0 && fp->dados[0] <= turnoAtual) {
        fp->tam--;
        if (fp->tam > 0) {
            fp->dados[0] = fp->dados[fp->tam];
            heapify(fp->dados, fp->tam, 0);
        }
        descartados++;
    }
    return descartados;
}