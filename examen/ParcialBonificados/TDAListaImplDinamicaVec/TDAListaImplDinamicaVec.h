#include <stddef.h>


typedef struct
{
    void* vec;
    int ce;
    size_t cap;
    size_t tamElem;
}
Lista;


typedef struct
{
	void* act;
	void* pri;
	void* ult;
	size_t tamElem;
	bool finIter;
}
ListaIterador;
