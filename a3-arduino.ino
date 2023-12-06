#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h> 

// Definições de buffer
#define TAMANHO_BUFFER 9
char buffer[TAMANHO_BUFFER];
int contadorBuffer = 0;

// Semáforos e Mutex
SemaphoreHandle_t mutexBuffer;
SemaphoreHandle_t espacosVazios;
SemaphoreHandle_t espacosOcupados;

// Função para enviar resposta via ESP-NOW
void enviarResposta(const uint8_t *macAddr, const char *resposta) {
  esp_now_send(macAddr, (uint8_t *)resposta, strlen(resposta));
}

// Função para o produtor (ESPs que enviam letras)
void tarefaProdutor(void *param) {
  while (1) {
    // Simula a produção de um item (letra)
    char novoItem = 'a' + rand() % 26;

    // Aguarda por uma vaga livre no buffer
    if (xSemaphoreTake(espacosVazios, portMAX_DELAY) == pdTRUE) {
      // Adquire o mutex para acessar o buffer
      xSemaphoreTake(mutexBuffer, portMAX_DELAY);

      // Adiciona o item ao buffer
      buffer[contadorBuffer] = novoItem;
      contadorBuffer++;

      // Libera o mutex
      xSemaphoreGive(mutexBuffer);

      // Libera uma vaga ocupada
      xSemaphoreGive(espacosOcupados);

      // Envia resposta ao produtor
      enviarResposta((uint8_t *)param, "Item produzido com sucesso");
    }

    // Aguarda 5 segundos antes da próxima produção
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

// Função para o consumidor (ESPs que solicitam letras)
void tarefaConsumidor(void *param) {
  while (1) {
    // Simula a solicitação de um item (letra)
    char itemSolicitado = 'a' + rand() % 26;

    // Aguarda por um item disponível no buffer
    if (xSemaphoreTake(espacosOcupados, portMAX_DELAY) == pdTRUE) {
      // Adquire o mutex para acessar o buffer
      xSemaphoreTake(mutexBuffer, portMAX_DELAY);

      // Procura o item solicitado no buffer
      char resposta[20];
      bool itemEncontrado = false;
      for (int i = 0; i < contadorBuffer; i++) {
        if (buffer[i] == itemSolicitado) {
          snprintf(resposta, sizeof(resposta), "Item consumido: %c", itemSolicitado);
          itemEncontrado = true;

          // Remove o item do buffer
          for (int j = i; j < contadorBuffer - 1; j++) {
            buffer[j] = buffer[j + 1];
          }
          contadorBuffer--;

          break;
        }
      }

      // Libera o mutex
      xSemaphoreGive(mutexBuffer);

      // Libera uma vaga livre
      xSemaphoreGive(espacosVazios);

      // Envia resposta ao consumidor
      if (itemEncontrado) {
        enviarResposta((uint8_t *)param, resposta);
      } else {
        enviarResposta((uint8_t *)param, "N.D. (Não Disponível)");
      }
    }

    // Aguarda 5 segundos antes da próxima solicitação
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Inicializa o sistema ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar o ESP-NOW");
    return;
  }

  // Configura a função de callback para receber mensagens ESP-NOW
  esp_now_register_recv_cb([](const uint8_t *macAddr, const uint8_t *data, int len) {
    Serial.printf("Recebido de %02X:%02X:%02X:%02X:%02X:%02X - %s\n",
                  macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5], data);

    // Verifica se a mensagem é de produtor ou consumidor e executa a tarefa correspondente
    if (strcmp((const char *)data, "produtor") == 0) {
      xTaskCreate(tarefaProdutor, "Produtor", 2048, (void *)macAddr, 1, NULL);
    } else if (strcmp((const char *)data, "consumidor") == 0) {
      xTaskCreate(tarefaConsumidor, "Consumidor", 2048, (void *)macAddr, 1, NULL);
    }
  });

  // Inicializa os semáforos e mutex
  mutexBuffer = xSemaphoreCreateMutex();
  espacosVazios = xSemaphoreCreateCounting(TAMANHO_BUFFER, TAMANHO_BUFFER);
  espacosOcupados = xSemaphoreCreateCounting(TAMANHO_BUFFER, 0);

  // Inicializa o buffer vazio
  for (int i = 0; i < TAMANHO_BUFFER; i++) {
    buffer[i] = ' ';
  }
}

void loop() {
  // Loop principal vazio, as tarefas são executadas em threads separadas
}
