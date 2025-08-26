#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define ARG_TXT 1
#define ARG_TIPO_TXT 2
#define ARG_BIN 3

#define TODO_OK 0
#define ERR_ARCHIVO 1
#define ERR_MEMORIA 2
#define ERR_LINEA_LARGA 3

#define TAM_DNI 8
#define TAM_APYN 20
#define TAM_SEXO 1
#define TAM_FECHA 8
#define TAM_SUELDO 11

#define TAM_LINEA 500


typedef struct
{
    int d;
    int m;
    int a;
}
Fecha;


typedef struct
{
    int dni;
    char apyn[TAM_APYN + 1];
    char sexo;
    Fecha fIngr;
    float sueldo;
}
Empleado;


typedef int (*TxtABin)(const char* linea, void* reg);
typedef bool (*EsErrorFatal)(int cod);


int empleadoTxtVABin(const char* linea, void* reg);
int empleadoTxtFABin(const char* linea, void* reg);
bool esErrorFatalEmpleado(int cod);
int convTxtABin(const char* nomArchTxt, const char* nomArchBin, size_t tamReg, TxtABin txtABin, EsErrorFatal esErrorFatal);
int mostrarEmpleadosBin(const char* nomArchBin);


// ConversorTxtABin Empleados.txt V Empleados.dat

int main(int argc, char* argv[])
{
    int ret;
//    generarEmpleadosBin(argv[ARG_BIN]);

    char tipoTxt = argv[ARG_TIPO_TXT][0];
    ret = convTxtABin(argv[ARG_TXT], argv[ARG_BIN], sizeof(Empleado), tipoTxt == 'V'? empleadoTxtVABin : empleadoTxtFABin, esErrorFatalEmpleado);

    mostrarEmpleadosBin(argv[ARG_BIN]);

    return ret;
}


int convTxtABin(const char* nomArchTxt, const char* nomArchBin, size_t tamReg, TxtABin txtABin, EsErrorFatal esErrorFatal)
{
    FILE* archTxt = fopen(nomArchTxt, "rt");

    if(!archTxt)
    {
        return ERR_ARCHIVO;
    }

    FILE* archBin = fopen(nomArchBin, "wb");

    if(!archBin)
    {
        fclose(archTxt);
        return ERR_ARCHIVO;
    }

    void* reg = malloc(tamReg);

    if(!reg)
    {
        fclose(archTxt);
        fclose(archBin);
        return ERR_MEMORIA;
    }

    char linea[TAM_LINEA + 1];
    int ret = TODO_OK;
    while(!esErrorFatal(ret) && fgets(linea, TAM_LINEA + 1, archTxt))
    {
        ret = txtABin(linea, reg);

        if(ret == TODO_OK)
        {
            fwrite(reg, tamReg, 1, archBin);
        }
    }

    fclose(archTxt);
    fclose(archBin);
    free(reg);

    return TODO_OK;
}


int empleadoTxtVABin(const char* linea, void* reg)
{
    Empleado* empl = reg;
    
    char* act = strchr(linea, '\n');

    if(!act)
    {
        return ERR_LINEA_LARGA;
    }

    // Sueldo
    *act = '\0';
    act = strrchr(linea, '|');
    sscanf(act + 1, "%f", &empl->sueldo);

    // FIngr
    *act = '\0';
    act = strrchr(linea, '|');
    sscanf(act + 1, "%d/%d/%d", &empl->fIngr.d, &empl->fIngr.m, &empl->fIngr.a);

    // Sexo
    *act = '\0';
    act = strrchr(linea, '|');
    empl->sexo = *(act + 1);

    // ApYN
    *act = '\0';
    act = strrchr(linea, '|');
    strncpy(empl->apyn, act + 1, TAM_APYN);
    empl->apyn[TAM_APYN] = '\0';

    // DNI
    *act = '\0';
    sscanf(linea, "%d", &empl->dni);

    return TODO_OK;
}


int empleadoTxtFABin(const char* linea, void* reg)
{
    Empleado* empl = reg;

    char* act = strchr(linea, '\n');

    if(!act)
    {
        return ERR_LINEA_LARGA;
    }

    // Sueldo
    *act = '\0';
    act -= TAM_SUELDO;
    sscanf(act, "%f", &empl->sueldo);

    // FIngr
    *act = '\0';
    act -= TAM_FECHA;
    sscanf(act, "%2d%2d%4d", &empl->fIngr.d, &empl->fIngr.m, &empl->fIngr.a);

    // Sexo
    *act = '\0';
    act -= TAM_SEXO;
    empl->sexo = *act;

    // ApYN
    *act = '\0';
    act -= TAM_APYN;
    strncpy(empl->apyn, act, TAM_APYN);
    empl->apyn[TAM_APYN] = '\0';

    // DNI
    *act = '\0';
    sscanf(linea, "%d", &empl->dni);

    return TODO_OK;
}


int generarEmpleadosBin(const char* nomArchBin)
{
    Empleado vEmpls[10] =
    {
        {12345678, "Juan Perez", 'M', {1, 1, 2020}, 500000.00},
        {23456789, "Ana Gomez", 'F', {15, 3, 2019}, 600000.50},
        {34567890, "Luis Fernandez", 'M', {20, 5, 2021}, 550000.75},
        {45678901, "Maria Lopez", 'F', {10, 7, 2018}, 700000.25},
        {56789012, "Carlos Ruiz", 'M', {30, 9, 2022}, 480000.00},
        {67890123, "Laura Martinez", 'F', {5, 11, 2020}, 520000.80},
        {78901234, "Pedro Sanchez", 'M', {12, 12, 2017}, 580000.90},
        {89012345, "Sofia Torres", 'F', {25, 2, 2023}, 620000.10},
        {90123456, "Javier Ramirez", 'M', {18, 4, 2016}, 540000.60},
        {12345679, "Clara Diaz", 'F', {22, 6, 2024}, 650000.30}
    };

    FILE* archBin = fopen(nomArchBin, "wb");

    if(!archBin)
    {
        return ERR_ARCHIVO;
    }

    fwrite(vEmpls, sizeof(Empleado), 10, archBin);

    fclose(archBin);

    return TODO_OK;
}


bool esErrorFatalEmpleado(int cod)
{
    switch(cod)
    {
        case ERR_LINEA_LARGA:
            return true;

        default:
            return false;
    }
}


int mostrarEmpleadosBin(const char* nomArchBin)
{
    FILE* archBin = fopen(nomArchBin, "rb");

    if(!archBin)
    {
        return ERR_ARCHIVO;
    }

    Empleado empl;
    while(fread(&empl, sizeof(Empleado), 1, archBin))
    {
        printf("%d|%s|%c|%02d/%02d/%04d|%.2f\n", empl.dni, empl.apyn, empl.sexo, empl.fIngr.d, empl.fIngr.m, empl.fIngr.a, empl.sueldo);
    }

    fclose(archBin);

    return TODO_OK;
}
