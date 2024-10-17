#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <cstdio>  // Para fopen y fprintf
using namespace std;

#define MAX_N 100  // Tamaño máximo del grafo permitido

// Estructura para representar una arista
struct Arista {
    char u, v;
    int peso;
};

// Clase que representa un grafo mediante una matriz de adyacencia
class Grafo {
private:
    int n;  // Número de nodos
    int matriz[MAX_N][MAX_N];  // Matriz de adyacencia
    char nodos[MAX_N];  // Vértices del grafo

public:
    Grafo(int n) : n(n) {
        inicializar_nodos();
    }

    void cargar_grafo() {
        cout << "Ingrese la matriz de adyacencia (" << n << "x" << n << "):\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> matriz[i][j];
            }
        }
    }

    void imprimir_matriz() const {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }
    }

    int getPeso(int i, int j) const {
        return matriz[i][j];
    }

    char getNodo(int i) const {
        return nodos[i];
    }

    int getNumNodos() const {
        return n;
    }

private:
    void inicializar_nodos() {
        for (int i = 0; i < n; ++i) {
            nodos[i] = 'A' + i;  // Asigna nombres A, B, C, ...
        }
    }
};

// Clase que implementa el algoritmo de Prim
class Prim {
private:
    Grafo& grafo;
    vector<Arista> L;  // Conjunto de aristas seleccionadas (MST)
    vector<bool> enMST;  // Marca si un nodo está en el MST
    vector<int> distancias;  // Pesos mínimos desde el MST hacia los nodos

public:
    Prim(Grafo& g) : grafo(g), enMST(g.getNumNodos(), false), distancias(g.getNumNodos(), INT_MAX) {}

    void ejecutar() {
        distancias[0] = 0;  // Empezamos desde el nodo 0

        for (int i = 0; i < grafo.getNumNodos(); ++i) {
            int u = seleccionar_minimo();
            enMST[u] = true;

            for (int v = 0; v < grafo.getNumNodos(); ++v) {
                int peso = grafo.getPeso(u, v);
                if (peso > 0 && !enMST[v] && peso < distancias[v]) {
                    distancias[v] = peso;
                    L.push_back({grafo.getNodo(u), grafo.getNodo(v), peso});
                }
            }
        }
    }

    void imprimir_conjunto_L() const {
        cout << "Conjunto L (Aristas del MST):\n";
        for (const auto& arista : L) {
            cout << arista.u << " -- " << arista.v << " [Peso: " << arista.peso << "]\n";
        }
    }

    void generar_grafo_dot(const string& filename) const {
        FILE* fp = fopen(filename.c_str(), "w");
        fprintf(fp, "graph G {\n");
        fprintf(fp, "graph [rankdir=LR];\n");
        fprintf(fp, "node [style=filled fillcolor=green];\n");

        // Imprimimos cada arista una sola vez
        for (const auto& arista : L) {
            if (arista.u < arista.v) {  // Asegura que solo imprimimos una dirección (u -- v)
                fprintf(fp, "%c -- %c [label=%d];\n", arista.u, arista.v, arista.peso);
            }
        }

        fprintf(fp, "}\n");
        fclose(fp);

        system("dot -Tpng -ografo.png grafo.txt");
        system("eog grafo.png &"); 
    }

private:
    int seleccionar_minimo() const {
        int minPeso = INT_MAX, minIndice = -1;
        for (int i = 0; i < grafo.getNumNodos(); ++i) {
            if (!enMST[i] && distancias[i] < minPeso) {
                minPeso = distancias[i];
                minIndice = i;
            }
        }
        return minIndice;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <tamaño del grafo>\n";
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 2 || n > MAX_N) {
        cout << "El tamaño del grafo debe ser mayor que 2 y menor o igual a " << MAX_N << ".\n";
        return 1;
    }

    Grafo grafo(n);
    grafo.cargar_grafo();

    cout << "Matriz de adyacencia del grafo:\n";
    grafo.imprimir_matriz();

    Prim prim(grafo);
    prim.ejecutar();
    prim.imprimir_conjunto_L();

    return 0;
}


