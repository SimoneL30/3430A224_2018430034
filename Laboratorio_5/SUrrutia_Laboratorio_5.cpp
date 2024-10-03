#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define TRUE 1
#define FALSE 0

enum {IZQUIERDO, DERECHO};

struct Nodo {
    int dato;
    int FE;
    Nodo* derecho;
    Nodo* izquierdo;
    Nodo* padre;
};

typedef Nodo* pNodo;
typedef Nodo* Arbol;

/* Insertar en arbol ordenado: */
void Insertar(Arbol* a, int dat);
/* Borrar un elemento: */
void Borrar(Arbol* a, int dat);
/* Funcion de busqueda: */
int Buscar(Arbol a, int dat);
/*Modificar un numero*/
void Modificar(Arbol* a, int viejo, int nuevo);
/* Comprobar si es un nodo hoja: */
int EsHoja(pNodo r);
/* Contar numero de nodos: */
int NumeroNodos(Arbol a, int* c);
/* Calcular la altura de un arbol: */
int AlturaArbol(Arbol a, int* altura);
/* Calcular altura de un dato: */
int Altura(Arbol a, int dat);
/* Generar salida para Graphiz */
void PreOrden(Arbol, std::ofstream &fp);

// Funciones de equilibrado:
void Equilibrar(Arbol* raiz, pNodo nodo, int, int);
void RotaDerechaDerecha(Arbol* raiz, pNodo nodo);
void RotaIzquierdaIzquierda(Arbol* raiz, pNodo nodo);
void RotaDerechaIzquierda(Arbol* raiz, pNodo nodo);
void RotaIzquierdaDerecha(Arbol* raiz, pNodo nodo);

/* Funciones auxiliares: */
void Podar(Arbol* a);
void auxContador(Arbol a, int*);
void auxAltura(Arbol a, int, int*);

void MenuPrincipal();
void GenerarGrafo(Arbol);

int main() {
    Arbol ArbolInt = NULL;
    int opcion = 0;
    int valor, nuevoValor;

    while (opcion != 6) {
        MenuPrincipal();
        std::cout << "Ingrese su opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                std::cout << "Ingrese su numero: ";
                std::cin >> valor;
                Insertar(&ArbolInt, valor);
                break;
            case 2:
                std::cout << "Ingrese el numero a buscar: ";
                std::cin >> valor;
                if (Buscar(ArbolInt, valor)) {
                    std::cout << "El numero " << valor << " fue encontrado en el arbol.\n";
                } else {
                    std::cout << "El numero " << valor << " NO fue encontrado en el arbol.\n";
                }
                break;
            case 3:
                std::cout << "Ingrese el numero a eliminar: ";
                std::cin >> valor;
                Borrar(&ArbolInt, valor);
                break;
            case 4:
                std::cout << "Ingrese el numero a modificar: ";
                std::cin >> valor;
                std::cout << "Ingrese el nuevo valor: ";
                std::cin >> nuevoValor;
                Modificar(&ArbolInt, valor, nuevoValor);
                break;
            case 5:
                GenerarGrafo(ArbolInt);
                break;
        }
    }

    /* Liberar memoria asociada al arbol. */
    Podar(&ArbolInt);
    return 0;
}

void GenerarGrafo(Arbol ArbolInt) {
    std::ofstream fp("grafo.txt");

    fp << "digraph G {\n";
    fp << "node [style=filled fillcolor=yellow];\n";

    fp << "nullraiz [shape=point];\n";
    if (ArbolInt) {
        fp << "nullraiz->" << ArbolInt->dato << " [label=" << ArbolInt->FE << "];\n";
    }
    PreOrden(ArbolInt, fp);

    fp << "}\n";
    fp.close();

    system("dot -Tpng -o grafo.png grafo.txt");
    system("eog grafo.png ");
}

void MenuPrincipal() {
    std::cout << "\n";
    std::cout << "<1> Ingresar numero\n";
    std::cout << "<2> Busqueda numero\n";
    std::cout << "<3> Eliminar numero\n";
    std::cout << "<4> Modificar elemento ingresado\n";
    std::cout << "<5> Generar Grafo\n";
    std::cout << "<6> Salir\n";
    std::cout << "\n";
}

