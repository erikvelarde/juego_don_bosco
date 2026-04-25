#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #include <conio.h>    // _kbhit(), _getch()
    #include <windows.h>  // Sleep()
#else
    #include <termios.h>    // tcgetattr(), tcsetattr()
    #include <unistd.h>     // read(), STDIN_FILENO
    #include <sys/select.h> // select(), fd_set
#endif

using namespace std;

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

// vector<string> textoPregunta = {
//     // NIVEL FACIL
//     "Cuando nacio Don Bosco?",
//     "Cual era el nombre de su madre?",
//     "Cual era su nombre completo?",
//     "A que edad hizo su primera comunion?",
//     "A que edad tuvo el primer suenio?",
//     "Como se llamaba su primera sociedad?",
//     "A que edad se ordeno sacerdote?",
//     "Por que llamo salesianos a sus seguidores?",
//     "Donde nacio Don Bosco?",
//     "Como se llamaba su padre?",
//     // NIVEL INTERMEDIO
//     "Que edad tenia cuando perdio a su padre?",
//     "Como se llamaban sus hermanos?",
//     "Donde encontro lugar para su oratorio?",
//     "Que institutos religiosos fundo?",
//     "Cuando murio Don Bosco?",
//     "Que son las tres S salesianas?",
//     "Como se llamaba el alumno santo de 15 anos?",
//     "Como se llamaba su perro protector?",
//     "Quien fue el primer sucesor al frente de los salesianos?",
//     "Que sistema educativo creo Don Bosco?",
//     // NIVEL DIFICIL
//     "Cual era la frase del reloj del seminario?",
//     "Que actividades se hacian en el oratorio?",
//     "Cual fue su frase antes de morir?",
//     "Cuales eran las reglas de la Sociedad de la Alegria?",
//     "En que anio fue canonizado Don Bosco?",
//     "Que papa canonizo a Don Bosco?",
//     "Que papa lo llamo Padre y Maestro de los Jovenes?",
//     "Quien fue el primer muchacho del oratorio?",
//     "Como se llamaba su confesor?",
//     "A que edad murio Don Bosco?"
// };

// La respuesta correcta de cada pregunta (siempre en este vector)
// vector<string> opcionCorrecta = {
//     // NIVEL FACIL
//     "16 de agosto de 1815", "Margarita Occhiena", "Juan Melchor Bosco Occhiena",
//     "11 anios", "9 anios", "Sociedad de la Alegria", "26 anios",
//     "Por San Francisco de Sales", "I Becchi, Italia", "Francisco Bosco",
//     // NIVEL INTERMEDIO
//     "2 anios", "Antonio y Jose", "Valdocco",
//     "Salesianos e Hijas de Maria Auxiliadora", "31 de enero de 1888",
//     "Salud, Sabiduria y Santidad", "Domingo Savio", "Gris", "Miguel Rua",
//     "Sistema preventivo",
//     // NIVEL DIFICIL
//     "Las horas pasan lentas para los tristes...", "Juegos, cantos y rezar",
//     "Los espero a todos en el paraiso", "Alegria, deber y amigo de Jesus",
//     "1934", "Pio XI", "Juan Pablo II", "Bartolome Garelli", "Jose Cafasso",
//     "72 anios"
// };
//
// // Primer distractor de cada pregunta
// vector<string> opcionDistractor1 = {
//     // NIVEL FACIL
//     "15 de agosto de 1810", "Maria Auxiliadora", "Juan Bosco",
//     "9 anios", "7 anios", "Sociedad de Jesus", "24 anios",
//     "Por San Juan Bosco", "Turin, Italia", "Jose Bosco",
//     // NIVEL INTERMEDIO
//     "5 anios", "Pedro y Luis", "Turin centro",
//     "Jesuitas y Franciscanos", "16 de agosto de 1815",
//     "Saber, Ser y Servir", "Bartolome Garelli", "Negro", "Juan Cagliero",
//     "Sistema represivo",
//     // NIVEL DIFICIL
//     "El tiempo es oro", "Solo estudiar", "No me olviden",
//     "Estudiar, rezar y trabajar", "1929", "Pio IX", "Pio XI",
//     "Domingo Savio", "Juan Cagliero", "68 anios"
// };
//
// // Segundo distractor de cada pregunta
// vector<string> opcionDistractor2 = {
//     // NIVEL FACIL
//     "31 de enero de 1888", "Ana Bosco", "Juan Francisco Bosco",
//     "13 anios", "11 anios", "Sociedad Salesiana", "28 anios",
//     "Por Maria Auxiliadora", "Roma, Italia", "Luis Bosco",
//     // NIVEL INTERMEDIO
//     "8 anios", "Carlos y Miguel", "Roma",
//     "Dominicos y Carmelitas", "1 de abril de 1934",
//     "Suenio, Santidad y Saber", "Miguel Rua", "Rocco", "Domingo Savio",
//     "Sistema tecnico",
//     // NIVEL DIFICIL
//     "Aprovecha el dia", "Solo trabajar", "Sigan mi ejemplo",
//     "Callar, obedecer y servir", "1888", "Juan Pablo II", "Benedicto XVI",
//     "Miguel Rua", "Francisco Pinardi", "75 anios"
// };

