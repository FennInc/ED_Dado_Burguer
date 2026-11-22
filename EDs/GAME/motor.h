#ifndef MOTOR_H
#define MOTOR_H

#include "estoque.h"

/*
 * motor.h - regras de turno do Dado Burger (spec, secoes 3.3 e 3.4).
 *
 * O motor guarda o estado da partida (turno, clientes atendidos, resultado,
 * estoque) e aplica as regras de turno, paciencia, vitoria e derrota.
 *
 * A paciencia mora no Cliente (modulo de quem cuida da fila). O motor nao
 * conhece esse tipo: quem chama passa um ponteiro para a paciencia do
 * cliente da frente.
 *
 * Fluxo de um turno:
 *   1) motorIniciarTurno(): limpa os pereciveis vencidos;
 *   2) o jogador age;
 *   3) se a acao foi realizada e gasta turno: motorAcaoRealizada();
 *      se a acao falhou ou foi uma consulta: nada e chamado.
 */

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

/* Zera o estado e guarda o total de clientes. Chamar no inicio da partida. */
void motorIniciarPartida(Partida *p, int clientesTotal);

/* Inicio de turno, antes da acao do jogador: descarta os pereciveis vencidos.
   Devolve quantas unidades foram descartadas. */
int motorIniciarTurno(Partida *p);

/* Registra uma acao que gasta turno (preparar, adicionar ou remover) e que
   foi realizada com sucesso.
     - o turno sobe 1;
     - a paciencia do cliente da frente (pacienciaCliente) desce 1;
     - se a acao concluiu o pedido (pedidoConcluido != 0), o cliente conta
       como atendido e, se era o ultimo, a partida termina em vitoria;
     - se nao concluiu e a paciencia chegou a 0, a partida termina em derrota.
   Se o pedido foi concluido nesta mesma acao, a paciencia zerada NAO causa
   derrota (a spec so da derrota se a paciencia zera "antes" da conclusao).
   Depois de uma conclusao sem fim de partida, quem chama deve passar ao
   proximo cliente e restaurar a paciencia dele.
   Nao faz nada se a partida ja terminou. */
void motorAcaoRealizada(Partida *p, int *pacienciaCliente, int pedidoConcluido);

/* Encerra a partida por saida voluntaria. */
void motorAbandonar(Partida *p);

/* Devolve 1 se a partida terminou (vitoria, derrota ou abandono). */
int motorTerminou(const Partida *p);

/* Pontuacao: numero de clientes atendidos (spec, secao 3.3). */
int motorPontuacao(const Partida *p);

#endif /* MOTOR_H */