#ifndef SOLUCION_PARCIAL_ECOMMERCE_H
#define SOLUCION_PARCIAL_ECOMMERCE_H

#include <stdbool.h>
#include "../Comun/Comun.h"


typedef struct
{
    char usuario[21];
    char nombre[51];
    char apellido[51];
    int nivel;
}
Cliente;


typedef struct
{
    char codigo[16];
    char descripcion[51];
    float precio;
}
Producto;


typedef struct
{
    int nivel;
    float porcDescuento;
    float topeDescuento;
    float minimoParaEnvioGratis;
}
Regla;


typedef struct
{
    char usuario[21];
    char producto[16];
}
Busqueda;


typedef struct
{
    char usuario[21];
    char producto[16];
    float precio;
    bool envioGratis;
}
Oferta;


typedef struct
{
    char usuario[21];
    int nroRegistro;
}
IndiceCliente;


typedef struct
{
    char codigo[16];
    int nroRegistro;
}
IndiceProducto;


int generarArchivoOfertas(const char* nombreArchClientes, const char* nombreArchProductos, const char* nombreArchReglas, const char* nombreArchOfertas);
int generarArchivoOfertas2(const char* nombreArchClientes, const char* nombreArchProductos, const char* nombreArchReglas, const char* nombreArchOfertas, const char* nombreArchBusquedas);
int generarArchivoOfertasTxt(const char* nombreArchivoOfertas, const char* nombreArchivoOfertasTxt);
int generarArchivoOfertas_ALU(const char* nombreArchClientes, const char* nombreArchProductos, const char* nombreArchReglas, const char* nombreArchOfertas);
int generarArchivoOfertasTxt_ALU(const char* nombreArchivoOfertas, const char* nombreArchivoOfertasTxt);
void generarArchivoClientes(const char* nombreArchClientes, bool generarIndice);
void generarArchivoProductos(const char* nombreArchProductos, bool generarIndice);
void generarArchivoReglas(const char* nombreArchReglas);
void generarArchivoBusquedas(const char* nombreArchBusquedas);
int mostrarArchivoOfertas(const char* nombreArchOfertas);
int mostrarArchivoOfertasTxt(const char* nombreArchivoOfertasTxt);
int mostrarArchivoClientes(const char* nombreArchClientes);
int mostrarArchivoProductos(const char* nombreArchProductos);
int mostrarArchivoBusquedas(const char* nombreArchBusquedas);
void cambiarExtension(const char* nombreArchivoOrig, char* nombreArchivoConExtension, const char* extension);


#endif //SOLUCION_PARCIAL_ECOMMERCE_H
