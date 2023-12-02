# Board-game-with-in-C
O programa se trata de um jogo de tabuleiro onde temos dois jogadores e o vencedor será quem tiver mais pedras no tabuleiro ao final do jogo. O jogo foi desenvolvido em C e está disponível em duas versões, uma versão utilizando pthread e outra utilizando chamadas de sistemas de compartilhamento de memória entre processos.

# Regras do jogo
* Jogagor 1 é representado pelas pedras vermelhas (V) e jogador 2 é representado pelas pedras pretas (P).
* Cada jogador pode colocar as suas pedras apenas nas casas adjacentes às suas pedras existentes no tabuleiro, desde que estas casas estejam vazias.

# Condições de conclusão
* Caso um jogador tenha mais peças do que outro jogador quando o tabuleiro estiver totalmente completo, ele será o vencedor.
* Caso um jogador não tenha mais movimentos válidas disponíveis, o vencedor será o seu adversário.
* Caso ambos os jogadores tenham a mesma quantidade de pedras no tabuleiro, e o tabuleiro estiver totalmente completo, o jogo terminará em empate. 

# Como jogar
* Ao clonar o repositório ou fazer o download do diretório do projeto, navegue até o subdiretório Pthreads ou SystemCalls (Ambos são o mesmo jogo, são apenas versões de implementação diferentes) e então execute os comandos.

Para compilar o código fonte do jogo:
```
make
```

Para executar o jogo:
```
make run
```