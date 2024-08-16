#include <stm32f1xx.h>
#include "hal.h"

/* Aquí tu implementación de controlador para el timer SysTick */

static volatile uint32_t ticks;

void Systick_init(void){
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/1000);
}

uint32_t Sistick_obtMilisegundos(void){
    return ticks;
}

void Sistick_esperaMilisegundos(uint32_t tiempo){
    const uint32_t inicial = ticks;
    while(ticks-inicial < tiempo);
}

void SysTick_Handler(void){
    ++ticks;
}