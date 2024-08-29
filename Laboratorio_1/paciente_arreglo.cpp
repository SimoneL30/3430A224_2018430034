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

int main() {
    
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
        cout << "Nombre: " << persona[i].nombre << endl;
        cout << "Edad: " << persona[i].edad << endl;
        cout << "Altura: " << persona[i].altura << endl;
        cout << "Peso: " << persona[i].peso << endl;
        cout << endl;
    }
    
    return 0;
}