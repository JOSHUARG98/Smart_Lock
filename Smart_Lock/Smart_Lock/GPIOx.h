/*
 * GPIOx.h
 *
 * Created: 01/02/2025 03:23:47 p. m.
 *  Author: joshu
 */ 


#ifndef GPIOX_H_
#define GPIOX_H_

#include <avr/io.h>

// Definiciones de macros para mejorar la legibilidad
#define INPUT  0
#define OUTPUT 1
#define LOW    0
#define HIGH   1
#define PULLUP 1

// Funciones del driver
void GPIO_pinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t mode);
void GPIO_digitalWrite(volatile uint8_t *port, uint8_t pin, uint8_t value);
uint8_t GPIO_digitalRead(volatile uint8_t *pin_reg, uint8_t pin);
void GPIO_enablePullup(volatile uint8_t *port, uint8_t pin, uint8_t enable);




#endif /* GPIOX_H_ */