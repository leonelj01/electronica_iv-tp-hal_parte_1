#include <stm32f1xx.h>
#include "hal.h"

/* Aquí tu implementación de controlador para puerto GPIO */

static void habilitaRelojPuertoC(void)
{
    RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPCEN;
}

typedef struct Pin{
    GPIO_TypeDef *puerto;           // Puntero a los registros del puerto. 
    int pin;                        // Numero del pin.
    void (*habilitaReloj)(void);    // Funcion que habilita el reloj.
}Pin;


// Tabla de descriptores de pin
static const Pin descriptores[Pin_NUM_HANDLES]={
    [Pin_led] = {.puerto=GPIOC, .pin=13, .habilitaReloj=habilitaRelojPuertoC}
};


void Pin_ponModoSalida(HPin pin){
    enum{MODO_SALIDA = 0b0010};
    if(pin >= Pin_NUM_HANDLES) return;
    const Pin *desc = &descriptores[pin];

    desc->habilitaReloj();

    const int offset = ((desc->pin % 8) * 4);
    const uint32_t mascaraReset = 0xF << offset;
    const uint32_t mascaraSet = MODO_SALIDA << offset;

    if(desc->pin < 0){
        desc->puerto->CRL = (desc->puerto->CRL & ~mascaraReset) | mascaraSet;
    }else{
        desc->puerto->CRH = (desc->puerto->CRH & ~mascaraReset) | mascaraSet;
    }
}

void Pin_ponEstadoBajo(HPin pin){
    if (pin >= Pin_NUM_HANDLES) return;
    const Pin *desc = &descriptores[pin];
    const uint32_t mascaraPin = 1 << desc->pin;
    desc->puerto->BRR = mascaraPin;
}

void Pin_ponEstadoAlto(HPin pin){
    if (pin >= Pin_NUM_HANDLES) return;
    const Pin *desc = &descriptores[pin];
    const uint32_t mascaraPin = 1 << desc->pin;
    desc->puerto->ODR = mascaraPin;
}

bool Pin_consultaEstado(HPin pin){
    if (pin >= Pin_NUM_HANDLES) return false;
    const Pin *desc = &descriptores[pin];
    const uint32_t mascaraPin = 1 << desc->pin;
    return desc->puerto->ODR & mascaraPin;
}

void Pin_invierteEstado(HPin pin){
    if (Pin_consultaEstado(pin)){
        Pin_ponEstadoBajo(pin);
    }else{
        Pin_ponEstadoAlto(pin);
    }
}
