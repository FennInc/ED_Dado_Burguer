#ifndef CONFIG_H
#define CONFIG_H
//lugar para guardar as constantes

//minimo e maximo de clientes pq quero que seja uma quantidade +- aleatoria toda vez
#define CFG_CLIENTES_MIN 3 
#define CFG_CLIENTES_MAX 7  

//ngc de semente para o uso em testes 
#define CFG_USAR_SEMENTE_FIXA 1
#define CFG_SEMENTE 42

//tamanho das receitas
#define CFG_RECEITA_TAM 8

//paciencia dos clientes
#define CFG_PACIENCIA_MAX 12

//validade dos pereciveis
#define CFG_VALIDADE_CARNE 3
#define CFG_VALIDADE_OVO   2
#define CFG_VALIDADE_BACON 3

//tamanho da hash
#define CFG_HASH_M 53

//maximo de caracteres do nome
#define CFG_TAM_NOME 20

#endif