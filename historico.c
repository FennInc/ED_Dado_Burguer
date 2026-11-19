#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "historico.h"
#include "timer.h"

/* Remove '\r' e '\n' do final de uma string lida com fgets */
static void remover_quebra_linha(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

/* Converte uma fatia de texto estritamente para inteiro validando erros */
static bool converter_inteiro(const char *texto, int *destino) {
    if (texto == NULL || *texto == '\0') {
        return false;
    }
    errno = 0;
    char *fim = NULL;
    long valor = strtol(texto, &fim, 10);

    /* Falha se nenhum dígito foi lido, se sobrou lixo ou se houve overflow */
    if (fim == texto || *fim != '\0' || errno == ERANGE) {
        return false;
    }
    *destino = (int)valor;
    return true;
}

/* Faz o parse de uma linha no formato: Nome Jogador;1500;8;42 */
static bool interpretar_linha_historico(char *linha, RegistroHistorico *reg) {
    char *sep1 = strchr(linha, ';');
    if (sep1 == NULL) return false;

    char *sep2 = strchr(sep1 + 1, ';');
    if (sep2 == NULL) return false;

    char *sep3 = strchr(sep2 + 1, ';');
    if (sep3 == NULL) return false;

    /* Garante que não há separadores extras na mesma linha */
    if (strchr(sep3 + 1, ';') != NULL) return false;

    /* Isola cada campo substituindo ';' por terminador nulo '\0' */
    *sep1 = '\0';
    *sep2 = '\0';
    *sep3 = '\0';

    /* Copia o nome respeitando o limite MAX_NOME sem risco de overflow */
    if (linha[0] == '\0') return false;
    strncpy(reg->nome_jogador, linha, MAX_NOME - 1);
    reg->nome_jogador[MAX_NOME - 1] = '\0';

    /* Converte os três campos numéricos */
    if (!converter_inteiro(sep1 + 1, &reg->pontuacao)) return false;
    if (!converter_inteiro(sep2 + 1, &reg->clientes_atendidos)) return false;
    if (!converter_inteiro(sep3 + 1, &reg->turnos_jogados)) return false;

    return true;
}
void inicializar_historico(ListaHistorico *lista, int cap_inicial){ 
    RegistroHistorico *dados=malloc(cap_inicial * sizeof(RegistroHistorico));
    lista->tamanho = 0 ;
    lista->capacidade = cap_inicial;
};
bool inserir_historico(ListaHistorico *lista, RegistroHistorico reg) {
    if (lista->tamanho == lista->capacidade) // lista cheia
        // Se capacity for 0, começa com 4; caso contrário, dobra
        int nova_capacidade = (lista->capacidade == 0) ? 4 : lista->capacidade * 2;

        RegistroHistorico *temp = realloc(lista->dados, nova_capacidade  *sizeof(*temp));
        if (temp == NULL) {
            return false; // Falha de alocação; dados originais permanecem intactos
        }
        lista->dados = temp;
        lista->capacidade = nova_capacidade;
        lista->dados[lista->tamanho] = reg;
        lista->tamanho++;
        return true;
}
void liberar_historico(ListaHistorico *lista) {
    if (lista == NULL) {
        return;
    }

    free(lista->dados);
    lista->dados = NULL;
    lista->tamanho = 0;
    lista->capacidade = 0;
}
bool carregar_historico(ListaHistorico *lista, const char *caminho) {
    if (lista == NULL || caminho == NULL) {
        return false;
    }

    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        /* Na 1ª execução o arquivo não existe: não é erro, lista inicia vazia */
        return true;
    }

    char linha[MAX_LINHA];
    int num_linha = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        num_linha++;
        remover_quebra_linha(linha);

        /* Ignora linhas vazias no arquivo */
        if (linha[0] == '\0') {
            continue;
        }

        RegistroHistorico reg;
        if (!interpretar_linha_historico(linha, &reg)) {
            fprintf(stderr, "Aviso: linha %d inválida ignorada em '%s'.\n",
                    num_linha, caminho);
            continue;
        }

        if (!inserir_historico(lista, reg)) {
            fprintf(stderr, "Erro: sem memória ao carregar '%s'.\n", caminho);
            fclose(arquivo);
            return false;
        }
    }

    if (ferror(arquivo)) {
        perror("Erro de leitura no arquivo de histórico");
        fclose(arquivo);
        return false;
    }

    fclose(arquivo);
    return true;
}

