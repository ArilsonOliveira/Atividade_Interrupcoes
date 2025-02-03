# Projeto de Controle de LEDs e Botões com RP2040

Este projeto utiliza o microcontrolador RP2040 da placa BitDogLab, conectada à matriz de LEDs 5x5 WS2812 e LEDs RGB, para demonstrar o controle de LEDs e manipulação de botões utilizando interrupções e debouncing. O projeto também explora a utilização de LEDs comuns, resistores de pull-up internos e a aplicação de interrupções para manipulação de botões.

## Vídeo de Demonstração

link: <https://drive.google.com/file/d/1o381WB0-3btwcqcVjnY2B-og84pEKRZo/view?usp=sharing>

## Funcionalidades

- LED RGB pisca continuamente 5 vezes por segundo.
- O Botão A incrementa o número exibido na matriz de LEDs.
- O Botão B decrementa o número exibido na matriz de LEDs.
- A matriz de LEDs WS2812 exibe os números de 0 a 9 de forma clara, utilizando um formato fixo (ex.: estilo digital).
- Debouncing é implementado via software para garantir que o acionamento dos botões seja confiável.

## Componentes Utilizados

- Matriz de LEDs WS2812 5x5 (conectada à GPIO 7).
- LED RGB (conectado às GPIOs 11, 12 e 13).
- Botões A (GPIO 5) e B (GPIO 6).
- Resistores de pull-up internos nos botões.
  
## Requisitos

- Uso de interrupções: As funcionalidades dos botões devem ser implementadas utilizando interrupções (IRQ).
- Debouncing: O tratamento do bouncing dos botões é feito via software.
- Controle de LEDs: O projeto faz uso de LEDs comuns e LEDs WS2812, demonstrando controle de diferentes tipos de LEDs.
- Organização do código: O código está bem estruturado e comentado para fácil entendimento.

## Dependências
- pico/stdlib.h: Biblioteca padrão do microcontrolador.
- hardware/pio.h: Para controle do PIO (usado no controle dos LEDs WS2812).
- hardware/timer.h: Para configuração de timers.
- ws2812.pio.h: Biblioteca específica para o controle dos LEDs WS2812.

## Estrutura do Código

Arquivos
├── main.c
├── ws2812.pio
├── CMakeLists.txt
├── README.md
├── .gitignore
├── wokwi.toml
├── diagram.json
├── pico_sdk_important.cmake

## Principais Funções

- main(): Controle principal do projeto, onde o LED RGB pisca e os botões A e B alteram o número exibido na matriz de LEDs.
- configurar_matriz_leds(): Configura a matriz de LEDs WS2812.
- atualizar_matriz(): Exibe o número atual na matriz de LEDs.
- tratar_botao(): Função de interrupção que trata os pressionamentos dos botões A e B, incrementando ou decrementando o número exibido.
- led_vermelho(): Função de interrupção do timer que controla o piscar do LED vermelho.
- converter_rgb_para_grb(): Converte valores RGB para o formato GRB compatível com o WS2812.

##Configuração de Hardware

- GPIO 7: Conexão com a matriz de LEDs WS2812.
- GPIOs 11, 12, 13: Conexões com o LED RGB.
- GPIO 5: Botão A (Incrementa o número).
- GPIO 6: Botão B (Decrementa o número).

## Funcionamento do Projeto
* LED vermelho: Pisca a uma frequência de 5 Hz, usando um timer para alternar seu estado.
* Botão A: Incrementa o número exibido na matriz de LEDs.
* Botão B: Decrementa o número exibido na matriz de LEDs.
* Matriz de LEDs 5x5: Exibe os números de 0 a 9, utilizando o formato digital.
* Debouncing: O software garante que cada pressionamento de botão seja tratado apenas uma vez, mesmo que o botão seja pressionado rapidamente.

## Considerações Finais
Este projeto foi desenvolvido para explorar conceitos de interrupções, debouncing de botões e controle de LEDs, utilizando o microcontrolador RP2040 e a placa BitDogLab. O projeto visa consolidar os conceitos de manipulação de hardware e software, além de proporcionar uma prática de controle de LEDs em diversos formatos e tipos.

