#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Enum para especificar los métodos de resolución de colisiones
enum CollisionResolution { LINEAL, CUADRATICO, DOBLE_HASH };

// Función para verificar si un número es primo
bool isPrime(int n) {
    if (n <= 1) return false; // Números menores o iguales a 1 no son primos
    if (n <= 3) return true; // 2 y 3 son primos
    if (n % 2 == 0 || n % 3 == 0) return false; // Eliminar múltiplos de 2 y 3
    for (int i = 5; i * i <= n; i += 6) { // Verifica posibles divisores
        if (n % i == 0 || n % (i + 2) == 0) return false; // Múltiplos de 6
    }
    return true; // Si pasa todas las pruebas, es primo
}

// Función para encontrar el próximo número primo
int getNextPrime(int n) {
    while (!isPrime(n)) { // Incrementa hasta encontrar un primo
        n++;
    }
    return n; // Devuelve el número primo encontrado
}

class HashTable {
private:
    vector<int> table; // Tabla hash como un vector
    int size; // Tamaño de la tabla
    CollisionResolution resolution; // Método de resolución de colisiones elegido

    // Función hash principal
    int hashFunction(int key) {
        return key % size; // Calcula la posición en la tabla
    }

    // Segunda función hash para el método de doble hash
    int secondaryHash(int key) {
        return (5 - (key % 5)); // Usa un valor que sea coprimo con `size`
    }

public:
    // Constructor que inicializa la tabla con un tamaño primo
    HashTable(int n, CollisionResolution res) : resolution(res) {
        size = getNextPrime(n);  // Asegura que `size` sea primo
        table.resize(size, -1); // Inicializa la tabla con -1 (vacía)
    }

    // Función para insertar un elemento en la tabla
    void insert(int key) {
        int pos = hashFunction(key); // Calcula la posición inicial
        cout << "K:" << pos << endl; // Muestra la posición calculada

        int i = 0;  // Contador de intentos
        while (table[pos] != -1 && i < size) {  // Limitar el número de intentos a `size`
            cout << "Colisión detectada en posición: " << pos << ". Intentando reasignación ";
            switch (resolution) {
                case LINEAL: // Método de resolución lineal
                    cout << "Lineal" << endl;
                    pos = (pos + 1) % size; // Mueve a la siguiente posición
                    break;
                case CUADRATICO: // Método de resolución cuadrática
                    cout << "Cuadrática" << endl;
                    pos = (pos + i * i) % size; // Calcula nueva posición cuadrática
                    i++;
                    break;
                case DOBLE_HASH: // Método de doble hash
                    cout << "Doble hash" << endl;
                    pos = (pos + i * secondaryHash(key)) % size; // Calcula nueva posición usando la segunda función hash
                    i++;
                    break;
            }
            cout << "Se reasignó en la posición " << pos << endl; // Muestra la nueva posición
        }

        if (i < size) { // Si se encontró una posición vacía
            table[pos] = key; // Inserta el elemento
            cout << "Insertado " << key << " en posición " << pos << endl; 
        } else {
            cout << "No se pudo insertar " << key << " después de " << i << " intentos." << endl;
        }
    }

    // Función para buscar un elemento en la tabla
    bool search(int key) {
        int pos = hashFunction(key); // Calcula la posición inicial
        int i = 0;

        while (table[pos] != key && i < size) {  // Limitar el número de intentos a `size`
            if (table[pos] == -1) { // Si se encuentra una posición vacía
                cout << "No se encuentra el número" << endl; 
                return false; 
            }
            switch (resolution) { // Usa el mismo método de resolución de colisiones
                case LINEAL:
                    pos = (pos + 1) % size; // Mueve a la siguiente posición
                    break;
                case CUADRATICO:
                    pos = (pos + i * i) % size; // Calcula nueva posición cuadrática
                    i++;
                    break;
                case DOBLE_HASH:
                    pos = (pos + i * secondaryHash(key)) % size; // Calcula nueva posición usando la segunda función hash
                    i++;
                    break;
            }
        }

        if (table[pos] == key) { 
            cout << "Fue encontrado en " << pos << endl; 
            return true; 
        } else {
            cout << "No se encuentra el número" << endl; 
            return false; 
        }
    }

    // Función para imprimir la tabla hash
    void print() const {
        for (int val : table) {
            cout << (val == -1 ? "-" : to_string(val)) << " | "; // Muestra un "-" para posiciones vacías
        }
        cout << endl; 
    }
};

// Función para generar un número aleatorio
int generate_random(int n) {
    return rand() % n; // Retorna un número aleatorio entre 0 y n-1
}

int main(int argc, char **argv) {
    // Verifica si se han pasado los argumentos necesarios
    if (argc < 3) {
        cout << "Uso: ./Lab_9 n {L|C|D}" << endl;
        return -1; 
    }

    int n = atoi(argv[1]); 
    char opc = toupper(*argv[2]); // Convierte la opción a mayúsculas
    CollisionResolution resolution;

    // Asigna el método de resolución de colisiones basado en la opción
    if (opc == 'L') resolution = LINEAL;
    else if (opc == 'C') resolution = CUADRATICO;
    else if (opc == 'D') resolution = DOBLE_HASH;
    else {
        cout << "Opción inválida" << endl; 
        return -1; 
    }

    cout << "La opción es: " << opc << endl; // Muestra la opción seleccionada

    srand(time(NULL)); // Inicializa la semilla para números aleatorios
    HashTable hashTable(n, resolution); // Crea la tabla hash

    // Generamos el array inicial con números aleatorios
    vector<int> array;
    for (int i = 0; i < n; i++) {
        array.push_back(generate_random(n * 2));  // Genera un rango más amplio para evitar repetidos
    }

    cout << "El arreglo es:" << endl; 
    for (int val : array) {
        cout << val << " | "; 
    }
    cout << endl;

    cout << "Opción " << opc << " seleccionada para colisiones" << endl; 
    for (int key : array) {
        hashTable.insert(key); // Inserta cada clave en la tabla hash
    }

    cout << "El arreglo con Hash quedó:" << endl; 
    hashTable.print();

    int opcionBuscar;
    while (true) {
        cout << "Desea buscar un numero:\n[0]No\n[1]Si: "; 
        cin >> opcionBuscar; 
        if (opcionBuscar == 0) break; 

        int numeroBuscado;
        cout << "Ingrese el número a buscar: "; 
        cin >> numeroBuscado; 
        hashTable.search(numeroBuscado); // Busca el número en la tabla hash
    }

    return 0; 
}