/* SALVAR HISTÓRICO DA LISTA CONTÍGUA PARA O ARQUIVO*/
bool salvar_historico(const ListaHistorico *lista, const char *caminho) {
    if (lista == NULL || caminho == NULL) {
        return false;
    }

    FILE *arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de histórico para gravação");
        return false;
    }

    for (int i = 0; i < lista->tamanho; i++) {
        const RegistroHistorico *reg = &lista->dados[i];

        /* Sanitiza o nome para evitar que um ';' digitado pelo usuário quebre o arquivo */
        char nome_limpo[MAX_NOME];
        strncpy(nome_limpo, reg->nome_jogador, MAX_NOME - 1);
        nome_limpo[MAX_NOME - 1] = '\0';

        for (size_t j = 0; nome_limpo[j] != '\0'; j++) {
            if (nome_limpo[j] == ';' || nome_limpo[j] == '\n' || nome_limpo[j] == '\r') {
                nome_limpo[j] = ' ';
            }
        }

        int status = fprintf(
            arquivo,
            "%s;%d;%d;%d\n",
            nome_limpo,
            reg->pontuacao,
            reg->clientes_atendidos,
            reg->turnos_jogados
        );

        if (status < 0) {
            perror("Erro ao escrever registro no arquivo de histórico");
            fclose(arquivo);
            return false;
        }
    }

    /* Verifica o retorno do fclose para garantir que o buffer foi descarregado no disco */
    if (fclose(arquivo) != 0) {
        perror("Erro ao finalizar gravação do arquivo de histórico");
        return false;
    }

    return true;
}
/* -------------------------------------------------------------------------
 * COMPARADOR COM CONTAGEM DE OPERAÇÕES E ESTABILIDADE
 * Retorna true se 'esq' deve aparecer antes de 'dir' na saída ordenada.
 * ------------------------------------------------------------------------- */
static bool deve_vir_antes(
    const RegistroHistorico *esq,
    const RegistroHistorico *dir,
    CriterioOrdenacao criterio,
    long *comparacoes
) {
    (*comparacoes)++;

    if (criterio == ORDENAR_POR_PONTUACAO) {
        if (esq->pontuacao != dir->pontuacao) {
            return esq->pontuacao > dir->pontuacao; /* Maior pontuação primeiro */
        }
        /* Desempate opcional por menos turnos; '>=' final preserva estabilidade */
        if (esq->turnos_jogados != dir->turnos_jogados) {
            return esq->turnos_jogados < dir->turnos_jogados;
        }
        return true;
    }

    /* Critério: ORDENAR_POR_NOME */
    int cmp = strcmp(esq->nome_jogador, dir->nome_jogador);
    if (cmp != 0) {
        return cmp < 0; /* Ordem alfabética A-Z */
    }
    /* Se o nome for idêntico, mantém o de maior pontuação ou a ordem original */
    return esq->pontuacao >= dir->pontuacao;
}

/* -------------------------------------------------------------------------
 * ETAPA DE INTERCALAÇÃO (MERGE)
 * ------------------------------------------------------------------------- */
static void intercalar(
    RegistroHistorico *vetor,
    RegistroHistorico *aux,
    int inicio,
    int meio,
    int fim,
    CriterioOrdenacao criterio,
    long *comparacoes
) {
    int i = inicio;     /* Cursor da metade esquerda [inicio..meio] */
    int j = meio + 1;   /* Cursor da metade direita [meio+1..fim] */
    int k = inicio;     /* Cursor do vetor auxiliar */

    while (i <= meio && j <= fim) {
        if (deve_vir_antes(&vetor[i], &vetor[j], criterio, comparacoes)) {
            aux[k++] = vetor[i++];
        } else {
            aux[k++] = vetor[j++];
        }
    }

    /* Copia eventuais elementos restantes da metade esquerda */
    while (i <= meio) {
        aux[k++] = vetor[i++];
    }

    /* Copia eventuais elementos restantes da metade direita */
    while (j <= fim) {
        aux[k++] = vetor[j++];
    }

    /* Devolve o segmento intercalado para o vetor de cópia */
    for (int idx = inicio; idx <= fim; idx++) {
        vetor[idx] = aux[idx];
    }
}

/* -------------------------------------------------------------------------
 * RECURSÃO PRINCIPAL DO MERGESORT
 * ------------------------------------------------------------------------- */
