#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../mapFilterReduce/Vector.h"

typedef struct
{
    int d;
    int m;
    int a;
}
Fecha;


int cmpFechas(const void* a, const void* b);
void imprimirFecha(const void* a);
void imprimirCadena(const void* a);
void filtrarFecha(void* elem, void* datos);
void fechaACad(const void* elem, const void* datosMap, void* elemMapeado);


int main()
{
    // Set the code page to support Spanish characters
//    system("chcp 65001 > nul");

    Vector miVec;

    vectorCrear(&miVec, sizeof(Fecha));

    srand(time(NULL));
    Fecha f;

    for(int i = 1; i <= 10; i++)
    {
        f.d = 1 + rand() % 28;
        f.m = 1 + rand() % 12;
        f.a = 1900 + rand() % 130;
        vectorInsertarAlFinal(&miVec, &f);
    }

    puts("Antes:");
    vectorMostrar(&miVec, imprimirFecha);

    time_t tIni = time(NULL);
    vectorOrdenar(&miVec, QSORT, cmpFechas);
    time_t tFin = time(NULL);

    time_t tiempoEjecucion = tFin - tIni;

    printf("Tiempo de ejecución: %lld\n", tiempoEjecucion);

    puts("\nDespués:");
    vectorMostrar(&miVec, imprimirFecha);

    Fecha fFiltro = {1, 1, 1970};

    puts("Fechas filtradas:");
    vectorRecorrer(&miVec, filtrarFecha, &fFiltro);

    puts("Con iterador");
    VectorIterador it;
    vectorIteradorCrear(&it, &miVec);

    Fecha* fAct = vectorIteradorPrimero(&it);
    while(!vectorIteradorFin(&it))
    {
        if((fAct->a * 10000 + fAct->m * 100 + fAct->d - (fFiltro.a * 10000 + fFiltro.m * 100 + fFiltro.d)) > 0)
        {
            printf("%02d/%02d/%04d\n", fAct->d, fAct->m, fAct->a);
        }

        fAct = vectorIteradorSiguiente(&it);
    }

    Vector vFechasCad;
    vectorMap(&miVec, fechaACad, NULL, &vFechasCad, 11);

    puts("Fechas Mapeadas a string:");
    vectorMostrar(&vFechasCad, imprimirCadena);


    vectorDestruir(&miVec);
    vectorDestruir(&vFechasCad);

    return 0;
}


int cmpFechas(const void* a, const void* b)
{
    const Fecha* f1 = a;
    const Fecha* f2 = b;
    return (f1->a * 10000 + f1->m * 100 + f1->d) - (f2->a * 10000 + f2->m * 100 + f2->d);
}


void imprimirFecha(const void* a)
{
    const Fecha* f = a;
    printf("%02d/%02d/%04d\n", f->d, f->m, f->a);
}


void filtrarFecha(void* elem, void* datos)
{
    Fecha* fElem = elem;
    Fecha* fFiltro = datos;

    if((fElem->a * 10000 + fElem->m * 100 + fElem->d - (fFiltro->a * 10000 + fFiltro->m * 100 + fFiltro->d)) > 0)
    {
        printf("%d/%d/%d\n", fElem->d, fElem->m, fElem->a);
    }
}


void fechaACad(const void* elem, const void* datosMap, void* elemMapeado)
{
    const Fecha* fecha = elem;
    char* fechaCad = elemMapeado;
    sprintf(fechaCad, "%d/%d/%d", fecha->d, fecha->m, fecha->a);
}


void imprimirCadena(const void* a)
{
    const char* cad = a;
    puts(cad);
}
