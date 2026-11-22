#include <stddef.h>
#include "partida.h"

//iniciar a partida
void IniciarPartida(Partida *p, int clientesTotal) {
    if (p == NULL) return;

    p->turno = 1;
    p->clientesTotal = clientesTotal;
    p->clientesAtendidos = 0;
    p->resultado = PARTIDA_EM_ANDAMENTO;
    estoqueInicializar(&p->estoque);
}

//inicia um turno
int IniciarTurno(Partida *p) {
    if (p == NULL || p->resultado != PARTIDA_EM_ANDAMENTO) return 0;

    int descartados = estoqueLimparVencidos(&p->estoque, p->turno);
    return descartados;
}

//fim de um turno
void turnosAcaoRealizada(Partida *p, int *pacienciaCliente, int pedidoConcluido) {
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

//desiste do jogo
void partidaAbandonar(Partida *p) {
    if (p == NULL || p->resultado != PARTIDA_EM_ANDAMENTO) return;
    p->resultado = PARTIDA_ABANDONADA;
}

//acabou o jogo
int partidaTerminou(const Partida *p) {
    if (p == NULL) return 1;
    return p->resultado != PARTIDA_EM_ANDAMENTO;
}

//ta no nome né
int partidaPontuacao(const Partida *p) {
    if (p == NULL) return 0;
    return p->clientesAtendidos;
}