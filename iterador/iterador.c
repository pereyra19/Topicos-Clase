#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct
{
    void *vec;
    size_t ce;
    size_t tamElem;
    size_t cap;
}
vector;

typedef struct
{
    void *act;
    void *ult;
    vector *vector;
    bool finIter;
}
vectorIterador;

typedef struct
{
    int d,m,a;
}
fecha;

typedef void (*Accion)(void *elem, void *datos);
void filtrarFecha(void *elem, void *datos);


// Iterador
void vectorIteradorCrear(vectorIterador * it, vector *v);
void *vectorIteradorPrimero(vectorIterador * it); // inicia la recorrida en caso de ser necesaria 
void *vectorIteradorSiguiente(vectorIterador * it); 
bool vectorIteradorFin(vectorIterador *it);
// podria haber un Iteradorfinal y un IteradorAnterior si queremos recorrer en sentido inverso

int main()
{

    fecha fFiltro = {1,1,1970};


    // vectorRecorrer(&miVec, filtrarFecha,&fFilto);


    puts("con iterador");
    
    vectorIterador it;
    vector miVec;
    vectorIteradorCrear(&it,&miVec);

    fecha * fAct = vectorIteradorPrimero(&it);
    while(!vectorIteradorFin(&it))
    {
        if(fAct->a *1000 + fAct->m *100 + fAct->d - (fFiltro.a *1000 + fFiltro.m *100 + fFiltro.d ) > 0)
        {
            printf("%02d/%02d/%04d \n",fAct->d, fAct->m, fAct->a);
        } 

        fAct = vectorIteradorSiguiente(&it);
    }

    vectorDestruir(&miVec);

    return 0;
}


void vectorRecorrer(vector *v, Accion accion, void *datos)
{
    void *ult = v->vec + (v->ce -1) * v->tamElem;

    for(void *i = v->vec; i<=ult ; i=+ v->tamElem)
    {
        accion(i, datos);
    }
}

void filtrarFecha(void *elem, void *datos)
{
    fecha *felem = elem;
    fecha *fFiltro = datos;

    // if((//reta de fechas-(fecha ))>0)
    // {
    //     printf("%d/%d/%d\n",felem->d,felem->m,felem->a,)
    // }
}

void vectorIteradorCrear(vectorIterador * it, vector *v)
{
    it->act = NULL;
    it->ult = NULL;
    it->finIter = true;
    it->vector = v;
}

void *vectorIteradorPrimero(vectorIterador * it) // inicia la recorrida en caso de ser necesaria 
{
    vector* v = it->vector;
    
    if(v->ce ==0)
    {
        it->finIter = true;
        return NULL;
    }

    it->act = v->vec;
    it->ult = v->vec + (v->ce - 1) *v->tamElem;
    it->finIter = false;

    return it->act;
}

void *vectorIteradorSiguiente(vectorIterador * it)
{
    void *sig = it->act + it->vector->tamElem;
    
    if(sig > it->ult)
    {
        it->finIter = true;
        return NULL;
    }

    it->act = sig;

    return sig;
}

bool vectorIteradorFin(vectorIterador *it)
{
    return it->finIter;
}