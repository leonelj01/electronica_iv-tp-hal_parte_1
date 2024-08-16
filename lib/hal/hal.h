#ifndef HAL_H
#define HAL_H
#include <stdbool.h>
#include <stdint.h>

/* Aquí inclusión de otros encabezados con definiciones necesarias */

/* Aquí tus definiciones de tipo y constantes */

typedef enum HPin{
    Pin_led,
    Pin_NUM_HANDLES
}HPin;

/* Aquí tus prototipos de funciones */

void Pin_ponModoSalida(HPin pin);

void Pin_ponEstadoBajo(HPin pin);

void Pin_ponEstadoAlto(HPin pin);

bool Pin_consultaEstado(HPin pin);

void Pin_invierteEstado(HPin pin);


// SysTick

void Systick_init(void);

uint32_t Sistick_obtMilisegundos(void);

void Sistick_esperaMilisegundos(uint32_t tiempo);

void SysTick_Handler(void);

#endif