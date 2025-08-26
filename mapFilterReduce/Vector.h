#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>

#define TAM 10

#define V_TODO_OK 0
#define LLENO 1
#define DUPLICADO 2
#define V_ERR_ARCHIVO 3
#define V_SIN_MEM 4

#define CAP_INI 8
#define FACTOR_INCR 1.5
#define FACTOR_DECR 0.75
#define FACTOR_OCUP 0.5

#define BURBUJEO 1
#define SELECCION 2
#define INSERCION 3
#define QSORT 4


typedef struct
{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}
Vector;


typedef struct
{
    void* act;
    void* ult;
    bool finIter;
    Vector* vector;
}
VectorIterador;


typedef int (*Cmp)(const void* a, const void* b);
typedef void (*Imprimir)(const void* a);
typedef void (*Accion)(void* elem, void* datos);
typedef void (*Reduce)(const void* elem, void* elemReduce);
typedef bool (*Filter)(const void* elem, const void* datosFiltro);
typedef void (*Map)(const void* elem, const void* datosMap, void* elemMapeado);


bool vectorCrear(Vector* v, size_t tamElem);
int vectorCrearDeArchivo(Vector* v, size_t tamElem, const char* nomArch);
bool vectorGuardarEnArchivo(const Vector* v, const char* nomArch);
int vectorOrdInsertar(Vector* v, const void* elem, Cmp cmp);
int vectorInsertarAlFinal(Vector* v, void* elem);
int vectorInsertarAlInicio(Vector* v, int elem);
int vectorInsertarEnPos(Vector* v, int elem, int pos);
int vectorOrdBuscar(const Vector* v, void* elem, Cmp cmp);
int vectorDesordBuscar(const Vector* v, int elem);
bool vectorOrdEliminar(Vector* v, int elem);
bool vectorEliminarDePos(Vector* v, int pos);
void vectorOrdenar(Vector* v, int metodo, Cmp cmp);
void vectorVaciar(Vector* v);
void vectorDestruir(Vector* v);
size_t vectorCE(const Vector* v);
void vectorMostrar(const Vector* v, Imprimir imprimir);
void vectorRecorrer(Vector* v, Accion accion, void* datos);
void vectorReduce(const Vector* v, Reduce reduce, void* reduceElem);
void vectorFilter(const Vector* v, Filter filter, const void* datosFiltro, Vector* vFiltrado);
bool vectorMap(const Vector* v, Map map, const void* datosMap, Vector* vMapeado, size_t tamElemMapeado);


// Iterador
void vectorIteradorCrear(VectorIterador* it, Vector* v);
void* vectorIteradorPrimero(VectorIterador* it);
void* vectorIteradorSiguiente(VectorIterador* it);
bool vectorIteradorFin(VectorIterador* it);


#endif // VECTOR_H
