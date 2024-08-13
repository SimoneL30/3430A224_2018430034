#include <iostream>
#include <string>
using namespace std;


void longitud(string palabra)
{ 
    cout << "\nLa longitud de la palabra " << palabra << " es: " << palabra.length();
}

void par(int numero)
{
    if (numero%2 == 0){
    cout << "\nEl numero " << numero << " es par";
    }
    else{
    cout << "\nEl numero " << numero << " es impar";
    }
}

int main()
{
    int numero;
    int i=1;
    string palabra;
    string palabra2 = "Charlotte";
    cout << "Ingresa un numero: "; cin>> numero;
    cout << "El numero ingresado es: " << numero;
    cout << "\nIngresa un string: "; cin>> palabra;
    par(numero); 
    
    for (i=1; i<=10; i++){
        par(i);
    }
    longitud(palabra);
    longitud(palabra2);
    
    
    return 0;
}