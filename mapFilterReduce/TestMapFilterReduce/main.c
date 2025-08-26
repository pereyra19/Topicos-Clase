#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../mapFilterReduce/vector.h"


void concat(const void* elem, void* elemReduce);
bool filtroCad(const void* elem, const void* datosFiltro);
void mostrarCad(const void* a);


int main()
{
    Vector miVec;
    vectorCrear(&miVec, 11);

    vectorInsertarAlFinal(&miVec, "Hola");
    vectorInsertarAlFinal(&miVec, "que");
    vectorInsertarAlFinal(&miVec, "tal?");

    char saludo[51];
    saludo[0] = '\0';

    vectorReduce(&miVec, concat, saludo);

    puts(saludo);

    Vector vFiltrado;
    vectorFilter(&miVec, filtroCad, "Isla", &vFiltrado);

    puts("Vector filtrado:");
    vectorMostrar(&vFiltrado, mostrarCad);

    vectorDestruir(&miVec);
    vectorDestruir(&vFiltrado);

    return 0;
}


void concat(const void* elem, void* elemReduce)
{
    const char* subCad = elem;
    char* cadena = elemReduce;
    strcat(cadena, " ");
    strcat(cadena, subCad);
}


bool filtroCad(const void* elem, const void* datosFiltro)
{
    const char* cadElem = elem;
    const char* cadFiltro = datosFiltro;
    return strcmpi(cadElem, cadFiltro) >= 0;
}


void mostrarCad(const void* a)
{
    const char* cad = a;
    puts(cad);
}
