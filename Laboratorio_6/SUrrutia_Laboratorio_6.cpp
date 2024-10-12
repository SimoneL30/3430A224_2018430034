#include <iostream>
#include <cstdlib>
#include <climits>
#include <cstring>

using namespace std;

#define MAX_N 100  // Tamaño máximo del grafo permitido

// Declaración de funciones
void leer_nodos(char vector[], int n);
void inicializar_vector_caracter(char vector[], int n);
void inicializar_vector_D(int D[], int M[MAX_N][MAX_N], int origen, int n);
void aplicar_dijkstra(char V[], char S[], char VS[], int D[], int M[MAX_N][MAX_N], int origen, int n);
void actualizar_VS(char V[], char S[], char VS[], int n);
int buscar_indice_caracter(char V[], char caracter, int n);
void agrega_vertice_a_S(char S[], char vertice, int n);
int elegir_vertice(char VS[], int D[], char V[]);
void actualizar_pesos(int D[], char VS[], int M[MAX_N][MAX_N], char V[], char v, int n);
int calcular_minimo(int dw, int dv, int mvw);
void imprimir_vector_entero(int vector[], int n);
void imprimir_matriz(int M[MAX_N][MAX_N], int n);
void imprimir_grafo(int M[MAX_N][MAX_N], char V[], int n);

// Lee los nodos y los asigna como caracteres consecutivos (A, B, C, ...) en un vector
void leer_nodos(char vector[], int n) {
    for (int i = 0; i < n; i++) {
        vector[i] = 'A' + i;
    }
}
// Inicializa un vector de caracteres llenándolo con espacios (' ')
void inicializar_vector_caracter(char vector[], int n) {
    for (int i = 0; i < n; i++) {
        vector[i] = ' ';
    }
}
// Copia los valores de la fila correspondiente al nodo de origen en el vector de distancias D[]
void inicializar_vector_D(int D[], int M[MAX_N][MAX_N], int origen, int n) {
    for (int i = 0; i < n; i++) {
        D[i] = M[origen][i];
    }
}
// Aplica el algoritmo de Dijkstra para encontrar las distancias mínimas desde el nodo de origen
void aplicar_dijkstra(char V[], char S[], char VS[], int D[], int M[MAX_N][MAX_N], int origen, int n) {
    inicializar_vector_D(D, M, origen, n);

    cout << "Estado inicial:\n";
    imprimir_matriz(M, n);

    agrega_vertice_a_S(S, V[origen], n); // Agrega el vértice de origen al conjunto S
    actualizar_VS(V, S, VS, n); // Actualiza los vértices que aún no están en S

    for (int i = 1; i < n; i++) {
        int v = elegir_vertice(VS, D, V); // Selecciona el vértice con la menor distancia
        agrega_vertice_a_S(S, v, n); // Agrega el vértice seleccionado al conjunto S
        actualizar_VS(V, S, VS, n); // Actualiza los vértices restantes
        actualizar_pesos(D, VS, M, V, v, n); // Actualiza las distancias mínimas
    }
}
// Actualiza el vector VS[] con los vértices que no están en el conjunto S[]
void actualizar_VS(char V[], char S[], char VS[], int n) {
    inicializar_vector_caracter(VS, n); // Inicializa VS con espacios
    int k = 0;
    for (int j = 0; j < n; j++) {
        if (buscar_indice_caracter(S, V[j], n) == -1) {
            VS[k++] = V[j]; // Agrega el vértice a VS
        }
    }
}
// Busca el índice de un carácter en un vector; devuelve -1 si no se encuentra
int buscar_indice_caracter(char V[], char caracter, int n) {
    for (int i = 0; i < n; i++) {
        if (V[i] == caracter) return i;
    }
    return -1;
}
// Agrega un vértice al conjunto S[]
void agrega_vertice_a_S(char S[], char vertice, int n) {
    for (int i = 0; i < n; i++) {
        if (S[i] == ' ') { // Busca el primer espacio disponible
            S[i] = vertice; // Agrega el vértice
            break;
        }
    }
}
// Selecciona el vértice con la menor distancia en D[] que no esté en S[]
int elegir_vertice(char VS[], int D[], char V[]) {
    int menor = INT_MAX, vertice = -1;
    for (int i = 0; VS[i] != ' '; i++) {
        int peso = D[buscar_indice_caracter(V, VS[i], MAX_N)];
        if (peso != -1 && peso < menor) { // Encuentra el mínimo
            menor = peso;
            vertice = VS[i];
        }
    }
    return vertice;
}
// Actualiza los valores en D[] basados en el vértice recién agregado a S[]
void actualizar_pesos(int D[], char VS[], int M[MAX_N][MAX_N], char V[], char v, int n) {
    int indice_v = buscar_indice_caracter(V, v, n);
    for (int i = 0; VS[i] != ' '; i++) {
        int indice_w = buscar_indice_caracter(V, VS[i], n);
        D[indice_w] = calcular_minimo(D[indice_w], D[indice_v], M[indice_v][indice_w]);
    }
}
// Calcula el mínimo entre dos rutas considerando el peso de un arco
int calcular_minimo(int dw, int dv, int mvw) {
    if (dw == -1) return (dv != -1 && mvw != -1) ? dv + mvw : -1;
    return (dv != -1 && mvw != -1) ? min(dw, dv + mvw) : dw;
}
// Imprime los elementos del vector D[] (distancias mínimas)
void imprimir_vector_entero(int vector[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "D[" << i << "]: " << vector[i] << " ";
    }
    cout << endl;
}
// Imprime la matriz de adyacencia del grafo
void imprimir_matriz(int M[MAX_N][MAX_N], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

void imprimir_grafo(int M[MAX_N][MAX_N], char V[], int n) {
    FILE *fp = fopen("grafo.txt", "w");
    fprintf(fp, "digraph G {\n");
    fprintf(fp, "%s\n", "graph [rankdir=LR]");
    fprintf(fp, "%s\n", "node [style=filled fillcolor=green];");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (M[i][j] > 0) {
                fprintf(fp, "%c -> %c [label=%d];\n", V[i], V[j], M[i][j]);
            }
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
    system("dot -Tpng -ografo.png grafo.txt");
    system("eog grafo.png &");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <tamaño del grafo>\n";
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 2 || n > MAX_N) {
        cout << "El tamaño del grafo debe ser mayor que 2 y menor o igual a " << MAX_N << ".\n";
        return 1;
    }

    int M[MAX_N][MAX_N];
    cout << "Ingrese la matriz de adyacencia (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> M[i][j];
        }
    }

    char V[MAX_N], S[MAX_N], VS[MAX_N];
    int D[MAX_N];

    leer_nodos(V, n);
    inicializar_vector_caracter(S, n);
    inicializar_vector_caracter(VS, n);

    cout << "Ingrese el vértice de origen (0-" << n - 1 << "): ";
    int origen;
    cin >> origen;

    aplicar_dijkstra(V, S, VS, D, M, origen, n);
    imprimir_vector_entero(D, n);
    imprimir_grafo(M, V, n);

    return 0;
}

