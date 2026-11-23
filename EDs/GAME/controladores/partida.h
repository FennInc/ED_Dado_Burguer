#ifndef PARTIDA_H
#define PARTIDA_H
#include "estoque.h"

//estados da partida
typedef enum {
    PARTIDA_EM_ANDAMENTO = 0,
    PARTIDA_VITORIA,
    PARTIDA_DERROTA,
    PARTIDA_ABANDONADA
} ResultadoPartida;


typedef struct {
    int turno;               /* turno atual; comeca em 1 */
    int clientesTotal;       /* clientes da partida */
    int clientesAtendidos;   /* pedidos concluidos ate agora */
    ResultadoPartida resultado;
    Estoque estoque;
} Partida;

void IniciarPartida(Partida *p, int clientesTotal);
int IniciarTurno(Partida *p);
void turnosAcaoRealizada(Partida *p, int *pacienciaCliente, int pedidoConcluido);
void partidaAbandonar(Partida *p);
int partidaTerminou(const Partida *p);
int partidaPontuacao(const Partida *p);

#endif