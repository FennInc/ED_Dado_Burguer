#ifndef TIPOS_H
#define TIPOS_H

#define MAX_NOME 64
#define MAX_LINHA 200
#define TAM 20
/* IDs padronizados dos ingredientes */
typedef enum {
    ing_pao_inferior=1,  // i
    ing_mostarda,        // m
    ing_carne,           // n
    ing_queijo_cheddar,  // q
    ing_queijo_suico,    // s
    ing_bacon,           // b
    ing_ovo_frito,       // o
    ing_cebola,          // c
    ing_tomate,          // t
    ing_picles,          // p
    ing_alface,          // a
    ing_ketchup,         // k
    ing_pao_superior,    // z
    TOTAL_INGREDIENTES
} IdIngrediente;

/* Nó usado tanto na Lista da Receita quanto na Pilha do Lanche */
struct str_NoIngrediente;       // não precisa ser separada
typedef struct str_NoIngrediente str_NoIngrediente;
typedef struct {
    IdIngrediente id;
    struct str_NoIngrediente *prox;
} NoIngrediente;

/* Estrutura de um Pedido armazenado na AVL/Hash  */
typedef struct {
    int codigo;
    char nome_lanche[MAX_NOME];
    NoIngrediente *receita_inicio; /* Lista encadeada da base para o topo */
    int qtd_ingredientes;
} Pedido;

/* Estrutura de um Cliente na Fila de Atendimento */
typedef struct {
    int id_cliente;
    char nome[MAX_NOME];
    int codigo_pedido;
    int paciencia_atual;
    int paciencia_maxima;
} Cliente;

/* Estrutura do jogador Histórico (nome + pontuação) */
typedef struct 
{
    char nome[TAM];
    int pontuacao;
}JOGADOR;

/* Registro salvo no Histórico */
typedef struct {
    char nome_jogador[MAX_NOME];
    int pontuacao;
    int clientes_atendidos;
    int turnos_jogados;
} RegistroHistorico;

#endif