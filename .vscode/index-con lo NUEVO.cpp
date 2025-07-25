// Hecho por: Paula Velazquez C.I: 31460243 y Oliver Guillen C.I: 31047247

#include "gameguest.h"

main(){
    SetConsoleOutputCP(CP_UTF8); // Sirve para imprimir caracteres especiales en la consola

    Jugador *perfiles = nullptr, *aux, *ranking = nullptr;
    int op = 1, opu, opa, opj;
    char priv;
    bool logueo;
    inicializarMisiones();
    CargarJugadoresDesdeArchivo(&perfiles, "C:/Users/User/Desktop/Oliver/Proyecto/GameQuest-Tracker/jugadores.txt");

    while(op != 0){
        system("cls");
        Logo();
        MenuLogueo();
        cout<< "Ingrese una opcion: ";
        cin>> op;

        

        switch (op){
            case 1:
                system("cls");
                Logo();
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
                                system("cls");
                                MenuUsuario(perfiles);
                                cout<< "Ingrese una opcion: ";
                                cin>> opu;

                                switch (opu){
                                    case 1:
                                        AdivinaElNumero(&perfiles);
                                        break;
                                    case 2:
                                        mostrarMisiones();
                                        system("pause");
                                        break;
                                    case 3:
                                        ranking = Ranking(perfiles);
                                        ImprimirRanking(ranking);
                                        system("pause");
                                        break;
                                    case 4:
                                        opj = 1;
                                        while(opj != 0){
                                            system("cls");
                                            PerfilJugador(perfiles);

                                            MenuModificar();
                                            cout<< "Ingrese una opcion: ";
                                            cin>> opj;

                                            switch (opj){
                                                case 1:
                                                    ModificarNombre(&perfiles);
                                                    break;
                                                case 2:
                                                    ModificarContrasena(&perfiles);
                                                    break;
                                                case 0:
                                                    cout<<"Saliendo..."<<endl;
                                                    break;
                                                
                                                default:
                                                    cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                                                    break;
                                            }
                                        }
                                        system("pause");
                                        break;
                                    case 0:
                                        cout<<"Saliendo..."<<endl;
                                        GuardarJugadoresEnArchivo(perfiles, "C:/Users/User/Desktop/Oliver/Proyecto/GameQuest-Tracker/jugadores.txt");
                                        break;
                                    
                                    default:
                                        cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                                        system("pause");
                                        break;
                                }
                            }


                        }
                        else{
                            opa = 1;
                            while(opa != 0){
                                system("cls");
                                MenuAdmin(perfiles);
                                cout<< "Ingrese una opcion: ";
                                cin>> opa;

                                switch (opa){
                                    case 1:
                                        system("cls");
                                        AgregarJugador(&perfiles);
                                        system("pause");
                                        break;
                                    case 2:
                                        system("cls");
                                        ImprimirJugadores(perfiles);
                                        system("pause");
                                        break;
                                    case 3:
                                        EliminarJugador(&perfiles);
                                        system("pause");
                                        break;
                                    case 7: 
                                        MostrarTop3Ganadores(perfiles);
                                        system("pause");
                                        break;
                                    case 8: 
                                        MostrarTop5Partidas(perfiles);
                                        system("pause");
                                        break;
                                    
                                    case 0:
                                        cout<<"Saliendo..."<<endl;
                                        GuardarJugadoresEnArchivo(perfiles, "C:/Users/User/Desktop/Oliver/Proyecto/GameQuest-Tracker/jugadores.txt");
                                        break;
                                    default:
                                        cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                                        system("pause");
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
                Logo();
                AgregarJugador(&perfiles);
                system("pause");
                break;
            case 0:
                cout<<"Saliendo..."<<endl;
                GuardarJugadoresEnArchivo(perfiles, "C:/Users/User/Desktop/Oliver/Proyecto/GameQuest-Tracker/jugadores.txt");
                break;
            
            default:
                cout<< "Opcion Invalida, ingrese una opcion valida"<<endl;
                system("pause");
                break;
        }
    }
}