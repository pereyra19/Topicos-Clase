#include<stdio.h>
#include<stdlib.h>


typedef int(*Cmp)(const void *e1, const void *e2);

void **crearMatriz(size_t tanCol,size_t tamFil, size_t tamElem);
void ordenarSeleccion(Vector *v, Cmp cmp);


int main()
{

    return 0;
}

void **crearMatriz(size_t columnas,size_t filas, size_t tamElem)
{
    void **m = malloc(filas *tamElem);
    if(!m)
    {
        return NULL;
    }

    void *ult = m +(filas-1);

    for(void **i=m; i<=ult; i++)
    {
        *i = malloc(columnas *tamElem);
        if(*i == NULL)
        {
            destruirVector(m,i -m);
            return NULL;
        }
    }
}

void destruirVector(void **m, size_t filas)
{
    void **ult = m + (filas-1);

    for(void **i= m; i<= ult ; i++)
    {
        free(*i);
    }

    free(m);
}

void ordenarSeleccion(Vector *v, Cmp cmp)
{
    void *ult = v->vec + (v->ce-1) *v->tamElem;
    void *m;

    for(void *i=v->vec; i<=ult; i+=v->tamElem)
    {
        m = buscarMenor(i,ult,cmp);

        intercambiar(m,i);
    }
}

void *buscarMenor(void *act, void *ult, Cmp cmp)
{
    void *ult = v->vec + (v->ce-1) *v->tamElem;
    void *m = act;

    for(void *i = act; i<=ult; i+=v->tamElem)
    {
        if(cmp(m,i)<0)
        {
            m = i;
        }
    }

    return m;
}   
