void Podar(Arbol* a) {
    if (*a) {
        Podar(&(*a)->izquierdo);
        Podar(&(*a)->derecho);
        free(*a);
        *a = NULL;
    }
}

void Insertar(Arbol* a, int dat) {
    pNodo padre = NULL;
    pNodo actual = *a;

    while (actual != NULL && dat != actual->dato) {
        padre = actual;

        if (dat < actual->dato)
            actual = actual->izquierdo;
        else if (dat > actual->dato)
            actual = actual->derecho;
    }

    if (actual != NULL)
        return;

    if (padre == NULL) {
        *a = new Nodo{dat, 0, NULL, NULL, NULL};
    } else if (dat < padre->dato) {
        actual = new Nodo{dat, 0, NULL, NULL, padre};
        padre->izquierdo = actual;
        Equilibrar(a, padre, IZQUIERDO, TRUE);
    } else if (dat > padre->dato) {
        actual = new Nodo{dat, 0, NULL, NULL, padre};
        padre->derecho = actual;
        Equilibrar(a, padre, DERECHO, TRUE);
    }
}

int Buscar(Arbol a, int dat) {
    while (a != NULL) {
        if (dat == a->dato) {
            return TRUE; // El dato fue encontrado
        } else if (dat < a->dato) {
            a = a->izquierdo; // Busca en el subárbol izquierdo
        } else {
            a = a->derecho; // Busca en el subárbol derecho
        }
    }
    return FALSE; // El dato no fue encontrado
}

void Borrar(Arbol* a, int dat) {
    if (*a == NULL) {
        std::cout << "El valor " << dat << " no se encuentra en el árbol." << std::endl;
        return;
    }

    if (dat < (*a)->dato) {
        Borrar(&(*a)->izquierdo, dat);
    } else if (dat > (*a)->dato) {
        Borrar(&(*a)->derecho, dat);
    } else {
        // Nodo encontrado
        if ((*a)->izquierdo == NULL) {
            pNodo temp = *a;
            *a = (*a)->derecho;
            delete temp;
        } else if ((*a)->derecho == NULL) {
            pNodo temp = *a;
            *a = (*a)->izquierdo;
            delete temp;
        } else {
            pNodo temp = (*a)->derecho;
            while (temp && temp->izquierdo != NULL) {
                temp = temp->izquierdo; // Encuentra el nodo más a la izquierda
            }
            (*a)->dato = temp->dato; // Copia el dato
            Borrar(&(*a)->derecho, temp->dato); // Elimina el nodo
        }

        std::cout << "El numero " << dat << " ha sido eliminado del árbol." << std::endl;
    }

    // Equilibrar el árbol después de la eliminación
    if (*a != NULL) {
        // Actualiza el factor de equilibrio
        Equilibrar(a, *a, (dat < (*a)->dato) ? IZQUIERDO : DERECHO, FALSE);
    }
}

void Modificar(Arbol* a, int viejo, int nuevo) {
    if (Buscar(*a, viejo)) { // Solo modifica si el nodo existe
        Borrar(a, viejo);
        Insertar(a, nuevo);
    } else {
        std::cout << "El valor " << viejo << " no se encuentra en el árbol." << std::endl;
    }
}

void Equilibrar(Arbol* a, pNodo nodo, int rama, int nuevo) {
    int salir = FALSE;

    while (nodo && !salir) {
        if (nuevo){
            if (rama == IZQUIERDO){
                nodo->FE -= 1;
            } else {
                nodo->FE += 1;
            }
        }
        else{
            if (rama == IZQUIERDO){
                nodo->FE += 1;
            } 
            else {
                nodo->FE -= 1;
            }
        }

        if (nodo->FE == 0)
            salir = TRUE;
        else if (nodo->FE == -2) {
            if (nodo->izquierdo->FE == 1)
                RotaIzquierdaDerecha(a, nodo);
            else
                RotaIzquierdaIzquierda(a, nodo);
            salir = TRUE;
        } else if (nodo->FE == 2) {
            if (nodo->derecho->FE == -1)
                RotaDerechaIzquierda(a, nodo);
            else
                RotaDerechaDerecha(a, nodo);
            salir = TRUE;
        }

        if (nodo->padre) {
            if (nodo->padre->derecho == nodo) {
                rama = DERECHO;
            } else {
                rama = IZQUIERDO;
            }
        }
        nodo = nodo->padre;
    }
}

