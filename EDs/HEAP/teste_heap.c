#include <stdio.h>
#include "config.h"
#include "heap.h"

int main(void) {
    int turno = 1; /* turno atual; no jogo, o dono sera o motor de turnos */
    char op;
    int n;

    FilaPrioridade HeapCarne;
    HeapCarne.tam = 0;

    printf("Teste do heap de pereciveis (carne, validade %d turnos)\n",
           CFG_VALIDADE_CARNE);
    printf("Comandos: p | c | s | i N | j N | q\n");

    while (1) {
        /* Inicio do turno, antes da acao do jogador. */
        int descartados = estoqueDescartarVencidos(&HeapCarne, turno);
        if (descartados > 0) {
            printf("Descartados por validade: %d\n", descartados);
        }

        printf("\n[turno %d] > ", turno);
        if (scanf(" %c", &op) != 1 || op == 'q') {
            break;
        }

        switch (op) {
        case 'p':
            if (HeapCarne.tam >= CAP) {
                printf("Estoque cheio! Nao foi possivel preparar carne.\n");
            } else {
                int venc = turno + CFG_VALIDADE_CARNE;
                fpInserir(&HeapCarne, venc);
                printf("Preparou uma carne (vence no turno %d).\n", venc);
                turno++; /* acao que consome turno */
            }
            break;

        case 'c': {
            int venc = fpRemoverMinimo(&HeapCarne);
            if (venc < 0) {
                printf("Sem estoque de carne para consumir.\n");
            } else {
                printf("Consumiu carne (vencimento %d).\n", venc);
                turno++;
            }
            break;
        }

        case 's':
            printf("Estoque: %d\n", HeapCarne.tam);
            break; /* consulta: nao consome turno */

        case 'i':
            if (scanf("%d", &n) == 1) {
                if (HeapCarne.tam >= CAP) {
                    printf("Nao foi possivel inserir o vencimento!\n");
                } else {
                    fpInserir(&HeapCarne, n);
                    printf("Inserido o vencimento %d.\n", n);
                }
            }
            break;

        case 'j':
            if (scanf("%d", &n) == 1 && n > 0) {
                turno += n; /* a limpeza roda no inicio da proxima volta */
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