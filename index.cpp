#include "gameguest.h"



main(){
    Jugador *perfiles = nullptr;

    for(int i = 1; i <= 3; i++){
        AgregarJugador(&perfiles);
        ImprimirJugadores(perfiles);
    }
}