// CONFIGURACION DE NIVELES
// Para avanzar de nivel el jugador necesita alcanzar el puntaje umbral.
// El puntaje se acumula durante toda la partida y nunca se reinicia.
//   Nivel 1 FACIL      → necesita 30 pts para avanzar
//   Nivel 2 INTERMEDIO → necesita 60 pts para avanzar
//   Nivel 3 DIFICIL    → necesita 90 pts para terminar

int puntajeParaAvanzar[4] = { 0, 30, 60, 90 };
// puntajeParaAvanzar[nivelActual] = pts necesarios para salir de ese nivel

// VARIABLES DE LOS JUGADORES

string nombreJugador[2];
int    vidasJugador[2];
int    puntajeJugador[2];   // puntaje total acumulado (NUNCA se reinicia)
int    nivelJugador[2];     // 1=Facil  2=Intermedio  3=Dificil  4=Terminado
bool   jugadorActivo[2];    // false cuando se queda sin vidas o termino el juego

int segundosPorPregunta = 30;

// Indices de las preguntas aun no usadas por cada jugador en su nivel actual
int preguntasDisponibles[2][10];
int cantidadDisponibles[2];

// FUNCIONES AUXILIARES
void mostrarLinea() {
    cout << "----------------------------------------" << endl;
}

int numeroAleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

// Convierte el numero de nivel a su nombre legible
string nombreDeNivel(int nivel) {
    switch (nivel) {
        case 1:  return "FACIL";
        case 2:  return "INTERMEDIO";
        case 3:  return "DIFICIL";
        default: return "TERMINADO";
    }
}

// Carga los indices de preguntas del nivel actual del jugador
void cargarPreguntasDelNivel(int jugador) {
    int indiceInicial = (nivelJugador[jugador] - 1) * 10; // 0, 10 o 20
    cantidadDisponibles[jugador] = 10;
    for (int i = 0; i < 10; i++) {
        preguntasDisponibles[jugador][i] = indiceInicial + i;
    }
}

// Muestra el puntaje y vidas actuales de ambos jugadores
void mostrarEstadoJugadores() {
    mostrarLinea();
    for (int jugador = 0; jugador < 2; jugador++) {
        int metaDelNivel = puntajeParaAvanzar[nivelJugador[jugador]];
        cout << nombreJugador[jugador]
             << " -> Nivel: "    << nombreDeNivel(nivelJugador[jugador])
             << " | Vidas: "     << vidasJugador[jugador]
             << " | Puntaje: "   << puntajeJugador[jugador] << "/" << metaDelNivel
             << endl;
    }
    mostrarLinea();
}

// FUNCION: leerRespuestaConCronometro()
// Muestra una cuenta regresiva en tiempo real mientras el jugador escribe.
// El jugador escribe su letra (a/b/c) y presiona Enter para confirmar.
// Puede usar Backspace para corregir antes de confirmar.
// Si el tiempo se acaba sin que presione Enter, devuelve '0'.

