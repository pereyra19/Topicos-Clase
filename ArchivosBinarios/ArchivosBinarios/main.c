#include <stdio.h>
#include <stdlib.h>
#include "../TDAVector/Vector.h"


#define CANT_PRODS 10

#define TODO_OK 0
#define ERR_ARCHIVO 1
#define SIN_MEM 2
#define NO_ENCONTRADO 3


typedef struct
{
    char codigo[5];
    char descripcion[21];
    float precio;
    int stock;
}
Producto;


typedef struct
{
    char codigo[5];
    int nroReg;
}
ProductoIdx;


typedef void (*Imprimir)(const void*);


int generarProductos(const char* nomArch);
int mostrarArchivo(const char* nomArch, Imprimir imprimir, size_t tamReg);
void mostrarProducto(const void* a);
int actualizarProductos(const char* nomArch, float porc);
int actualizar1Producto(const char* nomArch, const Vector* vIdx, const char* codigo, float porc);
int generarIndiceProductos(const char* nomArch, const char* nomArchIdx);
int cmpProdIdx(const void* a, const void* b);


int main()
{
    system("chcp 65001 > nul");

    int ret;

    ret = generarProductos("Productos.dat");

    if(ret != TODO_OK)
    {
        puts("Error generando el archivo.");
        return ret;
    }

    ret = generarIndiceProductos("Productos.dat", "Productos.idx");

    if(ret != TODO_OK)
    {
        puts("Error generando el índice.");
        return ret;
    }

    puts("Antes de actualizar:");
    ret = mostrarArchivo("Productos.dat", mostrarProducto, sizeof(Producto));

    if(ret != TODO_OK)
    {
        puts("Error mostrando el archivo.");
        return ret;
    }

//    ret = actualizarProductos("Productos.dat", 10);
    
    Vector vIdx;
    ret = vectorCrearDeArchivo(&vIdx, sizeof(ProductoIdx), "Producto.idx");

    if(ret != TODO_OK)
    {
        puts("Error creando el vector.");
        return ret;
    }    

    ret = actualizar1Producto("Productos.dat", &vIdx, "KIWI", 10);

    if(ret != TODO_OK)
    {
        puts("Error actualizando el archivo.");
        return ret;
    }    

    vectorDestruir(&vIdx);

    puts("Después de actualizar:");
    ret = mostrarArchivo("Productos.dat", mostrarProducto, sizeof(Producto));

    if(ret != TODO_OK)
    {
        puts("Error mostrando el archivo.");
        return ret;
    }

    return TODO_OK;
}


int generarProductos(const char* nomArch)
{
    Producto vProds[CANT_PRODS] =
    {
        {"PERA", "Pera Williams", 1000.0, 100},
        {"MANZ", "Manzana Verde", 2000.0, 25},
        {"NARA", "Naranja", 3500.0, 35},
        {"BANA", "Banana", 4200.0, 140},
        {"KIWI", "Kiwi", 2500.0, 250},
        {"UVA", "Uva", 3600.0, 360},
        {"MELO", "Melon", 4700.0, 170},
        {"FRUT", "Frutilla", 3800.0, 280},
        {"MORA", "Mora", 5900.0, 190},
        {"ARAN", "Arandano", 1500.0, 190}
    };

    FILE* archProds = fopen(nomArch, "wb");

    if(!archProds)
    {
        return ERR_ARCHIVO;
    }

    fwrite(vProds, sizeof(Producto), CANT_PRODS, archProds);

    fclose(archProds);

    return TODO_OK;
}


int mostrarArchivo(const char* nomArch, Imprimir imprimir, size_t tamReg)
{
    FILE* arch = fopen(nomArch, "rb");

    if(!arch)
    {
        return ERR_ARCHIVO;
    }

    void* reg = malloc(tamReg);

    if(!reg)
    {
        fclose(arch);
        return SIN_MEM;
    }

    while(fread(reg, tamReg, 1, arch))
    {
        imprimir(reg);
    }

    free(reg);
    fclose(arch);

    return TODO_OK;
}


void mostrarProducto(const void* a)
{
    const Producto* prod = a;
    printf("%-4s\t%-20s\t%8.2f\t%4d\n", prod->codigo, prod->descripcion, prod->precio, prod->stock);
}


int actualizarProductos(const char* nomArch, float porc)
{
    FILE* archProds = fopen(nomArch, "r+b");

    if(!archProds)
    {
        return ERR_ARCHIVO;
    }

    float factorIncr = 1 + porc / 100;
    
    Producto prod;

    while(fread(&prod, sizeof(Producto), 1, archProds))
    {
        prod.precio *= factorIncr;
        fseek(archProds, (long)-sizeof(Producto), SEEK_CUR);
        fwrite(&prod, sizeof(Producto), 1, archProds);
        fseek(archProds, 0L, SEEK_CUR);
    }

    fclose(archProds);

    return TODO_OK;
}


int generarIndiceProductos(const char* nomArch, const char* nomArchIdx)
{
    FILE* archProds = fopen(nomArch, "rb");

    if(!archProds)
    {
        return ERR_ARCHIVO;
    }

    Vector vIdx;
    if(!vectorCrear(&vIdx, sizeof(ProductoIdx)))
    {
        fclose(archProds);
        return SIN_MEM;
    }

    Producto prod;
    ProductoIdx prodIdx;
    int cont = 0;

    while(fread(&prod, sizeof(Producto), 1, archProds))
    {
        strcpy(prodIdx.codigo, prod.codigo);
        prodIdx.nroReg = cont++;
        
        if(vectorOrdInsertar(&vIdx, &prodIdx, cmpProdIdx) != TODO_OK)
        {
            fclose(archProds);
            vectorDestruir(&vIdx);
            return SIN_MEM;
        }
    }

    fclose(archProds);

    if(!vectorGuardarEnArchivo(&vIdx, nomArchIdx))
    {
        vectorDestruir(&vIdx);
        return ERR_ARCHIVO;
    }

    vectorDestruir(&vIdx);

    return TODO_OK;
}


int cmpProdIdx(const void* a, const void* b)
{
    ProductoIdx* p1 = a;
    ProductoIdx* p2 = b;

    return strcmp(p1->codigo, p2->codigo);
}


int actualizar1Producto(const char* nomArch, const Vector* vIdx, const char* codigo, float porc)
{
    FILE* archProds = fopen(nomArch, "r+b");

    if(!archProds)
    {
        return ERR_ARCHIVO;
    }

    ProductoIdx prodIdx;
    strcpy(prodIdx.codigo, codigo);

    int pos = vectorOrdBuscar(vIdx, &prodIdx, cmpProdIdx);

    if(pos == -1)
    {
        puts("Código no encontrado.");
        fclose(archProds);
        return NO_ENCONTRADO;
    }

    Producto prod;

    fseek(archProds, prodIdx.nroReg * sizeof(Producto), SEEK_SET);
    fread(&prod, sizeof(Producto), 1, archProds);
    prod.precio *= 1 + porc / 100;
    fseek(archProds, prodIdx.nroReg * sizeof(Producto), SEEK_SET);
    fwrite(&prod, sizeof(Producto), 1, archProds);
    
    fclose(archProds);

    return TODO_OK;
}