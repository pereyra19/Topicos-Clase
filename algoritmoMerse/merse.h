#ifndef MERSE_H_INCLUDED
#define MERSE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h> 

#define ERR_ARCH 1
#define TAM_NOM_ARCH 10
#define TODO_OK 0

typedef struct
{
    char codigo;
    int stock;
    char * descripcion;
}producto;

typedef struct
{
    char codigo;
    int cant;
}movimiento;

#endif // MERSE_H_INCLUDED
