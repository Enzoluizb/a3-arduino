# Projeto A3 Arduino

Este projeto foi desenvolvido como parte do curso A3 de Arduino, pelos integrantes da equipe Enzo Luiz Bertoli e Gabriel de Sousa.

## Descrição

O projeto consiste em um exemplo de programação concorrente para microcontroladores usando a biblioteca ESP-NOW no ambiente Arduino. A programação concorrente é utilizada para lidar com múltiplas tarefas simultaneamente, neste caso, implementando um produtor e um consumidor que se comunicam através do protocolo ESP-NOW.

O produtor gera letras aleatórias e as coloca em um buffer compartilhado, enquanto o consumidor solicita letras e as remove do buffer, se disponíveis.

## Integrantes da Equipe

- Enzo Luiz Bertoli
- Gabriel de Sousa

## Repositório

O código-fonte do projeto está disponível no seguinte repositório do GitHub:

[https://github.com/Enzoluizb/a3-arduino](https://github.com/Enzoluizb/a3-arduino)

## Configuração e Uso

1. **Configuração Inicial:**
   - Certifique-se de ter o ambiente Arduino configurado corretamente.
   - Instale as bibliotecas necessárias (`esp_now.h`, `WiFi.h`, etc.).

2. **Upload do Código:**
   - Faça o upload do código para o seu microcontrolador Arduino compatível com ESP8266/ESP32.

3. **Execução:**
   - Após o upload, os dispositivos ESP podem ser configurados como produtores ou consumidores.
   - Eles se comunicarão através do protocolo ESP-NOW.

## Contribuições

Contribuições são bem-vindas! Se você encontrar problemas ou tiver sugestões de melhorias, sinta-se à vontade para abrir uma "issue" ou enviar um "pull request" no repositório.

## Licença

Este projeto está licenciado sob a [Licença MIT](LICENSE).