static void mergesort_recursivo(
    RegistroHistorico *vetor,
    RegistroHistorico *aux,
    int inicio,
    int fim,
    CriterioOrdenacao criterio,
    long *comparacoes
) {
    if (inicio >= fim) {
        return;
    }

    int meio = inicio + (fim - inicio) / 2;

    mergesort_recursivo(vetor, aux, inicio, meio, criterio, comparacoes);
    mergesort_recursivo(vetor, aux, meio + 1, fim, criterio, comparacoes);
    intercalar(vetor, aux, inicio, meio, fim, criterio, comparacoes);
}

/* CLONA A LISTA CONTÍGUA, ORDENA A CÓPIA E CRONOMETRA
 * Nota: O chamador é responsável por dar free() no ponteiro retornado.
 * ------------------------------------------------------------------------- */
RegistroHistorico* gerar_copia_ordenada(
    const ListaHistorico *lista,
    CriterioOrdenacao criterio,
    MetricasOrdenacao *metricas
) {
    if (lista == NULL || lista->tamanho <= 0) {
        if (metricas != NULL) {
            metricas->elementos_ordenados = 0;
            metricas->comparacoes = 0;
            metricas->tempo_segundos = 0.0;
        }
        return NULL;
    }

    int n = lista->tamanho;
    size_t bytes = (size_t)n * sizeof(RegistroHistorico);

    /*  Aloca a cópia para nunca mutar lista->dados (nem a ordem do arquivo) */
    RegistroHistorico *copia = (RegistroHistorico *)malloc(bytes);
    if (copia == NULL) {
        return NULL;
    }

    /*  Aloca o buffer auxiliar do Mergesort UMA única vez */
    RegistroHistorico *aux = (RegistroHistorico *)malloc(bytes);
    if (aux == NULL) {
        free(copia);
        return NULL;
    }

    memcpy(copia, lista->dados, bytes);

    long comparacoes = 0;

    /*  Cronometra APENAS o algoritmo de ordenação (sem malloc/memcpy/printf) */
    inicia_tempo(); /* Ajuste para o nome exato da função do timer.h */
    mergesort_recursivo(copia, aux, 0, n - 1, criterio, &comparacoes);
    double tempo_gasto = finaliza_tempo(); /* Ajuste para o retorno do timer.h */

    free(aux);

    if (metricas != NULL) {
        metricas->elementos_ordenados = n;
        metricas->comparacoes = comparacoes;
        metricas->tempo_segundos = tempo_gasto;
    }

    return copia;
}

/* -------------------------------------------------------------------------
 * EXIBE A TABELA FORMATADA NO TERMINAL E LIBERA A CÓPIA TEMPORÁRIA
 * ------------------------------------------------------------------------- */
bool exibir_historico_ordenado(
    const ListaHistorico *lista,
    CriterioOrdenacao criterio,
    MetricasOrdenacao *metricas_out
) {
    if (lista == NULL) {
        return false;
    }

    if (lista->tamanho == 0) {
        printf("\nNenhum registro de partida encontrado no historico.\n");
        return true;
    }

    MetricasOrdenacao metricas;
    RegistroHistorico *copia = gerar_copia_ordenada(lista, criterio, &metricas);
    if (copia == NULL) {
        fprintf(stderr, "Erro: falha de memoria ao ordenar o historico.\n");
        return false;
    }

    const char *titulo_criterio = (criterio == ORDENAR_POR_PONTUACAO)
        ? "MAIOR PONTUACAO"
        : "ORDEM ALFABETICA (A-Z)";

    printf("\n========================================================================\n");
    printf(" HISTORICO DE PARTIDAS - ORDENADO POR: %s\n", titulo_criterio);
    printf("========================================================================\n");
    printf(" %-4s | %-28s | %-10s | %-9s | %-6s\n",
           "Pos", "Jogador", "Pontos", "Clientes", "Turnos");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < lista->tamanho; i++) {
        printf(" %-4d | %-28s | %-10d | %-9d | %-6d\n",
               i + 1,
               copia[i].nome_jogador,
               copia[i].pontuacao,
               copia[i].clientes_atendidos,
               copia[i].turnos_jogados);
    }

    printf("========================================================================\n");
    printf(" [Metricas Mergesort] Registros: %d | Comparacoes: %ld | Tempo: %.6f s\n",
           metricas.elementos_ordenados,
           metricas.comparacoes,
           metricas.tempo_segundos);
    printf("========================================================================\n");

    if (metricas_out != NULL) {
        *metricas_out = metricas;
    }

    /* Libera a cópia imediatamente; lista->dados segue intocada */
    free(copia);
    return true;
}