void RotaIzquierdaDerecha(Arbol* raiz, pNodo nodo) {
    pNodo Padre = nodo->padre;
    pNodo P = nodo;
    pNodo Q = P->izquierdo;
    pNodo R = Q->derecho;
    pNodo B = R->izquierdo;
    pNodo C = R->derecho;

    if (Padre){
        if (Padre->derecho == nodo){
            Padre->derecho = R;
        }
        else {
            Padre->izquierdo = R;
        }
    }
    else
        *raiz = R;

    Q->derecho = B;
    P->izquierdo = C;
    R->izquierdo = Q;
    R->derecho = P;

    R->padre = Padre;
    P->padre = Q->padre = R;
    if (B) B->padre = Q;
    if (C) C->padre = P;

    switch (R->FE) {
        case -1: Q->FE = 0; P->FE = 1; break;
        case 0: Q->FE = 0; P->FE = 0; break;
        case 1: Q->FE = -1; P->FE = 0; break;
    }

    R->FE = 0;
}

void RotaDerechaIzquierda(Arbol* a, pNodo nodo) {
    pNodo Padre = nodo->padre;
    pNodo P = nodo;
    pNodo Q = P->derecho;
    pNodo R = Q->izquierdo;
    pNodo B = R->izquierdo;
    pNodo C = R->derecho;

    if (Padre){
        if (Padre->derecho == nodo){
            Padre->derecho = R;
        } else {
            Padre->izquierdo = R;
        }
    }
    else
        *a = R;

    P->derecho = B;
    Q->izquierdo = C;
    R->izquierdo = P;
    R->derecho = Q;

    R->padre = Padre;
    P->padre = Q->padre = R;
    if (B) B->padre = P;
    if (C) C->padre = Q;

    switch (R->FE) {
        case -1: P->FE = 0; Q->FE = 1; break;
        case 0: P->FE = 0; Q->FE = 0; break;
        case 1: P->FE = -1; Q->FE = 0; break;
    }
    R->FE = 0;
}

void RotaIzquierdaIzquierda(Arbol* a, pNodo nodo) {
    pNodo Padre = nodo->padre;
    pNodo P = nodo;
    pNodo Q = P->izquierdo;
    pNodo B = Q->derecho;

    if (Padre){
        if (Padre->derecho == P){
            Padre->derecho = Q;
        }
        else {
            Padre->izquierdo = Q;
        }
    }
    else
        *a = Q;

    P->izquierdo = B;
    Q->derecho = P;

    P->padre = Q;
    if (B) B->padre = P;
    Q->padre = Padre;

    P->FE = 0;
    Q->FE = 0;
}

void RotaDerechaDerecha(Arbol* a, pNodo nodo) {
    pNodo Padre = nodo->padre;
    pNodo P = nodo;
    pNodo Q = P->derecho;
    pNodo B = Q->izquierdo;

    if (Padre){
        if (Padre->derecho == P){
            Padre->derecho = Q;
        }
        else {
            Padre->izquierdo = Q;
        }
    }
    else
        *a = Q;

    P->derecho = B;
    Q->izquierdo = P;

    P->padre = Q;
    
    if (B){
        B->padre = P;
    } 
    Q->padre = Padre;

    P->FE = 0;
    Q->FE = 0;
}

void PreOrden(Arbol a, std::ofstream &fp) {
    if (a) {
        fp << a->dato << ";\n";
        if (a->izquierdo) {
            fp << a->dato << "->" << a->izquierdo->dato << ";\n";
            PreOrden(a->izquierdo, fp);
        }
        if (a->derecho) {
            fp << a->dato << "->" << a->derecho->dato << ";\n";
            PreOrden(a->derecho, fp);
        }
    }
}
