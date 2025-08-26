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




