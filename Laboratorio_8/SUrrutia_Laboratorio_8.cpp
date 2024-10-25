#include <iostream>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#include <chrono>  // Para medir el tiempo

using namespace std;

void Reduce(int A[], int INI, int FIN, int& POS) {
    int IZQ = INI;
    int DER = FIN;
    POS = INI; // Inicializar la posición del pivote
    bool BAND = true;

    while (BAND) {
        // Mover DER hacia la izquierda
        while ((A[POS] <= A[DER]) && (POS != DER)) {
            DER--;
        }
        
        if (POS == DER) {
            BAND = false; // No hay más elementos que mover
        } else {
            // Intercambiar A[POS] con A[DER] usando AUX
            int AUX = A[POS];
            A[POS] = A[DER];
            A[DER] = AUX;
            POS = DER; // Actualizar la posición del pivote
        }

        // Mover IZQ hacia la derecha
        while ((A[POS] >= A[IZQ]) && (POS != IZQ)) {
            IZQ++;
        }
        
        if (POS == IZQ) {
            BAND = false; // No hay más elementos que mover
        } else {
            // Intercambiar A[POS] con A[IZQ] usando AUX
            int AUX = A[POS];
            A[POS] = A[IZQ];
            A[IZQ] = AUX;
            POS = IZQ; // Actualizar la posición del pivote
        }
    }
}

void seleccion(int* arr, int N) {
    for (int i = 0; i < N-1; ++i) {
        int MENOR = arr[i];
        int K = i;
        for (int j = i + 1; j < N; ++j) {
            if (arr[j] < MENOR) {
                MENOR = arr[j];
                K = j;
            }
        }
        arr[K] = arr[i];
        arr[i] = MENOR;
    }
}

void llenarArreglo(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 100000 + 1; // Genera un número entre 1 y 10,000,000
    }
}

void mostrarArreglo(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void quickSort(int A[], int N) {
    int PILAMENOR[1000]; // Tamaño máximo de la pila
    int PILAMAYOR[1000]; // Tamaño máximo de la pila
    int TOPE = 0; // Inicializa TOPE en 0

    PILAMENOR[TOPE] = 0; // INI
    PILAMAYOR[TOPE] = N - 1; // FIN
    TOPE++; // Aumentar TOPE

    while (TOPE > 0) {
        TOPE--;
        int INI = PILAMENOR[TOPE];
        int FIN = PILAMAYOR[TOPE];

        int POS;
        Reduce(A, INI, FIN, POS);

        // Si hay elementos a la izquierda del pivote
        if (INI < (POS - 1)) {
            PILAMENOR[TOPE] = INI;
            PILAMAYOR[TOPE] = POS - 1;
            TOPE++;
        }

        // Si hay elementos a la derecha del pivote
        if (FIN > (POS + 1)) {
            PILAMENOR[TOPE] = POS + 1;
            PILAMAYOR[TOPE] = FIN;
            TOPE++;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " N VER" << endl;
        return 1;
    }

    int N = atoi(argv[1]);  // Convertir el primer argumento a entero
    bool VER = atoi(argv[2]) != 0;  // Convertir el segundo argumento a booleano

    const int MAX_SIZE = 10000;
    if (N > MAX_SIZE) {
        cout << "El valor N excede el tamaño máximo permitido de " << MAX_SIZE << "." << endl;
        return 1;
    }

    int arr[N]; // Declarar arreglo con tamaño N
    srand(static_cast<unsigned int>(time(0))); // Inicializar la semilla para números aleatorios

    // Llenar el arreglo
    llenarArreglo(arr, N);

    // Mostrar el arreglo original si VER es verdadero
    if (VER) {
        cout << "\nArreglo de números enteros:\n";
        mostrarArreglo(arr, N);
    }

    // Tiempo para seleccion
    auto start_seleccion = chrono::high_resolution_clock::now();
    seleccion(arr, N);
    auto end_seleccion = chrono::high_resolution_clock::now();
    auto duration_seleccion = chrono::duration_cast<chrono::milliseconds>(end_seleccion - start_seleccion); // Duración en milisegundos

    if (VER) {
        cout << "\nArreglo de números ordenados por Seleccion:\n";
        mostrarArreglo(arr, N);
    }

    // Reiniciar el arreglo para QuickSort
    int arr_quick[N];
    llenarArreglo(arr_quick, N);

    // Tiempo para QuickSort
    auto start_quicksort = chrono::high_resolution_clock::now(); // Iniciar el cronómetro para quicksort
    quickSort(arr_quick, N);
    auto end_quicksort = chrono::high_resolution_clock::now(); // Detener el cronómetro para quicksort
    auto duration_quicksort = chrono::duration_cast<chrono::milliseconds>(end_quicksort - start_quicksort); // Duración en milisegundos

    // Mostrar el arreglo ordenado por QuickSort si VER es verdadero
    if (VER) {
        cout << "\nArreglo de números ordenados por Quicksort:\n";
        mostrarArreglo(arr_quick, N);
    }

    // Mostrar el tiempo de ejecución
    cout << "\nTiempo de ejecución del algoritmo Seleccion: " << duration_seleccion.count() << " milisegundos" << endl;
    cout << "Tiempo de ejecución del algoritmo Quicksort: " << duration_quicksort.count() << " milisegundos" << endl;

    return 0;
}
