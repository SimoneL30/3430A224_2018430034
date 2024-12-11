#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdlib>

using namespace std;

int U[128][128];  // Matriz de emparejamiento
int gap;          // Puntaje de no emparejar (V)
string S, T;      // Cadenas de ADN
int** f;          // Matriz de puntuacion dinamica
char** traceback; // Matriz de traceback

void mostrarEstadisticasAlineacion(int longitud, int identidad, int similitud, int huecos, float puntaje);
int obtenerIndice(char c) { 
    return static_cast<int>(c); 
}
// Leer la matriz de emparejamiento U desde un archivo
void leerMatriz(const string& archivo) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << archivo << "\n";
        exit(1);
    }

    char bases[] = {'A', 'G', 'C', 'T'};
    string line;
    for (int i = 0; i < 4; ++i) {
        getline(file, line);
        istringstream iss(line);
        for (int j = 0; j < 4; ++j) {
            iss >> U[bases[i]][bases[j]];
        }
    }
    file.close();
}

// Leer una secuencia de un archivo
string leerSecuencia(const string& archivo) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << archivo << "\n";
        exit(1);
    }

    string sequence;
    getline(file, sequence);
    file.close();
    return sequence;
}

// Inicializar la matriz de puntuacion f
void inicializarMatriz(int n, int m) {
    f = new int*[n + 1];
    traceback = new char*[n + 1];
    for (int i = 0; i <= n; ++i) {
        f[i] = new int[m + 1];
        traceback[i] = new char[m + 1];
    }

    // Inicializar la primera fila y columna con el puntaje de gaps
    for (int i = 0; i <= n; ++i) {
        f[i][0] = i * gap;
        traceback[i][0] = 'U'; // Up
    }
    for (int j = 0; j <= m; ++j) {
        f[0][j] = j * gap;
        traceback[0][j] = 'L'; // Left
    }
    traceback[0][0] = '0'; // Start     
}

// Calcular la matriz de puntuacion usando el algoritmo de Needleman-Wunsch
void calcularMatriz(int n, int m) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int indexS = obtenerIndice(S[i - 1]);
            int indexT = obtenerIndice(T[j - 1]);
            
            int matchMismatch = f[i - 1][j - 1] + U[S[j - 1]][T[i - 1]];
            int deleteGap = f[i - 1][j] + gap;
            int insertGap = f[i][j - 1] + gap;

            f[i][j] = max(matchMismatch, max(deleteGap, insertGap));

            if (f[i][j] == matchMismatch) {
                traceback[i][j] = 'D'; // Diagonal
            }
            else if (f[i][j] == deleteGap) {
                traceback[i][j] = 'U'; // Up
            }
            else {
                traceback[i][j] = 'L'; // Left
            }
        }
    }
}
// Imprimir la matriz de traceback
void mostrarTraceback(int n, int m) {
    cout << "\nMatriz de traceback:" << endl;
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            cout << traceback[i][j] << " ";
        }
        cout << endl;
    }
}

