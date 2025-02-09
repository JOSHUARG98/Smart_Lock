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
#include "config.h"
#include "GPIOx.h"

/******************************************************************/
/**************************DEFINICIONES**************************/

#define LED1 PC1  // Definir el pin PC1 como LED1
#define LED2 PC2  // Definir el pin PC2 como LED2
#define BUZZER PC3 // Definir el pin PC3 para el buzzer


// Mapeo de PAD NUMERICO //

// Definición de filas (renglones)
#define FILA_1  PD7
#define FILA_2  PD6
#define FILA_3  PD5
#define FILA_4  PD4

// Definici�n de columnas
#define COLUMNA_1  PD3
#define COLUMNA_2  PD2
#define COLUMNA_3  PD1
#define MASCARA ( (1 << PIND1) | (1 << PIND2) | (1 << PIND3) )



/******************************************************************/
/**************************DEFINICIONES FIN**************************/

void setup_gpio(void) ;
void toggle_leds(void);
void beep_buzzer(void);

int main(void)
{
	    setup_gpio(); // Configurar GPIOs
		
			// Inicializar los pines de los leds
			GPIO_digitalWrite(&PORTD, FILA_1, LOW);   // Apagar LED1
			GPIO_digitalWrite(&PORTD, FILA_2, HIGH);   // Apagar LED1
			GPIO_digitalWrite(&PORTD, FILA_3, HIGH);   // Apagar LED1
			GPIO_digitalWrite(&PORTD, FILA_4, HIGH);   // Apagar LED1
			
			beep_buzzer();	// Secuencia de Beep del buzzer
			_delay_ms(1000);
			beep_buzzer();	// Secuencia de Beep del buzzer		

    while (1) {
		
		if ((PIND & MASCARA) != MASCARA) {
			beep_buzzer();
			} else {
		}
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
	
	// Habilitar resistencias de pull up internas del microcobtrolador
	
	GPIO_enablePullup(&PORTD, COLUMNA_1, 1); // Habilitamos una resistencia de pull up para la entrada de la COLUMNA1
	GPIO_enablePullup(&PORTD, COLUMNA_2, 1);// Habilitamos una resistencia de pull up para la entrada de la COLUMNA2
	GPIO_enablePullup(&PORTD, COLUMNA_3, 1); // Habilitamos una resistencia de pull up para la entrada de la COLUMNA2
	
	// Inicializar los pines de los leds 
	 GPIO_digitalWrite(&PORTC, LED1, LOW);   // Apagar LED1
	 GPIO_digitalWrite(&PORTC, LED2, LOW);   // Apagar LED1
	 GPIO_digitalWrite(&PORTC, BUZZER, LOW);   // Apagar LED1
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
	 GPIO_digitalWrite(&PORTC, BUZZER, LOW);   // APAGA BUZZER
}


