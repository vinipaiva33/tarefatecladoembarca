#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"

#define BLUE_LED_PIN 10    // Pino do LED Azul
#define RED_LED_PIN 9      // Pino do LED Vermelho
#define GREEN_LED_PIN 14   // Pino do LED Verde
#define BUZZER_PIN 22      // Pino do Buzzer

#define ROW_0 8            // Pinos do KeyPad
#define ROW_1 7
#define ROW_2 6
#define ROW_3 5
#define COL_0 4
#define COL_1 3
#define COL_2 2
#define COL_3 26           // ADC_VREF para Keypad C4

void init_gpio() {
    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);

    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);

    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    int rows[] = {ROW_0, ROW_1, ROW_2, ROW_3};
    int cols[] = {COL_0, COL_1, COL_2, COL_3};

    for (int i = 0; i < 4; i++) {
        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_OUT);
        gpio_put(rows[i], 1);

        gpio_init(cols[i]);
        gpio_set_dir(cols[i], GPIO_IN);
        gpio_pull_down(cols[i]);
    }
}

int read_keypad() {
    int rows[] = {ROW_0, ROW_1, ROW_2, ROW_3};
    int cols[] = {COL_0, COL_1, COL_2, COL_3};

    int key_map[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int r = 0; r < 4; r++) {
        gpio_put(rows[r], 0);  // Ativa a linha
        sleep_ms(10);  // Pequeno delay para estabilização
        for (int c = 0; c < 4; c++) {
            if (gpio_get(cols[c])) {
                gpio_put(rows[r], 1);  // Desativa a linha
                sleep_ms(200);  // Debounce delay
                return key_map[r][c];
            }
        }
        gpio_put(rows[r], 1);  // Desativa a linha
    }

    return -1;  // Nenhuma tecla pressionada
}

void control_leds_and_buzzer(int command) {
    gpio_put(BLUE_LED_PIN, 0);
    gpio_put(RED_LED_PIN, 0);
    gpio_put(GREEN_LED_PIN, 0);
    gpio_put(BUZZER_PIN, 0);

    switch (command) {
        case '1': gpio_put(BLUE_LED_PIN, 1); break;
        case '2': gpio_put(RED_LED_PIN, 1); break;
        case '3': gpio_put(GREEN_LED_PIN, 1); break;
        case '4':
            gpio_put(BLUE_LED_PIN, 1);
            gpio_put(RED_LED_PIN, 1);
            gpio_put(GREEN_LED_PIN, 1);
            break;
        case '5':
            gpio_put(BLUE_LED_PIN, 1);
            gpio_put(BUZZER_PIN, 1);
            break;
        case '6':
            gpio_put(RED_LED_PIN, 1);
            gpio_put(BUZZER_PIN, 1);
            break;
        case '7':
            gpio_put(GREEN_LED_PIN, 1);
            gpio_put(BUZZER_PIN, 1);
            break;
        case '8':
            gpio_put(BLUE_LED_PIN, 1);
            gpio_put(RED_LED_PIN, 1);
            gpio_put(GREEN_LED_PIN, 1);
            gpio_put(BUZZER_PIN, 1);
            break;
        case '9':
            gpio_put(RED_LED_PIN, 1);
            sleep_ms(1000);
            gpio_put(RED_LED_PIN, 0);
            gpio_put(BLUE_LED_PIN, 1);
            sleep_ms(1000);
            gpio_put(BLUE_LED_PIN, 0);
            gpio_put(GREEN_LED_PIN, 1);
            sleep_ms(1000);
            gpio_put(GREEN_LED_PIN, 0);
            break;
        case '0':
            gpio_put(RED_LED_PIN, 1);
            gpio_put(BUZZER_PIN, 1);
            sleep_ms(1000);
            gpio_put(RED_LED_PIN, 0);
            gpio_put(BLUE_LED_PIN, 1);
            gpio_put(BUZZER_PIN, 1);
            sleep_ms(1000);
            gpio_put(BLUE_LED_PIN, 0);
            gpio_put(GREEN_LED_PIN, 1);
            gpio_put(BUZZER_PIN, 1);
            sleep_ms(1000);
            gpio_put(GREEN_LED_PIN, 0);
            gpio_put(BUZZER_PIN, 0);
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D': {
            int times = command - 'A' + 1;
            for (int i = 0; i < times; i++) {
                gpio_put(BLUE_LED_PIN, 1);
                gpio_put(RED_LED_PIN, 1);
                gpio_put(GREEN_LED_PIN, 1);
                gpio_put(BUZZER_PIN, 1);
                sleep_ms(250);
                gpio_put(BLUE_LED_PIN, 0);
                gpio_put(RED_LED_PIN, 0);
                gpio_put(GREEN_LED_PIN, 0);
                gpio_put(BUZZER_PIN, 0);
                sleep_ms(250);
            }
            break;
        }
        case '#': gpio_put(BUZZER_PIN, 1); break;
        default: break;
    }
}

int main() {
    stdio_usb_init();  // Inicializa a USB para comunicação serial no Wokwi
    init_gpio();

    while (true) {
        int key = read_keypad();
        if (key == '*') {
            sleep_ms(100);  // Debounce delay
            key = read_keypad();
            if (key != -1) {
                printf("Key Pressed: %c\n", key);
                control_leds_and_buzzer(key);
                sleep_ms(1000);  // Delay entre cada comando
            }
        }
        sleep_ms(100);  // Pequeno delay para evitar bouncing
    }

    return 0;
}
