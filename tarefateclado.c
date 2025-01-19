#include "pico/stdlib.h"
#include <stdio.h>

#define RED 11
#define BLUE 22
#define GREEN 19
#define BUZZER 28

char envio=0;//Determina se a tecla de envio '*' foi pressionada
char tecla='n';

const uint8_t colunas[4] = {1, 2, 3, 4}; // Pinos das colunas
const uint8_t linhas[4] = {5, 6, 7, 8};  // Pinos das linhas

const char teclado[4][4] = 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char leitura_teclado();

int main() 
{
    char aux;
    // Inicializa a UART (Serial)
    stdio_init_all();

    gpio_init(RED); //inicializa o pino do LED VERMELHO
    gpio_init(GREEN); //inicializa o pino do LED VERDE
    gpio_init(BLUE); //inicializa o pino do LED AZUL
    gpio_init(BUZZER); //inicializa o pino do BUZZER
    gpio_set_dir(RED, GPIO_OUT); //define o pino como saída
    gpio_set_dir(GREEN, GPIO_OUT); //define o pino como saída
    gpio_set_dir(BLUE, GPIO_OUT); //define o pino como saída
    gpio_set_dir(BUZZER, GPIO_OUT); //define o pino como saída
    gpio_put(RED, false);
    gpio_put(GREEN, false);
    gpio_put(BLUE, false);
    gpio_put(BUZZER, false);

    // Configuração dos pinos das colunas como saídas digitais
    for (int i = 0; i < 4; i++)
    {//Os pinos de 1 a 4 sao outputs
        gpio_init(colunas[i]);
        gpio_set_dir(colunas[i], GPIO_OUT);
        gpio_put(colunas[i], 1); // Inicializa todas as colunas como baixo
    }

    // Configuração dos pinos das linhas como entradas digitais
    for (int i = 0; i < 4; i++)
    {//Os pinos de 5 a 8 sao inputs
        gpio_init(linhas[i]);
        gpio_set_dir(linhas[i], GPIO_IN);
        gpio_pull_up(linhas[i]); // Habilita pull-up para as linhas
    }
    printf("Tecla pressionada: %c\n", tecla);
    while (true) 
    {
      aux=leitura_teclado();
      if(aux == '*')
         envio=1;
      else if(aux!='n'){
         tecla=aux;
         printf("Tecla pressionada: %c\n", tecla);
        }
        if (envio>0 && tecla!='n') // Só exibe se a tecla e o '*' foram pressionados
        {
            printf("\nEnviado !\n\n");
            switch (tecla) {
        case '1': gpio_put(BLUE, 1); sleep_ms(1000); gpio_put(BLUE, 0); break;
        case '2': gpio_put(RED, 1); sleep_ms(1000); gpio_put(RED, 0); break;
        case '3': gpio_put(GREEN, 1); break;
        case '4':
            gpio_put(BLUE, 1);
            gpio_put(RED, 1);
            gpio_put(GREEN, 1);
            sleep_ms(1000);
            gpio_put(BLUE, 0);
            gpio_put(RED, 0);
            gpio_put(GREEN, 0);
            break;
        case '5':
            gpio_put(BLUE, 1);
            gpio_put(BUZZER, 1);
            sleep_ms(1000);
            gpio_put(BLUE, 0);
            gpio_put(BUZZER, 0);
            break;
        case '6':
            gpio_put(RED, 1);
            gpio_put(BUZZER, 1);
            sleep_ms(1000);
            gpio_put(RED, 0);
            gpio_put(BUZZER, 0);
            break;
        case '7':
            gpio_put(GREEN, 1);
            gpio_put(BUZZER, 1);
            sleep_ms(1000);
            gpio_put(GREEN, 0);
            gpio_put(BUZZER, 0);
            break;
        case '8':
            gpio_put(BLUE, 1);
            gpio_put(RED, 1);
            gpio_put(GREEN, 1);
            gpio_put(BUZZER, 1);
            sleep_ms(1000);
            gpio_put(BLUE, 0);
            gpio_put(RED, 0);
            gpio_put(GREEN, 0);
            gpio_put(BUZZER, 0);
            break;
        case '9':
            gpio_put(RED, 1);
            sleep_ms(1000);
            gpio_put(RED, 0);
            gpio_put(BLUE, 1);
            sleep_ms(1000);
            gpio_put(BLUE, 0);
            gpio_put(GREEN, 1);
            sleep_ms(1000);
            gpio_put(GREEN, 0);
            break;
        case '0':
            gpio_put(RED, 1);
            gpio_put(BUZZER, 1);
            sleep_ms(1000);
            gpio_put(RED, 0);
            gpio_put(BLUE, 1);
            gpio_put(BUZZER, 1);
            sleep_ms(1000);
            gpio_put(BLUE, 0);
            gpio_put(GREEN, 1);
            gpio_put(BUZZER, 1);
            sleep_ms(1000);
            gpio_put(GREEN, 0);
            gpio_put(BUZZER, 0);
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D': {
            int times = tecla - 'A' + 1;
            for (int i = 0; i < times; i++) {
                gpio_put(BLUE, 1);
                gpio_put(RED, 1);
                gpio_put(GREEN, 1);
                gpio_put(BUZZER, 1);
                sleep_ms(250);
                gpio_put(BLUE, 0);
                gpio_put(RED, 0);
                gpio_put(GREEN, 0);
                gpio_put(BUZZER, 0);
                sleep_ms(250);
            }
            break;
        }
        case '#': gpio_put(BUZZER, 1); sleep_ms(500); gpio_put(BUZZER, 0); break;
        default: break;
        }
        tecla='n';
        printf("Tecla pressionada: %c\n", tecla);
        }
      envio=0;//Isso aqui impede o usuário de 1º apertar pra enviar e so depois selecionar a tecla
      sleep_ms(200); // Intervalo de tempo menor para uma leitura mais rápida
    }
 return 0;//Teoricamente, nunca chega aqui por causa do loop infinito
}

// Função para ler o teclado matricial
char leitura_teclado()
{
    char numero = 'n'; // Valor padrão para quando nenhuma tecla for pressionada

    // Desliga todos os pinos das colunas
    for (int i = 0; i < 4; i++)
    {
        gpio_put(colunas[i], 1);
    }

    for (int coluna = 0; coluna < 4; coluna++)
    {
        // Ativa a coluna atual (coloca o pino da coluna como 1)
        gpio_put(colunas[coluna], 0);

        for (int linha = 0; linha < 4; linha++)
        {
            // Verifica o estado da linha. Se estiver em 0, a tecla foi pressionada
            if (gpio_get(linhas[linha]) == 0)
            {
                numero = teclado[linha][coluna]; // Mapeia a tecla pressionada
                // Aguarda a tecla ser liberada (debounce)
                while (gpio_get(linhas[linha]) == 0)
                {
                    sleep_ms(10); // Aguarda a tecla ser liberada
                }
                break; // Sai do laço após detectar a tecla
            }
        }

        // Desativa a coluna atual (coloca o pino da coluna como 0)
        gpio_put(colunas[coluna], 1);

        if (numero != 'n') // Se uma tecla foi pressionada, sai do laço de colunas
        {
            break;
        }
    }
 return numero; // Retorna a tecla pressionada
}
