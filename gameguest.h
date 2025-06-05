#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <windows.h>
#include <ctime> // Para fecha actual

using namespace std;

/*
-Si el usuario tiene mas de 5 vic sube de nivel
- por cada fallo se le quita 0.50
- y si adivina a la primera 1.50 (son 3 intentos)
- Verificar que no exita el mismo alias a la hora de registrar
- Verificar que el nombre no tenga numero
*/

// Estructuras de Datos

const int MAX_MISIONES = 10;

// Estructura para almacenar información sobre logros obtenidos
struct LogroObtenido {
    string titulo;
    string descripcion_detallada;
    string fecha_obtencion;
    LogroObtenido *prox;
};

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

    bool misiones_completadas[MAX_MISIONES]{};
    LogroObtenido *logros_obtenidos = nullptr;
    Jugador *prox;
};

struct Mision {
    string titulo;
    string descripcion;
    string requisitos;
    float saldo_otorgado;
    Mision *prox;
};

// Variables Globales

int id_logueo = -1; //-1 porque ningun usuario esta logueado al inicio
mt19937 globalGenerador(chrono::system_clock::now().time_since_epoch().count());
Mision *lista_misiones = nullptr; // Cabeza de la lista enlazada de misiones

string misiones_titulos[MAX_MISIONES]; //guarda el nombre de las misiones
int cant_misiones = 0;

// VER ESTO!!
string obtenerFechaHoy() {
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%d/%m/%Y", now); //El formato "%d/%m/%Y" significa: día con dos dígitos (%d), slash, mes con dos dígitos (%m), slash, año con cuatro dígitos (%Y).
    return string(buf);
}

// --- AGREGA LOGRO A LA LISTA DE LOGROS OBTENIDOS ---
void agregarLogroObtenido(Jugador *jugador, const string& titulo, const string& descripcion) {
    LogroObtenido *nuevo = new LogroObtenido;
    nuevo->titulo = titulo;
    nuevo->descripcion_detallada = descripcion;
    nuevo->fecha_obtencion = obtenerFechaHoy();
    nuevo->prox = nullptr;

    if (!jugador->logros_obtenidos) { //Si el jugador NO tiene logros aún ese será el primero.
        jugador->logros_obtenidos = nuevo;
    } else {
        LogroObtenido *aux = jugador->logros_obtenidos;
        while (aux->prox) aux = aux->prox;
        aux->prox = nuevo;
    }
}

Mision *crearMision(string titulo, string descripcion, string requisitos, float saldo_otorgado) {
    Mision *nuevo = new Mision;
    nuevo->titulo = titulo;
    nuevo->descripcion = descripcion;
    nuevo->requisitos = requisitos;
    nuevo->saldo_otorgado = saldo_otorgado;
    nuevo->prox = nullptr;
    return nuevo;
}

void insertarMision(Mision **lista, string titulo, string descripcion, string requisitos, float saldo_otorgado) {
    Mision *nuevo = crearMision(titulo, descripcion, requisitos, saldo_otorgado);
    if (!*lista) {
        *lista = nuevo;
    } else {
        Mision *temp = *lista;
        while (temp->prox) {
            temp = temp->prox;
        }
        temp->prox = nuevo;
    }
    if (cant_misiones < MAX_MISIONES) {  //Verifica que no exceda el maximo de misiones!!!
        misiones_titulos[cant_misiones++] = titulo;
    }
}

void inicializarMisiones() {
    insertarMision(&lista_misiones, "Primer Acierto", "Adivina el número a la primera.", "Adivinar a la primera", 2);
    insertarMision(&lista_misiones, "Tres Victorias", "Gana tres partidas.", "Alcanzar 3 victorias", 5);
    insertarMision(&lista_misiones, "Jugador Persistente", "Juega 5 partidas.", "Jugar 5 partidas", 3);
}

// Busca el índice de una misión por su título en el arreglo. Si no existe, devuelve -1.
int buscarIndiceMision(string buscado) {
    for (int i = 0; i < cant_misiones; i++) {
        if (misiones_titulos[i] == buscado) {
            return i; // Si lo encuentra devuelve la posición
        }
    }
    return -1; // Si no lo encuentra, devuelve -1
}

