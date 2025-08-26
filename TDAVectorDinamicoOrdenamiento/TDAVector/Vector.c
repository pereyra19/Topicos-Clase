#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Vector.h"


bool redimensionarVector(Vector* v, float factor);
size_t max(size_t a, size_t b);
void ordenarBurbujeo(Vector* v);
void ordenarSeleccion(Vector* v, Cmp cmp);
void ordenarInsercion(Vector* v);
void intercambiar(void* a, void* b, size_t tamElem);
void* buscarMenor(void* ini, void* fin, Cmp cmp, size_t tamElem);


bool vectorCrear(Vector* v, size_t tamElem)
{
    v->ce = 0;
    v->vec = malloc(CAP_INI * tamElem);

    if(v->vec == NULL)
    {
        v->cap = 0;
        v->tamElem = 0;
        return false;
    }

    v->cap = CAP_INI;
    v->tamElem = tamElem;

    return true;
}


int vectorOrdInsertar(Vector* v, const void* elem, Cmp cmp)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, FACTOR_INCR))
        {
            return LLENO;
        }
    }
    if (v->ce == 0)  // importante para caso de seguridad de Underflow/Overflow de punteros
    {
        memcpy(v->vec, elem, v->tamElem);
        v->ce = 1;
        return TODO_OK;
    }


    void* i = v->vec;
    void* ult = v->vec + (v->ce - 1) * v->tamElem; 

    while(i <= ult && cmp(elem, i) > 0)
    {
        i += v->tamElem;
    }

    if(cmp(elem, i) == 0)
    {
        return DUPLICADO;
    }

    void* posIns = i;

    for(i = ult; i >= posIns; i -= v->tamElem)
    {
       memcpy(i + v->tamElem, i, v->tamElem);
    }

    memcpy(posIns, elem, v->tamElem);

    v->ce++;

    return TODO_OK;
}


int vectorInsertarAlFinal(Vector* v, const void* elem)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, FACTOR_INCR))
        {
            return LLENO;
        }
    }

    void* posIns = v->vec + v->ce * v->tamElem;
    memcpy(posIns, elem, v->tamElem); // memcpy lo trabaja como una cadena
    v->ce++;

    return TODO_OK;
}


int vectorInsertarAlInicio(Vector* v, const int *elem)
{
    if(v->cap == v->ce)
    {
        if(!redimensionarVector(v,FACTOR_INCR))
        {
            return LLENO;
        }
    }

    void *ini = v->vec;
    void *ult = v->vec + ( v->ce - 1 ) * v->tamElem;

    if(v->ce !=0)
    {
        for(void * i = ult ; i >= ini ; i -= v->tamElem )
        {
            memcpy(i + v->tamElem, i , v->tamElem);
        }
    }

    v->ce ++;
    memcpy(ini, elem, v->tamElem);

    return TODO_OK;
}

int vectorInsertarEnPos(Vector* v,const int * elem, int pos, Cmp cmp)
{
    if(pos > v->ce+1) 
    {
        return -1;
    }

    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, FACTOR_INCR))
        {
            return LLENO;
        }
    }

    void *pos = v->vec + (pos - 1) * v->tamElem;
    void *ult = v->vec + (v->ce -1) * v->tamElem;
    
    for(void *i = ult ; i >= pos ; i-= v->tamElem)
    {
        memcpy(i + v->tamElem, i, v->tamElem);
    }

    memcpy(pos, elem, v->tamElem);
    v->ce++;

    return TODO_OK;
}


int vectorOrdBuscar(const Vector* v,const void *elem)
{
    if(v->ce == 0)
        return -1;

    const int* li = v->vec;
    const int* ls = v->vec + (v->ce - 1) * v->tamElem;
    const int* m = li + (ls - li) / 2;

    int comp = elem - *m;

    while(li <= ls && comp != 0)
    {
        if(comp > 0)
        {
            li = m + 1;
        }
        else
        {
            ls = m - 1;
        }

        m = li + (ls - li) / 2;
        comp = elem - *m;
    }

    if(comp == 0)
    {
        return m - v->vec;
    }

    return -1;
}
// no funciona siempre.... para esto usamos una parecia que es segura

int vectorOrdBuscar(Vector * v, void* elem, Cmp cmp)
{
    if(v->ce == 0)
        return -1;

    const void* li = v->vec;
    const void* ls = v->vec + (v->ce - 1) * v->tamElem;
    const void* m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;
    int comp = cmp(elem, m);

    while(li <= ls && comp != 0)
    {
        if(comp > 0)
        {
            li = m + v->tamElem;
        }
        else
        {
            ls = m - v->tamElem;
        }

        m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;
        comp = cmp(elem, m);
    }

    if(comp == 0)
    {
        memcpy(elem, m, v->tamElem);
        return (m - v->vec) / v->tamElem;
    }

    return -1;
}

