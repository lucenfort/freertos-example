#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Struct que irá guardar os dados das tarefas
typedef struct {
    xTaskHandle tarefa_sortear;
    xTaskHandle tarefa_imprimir;
    xTaskHandle tarefa_contar;
    xQueueHandle fila_numeros;
} Tarefas;

// Função para sortear o número
void sortear(void * args)
{
    Tarefas * tarefas = (Tarefas*)args;
    printf("Tarefa Sorteadora iniciada.\n");

    // Criação da semente da função random
    srand(time(NULL));

    // Variáveis que irão conter os números, e uma para contagem de execuções
    float numero;
    int cont = 0;

    while(1) {

        numero = (rand() % 100)/100.0; // Sorteio

        char num[12];
        snprintf(num, 11, "%0.2f\r\n", numero); // Formatação da string

        // Enviando número para a fila
        xQueueSendToBack(tarefas->fila_numeros, num, 0);

        printf("Número %d: %s", cont, num);
        cont++;

        vTaskDelay(1000/portTICK_PERIOD_MS); // Pausando a tarefa por 1 segundo
    }
}

// Função para Imprimir
void imprimir(void * args)
{
    Tarefas * tarefas = (Tarefas*)args;
    printf("Tarefa Impressora iniciada.\n");
    
    char num[12];
    while(1) {
        // Recepcionar número da fila
        if(xQueueReceive(tarefas->fila_numeros, num, 0) == pdTRUE)
            printf("Número escolhido: %s\n", num);

        vTaskDelay(500/portTICK_PERIOD_MS); // Pausando a tarefa por 0.5 segundo
    }
}

// Função para contar
void contar(void * args)
{
    Tarefas * tarefas = (Tarefas*)args;
    printf("Tarefa Contadora iniciada.\n");

    while(1) {
        printf("Números na pilha: %d\n", uxQueueMessagesWaiting(tarefas->fila_numeros));

        vTaskDelay(3000/portTICK_PERIOD_MS); // Pausando a tarefa por 3 segundos
    }
}

extern "C" void app_main()
{
    // Alocação de memmória para a estrutura Tarefas
    Tarefas * tarefas = (Tarefas*) malloc(sizeof(Tarefas));

    // Criação da fila a ser usada
    tarefas->fila_numeros = xQueueCreate(10, sizeof(char*));

    // Alocação da memória para as tarefas
    xTaskCreatePinnedToCore(sortear, "Sorteia", 8192, tarefas, 5, &tarefas->tarefa_sortear, 0);
    xTaskCreatePinnedToCore(imprimir, "Imprime", 8192, tarefas, 5, &tarefas->tarefa_imprimir, 1);
    xTaskCreatePinnedToCore(contar, "Conta", 8192, tarefas, 5, &tarefas->tarefa_contar, 0);
}