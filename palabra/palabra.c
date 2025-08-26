#include <stdio.h>
#include <ctype.h>
#include "Palabra.h"


// bool esLetra(char c)
// {
//     return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
// }


void palabraTitulo(Palabra* pal)
{
    char* iPal = pal->vPal;

    *iPal = toupper(*iPal);
    iPal++;

    while(*iPal != '\0')
    {
        *iPal = tolower(*iPal);
        iPal++;
    }
}


void palabraMostrar(const Palabra* pal)
{
    puts(pal->vPal);
}


#include "Secuencia.h"
#include "Palabra.h"


void secuenciaPalabrasCrear(SecuenciaPalabras* sec, const char* cad)
{
    sec->cursor = (char*)cad;
    sec->finSec = false;
}


bool secuenciaPalabrasLeer(SecuenciaPalabras* sec, Palabra* pal)
{
    while(*sec->cursor != '\0' && !esLetra(*(sec->cursor)))
    {
        sec->cursor++;
    }

    if(*sec->cursor == '\0')
    {
        sec->finSec = true;
        return false;
    }

    char* iPal = pal->vPal;
    *iPal = *sec->cursor;
    iPal++;
    sec->cursor++;

    while(*sec->cursor != '\0' && esLetra(*(sec->cursor)))
    {
        *iPal = *sec->cursor;
        iPal++;
        sec->cursor++;
    }

    *iPal = '\0';

    return true;
}


bool secuenciaPalabrasFin(const SecuenciaPalabras* sec)
{
    return sec->finSec;
}


void secuenciaPalabrasEscribir(SecuenciaPalabras* sec, const Palabra* pal)
{
    const char* iPal = pal->vPal;

    while(*iPal != '\0')
    {
        *sec->cursor = *iPal;
        sec->cursor++;
        iPal++;
    }
}


void secuenciaPalabrasEscribirCaracter(SecuenciaPalabras* sec, char c)
{
    *sec->cursor = c;
    sec->cursor++;
}


void secuenciaPalabrasCerrar(SecuenciaPalabras* sec)
{
    *sec->cursor = '\0';
}
