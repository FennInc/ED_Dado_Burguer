COMANDOS DE COMPILACAO PRA FACILITAR A VIDA

makefile:
make                 # compila e gera ./dado_burger
make game-run        # compila (se preciso) e executa com os arquivos de teste
make clean           # remove o executavel

Linux:
gcc -std=c99 -Wall -o dado_burger \
    $(find EDs -type f -name '*.c') \
    $(find EDs -type d -printf '-I%p ') \
    -lm

./dado_burger TXTs/pedidos.txt TXTs/historico.txt

Windows:
$fontes = Get-ChildItem EDs -Recurse -Filter *.c | Resolve-Path -Relative
$inc = Get-ChildItem EDs -Recurse -Directory | ForEach-Object { "-I" + (Resolve-Path -Relative $_.FullName) }
gcc -std=c99 -Wall -o dado_burger $fontes $inc -IEDs -lm

.\dado_burger.exe TXTs\pedidos.txt TXTs\historico.txt