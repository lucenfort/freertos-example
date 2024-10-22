#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Struct para armazenar dados das tarefas
typedef struct {
    xTaskHandle tarefa_sortear;
    xTaskHandle tarefa_imprimir;
    xTaskHandle tarefa_contar;
    xQueueHandle fila_numeros;
} Tarefas;

// Função para sortear números
void sortear(void * args)
{
    Tarefas * tarefas = (Tarefas*)args;
    printf("Tarefa Sorteadora iniciada.\n");
    
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    char num[12];

    while(1) {
        float numero = (rand() % 100) / 100.0; // Sorteio de número entre 0 e 1
        snprintf(num, sizeof(num), "%0.2f\r\n", numero); // Formatação da string

        // Envio do número para a fila
        if (xQueueSendToBack(tarefas->fila_numeros, &num, 0) != pdPASS) {
            printf("Falha ao enviar número para a fila.\n");
        } else {
            printf("Número sorteado: %s", num);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Pausa de 1 segundo
    }
}

// Função para imprimir números
void imprimir(void * args)
{
    Tarefas * tarefas = (Tarefas*)args;
    printf("Tarefa Impressora iniciada.\n");
    
    char num[12];
    while(1) {
        // Recebe número da fila
        if(xQueueReceive(tarefas->fila_numeros, &num, portMAX_DELAY) == pdTRUE) {
            printf("Número escolhido: %s\n", num);
        }

        vTaskDelay(500 / portTICK_PERIOD_MS); // Pausa de 0.5 segundo
    }
}

// Função para contar números na fila
void contar(void * args)
{
    Tarefas * tarefas = (Tarefas*)args;
    printf("Tarefa Contadora iniciada.\n");

    while(1) {
        printf("Números na fila: %d\n", uxQueueMessagesWaiting(tarefas->fila_numeros));
        vTaskDelay(3000 / portTICK_PERIOD_MS); // Pausa de 3 segundos
    }
}

extern "C" void app_main()
{
    Tarefas * tarefas = (Tarefas*) malloc(sizeof(Tarefas));

    // Criação da fila
    tarefas->fila_numeros = xQueueCreate(10, sizeof(char[12])); // Corrigido para armazenar strings

    // Criação das tarefas
    xTaskCreatePinnedToCore(sortear, "Sorteia", 8192, tarefas, 5, &tarefas->tarefa_sortear, 0);
    xTaskCreatePinnedToCore(imprimir, "Imprime", 8192, tarefas, 5, &tarefas->tarefa_imprimir, 1);
    xTaskCreatePinnedToCore(contar, "Conta", 8192, tarefas, 5, &tarefas->tarefa_contar, 0);
}
