/*
 * GPIOx.c
 *
 * Created: 01/02/2025 03:24:43 p. m.
 *  Author: joshu
 */ 


#include "GPIOx.h"
#include <avr/io.h>

// Configura un pin como entrada o salida
void GPIO_pinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t mode) {
	if (mode == OUTPUT) {
		*ddr |= (1 << pin);  // Configurar como salida
		} else {
		*ddr &= ~(1 << pin); // Configurar como entrada
	}
}

// Escribe un valor en un pin de salida
void GPIO_digitalWrite(volatile uint8_t *port, uint8_t pin, uint8_t value) {
	if (value == HIGH) {
		*port |= (1 << pin);  // Poner en alto
		} else {
		*port &= ~(1 << pin); // Poner en bajo
	}
}

// Lee el estado de un pin de entrada
uint8_t GPIO_digitalRead(volatile uint8_t *pin_reg, uint8_t pin) {
	return (*pin_reg & (1 << pin)) ? HIGH : LOW;
}

// Activa o desactiva la resistencia pull-up en un pin de entrada
void GPIO_enablePullup(volatile uint8_t *port, uint8_t pin, uint8_t enable) {
	if (enable) {
		*port |= (1 << pin);  // Activar pull-up
		} else {
		*port &= ~(1 << pin); // Desactivar pull-up
	}
}