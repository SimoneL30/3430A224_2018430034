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
    for (int i = 0; i <= n; ++i) {
        f[i] = new int[m + 1];
    }

    // Inicializar la primera fila y columna con el puntaje de gaps
    for (int i = 0; i <= n; ++i) f[i][0] = i * gap;
    for (int j = 0; j <= m; ++j) f[0][j] = j * gap;
}

// Calcular la matriz de puntuacion usando el algoritmo de Needleman-Wunsch
void calcularMatriz(int n, int m) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int matchMismatch = f[i - 1][j - 1] + U[S[j - 1]][T[i - 1]];
            int deleteGap = f[i - 1][j] + gap;
            int insertGap = f[i][j - 1] + gap;

            f[i][j] = max(matchMismatch, max(deleteGap, insertGap));
        }
    }
}

// Reconstruir la alineacion optima
void reconstruirAlineacion(int n, int m) {
    string alineacionS, alineacionT;

    while (n > 0 || m > 0) {
        if (n > 0 && m > 0 && f[n][m] == f[n - 1][m - 1] + U[S[m - 1]][T[n - 1]]) {
            alineacionS = S[m - 1] + alineacionS;
            alineacionT = T[n - 1] + alineacionT;
            --n; --m;
        } else if (n > 0 && f[n][m] == f[n - 1][m] + gap) {
            alineacionS = '-' + alineacionS;
            alineacionT = T[n - 1] + alineacionT;
            --n;
        } else {
            alineacionS = S[m - 1] + alineacionS;
            alineacionT = '-' + alineacionT;
            --m;
        }
    }

    cout << "Secuencia 1: " << S << "\n";
    cout << "Secuencia 2: " << T << "\n";
    cout << "Alineacion:\n" << alineacionS << "\n" << alineacionT << "\n";
    cout << "Puntaje maximo: " << f[T.length()][S.length()] << "\n";
}
void generarImagen(const string& archivo, int n, int m) {
    ofstream File(archivo);
    if (!File.is_open()) {
        cerr << "Error: No se pudo crear el archivo " << archivo << "\n";
        exit(1);
    }

    // Encabezado del archivo DOT
    File << "digraph G {\n";
    File << "  graph [rankdir=LR, nodesep=0.5, ranksep=0.5];\n"; // Orientacion izquierda-derecha para facilitar alineacion
    File << "  node [shape=circle, width=0.3, height=0.3, style=filled, fontcolor=black];\n";

    // Primera fila (secuencia S en la parte superior)
    File << "  { rank=source; empty [shape=none, label=\"\"]; "; // Celda vacia en la esquina superior izquierda
    for (int j = 0; j < m; ++j) {
        File << "header" << j << " [label=\"" << S[j] << "\", shape=none]; "; // Encabezados de columnas
    }
    File << "}\n";

    // Crear filas de la matriz (secuencia T a la izquierda y la cuadricula)
    for (int i = 0; i < n; ++i) {
        File << "  { rank=same; ";
        File << "label" << i << " [label=\"" << T[i] << "\", shape=none, fontcolor=black]; "; // Etiqueta de fila a la izquierda
        for (int j = 0; j < m; ++j) {
            string nodeName = "node" + to_string(i) + "_" + to_string(j);
            if (S[j] == T[i]) {
                File << nodeName << " [fillcolor=purple, label=\"\"];\n"; // Nodo morado (coincidencia)
 } else {
                File << nodeName << " [fillcolor=white, label=\"\"];\n"; // Nodo blanco (sin coincidencia)
            }
        }
        File << "}\n";
    }

    // Conexiones horizontales entre nodos en cada fila
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            string currentNode = "node" + to_string(i) + "_" + to_string(j);
            string prevNode = "node" + to_string(i) + "_" + to_string(j - 1);
            File << prevNode << " -> " << currentNode << " [color=gray, arrowhead=none];\n";
        }
    }

    // Conexiones verticales entre filas
    for (int j = 0; j < m; ++j) {
        for (int i = 1; i < n; ++i) {
            string currentNode = "node" + to_string(i) + "_" + to_string(j);
            string prevNode = "node" + to_string(i - 1) + "_" + to_string(j);
            File << prevNode << " -> " << currentNode << " [color=gray, arrowhead=none];\n";
        }
    }

    File << "}\n";
    File.close();

    system(("dot -Tpng " + archivo + " -o matriz_coincidencias.png").c_str());
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
    reconstruirAlineacion(n, m);
     // Generar la imagen de coincidencias
    generarImagen("matriz_coincidencias.dot", n, m);
    for (int i = 0; i <= n; ++i) {
        delete[] f[i];
    }
    delete[] f;

    return 0;
}
