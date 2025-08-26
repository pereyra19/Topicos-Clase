#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>

#define TAM 10

#define TODO_OK 0
#define LLENO 1
#define DUPLICADO 2


typedef struct
{
    int vec[TAM];
    size_t ce;
}Vector;


bool vectorCrear(Vector* v);
int vectorOrdInsertar(Vector* v, int elem);
int vectorInsertarAlFinal(Vector* v, int elem);
int vectorInsertarAlInicio(Vector* v, int elem);
int vectorInsertarEnPos(Vector* v, int elem, int pos);
int vectorOrdBuscar(const Vector* v, int elem);
int vectorDesordBuscar(const Vector* v, int elem);
bool vectorOrdEliminar(Vector* v, int elem);
bool vectorEliminarDePos(Vector* v, int pos);
void vectorOrdenar(Vector* v);
void vectorVaciar(Vector* v);
void vectorDestruir(Vector* v);



#endif // VECTOR_H
