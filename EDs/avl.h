#ifndef AVL_H
#define AVL_H

/* ============================================================
   Estrutura do nó: igual ao NodoArv da Abp, mas com 'altura'.
   ============================================================ */
typedef struct{
    int cod;
    char receita[8];
}Pedido; //transformei o produto na formatação de pedido

typedef struct str_NodoAvl NodoAvl;

struct str_NodoAvl {
    Pedido  dado;
    NodoAvl *esq;
    NodoAvl *dir;
    int      altura;   /* folha = 0; NULL = -1 (por convenção) */
};

//DESGRAÇA DE CONTRATOS
extern int g_rotacoes;

int avlAltura(const NodoAvl *no);
int avlFb(const NodoAvl *no);
void avlAtualizarAltura(NodoAvl *no);

NodoAvl* avlRotDir(NodoAvl *y);
NodoAvl* avlRotEsq(NodoAvl *x);
NodoAvl* avlReequilibrar(NodoAvl *raiz);

NodoAvl* avlBuscar(NodoAvl *raiz, int cod);
NodoAvl* avlInserir(NodoAvl *raiz, Pedido valor);

NodoAvl* avlMinimo(NodoAvl *raiz);
NodoAvl* avlRemover(NodoAvl *raiz, int cod);

NodoAvl* avlDestruir(NodoAvl *raiz);

int avlContarNos(const NodoAvl *raiz);

void avlGravarMedicoes(const NodoAvl *raiz);

#endif