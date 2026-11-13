#include <stdio.h>
enum{} estadoAtual;
void impressao_menu_principal(){
    printf("<<<MENU>>>
            1)NOVO JOGO---    
            2)COMO JOGAR--
            3)HISTORICO-- 
            Choose an option please");
}
void como_jogar(){
    printf("REGRAS BASICAS\n");
    printf("OBJETIVO DO JOGO\n.O jogador é o mais novo funcionário do Dado Burger e deverá atender aos pedidos
de uma série de clientes ao longo do dia.");
    printf("LISTA DE INGREDIENTES E SEUS SIMBOLOS\n");
    printf("
                            Pão (de cima)\t z 
                            Ketchup\t k 
                            Mostarda\t m 
                            Picles\t p 
                            Alface\t a 
                            Queijo suíço\t s 
                            Queijo cheddar\t q 
                            Cebola\t c 
                            Tomate\t t 
                            Carne\t n 
                            Ovo frito\t o 
                            Bacon\t b ");
    printf("Ações do jogador
    As seguintes ações consomem um turno cada:
    • Preparar um ingrediente
    • Adicionar um ingrediente ao hambúrguer
    • Remover o ingrediente do topo do hambúrguer
    Ações de consulta (ver estoque, ver pedido, etc.) não consomem turno.");
    printf("CONDICOES DE VITORIA E DE DERROTA\n");
    printf("Derrota. Se a paciência do cliente da frente chegar a zero antes de o pedido ser concluído,voce perdeu .");
    printf("Vitória. Se todos os clientes forem atendidos sem que nenhum perca a paciência ,venceu. ");
}

    