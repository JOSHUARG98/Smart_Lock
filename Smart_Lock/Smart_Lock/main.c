/******************************************************************************
 * 
 *                           Smart_Lock.c
 * 
 *  Created:    01/02/2025 13:54:27
 *  Author:     Rosas Garcia Angel Joshua
 *  Repository: https://github.com/JOSHUARG98/Smart_Lock.git
 * 
 *   ---------------------------------------------------------------------------
 *  |                        POWERED BY CALYPSO                                 |
 *  |                    Calypso Smart Lock System                              |
 *   ---------------------------------------------------------------------------
 * 
 *******************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/eeprom.h>
#include "config.h"
#include "GPIOx.h"


/******************************************************************/
/**************************DEFINICIONES**************************/

#define LED1 PC1  // Definir el pin PC1 como LED1
#define LED2 PC2  // Definir el pin PC2 como LED2
#define BUZZER PC3 // Definir el pin PC3 para el buzzer

// Mapeo de PAD NUMÉRICO //

// Definición de filas (renglones)
#define FILA_1  PD7
#define FILA_2  PD6
#define FILA_3  PD5
#define FILA_4  PD4

// Definición de columnas
#define COLUMNA_1  PD3
#define COLUMNA_2  PD2
#define COLUMNA_3  PD1
#define MASCARA ( (1 << PIND1) | (1 << PIND2) | (1 << PIND3) )

#define EEPROM_ADDR 0x000  // Dirección donde se guardará la contraseña
#define PASSWORD_LENGTH 4  // Tamaño de la contraseña

// Matriz de teclas según la posición en el teclado 4x3
const char key_map[4][3] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}
};

char contra_temporal[PASSWORD_LENGTH]; // Arreglo para almacenar la contraseña ingresada

/******************************************************************/
/**************************DEFINICIONES FIN**************************/

void setup_gpio(void);
void toggle_leds(void);
void beep_buzzer(void);
char read_keypad(void);
void EEPROM_write(uint16_t address, uint8_t data);
uint8_t EEPROM_read(uint16_t address);



int main(void)
{
	    setup_gpio(); // Configurar GPIOs
		    char key;
			// Inicializar los pines de los leds
			GPIO_digitalWrite(&PORTD, FILA_1, HIGH);   // Apagar LED1
			GPIO_digitalWrite(&PORTD, FILA_2, HIGH);   // Apagar LED1
			GPIO_digitalWrite(&PORTD, FILA_3, HIGH);   // Apagar LED1
			GPIO_digitalWrite(&PORTD, FILA_4, HIGH);   // Apagar LED1
			
			toggle_leds();
			beep_buzzer();	// Secuencia de Beep del buzzer
			_delay_ms(1000);
			beep_buzzer();	// Secuencia de Beep del buzzer
			
    while (1) {
		        capturar_contra(); // Capturar 4 teclas y escribir en EEPROM
		        _delay_ms(1000); // Pequeña pausa antes de capturar otra contraseña
	}
}


void setup_gpio(void) {
		// Configurar PC1, PC2 y PC3 como salidas
	GPIO_pinMode(&DDRC, LED1, OUTPUT);  // Configurar LED como salida
	GPIO_pinMode(&DDRC, LED2, OUTPUT);  // Configurar LED como salida
	GPIO_pinMode(&DDRC, BUZZER, OUTPUT);  // Configurar LED como salida
	
	// Configurar filas como salidas
	GPIO_pinMode(&DDRD, FILA_1, OUTPUT);  // Configurar FILA_1 como salida
	GPIO_pinMode(&DDRD, FILA_2, OUTPUT);  // CConfigurar FILA_2 como salida
	GPIO_pinMode(&DDRD, FILA_3, OUTPUT);  // CConfigurar FILA_3 como salida
	GPIO_pinMode(&DDRD, FILA_4, OUTPUT);  // CConfigurar FILA_4 como salida
	
	// Configurar Columnas como entradas
	GPIO_pinMode(&DDRD, COLUMNA_1, INPUT);  // Configurar COLUMNA_1 como salida
	GPIO_pinMode(&DDRD, COLUMNA_2, INPUT);  // CConfigurar COLUMNA_2 como salida
	GPIO_pinMode(&DDRD, COLUMNA_3, INPUT);  // CConfigurar COLUMNA_3 como salida
	
	// Habilitar resistencias de pull up internas del microcontrolador
	
	GPIO_enablePullup(&PORTD, COLUMNA_1, 1); // Habilitamos una resistencia de pull up para la entrada de la COLUMNA1
	GPIO_enablePullup(&PORTD, COLUMNA_2, 1);// Habilitamos una resistencia de pull up para la entrada de la COLUMNA2
	GPIO_enablePullup(&PORTD, COLUMNA_3, 1); // Habilitamos una resistencia de pull up para la entrada de la COLUMNA2
	
	// Inicializar los pines de los leds 
	 GPIO_digitalWrite(&PORTC, LED1, LOW);   // Apagar LED1
	 GPIO_digitalWrite(&PORTC, LED2, LOW);   // Apagar LED2
	 GPIO_digitalWrite(&PORTC, BUZZER, LOW);   // Apagar BUZZER
}



