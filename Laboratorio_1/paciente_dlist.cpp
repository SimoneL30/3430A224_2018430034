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
//  Funcion para eliminar pacientes de la lista
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
//  Funcion para mostrar los datos de la lista
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
int main() {
//Se crea la lista
    Paciente* cabeza = nullptr;
//  Se crea el arreglo
    Paciente persona[3];
//  Se almacenan los datos
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
    
//  Se recorre el arreglo para mostras la informacion
    for (int i = 0; i < 3; ++i) {
        agregarPaciente(cabeza, persona[i].nombre, persona[i].edad, persona[i].altura, persona[i].peso);
    }
    eliminarPaciente(cabeza, "Jessica");
    imprimirPaciente(cabeza);
    return 0;
}
    