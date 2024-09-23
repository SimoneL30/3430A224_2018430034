 #include <iostream>
#include <fstream>
#include <string>


using namespace std;

struct Node {
    int info;
    Node* left;
    Node* right;
};

// Crear un nuevo nodo
Node* createNode(int data) {
    Node* newNode = new Node;
    newNode->info = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

class Arbol {
private:
    Node* root = nullptr;

    // Metodo recursivo privado para insertar un nodo en el arbol binario de busqueda
    Node* insertNode(Node* node, int data) {
        if (node == nullptr) {
            // Crear un nuevo nodo si se encuentra una posicion vacia
            return createNode(data);
        }

        if (data < node->info) {
            // Insertar en el subarbol izquierdo
            node->left = insertNode(node->left, data);
        } else if (data > node->info) {
            // Insertar en el subarbol derecho
            node->right = insertNode(node->right, data);
        } else {
            // El dato ya esta en el arbol
            cout << "El nodo ya se encuentra en el árbol: " << data << endl;
        }

        return node;
    }

    // Metodo recursivo privado para imprimir el arbol en preorden
    void printPreOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }
        cout << node->info << " "; // Imprimir el dato del nodo actual
        printPreOrder(node->left);  // Recorrer el subarbol izquierdo
        printPreOrder(node->right); // Recorrer el subarbol derecho
    }
    // Metodo recursivo privado para imprimir el arbol en Inorden
    void printInOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }
        printInOrder(node->left);  // Recorrer el subarbol izquierdo
        cout << node->info << " "; // Imprimir el dato del nodo actual
        printInOrder(node->right); // Recorrer el subarbol derecho
    }
    // Metodo recursivo privado para imprimir el arbol en prostorden
    void printPostOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }
        printPostOrder(node->left);  // Recorrer el subarbol izquierdo
        printPostOrder(node->right); // Recorrer el subarbol derecho
        cout << node->info << " "; // Imprimir el dato del nodo actual
    }
    bool search(Node* root, int dato) {
        while (root != nullptr) {
            if (root->info == dato) {
                return true;  // Retorna verdadero si encuentra el dato
            } 
            else if (dato < root->info) {
                root = root->left;  // Mueve el puntero al subárbol izquierdo
            }
            else {
                root = root->right;  // Mueve el puntero al subárbol derecho
            }
        }
        return false;  // Retorna falso si no encuentra el dato
    }

    //Eliminar un nodo
    Node* removeNode(Node* root, int key) {
    if (!root) {
        return nullptr; // El nodo no se encontró
    }

    // Buscar el nodo a eliminar
    if (key < root->info) {
        root->left = removeNode(root->left, key);
    } else if (key > root->info) {
        root->right = removeNode(root->right, key);
    } else { // Nodo encontrado
        // Nodo sin hijo o con un solo hijo
        if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp; // Retorna el hijo derecho
        }
        if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp; // Retorna el hijo izquierdo
        }

        // Nodo con dos hijos: encontrar el mínimo del subárbol derecho
        Node* temp = root->right;
        while (temp && temp->left) {
            temp = temp->left; // Encontrar el mínimo
        }

        // Reemplazar el valor del nodo actual con el mínimo
        root->info = temp->info;
        // Eliminar el nodo duplicado en el subárbol derecho
        root->right = removeNode(root->right, temp->info);
    }
    return root; // Retornar la nueva raíz
}

public:
    Arbol() : root(nullptr) {}

    // Metodo publico para insertar un nodo en el arbol
    void insert(int data) {
        root = insertNode(root, data);
    }

    // Metodo publico para imprimir el arbol en preorden
    void printPreOrder() const {
        printPreOrder(root);
        cout << endl;
    }

    void printInOrder() const {
        printInOrder(root);
        cout << endl;
    }
    void printPostOrder() const {
        printPostOrder(root);
        cout << endl;
    }

    void search(int data) {
        bool found = search(root, data);
        if (found) {
            cout << "Se encontró el dato" << endl;
        } 
        else {
            cout << "No se encontró el dato" << endl;
        }
    }
    void remove(int data) {
        // Buscar el dato en el árbol
        if (!search(root, data)) {
            cout << "No se encontró el dato" << endl;
            return; // Salir si el dato no existe
        }
        // Si el dato existe, eliminarlo
        cout << "Se encontró el dato, se eliminará" << endl;
        removeNode(root, data);
    }

    void modify(int previous, int later) {
        if (search(root, later)) {
        cout << "El dato que se intenta agregar ya existe." << endl;
        return; // Salir si el dato ya está en el árbol
    }

    if (!search(root, previous)) {
        cout << "No se encontró el dato a eliminar." << endl;
        return; // Salir si el dato a eliminar no está en el árbol
    }
 
    // Si ambos datos son válidos, proceder con la modificación
    remove(previous);
    insert(later);
    cout << "Número modificado." << endl;  
    }
    void recorrer(Node* node, ofstream& fp) {
        if (node != nullptr) {
            if (node->left != nullptr) {
                fp << to_string(node->info) << "->" << to_string(node->left->info) << ";" << endl;
            } else {
                string cadena = "\""+ to_string(node->info) + "i\"";
                fp << cadena << "[shape=point];" << endl;
                fp << to_string(node->info) << "->" << cadena << ";" << endl;
            }

            if (node->right != nullptr) {
                fp << to_string(node->info)  << "->" << to_string(node->right->info) << ";" << endl;
            } else {
                string cadena = "\"" + to_string(node->info) + "d\"";
                fp << cadena << "[shape=point];" << endl;
                fp << to_string(node->info) << "->" << cadena << ";" << endl;
            }

            recorrer(node->left, fp);
            recorrer(node->right, fp);
        }
    }

    // Generar y mostrar la visualizacion del arbol
    void visualize() {
        ofstream fp("arbol.txt");

        if (!fp.is_open()) {
            cerr << "Error al abrir el archivo arbol.txt" << endl;
            return;
        }

        fp << "digraph G {" << endl;
        fp << "node [style=filled fillcolor=purple];" << endl;

        recorrer(root, fp);

        fp << "}" << endl;

        fp.close();

        // Generar y mostrar la imagen del arbol
        system("dot -Tpng -o arbol.png arbol.txt");
        system("eog arbol.png");
    }

};

