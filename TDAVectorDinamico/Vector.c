#include <stdlib.h>
#include "Vector.h"
#include <time.h>
#include <stdio.h>


bool redimensionarVector(Vector* v, float factor);
size_t max(size_t a, size_t b);
void ordenarBurbujeo(Vector* v);
void ordenarSeleccion(Vector* v);
void ordenarInsercion(Vector* v);
void intercambiar(int* a, int* b);


int main()
{
    system("chcp 65001 > nul");

    Vector mivec;

    vectorCrear(&mivec);

    srand(time(NULL));

    for(int i=1 ; i<= 10; i++)
    {
        vectorInsertarAlFinal(&mivec, rand() %100);
    }

    vectorMostrar(&mivec);

    vectorOrdenar(&mivec,BURBUJEO);

    vectorMostrar(&mivec);

}

bool vectorCrear(Vector* v)
{
    v->ce = 0;
    v->vec = malloc(CAP_INI * sizeof(int));

    if(v->vec == NULL)
    {
        v->cap = 0;
        return false;
    }

    v->cap = CAP_INI;

    return true;
}


int vectorOrdInsertar(Vector* v, int elem) //ok
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, CAP_INI))
        {
            return LLENO;
        }
    }

    int* i = v->vec;
    int* ult = v->vec + v->ce - 1;

    while(i <= ult && elem > *i)
    {
        i++;
        if(elem == *i)
        {
            return DUPLICADO; 
        }
    }

    int* posIns = i;

    for(i = ult; i >= posIns; i--)
    {
       *(i + 1) = *i;
    }

    *posIns = elem;

    v->ce++;

    return TODO_OK;
}


int vectorInsertarAlFinal(Vector* v, int elem) // ok
{
    if(v->ce == v->cap)
    {
        if (!redimensionarVector(v,CAP_INI))
        {
            return LLENO;
        }
    }

    int *ult = v->vec + v->ce;  // no se resta 1 ya que quiero quedar en la ultima pos sin algun valor

    *ult = elem;
    v->ce ++;

    return TODO_OK;
}

int vectorInsertarAlInicio(Vector* v, int elem) // DESPLAZAR todos los elementos para insertar
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, CAP_INI))
        {
            return LLENO;
        }
    }
    if( v->ce>0 ) // seguridad por si el vector esta vacio
    {
        for (int *i = v->vec + v->ce-1 ; i >= v->vec ; i--) // desplazando los elem de las ultimas pos
        {
            *(i+1) = *i;
        }
    }

    *v->vec = elem;
    v->ce ++;

    return TODO_OK;
}


int vectorInsertarEnPos(Vector* v, int elem, int pos) // tengo q insertar en cadena y dsp desplazar
{
    if(v->ce == v->cap) // 
    {
        if(!redimensionarVector(v, CAP_INI))
        {
            return LLENO;
        }
    }

    int *m = v->vec + pos-1;  // puntero en pos indicada
    int *ult = v->vec + v->cap-1;

    if( pos>=0 && m <= ult ) // importante pos >= 0
    {
        *m = elem;

    }

    return TODO_OK;
}


int vectorOrdBuscar(const Vector* v, int elem)
{
    if(v->ce == 0)
        return NULL;

    const int* li = v->vec;
    const int* ls = v->vec + v->ce - 1;
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

    return NULL;
}


int *vectorDesordBuscar(const Vector* v, int elem) //ok
{
    int *i = v->vec;
    int *ult = v->vec + v->ce-1;

    while(i<=ult)
    {
        if(elem == *i)
            return i;
        i++;
    }
    return NULL;
}


bool vectorOrdEliminar(Vector* v, int elem)
{
    int pos = vectorOrdBuscar(v, elem);

    if(pos == -1)
    {
        return false;
    }

    vectorEliminarDePos(v, pos);

    return true;
}


void vectorEliminarDePos(Vector* v, int pos)
{
    int* i = v->vec + pos;
    int* ult = v->vec + v->ce - 1;

    for(i ; i < ult; i++)
    {
        *i = *(i + 1);
    }

    v->ce--;

    if((float)v->ce / v->cap <= FACTOR_OCUP)
    {
        redimensionarVector(v, FACTOR_DECR);
    }
}

void vectorOrdenar(Vector* v, int metodo)
{
    switch (metodo)
    {
        case BURBUJEO:
            ordenarBurbujeo(v);
            break;
        case SELECCION:
            ordenarSeleccion(v);
            break;
        case INSERCION:
            ordenarInsercion(v);
            break;
    }

}

void ordenarBurbujeo(Vector * v)
{
    int *ult = v->vec + v->ce-1;

    for(int i=1, *limj = ult-1; i < v->ce-1 ; i++, limj--)
    {
        for (int* j=v->vec ; j <= limj ; j++)
        {
            if(*j > *(j+1))
            {
                intercambiar(j, j+1);

            }
        }
    }

}

//void ordenarSeleccion(Vector * v)
//{


//}

/*void ordenarInsercion(Vector * v)
{

}
*/
void intercambiar ( int *a, int *b)
{
    int atem = *a;
    *a = *b;
    *b = atem;
}


//void vectorOrdenar(Vector* v)
//{

//}

// buscar menor

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
}


size_t vectorCE(const Vector* v)
{
    return v->ce;
}


bool redimensionarVector(Vector* v, float factor)
{
    size_t nuevaCap = max(v->cap * factor, CAP_INI);
    int* nuevoVec = realloc(v->vec, nuevaCap * sizeof(int));

    if(!nuevoVec) // nuevoVec == NULL
    {
        return false;
    }

    if(v->cap != nuevaCap)
    {
        printf("Redimensión del vector de %Iu a %Iu.\n", v->cap, nuevaCap);
    }

    v->cap = nuevaCap;
    v->vec = nuevoVec;

    return true;
}

size_t max(size_t a, size_t b)
{
    return a > b ? a : b;
}

void vectorMostrar( const Vector *v)
{
    int *ult = v->vec + v->ce -1;

    for(int *i = v->vec ; i <= ult ; i++)
    {
        printf("%3d", *i);
    }

    putchar('\n');

}
















