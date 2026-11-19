#ifndef UI_H
#define UI_H

#include "tipos.h"
#include "montagem.h"

/* Resumo visual de cada Heap Mínimo de perecíveis (fornecido pelo Bloco 2) */
typedef struct {
    int quantidade;
    int proximo_vencimento; /* Turno em que a unidade do topo vence */
} InfoPerecivel;

/* Snapshot do estoque da cozinha para exibição no HUD */
typedef struct {
    int pao_inferior;
    int pao_superior;
    int queijo;
    int alface;
    int tomate;
    InfoPerecivel carne;
    InfoPerecivel bacon;
    InfoPerecivel ovo;
} EstoqueVisual;

/* Funções utilitárias de terminal */
void limpar_tela(void);
const char* nome_ingrediente(IdIngrediente id);
int ler_opcao_inteira(const char *prompt, int min, int max);

/* Renderização do HUD principal da partida (Passo 5.3) */
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