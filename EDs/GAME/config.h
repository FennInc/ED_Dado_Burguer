#ifndef CONFIG_H
#define CONFIG_H

/*
 * config.h - parametros configuraveis do Dado Burger (spec, secao 3.2).
 * Cada valor deve ser justificado na secao "O jogo" do relatorio.
 * Valores marcados com PROVISORIO ainda precisam ser decididos pelo grupo.
 */

/* ---------- Clientes por partida ---------- */
#define CFG_CLIENTES_MIN 3   /* minimo de clientes sorteado por partida */
#define CFG_CLIENTES_MAX 7   /* maximo de clientes sorteado por partida */

/* ---------- Sorteio (semente) ---------- */
/* 1 = semente fixa (partidas e medicoes repetiveis); 0 = semente variavel */
#define CFG_USAR_SEMENTE_FIXA 1
#define CFG_SEMENTE 42       /* usada quando CFG_USAR_SEMENTE_FIXA == 1 */

/* ---------- Paciencia ---------- */
#define CFG_PACIENCIA_MAX 12 /* turnos por cliente. PROVISORIO */

/* ---------- Validade dos pereciveis (turnos) ---------- */
/* Valores do exemplo do apendice B da spec. PROVISORIO */
#define CFG_VALIDADE_CARNE 3
#define CFG_VALIDADE_OVO   2
#define CFG_VALIDADE_BACON 3

/* ---------- Tabela hash (cache de pedidos) ---------- */
#define CFG_HASH_M 13       /* tamanho da tabela. PROVISORIO */

/* ---------- Historico ---------- */
#define CFG_TAM_NOME 20      /* maximo de caracteres do nome (sem o '\0') */

#endif /* CONFIG_H */