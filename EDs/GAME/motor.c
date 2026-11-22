#include <stddef.h>
#include "motor.h"

void motorIniciarPartida(Partida *p, int clientesTotal) {
    if (p == NULL) return;

    p->turno = 1;
    p->clientesTotal = clientesTotal;
    p->clientesAtendidos = 0;
    p->resultado = PARTIDA_EM_ANDAMENTO;

    /* TODO: inicializar o estoque (modulo estoque) */
    estoqueInicializar(&p->estoque);
}

int motorIniciarTurno(Partida *p) {
    if (p == NULL || p->resultado != PARTIDA_EM_ANDAMENTO) return 0;

    /* TODO: descartados = limpeza dos vencidos no estoque, com o turno atual */
    int descartados = estoqueLimparVencidos(&p->estoque, p->turno);
    if (descartados > 0) {
    }
    return descartados;
}

void motorAcaoRealizada(Partida *p, int *pacienciaCliente, int pedidoConcluido) {
    if (p == NULL || p->resultado != PARTIDA_EM_ANDAMENTO) return;

    p->turno++;

    if (pacienciaCliente != NULL) {
        (*pacienciaCliente)--;
    }

    if (pedidoConcluido) {
        p->clientesAtendidos++;
        if (p->clientesAtendidos >= p->clientesTotal) {
            p->resultado = PARTIDA_VITORIA;
        }
    } else if (pacienciaCliente != NULL && *pacienciaCliente <= 0) {
        p->resultado = PARTIDA_DERROTA;
    }
}

void motorAbandonar(Partida *p) {
    if (p == NULL || p->resultado != PARTIDA_EM_ANDAMENTO) return;
    p->resultado = PARTIDA_ABANDONADA;
}

int motorTerminou(const Partida *p) {
    if (p == NULL) return 1;
    return p->resultado != PARTIDA_EM_ANDAMENTO;
}

int motorPontuacao(const Partida *p) {
    if (p == NULL) return 0;
    return p->clientesAtendidos;
}