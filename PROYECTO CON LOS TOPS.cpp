// Hecho por: Paula Velazquez C.I: 31460243 y Oliver Guillen C.I: 31047247

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <windows.h>
#include <ctime> // Para fecha actual
#include <fstream>
#include <sstream>
#include <iomanip> // Para setprecision

using namespace std;

// Estructuras de Datos

struct LogroObtenido {
    string titulo;
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

int id_logueo;
mt19937 globalGenerador(chrono::system_clock::now().time_since_epoch().count());
Mision *lista_misiones = nullptr; // Cabeza de la lista enlazada de misiones

// LOGO

void Logo(){
    string logo =
        "████████╗██╗   ██╗  ██████╗ █████╗ ███████╗██╗███╗   ██╗ ██████╗ \n"
        "╚══██╔══╝██║   ██║ ██╔════╝██╔══██╗██╔════╝██║████╗  ██║██╔═  ██╗\n"
        "   ██║   ██║   ██║ ██║     ███████║███████╗██║██╔██╗ ██║██║   ██║\n"
        "   ██║   ██║   ██║ ██║     ██╔══██║╚════██║██║██║╚██╗██║██║   ██║\n"
        "   ██║   ╚██████╔╝ ╚██████╗██║  ██║███████║██║██║ ╚████║╚██████╔╝\n"
        "   ╚═╝    ╚═════╝   ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝╚═╝  ╚═══╝ ╚═════╝ ";

    cout << logo << endl;
}

// --- AGREGA LOGRO A LA LISTA DE LOGROS OBTENIDOS ---

bool TieneLogro(Jugador *jugador, const string &titulo_buscado) {
    LogroObtenido *aux = jugador->logros_obtenidos;
    while(aux){
        if (aux->titulo == titulo_buscado) {
            return true;
        }
        aux = aux->prox;
    }
    return false;
}

string obtenerFechaHoy() {
    time_t t = time(nullptr);
    tm *ahora = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%d/%m/%Y", ahora); //El formato "%d/%m/%Y" significa: día con dos dígitos (%d), slash, mes con dos dígitos (%m), slash, año con cuatro dígitos (%Y).
    return string(buf);
}

void agregarLogroObtenido(Jugador *jugador, const string &titulo) {
    if (TieneLogro(jugador, titulo)) {
        return;
    }

    LogroObtenido *nuevo = new LogroObtenido;
    nuevo->titulo = titulo;
    nuevo->fecha_obtencion = obtenerFechaHoy();
    nuevo->prox = nullptr;

    if (!jugador->logros_obtenidos) { //Si el jugador NO tiene logros aún ese será el primero.
        jugador->logros_obtenidos = nuevo;
    } 
    else {
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
}

void inicializarMisiones() { // Con los archivos esto se va!!
    insertarMision(&lista_misiones, "¿La primera es la vencida?", "Adivina el número a la primera.", "Adivinar a la primera", 2);
    insertarMision(&lista_misiones, "Gana dinero extra", "Gana tres partidas.", "Alcanzar 3 victorias", 5);
    insertarMision(&lista_misiones, "Jugador Constante", "Juega 5 partidas.", "Jugar 5 partidas", 3);
}

Mision *buscarMisionPorTitulo(const string& buscado) {
    Mision* actual = lista_misiones;
    while (actual != nullptr) {
        if (actual->titulo == buscado) {
            return actual;
        }
        actual = actual->prox;
    }
    return nullptr;
}

void verificarMisiones(Jugador **perfiles) {
    Jugador *jugador_actual = nullptr;
    Jugador *aux_jugador = *perfiles;

    while (aux_jugador != nullptr) {
        if (aux_jugador->id_Jugador == id_logueo) {
            jugador_actual = aux_jugador;
            break;
        }
        aux_jugador = aux_jugador->prox;
    }

    if (jugador_actual) {
        Mision *mision_actual = lista_misiones;
        while (mision_actual != nullptr) {
            bool cumplida = false;
            if (mision_actual->titulo == "Gana dinero extra" && jugador_actual->victorias >= 3) {
                cumplida = true;
            } else if (mision_actual->titulo == "Jugador Constante" && jugador_actual->partidas_jugadas >= 5) {
                cumplida = true;
            }

            if (cumplida && !TieneLogro(jugador_actual, mision_actual->titulo)) {
                cout << "\n¡Misión Cumplida! '" << mision_actual->titulo << "': +" << mision_actual->saldo_otorgado << " saldo." << endl;
                cout << "Descripción: " << mision_actual->descripcion << endl;
                jugador_actual->saldo += mision_actual->saldo_otorgado;

                // Ahora agregamos la misión como un logro obtenido
                agregarLogroObtenido(jugador_actual, mision_actual->titulo);
            }
            mision_actual = mision_actual->prox;
        }
    }
}

void mostrarMisiones() {
    Mision *mision_actual = lista_misiones;

    cout << "\n--- Misiones Disponibles ---" << endl;
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
// Menus

bool Vacio(Jugador *lista){
    return lista == nullptr;
}

void MenuLogueo(){
    cout<< "Bienvenido a Tu Casino"<<endl;
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

void MenuModificar(){
    cout<<"Que te interesa modificar?: "<<endl;
    cout<<"1. Cambiar Nombre"<<endl;
    cout<<"2. Cambiar Contraseña"<<endl;
    cout<<"0. Salir"<<endl;
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
        cout << "Ingrese su año de nacimiento Ej.(YYYY): ";
        cin >> ano;

        edad = anoAct - ano;

        if(edad < 18){
            cout<<"No cumples la edad necesaria para registrarte en esta plataforma, tienes que tener +18 años!"<<endl;
        }
        else if(edad > 85){
            cout<<"La edad máxima permitida es 85 años. Intente de nuevo."<<endl;
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
    while (perfil != nullptr && perfil->id_Jugador < 6){ // Nota: Este límite (perfil->id_Jugador < 6) podría no ser el deseado para un ranking top N si los IDs no son secuenciales o si solo hay pocos jugadores. Se recomienda limitar por 'pos' o un contador.
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
                cout << "Ningún logro obtenido aún." << endl;
            }
            while (logro) {
                cout << "- " << logro->titulo << " (" << logro->fecha_obtencion << ")" <<endl;
                logro = logro->prox;
            }
            cout<<"========================================="<<endl;
        }
        aux = aux->prox;
    }
}

void ModificarNombre(Jugador **perfiles){
    Jugador *aux = *perfiles;
    string nombre;

    cin.ignore();
    nombre = ValidarNombre();

    while(aux != nullptr){
        if(aux->id_Jugador == id_logueo){
            aux->nombre = nombre;
        }
        aux = aux->prox;
    }
}

void ModificarContrasena(Jugador **perfiles){
    Jugador *aux = *perfiles;
    string contrasena;

    cin.ignore();
    cout << "Ingrese su contrasena: ";
    getline(cin, contrasena);

    while(aux != nullptr){
        if(aux->id_Jugador == id_logueo){
            aux->contrasena = contrasena;
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
    Jugador *jugador_actual = nullptr, *aux = *perfiles;
    int adivinar, num, op = 1;
    
    while(aux != nullptr){
        if(aux->id_Jugador == id_logueo){
            jugador_actual = aux;
            break;
        }
        aux = aux->prox;
    }

    if (!jugador_actual) { // Si no se encuentra el jugador (esto no debería pasar si IniciarSesion funciona bien)
        cout << "Error: Jugador no encontrado." << endl;
        return;
    }

    system("cls");
    cout<<"Bienvenido a Adivina el Numero"<<endl;
    cout<<"--------------------------------------------------------------------"<<endl;
    cout<<"Reglas: "<<endl;
    cout<<"1. Tendras 3 intentos para adivinar un numero del 1 al 10"<<endl;
    cout<<"2. Por cada intento fallido te quitaremos 0.50 bs"<<endl;
    cout<<"3. Pero si adivinas a la primera te ganaras 1.50 bs"<<endl;
    cout<<"4. Si ganas despues del primer intento se te suma 0.50 bs a tu saldo"<<endl;
    cout<<"--------------------------------------------------------------------"<<endl;

    do{
        cout<<"\nQuieres Jugar?"<<endl;
        cout<<"1. Si"<<endl;
        cout<<"0. No"<<endl;
        cout<< "Ingrese una opcion: ";
        cin>> op;

        if(op == 1){
            system("cls");
            adivinar = generarNumeroAleatorioOptimizada(1, 10);
            int i = 0;
            bool ganado = false;

            while(i < 3){
                cout<<"-------------------------------"<<endl;
                cout<<"Tu Saldo es: "<<jugador_actual->saldo<<endl;
                cout<<"-------------------------------"<<endl;
                do {
                    cout << "Ingrese el numero a adivinar: ";
                    cin >> num;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Entrada invalida. Intente de nuevo." << endl;
                        continue;
                    }

                    if (num < 1 || num > 10) {
                        cout << "El intervalo para adivinar es del 1 al 10" << endl;
                    }
                } while (num < 1 || num > 10);

                if(num == adivinar){
                    cout<< "Ganastes!!" << endl;
                    jugador_actual->victorias += 1;

                    if(i == 0){
                        jugador_actual->saldo += 1.50;
                        // Nota: La siguiente línea suma 2 al saldo, esto podría ser un error si ya sumaste 1.50
                        // jugador_actual->saldo += 2; 

                        agregarLogroObtenido(jugador_actual, "¿La primera es la vencida?");
                        verificarMisiones(perfiles);
                    }
                    
                    jugador_actual->saldo += 0.50;
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

                    if(jugador_actual->saldo >= 0.50){
                        jugador_actual->saldo -= 0.50;
                    } else {
                        cout << "No tienes saldo suficiente para seguir jugando." << endl;
                        break;
                    }
                }
                i++;
            }

            if(!ganado){
                cout<<"Perdiste :("<<endl;
                cout<<"El numero era: "<<adivinar<<endl;

                jugador_actual->derrotas += 1;
            }

            PuntuacionJugador(perfiles, jugador_actual->victorias, jugador_actual->derrotas, jugador_actual->saldo);
            verificarMisiones(perfiles);
        }

    }while(op != 0);
}

// Admin

void MenuAdmin(Jugador *perfiles){
    Jugador *aux;
    string nombre;

    aux = perfiles;
    while(aux != nullptr){
        if(aux->id_Jugador == id_logueo){
            nombre = aux->nombre;
        }
        aux = aux->prox;
    }

    cout<< "Hola admin, "<<nombre<<" elije una opcion: "<<endl;
    cout<<"1. Agregar un Jugador"<<endl;
    cout<<"2. Ver Jugadores"<<endl;
    cout<<"3. Eliminar Jugador"<<endl;
    cout<<"4. Agregar una Mision"<<endl;
    cout<<"5. Ver Mision"<<endl;
    cout<<"6. Eliminar Mision"<<endl;
    cout<<"7. Top 3 Mejores Jugadores"<<endl; // Ahora este es el texto del menú
    cout<<"8. Top 5 de Jugadores con mas partidas"<<endl; // Y este
    cout<<"0. Cerrar Sesion"<<endl;
}

void ImprimirJugadores(Jugador *perfiles){
    Jugador *perfil = perfiles;

    cout<<"\t\tTABLA DE JUGADORES"<<endl;
    cout<<"=================================================" << endl;
    cout<<"\tID|    Alias     | PJ | V | D | BS "<<endl;
    cout<<"\t-----------------------------------"<<endl;
    while (perfil != nullptr){
        if(perfil->privilegio == 'u'){
            cout<<"\t"<< perfil->id_Jugador << espaciar(to_string(perfil->id_Jugador).size(), 7)
            << perfil->alias << espaciar(perfil->alias.size(), 13)
            << perfil->partidas_jugadas << espaciar(to_string(perfil->partidas_jugadas).size(), 4)
            << perfil->victorias << espaciar(to_string(perfil->victorias).size(), 4)
            << perfil->derrotas << espaciar(to_string(perfil->derrotas).size(), 4)
            << perfil->saldo << endl;
            cout<<"\t------------------------------------"<<endl;
        }
        perfil = perfil->prox;
    }
    cout<<"=================================================" << endl;
}
void EliminarJugador(Jugador **perfiles){
    Jugador *temp = *perfiles, *anterior = nullptr;
    int id_eliminar;

    system("cls");
    cout << "\nEliminar Jugador" << endl;
    cout << "=========================================" << endl;
    cout << "Ingrese el ID del jugador a eliminar: ";
    cin >> id_eliminar;

    if (*perfiles == nullptr) {
        cout << "No hay jugadores para eliminar." << endl;
        return;
    }
    while (temp != nullptr) {
        if (temp->id_Jugador == id_eliminar) {
            if (anterior == nullptr) {
                *perfiles = temp->prox;
            } else {
                anterior->prox = temp->prox;
            }

            
            LogroObtenido *LogroActual = temp->logros_obtenidos;
            while (LogroActual != nullptr) {
                LogroObtenido *logroaEliminar = LogroActual;
                LogroActual = LogroActual->prox;
                delete logroaEliminar;
            }

            delete temp;
            cout << "El jugador con ID " << id_eliminar << " se eliminó con éxito " << endl;
            return; 
        }
        anterior = temp;
        temp = temp->prox;
    }
    cout << "Jugador con ID " << id_eliminar << " no encontrado " << endl;
}

void MostrarTop3Ganadores(Jugador *jugadores) {
    system("cls");
    cout << "\t\tTOP 3 JUGADORES GANADORES" << endl;
    cout << "=================================================" << endl;
    cout << "\tPOS |   Nombre    | V | D | BS " << endl;
    cout << "\t-----------------------------------" << endl;

    Jugador *rankingJugadores = RankingPorVictorias(jugadores); 
    if (Vacio(rankingJugadores)) {
        cout << "No hay jugadores para mostrar." << endl;
    } else {
        Jugador *actual = rankingJugadores;
        int i = 0;
        while (actual != nullptr && i < 3) { // Solo los 3 primeros
            cout << "\t" << (i + 1) << espaciar(to_string(i + 1).size(), 7)
                 << actual->alias << espaciar(actual->alias.size(), 13)
                 << actual->victorias << espaciar(to_string(actual->victorias).size(), 4)
                 << actual->derrotas << espaciar(to_string(actual->derrotas).size(), 4)
                 << fixed << setprecision(2) << actual->saldo << endl;
            cout << "\t-----------------------------------" << endl;
            actual = actual->prox;
            i++;
        }
    }
    cout << "=================================================" << endl;
    LiberarMemoriaJugadores(rankingJugadores); 
}

Jugador *RankingPorVictorias(Jugador *jugadores) {
    Jugador *aux = jugadores, *lista = nullptr;

    if (Vacio(jugadores)) {
        return nullptr;
    }

    while (aux != nullptr) {
        Jugador *nuevo = new Jugador(*aux); 
        nuevo->prox = nullptr;


        if (lista == nullptr ||
            nuevo->victorias > lista->victorias ||
            (nuevo->victorias == lista->victorias && nuevo->derrotas < lista->derrotas)) {
            nuevo->prox = lista;
            lista = nuevo;
            }
            else{
                Jugador *actual = lista;
                while (actual->prox != nullptr && (actual->prox->victorias > nuevo->victorias ||(actual->prox->victorias == nuevo->victorias && actual->prox->derrotas < nuevo->derrotas))) {
                    actual=actual->prox; 
                    }
                    nuevo->prox = actual->prox;
                    actual->prox = nuevo;
            }
    }
}
    

Jugador *RankingPorPartidas(Jugador *jugadores){
    Jugador *aux = jugadores, *lista = nullptr;

    if (Vacio(jugadores)) {
        return nullptr;
    }

    while (aux != nullptr) {
        Jugador *nuevo = new Jugador(*aux); 
        nuevo->prox = nullptr;


        if (lista == nullptr ||
            nuevo->partidas_jugadas > lista->partidas_jugadas ||
            (nuevo->partidas_jugadas == lista->partidas_jugadas && nuevo->derrotas < lista->derrotas)) {
            nuevo->prox = lista;
            lista = nuevo;
        } else {
            Jugador *actual = lista;
                while (actual->prox != nullptr && (actual->prox->victorias > nuevo->victorias ||(actual->prox->victorias == nuevo->victorias && actual->prox->derrotas < nuevo->derrotas))) {
                    actual=actual->prox; 
                    }
                    nuevo->prox = actual->prox;
                    actual->prox = nuevo;
        }
        aux = aux->prox;
    }
    return lista;
}

void MostrarTop5Partidas(Jugador *jugadores) {
    system("cls");
    cout << "\t\tTOP 5 JUGADORES CON MÁS PARTIDAS" << endl;
    cout << "=================================================" << endl;
    cout << "\tPOS |   Nombre    | PJ | V | D | BS " << endl;
    cout << "\t-----------------------------------" << endl;

    Jugador *jugadoresClasificados = RankingPorPartidas(jugadores); 
    if (Vacio(jugadoresClasificados)) {
        cout << "No hay jugadores para mostrar." << endl;
    } else {
        Jugador *actual = jugadoresClasificados;
        int i = 0;
        while (actual != nullptr && i < 5) {
            cout << "\t" << (i + 1) << espaciar(to_string(i + 1).size(), 7)
                 << actual->alias << espaciar(actual->alias.size(), 13)
                 << actual->partidas_jugadas << espaciar(to_string(actual->partidas_jugadas).size(), 4)
                 << actual->victorias << espaciar(to_string(actual->victorias).size(), 4)
                 << actual->derrotas << espaciar(to_string(actual->derrotas).size(), 4)
                 << fixed << setprecision(2) << actual->saldo << endl;
            cout << "\t-----------------------------------" << endl;
            actual = actual->prox;
            i++;
        }
    }
    cout << "=================================================" << endl;
    LiberarMemoriaJugadores(jugadoresClasificados); 
}


void CargarJugadoresDesdeArchivo(Jugador **perfiles, const string &filename) {
    ifstream archivo(filename);
    int id_Jugador, ano, victorias, derrotas, partidas_jugadas;
    string nombre, alias, contrasena, line;
    float saldo;
    char privilegio;

    if (archivo.is_open()) {
        while (getline(archivo, line)) {
            if (line.empty()){
                continue;
            }
            if (line == "---") {
                continue;
            }

            stringstream ss(line);
            string segmento;

            getline(ss, segmento, '|');
            id_Jugador = stoi(segmento);
            
            getline(ss, nombre, '|');

            getline(ss, alias, '|');

            getline(ss, contrasena, '|');

            getline(ss, segmento, '|');
            ano = stoi(segmento);

            getline(ss, segmento, '|');
            victorias = stoi(segmento);

            getline(ss, segmento, '|');
            derrotas = stoi(segmento);

            getline(ss, segmento, '|');
            partidas_jugadas = stoi(segmento);

            getline(ss, segmento, '|');
            saldo = stof(segmento);

            getline(ss, segmento, '|');
            privilegio = segmento[0];

            // Creo nuevo nodo
            Jugador *nuevoJugador = new Jugador;
            nuevoJugador->id_Jugador = id_Jugador;
            nuevoJugador->nombre = nombre;
            nuevoJugador->alias = alias;
            nuevoJugador->contrasena = contrasena;
            nuevoJugador->ano = ano;
            nuevoJugador->victorias = victorias;
            nuevoJugador->derrotas = derrotas;
            nuevoJugador->partidas_jugadas = partidas_jugadas;
            nuevoJugador->saldo = saldo;
            nuevoJugador->privilegio = privilegio;
            nuevoJugador->logros_obtenidos = nullptr;
            nuevoJugador->prox = nullptr;

            // Agrego a la lista
            if (*perfiles == nullptr) {
                *perfiles = nuevoJugador;
            } else {
                Jugador *aux = *perfiles;
                while (aux->prox != nullptr) {
                    aux = aux->prox;
                }
                aux->prox = nuevoJugador;
            }

            while (getline(archivo, line) && line != "---") {
                if (line.empty()) {
                    continue;
                }
                
                size_t pos = line.find('|');
                if (pos != string::npos) {
                    string titulo_logro = line.substr(0, pos);
                    string fecha_obtencion_logro = line.substr(pos + 1);

                    LogroObtenido *nuevoLogro = new LogroObtenido;
                    nuevoLogro->titulo = titulo_logro;
                    nuevoLogro->fecha_obtencion = fecha_obtencion_logro;
                    nuevoLogro->prox = nullptr;

                    if (nuevoJugador->logros_obtenidos == nullptr) {
                        nuevoJugador->logros_obtenidos = nuevoLogro;
                    } else {
                        LogroObtenido *auxLogro = nuevoJugador->logros_obtenidos;
                        while (auxLogro->prox != nullptr) {
                            auxLogro = auxLogro->prox;
                        }
                        auxLogro->prox = nuevoLogro;
                    }
                }
            }
        }
        archivo.close();
    } 
    else{
        cout << "No se pudo abrir el archivo '" << filename << "' para lectura." << endl;
    }
}

void GuardarJugadoresEnArchivo(Jugador *perfiles, const string &filename) {
    ofstream archivo(filename);
    Jugador *currentJugador = perfiles;

    if (archivo.is_open()) {
        while (currentJugador != nullptr) {
            archivo << currentJugador->id_Jugador << "|"
                    << currentJugador->nombre << "|"
                    << currentJugador->alias << "|"
                    << currentJugador->contrasena << "|"
                    << currentJugador->ano << "|"
                    << currentJugador->victorias << "|"
                    << currentJugador->derrotas << "|"
                    << currentJugador->partidas_jugadas << "|"
                    << fixed << setprecision(2) << currentJugador->saldo << "|"
                    << currentJugador->privilegio << endl;

            LogroObtenido *currentLogro = currentJugador->logros_obtenidos;
            while (currentLogro != nullptr) {
                
                archivo << currentLogro->titulo << "|"
                        << currentLogro->fecha_obtencion << endl;
                currentLogro = currentLogro->prox;
            }

            if (currentJugador->prox != nullptr || currentJugador->logros_obtenidos != nullptr) {
                archivo << "---" << endl;
            }
            
            currentJugador = currentJugador->prox;
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo '" << filename << "' para escritura." << endl;
    }
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