// Reconstruir la alineación óptima y calcular estadísticas
void reconstruirAlineacion(const string& S, const string& T, int** f, char** traceback, int n, int m, int U[128][128], int gap) {
    string alineacionS = "", alineacionT = "";
    int identidad = 0, similitud = 0, huecos = 0;

    int i = n, j = m;
    while (i > 0 || j > 0) {
        if (traceback[i][j] == 'D') {
            alineacionS = S[j - 1] + alineacionS;
            alineacionT = T[i - 1] + alineacionT;
            if (S[j - 1] == T[i - 1]) identidad++;
            similitud++;
            --i; --j;
        } else if (traceback[i][j] == 'U') {
            alineacionS = '-' + alineacionS;
            alineacionT = T[i - 1] + alineacionT;
            --i;
            huecos++;
        } else if (traceback[i][j] == 'L') {
            alineacionS = S[j - 1] + alineacionS;
            alineacionT = '-' + alineacionT;
            --j;
            huecos++;
        }
    }

    int longitudAlineacion = alineacionS.size();
    double porcentajeSimilitud = (static_cast<double>(similitud) / longitudAlineacion) * 100.0;

    cout << "Secuencia 1: " << S << "\n";
    cout << "Secuencia 2: " << T << "\n";
    cout << "Alineación:\n" << alineacionS << "\n" << alineacionT << "\n";

    mostrarEstadisticasAlineacion(longitudAlineacion, identidad, similitud, huecos, (float)f[n][m]);
    mostrarTraceback(n, m);
}
// Función para calcular y mostrar estadísticas de alineación
void mostrarEstadisticasAlineacion(int longitud, int identidad, int similitud, int huecos, float puntaje) {
    float porcentajeIdentidad = (float)identidad / longitud * 100;
    float porcentajeSimilitud = (float)similitud / longitud * 100;
    float porcentajeHuecos = (float)huecos / longitud * 100;
    cout << "\n";
    cout << "Longitud: " << longitud << "\n";
    cout << "Identidad: " << identidad << "/" << longitud << " (" << porcentajeIdentidad << "%)\n";
    cout << "Similitud: " << similitud << "/" << longitud << " (" << porcentajeSimilitud << "%)\n";
    cout << "Gaps: " << huecos << "/" << longitud << " (" << porcentajeHuecos << "%)\n";
    cout << "Puntaje máximo: " << puntaje << "\n";
}

void generarImagen(const string& archivo, const string& S, const string& T, int n, int m) {
    ofstream File(archivo);
    if (!File.is_open()) {
        cerr << "Error: No se pudo crear el archivo " << archivo << "\n";
        exit(1);
    }

    // Encabezado del archivo DOT
    File << "digraph G {\n";
    File << "  graph [splines=false, nodesep=0.1, ranksep=0.1, bgcolor=white];\n";
    File << "  node [shape=point, width=0.1, height=0.1, color=black];\n";

    // Crear los nodos y conexiones basadas en coincidencias
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (S[j] == T[i]) {  // Coincidencia
                string nodeName = "node" + to_string(i) + "_" + to_string(j);
                File << "  " << nodeName << " [pos=\"" << j << "," << -i << "!\"];\n";

                // Conexiones diagonales entre coincidencias
                if (i > 0 && j > 0 && S[j - 1] == T[i - 1]) {
                    string prevNodeName = "node" + to_string(i - 1) + "_" + to_string(j - 1);
                    File << "  " << prevNodeName << " -> " << nodeName << " [color=black, penwidth=1.5];\n";
                }
            }
        }
    }

    File << "}\n";
    File.close();

    // Generar la imagen PNG a partir del archivo DOT
    string command = "dot -Tpng -Gdpi=300 " + archivo + " -o coincidencias.png";
    cout << "Ejecutando comando: " << command << endl;
    system(command.c_str());
    cout << "El gráfico de coincidencias ha sido generado como 'coincidencias.png'. Verifica el archivo en el directorio actual.\n";
}

int main(int argc, char* argv[]) {
    if (argc != 9) {
        cerr << "Uso: ./proyecto -C1 Secuencia1.txt -C2 Secuencia2.txt -U matriz.txt -V val\n";
        return 1;
    }

    string fileC1, fileC2, fileU;
    gap = 0;
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-C1") == 0) fileC1 = argv[i + 1];
        else if (strcmp(argv[i], "-C2") == 0) fileC2 = argv[i + 1];
        else if (strcmp(argv[i], "-U") == 0) fileU = argv[i + 1];
        else if (strcmp(argv[i], "-V") == 0) gap = stoi(argv[i + 1]);
        else {
            cerr << "Parametro desconocido: " << argv[i] << "\n";
            return 1;
        }
    }

    leerMatriz(fileU);
    S = leerSecuencia(fileC1);
    T = leerSecuencia(fileC2);

    int n = T.length();  // Filas de la matriz
    int m = S.length();  // Columnas de la matriz

    inicializarMatriz(n, m);
    calcularMatriz(n, m);

    // Reconstruir y mostrar alineacion
    reconstruirAlineacion(S, T, f, traceback, n, m, U, gap);
    // Generar la imagen de coincidencias
    generarImagen("matriz_coincidencias.dot", S, T, n, m);
    for (int i = 0; i <= n; ++i) {
        delete[] f[i];
    }
    delete[] f;

    return 0;
}
