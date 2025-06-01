#include <iostream>
#include <string>
#include <random>    
#include <chrono> 
#include <windows.h>

using namespace std;

/* 

-Si el usuario tiene mas de 5 vic sube de nivel
- por cada fallo se le quita 0.50
- y si adivina a la primera 1.50 (son 3 intentos)
- Verificar que no exita el mismo alias a la hora de registrar
- Verificar que el nombre no tenga numero

*/

// Estructuras de Datos

struct Jugador{
    int id_Jugador;
    string nombre;
    string alias;
    string contrasena;
    int ano; // es año
    int victorias;
    int derrotas;
    int partidas_jugadas;
    float saldo;
    char privilegio;

    Jugador *prox;
};

struct Misiones{
    string titulo;
    string descripcion;
    string condiciones;

    Misiones *prox;
};

// Variables Globales

int id_logueo;
mt19937 globalGenerador(chrono::system_clock::now().time_since_epoch().count());


// Funciones y Procedimientos

bool Vacio(Jugador *lista){
    return lista == nullptr;
}

void MenuLogueo(){
    cout<< "Bienvenido a GameQuest Tracker"<<endl;
    cout<<"1. Iniciar Sesion"<<endl;
    cout<<"2. Registrarse"<<endl;
    cout<<"0. Salir"<<endl;
}

void MenuUsuario(Jugador *perfiles){
    Jugador *aux;
    string nombre;

    aux = perfiles;
    while(aux != nullptr){
        if(aux->id_Jugador == id_logueo){
            nombre = aux->nombre;
        }
        aux = aux->prox;
    }

    cout<< "Bienvenido "<<nombre<<" a GameQuest Tracker"<<endl;
    cout<<"1. Apuesta en Adivina el numero"<<endl;
    cout<<"2. Ver Misiones"<<endl;
    cout<<"3. Ver Raking"<<endl;
    cout<<"4. Ver tu perfil"<<endl;
    cout<<"0. Cerrar Sesion"<<endl;
}

// Jugador

Jugador *CrearNodoJugador(int id_Jugador, string nombre, string alias, int ano, string contrasena){
    float saldo = 50;
    Jugador *nuevo = new Jugador;

    nuevo->id_Jugador = id_Jugador;
    nuevo->nombre = nombre;
    nuevo->alias = alias;
    nuevo->contrasena = contrasena;
    nuevo->ano = ano;
    nuevo->victorias = 0;
    nuevo->derrotas = 0;
    nuevo->partidas_jugadas = 0;
    nuevo->saldo = saldo;
    nuevo->privilegio = 'u';

    nuevo->prox = NULL;
    
    return nuevo;
}

void InsertarJugador(Jugador **perfiles, int id_Jugador, string nombre, string alias, int ano, string contrasena){
    Jugador *nuevo = CrearNodoJugador(id_Jugador, nombre, alias, ano, contrasena), *aux = *perfiles;

    if(Vacio(*perfiles)){
        *perfiles = nuevo;
    }
     else{
        while(aux->prox != nullptr){
            aux = aux->prox;
        }
        aux->prox = nuevo;
    }
}

void AgregarJugador(Jugador **perfiles) {
    Jugador *aux = *perfiles;
    int id, ano;
    string nombre, alias, contrasena;

    if (Vacio(*perfiles)){
        id = 1;
    } 
    else{
        while (aux->prox != nullptr){
            aux = aux->prox;
        }
        id = aux->id_Jugador + 1; 
    }

    cout << "Ingrese su nombre: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Ingrese su alias de juego: ";
    getline(cin, alias);
    cout << "Ingrese su contrasena: ";
    getline(cin, contrasena);
    cout << "Ingrese su año de nacimiento: ";
    cin >> ano;
    cin.ignore();

    InsertarJugador(perfiles, id, nombre, alias, ano, contrasena);
}