void verificarMisiones(Jugador **perfiles) {
    Jugador *jugador_actual = nullptr;
    Jugador *aux_jugador = *perfiles; //recorrer lista de jugadores desde el inicio
    while (aux_jugador != nullptr) {
        if (aux_jugador->id_Jugador == id_logueo) { //compara el id de cada jugador con el id del usuario logueado
            jugador_actual = aux_jugador;
            break;
        }
        aux_jugador = aux_jugador->prox;
    }

    if (jugador_actual) { //solo entra al bloque si se encontro al jugador logueado y lo tengo guardado en jugador actual
        Mision *mision_actual = lista_misiones; //recorrer lista de misiones
        int posicion_mision = 0;
        while (mision_actual != nullptr) { //recorre todas las misiones
            bool cumplida = false;
            if (mision_actual->titulo == "Primer Acierto" && jugador_actual->partidas_jugadas >= 1 && jugador_actual->victorias >= 1) {
                cumplida = true;
            } else if (mision_actual->titulo == "Tres Victorias" && jugador_actual->victorias >= 3) {
                cumplida = true;
            } else if (mision_actual->titulo == "Jugador Persistente" && jugador_actual->partidas_jugadas >= 5) {
                cumplida = true;
            }

            // Si cumplida y no la había completado
            int indice = buscarIndiceMision(mision_actual->titulo); //cada jugador tiene un arreglo de booleanos (misiones_completadas) donde cada posición indica si esa misión está completada para ese jugador.
            if (cumplida && indice != -1 && !jugador_actual->misiones_completadas[indice]) { //Si el índice es -1, significa que el título de la misión actual NO está en el arreglo de títulos de misiones.
                cout << "\n¡Misión Cumplida! '" << mision_actual->titulo << "': +" << mision_actual->saldo_otorgado << " saldo." << endl;
                cout << "Descripción: " << mision_actual->descripcion << endl;
                jugador_actual->saldo += mision_actual->saldo_otorgado; //suma la recompensa de la mision al saldo del jugador
                jugador_actual->misiones_completadas[indice] = true;

                // NUEVO: Agregar logro con descripción detallada y fecha
                string descripcion_detallada = "Has superado la misión '" + mision_actual->titulo + "'. " + mision_actual->descripcion +
                                               " Requisito: " + mision_actual->requisitos + ".";
                agregarLogroObtenido(jugador_actual, mision_actual->titulo, descripcion_detallada);
            }
            mision_actual = mision_actual->prox;
            posicion_mision++;
        }

        // Subida de nivel
        if (jugador_actual->victorias > 5 && jugador_actual->privilegio == 'u') {
            cout << "\n¡Felicidades! Has subido de nivel." << endl;
            jugador_actual->privilegio = 'a';
        }
    }
}

void mostrarMisiones() {
    cout << "\n--- Misiones Disponibles ---" << endl;
    Mision *mision_actual = lista_misiones;
    if (!mision_actual) {
        cout << "No hay misiones disponibles por ahora." << endl;
        return;
    }
    while (mision_actual != nullptr) {
        cout << "Título: " << mision_actual->titulo << endl;
        cout << "Descripción: " << mision_actual->descripcion << endl;
        cout << "Requisitos: " << mision_actual->requisitos << endl;
        cout << "Recompensa: +" << mision_actual->saldo_otorgado << " saldo" << endl;
        cout << "--------------------------" << endl;
        mision_actual = mision_actual->prox;
    }
}
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

    cout<< "Hola, "<<nombre<<" elije una opcion: "<<endl;
    cout<<"1. Apuesta en Adivina el numero"<<endl;
    cout<<"2. Ver Misiones"<<endl;
    cout<<"3. Ver Raking"<<endl;
    cout<<"4. Ver tu perfil"<<endl;
    cout<<"0. Cerrar Sesion"<<endl;
}

// Validaciones

string ValidarNombre(){
    string nombre;
    bool nombreValido = false;

    do {
        cout << "Ingrese su nombre: ";
        getline(cin, nombre);

        nombreValido = true;
        for(char c : nombre) {
            if(isdigit(c)) {
                nombreValido = false;
                cout << "El nombre no debe contener números. Intente de nuevo." << endl;
                break;
            }
        }
        if(nombre.empty()) {
            nombreValido = false;
            cout << "El nombre no puede estar vacío. Intente de nuevo." << endl;
        }
    } while(!nombreValido);

    return nombre;
}

int ValidarEdad(){
    int ano, anoAct = 2025, edad;
    bool anoValido = false;

    do{
        cout << "Ingrese su año de nacimiento: ";
        cin >> ano;

        edad = anoAct - ano;

        if(edad < 18){
            cout<<"No cumples la edad necesaria para registrarte en esta plataforma, tienes que tener +18 años!"<<endl;
        }
        else{
            anoValido = true;
        }
    }while(!anoValido);

    return ano;
}

string ValidarAlias(Jugador *perfiles) {
    Jugador *aux;
    string alias;
    bool aliasValido = false;

    do {
        cout << "Ingrese su alias de juego: ";
        getline(cin, alias);

        aliasValido = true;
        aux = perfiles;
        while (aux != nullptr) {
            if (aux->alias == alias) {
                aliasValido = false;
                cout << "El alias '" << alias << "' ya existe. Intente con otro." << endl;
                break;
            }
            aux = aux->prox;
        }
        if(alias.empty()) {
            aliasValido = false;
            cout << "El alias no puede estar vacío. Intente de nuevo." << endl;
        }
    }while(!aliasValido);

    return alias;
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

    for (int i = 0; i < MAX_MISIONES; i++) nuevo->misiones_completadas[i] = false;
    nuevo->logros_obtenidos = nullptr;

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

    cout<<"\t\tRegistrate"<<endl;
    cout<<"========================================="<<endl;

    cin.ignore();
    nombre = ValidarNombre();
    alias = ValidarAlias(*perfiles);

    cout << "Ingrese su contrasena: ";
    getline(cin, contrasena);

    ano = ValidarEdad();

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
    Jugador *aux;
    string alias,contrasena;
    bool entrar = false;

    cout<<"\t\tIniciar Sesion"<<endl;
    cout<<"========================================="<<endl;
    cin.ignore();
    do{
        aux = perfiles;
        cout<< "Ingrese su Alias: ";
        getline(cin, alias);
        cout<< "Ingrese su Contraseña: ";
        getline(cin, contrasena);

        while(aux != nullptr){
            if(aux->alias == alias && aux->contrasena == contrasena){
                id_logueo = aux->id_Jugador;
                entrar = true;
                return true;
            }
            aux = aux->prox;
        }
        if(!entrar) cout << "Alias o contraseña incorrectos. Intente de nuevo." << endl;
    }while(!entrar);
    return false;
}

