#include <stddef.h>
#include "config.h"
#include "estoque.h"

void estoqueInicializar(Estoque *est) {
    if (est == NULL) return;

    est->carne.tam = 0;
    est->ovo.tam = 0;
    est->bacon.tam = 0;

    for (int i = 0; i < PERENE_QTD; i++) {
        est->perenes[i] = 0;
    }
}

int estoqueEhPerecivel(char ing) {
    return ing == 'n' || ing == 'o' || ing == 'b';
}

int estoqueIndicePerene(char ing) {
    switch (ing) {
    case 's': return PERENE_QUEIJO_SUICO;
    case 'q': return PERENE_QUEIJO_CHEDDAR;
    case 'c': return PERENE_CEBOLA;
    case 't': return PERENE_TOMATE;
    default:  return -1;
    }
}

const char *estoqueNomeIngrediente(char ing) {
    switch (ing) {
    case 'n': return "carne";
    case 'o': return "ovo frito";
    case 'b': return "bacon";
    case 's': return "queijo suico";
    case 'q': return "queijo cheddar";
    case 'c': return "cebola";
    case 't': return "tomate";
    default:  return "?";
    }
}

int estoqueValidade(char ing) {
    switch (ing) {
    case 'n': return CFG_VALIDADE_CARNE;
    case 'o': return CFG_VALIDADE_OVO;
    case 'b': return CFG_VALIDADE_BACON;
    default:  return 0;
    }
}

FilaPrioridade *estoqueHeapPerecivel(Estoque *est, char ing) {
    if (est == NULL) return NULL;

    switch (ing) {
    case 'n': return &est->carne;
    case 'o': return &est->ovo;
    case 'b': return &est->bacon;
    default:  return NULL;
    }
}

int estoqueLimparVencidos(Estoque *est, int turnoAtual) {
    if (est == NULL) return 0;

    return fpDescartarVencidos(&est->carne, turnoAtual)
         + fpDescartarVencidos(&est->ovo, turnoAtual)
         + fpDescartarVencidos(&est->bacon, turnoAtual);
}

int estoquePreparar(Estoque *est, char ing, int turnoAtual, int *outVencimento) {
    if (est == NULL) return -1;

    if (estoqueEhPerecivel(ing)) {
        FilaPrioridade *h = estoqueHeapPerecivel(est, ing);
        if (h == NULL || h->tam >= CAP) {
            return 0;
        }
        int venc = turnoAtual + estoqueValidade(ing);
        fpInserir(h, venc);
        if (outVencimento != NULL) {
            *outVencimento = venc;
        }
        return 1;
    }

    int idx = estoqueIndicePerene(ing);
    if (idx >= 0) {
        est->perenes[idx]++;
        if (outVencimento != NULL) {
            *outVencimento = -1;
        }
        return 1;
    }

    return -1;
}

int estoqueConsumir(Estoque *est, char ing, int *outVencimento) {
    if (est == NULL) return -1;

    if (estoqueEhPerecivel(ing)) {
        FilaPrioridade *h = estoqueHeapPerecivel(est, ing);
        int venc = fpRemoverMinimo(h);
        if (venc < 0) {
            return 0;
        }
        if (outVencimento != NULL) {
            *outVencimento = venc;
        }
        return 1;
    }

    int idx = estoqueIndicePerene(ing);
    if (idx >= 0) {
        if (est->perenes[idx] <= 0) {
            return 0;
        }
        est->perenes[idx]--;
        if (outVencimento != NULL) {
            *outVencimento = -1;
        }
        return 1;
    }

    return -1;
}

int estoqueInserirVencimento(Estoque *est, char ing, int vencimento) {
    if (est == NULL || !estoqueEhPerecivel(ing)) return -1;

    FilaPrioridade *h = estoqueHeapPerecivel(est, ing);
    if (h == NULL || h->tam >= CAP) {
        return 0;
    }

    fpInserir(h, vencimento);
    return 1;
}

int estoqueQuantidade(const Estoque *est, char ing) {
    if (est == NULL) return -1;

    if (estoqueEhPerecivel(ing)) {
        const FilaPrioridade *h = estoqueHeapPerecivel((Estoque *)est, ing);
        return (h != NULL) ? h->tam : -1;
    }

    int idx = estoqueIndicePerene(ing);
    if (idx >= 0) {
        return est->perenes[idx];
    }

    return -1;
}

