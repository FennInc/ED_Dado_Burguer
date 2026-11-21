#include <stdio.h>
#include "config.h"
#include "estoque.h"

/*
 * Teste interativo do estoque completo (spec, secao 4.7):
 *   pereciveis (heap minimo cada): n = carne, o = ovo frito, b = bacon
 *   perenes (contador):            s = queijo suico, q = queijo cheddar,
 *                                  c = cebola, t = tomate
 *
 * Comandos (preparar e consumir gastam 1 turno; consultar nao gasta):
 *   p X     preparar o ingrediente X
 *   c X     consumir o ingrediente X
 *   e       ver o estoque de todos
 *   i X N   inserir o vencimento N no heap de X (so pereciveis, so para teste)
 *   j N     pular N turnos sem agir (so para teste)
 *   x       sair
 */

int main(void) {
    int turno = 1;
    char op, ing;
    int n;

    Estoque est;
    estoqueInicializar(&est);

    printf("Teste do estoque. Comandos: p X | c X | e | i X N | j N | x\n");
    printf("Ingredientes: n o b (pereciveis) | s q c t (perenes)\n");

    while (1) {
        /* Inicio do turno, antes de qualquer acao do jogador */
        int descartados = estoqueLimparVencidos(&est, turno);
        if (descartados > 0) {
            printf("Descartados por validade: %d\n", descartados);
        }

        printf("\n[turno %d] > ", turno);
        if (scanf(" %c", &op) != 1 || op == 'x') {
            break;
        }

        switch (op) {
        case 'p':
            if (scanf(" %c", &ing) != 1) break;
            {
                int venc = -1;
                int res = estoquePreparar(&est, ing, turno, &venc);
                if (res == 1) {
                    if (estoqueEhPerecivel(ing)) {
                        printf("Preparou %s (vence no turno %d).\n",
                               estoqueNomeIngrediente(ing), venc);
                    } else {
                        printf("Preparou %s.\n", estoqueNomeIngrediente(ing));
                    }
                    turno++;
                } else if (res == 0) {
                    printf("Estoque cheio! Nao foi possivel preparar %s.\n",
                           estoqueNomeIngrediente(ing));
                } else {
                    printf("Ingrediente invalido.\n");
                }
            }
            break;

        case 'c':
            if (scanf(" %c", &ing) != 1) break;
            {
                int venc = -1;
                int res = estoqueConsumir(&est, ing, &venc);
                if (res == 1) {
                    if (estoqueEhPerecivel(ing)) {
                        printf("Consumiu %s (vencimento %d).\n",
                               estoqueNomeIngrediente(ing), venc);
                    } else {
                        printf("Consumiu %s.\n", estoqueNomeIngrediente(ing));
                    }
                    turno++;
                } else if (res == 0) {
                    printf("Sem estoque de %s.\n", estoqueNomeIngrediente(ing));
                } else {
                    printf("Ingrediente invalido.\n");
                }
            }
            break;

        case 'e':
            printf("--- Pereciveis ---\n");
            printf("%s: %d\n", estoqueNomeIngrediente('n'), estoqueQuantidade(&est, 'n'));
            printf("%s: %d\n", estoqueNomeIngrediente('o'), estoqueQuantidade(&est, 'o'));
            printf("%s: %d\n", estoqueNomeIngrediente('b'), estoqueQuantidade(&est, 'b'));
            printf("--- Perenes ---\n");
            printf("%s: %d\n", estoqueNomeIngrediente('s'), estoqueQuantidade(&est, 's'));
            printf("%s: %d\n", estoqueNomeIngrediente('q'), estoqueQuantidade(&est, 'q'));
            printf("%s: %d\n", estoqueNomeIngrediente('c'), estoqueQuantidade(&est, 'c'));
            printf("%s: %d\n", estoqueNomeIngrediente('t'), estoqueQuantidade(&est, 't'));
            break; /* consulta: nao consome turno */

        case 'i':
            if (scanf(" %c %d", &ing, &n) == 2 && estoqueEhPerecivel(ing)) {
                if (estoqueInserirVencimento(&est, ing, n) == 1) {
                    printf("Inserido o vencimento %d em %s.\n",
                           n, estoqueNomeIngrediente(ing));
                } else {
                    printf("Nao foi possivel inserir o vencimento em %s (estoque cheio)!\n",
                           estoqueNomeIngrediente(ing));
                }
            } else {
                printf("Uso: i X N (X = n, o ou b).\n");
            }
            break;

        case 'j':
            if (scanf("%d", &n) == 1 && n > 0) {
                turno += n;
                printf("Pulou %d turnos.\n", n);
            }
            break;

        default:
            printf("Comando invalido.\n");
            break;
        }
    }

    return 0;
}