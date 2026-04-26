#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>    // _kbhit(), _getch()
#include <windows.h>  // Sleep()

using namespace std;

// ─── COLORES ANSI ────────────────────────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define ROJO    "\033[31m"
#define VERDE   "\033[32m"
#define AMARILLO "\033[33m"
#define CYAN    "\033[36m"
// ─────────────────────────────────────────────────────────────────────────────

// BANCO DE PREGUNTAS
//
// Cada pregunta tiene tres vectores paralelos: el texto, y tres opciones.
// La respuesta correcta es SIEMPRE opcionCorrecta[i].
// Las otras dos opciones son los distractores.
//
// indices  0- 9 → Nivel 1 - FACIL
// indices 10-19 → Nivel 2 - INTERMEDIO
// indices 20-29 → Nivel 3 - DIFICIL
//
vector<string> textoPregunta = {
    // NIVEL FÁCIL
    "¿Cuándo nació Don Bosco?",
    "¿Cuál era el nombre de su madre?",
    "¿Cuál era su nombre completo?",
    "¿A qué edad hizo su primera comunión?",
    "¿A qué edad tuvo el primer sueño?",
    "¿Cómo se llamaba su primera sociedad?",
    "¿A qué edad se ordenó sacerdote?",
    "¿Por qué llamó salesianos a sus seguidores?",
    "¿Dónde nació Don Bosco?",
    "¿Cómo se llamaba su padre?",
    // NIVEL INTERMEDIO
    "¿Qué edad tenía cuando perdió a su padre?",
    "¿Cómo se llamaban sus hermanos?",
    "¿Dónde encontró lugar para su oratorio?",
    "¿Qué institutos religiosos fundó?",
    "¿Cuándo murió Don Bosco?",
    "¿Qué son las tres S salesianas?",
    "¿Cómo se llamaba el alumno santo de 15 años?",
    "¿Cómo se llamaba su perro protector?",
    "¿Quién fue su primer sucesor al frente de los salesianos?",
    "¿Qué sistema educativo creó Don Bosco?",
    // NIVEL DIFÍCIL
    "¿Cuál era la frase del reloj del seminario?",
    "¿Qué actividades se hacían en el oratorio?",
    "¿Cuál fue su frase antes de morir?",
    "¿Cuáles eran las reglas de la Sociedad de la Alegría?",
    "¿En qué año fue canonizado Don Bosco?",
    "¿Qué papa canonizó a Don Bosco?",
    "¿Qué papa lo llamó Padre y Maestro de los Jóvenes?",
    "¿Quién fue el primer muchacho del oratorio?",
    "¿Cómo se llamaba su confesor?",
    "¿A qué edad murió Don Bosco?"
};

vector<string> opcionCorrecta = {
    // NIVEL FÁCIL
    "16 de agosto de 1815", "Margarita Occhiena", "Juan Melchor Bosco Occhiena",
    "11 años", "9 años", "Sociedad de la Alegría", "26 años",
    "Por San Francisco de Sales", "I Becchi, Italia", "Francisco Bosco",
    // NIVEL INTERMEDIO
    "2 años", "Antonio y José", "Valdocco",
    "Salesianos e Hijas de María Auxiliadora", "31 de enero de 1888",
    "Salud, Sabiduría y Santidad", "Domingo Savio", "Gris", "Miguel Rúa",
    "Sistema preventivo",
    // NIVEL DIFÍCIL
    "Las horas pasan lentas para los tristes...", "Juegos, cantos y rezar",
    "Los espero a todos en el paraíso", "Alegría, deber y amigo de Jesús",
    "1934", "Pío XI", "Juan Pablo II", "Bartolomé Garelli", "José Cafasso",
    "72 años"
};

vector<string> opcionDistractor1 = {
    // NIVEL FÁCIL
    "15 de agosto de 1810", "María Auxiliadora", "Juan Bosco",
    "9 años", "7 años", "Sociedad de Jesús", "24 años",
    "Por San Juan Bosco", "Turín, Italia", "José Bosco",
    // NIVEL INTERMEDIO
    "5 años", "Pedro y Luis", "Turín centro",
    "Jesuitas y Franciscanos", "16 de agosto de 1815",
    "Saber, Ser y Servir", "Bartolomé Garelli", "Negro", "Juan Cagliero",
    "Sistema represivo",
    // NIVEL DIFÍCIL
    "El tiempo es oro", "Solo estudiar", "No me olviden",
    "Estudiar, rezar y trabajar", "1929", "Pío IX", "Pío XI",
    "Domingo Savio", "Juan Cagliero", "68 años"
};

