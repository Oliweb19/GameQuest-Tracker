#include "gameguest.h"

main(){
    Jugador *perfiles = nullptr, *aux;
    int op = 1, opu = 1;
    char priv;
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
                        system("cls");
                        cout<< "Cargando sus datos..."<<endl;
                        aux = perfiles;
                        while(aux != nullptr){
                            if(aux->id_Jugador == id_logueo){
                                priv = aux->privilegio;
                            }
                            aux = aux->prox;
                        }

                        if(priv == 'u'){
                            while(opu != 0){
                                MenuUsuario(perfiles);
                                cout<< "Ingrese una opcion: ";
                                cin>> opu;

                                switch (opu){
                                case 1:
                                    AdivinaElNumero();
                                    break;
                                
                                default:
                                    cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                                    break;
                                }
                            }


                        }
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