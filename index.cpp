#include "gameguest.h"

main(){
    Jugador *perfiles = nullptr;
    int op = 1;
    bool logueo;

    while(op != 0){
        MenuLogueo();
        cout<< "Ingrese una opcion: ";
        cin>> op;

        switch (op){
            case 1:
                logueo = false;
                while (!logueo){
                    logueo = IniciarSesion(perfiles);
                    if(logueo == false){
                        cout<< "Ingrese sus datos nuevamente."<<endl;
                    }
                    else{
                        cout<< "Bienvenido a GameQuest Tracker"<<endl;
                        
                    }
                }
                break;
            case 2:
                system("cls");
                AgregarJugador(&perfiles);
                break;
            
            default:
                cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                break;
        }
    }


    
}