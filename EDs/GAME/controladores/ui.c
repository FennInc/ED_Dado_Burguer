#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "tipos.h"
#include "estoque.h"
#define MAX_CAMADAS_VISUAIS 32
#define LARGURA_BARRA_PACIENCIA 10
int ler_opcao_inteira(const char *prompt, int min, int max) {
    char linha[64];

    while (1) {
        printf("%s", prompt);

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            return min; /* Trata EOF (Ctrl+D / Ctrl+Z) sem loop infinito */
        }

        /* Se a linha foi maior que 63 chars e não coube o '\n', limpa o resto */
        if (strchr(linha, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrada muito longa. Digite um numero entre %d e %d.\n", min, max);
            continue;
        }

        errno = 0;
        char *fim = NULL;
        long valor = strtol(linha, &fim, 10);

        /* Pula espaços em branco eventuais depois do número antes do '\n' */
        while (*fim == ' ' || *fim == '\t') {
            fim++;
        }

        /* Só aceita se leu pelo menos 1 dígito, não sobrou letra e está no intervalo */
        if (fim == linha || (*fim != '\n' && *fim != '\0') ||
            errno == ERANGE || valor < min || valor > max) {
            printf("Opcao invalida! Escolha entre %d e %d.\n", min, max);
            continue;
        }

        return (int)valor;
    }
}
void impressao_menu_principal(void) {
    printf("<<<MENU>>>\n"
           "1) NOVO JOGO\n"
           "2) COMO JOGAR\n"
           "3) HISTORICO\n"
           "Choose an option please");
}

void como_jogar(void) {
    printf("REGRAS BASICAS\n");
    printf("OBJETIVO DO JOGO\n"
           "O jogador é o mais novo funcionário do Dado Burger e deverá atender aos pedidos "
           "de uma série de clientes ao longo do dia.\n");

    printf("LISTA DE INGREDIENTES E SEUS SIMBOLOS\n");
    printf("Pão (de cima)\t z\n"
           "Ketchup\t k\n"
           "Mostarda\t m\n"
           "Picles\t p\n"
           "Alface\t a\n"
           "Queijo suíço\t s\n"
           "Queijo cheddar\t q\n"
           "Cebola\t c\n"
           "Tomate\t t\n"
           "Carne\t n\n"
           "Ovo frito\t o\n"
           "Bacon\t b\n");

    printf("Ações do jogador\n"
           "As seguintes ações consomem um turno cada:\n"
           "- Preparar um ingrediente\n"
           "- Adicionar um ingrediente ao hambúrguer\n"
           "- Remover o ingrediente do topo do hambúrguer\n"
           "Ações de consulta não consomem turno.\n");

    printf("CONDICOES DE VITORIA E DE DERROTA\n");
    printf("Derrota: se a paciência do cliente da frente chegar a zero antes de o pedido ser concluído, você perdeu.\n");
    printf("Vitória: se todos os clientes forem atendidos sem que ninguém perca a paciência, você venceu.\n");
}

void limpar_tela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


   const char *nome_ingrediente(IdIngrediente id)
{
    switch (id) {
        case ing_ketchup:        return "Ketchup";
        case ing_mostarda:       return "Mostarda";
        case ing_carne:          return "Carne";
        case ing_queijo_cheddar: return "Queijo Cheddar";
        case ing_queijo_suico:   return "Queijo Suico";
        case ing_bacon:          return "Bacon";
        case ing_ovo_frito:      return "Ovo Frito";
        case ing_cebola:         return "Cebola";
        case ing_tomate:         return "Tomate";
        case ing_picles:         return "Picles";
        case ing_alface:         return "Alface";
        case ing_pao_superior:   return "Pao Superior";
        default:                 return "Desconhecido";
    }
}

/* Desenha a barra proporcional [#######---] 7/10 turnos */
static void imprimir_barra_paciencia(int atual, int maxima) {
    if (atual < 0) atual = 0;
    if (maxima <= 0) maxima = 1;

    int preenchidos = (atual * LARGURA_BARRA_PACIENCIA) / maxima;
    if (atual > 0 && preenchidos == 0) {
        preenchidos = 1; /* Garante ao menos 1 '#' enquanto ainda houver paciência */
    }
    if (preenchidos > LARGURA_BARRA_PACIENCIA) {
        preenchidos = LARGURA_BARRA_PACIENCIA;
    }

    printf(" Paciencia: [");
    for (int i = 0; i < LARGURA_BARRA_PACIENCIA; i++) {
        putchar(i < preenchidos ? '#' : '-');
    }
    printf("] %d/%d turnos\n", atual, maxima);
}

