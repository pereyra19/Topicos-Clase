#ifndef EXAMEN_H_INCLUDED
#define EXAMEN_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>


#define CANTNOMBRE 12
#define CANTNOMBREAUX 100
#define esLetra(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z'))
#define esNumero(x) ((x)>=1 && (x)<=9)





// TDA VECTOR 
typedef struct
{
    void *vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}
vector;


typedef struct
{
    int codigo;
    char nombre[40];
}
producto;


typedef int(*Cmp)(const void *a, const void *b);


#define CAPINI 5
#define FACTOR_INCR 1.3
#define FACTOR_DECR 0.75
#define FACTOR_OCUP 1
#define ERR_MEMORIA -1 
#define TODO_OK 0
#define DUPLICADO 1


bool crearVector(vector *v, size_t tamElem, size_t capIni);
int insertarAlPrincipio(vector *v, void *elem);
int insertarAlPrincipio(vector *v, void *elem);
int insertarAlFinal(vector *v, void *elem);
int insertarEnPos(vector *v, void *elem, int pos);
int insertarVecOrd(vector *v, void *elem, Cmp cmp);
bool redimensionarVector(vector* v, float factor);
size_t max(size_t a, size_t b);
bool esEliminarElem(vector *v, void *elem);
int eliminarElemVecOrd(vector *v, void *pos);
void destruirVec(vector *v);



#endif // EXAMEN_H_INCLUDED
