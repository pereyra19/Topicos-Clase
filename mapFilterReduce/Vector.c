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


int vectorCrearDeArchivo(Vector* v, size_t tamElem, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "rb");

    if(!arch)
    {
        return V_ERR_ARCHIVO;
    }

    fseek(arch, 0L, SEEK_END);
    size_t cantBytes = ftell(arch);
    size_t cantRegs = cantBytes / tamElem;

    v->vec = malloc(cantBytes);

    if(!v->vec)
    {
        return V_SIN_MEM;
    }

    rewind(arch);

    fread(v->vec, tamElem, cantRegs, arch);
    v->ce = cantRegs;
    v->cap = cantRegs;
    v->tamElem = tamElem;

    fclose(arch);

    return V_TODO_OK;
}


bool vectorGuardarEnArchivo(const Vector* v, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "wb");

    if(!arch)
    {
        return false;
    }

    fwrite(v->vec, v->tamElem, v->ce, arch);

    fclose(arch);

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

    void* i = v->vec;
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    while(i <= ult && cmp(elem, i) > 0)
    {
        i += v->tamElem;
    }

    if(i <= ult && cmp(elem, i) == 0)
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

    return V_TODO_OK;
}


int vectorInsertarAlFinal(Vector* v, void* elem)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, FACTOR_INCR))
        {
            return LLENO;
        }
    }

    void* posIns = v->vec + v->ce * v->tamElem;
    memcpy(posIns, elem, v->tamElem);
    v->ce++;

    return V_TODO_OK;
}


// int vectorInsertarAlInicio(Vector* v, int elem);
// int vectorInsertarEnPos(Vector* v, int elem, int pos);


int vectorOrdBuscar(const Vector* v, void* elem, Cmp cmp)
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
        //return (m - v->vec) / v->tamElem; duduso 
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
        if(cmp(j ,m) < 0)
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

    // if(v->cap != nuevaCap)
    // {
    //     printf("Redimensión del vector de %lu a %lu.\n", v->cap, nuevaCap);
    // }

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


void vectorRecorrer(Vector* v, Accion accion, void* datos)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        accion(i, datos);
    }
}


void vectorReduce(const Vector* v, Reduce reduce, void* reduceElem)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        reduce(i, reduceElem);
    }    
}


void vectorFilter(const Vector* v, Filter filter, const void* datosFiltro, Vector* vFiltrado)
{
    vectorCrear(vFiltrado, v->tamElem);
    
    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        if(filter(i, datosFiltro))
        {
            vectorInsertarAlFinal(vFiltrado, i);
        }
    }      
}


bool vectorMap(const Vector* v, Map map, const void* datosMap, Vector* vMapeado, size_t tamElemMapeado)
{
    vectorCrear(vMapeado, tamElemMapeado);
    
    void* elemMapeado = malloc(tamElemMapeado);

    if(!elemMapeado)
    {
        return false;
    }

    void* ult = v->vec + (v->ce - 1) * v->tamElem;

    for(void* i = v->vec; i <= ult; i += v->tamElem)
    {
        map(i, datosMap, elemMapeado);
        vectorInsertarAlFinal(vMapeado, elemMapeado);
    }
    
    free(elemMapeado);

    return true;
}


// Iterador

void vectorIteradorCrear(VectorIterador* it, Vector* v)
{
    it->act = NULL;
    it->ult = NULL;
    it->finIter = true;
    it->vector = v;
}


void* vectorIteradorPrimero(VectorIterador* it)
{
    Vector* v = it->vector;

    if(v->ce == 0)
    {
        it->finIter = true;
        return NULL;
    }

    it->act = v->vec;
    it->ult = v->vec + (v->ce - 1) * v->tamElem;
    it->finIter = false;
    return it->act;
}


void* vectorIteradorSiguiente(VectorIterador* it)
{
    void* sig = it->act + it->vector->tamElem;

    if(sig > it->ult)
    {
        it->finIter = true;
        return NULL;
    }

    it->act = sig;

    return sig;
}


bool vectorIteradorFin(VectorIterador* it)
{
    return it->finIter;
}