/* Imprime Receita Alvo e Lanche na Bandeja lado a lado, alinhados pela base */
static void imprimir_comparacao_lanche(const Pedido *pedido, const PilhaBurger *lanche) {
    IdIngrediente vet_receita[MAX_CAMADAS_VISUAIS];
    IdIngrediente vet_bandeja[MAX_CAMADAS_VISUAIS];
    int qtd_receita = 0;
    int qtd_bandeja = 0;

    /*  Copia a Lista da Receita (já está da Base [0] para o Topo [qtd-1]) */
    if (pedido != NULL) {
        const NoIngrediente *atual = pedido->receita_inicio;
        while (atual != NULL && qtd_receita < MAX_CAMADAS_VISUAIS) {
            vet_receita[qtd_receita++] = atual->id;
            atual = atual->prox;
        }
    }

    /*  Copia a Pilha da Bandeja invertendo para ficar Base [0] -> Topo [qtd-1] */
    if (lanche != NULL && lanche->quantidade > 0) {
        int total = lanche->quantidade;
        if (total > MAX_CAMADAS_VISUAIS) {
            total = MAX_CAMADAS_VISUAIS;
        }
        qtd_bandeja = total;

        const NoIngrediente *atual = lanche->topo;
        for (int idx = total - 1; idx >= 0 && atual != NULL; idx--) {
            vet_bandeja[idx] = atual->id;
            atual = atual->prox;
        }
    }

    int max_linhas = (qtd_receita > qtd_bandeja) ? qtd_receita : qtd_bandeja;
    if (max_linhas == 0) {
        max_linhas = 1; /* Mantém ao menos uma linha visual vazia */
    }

    printf(" RECEITA ALVO (Topo -> Base)      | SEU LANCHE NA BANDEJA (Topo -> Base)\n");

    /*  Imprime de cima para baixo (do maior índice até a base 0) */
    for (int linha = max_linhas - 1; linha >= 0; linha--) {
        char col_esq[48] = "";
        char col_dir[48] = "";

        if (linha < qtd_receita) {
            IdIngrediente id_r = vet_receita[linha];
            snprintf(col_esq, sizeof(col_esq), "   [%d] %s",
                     (int)id_r, nome_ingrediente(id_r));
        }

        if (linha < qtd_bandeja) {
            IdIngrediente id_b = vet_bandeja[linha];
            snprintf(col_dir, sizeof(col_dir), "   [%d] %s",
                     (int)id_b, nome_ingrediente(id_b));
        }

        /* %-34s fixa a largura exata da coluna esquerda para o '|' nunca entortar */
        printf("%-34s|%s\n", col_esq, col_dir);
    }
}

/*  RENDERIZAÇÃO COMPLETA DO HUD DA PARTIDA */
void renderizar_tela_jogo(
    int turno_atual,
    int pontuacao,
    int clientes_na_fila,
    const Cliente *cliente,
    const Pedido *pedido,
    const PilhaBurger *lanche,
    const Estoque *estoque
) {
    limpar_tela();

    printf("========================================================================\n");
    printf(" DADO BURGER | Turno: %d | Pontos: %d | Clientes na Fila: %d\n",
           turno_atual, pontuacao, clientes_na_fila);
    printf("========================================================================\n");

    if (cliente != NULL && pedido != NULL) {
        printf(" CLIENTE ATUAL: %s (Pedido #%d - %s)\n",
               cliente->nome, pedido->codigo, pedido->nome_lanche);
        imprimir_barra_paciencia(cliente->paciencia_atual, cliente->paciencia_maxima);
    } else {
        printf(" CLIENTE ATUAL: Aguardando proximo cliente...\n");
    }

    printf("------------------------------------------------------------------------\n");
    imprimir_comparacao_lanche(pedido, lanche);
    printf("------------------------------------------------------------------------\n");

    printf(" ESTOQUE DA COZINHA:\n");
    if (estoque != NULL) {
        printf("  Pao ilimitado | Mostarda ilimitado | Ketchup ilimitado | Picles ilimitado | Alface ilimitado |\n| Queijo Cheddar (%d) | Queijo suico (%d) | Tomate (%d) | Cebola (%d) |\n| Carne (%d) | Ovo Frito (%d) | Bacon (%d) |",
               estoque->perenes[PERENE_QUEIJO_CHEDDAR],
               estoque->perenes[PERENE_QUEIJO_SUICO],
               estoque->perenes[PERENE_TOMATE], 
               estoque->perenes[PERENE_CEBOLA],
               estoque->carne.tam,
               estoque->ovo.tam,
               estoque->bacon.tam
        );
    }

    printf("========================================================================\n");
    printf(" ACOES (Custo: 1 turno):\n");
    printf("   [1] Preparar perecivel (Carne/Bacon/Ovo)\n");
    printf("   [2] Adicionar ingrediente ao lanche\n");
    printf("   [3] Remover ingrediente do topo (Descartar)\n");
    printf("   [4] Entregar pedido ao cliente\n");
    printf("   [0] Abandonar partida\n");
}

//Imprime o HUD da partida no terminal com pedido, lanche, estoque, fila e turnos/paciencia.