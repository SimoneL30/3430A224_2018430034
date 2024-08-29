#include <iostream>
#include <fstream>
using namespace std;
//  Se crea una estructura 
struct Paciente {
    string nombre;
    int edad;
    double altura;
    double peso;
    Paciente* next;
};
//  Funcion para añadir pacientes a la lista
void agregarPaciente(Paciente*& cabeza, const string& nombre, int edad, double altura, double peso) {
    Paciente* newPaciente = new Paciente();
    newPaciente->nombre = nombre;
    newPaciente->edad = edad;
    newPaciente->altura = altura;
    newPaciente->peso = peso;
    newPaciente->next = cabeza;
    cabeza = newPaciente;
}
//  Funcion para eliminar pacientes
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
//  Funcion para mostrar los datos
void imprimirPaciente(const Paciente* cabeza) {
    const Paciente* actual = cabeza;
    while (actual != nullptr) {
        cout << "Nombre: " << actual->nombre << endl;
        cout << "Edad: " << actual->edad << endl;
        cout << "Altura: " << actual->altura << endl;
        cout << "Peso: " << actual->peso << endl;
        actual = actual->next;
    }
}
// Funcion para calcular el promedio de la edad 
void promedioEdad(const Paciente* cabeza) {
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
    } else {
        cout << "No hay pacientes para calcular el promedio de edad." << endl;
    }
}
// Funcion para calcular el IMC de cada paciente
void indiceMasa(const Paciente* cabeza) {
    double imc = 0;
    const Paciente* actual = cabeza;
    while (actual != nullptr) {
        if (actual->altura > 0) {  // Evitar división por cero
            imc = actual->peso / (actual->altura * actual->altura);
            cout << "El IMC de " << actual->nombre << " es: " << imc << endl;
        } else {
            cout << "Altura de " << actual->nombre << " no puede ser cero para calcular el IMC." << endl;
        }
        actual = actual->next;
    }
}
//  Funcion para crear el archivo de texto con los datos
void crearArchivo(const Paciente* cabeza) {
    ofstream archivo("archivo.csv");

    if (!archivo) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    archivo << "Nombre,Edad,Altura,Peso\n"; // Encabezado del archivo CSV

    const Paciente* actual = cabeza;
    while (actual != nullptr) {
        archivo << actual->nombre << ','
                << actual->edad << ','
                << actual->altura << ','
                << actual->peso << '\n';
        actual = actual->next;
    }

    archivo.close();
    cout << "Se estan guardando los datos en el archivo.csv" << endl;
}
//  Funcion para crear el menu 
int menu(Paciente*& cabeza) {
    while (true) {
        int opcion;
        cout << "Ingrese una opcion" << endl;
        cout << "1. Agregar paciente." << endl;
        cout << "2. Eliminar paciente." << endl;
        cout << "3. Mostrar todos los pacientes y su informacion." << endl;
        cout << "4. Crear archivo con los datos de los pacientes." << endl;
        cout << "5. Salir." << endl;
        cin >> opcion;

        switch (opcion) {
//  Dependiendo del caso se llama a la funcion requerida y se le pasa los datos ingresados
        case 1: {
            string nombre;
            int edad;
            double altura;
            double peso;

            cout << "Ingresa el nombre: ";
            cin >> nombre;
            cout << "Ingresa la edad: ";
            cin >> edad;
            cout << "Ingresa la altura: ";
            cin >> altura;
            cout << "Ingresa el peso: ";
            cin >> peso;

            agregarPaciente(cabeza, nombre, edad, altura, peso);
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
            indiceMasa(cabeza);
            break;
        }
        case 4: {
            crearArchivo(cabeza);
            break;
        }
        
        case 5: {
            while (cabeza != nullptr) {
                Paciente* temp = cabeza;
                cabeza = cabeza->next;
                delete temp;
            }
            return 0;
        }
//  En caso de no poner una opcion valida, se muestra este mensaje
        default:
            cout << "Opcion invalida. Intenta otra vez.\n";
        }
    }
}

int main() {
    Paciente* cabeza = nullptr;

//  Se agregan pacientes iniciales
    Paciente persona[3];
//  Almacenando datos en el arreglo
    persona[0].nombre = "Charlotte";
    persona[0].edad = 1;
    persona[0].altura = 1.10;
    persona[0].peso = 11.200;

    persona[1].nombre = "Agatha";
    persona[1].edad = 8;
    persona[1].altura = 1.40;
    persona[1].peso = 40.500;

    persona[2].nombre = "Jessica";
    persona[2].edad = 50;
    persona[2].altura = 1.65;
    persona[2].peso = 90.400;
    
    for (int i = 0; i < 3; ++i) {
        agregarPaciente(cabeza, persona[i].nombre, persona[i].edad, persona[i].altura, persona[i].peso);
    }
    
    menu(cabeza);
    return 0;
}