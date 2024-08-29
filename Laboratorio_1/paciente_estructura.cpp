#include <iostream>
using namespace std;
//  Se crea una estructura 
struct Paciente {
    string nombre;
    int edad;
    double altura;
    double peso;
    Paciente* next;
};

int main() {
    Paciente* cabeza = nullptr;

    cout << "Nombre: Charlotte\nEdad: 1\nAltura: 1.10\nPeso: 11.200" << endl;
    cout << "Nombre: Agatha\nEdad: 8\nAltura: 1.40\nPeso: 40.500" << endl;
    cout << "Nombre: Jessica\nEdad: 50\nAltura: 1.65\nPeso: 90.400" << endl;
    

    return 0;
}