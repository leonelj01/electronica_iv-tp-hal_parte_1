# Electrónica IV - TP - HAL Parte 1

Modalidad: *individual*\
Plazo: *una semana*

## Objetivos

Luego de completar con éxito este práctico podrás

1. Reconocer el concepto de Capa de Abstracción de Hardware y su importancia en
   el desarrollo de sistemas embebidos
2. Interpretar la información de referencia de un microcontrolador y crear
   interfaces abstractas para acceder a las funciones provistas por los
   periféricos
3. Aplicar criterios de desarrollo de interfaces abstractas desacopladas al
   diseño de capas de abstracción de hardware que permiten desacoplar la lógica
   de la aplicación de una implementación concreta de hardware

## Metodología de trabajo

**Para completar este práctico** deberás realizar un *fork* de este repositorio,
donde desarrollarás tu solución. La entrega será mediante un enlace a dicho
*fork*. Desarrollarás una capa de abstracción de hardware (HAL) que exponiendo
las funciones de pin de entrada/salida y temporizador. Implementarás los pines
de entrada/salida mediante el periférico GPIO y el temporizador mediante el
periférico SysTick del microcontrolador STM32F103C8T6. Y escribirás una
aplicación sencilla que, utilizando esta capa de abstracción de hardware,
realizará el parpadeo del LED integrado en la placa de desarrollo con una
frecuencia de un Hertz.

La HAL que desarrolles estará contenida en una librería llamada "hal", ubicada
en `lib/hal` y cuyo encabezado principal será `hal.h`. Ubicarás el código del
controlador de puerto GPIO en el archivo `gpio.c` y el códio del controlador del
timer systick en el archivo `timer_systick.c`. Utilizarás como base para tus
drivers las definiciones incluidas por encabezado `stm32f1xx.h`. Dicho
encabezado define tipos de datos, punteros y constantes para acceder al control
de los distintos periféricos provistos por ST Microelectronics e incluye también
las librerías *CMSIS core*, que permiten determinar la frecuencia de reloj y
configurar el timer SysTick.

> *NOTA:* El encabezado `hal.h` *no puede incluir ninguna referencia a los
> encabezados de las librerías de soporte del microcontrolador. El uso de las
> mismas estará limitado a los archivos `gpio.c` y `timer_systick.c`.
> Recomendamos utilizar el concepto de *handle* (referencia abstracta, índice en
> que una tabla de recursos) para permitir el acceso abstracto al puerto GPIO
> sin exponer los detalles internos del mismo.

Durante tu desarrollo debes investigar el funcionamiento de los periféricos GPIO
y SysTick. El periférico GPIO fue desarrollado por ST Microelectronics y puedes
ubicar la información sobre el mismo en el *manual de referencia de la familia*
del microcontrolador STM32F103 (RM0008 Reference Manual, 2021, capítulos 7 y 9).
El caítulo 7 versa sobre el periférico de control de reloj y reset, que debes
utilizar para activar el reloj de los puertos GPIO, mientras que el capítulo 9
versa sobre el uso de los puertos en sí. El periférico SysTick fue desarrollado
por ARM y puedes encontrar detalles de su operación en el manual de referencia
de ARMv7-M (ARMv7-M Architecture Reference Manual, 2014, sección B3.3).
Realizarás la configuración del timer SysTick con ayuda de las librerías *CMSIS*
*core* incluidas (CMSIS-Core, 2023, sección Systick Timer).

Ubicarás el programa principal en el archivo `src/main.c`, puedes utilizar como
base el Listado 1. Debes documentar tu trabajo en de desarrollo de la librería
y el programa en el archivo `memoria_de_diseño.md`. Durante el proceso de
desarrollo debes hacer uso del hardware suministrado por la cátedra (placa de
desarrollo, debugger e interfaz serie) para comprobar el funcionamiento de tu
programa a medida que lo escribes. Se provee una configuración de ejecución en
emulador (env:emulador) y una para ejecución en el hardware
(env:bluepill_f103c8). El emulador no implementa la mayoría de los periféricos
y se recomienda utilizar el debugger en ese caso.

## Capa de Abstracción de Hardware (HAL)

Una *estructura de datos* es un conjunto de variables, llamadas celdas,
relacionadas entre sí y dispuestas en memoria de manera estructurada. Un *tipo*
*de dato abstracto* es un modelo matemático implementado mediante una estructura
de datos y asociado a un conjunto de operaciones definidas de modo abstracto,
esto es independiente a la implementación de la estructura de datos. El usuario
accederá a los datos solamente mediante las operaciones definidas
(Aho et al, 1988).

Una capa de abstracción de hardware (Hardware Abstraction Layer, HAL) es un
*conjunto de tipos de datos abstractos* que permiten controlar los periféricos
de entrada/salida en base a operaciones abstractas e independientes de los
detalles de implementación. El Listado 1 muestra un programa que hace parpadear
un LED conectado a un puerto de entrad/salida de propósito general de un
microcontrolador con una frecuencia de un hertz. Observa que en ningún momento
expone los detalles internos del funcionamiento del microcontrolador

Listado 1: Parpadeo de un LED conectado a puerto GPIO.

```c
#include "hal.h"

int main(void)
{
    Temporizador_inicializa();
    Pin_configuraSalidaLenta(Pin_LED);
    Pin_enAlto(Pin_LED);
    for(;;)
    {
        Temporizador_esperaMilis(500);
        Pin_cambiaEstado(Pin_LED);
    }
    return 0;
}
```

## Referencias

Aho, A.V., Hopcroft, J.E. y Ullman, J.D. (1988) *Estructuras de datos y algoritmos.* Addison-Wesley.

*ARMv7-M Architecture Reference Manual* (2 de diciembre de 2014) ARM Limited. Accesible en <https://developer.arm.com/documentation/ddi0403/ee/>

*CMSIS-Core (Cortex-M)* (18 de diciembre de 2023) ARM Limited. Sección Systick Timer accesible en <https://arm-software.github.io/CMSIS_6/latest/Core/group__SysTick__gr.html>

*RM0008 Reference Manual* (23 de febrero de 2021) ST Microelectronics. Accesible en <https://www.st.com/en/microcontrollers-microprocessors/stm32f103/documentation.html> como Reference Manual.