vector<string> opcionDistractor2 = {
    // NIVEL FÁCIL
    "31 de enero de 1888", "Ana Bosco", "Juan Francisco Bosco",
    "13 años", "11 años", "Sociedad Salesiana", "28 años",
    "Por María Auxiliadora", "Roma, Italia", "Luis Bosco",
    // NIVEL INTERMEDIO
    "8 años", "Carlos y Miguel", "Roma",
    "Dominicos y Carmelitas", "1 de abril de 1934",
    "Sueño, Santidad y Saber", "Miguel Rúa", "Rocco", "Domingo Savio",
    "Sistema técnico",
    // NIVEL DIFÍCIL
    "Aprovecha el día", "Solo trabajar", "Sigan mi ejemplo",
    "Callar, obedecer y servir", "1888", "Juan Pablo II", "Benedicto XVI",
    "Miguel Rúa", "Francisco Pinardi", "75 años"
};

// CONFIGURACION DE NIVELES
int puntajeParaAvanzar[4] = { 0, 30, 60, 90 };

// VARIABLES DE LOS JUGADORES
string nombreJugador[2];
int    vidasJugador[2];
int    puntajeJugador[2];
int    nivelJugador[2];
bool   jugadorActivo[2];

int segundosPorPregunta = 30;

int preguntasDisponibles[2][10];
int cantidadDisponibles[2];

// FUNCIONES AUXILIARES

void limpiarTerminal() {
    system("cls");
}

void mostrarLinea() {
    cout << "----------------------------------------" << endl;
}

int numeroAleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

string nombreDeNivel(int nivel) {
    switch (nivel) {
        case 1:  return "FACIL";
        case 2:  return "INTERMEDIO";
        case 3:  return "DIFICIL";
        default: return "TERMINADO";
    }
}

void cargarPreguntasDelNivel(int jugador) {
    int indiceInicial = (nivelJugador[jugador] - 1) * 10;
    cantidadDisponibles[jugador] = 10;
    for (int i = 0; i < 10; i++) {
        preguntasDisponibles[jugador][i] = indiceInicial + i;
    }
}

void mostrarEstadoJugadores() {
    mostrarLinea();
    for (int jugador = 0; jugador < 2; jugador++) {
        int metaDelNivel = puntajeParaAvanzar[nivelJugador[jugador]];
        cout << CYAN << nombreJugador[jugador] << RESET
             << " -> Nivel: "    << nombreDeNivel(nivelJugador[jugador])
             << " | Vidas: "     << ROJO << vidasJugador[jugador] << RESET
             << " | Puntaje: "   << AMARILLO << puntajeJugador[jugador] << "/" << metaDelNivel << RESET
             << endl;
    }
    mostrarLinea();
}

char leerRespuestaConCronometro() {
    time_t momentoInicio       = time(0);
    int    ultimoSegundoMostrado = -1;
    char   teclaEscrita        = ' ';

    while (true) {
        int segundosTranscurridos = (int)(time(0) - momentoInicio);
        int segundosRestantes     = segundosPorPregunta - segundosTranscurridos;

        if (segundosRestantes != ultimoSegundoMostrado) {
            ultimoSegundoMostrado = segundosRestantes;
            if (teclaEscrita == ' ')
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: _   " << flush;
            else
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: " << teclaEscrita << "   " << flush;
        }

        if (segundosRestantes <= 0) {
            cout << "\n";
            return '0';
        }

        if (_kbhit()) {
            char teclaPresionada = (char)_getch();

            if (teclaPresionada == 13) {
                cout << "\n";
                return (teclaEscrita == ' ') ? ' ' : teclaEscrita;

            } else if (teclaPresionada == 8) {
                teclaEscrita = ' ';
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: _   " << flush;

            } else {
                teclaEscrita = teclaPresionada;
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: " << teclaPresionada << "   " << flush;
            }
        }

        Sleep(50);
    }
}

