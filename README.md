# Controle de Tarefas com FreeRTOS

Este repositório contém um projeto de exemplo que demonstra o uso do FreeRTOS para implementar um sistema multitarefa. O sistema é composto por três tarefas principais: uma que sorteia números aleatórios, outra que imprime esses números e uma terceira que conta quantos números estão na fila.

## Estrutura do Projeto

O projeto é organizado em uma única aplicação que utiliza FreeRTOS, permitindo a execução simultânea de diferentes tarefas. As principais funções do projeto são:

1. **Sorteio de Números**: Gera números aleatórios entre 0 e 1 e os envia para uma fila.
2. **Impressão de Números**: Recebe números da fila e os imprime no console.
3. **Contagem de Números**: Monitora a quantidade de números presentes na fila a cada três segundos.

## Tecnologias Utilizadas

- FreeRTOS
- C/C++
- ESP-IDF (para desenvolvimento em ESP32)

## Como Usar

### Requisitos

- Um microcontrolador ESP32 com suporte para FreeRTOS.
- Ambiente de desenvolvimento ESP-IDF configurado.
