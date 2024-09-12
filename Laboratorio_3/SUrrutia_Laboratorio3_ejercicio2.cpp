#include <iostream>
using namespace std;

struct Pila {
    int** datos;     // Apuntador a la matriz bidimensional
    int tope;        // indice del tope de la pila (numero de elementos)
    int filas;       // Numero de filas de la matriz
    int columnas;    // Numero de columnas de la matriz
    int maximo;      // Tamaño maximo total de la pila (filas * columnas)
};

void inicializarPila(Pila& pila, int filas, int columnas) {
    pila.filas = filas;
    pila.columnas = columnas;
    pila.maximo = filas * columnas;
    pila.tope = 0;

    // Crear la matriz bidimensional
    pila.datos = new int*[filas];
    for (int i = 0; i < filas; i++) {
        pila.datos[i] = new int[columnas];
        for (int j = 0; j < columnas; j++) {
            pila.datos[i][j] = -1;  //representando una celda vacia
        }
    }
}

void agregar(Pila& pila, int dato) {
    if (pila.tope == pila.maximo) {
        cout << "La pila esta llena. No se pueden agregar mas datos." << endl;
    } 
    else {
        int fila = pila.filas - 1 - (pila.tope / pila.columnas);
        int columna = pila.tope % pila.columnas;
        pila.datos[fila][columna] = dato;
        pila.tope++;
    }
}

void eliminar(Pila& pila) {
    if (pila.tope == 0) {
        cout << "La matriz esta vacia. No se puede eliminar ningun dato." << endl;
    } 
    else {
        pila.tope--;
        int fila = pila.tope / pila.columnas;
        int columna = pila.tope % pila.columnas;
        cout << "Dato eliminado: " << pila.datos[fila][columna] << endl;
        pila.datos[fila][columna] = -1;  // Marcar como vacia
    }
}

void imprimirPila(const Pila& pila) {
    if (pila.tope == 0) {
        cout << "La matriz esta vacia." << endl;
    } 
    else {
        cout << "Los elementos de la pila en forma de matriz son:" << endl;
        for (int i = 0; i < pila.filas; i++) {
            for (int j = 0; j < pila.columnas; j++) {
                if (pila.datos[i][j] == -1) {
                    cout << "X ";  // Mostrar 'X' si la celda esta vacia
                } 
                else {
                    cout << pila.datos[i][j] << " ";
                }
            }
            cout << endl;
        }
    }
}

bool buscarEspacioVacio(Pila& pila, int& fila, int& columna) {
    for (int i = 0; i < pila.filas; i++) {
        for (int j = 0; j < pila.columnas; j++) {
            if (pila.datos[i][j] == -1) {  // Encontrar la primera celda vacia
                fila = i;
                columna = j;
                return true;
            }
        }
    }
    return false;
}

void moverDato(Pila& pila, int fila, int columna, int nuevaColumna) {
    if (pila.datos[fila][columna] == -1) {
        cout << "No hay ningun dato en esa posicion." << endl;
        return;
    }

    // Mover los datos que estan arriba del dato seleccionado
    for (int i = fila - 1; i >= 0; i--) {
        if (pila.datos[i][columna] != -1) {
            int nuevaFila, nuevaColumnaTmp;
            if (buscarEspacioVacio(pila, nuevaFila, nuevaColumnaTmp)) {
                pila.datos[nuevaFila][nuevaColumnaTmp] = pila.datos[i][columna];
                pila.datos[i][columna] = -1;
            } else {
                cout << "No hay espacio vacio para mover los datos bloqueantes." << endl;
                return;
            }
        }
    }

    // Mover el dato seleccionado a la nueva columna
    for (int i = pila.filas - 1; i >= 0; i--) {
        if (pila.datos[i][nuevaColumna] == -1) {  // Encontrar el espacio vacio en la nueva columna
            pila.datos[i][nuevaColumna] = pila.datos[fila][columna];
            pila.datos[fila][columna] = -1;
            cout << "Dato movido exitosamente." << endl;
            return;
        }
    }

    cout << "No hay espacio disponible en la columna seleccionada." << endl;
}

void destruirPila(Pila& pila) {
    for (int i = 0; i < pila.filas; i++) {
        delete[] pila.datos[i];
    }
    delete[] pila.datos;
    pila.tope = 0;
    pila.maximo = 0;
}

int menu() {
    int filas, columnas, dato, fila, columna, nuevaColumna, opcion, cantidad;

    Pila pila;

    cout << "Ingresa el numero de filas de la matriz: ";
    cin >> filas;
    cout << "Ingresa el numero de columnas de la matriz: ";
    cin >> columnas;

    inicializarPila(pila, filas, columnas);  

    while (true) {
        cout << "Agregar/push [1]\nRemover/pop  [2]\nVer matriz   [3]\nMover dato   [4]\nSalir        [0]\n>> ";
        cin >> opcion;
        switch (opcion) {
            case 1: {
                cout << "¿Cuantos datos deseas agregar?: ";
                cin >> cantidad;
                if (cantidad > pila.maximo - pila.tope) {
                    cout << "Solo puedes agregar " << pila.maximo - pila.tope << " datos mas." << endl;
                    cantidad = pila.maximo - pila.tope;
                }
                for (int i = 0; i < cantidad; i++) {
                    cout << "Ingresa el dato " << (i + 1) << ": ";
                    cin >> dato;
                    agregar(pila, dato);
                }
                break;
            }
            case 2: {
                eliminar(pila);
                break;
            }
            case 3: {
                imprimirPila(pila);
                break;
            }
            case 4: {
                cout << "Ingresa la fila del dato a mover (0 a " << filas - 1 << "): ";
                cin >> fila;
                cout << "Ingresa la columna del dato a mover (0 a " << columnas - 1 << "): ";
                cin >> columna;
                cout << "Ingresa la nueva columna para mover el dato (0 a " << columnas - 1 << "): ";
                cin >> nuevaColumna;
                moverDato(pila, fila, columna, nuevaColumna);
                break;
            }
            case 0: {
                destruirPila(pila);
                return 0;
            }
            default: {
                cout << "Opcion invalida. Intenta otra vez." << endl;
                break;
            }
        }
    }
}

int main() {
    menu();
    return 0;
}
