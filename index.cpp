#include "gameguest.h"

main(){
    SetConsoleOutputCP(CP_UTF8); // Sirve para imprimir caracteres especiales en la consola

    Jugador *perfiles = nullptr, *aux, *ranking = nullptr;
    int op = 1, opu;
    char priv;
    bool logueo;

    while(op != 0){
        system("cls");
        MenuLogueo();
        cout<< "Ingrese una opcion: ";
        cin>> op;

        switch (op){
            case 1:
                system("cls");
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
                            opu = 1;
                            while(opu != 0){
                                MenuUsuario(perfiles);
                                cout<< "Ingrese una opcion: ";
                                cin>> opu;

                                switch (opu){
                                    case 1:
                                        AdivinaElNumero(&perfiles);
                                        break;
                                    case 2:
                                        mostrarMisiones();
                                        break;
                                    case 3:
                                        ranking = Ranking(perfiles);
                                        ImprimirRanking(ranking);
                                        break;
                                    case 4:
                                        PerfilJugador(perfiles);
                                        break;
                                    case 0:
                                        cout<<"Saliendo..."<<endl;
                                        break;
                                    
                                    default:
                                        cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                                        break;
                                }
                            }


                        }
                    }
                }
                system("pause");
                break;
            case 2:
                system("cls");
                AgregarJugador(&perfiles);
                system("pause");
                break;
            case 3: // Lo estoy usando para ver los perfiles despues de que juego.
                system("cls");
                //ImprimirJugadores(perfiles);
                ImprimirRanking(perfiles);
                system("pause");
                break;
            case 0:
                cout<<"Saliendo..."<<endl;
                break;
            
            default:
                cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                system("pause");
                break;
        }
    }
}