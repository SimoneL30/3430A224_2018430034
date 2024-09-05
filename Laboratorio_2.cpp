#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Paciente {
    string nombre;
    int edad;
    double altura;
    double peso;
    double a1c;
    double imc;
    double aimc;
    Paciente* next;
};

// Funcion agregar los pacientes con prioridad
void agregarCabezaPrioridad(Paciente*& cabezaPrioridad, Paciente* newPaciente) {
    if (cabezaPrioridad == nullptr || (newPaciente->aimc > cabezaPrioridad->aimc)) {
        newPaciente->next = cabezaPrioridad;
        cabezaPrioridad = newPaciente;
    } 
    else {
        Paciente* actual = cabezaPrioridad;
        while (actual->next != nullptr && (actual->next->aimc >= newPaciente->aimc)) {
            actual = actual->next;
        }
        newPaciente->next = actual->next;
        actual->next = newPaciente;
    }
}

//Funcion para agregar los pacientes
void agregarPaciente(Paciente*& cabeza, Paciente*& cabezaPrioridad, const string& nombre, int edad, double altura, double peso, double a1c) {
    Paciente* newPaciente = new Paciente();
    newPaciente->nombre = nombre;
    newPaciente->edad = edad;
    newPaciente->altura = altura;
    newPaciente->peso = peso;
    newPaciente->a1c = a1c;
    
    if (altura > 0) {
        newPaciente->imc = peso / (altura * altura);
    } 
    else {
        newPaciente->imc = 0;
    }

    if (a1c < 5.7) {
        newPaciente->aimc = 0;
    } 
    else if (a1c >= 5.6 && a1c < 6.5) {
        newPaciente->aimc = newPaciente->imc;
    } 
    else {
        newPaciente->aimc = newPaciente->imc * 2;
    }

    newPaciente->next = cabeza;
    cabeza = newPaciente;

    agregarCabezaPrioridad(cabezaPrioridad, newPaciente);
}

void eliminarPaciente(Paciente*& cabeza, const string& nombre) {
    Paciente* actual = cabeza;
    Paciente* previous = nullptr;

    while (actual != nullptr && actual->nombre != nombre) {
        previous = actual;
        actual = actual->next;
    }

    if (actual != nullptr) {
        if (previous == nullptr) {
            cabeza = actual->next;
        } else {
            previous->next = actual->next;
        }
        delete actual;
    }
}

void imprimirPaciente(Paciente* cabeza) {
    const Paciente* actual = cabeza;
    while (actual != nullptr) {
        cout << "Nombre: " << actual->nombre << endl;
        cout << "Edad: " << actual->edad << endl;
        cout << "Altura: " << actual->altura << endl;
        cout << "Peso: " << actual->peso << endl;
        cout << "A1C: " << actual->a1c << endl;
        cout << "IMC: " << actual->imc << endl; 
        actual = actual->next;
    }
}

void promedioEdad(Paciente* cabeza) {
    double contador = 0;
    double contadorpacientes = 0;
    double promedioDeEdad = 0;
    const Paciente* actual = cabeza;
    while (actual != nullptr) {
        contadorpacientes += 1;
        contador += actual->edad;
        actual = actual->next;
    }
    if (contadorpacientes > 0) {
        promedioDeEdad = contador / contadorpacientes;
        cout << "La edad promedio es " << promedioDeEdad << endl;
    } 
    else {
        cout << "No hay pacientes para calcular el promedio de edad." << endl;
    }
}

void leerArchivo(const string& nombreDelArchivo, Paciente*& cabeza, Paciente*& cabezaPrioridad) {
    ifstream archivo(nombreDelArchivo);
    if (!archivo) {
        cerr << "Error al cargar el archivo: " << nombreDelArchivo << endl;
        return;
    }
    string fila;
    getline(archivo, fila);
    while (getline(archivo, fila)) {
        istringstream iss(fila);
        string nombre, edad, altura, peso, a1c;

        getline(iss, nombre, ',');
        getline(iss, edad, ',');
        getline(iss, altura, ',');
        getline(iss, peso, ',');
        getline(iss, a1c);

        agregarPaciente(cabeza, cabezaPrioridad, nombre, stoi(edad), stod(altura), stod(peso), stod(a1c));
    }
}

void crearArchivo(Paciente* cabezaPrioridad) {
    ofstream archivo("archivo.csv");

    if (!archivo) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    archivo << "Nombre,Edad,Altura,Peso,A1C,IMC\n"; 

    const Paciente* actual = cabezaPrioridad;
    while (actual != nullptr) {
        archivo << actual->nombre << ','
                << actual->edad << ','
                << actual->altura << ','
                << actual->peso << ','
                << actual->a1c << ','
                << actual->imc << '\n';
        actual = actual->next;
    }

    archivo.close();
    cout << "Se estan guardando los datos en el archivo.csv" << endl;
}

