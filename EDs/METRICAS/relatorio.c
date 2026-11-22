//IA foi utilizada para gerar parte da lógica de manipulação de arquivos e formatação no terminal.
//seus comentários são sinalizados pelo uso de '/* */'

#include <stdio.h>
#include <string.h>
#include "relatorio.h"
#include "avl.h"

#define REL_TAM_CAMINHO 256
#define REL_TAM_LINHA   512

static char g_caminho[REL_TAM_CAMINHO] = "medicoes.log";
static int  g_iniciado = 0;
static int  g_mostrarNaTela = 1;

/* ---------- Auxiliares internas ---------- */

/* Grava uma linha pronta na tela (se ligada) e no arquivo (sempre). */
static void emitir(const char *linha) {
    if (!g_iniciado) {
        relatorioIniciar(g_caminho); /* garante que o arquivo exista */
    }

    if (g_mostrarNaTela) {
        fputs(linha, stdout);
    }

    FILE *f = fopen(g_caminho, "a");
    if (f != NULL) {
        fputs(linha, f);
        fclose(f);
    }
}

//ecreve o titulo da seção
static void emitirSecao(const char *titulo) {
    char linha[REL_TAM_LINHA];
    snprintf(linha, sizeof(linha), "-- %s --\n", titulo);
    emitir(linha);
}

//escreve variavel do tipo int
static void emitirInt(const char *nome, long valor) {
    char linha[REL_TAM_LINHA];
    snprintf(linha, sizeof(linha), "%s: %ld\n", nome, valor);
    emitir(linha);
}

//escreve variavel do tipo double
static void emitirDouble(const char *nome, double valor, int casas,
                         const char *unidade) {
    char linha[REL_TAM_LINHA];
    snprintf(linha, sizeof(linha), "%s: %.*f%s\n", nome, casas, valor, unidade);
    emitir(linha);
}

/* ---------- Interface publica ---------- */

//inicia o relatório
int relatorioIniciar(const char *caminho) {
    if (caminho != NULL) {
        strncpy(g_caminho, caminho, REL_TAM_CAMINHO - 1);
        g_caminho[REL_TAM_CAMINHO - 1] = '\0';
    }
    g_iniciado = 1;

    FILE *f = fopen(g_caminho, "w"); /* cria ou sobrescreve */
    if (f == NULL) {
        fprintf(stderr, "Erro: nao foi possivel criar '%s'\n", g_caminho);
        return -1;
    }
    fclose(f);
    return 0;
}

//verifica se deve printar o relatorio
void relatorioMostrarNaTela(int ligado) {
    g_mostrarNaTela = ligado ? 1 : 0;
}

void relatorioParametros(unsigned int semente, int numClientes) {
    emitirSecao("Partida: parametros");
    emitirInt("Semente", (long)semente);
    emitirInt("Clientes na partida", numClientes);
}

void relatorioTitulo(void) {
    emitir("=== RELATORIO DE DESEMPENHO ===\n");
}

void relatorioCarregamentoAvl(const NodoAvl *raiz, double tempo) {
    int totalNos  = avlContarNos(raiz);
    int alturaArv = avlAltura(raiz) + 1; /* convencao da spec: vazia = 0, so raiz = 1 */

    emitirSecao("AVL: carregamento");
    emitirInt("Nos inseridos", totalNos);
    emitirInt("Altura resultante", alturaArv);
    emitirInt("Rotacoes (insercao)", g_rotacoes);
    emitirDouble("Tempo de carregamento", tempo, 6, " s");
}

void relatorioHistoricoCarregado(long entradas) {
    emitirSecao("Historico: carregamento");
    emitirInt("Entradas carregadas", entradas);
}

void relatorioBuscasAvl(long buscas, long comparacoes,
                        double mediaComparacoes, double tempoTotal) {
    emitirSecao("AVL: buscas durante a partida");
    emitirInt("Total de buscas", buscas);
    emitirInt("Total de comparacoes", comparacoes);
    emitirDouble("Media de comparacoes por busca", mediaComparacoes, 2, "");
    emitirDouble("Tempo total acumulado", tempoTotal, 6, " s");
}

void relatorioHash(int m, long n, double fatorCarga,
                   long hits, long misses, double taxaAcerto, long colisoes) {
    emitirSecao("Tabela hash");
    emitirInt("Tamanho (m)", m);
    emitirInt("Entradas ocupadas (n)", n);
    emitirDouble("Fator de carga (lambda)", fatorCarga, 2, "");
    emitirInt("Total de hits", hits);
    emitirInt("Total de misses", misses);
    emitirDouble("Taxa de acerto", taxaAcerto, 1, "%");
    emitirInt("Total de colisoes", colisoes);
}

void relatorioOrdenacao(long tamanho, const char *algoritmo, double tempo) {
    char linha[REL_TAM_LINHA];

    emitirSecao("Ordenacao do historico (ao acessar Historico)");

    snprintf(linha, sizeof(linha), "Tamanho: %ld entradas\n", tamanho);
    emitir(linha);
    emitirDouble(algoritmo, tempo, 6, " s");
}

void relatorioFim(void) {
    emitir("================================\n");
}