#include<stdio.h>
#define MAX_LINHA 200
#define TAM_LINHA 20
int main ( int argc , char * argv []) {
    int total = 0;
    JOGADOR jogadores[MAX_LINHA]={};
    if ( argc < 3) {
    fprintf ( stderr , " Uso : % s < pedidos > < historico >\ n" , argv
    [0]) ;
    return 1;
    }
    FILE * fp = fopen ( argv [1] , " r") ; /* abre o arquivo de pedidos*/
    if(fp==NULL){
        printf("Erro de abertura do arquivo de pedidos");
        return 1;
    }
    FILE * fh = fopen ( argv [2] , " r") ; /* abre o arquivo de
    historico */
    if(fh==NULL){
        printf("Erro de abertura do arquivo de historicos");
        return 1;
    }
    char linha_temp[TAM_LINHA];
    while (total < MAX_LINHA) {
        
        // Lê a primeira linha (Nome)
        if (fgets(vetor_jogadores[total].nome, TAM_LINHA, fh) == NULL) {
            break; // Fim do arquivo
        }
        
        // Limpa o '\n' do nome
        jogadores[total].nome[strcspn(jogadores[total].nome, "\n")] = '\0';

        // Lê a segunda linha (Pontuação)
        if (fgets(linha_temp, TAM_LINHA, fh) == NULL) {
            printf("Aviso: O arquivo terminou sem a pontuacao do ultimo jogador.\n");
            break; 
        }
        
        // 4. Converte a string da pontuação para número inteiro (int) e salva na struct
        jogadores[total].pontuacao = atoi(linha_temp);

        total++;
    }

}