void hacerPregunta(int jugador) {

    int posicionAleatoria = numeroAleatorio(0, cantidadDisponibles[jugador] - 1);
    int indicePregunta    = preguntasDisponibles[jugador][posicionAleatoria];

    preguntasDisponibles[jugador][posicionAleatoria] = preguntasDisponibles[jugador][cantidadDisponibles[jugador] - 1];
    cantidadDisponibles[jugador]--;

    string textoOpcion1 = opcionCorrecta[indicePregunta];
    string textoOpcion2 = opcionDistractor1[indicePregunta];
    string textoOpcion3 = opcionDistractor2[indicePregunta];

    for (int vuelta = 0; vuelta < 6; vuelta++) {
        int intercambio = numeroAleatorio(1, 3);
        string temporal;
        if (intercambio == 1)      { temporal = textoOpcion1; textoOpcion1 = textoOpcion2; textoOpcion2 = temporal; }
        else if (intercambio == 2) { temporal = textoOpcion2; textoOpcion2 = textoOpcion3; textoOpcion3 = temporal; }
        else                       { temporal = textoOpcion1; textoOpcion1 = textoOpcion3; textoOpcion3 = temporal; }
    }

    char letraCorrecta;
    if      (textoOpcion1 == opcionCorrecta[indicePregunta]) letraCorrecta = 'a';
    else if (textoOpcion2 == opcionCorrecta[indicePregunta]) letraCorrecta = 'b';
    else                                                     letraCorrecta = 'c';

    // Encabezado del turno
    int metaDelNivel = puntajeParaAvanzar[nivelJugador[jugador]];
    cout << "\nTurno de: " << CYAN << BOLD << nombreJugador[jugador] << RESET
         << "  [Nivel: "   << nombreDeNivel(nivelJugador[jugador])
         << " | Vidas: "   << ROJO << vidasJugador[jugador] << RESET
         << " | Puntaje: " << AMARILLO << puntajeJugador[jugador] << "/" << metaDelNivel << RESET << "]\n";
    mostrarLinea();

    // Pregunta y opciones
    cout << CYAN << BOLD << textoPregunta[indicePregunta] << RESET << "\n";
    cout << "a) " << textoOpcion1 << "\n";
    cout << "b) " << textoOpcion2 << "\n";
    cout << "c) " << textoOpcion3 << "\n\n";

    char respuestaJugador = leerRespuestaConCronometro();

    // TIEMPO AGOTADO
    if (respuestaJugador == '0') {
        cout << ROJO << ">> TIEMPO AGOTADO! Pierdes 1 vida." << RESET << "\n";
        vidasJugador[jugador]--;
        if (vidasJugador[jugador] <= 0) {
            vidasJugador[jugador] = 0;
            jugadorActivo[jugador] = false;
            cout << ROJO << ">> " << nombreJugador[jugador] << " se quedo sin vidas!" << RESET << "\n";
        }
        return;
    }

    if (respuestaJugador == 'A') respuestaJugador = 'a';
    if (respuestaJugador == 'B') respuestaJugador = 'b';
    if (respuestaJugador == 'C') respuestaJugador = 'c';

    cout << ">> Respondiste: " << respuestaJugador << "\n";

    string textoRespuestaCorrecta;
    if      (letraCorrecta == 'a') textoRespuestaCorrecta = textoOpcion1;
    else if (letraCorrecta == 'b') textoRespuestaCorrecta = textoOpcion2;
    else                           textoRespuestaCorrecta = textoOpcion3;

    // EVALUAR RESPUESTA
    switch (respuestaJugador) {
        case 'a':
        case 'b':
        case 'c':
            if (respuestaJugador == letraCorrecta) {
                cout << VERDE << ">> CORRECTO! +10 puntos" << RESET << "\n";
                puntajeJugador[jugador] += 10;
            } else {
                cout << ROJO << ">> INCORRECTO. Pierdes 1 vida." << RESET << "\n";
                cout << ">> Respuesta correcta: " << VERDE << textoRespuestaCorrecta << RESET << "\n";
                vidasJugador[jugador]--;
                if (vidasJugador[jugador] <= 0) {
                    vidasJugador[jugador]  = 0;
                    jugadorActivo[jugador] = false;
                    cout << ROJO << ">> " << nombreJugador[jugador] << " se quedo sin vidas!" << RESET << "\n";
                }
            }
            break;
        default:
            cout << ROJO << ">> Tecla invalida. Pierdes 1 vida." << RESET << "\n";
            vidasJugador[jugador]--;
            if (vidasJugador[jugador] <= 0) {
                vidasJugador[jugador]  = 0;
                jugadorActivo[jugador] = false;
                cout << ROJO << ">> " << nombreJugador[jugador] << " se quedo sin vidas!" << RESET << "\n";
            }
            break;
    }
}