void toggle_leds(void) {
	 GPIO_digitalWrite(&PORTC, LED1, HIGH);   // Enciende LED1
	 _delay_ms(10000); // Esperar 2 segundos
	// Apagar LED1, encender LED2 y activar buzzer
	 GPIO_digitalWrite(&PORTC, LED1, LOW);   // Apaga LED1
	 GPIO_digitalWrite(&PORTC, LED2, HIGH);   // Enciende LED2
	 _delay_ms(10000); // Esperar 2 segundos
	 GPIO_digitalWrite(&PORTC, LED2, LOW);   // Apaga LED2
}

void beep_buzzer(void) {
	// Activar el buzzer
	 GPIO_digitalWrite(&PORTC, BUZZER, HIGH);   // Enciende BUZZER
	// Esperar 64 milisegundos
	_delay_ms(1000);
	// Desactivar el buzzer
	 GPIO_digitalWrite(&PORTC, BUZZER, LOW);   // APAGAR BUZZER
}

char read_keypad(void) {
	for (uint8_t row = 0; row < 4; row++) {
		// Activar solo una fila en LOW
		PORTD |= (1 << FILA_1) | (1 << FILA_2) | (1 << FILA_3) | (1 << FILA_4); // Desactivar todas las filas (HIGH)
		PORTD &= ~(1 << (PD7 - row)); // Activar fila actual (LOW)

		_delay_ms(10); // Pequeña pausa para estabilidad

		// Leer columnas
		if (!(PIND & (1 << COLUMNA_1))) return key_map[row][0];
		if (!(PIND & (1 << COLUMNA_2))) return key_map[row][1];
		if (!(PIND & (1 << COLUMNA_3))) return key_map[row][2];
	}

	return 0; // No se presionó ninguna tecla
}


void EEPROM_write(uint16_t address, uint8_t data) {
	while (EECR & (1 << EEPE)); // Esperar si EEPROM está ocupada
	EEAR = address;  // Configurar dirección
	EEDR = data;     // Escribir dato
	EECR |= (1 << EEMPE); // Habilitar escritura
	EECR |= (1 << EEPE);  // Iniciar escritura
}

uint8_t EEPROM_read(uint16_t address) {
	while (EECR & (1 << EEPE)); // Esperar si EEPROM está ocupada
	EEAR = address;  // Configurar dirección
	EECR |= (1 << EERE); // Iniciar lectura
	return EEDR;    // Retornar el dato leído
}

// Función para capturar la contraseña desde el teclado
void capturar_contra(void) {
	uint8_t i = 0;
	while (i < PASSWORD_LENGTH) {
		char tecla = read_keypad(); // Leer tecla del teclado matricial

		if (tecla) { // Si se presiona una tecla válida
			contra_temporal[i] = tecla;
			i++;
		   beep_buzzer(); //Suena el buzzer al presionar
		   _delay_ms(500); // Debounce
			if (i == PASSWORD_LENGTH) { // Si se han ingresado 4 teclas
				eeprom_write_block((const void*)contra_temporal, (void*)EEPROM_ADDR, PASSWORD_LENGTH);
				beep_buzzer(); // Confirmación de escritura
				_delay_ms(200); // Debounce
				beep_buzzer(); // Confirmación de escritura
				_delay_ms(200); // Debounce
				beep_buzzer(); // Confirmación de escritura
								
				break;
			}
		}
		_delay_ms(200); // Debounce
	}
}




