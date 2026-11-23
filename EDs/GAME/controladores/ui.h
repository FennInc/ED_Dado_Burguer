#ifndef UI_H
#define UI_H
#include"pilhaEnc.h"
#include "tipos.h"
#include "montagem.h"
#include "estoque.h"

/* Resumo visual de cada Heap Mínimo de perecíveis (fornecido pelo Bloco 2) */
typedef struct {
    int quantidade;
    int proximo_vencimento; /* Turno em que a unidade do topo vence */
} InfoPerecivel;

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
    const Estoque *estoque
);

void impressao_menu_principal(void);
void como_jogar(void);
void imprimir_barra_paciencia(int atual, int maxima);
void imprimir_comparacao_lanche(const Pedido *pedido, const PilhaBurger *lanche);

#endif