#ifdef _WIN32
// VERSION WINDOWS
// _kbhit() detecta si hay una tecla disponible sin bloquear el programa.
// _getch() la lee sin esperar Enter y sin mostrarla en pantalla.
// Sleep(ms) pausa N milisegundos para no saturar el CPU.
char leerRespuestaConCronometro() {
    time_t momentoInicio       = time(0);
    int    ultimoSegundoMostrado = -1;
    char   teclaEscrita        = ' '; // ' ' = el jugador aun no escribio nada

    while (true) {
        int segundosTranscurridos = (int)(time(0) - momentoInicio);
        int segundosRestantes     = segundosPorPregunta - segundosTranscurridos;

        // Redibujar la linea del cronometro una vez por segundo
        if (segundosRestantes != ultimoSegundoMostrado) {
            ultimoSegundoMostrado = segundosRestantes;
            if (teclaEscrita == ' ')
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: _   " << flush;
            else
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: " << teclaEscrita << "   " << flush;
        }

        // Tiempo agotado: salir sin respuesta
        if (segundosRestantes <= 0) {
            cout << "\n";
            return '0';
        }

        if (_kbhit()) {
            char teclaPresionada = (char)_getch();

            if (teclaPresionada == 13) {
                // Enter (ASCII 13): confirmar lo que se escribio
                cout << "\n";
                return (teclaEscrita == ' ') ? ' ' : teclaEscrita;

            } else if (teclaPresionada == 8) {
                // Backspace (ASCII 8): borrar la tecla escrita
                teclaEscrita = ' ';
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: _   " << flush;

            } else {
                // Cualquier otra tecla: guardarla y mostrarla
                teclaEscrita = teclaPresionada;
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: " << teclaPresionada << "   " << flush;
            }
        }

        Sleep(50); // revisar cada 50ms para no saturar el CPU
    }
}

#else
char leerRespuestaConCronometro() {

    // Guardar la configuracion original para restaurarla al terminar
    struct termios configuracionOriginal;
    tcgetattr(STDIN_FILENO, &configuracionOriginal);

    // Activar modo raw: sin buffer de linea, sin eco automatico
    struct termios modoRaw = configuracionOriginal;
    modoRaw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &modoRaw);

    time_t momentoInicio         = time(0);
    int    ultimoSegundoMostrado = -1;
    char   teclaEscrita          = ' ';
    char   resultado             = '0'; // valor por defecto si se agota el tiempo

    while (true) {
        int segundosTranscurridos = (int)(time(0) - momentoInicio);
        int segundosRestantes     = segundosPorPregunta - segundosTranscurridos;

        // Redibujar la linea del cronometro una vez por segundo
        if (segundosRestantes != ultimoSegundoMostrado) {
            ultimoSegundoMostrado = segundosRestantes;
            if (teclaEscrita == ' ')
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: _   " << flush;
            else
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: " << teclaEscrita << "   " << flush;
        }

        // Tiempo agotado
        if (segundosRestantes <= 0) {
            cout << "\n";
            resultado = '0';
            break;
        }

        // Esperar hasta 100ms a que el jugador presione algo
        fd_set descriptoresListos;
        FD_ZERO(&descriptoresListos);
        FD_SET(STDIN_FILENO, &descriptoresListos);

        struct timeval tiempoEspera;
        tiempoEspera.tv_sec  = 0;
        tiempoEspera.tv_usec = 100000; // 100 milisegundos

        int hayTeclaDisponible = select(STDIN_FILENO + 1, &descriptoresListos, NULL, NULL, &tiempoEspera);

        if (hayTeclaDisponible > 0) {
            char teclaPresionada;
            read(STDIN_FILENO, &teclaPresionada, 1);

            if (teclaPresionada == '\n' || teclaPresionada == '\r') {
                // Enter: confirmar
                cout << "\n";
                resultado = (teclaEscrita == ' ') ? ' ' : teclaEscrita;
                break;

            } else if (teclaPresionada == 127 || teclaPresionada == 8) {
                // Backspace: borrar
                teclaEscrita = ' ';
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: _   " << flush;

            } else {
                // Cualquier otra tecla: guardar y mostrar
                teclaEscrita = teclaPresionada;
                cout << "\rTiempo: " << segundosRestantes << " seg | Tu respuesta: " << teclaPresionada << "   " << flush;
            }
        }
        // Si hayTeclaDisponible == 0 pasaron 100ms sin input, el bucle sigue
    }

    // Restaurar la configuracion original del terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &configuracionOriginal);
    return resultado;
}
#endif

