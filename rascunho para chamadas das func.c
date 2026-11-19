//historicos_func
ListaHistorico hist;
if (!inicializar_historico(&hist, 16)) {
    return 1;
}

/* 1. Na abertura do programa (lê o arquivo se existir, ou segue vazio se não existir) */
if (!carregar_historico(&hist, argv[2])) {
    liberar_historico(&hist);
    return 1;
}

/* 2. Ao término de cada partida: anexa o resultado e persiste no disco */
RegistroHistorico nova_partida = { "Pedro", 1250, 6, 34 };
inserir_historico(&hist, nova_partida);
salvar_historico(&hist, argv[2]);

/* 3. Antes de encerrar o programa: libera toda a memória dinâmica */
liberar_historico(&hist);
renderizar_tela_jogo(
    turno,
    pontos,
    fila.tamanho,
    cliente_atual(&fila),
    &pedido_atual,
    &lanche_na_bandeja,
    &estoque_visual
);

int acao = ler_opcao_inteira(" Escolha sua acao: ", 0, 4);