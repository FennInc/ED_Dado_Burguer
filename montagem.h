#ifndef LISTADUPLAENC_H
#define LISTADUPLAENC_H

typedef struct {
    int cod;
    char nome[50];
    float preco;
} Produto;

typedef struct str_NodoD NodoD;

struct str_NodoD {
    NodoD *ant;
    NodoD *prox;
    IdIngrediente id;
};

typedef struct {
    NodoD *ini;
    NodoD *fim;
} ListaDuplaEnc;

void    inicializar(ListaDuplaEnc *lista);
void    imprimir(ListaDuplaEnc *lista);
void    imprimirInverso(ListaDuplaEnc *lista);
Produto acessar(ListaDuplaEnc *lista, int cod);
int     inserirInicio(ListaDuplaEnc *lista, IdIngrediente id);
int     inserirFim(ListaDuplaEnc *lista, IdIngrediente id);
int     removerPorCod(ListaDuplaEnc *lista, int cod);
void    destruir(ListaDuplaEnc *lista);
int     tamanho(ListaDuplaEnc *lista);

#endif
