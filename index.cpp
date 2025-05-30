#include "gameguest.h"

main(){
    Jugador *perfiles = nullptr;
    int op = 1;

    while(op != 0){
        MenuLogueo();
        cout<< "Ingrese una opcion: ";
        cin>> op;

        switch (op){
            case 1:
                /* Parte de logueo */
                break;
            case 2:
                AgregarJugador(&perfiles);
                break;
            
            default:
                cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                break;
        }
    }


    
}