bool verify(float options) {
    int integer = (int)options;  // Convierte el número a entero
    return (options - integer) == 0;  // Verifica si la parte decimal es cero
}
void find(Arbol& arbol) {
    float number_to_find;
    cout << "Ingresa un número entero para buscar en el árbol: ";
    cin >> number_to_find;

    if (verify(number_to_find)) {
        arbol.search(static_cast<int>(number_to_find));  // Cast a entero para la búsqueda
    } else {
        cout << "Ingresa un valor válido" << endl;
    }
}

void add(Arbol& arbol) {
    float number_to_add;

    try {
        // Solicitar y validar la entrada del usuario
        cout << "Ingresa un número entero para agregar en el árbol: ";
        cin >> number_to_add;

        if (!verify(number_to_add)) {
            throw invalid_argument("Número inválido.");
        }

        // Insertar el número en el árbol
        arbol.insert(number_to_add);
        cout << "Se ha insertado el número." << endl;
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << ". Inténtelo de nuevo." << endl;
    }
}
void change(Arbol& arbol) {
    float number_to_change, number_to_enter;

    try {
        // Solicitar el número a modificar
        cout << "Ingresa el número entero a modificar del árbol: ";
        cin >> number_to_change;

        // Verificar si la entrada es válida
        if (!verify(number_to_change)) {
            throw invalid_argument("Número invalido"); // Lanzar excepción si no es válido
        }

        // Solicitar el nuevo número a ingresar
        cout << "Ingresa el nuevo número entero a colocar: ";
        cin >> number_to_enter;

        // Verificar si la entrada es válida
        if (!verify(number_to_enter)) {
            throw invalid_argument("Número inválido."); // Lanzar excepción si no es válido
        }

        // Modificar el número en el árbol
        arbol.modify(number_to_change, number_to_enter);
        cout << "Número modificado." << endl;
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << ". Inténtalo de nuevo." << endl;
    }
}
void eliminate(Arbol& arbol) {
    float number_to_eliminate;

    try {
        // Solicitar el número y validarlo
        cout << "Ingresa un número entero para eliminar del árbol: ";
        cin >> number_to_eliminate;

        if (!verify(number_to_eliminate)) {
            throw invalid_argument("Número inválido.");
        }

        // Eliminar el número del árbol
        arbol.remove(number_to_eliminate);
        cout << "Número eliminado." << endl;
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << ". Inténtalo de nuevo." << endl;
    }
}
void menu(Arbol& arbol) {
    int option;

    while(true){
        cout << "Ingresa una opcion" << endl;
        cout << "Visualizar el Arbol [1]" << endl;
        cout << "Eliminar un número  [2]" << endl;
        cout << "Modificar un nodo   [3]" << endl;
        cout << "Buscar un número    [4]" << endl;
        cout << "Insertar un número  [5]" << endl;
        cout << "Salir               [0]\n>> ";
        cin >> option;
        switch (option) {
            case 1: {
                int num;
                cout << "Elija el orden en el que quiere visualizar el Arbol." << endl;
                cout << "1. Pre-orden." << endl;
                cout << "2. En orden." << endl;
                cout << "3. Post-orden.\n>> ";
                cin >> num;
                if (num == 1) {
                    cout << "Recorrido en Pre-orden: ";
                    arbol.printPreOrder();
                }
                else if (num == 2) {
                    cout << "Recorrido en orden: ";
                    arbol.printInOrder();
                }
                else if (num == 3) {
                    cout << "Recorrido en Post-orden: ";
                    arbol.printPostOrder();
                }
                else {
                    cout << "Opcion inválida. Intenta otra vez." << endl;
                }
                break;
            }
            case 2: {
                eliminate(arbol);
                break;
            }
            case 3: {
                change(arbol);
                break;
            }
            case 4: {
                find(arbol);
                break;
            }
            case 5: {
                add(arbol);
                break;
            }
            case 0: {
                cout << "Saliendo..." << endl;
                return; 
            }
            default: {
                cout << "Opción inválida. Intenta otra vez." << endl;
                break; 
            }
        }
    }
}

int main() {
    Arbol arbol;

    // Insertar numeros en el arbol usando el metodo insert
    arbol.insert(120);
    arbol.insert(87);
    arbol.insert(140);
    arbol.insert(43);
    arbol.insert(99);
    arbol.insert(130);
    arbol.insert(22);
    arbol.insert(65);
    arbol.insert(93);
    arbol.insert(135);
    arbol.insert(56);
    
    menu(arbol);

    arbol.visualize();

    return 0;
}