int buscarVecOrd_ALU(Vector *v, void *elem, Cmp cmp)
{
    if (v->ce == 0)
        return -1;

    char *base = (char*)v->vec;
    int li = 0;
    int ls = v->ce - 1;
    int mid = (li + ls) / 2;
    void *m = base + mid * v->tamElem;
    int comp = cmp(elem, m);

    while (li <= ls && comp != 0)
    {
        if (comp < 0)
            ls = mid - 1;

        if(comp>0)
			li = mid + 1;

        mid = (li + ls) / 2;
        m   = base + mid * v->tamElem;
        comp = cmp(elem, m);
    }

    if (comp == 0)
    {
        memcpy(elem, m, v->tamElem);
        return 1;
    }

    return -1;
}



// int vectorDesordBuscar(const Vector* v, int elem);


// bool vectorOrdEliminar(Vector* v, int elem)
// {
//     int pos = vectorOrdBuscar(v, elem);

//     if(pos == -1)
//     {
//         return false;
//     }

//     vectorEliminarDePos(v, pos);

//     return true;
// }


// bool vectorEliminarDePos(Vector* v, int pos)
// {
//     if(pos < 0)
//     {
//         return false;
//     }

//     int* i = v->vec + pos;
//     int* ult = v->vec + v->ce - 1;

//     if(i > ult)
//     {
//         return false;
//     }

//     for(; i < ult; i++)
//     {
//         *i = *(i + 1);
//     }

//     v->ce--;

//     if((float)v->ce / v->cap <= FACTOR_OCUP)
//     {
//         redimensionarVector(v, FACTOR_DECR);
//     }

//     return true;
// }


void vectorOrdenar(Vector* v, int metodo, Cmp cmp)
{
    switch(metodo)
    {
    case BURBUJEO:
        // ordenarBurbujeo(v);
        break;

    case SELECCION:
        ordenarSeleccion(v, cmp);
        break;

    case INSERCION:
        // ordenarInsercion(v);
        break;
    
    case QSORT:
        qsort(v->vec, v->ce, v->tamElem, cmp);
        break;
    }
}


// void ordenarBurbujeo(Vector* v)
// {
//     int* ult = v->vec + v->ce - 1;

//     for(int i = 1, *limJ = ult - 1; i < v->ce; i++, limJ--)
//     {
//         for(int* j = v->vec; j <= limJ; j++)
//         {
//             if(*j > *(j + 1))
//             {
//                 intercambiar(j, j + 1);
//             }
//         }
//     }
// }


void ordenarSeleccion(Vector* v, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* m;

    for(void* i = v->vec; i < ult; i += v->tamElem)
    {
        m = buscarMenor(i, ult, cmp, v->tamElem);
        intercambiar(m, i, v->tamElem);
    }
}


void* buscarMenor(void* ini, void* fin, Cmp cmp, size_t tamElem)
{
    void* m = ini;

    for(void* j = ini + tamElem; j <= fin; j += tamElem)
    {
        if(cmp(m,j) < 0)
        {
            m = j;
        }
    }

    return m;
}


// void ordenarInsercion(Vector* v)
// {
//     int* ult = v->vec + v->ce - 1;
//     int* j;
//     int elemAIns;

//     for(int* i = v->vec + 1; i <= ult; i++)
//     {
//         elemAIns = *i;

//         j = i - 1;
//         while(j >= v->vec && elemAIns < *j)
//         {
//             *(j + 1) = *j;
//             j--;
//         }

//         *(j + 1) = elemAIns;
//     }
// }


void intercambiar(void* a, void* b, size_t tamElem)
{
    void* aTemp = malloc(tamElem);
    memcpy(aTemp, a, tamElem);
    memcpy(a, b, tamElem);
    memcpy(b, aTemp, tamElem);
    free(aTemp);
}


void vectorVaciar(Vector* v)
{
    v->ce = 0;
    redimensionarVector(v, 0);
}


void vectorDestruir(Vector* v)
{
    v->ce = 0;
    free(v->vec);
    v->vec = NULL;
    v->cap = 0;
    v->tamElem = 0;
}


size_t vectorCE(const Vector* v)
{
    return v->ce;
}


bool redimensionarVector(Vector* v, float factor)
{
    size_t nuevaCap = max(v->cap * factor, CAP_INI);
    void* nuevoVec = realloc(v->vec, nuevaCap * v->tamElem);

    if(!nuevoVec) // nuevoVec == NULL
    {
        return false;
    }

    if(v->cap != nuevaCap)
    {
        printf("Redimensión del vector de %lu a %lu.\n", v->cap, nuevaCap);
    }

    v->cap = nuevaCap;
    v->vec = nuevoVec;

    return true;
}


size_t max(size_t a, size_t b)
{
    return a > b ? a : b;
}


void vectorMostrar(const Vector* v, Imprimir imprimir)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        imprimir(i);
    }

    putchar('\n');
}


