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

// Jugador

Jugador *CrearNodoJugador(int id_Jugador, string nombre, string alias, int ano){
    float saldo = 50;
    Jugador *nuevo = new Jugador;

    nuevo->id_Jugador = id_Jugador;
    nuevo->nombre = nombre;
    nuevo->alias = alias;
    nuevo->ano = ano;
    nuevo->victorias = 0;
    nuevo->derrotas = 0;
    nuevo->partidas_jugadas = 0;
    nuevo->saldo = saldo;
    nuevo->estatus = 0;

    nuevo->prox = NULL;
    
    return nuevo;
}

void InsertarJugador(Jugador **perfiles, int id_Jugador, string nombre, string alias, int ano){
    Jugador *nuevo = CrearNodoJugador(id_Jugador, nombre, alias, ano), *aux = *perfiles;

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
    int id = 1, ano;
    string nombre, alias;

    if (!Vacio(*perfiles)) {
        while (aux->prox != nullptr) {
            aux = aux->prox;
        }
        id = aux->id_Jugador + 1;
        
    }

    cout << "Ingrese su nombre: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Ingrese su alias de juego: ";
    getline(cin, alias);
    cout << "Ingrese su año de nacimiento: ";
    cin >> ano;
    cin.ignore();

    InsertarJugador(perfiles, id, nombre, alias, ano);
}

void ImprimirJugadores(Jugador *perfiles) {
    cout<< "----------------------------------------------------"<<endl;
    if(Vacio(perfiles)){
        cout << "No hay datos del Jugador." << endl;
        return;
    }
    cout << "ID: " << perfiles->id_Jugador << endl;
    cout << "Nombre: " << perfiles->nombre << endl;
    cout << "Alias: " << perfiles->alias << endl;
    cout << "Año de nacimiento: " << perfiles->ano << endl;
    cout << "Victorias: " << perfiles->victorias << endl;
    cout << "Derrotas: " << perfiles->derrotas << endl;
    cout << "Partidas jugadas: " << perfiles->partidas_jugadas << endl;
    cout << "Saldo: " << perfiles->saldo << endl;
    if(perfiles->estatus == 1){
        cout << "Estatus: Activo" << endl;
    }
    else{
        cout << "Estatus: Inactivo" << endl;
    }
    cout<< "----------------------------------------------------"<<endl;

    
}
