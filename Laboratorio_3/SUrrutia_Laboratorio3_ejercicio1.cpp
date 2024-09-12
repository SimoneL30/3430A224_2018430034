#include <iostream>
using namespace std;

struct Pila{
    int dato;
    Pila* siguiente;

};

void agregar(Pila*& cabeza, int dato, int& tope, int maximo){
    Pila* nuevaPila = new Pila();
    nuevaPila->dato = dato;
    nuevaPila->siguiente = cabeza;
    cabeza = nuevaPila;
    tope++;
}
void eliminar(Pila*& cabeza, int num, int tope){
    if (cabeza == NULL){
        return;
    }
    Pila* actual = cabeza;
    Pila* anterior = NULL;

    while (actual != NULL) {
        if (actual->dato == num) { 
            if (anterior == NULL) {
                cabeza = actual->siguiente;
            } 
            else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual; 
            tope--;
            return; 
        }
        anterior = actual;
        actual = actual->siguiente;
    }

}
void imprimirPila(const Pila* cabeza){
    const Pila* actual = cabeza;
    while(actual != nullptr) {
        cout << actual->dato << endl;
        actual = actual->siguiente;
    }
    
}
int menu(Pila* cabeza) {
    int dato;
    int maximo;
    int tope = 0;
    int opcion;

    cout << "Ingresa el tamaño maximo de la pila: ";
    cin >> maximo;

    while (true) {
        cout << "Agregar/push [1]\nRemover/pop  [2]\nVer pila     [3]\nSalir        [0]\n>> ";
        cin >> opcion;
        switch(opcion) {
            case 1: {
                if (tope == maximo) {
                    cout << "La pila esta llena. No se pueden agregar mas datos." << endl;
                } else {
                    for (int i = 0; i < maximo && tope < maximo; i++) {
                        cout << "Ingresa el dato " << (i + 1) << ": ";
                        cin >> dato;
                        agregar(cabeza, dato, tope, maximo);
                    }
                }
                break;
            }
            case 2: {
                int num;
                cout << "Ingresa el dato que vas a eliminar: ";
                cin >> num;
                eliminar(cabeza, num, tope);
                break;
            }
            case 3: {
                if (tope == 0) {
                    cout << "La pila esta vacia. " << endl;
                }
                else {
                    cout << "Los elementos de la pila son: " << endl;
                    imprimirPila(cabeza);
                }
                break;

            }
            case 0: {
                while (cabeza != nullptr) {
                    Pila* temp = cabeza;
                    cabeza = cabeza->siguiente;
                    delete temp;
                }
                return 0;
            }
            default:
            cout << "Opcion invalida. Intenta otra vez.\n";
            
        }

    }

}
int main (){
    Pila* cabeza = nullptr;
    menu(cabeza);

    return 0;

}