int tamanoCola(Paciente*& cabezaPrioridad) {
    int contador = 0;
    Paciente* actual = cabezaPrioridad;
    
    while (actual != nullptr) {
        contador++;
        actual = actual->next;
    }

    return contador;
}
void buscarPorImc(Paciente* cabezaPrioridad, double imcIngresado){
    Paciente* actual = cabezaPrioridad;
    bool encontrado = false;
    
    while (actual != nullptr) {
        if (actual->imc == imcIngresado) {
            cout << "Paciente encontrado: " << actual->nombre << endl;
            cout << "IMC: " << actual->imc << endl;
            encontrado = true;
        }
        actual = actual->next;
    }
    
    if (!encontrado){
        cout << "No se encontraron pacientes con el IMC ingresado." << endl;
    }
}

void buscarPorA1C(Paciente* cabezaPrioridad, double a1cIngresado){
    Paciente* actual = cabezaPrioridad;
    bool encontrado = false;
    
    while (actual != nullptr) {
        if (actual->a1c == a1cIngresado) {
            cout << "Paciente encontrado: " << actual->nombre << endl;
            cout << "A1C: " << actual->a1c << endl;
            encontrado = true;
        }
        actual = actual->next;
    }
    
    if (!encontrado){
        cout << "No se encontraron pacientes con el IMC ingresado." << endl;
    }
}

int menu(Paciente*& cabeza, Paciente*& cabezaPrioridad) {
    while (true) {
        int opcion;
        cout << "Ingrese una opcion" << endl;
        cout << "1. Agregar paciente." << endl;
        cout << "2. Eliminar paciente." << endl;
        cout << "3. Mostrar todos los pacientes y su informacion." << endl;
        cout << "4. Crear archivo con los datos de los pacientes." << endl;
        cout << "5. Leer datos desde archivo CSV." << endl;
        cout << "6. Mostrar pacientes con prioridad." << endl;
        cout << "7. Mostrar el tamaño de la cola." << endl;
        cout << "8. Buscar por IMC o A1C." << endl;
        cout << "9. Salir." << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombre;
                int edad;
                double altura;
                double peso;
                double a1c;

                cout << "Ingresa el nombre: ";
                cin >> nombre;
                cout << "Ingresa la edad: ";
                cin >> edad;
                cout << "Ingresa la altura: ";
                cin >> altura;
                cout << "Ingresa el peso: ";
                cin >> peso;
                cout << "Ingresa el A1C: ";
                cin >> a1c;
                agregarPaciente(cabeza, cabezaPrioridad, nombre, edad, altura, peso, a1c);
                break;
            }
            case 2: {
                string nombre;
                cout << "Ingresa el nombre del paciente que eliminaras: ";
                cin >> nombre;
                eliminarPaciente(cabeza, nombre);
                break;
            }
            case 3: {
                imprimirPaciente(cabeza);
                promedioEdad(cabeza);
                break;
            }
            case 4: {
                crearArchivo(cabezaPrioridad);
                break;
            }
            case 5: {
                cout << "Leyendo el archivo csv con pacientes." << endl;
                leerArchivo("patient_list.csv", cabeza, cabezaPrioridad);
                break;
            }
            case 6: {
                cout << "Mostrando pacientes segun prioridad:" << endl;
                imprimirPaciente(cabezaPrioridad);
                break;
            }
            case 7: {
                cout << "El tamaño de la cola es: " << tamanoCola(cabezaPrioridad) << endl;
                break;
            }
              case 8: {
                double imcIngresado;
                double a1cIngresado;
                int eleccion;
                cout << "¿Por que categoria quieres buscar?" << endl;
                cout << "1. IMC. \n2. A1C\nIngresa tu opcion: ";
                cin >> eleccion;
                if(eleccion == 1){
                    cout << "Ingresa el IMC que deseas buscar: ";
                    cin >> imcIngresado;
                    buscarPorImc(cabezaPrioridad, imcIngresado);
                }
                else if(eleccion == 2){
                    cout << "Ingresa el A1C que deseas buscar:";
                    cin >> a1cIngresado;
                    buscarPorA1C(cabezaPrioridad, a1cIngresado);
                }
                else{
                    cout << "Opcion invalida. Intenta otra vez." << endl;
                }
                break;
            }
            case 9: {
                while (cabeza != nullptr) {
                    Paciente* temp = cabeza;
                    cabeza = cabeza->next;
                    delete temp;
                }
                while (cabezaPrioridad != nullptr) {
                    Paciente* temp = cabezaPrioridad;
                    cabezaPrioridad = cabezaPrioridad->next;
                    delete temp;
                }
                return 0;
            }
            default:
                cout << "Opcion invalida. Intenta otra vez.\n";
        }
    }
}

int main() {
    Paciente* cabeza = nullptr;
    Paciente* cabezaPrioridad = nullptr;

    agregarPaciente(cabeza, cabezaPrioridad, "Charlotte", 1, 1.10, 11.200, 6.5);
    agregarPaciente(cabeza, cabezaPrioridad, "Agatha", 8, 1.40, 40.500, 5.0);
    agregarPaciente(cabeza, cabezaPrioridad, "Jessica", 50, 1.65, 90.400, 5.7);

    menu(cabeza, cabezaPrioridad);
    return 0;
}