void hacerPregunta(int jugador) {

    // Elegir una pregunta aleatoria de las preguntas disponible del jugador
    int posicionAleatoria = numeroAleatorio(0, cantidadDisponibles[jugador] - 1);
    int indicePregunta    = preguntasDisponibles[jugador][posicionAleatoria];

    // Retirar la pregunta usada (swap con el ultimo elemento)
    preguntasDisponibles[jugador][posicionAleatoria] = preguntasDisponibles[jugador][cantidadDisponibles[jugador] - 1];
    cantidadDisponibles[jugador]--;

    // Copiar las tres opciones para poder mezclarlas libremente
    // La respuesta correcta empieza en textoOpcion1 pero puede terminar en cualquier lugar
    string textoOpcion1 = opcionCorrecta[indicePregunta];
    string textoOpcion2 = opcionDistractor1[indicePregunta];
    string textoOpcion3 = opcionDistractor2[indicePregunta];

    // Mezclar las tres opciones con intercambios aleatorios
    for (int vuelta = 0; vuelta < 6; vuelta++) {
        int intercambio = numeroAleatorio(1, 3);
        string temporal;
        if (intercambio == 1)      { temporal = textoOpcion1; textoOpcion1 = textoOpcion2; textoOpcion2 = temporal; }
        else if (intercambio == 2) { temporal = textoOpcion2; textoOpcion2 = textoOpcion3; textoOpcion3 = temporal; }
        else                       { temporal = textoOpcion1; textoOpcion1 = textoOpcion3; textoOpcion3 = temporal; }
    }

    // Despues de mezclar, detectar en cual letra (a/b/c) quedo la respuesta correcta
    // Solo hay que comparar cual de las tres opciones mezcladas coincide con opcionCorrecta
    char letraCorrecta;
    if      (textoOpcion1 == opcionCorrecta[indicePregunta]) letraCorrecta = 'a';
    else if (textoOpcion2 == opcionCorrecta[indicePregunta]) letraCorrecta = 'b';
    else                                                     letraCorrecta = 'c';

    // Mostrar encabezado del turno
    int metaDelNivel = puntajeParaAvanzar[nivelJugador[jugador]];
    cout << "\nTurno de: " << nombreJugador[jugador]
         << "  [Nivel: "   << nombreDeNivel(nivelJugador[jugador])
         << " | Vidas: "   << vidasJugador[jugador]
         << " | Puntaje: " << puntajeJugador[jugador] << "/" << metaDelNivel << "]\n";
    mostrarLinea();

    // Mostrar la pregunta y las tres opciones ya mezcladas
    cout << textoPregunta[indicePregunta] << "\n";
    cout << "a) " << textoOpcion1 << "\n";
    cout << "b) " << textoOpcion2 << "\n";
    cout << "c) " << textoOpcion3 << "\n\n";

    // Leer la respuesta del jugador con cronometro en tiempo real
    char respuestaJugador = leerRespuestaConCronometro();

    // TIEMPO AGOTADO
    if (respuestaJugador == '0') {
        cout << ">> TIEMPO AGOTADO! Pierdes 1 vida.\n";
        vidasJugador[jugador]--;
        if (vidasJugador[jugador] <= 0) {
            vidasJugador[jugador] = 0;
            jugadorActivo[jugador] = false;
            cout << ">> " << nombreJugador[jugador] << " se quedo sin vidas!\n";
        }
        return;
    }

    // Normalizar a minuscula por si el jugador escribio mayuscula
    if (respuestaJugador == 'A') respuestaJugador = 'a';
    if (respuestaJugador == 'B') respuestaJugador = 'b';
    if (respuestaJugador == 'C') respuestaJugador = 'c';

    cout << ">> Respondiste: " << respuestaJugador << "\n";

    // Obtener el texto de la opcion correcta para mostrarlo si el jugador falla
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
                cout << ">> CORRECTO! +10 puntos\n";
                puntajeJugador[jugador] += 10;
            } else {
                cout << ">> INCORRECTO. Pierdes 1 vida.\n";
                cout << ">> Respuesta correcta: " << textoRespuestaCorrecta << "\n";
                vidasJugador[jugador]--;
                if (vidasJugador[jugador] <= 0) {
                    vidasJugador[jugador]  = 0;
                    jugadorActivo[jugador] = false;
                    cout << ">> " << nombreJugador[jugador] << " se quedo sin vidas!\n";
                }
            }
            break;
        default:
            cout << ">> Tecla invalida. Pierdes 1 vida.\n";
            vidasJugador[jugador]--;
            if (vidasJugador[jugador] <= 0) {
                vidasJugador[jugador]  = 0;
                jugadorActivo[jugador] = false;
                cout << ">> " << nombreJugador[jugador] << " se quedo sin vidas!\n";
            }
            break;
    }
}