void evaluarAvanceDeNivel(int jugador) {

    if (!jugadorActivo[jugador]) return;

    if (puntajeJugador[jugador] >= puntajeParaAvanzar[nivelJugador[jugador]]) {

        if (nivelJugador[jugador] < 3) {
            cout << "\n" << AMARILLO << "*** " << nombreJugador[jugador]
                 << " supero el nivel " << nombreDeNivel(nivelJugador[jugador])
                 << " con " << puntajeJugador[jugador] << " puntos!"
                 << " Avanza al nivel " << nombreDeNivel(nivelJugador[jugador] + 1)
                 << " ***" << RESET << "\n";
            nivelJugador[jugador]++;
            cargarPreguntasDelNivel(jugador);

        } else {
            cout << "\n" << VERDE << "*** " << nombreJugador[jugador]
                 << " completo todos los niveles con "
                 << puntajeJugador[jugador] << " puntos! ***" << RESET << "\n";
            nivelJugador[jugador]  = 4;
            jugadorActivo[jugador] = false;
        }
    }
}

void mostrarGanador() {
    cout << "\n" << BOLD << "----------------------------------------\n";
    cout << "          RESULTADOS FINALES\n";
    cout << "----------------------------------------\n" << RESET;

    for (int jugador = 0; jugador < 2; jugador++) {
        cout << CYAN << nombreJugador[jugador] << RESET
             << " -> Nivel alcanzado: " << nombreDeNivel(nivelJugador[jugador])
             << " | Vidas: "            << ROJO << vidasJugador[jugador] << RESET
             << " | Puntaje total: "    << AMARILLO << puntajeJugador[jugador] << RESET
             << "\n";
    }
    mostrarLinea();

    // Criterio 1: vidas
    if (vidasJugador[0] > 0 && vidasJugador[1] <= 0) {
        cout << VERDE << BOLD << nombreJugador[0] << " gana por conservar vidas!" << RESET << "\n";
        return;
    }
    if (vidasJugador[1] > 0 && vidasJugador[0] <= 0) {
        cout << VERDE << BOLD << nombreJugador[1] << " gana por conservar vidas!" << RESET << "\n";
        return;
    }

    // Criterio 2: nivel mas alto alcanzado
    if (nivelJugador[0] > nivelJugador[1]) {
        cout << VERDE << BOLD << nombreJugador[0] << " gana por haber llegado mas lejos!" << RESET << "\n";
        return;
    }
    if (nivelJugador[1] > nivelJugador[0]) {
        cout << VERDE << BOLD << nombreJugador[1] << " gana por haber llegado mas lejos!" << RESET << "\n";
        return;
    }

    // Criterio 3: puntaje total acumulado
    if (puntajeJugador[0] > puntajeJugador[1]) {
        cout << VERDE << BOLD << nombreJugador[0] << " gana con " << puntajeJugador[0] << " puntos!" << RESET << "\n";
    } else if (puntajeJugador[1] > puntajeJugador[0]) {
        cout << VERDE << BOLD << nombreJugador[1] << " gana con " << puntajeJugador[1] << " puntos!" << RESET << "\n";
    } else {
        cout << AMARILLO << BOLD << "EMPATE, VUELVAN A JUGAR UNA VEZ MAS!" << RESET << "\n";
    }
}


// FUNCION PRINCIPAL
int main() {
    srand(time(0));

    cout << CYAN << BOLD;
    cout << "----------------------------------------\n";
    cout << "   JUEGO DE PREGUNTAS - DON BOSCO\n";
    cout << "----------------------------------------\n";
    cout << RESET;
    mostrarLinea();

    cout << "Nombre del Jugador 1: ";
    cin >> nombreJugador[0];
    cout << "Nombre del Jugador 2: ";
    cin >> nombreJugador[1];

    for (int jugador = 0; jugador < 2; jugador++) {
        vidasJugador[jugador]   = 3;
        puntajeJugador[jugador] = 0;
        nivelJugador[jugador]   = 1;
        jugadorActivo[jugador]  = true;
        cargarPreguntasDelNivel(jugador);
    }

    cout << "\nPuntaje necesario para avanzar:\n";
    cout << "  Facil      ->  30 pts\n";
    cout << "  Intermedio ->  60 pts\n";
    cout << "  Dificil    ->  90 pts\n";
    cout << "El puntaje se acumula durante toda la partida.\n";
    cout << "Tiempo limite por pregunta: " << segundosPorPregunta << " segundos.\n";

    int numeroDeRonda = 1;

    while (jugadorActivo[0] || jugadorActivo[1]) {

        cout << "\n" << BOLD << "======== RONDA " << numeroDeRonda << " ========" << RESET << "\n";
        mostrarEstadoJugadores();

        for (int jugador = 0; jugador < 2; jugador++) {
            if (jugadorActivo[jugador]) {
                hacerPregunta(jugador);
                evaluarAvanceDeNivel(jugador);
            }
        }

        numeroDeRonda++;
    }
    limpiarTerminal();

    mostrarGanador();

    return 0;
}
