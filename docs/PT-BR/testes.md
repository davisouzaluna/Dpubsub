# Realizando os testes do DpubSub

Para realizar os testes com o Dpubsub, primeiro é importante realizar a compilação do projeto. Para saber mais, veja em [Compilando](./Compilando.md).

Logo depois de realizar a compilação, entre dentro do diretório de testes(lembrando que você deve esta dentro do diretório de build). Lá você vai encontrar os executáveis:

- `api_test`
- `client_test` 

### `api_test`

Ele vai testar as funções de `client_tcp_api.h`, se conectando, publicando e se subscrevendo em um dado tópico no Broker.

### `client_test`

Ele vai testar as funções primárias de `client.h` e `packets.h`, realizando a subscrição, publicação, enviando um ping, conexão e desconexão com o broker. Uma função experimental é a manipulação de pacotes do tipo *PUBLISH*

