## Projeto 2 - 2020/2 - FSE

__Aluno__: Elias Bernardo Marques Magalhães
__Matrícula__: 150/009011

Este trabalho tem por objetivo a criação de um sistema distribuído de automação residencial para monitoramento e acionamento de sensores e dispositivos. O sistema deve ser desenvolvido para funcionar em um conjunto de placas Raspberry Pi com um servidor central responsável pelo controle e interface com o usuário e servidores distribuídos para leitura e acionamento dos dispositivos. Dentre os dispositivos envolvidos estão o monitoramento de temperatura, sensores de presença, sensores de abertura e fechamento de portas e janelas, acionamento de lâmpadas, aparelhos de ar-condicionado e alarme.

## Compilação e utilização

Instruções mais detalhadas se encontram no readme das pastas __central/__ e __distribuido/__

## Detalhes da implementação

### Sockets 

A implementação dos sockets foi feita de forma bi-direcional, sendo aberto um socket no servidor central que fica aguardando o servidor distribuido se conectar, mantendo a conexão aberta enquanto ambos estiverem rodando. 

As mensagens são enviadas utilizando o seguinte formato:

    "evento";"payload"

Sendo evento o nome do evento que especifica o tipo da mensagem, e payload o conteúdo. Ambos os servidores fazem o parse das mensagens com a função strtok e, a partir do evento, jogam as mensagens para seus respectivos event handlers (numa ideia de _pub-sub_).

### Sensores 

Para a detecção de input ou não dos sensores, é utilizado o wiringPiSR, sendo criada uma trhread apenas para as interrupções