#include<stdio.h>
void novoJogo(NodoAvl* raizPedidos, TabelaHash* cache) {
    // SETUP DA PARTIDA
    int turnoAtual = 1;
    bool gameOver = false;
    bool vitoria = false;
    int pontuacao = 0;
    
    // Inicialização das estruturas 
    FilaClientes* fila = criarFilaClientes();
    PilhaMontagem* hamburguerAtual = criarPilhaMontagem();
    HeapPereciveis* heapCarne = criarHeap();
    HeapPereciveis* heapBacon = criarHeap();
    
    // Sortear e enfileirar os clientes desta partida
    // Opcional: perguntar ao usuário a dificuldade ou ler de um config
    gerarClientes(fila, 5); // Exemplo: 5 clientes na fila
    
    // Carrega a receita do primeiro cliente buscando na Hash/AVL
    ListaReceita* receitaAtual = buscarPedido(fila->frente->idPedido, raizPedidos, cache);

    // GAME LOOP 
    while (!gameOver) {
        limparTelaTerminal(); // system("clear") o
        
        // Renderização da interface em texto (HUD)
        imprimirInterface(fila, hamburguerAtual, receitaAtual, heapCarne, heapBacon, turnoAtual);
        
        //  ENTRADA DO JOGADOR
        char acao;
        printf("\nEscolha sua acao (P = Preparar, A = Adicionar, R = Remover, S = Servir, Q = Sair): ");
        scanf(" %c", &acao);
        
        //  PROCESSAMENTO DA AÇÃO
        int turnosGastos = 0;
        switch (toupper(acao)) {
            case 'P':
                prepararIngrediente(heapCarne, turnoAtual); // Ex: insere no Heap
                turnosGastos = 1;
                break;
            case 'A':
                adicionarIngrediente(hamburguerAtual, heapCarne); 
                turnosGastos = 1;
                break;
            case 'R':
                removerIngrediente(hamburguerAtual);
                turnosGastos = 1;
                break;
            case 'S':
                if (validarPedido(hamburguerAtual, receitaAtual)) {
                    printf("\nPedido entregue com sucesso!\n");
                    pontuacao += 100;
                    desenfileirarCliente(fila);
                    esvaziarPilha(hamburguerAtual); // Limpa para o próximo
                    
                    if (!filaVazia(fila)) {
                        receitaAtual = buscarPedido(fila->frente->idPedido, raizPedidos, cache);
                    }
                } else {
                    printf("\nHamburguer incorreto!\n");
                }
                turnosGastos = 1; // Tentar servir também gasta turno
                break;
            case 'Q':
                gameOver = true;
                break;
            default:
                printf("\nAcao invalida!\n");
                continue; // Não avança o turno
        }
        
        //  ATUALIZAÇÃO DE ESTADO E REGRAS DE NEGÓCIO
        if (turnosGastos > 0) {
            turnoAtual += turnosGastos;
            
            // Reduz paciência do cliente do topo da fila
            fila->frente->paciencia -= turnosGastos;
            
            // Remove perecíveis estragados do topo dos Heaps
            limparEstragados(heapCarne, turnoAtual);
            limparEstragados(heapBacon, turnoAtual);
            
            // Condições de fim de jogo
            if (fila->frente->paciencia <= 0) {
                gameOver = true;
                vitoria = false;
            } else if (filaVazia(fila)) {
                gameOver = true;
                vitoria = true;
            }
        }
    }

    // (Fim de jogo)
    if (vitoria) {
        printf("\nVoce venceu! Pontuacao: %d\n", pontuacao);
    } else {
        printf("\nO cliente perdeu a paciencia! Fim de Jogo.\n");
    }
    
    // Salvar no histórico
    salvarHistorico(pontuacao);
    liberarFila(fila);
    liberarPilha(hamburguerAtual);
    liberarHeap(heapCarne);
    liberarHeap(heapBacon);
}