#include "gameguest.h"



main(){
    Jugador *perfiles = nullptr;
    int i = 1;

    while(i <= 3){
        AgregarJugador(&perfiles);
        ImprimirJugadores(perfiles);

        i++;
    }
}