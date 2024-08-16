#include "hal.h"

int main(void)
{
    Pin_ponModoSalida(Pin_led);
    Pin_ponEstadoAlto(Pin_led);
    Systick_init();

    for(;;){
        Sistick_esperaMilisegundos(500);
        Pin_invierteEstado(Pin_led);
    }
    return 0;
}