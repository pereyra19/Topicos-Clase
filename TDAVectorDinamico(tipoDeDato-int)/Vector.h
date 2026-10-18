#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>

#define TAM 10

#define TODO_OK 0
#define LLENO 1
#define DUPLICADO 2

#define CAP_INI 8
#define FACTOR_INCR 1.5
#define FACTOR_DECR 0.75
#define FACTOR_OCUP 0.5

#define BURBUJEO 1
#define SELECCION 2
#define INSERCION 3


typedef struct
{
    int* vec;
    size_t ce;
    size_t cap;
}
Vector;


bool vectorCrear(Vector* v);
int vectorOrdInsertar(Vector* v, int elem);
int vectorInsertarAlFinal(Vector* v, int elem);
int vectorInsertarAlInicio(Vector* v, int elem);
int vectorInsertarEnPos(Vector* v, int elem, int pos);
int vectorOrdBuscar(const Vector* v, int elem);
int *vectorDesordBuscar(const Vector* v, int elem);
bool vectorOrdEliminar(Vector* v, int elem);
bool vectorEliminarDePos(Vector* v, int pos);
void vectorOrdenar(Vector* v, int metodo);
void vectorVaciar(Vector* v);
void vectorDestruir(Vector* v);
size_t vectorCE(const Vector* v);
void vectorMostrar(const Vector* v);


#endif // VECTOR_H
