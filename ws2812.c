#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

// Definições
#define IS_RGBW false
#define MATRIZ_LEDS 25
#define WS2812 7
#define BOTAO_A 5
#define BOTAO_B 6
#define LED_RED_PIN 13
#define LED_B_PIN 12
#define LED_G_PIN 11
#define DEBOUNCE_TIME 200
#define TOTAL_CORES (sizeof(cores) / sizeof(cores[0]))

// Variáveis globais do programa
int numero_atual = 0;
uint32_t buffer_leds[MATRIZ_LEDS] = {0};
volatile bool led_red_state = false;
volatile bool atualizar_display = false;
volatile uint32_t ultimo_botao_a = 0;
volatile uint32_t ultimo_botao_b = 0;
int indice_cor = 0; // definir cor atual

// Convertendo RGB para GRB 
static inline uint32_t converter_rgb_para_grb(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função para enviar um pixel para a matriz de LEDs 
static inline void set_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); // são invertidas as ordens dos bits para ser compativel com a biblioteca ws2812
    sleep_us(10); 
}


// Função para configuração da matriz de LEDs 5x5 WS2812
void configurar_matriz_leds() {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812, 800000, IS_RGBW);
}



// Função para mostrar cada número na matriz de LEDs
void atualizar_matriz(int numero, uint32_t cor) {
    static const uint32_t frames_numeros[10][MATRIZ_LEDS] = {
        
        {0,1,1,1,0, 
         0,1,0,1,0, 
         0,1,0,1,0, // numero 0
         0,1,0,1,0, 
         0,1,1,1,0},
        //--------------------------
        {0,1,1,1,0, 
         0,0,1,0,0,  
         0,0,1,0,0,  // numero 1
         0,1,1,0,0,  
         0,0,1,0,0},
        //--------------------------
        {0,1,1,1,0,
         0,1,0,0,0, 
         0,1,1,1,0, // numero 2
         0,0,0,1,0, 
         0,1,1,1,0},
        //--------------------------
        {0,1,1,1,0, 
         0,0,0,1,0,  // numero 3
         0,1,1,1,0, 
         0,0,0,1,0, 
         0,1,1,1,0},
        //--------------------------
        {0,1,0,0,0, 
         0,0,0,1,0,  // numero 4
         0,1,1,1,0, 
         0,1,0,1,0, 
         0,1,0,1,0},
        //--------------------------
        {0,1,1,1,0, 
         0,0,0,1,0,  // numero 5
         0,1,1,1,0, 
         0,1,0,0,0, 
         0,1,1,1,0},
        //--------------------------
        {0,1,1,1,0, 
         0,1,0,1,0,  // numero 6
         0,1,1,1,0, 
         0,1,0,0,0, 
         0,1,1,1,0},   
        //--------------------------
        {0,1,0,0,0, 
         0,0,0,1,0, // numero 7
         1,1,1,0,0, 
         0,0,0,1,0, 
         0,1,1,1,0},
        //--------------------------
        {0,1,1,1,0, 
         0,1,0,1,0,  // numero 8
         0,1,1,1,0, 
         0,1,0,1,0, 
         0,1,1,1,0},
         //--------------------------
        {0,1,1,1,0, 
         0,0,0,1,0,  // numero 9
         0,1,1,1,0, 
         0,1,0,1,0, 
         0,1,1,1,0}
    };
    
    for (int i = 0; i < MATRIZ_LEDS; i++) {
        buffer_leds[i] = frames_numeros[numero][i] ? cor :0; // converter_rgb_para_grb(0, 10, 10) : 0;
    }
    
    for (int i = 0; i < MATRIZ_LEDS; i++) {
        set_pixel(buffer_leds[i]);
    }
}

// Função de interrupção para os botões
void tratar_botao(uint gpio, uint32_t events) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (gpio == BOTAO_A && (tempo_atual - ultimo_botao_a > DEBOUNCE_TIME)) {
        ultimo_botao_a = tempo_atual;
        numero_atual = (numero_atual + 1) % 10;
        atualizar_display = true;
    } else if (gpio == BOTAO_B && (tempo_atual - ultimo_botao_b > DEBOUNCE_TIME)) {
        ultimo_botao_b = tempo_atual;
        numero_atual = (numero_atual - 1 + 10) % 10;
        atualizar_display = true;
    }
}

// Callback da interrupção do timer para piscar o LED vermelho
bool piscar_led_vermelho(struct repeating_timer *t) {
    led_red_state = !led_red_state;
    gpio_put(LED_RED_PIN, led_red_state);
    return true;
}

int main() {
    stdio_init_all();
    sleep_ms(1000);
    configurar_matriz_leds();
    
    uint32_t cores[] = {
    converter_rgb_para_grb(25, 0, 0),    // Vermelho
    converter_rgb_para_grb(0, 25, 0),    // Verde
    converter_rgb_para_grb(0, 0, 25),    // Azul
    converter_rgb_para_grb(25, 25, 0),  // Amarelo
    converter_rgb_para_grb(0, 25, 25),  // Ciano
    converter_rgb_para_grb(25, 0, 25),  // Magenta
    converter_rgb_para_grb(25, 14, 0),  // Laranja
    converter_rgb_para_grb(5, 0, 12),   // Índigo
    converter_rgb_para_grb(12, 12, 12) // Branco
};
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, tratar_botao);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, tratar_botao);

    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    struct repeating_timer timer;
    add_repeating_timer_ms(-100, piscar_led_vermelho, NULL, &timer);
    atualizar_matriz(numero_atual, cores[indice_cor]);

    

    while (1) {
        if (atualizar_display) {
            atualizar_matriz(numero_atual, cores[indice_cor]);
            // alternar para a próxima cor 
            indice_cor = (indice_cor + 1) % (sizeof(cores) / sizeof(cores[0]));
            atualizar_display = false;
        }
        tight_loop_contents();
    }
    return 0;
}