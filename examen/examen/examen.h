#ifndef EXAMEN_H
#define EXAMEN_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>


#define CANTNOMBRE 12
#define CANTNOMBREAUX 100
#define esLetra(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z'))
// Macro para convertir un solo carácter ASCII a mayúscula      (   32   )
#define A_MAYUS(c) ( ((c) >= 'a' && (c) <= 'z') ? (char)((c) - ('a' - 'A')) : (c) )
#define esNumero(x) ((x)>=1 && (x)<=9)


typedef struct
{
    void *act;
    void *ult;
    vector *vector;
    bool finIter;
}
vectorIterador;

// TDA VECTOR 
typedef struct
{
    void *vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}
vector;


#define TAMCODIGO 4
#define TAMNOMBRE 12

//convertirArch
// typedef struct
// {
//     int codigo;
//     char nombre[40];
// }
// producto;

// merse
typedef struct
{
    int codigo;
    char nombre[40];
    int stock;
}
producto;

typedef struct
{
    int codigo;
    char nombre[40];
    int stock;
    char descripcion;
}
movimiento ;

typedef int(*Cmp)(const void *a, const void *b);
typedef void(*BINaTXT)(const void *reg, FILE *arch);
typedef int(*TXTaBIN)(void *reg, char *linea);
typedef void (*Reduce)(const void* elem, void* elemReduce);
typedef bool (*Filter)(const void* elem, const void* datosFiltro);
typedef void (*Map)(const void* elem, const void* datosMap, void* elemMapeado);

#define CANTLINEA 100
#define CAPINI 5
#define ERR_TAM_LINEA -3

#define FACTOR_INCR 1.3
#define FACTOR_DECR 0.75
#define FACTOR_OCUP 1
#define ERR_MEMORIA -1 
#define TODO_OK 0
#define DUPLICADO 1
#define ERR_ARCH -2

// TDA VECTOR 
bool crearVector(vector *v, size_t tamElem, size_t capIni);
void destruirVec(vector *v);
int insertarAlPrincipio(vector *v, void *elem);
int insertarAlFinal(vector *v, void *elem);
int insertarEnPos(vector *v, void *elem, int pos);
int insertarVecOrd(vector *v, void *elem, Cmp cmp);
bool redimensionarVector(vector* v, float factor);
size_t max(size_t a, size_t b);
bool esEliminarElem(vector *v, void *elem);
int eliminarElemVecOrd(vector *v, void *pos);
void intercambiarProducto(void *a , void *b);
int vectorOrdBuscar(const vector* v, void* elem, Cmp cmp);
// Ordenamiento
void ordenarBurbujeo(vector* v, Cmp cmp);
void ordenarSeleccion(vector *v, Cmp cmp);
void *buscarMenor(vector *v, void *pos , Cmp cmp);
void *buscarElemVecOrd(vector *v, void *elem, Cmp cmp);
int cmpProducto(const void *a, const void *b);
//Archivo Binario
int vectorCrearDeArchivo(vector* v, size_t tamElem, const char* nomArch);
int convertirBINaTXT (const char *nomArchBin, const char *nomArchTxt, size_t tamReg, BINaTXT binaTxt);
void binaTxtV(const void *reg, FILE *archTxt);
void binaTxtF(const void *reg, FILE *archTxt);
int productoTxtaBinV(void *reg, char *linea);
// Iterador
void vectorIteradorCrear(vectorIterador *it, vector *v);
void *vectorIteradorPrimero(vectorIterador * it); // inicia la recorrida en caso de ser necesaria 
void *vectorIteradorSiguiente(vectorIterador * it); 
bool vectorIteradorFin(vectorIterador *it);
// podria haber un Iteradorfinal y un IteradorAnterior si queremos recorrer en sentido inverso
// map - filter - reduce 
void vectorReduce(const vector* v, Reduce reduce, void* reduceElem);
void vectorFilter(const vector* v, Filter filter, const void* datosFiltro, vector* vFiltrado);
bool vectorMap(const vector* v, Map map, const void* datosMap, vector* vMapeado, size_t tamElemMapeado);





#endif // EXAMEN_H