// EVALUAR SI EL JUGADOR SUPERO SU NIVEL ACTUAL
// Se llama inmediatamente despues de cada pregunta.
// Condicion: puntaje acumulado >= puntajeParaAvanzar del nivel actual.
void evaluarAvanceDeNivel(int jugador) {

    if (!jugadorActivo[jugador]) return; // jugador eliminado, nada que hacer

    if (puntajeJugador[jugador] >= puntajeParaAvanzar[nivelJugador[jugador]]) {

        if (nivelJugador[jugador] < 3) {
            // Subir al siguiente nivel
            cout << "\n*** " << nombreJugador[jugador]
                 << " supero el nivel " << nombreDeNivel(nivelJugador[jugador])
                 << " con " << puntajeJugador[jugador] << " puntos!"
                 << " Avanza al nivel " << nombreDeNivel(nivelJugador[jugador] + 1)
                 << " ***\n";
            nivelJugador[jugador]++;
            cargarPreguntasDelNivel(jugador);

        } else {
            // Supero el nivel DIFICIL: termino toda la partida
            cout << "\n*** " << nombreJugador[jugador]
                 << " completo todos los niveles con "
                 << puntajeJugador[jugador] << " puntos! ***\n";
            nivelJugador[jugador]  = 4;
            jugadorActivo[jugador] = false;
        }
    }
}

// MOSTRAR GANADOR AL TERMINAR EL JUEGO
// Criterios en orden de prioridad:
//   1. Quien conservo vidas cuando el otro las perdio todas.
//   2. Quien llego a un nivel mas alto.
//   3. Quien acumulo mas puntaje.
//   4. Empate si todo lo anterior es igual.
void mostrarGanador() {
    cout << "\n----------------------------------------\n";
    cout << "          RESULTADOS FINALES\n";
    cout << "----------------------------------------\n";

    for (int jugador = 0; jugador < 2; jugador++) {
        cout << nombreJugador[jugador]
             << " -> Nivel alcanzado: " << nombreDeNivel(nivelJugador[jugador])
             << " | Vidas: "            << vidasJugador[jugador]
             << " | Puntaje total: "    << puntajeJugador[jugador]
             << "\n";
    }
    mostrarLinea();

    // Criterio 1: vidas
    if (vidasJugador[0] > 0 && vidasJugador[1] <= 0) {
        cout << nombreJugador[0] << " gana por conservar vidas!\n";
        return;
    }
    if (vidasJugador[1] > 0 && vidasJugador[0] <= 0) {
        cout << nombreJugador[1] << " gana por conservar vidas!\n";
        return;
    }

    // Criterio 2: nivel mas alto alcanzado
    if (nivelJugador[0] > nivelJugador[1]) {
        cout << nombreJugador[0] << " gana por haber llegado mas lejos!\n";
        return;
    }
    if (nivelJugador[1] > nivelJugador[0]) {
        cout << nombreJugador[1] << " gana por haber llegado mas lejos!\n";
        return;
    }

    // Criterio 3: puntaje total acumulado
    if (puntajeJugador[0] > puntajeJugador[1]) {
        cout << nombreJugador[0] << " gana con " << puntajeJugador[0] << " puntos!\n";
    } else if (puntajeJugador[1] > puntajeJugador[0]) {
        cout << nombreJugador[1] << " gana con " << puntajeJugador[1] << " puntos!\n";
    } else {
        cout << "EMPATE!\n";
    }
}


// FUNCION PRINCIPAL
int main() {
    srand(time(0)); // inicializar el generador de numeros aleatorios

    cout << "----------------------------------------\n";
    cout << "   JUEGO DE PREGUNTAS - DON BOSCO\n";
    cout << "----------------------------------------\n";
    mostrarLinea();

    cout << "Nombre del Jugador 1: ";
    cin >> nombreJugador[0];
    cout << "Nombre del Jugador 2: ";
    cin >> nombreJugador[1];

    // Inicializar ambos jugadores
    for (int jugador = 0; jugador < 2; jugador++) {
        vidasJugador[jugador]   = 3;
        puntajeJugador[jugador] = 0;
        nivelJugador[jugador]   = 1;       // todos empiezan en FACIL
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

    // Bucle principal: continua mientras al menos un jugador siga activo
    while (jugadorActivo[0] || jugadorActivo[1]) {

        cout << "\n======== RONDA " << numeroDeRonda << " ========\n";
        mostrarEstadoJugadores();

        for (int jugador = 0; jugador < 2; jugador++) {
            if (jugadorActivo[jugador]) {
                hacerPregunta(jugador);
                evaluarAvanceDeNivel(jugador);
            }
        }

        numeroDeRonda++;
    }

    mostrarGanador();

    return 0;
}
