#ifndef PALABRA_H
#define PALABRA_H

#include <stdbool.h>

#define TAM 51

#define esLetra(car) (((car) >= 'a' && (car) <= 'z') || ((car) >= 'A' && (car) <= 'Z'))

typedef struct
{
    char vPal[TAM];
}
Palabra;

void palabraTitulo(Palabra* pal);
void palabraMostrar(const Palabra* pal);

//bool esLetra(char c);



#endif // PALABRA_H


#ifndef SECUENCIA_H
#define SECUENCIA_H

#include <stdbool.h>
#include "Palabra.h"


typedef struct
{
    char* cursor;
    bool finSec;
}
SecuenciaPalabras;


void secuenciaPalabrasCrear(SecuenciaPalabras* sec, const char* cad);
bool secuenciaPalabrasLeer(SecuenciaPalabras* sec, Palabra* pal);
bool secuenciaPalabrasFin(const SecuenciaPalabras* sec);
void secuenciaPalabrasEscribir(SecuenciaPalabras* sec, const Palabra* pal);
void secuenciaPalabrasEscribirCaracter(SecuenciaPalabras* sec, char c);
void secuenciaPalabrasCerrar(SecuenciaPalabras* sec);

#endif // SECUENCIA_H
