#include <stdio.h>
#include <stdlib.h>

#define ARG_BIN 1
#define ARG_TXT 2
#define ARG_TIPO_TXT 3

#define TODO_OK 0
#define ERR_ARCHIVO 1
#define ERR_MEMORIA 2

#define TAM_DNI 8
#define TAM_APYN 20
#define TAM_SEXO 1
#define TAM_FECHA 8
#define TAM_SUELDO 11


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


typedef void (*TxtABin)(const void* reg, FILE* archTxt);

void empleadoBinATxtV(const void* reg, FILE* archTxt);
void empleadoBinATxtF(const void* reg, FILE* archTxt);
int convTxtABin(const char* nomArchBin, const char* nomArchTxt, size_t tamReg, TxtABin binATxt);
int generarEmpleadosBin(const char* nomArchBin);


// ConversorBinATxt Empleados.dat Empleados.txt V

int main(int argc, char* argv[])
{
    int ret;
//    generarEmpleadosBin(argv[ARG_BIN]);

    char tipoTxt = argv[ARG_TIPO_TXT][0];
    ret = convTxtABin(argv[ARG_BIN], argv[ARG_TXT], sizeof(Empleado), tipoTxt == 'V'? empleadoBinATxtV : empleadoBinATxtF);

    return ret;
}


int convBinaTxt(const char* nomArchBin, const char* nomArchTxt, size_t tamReg, TxtABin binATxt)
{
    FILE* archBin = fopen(nomArchBin, "rb");
    if(!archBin)
    {
        return ERR_ARCHIVO;
    }

    FILE* archTxt = fopen(nomArchTxt, "wt");

    if(!archTxt)
    {
        fclose(archBin);
        return ERR_ARCHIVO;
    }

    void* reg = malloc(tamReg);

    if(!reg)
    {
        fclose(archBin);
        fclose(archTxt);
        return ERR_MEMORIA;
    }

    while(fread(reg, tamReg, 1, archBin))
    {
        binATxt(reg, archTxt);
    }

    fclose(archBin);
    fclose(archTxt);
    free(reg);

    return TODO_OK;
}


void empleadoBinATxtV(const void* reg, FILE* archTxt)
{
    const Empleado* empl = reg;
    fprintf(archTxt, "%d|%s|%c|%d/%d/%d|%.2f\n", empl->dni, empl->apyn, empl->sexo, empl->fIngr.d, empl->fIngr.m, empl->fIngr.a, empl->sueldo);
}


void empleadoBinATxtF(const void* reg, FILE* archTxt)
{
    const Empleado* empl = reg;
    fprintf(archTxt, "%0*d%-*s%c%02d%02d%04d%0*.2f\n", TAM_DNI, empl->dni, TAM_APYN, empl->apyn, empl->sexo, empl->fIngr.d, empl->fIngr.m, empl->fIngr.a, TAM_SUELDO, empl->sueldo);
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



