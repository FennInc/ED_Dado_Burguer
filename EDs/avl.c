#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

int g_rotacoes = 0; // trem global de rotações
int g_comparacoes = 0; // trem global de comparações
int g_buscas = 0; //global de buscas
double g_tempo_buscas = 0.0; //global de tempo de buscas

/* Retorna -1 para NULL (convenção: folha=0, NULL=-1).
   Necessário para que 1 + max(h_esq, h_dir) funcione corretamente. */
int avlAltura(const NodoAvl *no) {
    if (no == NULL) return -1;
    return no->altura;
}

/* FB = h(esq) - h(dir). Positivo = pesado à esq; negativo = pesado à dir. */
int avlFb(const NodoAvl *no) {
    if (no == NULL) return 0;
    return avlAltura(no->esq) - avlAltura(no->dir);
}

/* Recalcula e armazena a altura de 'no' com base nas alturas dos filhos.
   Chamado sempre que um filho muda (inserção, remoção ou rotação). */
void avlAtualizarAltura(NodoAvl *no) {
    int ae = avlAltura(no->esq);
    int ad = avlAltura(no->dir);
    if (ae > ad)
        no->altura = 1 + ae;
    else
        no->altura = 1 + ad;
}

/* Rotação simples à DIREITA (caso LL).
   x sobe; B (filho dir de x) passa para filho esq de y.
   Atualiza y primeiro (ficou abaixo), depois x. */
NodoAvl* avlRotDir(NodoAvl *y) {
    g_rotacoes++; // Incrementa a cada rotação
    NodoAvl *x = y->esq;
    NodoAvl *B = x->dir;
    x->dir = y;
    y->esq = B;
    avlAtualizarAltura(y);
    avlAtualizarAltura(x);
    return x;
}

NodoAvl* avlRotEsq(NodoAvl *x) {
    g_rotacoes++; // Incrementa a cada rotação
    NodoAvl *y = x->dir;
    NodoAvl *B = y->esq;
    y->esq = x;
    x->dir = B;
    avlAtualizarAltura(x);
    avlAtualizarAltura(y);
    return y;
}

/* Verifica o FB e aplica a rotação correta.
   Usada tanto na inserção quanto na remoção. */
NodoAvl* avlReequilibrar(NodoAvl *raiz) {
    avlAtualizarAltura(raiz);
    int fb = avlFb(raiz);

    /* LL: pesado à esq, filho esq também pesado à esq ou equilibrado */
    if (fb == +2 && avlFb(raiz->esq) >= 0)
        return avlRotDir(raiz);

    /* RR: pesado à dir, filho dir também pesado à dir ou equilibrado */
    if (fb == -2 && avlFb(raiz->dir) <= 0)
        return avlRotEsq(raiz);

    /* LR: pesado à esq, filho esq pesado à dir → transforma LR em LL */
    if (fb == +2 && avlFb(raiz->esq) < 0) {
        raiz->esq = avlRotEsq(raiz->esq);
        return avlRotDir(raiz);
    }

    /* RL: pesado à dir, filho dir pesado à esq → transforma RL em RR */
    if (fb == -2 && avlFb(raiz->dir) > 0) {
        raiz->dir = avlRotDir(raiz->dir);
        return avlRotEsq(raiz);
    }

    return raiz;   /* sem desequilíbrio */
}

/* Busca: idêntica à Abp — a altura garantida torna-a O(log n). */
//incrementa a cada comparação
NodoAvl* avlBuscar(NodoAvl *raiz, int cod) {
    if (raiz == NULL)           return NULL;
    if (cod == raiz->dado.cod){ g_comparacoes++; return raiz; }
    if (cod < raiz->dado.cod){ g_comparacoes++; return avlBuscar(raiz->esq, cod); }
    g_comparacoes++;
    return avlBuscar(raiz->dir, cod);
}

