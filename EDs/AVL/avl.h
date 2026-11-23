#ifndef AVL_H
#define AVL_H

/* ============================================================
   Estrutura do nó: igual ao NodoArv da Abp, mas com 'altura'.
   ============================================================ */
typedef struct{
    int cod;
    char receita[8];
}PedidoAVL; //transformei o produto na formatação de PedidoAVL

typedef struct str_NodoAvl NodoAvl;
struct str_NodoAvl { //da dando erro de compilação mas n acho que é nada
    PedidoAVL  dado;
    NodoAvl *esq;
    NodoAvl *dir;
    int      altura;   /* folha = 0; NULL = -1 (por convenção) */
};

//contratos
extern int g_rotacoes;
extern int g_buscas;
extern int g_comparacoes;
extern double g_tempo_buscas;

int avlAltura(const NodoAvl *no);
int avlFb(const NodoAvl *no);
void avlAtualizarAltura(NodoAvl *no);

NodoAvl* avlRotDir(NodoAvl *y);
NodoAvl* avlRotEsq(NodoAvl *x);
NodoAvl* avlReequilibrar(NodoAvl *raiz);

NodoAvl* avlBuscar(NodoAvl *raiz, int cod);
NodoAvl* avlInserir(NodoAvl *raiz, PedidoAVL valor);

NodoAvl* avlMinimo(NodoAvl *raiz);
NodoAvl* avlMaximo(NodoAvl *raiz);
NodoAvl* avlRemover(NodoAvl *raiz, int cod);

NodoAvl* avlDestruir(NodoAvl *raiz);

int avlContarNos(const NodoAvl *raiz);

#endif