#include "Vector.h"



int main()
{

}



bool vectorCrear(Vector* v)
{
    v->ce = 0;
    return true;
}


int vectorOrdInsertar(Vector* v, int elem)
{
    if(v->ce == TAM)
    {
        return LLENO;
    }

    int* i = v->vec;
    int* ult = v->vec + v->ce - 1;

    while(i <= ult && elem > *i)
    {
        i++;
    }

    if(i <= ult && elem == *i)
    {
        return DUPLICADO;
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


int vectorInsertarAlFinal(Vector* v, int elem)
{
    int* ult = v->vec + v->ce-1;

    *ult = elem;
}
int vectorInsertarAlInicio(Vector* v, int elem)
{
    *v->vec = elem;
}
int vectorInsertarEnPos(Vector* v, int elem, int pos)
{
    int* i = v->vec + pos-1;

    *i = elem;

}



int vectorOrdBuscar(const Vector* v, int elem)
{
    if(v->ce == 0)
        return -1;

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

    return -1;
}


int vectorDesordBuscar(const Vector* v, int elem)
{
    int* ult = v->vec + v->ce-1;

    for (int* i= v->vec ; i<= v->ce ; i++)
    {
        if( *i == elem)
            return ult - v->vec;
    }

    return -1;
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


bool vectorEliminarDePos(Vector* v, int pos)
{
    if(pos < 0)
    {
        return false;
    }

    int* i = v->vec + pos;
    int* ult = v->vec + v->ce - 1;

    if(i > ult)
    {
        return false;
    }

    for(; i < ult; i++)
    {
        *i = *(i + 1);
    }

    v->ce--;

    return true;
}






void vectorVaciar(Vector* v)
{
    v->ce = 0;
}


void vectorDestruir(Vector* v)
{
    v->ce = 0;
}
