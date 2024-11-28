# Alineamiento de Secuencias de ADN utilizando Needleman-Wunsch

Este proyecto implementa el algoritmo de alineamiento de secuencias de ADN usando el algoritmo de *Needleman-Wunsch* para encontrar la mejor alineación entre dos secuencias de ADN. Además, genera una representación gráfica de la matriz de coincidencias en formato PNG utilizando Graphviz.

## Tabla de Contenidos
1. [Descripción](#descripción)
2. [Instalación](#instalación)
3. [Uso](#uso)
4. [Contribución](#contribución)

## Descripción

Este proyecto realiza una alineamiento global de dos secuencias de ADN, considerando un puntaje de coincidencia/mismatch entre nucleótidos y un puntaje de gap (desajuste). El algoritmo usado es el clásico *Needleman-Wunsch*, que se utiliza comúnmente para alinear secuencias biológicas. El programa también puede generar una visualización gráfica de la matriz de alineación utilizando Graphviz.

## Instalación

### Requisitos previos

1. *Compilador C++* (se recomienda usar g++).
2. *Graphviz* (para generar la visualización de la matriz de alineación).
   
   Para instalar Graphviz, usa el siguiente comando en sistemas basados en Debian/Ubuntu:

   sudo apt-get install graphviz

**Primero, clona el repositorio:**

*git clone https://github.com/usuario/Proyecto_U_III.git*

*cd Proyecto_U_III*

Compila el código fuente con g++:

*g++ Alineamiento.cpp -o alineamiento*

## Uso
El programa se ejecuta desde la línea de comandos con los siguientes parámetros:

*:~$ ./alineamiento -C1 cad1.tex -C2 cad2.tex -U funU.tex -V val*

**Parámetros:**

**-C1 Secuencia 1:** Archivo de texto que contiene la primera secuencia de ADN.

**-C2 Secuencia 2:** Archivo de texto que contiene la segunda secuencia de ADN.

**-U Matriz de emparejamiento:** Archivo de texto que contiene la matriz de puntuación para las coincidencias/mismatches.

**-V Valor de Gap:** Puntaje asignado a los gaps (desajustes entre los nucleótidos).

**Ejemplo:**

Supongamos que tienes dos archivos de secuencias de ADN: secuencia1.txt y secuencia2.txt, y un archivo de matriz de emparejamiento llamado matriz.txt. Si deseas usar un valor de gap de -2, el comando sería el siguiente:

*./alineamiento -C1 secuencia1.txt -C2 secuencia2.txt -U matriz.txt -V -2*

El programa mostrará las secuencias, alineamiento de estas, su puntaje máximo y generará un archivo gráfico de la matriz de coincidencias.

**Ejemplo de salida:**

Secuencia 1: AGCTG

Secuencia 2: AGCT

Alineación:

AGCTG

AGCT-

Puntaje máximo: 15

## Contribución
**Si deseas contribuir a este proyecto, sigue estos pasos:**

Haz un fork del repositorio.

Crea una nueva rama (*git checkout -b nombre_de_la_rama*).

Realiza tus cambios y haz un commit (*git commit -m "Mensaje"*).

Empuja tu rama a tu repositorio (*git push origin nombre_de_la_rama*).

Crea un pull request desde tu rama.