void ImprimirJugadores(Jugador *perfiles) {
    Jugador *aux = perfiles;

    cout<< "----------------------------------------------------"<<endl;
    if(Vacio(perfiles)){
        cout << "No hay datos del Jugador." << endl;
        return;
    }
    while (aux != nullptr) {
        cout << "ID: " << aux->id_Jugador << endl;
        cout << "Nombre: " << aux->nombre << endl;
        cout << "Alias: " << aux->alias << endl;
        cout << "Contrasena: " << aux->contrasena << endl;
        cout << "Año de nacimiento: " << aux->ano << endl;
        cout << "Victorias: " << aux->victorias << endl;
        cout << "Derrotas: " << aux->derrotas << endl;
        cout << "Partidas jugadas: " << aux->partidas_jugadas << endl;
        cout << "Saldo: " << aux->saldo << endl;
        if(aux->privilegio == 'u'){
            cout << "Privilegio de Usuarios" << endl;
        }
        else{
            cout << "Privilegio de Admin" << endl;
        }
        cout<< "----------------------------------------------------"<<endl;
        aux = aux->prox;
    }

    
}

bool IniciarSesion(Jugador *perfiles){
    Jugador *aux = perfiles;
    string alias,contrasena;

    cout<< "Ingrese su Alias: ";
    cin.ignore();
    getline(cin, alias);
    cout<< "Ingrese su Contrasena: ";
    getline(cin, contrasena);

    while(aux != nullptr){
        if(aux->alias == alias){
            if(aux->contrasena == contrasena){
                id_logueo = aux->id_Jugador;
                return true;
            }
            else{
                return false;
            }
        }
        aux = aux->prox;
    }
}

// Simulador de juego

int generarNumeroAleatorioOptimizada(int min, int max) {
    uniform_int_distribution<int> distribucion(min, max);
    return distribucion(globalGenerador); // Usamos el generador global
}

void PuntuacionJugador(Jugador **perfiles, int victorias, int derrotas, float saldo){
    Jugador *aux = *perfiles;

    while(aux != nullptr){
        if(aux->id_Jugador == id_logueo){
            aux->victorias = victorias;
            aux->derrotas = derrotas;
            aux->partidas_jugadas += 1;
            aux->saldo = saldo;
        }
        aux = aux->prox;
    }
}

void AdivinaElNumero(Jugador **perfiles){
    Jugador *aux = *perfiles;
    int adivinar, num, op = 1;
    int victorias = 0, derrotas = 0;
    float saldo = 0;

    // Buscar datos del jugador logueado
    while(aux != nullptr){
        if(aux->id_Jugador == id_logueo){
            saldo = aux->saldo;
            victorias = aux->victorias;
            derrotas = aux->derrotas;
            break;
        }
        aux = aux->prox;
    }
    
    cout<<"Bienvenido a Adivina el Numero"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    cout<<"Reglas: "<<endl;
    cout<<"1. Tendras 3 intentos para adivinar un numero del 1 al 10"<<endl;
    cout<<"2. Por cada intento fallido te quitaremos 0.50 bs"<<endl;
    cout<<"3. Pero si adivinas a la primera te ganaras 1.50 bs"<<endl;
    cout<<"---------------------------------------------------------"<<endl;

    do{
        cout<<"\nQuieres Jugar?"<<endl;
        cout<<"1. Si"<<endl;
        cout<<"0. No"<<endl;
        cout<< "Ingrese una opcion: ";
        cin>> op;

        if(op == 1){
            adivinar = generarNumeroAleatorioOptimizada(1, 10);
            int i = 0;
            bool ganado = false;

            while(i < 3){
                cout<<"Tu Saldo es: "<<saldo<<endl;
                cout<<"-------------------------------"<<endl;
                cout<< "Ingrese el numero a adivinar: ";
                cin >> num;

                // Validar que no ingrese letras
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Entrada invalida. Intente de nuevo." << endl;
                    continue;
                }

                if(num == adivinar){
                    cout<< "Ganaste!!" << endl;
                    victorias += 1;
                    if(i == 0){
                        saldo += 1.50;
                    }
                    PuntuacionJugador(perfiles, victorias, derrotas, saldo);
                    ganado = true;
                    break;
                } 
                else{
                    if(num > adivinar){
                        cout<< "El numero es menor"<<endl;
                    } 
                    else{
                        cout<< "El numero es mayor"<<endl;
                    }
                    saldo -= 0.50;
                }
                i++;
            }

            if(!ganado){
                cout<<"Perdiste :("<<endl;
                cout<<"El numero era: "<<adivinar<<endl;
                derrotas += 1;
                PuntuacionJugador(perfiles, victorias, derrotas, saldo);
            }
        }

    }while(op != 0);
}