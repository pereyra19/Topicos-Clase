#ifndef SOLUCION_BIBLIOTECA_H
#define SOLUCION_BIBLIOTECA_H

#include <Vector.h>





int generarArchLibros(const char* nombreArchLibros, const char* nombreArchLibrosIndice);
int generarArchMovimientosLibro(const char* nombreArchMovimientos);
void mostrarLibros(const char* nombreArchLibros);
int cargarVectorMovimientosLibro(Vector* vecMovs, const char* nombreArchMov);
int actualizarArchivoLibros(const char* nombreArchivoLibros, const char* nombreArchivoLibrosIndice, Vector* vecMovs);
int normalizarNombresLibros(const char* nombreArchivoLibros);
void mostrarMovimientoLibro(void* pvmov, void*);


#endif // SOLUCION_BIBLIOTECA_H
