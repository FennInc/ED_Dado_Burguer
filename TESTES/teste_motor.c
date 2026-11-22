#include <stdio.h>
#include "config.h"
#include "motor.h"

/*
 * Teste interativo do motor de turnos. Simula um cliente da frente com uma
 * paciencia local (pac); no jogo, ela mora no Cliente da fila.
 *
 * Comandos:
 *   a   acao que gasta turno (preparar, adicionar ou remover) e NAO conclui o pedido
 *   c   acao que gasta turno e CONCLUI o pedido do cliente da frente
 *   x   abandonar a partida
 *   q   sair do teste
 * Em cada volta o teste mostra o estado e chama o inicio de turno.
 *
 * Partida de teste: TESTE_CLIENTES clientes; paciencia = CFG_PACIENCIA_MAX.
 * Abaixo, P = CFG_PACIENCIA_MAX.
 *
 * ROTEIROS E RESULTADOS ESPERADOS
 *
 * 1) Derrota por paciencia:   a repetido P vezes
 *    - a paciencia chega a 0 e a partida termina em DERROTA
 *    - pontuacao 0; o turno final e P + 1
 *
 * 2) Vitoria:                 c c c
 *    - cada c atende um cliente e a paciencia volta a P
 *    - no terceiro c a partida termina em VITORIA, pontuacao 3
 *
 * 3) Concluir no ultimo turno de paciencia:   a repetido (P - 1) vezes, depois c
 *    - o pedido e concluido na mesma acao em que a paciencia zeraria
 *    - NAO ha derrota; atendidos = 1 e a paciencia volta a P
 *
 * 4) Abandono:                c x
 *    - resultado ABANDONADA, pontuacao 1
 *
 * 5) Acao depois do fim:      a repetido P vezes, depois a c
 *    - apos a derrota, nada muda: turno e pontuacao permanecem
 */

#define TESTE_CLIENTES 3

static const char *nomeResultado(ResultadoPartida r) {
    switch (r) {
    case PARTIDA_EM_ANDAMENTO: return "em andamento";
    case PARTIDA_VITORIA:      return "VITORIA";
    case PARTIDA_DERROTA:      return "DERROTA";
    case PARTIDA_ABANDONADA:   return "ABANDONADA";
    default:                   return "?";
    }
}

int main(void) {
    Partida p;
    int pac = CFG_PACIENCIA_MAX;
    char op;

    motorIniciarPartida(&p, TESTE_CLIENTES);

    printf("Teste do motor de turnos (%d clientes, paciencia %d)\n",
           TESTE_CLIENTES, CFG_PACIENCIA_MAX);
    printf("Comandos: a | c | x | q\n");

    while (1) {
        int descartados = motorIniciarTurno(&p);
        if (descartados > 0) {
            printf("Descartados por validade: %d\n", descartados);
        }

        printf("\n[turno %d] paciencia %d | atendidos %d/%d | %s\n",
               p.turno, pac, p.clientesAtendidos, p.clientesTotal,
               nomeResultado(p.resultado));

        if (motorTerminou(&p)) {
            printf("Partida encerrada. Pontuacao: %d\n", motorPontuacao(&p));
        }

        printf("> ");
        if (scanf(" %c", &op) != 1 || op == 'q') {
            break;
        }

        switch (op) {
        case 'a':
            motorAcaoRealizada(&p, &pac, 0);
            break;

        case 'c':
            motorAcaoRealizada(&p, &pac, 1);
            if (!motorTerminou(&p)) {
                pac = CFG_PACIENCIA_MAX; /* proximo cliente chega com paciencia cheia */
            }
            break;

        case 'x':
            motorAbandonar(&p);
            break;

        default:
            printf("Comando invalido.\n");
            break;
        }
    }

    return 0;
}