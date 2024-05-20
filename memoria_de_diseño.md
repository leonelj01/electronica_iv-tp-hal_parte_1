# Electrónica IV - TP - HAL Parte 1 - Memoria de Diseño

Alumno: **Juarez, Gustavo Leonel**

## 1. Especificaciones 

El objetivo de este trabajo practico es desarrollar una capa de abstracción de hardware (HAL) que permita controlar los pines de entrada 
y salida y el temporizador del bluepill. Luego con dicho programa lo utilizaremos para hacer parpadear un led integrado en la placa la 
cual tendrá una frecuencia de un 1 Hz.

## 2. Pines de Entrada/Salid

Implementaremos en primer lugar las caracteristicas necesarias para completar el programa de parpadeo, por lo tanto averiguaremos cual es
el puerto y el pin donde se encuentra ubicado el led integrado de la bluepill. En la figura 1, podemos ver el pinout correspondiente a la 
placa a utilizar. Con lo cual sabemos que el led se encuentra en el PC13, es decir, el pin 13 del puerto C.

![](figuras\bluepill_pinout.png)

## 2.1 Habilitación de reloj

Los pines de entrada y salida son controlados por le periférico GPIO al bus periférico APB2 del microcontrolador. Los periféricos APB1 y 
APB2 cuentan con una habilitación de reloj comandada por el periférico RCC (Reset and Clock Control). Para utilizarlo debemos en primer
lugar habilitar su reloj. Por ello según el manual de referencia de la familia del microcontrolador (RM0008, 2021, sección 7.3.7) el reloj
del puerto GPIOC se habilita poniendo en uno el bit 4 (IOPCEN) del registro de offset (APB2ENR) en el periférico RCC, de dirección base
0x40021000 según (hoja de datos STM32108X, 2015).

Poner en 0 o en 1 bits individuales en un registro es necesario hacer una operacion de mascara. El codigo 2.1 muestra la operacion de 
mascara en donde se pone en 0 los bits marcados como unos en MASCARA_RESET y luego se ponen en 1 los bits marcados como unos en MASCARAS_SET

***Codigo 2.1: operaciones de mascaras***

```c
    enum{MASCARA_RESET = 0xF (3*4),
         MASCARA_SET   = 0b0100 (3*4)};
    palabra = (palabra & ~MASCARA_RESET) | MASCARA_SET;
```

***El codigo 2.2 muestra la habilitacion de reloj del puerto GPIOC usando las definiciones en `stm32f1xx.h`.***

```c
    static void habilitaRelojPuertoC(void)
    {
        RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPCEN;
    }
```

## 2.2 Interfaz abstracta

Nuestra interfaz abstracta no debe dar ningun detalle sobre el hardware o de las librerias utilizadas. Para lograr esto utilizaremos 
el concepto de `handle` para referirnos de forma abstracta a los procedimientos utilizados. Entonces asi podremos acceder a los pines 
mediante un enum de C. En el siguiente codigo se puede observar las definiciones para realizar esta parte del practico:

***Codigo 2.3: Handles y operaciones para los pines.***

```c
typedef enum HPin{
    Pin_led,
    Pin_NUM_HANDLES
}HPin;

void Pin_ponModoSalida(HPin pin);

void Pin_ponEstadoBajo(HPin pin);

void Pin_ponEstadoAlto(HPin pin);

bool Pin_consultaEstado(HPin pin);

void Pin_invierteEstado(HPin pin);
```

## 2.3 Descriptor de pin

En la interfaz abstracta los pines son descriptos mediante `handles`, numeros enteros. Pero a la hora de realizar la implementacion necesitamos un descriptor de pin, el cual debe ser capaz de realizar las operaciones previstas por la interfaz. Los diferentes perifericos tienen registros y librerias las cuales los controlan que vienen definidas en la libreria de soporte.

Para implementar las operaciones en un pin se necesita tener acceso a los registros de control del mismo (mediante GPIO), al numero de pin y a la rutina de habilitacion de reloj definida anteriormente (`seccion 2.1`). En el codigo 2.4 vemos la definición de un *pin* y la tabla de descriptores que asocia a cada handle un descriptor.

***Codigo 2.4: definicion de "Pin" y tabla de descriptores***

```c
typedef struct Pin{
    GPIO_Typedef *puntero;  // puntero a los registros del puerto
    int pin;    // número del pin
    void (*habilitaReloj)(void);    // función que habilita el reloj
}Pin;

// Tabla de descriptores
static const Pin descriptores[Pin_NUM_HANDLES]={
    [Pin_led] = {.puerto=GPIOC, .pin=13, .habilitaReloj=habilitaRelojPuertoC}
};
```

## 2.5 Modo de pin


## Programa principal

El programa que implementará nuestra interfaz abstracta es el siguiente:

```c
#include "hal.h"

int main(void)
{
    temporizador_inicializa();
    Pin_ponModoSalida(Pin_led);
    Pin_ponEstadoAlto(Pin_led);
    for(;;){
        temporizador_espera(500);
        Pin_invierteEstado(Pin_led);
    }

    return 0;
}
```

## Referencias
- *RM0008* (febrero 2021) ST.
- *Handle* (29 de octubre 2021), Wikipedia. <https://es.wikipedia.org/wiki/Handle>
- *Hoja de datos STM32F103x* (agosto 2015) ST.
- *CMSIS-core* (8 de mayo 2024). <https://arm-software.github.io/CMSIS_6/latest/Core/group__SysTick__gr.html>
- Predictable design (2024) *BluePill STM32F103 pinout*. <https://predictabledesigns.com/introduction-stm32-blue-pill-stm32duino/>