Jugador *Ranking(Jugador *perfiles){
    Jugador *aux = perfiles, *ranking = nullptr;

    if (Vacio(perfiles)) {
        cout << "No hay jugadores para mostrar en el ranking." << endl;
    }

    while (aux != nullptr) {
        Jugador *nuevo = new Jugador(*aux);
        nuevo->prox = nullptr;

        // Insertar ordenado en la lista ranking
        if (ranking == nullptr || 
            nuevo->victorias > ranking->victorias ||
            (nuevo->victorias == ranking->victorias && nuevo->derrotas < ranking->derrotas)) {
            // Insertar al inicio
            nuevo->prox = ranking;
            ranking = nuevo;
        } else {
            Jugador *r = ranking;
            while (r->prox != nullptr && (
                r->prox->victorias > nuevo->victorias ||
                (r->prox->victorias == nuevo->victorias && r->prox->derrotas < nuevo->derrotas)
            )) {
                r = r->prox;
            }
            nuevo->prox = r->prox;
            r->prox = nuevo;
        }

        aux = aux->prox;
    }

    return ranking;
}

string espaciar(int tamanio, int valor){
    int espacio = 0;
    string texto = "";

    espacio = valor - tamanio;

    for(int i = 0; i < espacio; i++){
        texto += " ";
    }

    return texto;
}

void ImprimirRanking(Jugador *ranking){
    Jugador *perfil = ranking;
    int pos = 1;

    cout<<"\t\tRANKING DE JUGADORES"<<endl;
    cout<<"=================================================" << endl;
    cout<<"\tTOP|    Alias     | V | D | BS "<<endl;
    cout<<"\t-------------------------------"<<endl;
    while (perfil != nullptr && perfil->id_Jugador < 6){
        cout<<"\t"<< pos << espaciar(to_string(pos).size(), 7)
            << perfil->alias << espaciar(perfil->alias.size(), 13)
            << perfil->victorias << espaciar(to_string(perfil->victorias).size(), 4)
            << perfil->derrotas << espaciar(to_string(perfil->derrotas).size(), 4)
            << perfil->saldo << endl;
        cout<<"\t-------------------------------"<<endl;
        perfil = perfil->prox;
        pos++;
    }
    cout<<"=================================================" << endl;
}

void PerfilJugador(Jugador *perfiles){
    Jugador *aux = perfiles;

    system("cls");
    cout<<"\t\tTu Perfil"<<endl;
    cout<<"========================================="<<endl;
    while(aux != nullptr){
        if(aux->id_Jugador == id_logueo){
            cout<<"\t\tSaldo: Bs."<<aux->saldo<<endl;
            cout<<"Nombre: "<<aux->nombre<<" | "<<"Alias: "<<aux->alias<<endl;
            cout<<"Contaseña: "<<aux->contrasena<<endl;
            cout<<"Victorias: "<<aux->victorias<<" | "<<"Derrotas: "<<aux->derrotas<<endl;
            cout<<"Partidas Jugadas: "<<aux->partidas_jugadas<<endl;
            cout<<"========================================="<<endl;
            cout<<"Logros Obtenidos:"<<endl;
            LogroObtenido *logro = aux->logros_obtenidos;
            if (!logro) {
                cout << "  Ningún logro obtenido aún." << endl;
            }
            while (logro) {
                cout << "- " << logro->titulo << " (" << logro->fecha_obtencion << "): " << logro->descripcion_detallada << endl;
                logro = logro->prox;
            }
            cout<<"========================================="<<endl;
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

    system("cls");
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
                        PuntuacionJugador(perfiles, victorias, derrotas, saldo);
                        verificarMisiones(perfiles);
                    }
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
                verificarMisiones(perfiles);
            }
        }

    }while(op != 0);
}
// Liberar memoria
void LiberarMemoriaJugadores(Jugador *perfiles) {
    while (perfiles != nullptr) {
        Jugador *aux = perfiles;
        perfiles = perfiles->prox;

        LogroObtenido *logro = aux->logros_obtenidos;
        while (logro) {
            LogroObtenido *tmp = logro;
            logro = logro->prox;
            delete tmp;
        }

        delete aux;
    }
}

void LiberarMemoriaMisiones(Mision *misiones) {
    while (misiones != nullptr) {
        Mision *aux = misiones;
        misiones = misiones->prox;
        delete aux;
    }
}