#include <iostream>
#include <string>

using namespace std;

/* Si el usuario tiene mas de 5 vic sube de nivel

- por cada fallo se le quita 0.50
- y si adivina a la primera 1.50 (son 3 intentos)

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
    int estatus; // Si esta en 1 es activo y si esta en 0 esta inactivo

    Jugador *prox;
};

struct Misiones{
    string titulo;
    string descripcion;
    string condiciones;

    Misiones *prox;
};

// Funciones y Procedimientos

bool Vacio(Jugador *lista){
    return lista == nullptr;
}

void MenuLogueo(){
    cout<< "Bienvenido a GameQuest Tracker"<<endl;
    cout<<"1. Iniciar Sesion"<<endl;
    cout<<"2. Registrarse"<<endl;
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
    nuevo->estatus = 0;

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
        if(aux->estatus == 1){
            cout << "Estatus: Activo" << endl;
        }
        else{
            cout << "Estatus: Inactivo" << endl;
        }
        cout<< "----------------------------------------------------"<<endl;
        aux = aux->prox;
    }

    
}
