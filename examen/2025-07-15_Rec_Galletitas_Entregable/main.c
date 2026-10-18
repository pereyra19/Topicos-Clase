/***************************************************************************************
******************************* Complete sus datos *************************************
****************************************************************************************
* Apellido, Nombres:pereyra roberto
*
* DNI: 
*
****************************************************************************************/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <stdio.h>
#include <SolucionParcialGalletitasTopicos.h>

#define TODO_OK 0
#define SIN_MEN -4
#define ERR_ARCHIVO -2
#define ERR_LINEA -3
#define TAMLINEA 300
#define CAP_INI 40
#define FAC_INCR 2


typedef void (*Actualizar)(void* actualizado, const void* actualizador);
typedef int (*Cmp)(const void* e1, const void* e2);

typedef struct
{
	void* vec;
	int ce;
	int cap;
	size_t tamElem;
}
Vector;


// tda
bool crearVector_ALU(Vector *v, size_t tamElem);
void destruirVector_ALU(Vector *v);
bool insertarVecOrd_ALU(Vector *v, const void *elem,Cmp cmp,Actualizar actualizar);
int buscarVecOrd_ALU(Vector *v,void * elem,Cmp cmp);
bool redimensionar_ALU(Vector *v, float factor);

int cmpTran_ALU(const void* e1, const void* e2);
void actualizarTran_ALU(void* actualizado, const void* actualizador);



int desencriptarTransferencias_ALU(const char* archTransferenciasEncr, const char* archTransferenciasDesencr);
int actualizarDepositos_ALU(const char* archTransferencias, const char* archDepositos);


int main()
{
	generarArchivos();

	desencriptarTransferencias("Transferencias.txt", "TransferenciasDesencriptadas.txt");
	//desencriptarTransferencias_ALU("Transferencias.txt", "TransferenciasDesencriptadas.txt");

	puts("Archivo Transferencias:");
	puts("Dep Orig | Dep Dest | Cod Prod | Cant");
	mostrarArchivoTransferencias("TransferenciasDesencriptadas.txt");

	puts("");
	puts("Depositos antes de actualizar:");
	mostrarArchivoDepositos("Depositos.dat");

	actualizarDepositos("TransferenciasDesencriptadas.txt", "Depositos.dat");
	//actualizarDepositos_ALU("TransferenciasDesencriptadas.txt", "Depositos.dat");


	puts("");
	puts("Depositos despues de actualizar:");
	mostrarArchivoDepositos("Depositos.dat");

	return 0;
}


int desencriptarTransferencias_ALU(const char* archTransferenciasEncr, const char* archTransferenciasDesencr)
{
    FILE *archTran(archTransferenciasEncr,"rt");
    if(!archTran)
    {
        return ERR_ARCHIVO;
    }
    FILE *archDes(archTransferenciasDesencr,"wt");
    if(!archDes)
    {
        fclose(archTran);
        return ERR_ARCHIVO;
    }








    return TODO_OK;
}




