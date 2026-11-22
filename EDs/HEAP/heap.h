#ifndef HEAP_H
#define HEAP_H

#define CAP 10

typedef struct {
    int dados[CAP]; //guarda cada ingrediente da heap com sua validade no vetor (validade = cod)
    int tam;
} FilaPrioridade;

void heapify(int a[], int n, int i);
void siftUp(int a[], int i);

void fpInserir(FilaPrioridade *fp, int valor);
int fpMinimo(FilaPrioridade *fp);
int fpRemoverMinimo(FilaPrioridade *fp);
int fpDescartarVencidos(FilaPrioridade *fp, int turnoAtual);

#endif