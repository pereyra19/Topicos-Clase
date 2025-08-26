/***************************************************************************************
******************************* Complete sus datos *************************************
****************************************************************************************
* Apellido, Nombres:pereyra roberto
*
* DNI: 42251380
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

	//desencriptarTransferencias("Transferencias.txt", "TransferenciasDesencriptadas.txt");
	desencriptarTransferencias_ALU("Transferencias.txt", "TransferenciasDesencriptadas.txt");

	puts("Archivo Transferencias:");
	puts("Dep Orig | Dep Dest | Cod Prod | Cant");
	mostrarArchivoTransferencias("TransferenciasDesencriptadas.txt");

	puts("");
	puts("Depositos antes de actualizar:");
	mostrarArchivoDepositos("Depositos.dat");

	//actualizarDepositos("TransferenciasDesencriptadas.txt", "Depositos.dat");
	actualizarDepositos_ALU("TransferenciasDesencriptadas.txt", "Depositos.dat");


	puts("");
	puts("Depositos despues de actualizar:");
	mostrarArchivoDepositos("Depositos.dat");

	return 0;
}


int desencriptarTransferencias_ALU(const char* archTransferenciasEncr, const char* archTransferenciasDesencr)
{
	FILE *archEncr = fopen(archTransferenciasEncr,"rt");
	if(!archEncr)
	{
		return ERR_ARCHIVO;
	}
	FILE *archDesen= fopen(archTransferenciasDesencr,"wt");
	if(!archDesen)
	{
		fclose(archEncr);
		return ERR_ARCHIVO;
	}

	Transferencia tranferencia;
	char *act;
	char linea[TAMLINEA+1];
	char aux[TAMLINEA+1];
	int ret = TODO_OK, i=0;
	char *ult;
	char *ini = linea;
	double cant;

	while(fgets(linea,TAMLINEA+1,archEncr) && ret == TODO_OK)
	{
		i=0;
		ult = linea + strlen(linea) -2;
		while(ult >= ini)
		{
			aux[i++] = (*ult -5);
			ult--;
		}

		aux[i] = '\0';
	//{ depOrigen: entero, depDestino: entero, codProd: cadena(15), cantidad: double }
		// cantidad
		act = strrchr(aux,'|');
		cant= atof(act+1);
		tranferencia.cantidad = (int)cant;
		// codProd
		*act = '\0';
		act = strrchr(aux,'|');
		strcpy(tranferencia.codProd,act+1);
		// dep destino
		*act = '\0';
		act = strrchr(aux,'|');
		tranferencia.depDestino = atoi(act+1);
		//dep origen
		*act = '\0';
		tranferencia.depOrigen = atoi(aux);

		fprintf(archDesen,"%d|%d|%s|%d\n",tranferencia.depOrigen,tranferencia.depDestino,tranferencia.codProd,tranferencia.cantidad);
	}

	fclose(archDesen);
	fclose(archEncr);
	return TODO_OK;
}


int actualizarDepositos_ALU(const char* archTransferencias, const char* archDepositos)
{
	FILE *archTran = fopen(archTransferencias, "rt");
	if(!archTran)
	{
		return ERR_ARCHIVO;
	}
	FILE *archDepo = fopen(archDepositos,"r+b");
	if(!archDepo)
	{
		fclose(archTran);
		return ERR_ARCHIVO;
	}

	Vector vecDep;

	if(!crearVector_ALU(&vecDep,sizeof(TransferenciaElem)))
	{
		fclose(archDepo);
		fclose(archTran);
		return SIN_MEN;
	}

	char linea[TAMLINEA+1];
	int ret = TODO_OK;
	char *act;
	TransferenciaElem origen;
	TransferenciaElem destino;
	int cant;

	while(fgets(linea,TAMLINEA+1,archTran) && ret == TODO_OK)
	{
		//depOrigen: entero, depDestino: entero, codProd: cadena(15), cantidad: int}
		act = strrchr(linea,'\n');
		if(act)
		{
			//cant
			*act = '\0';
			act = strrchr(linea,'|');
			cant = atoi(act+1);
			origen.cantidad = -cant;
			destino.cantidad = cant;
			//codProd
			*act = '\0';
			act = strrchr(linea,'|');
			strcpy(origen.codProd,act+1);
			// dep destino
			*act = '\0';
			act = strrchr(linea,'|');
			destino.nroDep = atoi(act+1);
			// dep origen
			*act = '\0';
			origen.nroDep = atoi(linea);

			strcpy(destino.codProd,origen.codProd);

			if(!insertarVecOrd_ALU(&vecDep,&origen,cmpTran_ALU,actualizarTran_ALU))
				ret = SIN_MEN;

			if(!insertarVecOrd_ALU(&vecDep,&destino,cmpTran_ALU,actualizarTran_ALU))
				ret = SIN_MEN;
		}
		else
			ret = ERR_LINEA;
	}
	if(ret!= TODO_OK)
	{
		destruirVector_ALU(&vecDep);
		fclose(archDepo);
		fclose(archTran);
		return ret;
	}

	Deposito deposito;
	TransferenciaElem transf;
	int band;

	while(fread(&deposito,sizeof(Deposito),1,archDepo))
	{
		transf.nroDep = deposito.nro_dep;
		strcpy(transf.codProd,deposito.cod_prod);
		band = buscarVecOrd_ALU(&vecDep,&transf,cmpTran_ALU);

		if(band==1)
		{
			deposito.stock += transf.cantidad;

			fseek(archDepo, -(long)sizeof(Deposito),SEEK_CUR);
			fwrite(&deposito,sizeof(Deposito),1,archDepo);
			fseek(archDepo, 0L ,SEEK_CUR);
		}
	}

	destruirVector_ALU(&vecDep);
	fclose(archDepo);
	fclose(archTran);
	return TODO_OK;
}

int buscarVecOrd_ALU(Vector *v, void *elem, Cmp cmp)
{
    if (v->ce == 0)
        return -1;

    char *base = (char*)v->vec;
    int li = 0;
    int ls = v->ce - 1;
    int mid = (li + ls) / 2;
    void *m = base + mid * v->tamElem;
    int comp = cmp(elem, m);

    while (li <= ls && comp != 0)
    {
        if (comp < 0)
            ls = mid - 1;

        if(comp>0)
			li = mid + 1;

        mid = (li + ls) / 2;
        m   = base + mid * v->tamElem;
        comp = cmp(elem, m);
    }

    if (comp == 0)
    {
        memcpy(elem, m, v->tamElem);
        return 1;
    }

    return -1;
}





bool insertarVecOrd_ALU(Vector *v,const void *elem,Cmp cmp,Actualizar actu)
{
	if(v->ce == v->cap)
	{
		if(!redimensionar_ALU(v,FAC_INCR))
		{
			return false;
		}
	}

	if(v->ce==0)
	{
		memcpy(v->vec,elem,v->tamElem);
		v->ce++;
		return true;
	}

	void *act = v->vec;
	void *ult = v->vec + (v->ce-1) * v->tamElem;

	while(act <= ult && cmp(elem,act)>0)
	{
		act += v->tamElem;
	}

	if(cmp(elem,act)==0)
	{
		actu(act,elem);
		return true;
	}

	void *ins = act;

	for(act = ult ; act >= ins; act-= v->tamElem)
	{
		memcpy(act+v->tamElem,act ,v->tamElem);
	}

	memcpy(ins,elem,v->tamElem);
	v->ce++;

	return true;
}

int cmpTran_ALU(const void *e1,const void *e2)
{
	const TransferenciaElem *a = e1;
	const TransferenciaElem *b = e2;
	int cant = a->nroDep - b->nroDep;

	if(cant ==0)
	{
		return strcmp(a->codProd,b->codProd);
	}
	return cant;
}

void actualizarTran_ALU(void* actualizado, const void* actualizador)
{
	TransferenciaElem *a = actualizado;
	const TransferenciaElem *b = actualizador;

	a->cantidad += b->cantidad;
}


bool crearVector_ALU(Vector *v, size_t tamElem)
{
	v->ce =0;
	v->vec = malloc(tamElem * CAP_INI);
	if(!v->vec)
	{
		return false;
	}
	v->tamElem =tamElem;
	v->cap =CAP_INI;
	return true;
}


void destruirVector_ALU(Vector *v)
{
	v->ce =0;
	free(v->vec);
	v->vec = NULL;
	v->tamElem =0;
	v->cap = 0;
}

bool redimensionar_ALU(Vector *v,float factor)
{
	size_t ncap = factor *v->cap;
	void *nvec = realloc(v->vec, ncap *v->tamElem);
	if(!nvec)
	{
		return false;
	}

	v->cap = ncap;
	v->vec = nvec;
	return true;
}
