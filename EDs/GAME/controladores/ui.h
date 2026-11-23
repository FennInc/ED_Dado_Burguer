#ifndef UI_H
#define UI_H
#include"pilhaEnc.h"
#include "tipos.h"
#include "montagem.h"

/* Resumo visual de cada Heap Mínimo de perecíveis (fornecido pelo Bloco 2) */
typedef struct {
    int quantidade;
    int proximo_vencimento; /* Turno em que a unidade do topo vence */
} InfoPerecivel;

/* Snapshot do estoque da cozinha para exibição no HUD */
typedef struct {
    int ing_pao_inferior;  // i
    int ing_mostarda;        // m
    int ing_queijo_cheddar;  // q
    int ing_queijo_suico;    // s
    int ing_bacon;          // b
    int ing_ovo_frito;      // o
    int ing_cebola;         // c
    int ing_tomate;         // t
    int ing_picles;         // p
    int ing_alface;         // a
    int ing_ketchup;        // k
    int ing_pao_superior;   // z
    InfoPerecivel ing_carne;
    InfoPerecivel info_bacon;
    InfoPerecivel info_ovo_frito;
} EstoqueVisual;

/* Funções utilitárias de terminal */
void limpar_tela(void);
const char* nome_ingrediente(IdIngrediente id);
int ler_opcao_inteira(const char *prompt, int min, int max);

/* Renderização do HUD principal da partida */
void renderizar_tela_jogo(
    int turno_atual,
    int pontuacao,
    int clientes_na_fila,
    const Cliente *cliente,
    const Pedido *pedido,
    const PilhaBurger *lanche,
    const EstoqueVisual *estoque
);

#endif