/* Inserção: descida igual à Abp; na subida, reequilibra cada ancestral.
   No máximo uma rotação por inserção.
   Uso: raiz = avlInserir(raiz, Pedido); */
NodoAvl* avlInserir(NodoAvl *raiz, Pedido valor) {
    if (raiz == NULL) {
        NodoAvl *novo = (NodoAvl*) malloc(sizeof(NodoAvl));
        novo->dado   = valor;
        novo->esq    = NULL;
        novo->dir    = NULL;
        novo->altura = 0;
        return novo;
    }
    if (valor.cod < raiz->dado.cod)
        raiz->esq = avlInserir(raiz->esq, valor);
    else if (valor.cod > raiz->dado.cod)
        raiz->dir = avlInserir(raiz->dir, valor);
    else
        return raiz;   /* duplicado: ignora */
    return avlReequilibrar(raiz);
}

/* Auxiliar: menor nó da subárvore (sucessor em-ordem). */
NodoAvl* avlMinimo(NodoAvl *raiz) {
    while (raiz->esq != NULL) raiz = raiz->esq;
    return raiz;
}

/* Remoção: mesmos três casos da Abp; na subida, reequilibra cada ancestral.
   Podem ser necessárias múltiplas rotações (uma por nível desequilibrado).
   Uso: raiz = avlRemover(raiz, cod); */
NodoAvl* avlRemover(NodoAvl *raiz, int cod) {
    if (raiz == NULL) return NULL;

    if (cod < raiz->dado.cod) {
        raiz->esq = avlRemover(raiz->esq, cod);
    } else if (cod > raiz->dado.cod) {
        raiz->dir = avlRemover(raiz->dir, cod);
    } else {
        if (raiz->esq == NULL) {
            NodoAvl *filho = raiz->dir; free(raiz); return filho;
        }
        if (raiz->dir == NULL) {
            NodoAvl *filho = raiz->esq; free(raiz); return filho;
        }
        /* dois filhos: substitui pelo sucessor em-ordem */
        NodoAvl *sucessor = avlMinimo(raiz->dir);
        raiz->dado = sucessor->dado;
        raiz->dir  = avlRemover(raiz->dir, sucessor->dado.cod);
    }
    return avlReequilibrar(raiz);
}

/* Destroi toda a árvore em pós-ordem.
   Uso: raiz = avlDestruir(raiz); */
NodoAvl* avlDestruir(NodoAvl *raiz) {
    if (raiz == NULL) return NULL;
    raiz->esq = avlDestruir(raiz->esq);
    raiz->dir = avlDestruir(raiz->dir);
    free(raiz);
    return NULL;
}

//trem de contar noh
int avlContarNos(const NodoAvl *raiz) {
    if (raiz == NULL) return 0;
    return 1 + avlContarNos(raiz->esq) + avlContarNos(raiz->dir);
}

void avlGravarMedicoes(const NodoAvl *raiz, const double tempo) {
    FILE *arquivo = fopen("medicoes.log", "a"); //adiciona no fim ao inves de sobreescrever, n sei qual dos dois deveria ser tho
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    int totalNos = avlContarNos(raiz);
    int alturaArv = avlAltura(raiz);

    // Grava as informações 
    fprintf(arquivo, "Nos: %d | Altura: %d | Rotacoes: %d | Tempo:%.2fs\n | Buscas: %d | Comparacoes: %d | Tempo de busca: %.2fs\n", totalNos, alturaArv+1, g_rotacoes, tempo, g_buscas, g_comparacoes, g_tempo_buscas);

    fclose(arquivo);
}

void emOrdem(const NodoAvl *raiz) {   // também chamado: central
    if (raiz == NULL) return;

    emOrdem(raiz->esq);               // 1. primeiro a esquerda
    printf("%d ", raiz->dado.cod);   // 2. visita a RAIZ (no meio)
    emOrdem(raiz->dir);               // 3. depois a direita
}