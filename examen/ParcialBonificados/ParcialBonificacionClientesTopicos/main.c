#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../SolucionParcialBonificacionClientesTopicos/UtilitariasParcialBonificacionClientes.h"
#include "../SolucionParcialBonificacionClientesTopicos/SolucionParcialBonificacionClientes.h"
#include "../SolucionParcialBonificacionClientesTopicos/TiposArchivos.h"

#define TDA_LISTA_IMPL_DINAMICA_VEC

#define TODO_OK 0
#define FACINCRE 2
#define CAP_INI 10
#define ERR_ARCHIVO -1
#define SIN_MEM -2



typedef struct
{
    void * vec;
    size_t ce;
    size_t tamElem;
    size_t cap;
}Vector;

typedef struct
{
	Vector* vec;
	void* act;
	void* ult;
	bool fin;
}
VectorIterador;


//puntero a funcion
typedef int (*Cmp)(const void *a,const void *b);

// funciones agregadas
bool crearVector_ALU(Vector *v, size_t tamElem);
void vectorDestruir_ALU(Vector *v);
bool vectorInsertarAlFinal_ALU(Vector *v,const void *elem);
bool redimensionar_ALU(Vector *v,float factor);
int vectorBuscarOrd(Vector *v,void *elem,Cmp cmp);
int cmpCliente_ALU(const void *a,const void *b);
void intercambiar_ALU(void *elem,void *act);
void *buscarMayor_ALU(Vector *v,void *pos,Cmp cmp);
void OrdenarSeleccion_ALU(Vector *v,Cmp cmp);
int cmpBoni_ALU(const void*a,const void*b);

bool finVecIterador_ALU(VectorIterador *it);
void *siguienteVecIterador_ALU(VectorIterador *it);
void *primerVecIterador_ALU(VectorIterador *it);
void crearVecIterador_ALU(Vector *v,VectorIterador *it);


///PARCIAL
int bonificarClientes_ALU(const char* clientes, const char* clientesInd, const char* ventas);
int listarClientesOrdXBonifDesc_ALU(const char* clientes);
int main()
{
    generarArchivoClientes();
    generarIndiceArchivoClientes();
    generarArchivoVentas();

    mostrarArchivoClientes();
    puts("\n");

    mostrarArchivoVentas();
    puts("\n");


    bonificarClientes_ALU("Clientes.dat", "Clientes.idx", "Ventas.dat");
    //bonificarClientes("Clientes.dat", "Clientes.idx", "Ventas.dat");

	puts("Clientes Bonificados");
	mostrarArchivoClientes();
	puts("\n");

    listarClientesOrdXBonifDesc_ALU("Clientes.dat");
    //listarClientesOrdXBonifDesc("Clientes.dat");

    return 0;
}


int bonificarClientes_ALU(const char* clientes, const char* clientesInd, const char* ventas)
{
/// Inserte el codigo aca ...
	FILE *archCli = fopen(clientes,"r+b");
	if(!archCli)
	{
		return ERR_ARCHIVO;
	}
	FILE *archInd = fopen (clientesInd,"rb");
	if(!archInd)
	{
		fclose(archCli);
		return ERR_ARCHIVO;
	}
	FILE *archVen = fopen(ventas,"rb");
	if(!archVen)
	{
		fclose(archCli);
		fclose(archInd);
		return ERR_ARCHIVO;
	}

	Vector vecInd;
	Indice indice;

	if(!crearVector_ALU(&vecInd,sizeof(Indice)))
	{
		fclose(archVen);
		fclose(archCli);
		fclose(archInd);
		return SIN_MEM;
	}
	int ret = TODO_OK;

	while(fread(&indice,sizeof(Indice),1,archVen) && ret == TODO_OK) // tda indice
	{
		if(!vectorInsertarAlFinal_ALU(&vecInd,&indice))
			ret = SIN_MEM;
	}

	if(ret==SIN_MEM)
	{
		vectorDestruir_ALU(&vecInd);
		fclose(archVen);
		fclose(archCli);
		fclose(archInd);
		return SIN_MEM;
	}

	float Tprecio;
	Venta venta;
	int band;
	Cliente cliente;

	while(fread(&venta,sizeof(Venta),1,archVen))
	{
		Tprecio=0;
		strcpy(indice.codCliente,venta.codCliente);
		band = vectorBuscarOrd(&vecInd,&indice,cmpCliente_ALU);

		if(band ==1)
		{
			fseek(archCli,indice.nroReg *sizeof(Cliente),SEEK_SET);
			fread(&cliente,sizeof(Cliente),1,archCli);


			while(fread(&venta,sizeof(Venta),1,archVen) && strcmpi(indice.codCliente,venta.codCliente)==0)
			{
				Tprecio += venta.precioUnit * venta.cantidad;
			}

			if(Tprecio>=300000)
				cliente.porcBonif = 0.20;
			if(Tprecio>=200000 && Tprecio<=300000)
				cliente.porcBonif = 0.10;
			if(Tprecio>=100000 && Tprecio<=200000)
				cliente.porcBonif = 0.05;

			if(cliente.porcBonif>0)
			{
				fseek(archCli,-(long)sizeof(Cliente),SEEK_CUR);
				fwrite(&cliente,sizeof(Cliente),1,archCli);
				fflush(archCli); // no necesario
			}
		}
		else
			printf("\nno encontramos el codigo del cliente %s",venta.codCliente);
	}

	vectorDestruir_ALU(&vecInd);
	fclose(archVen);
	fclose(archCli);
	fclose(archInd);
	return TODO_OK;
}


int listarClientesOrdXBonifDesc_ALU(const char* clientes)
{
/// Inserte el codigo aca ...
	FILE *arch = fopen(clientes,"rb");
	if(!arch)
	{
		return ERR_ARCHIVO;
	}

	Vector vecCli;

	if(!crearVector_ALU(&vecCli,sizeof(Cliente)))
	{
		fclose(arch);
		return SIN_MEM;
	}


	Cliente cliente;
	int ret = TODO_OK;

	while(fread(&cliente,sizeof(Cliente),1,arch) && ret==TODO_OK)
	{
		if(!vectorInsertarAlFinal_ALU(&vecCli,&cliente))
			ret = SIN_MEM;
	}

	if(ret == SIN_MEM)
	{
		vectorDestruir_ALU(&vecCli);
		fclose(arch);
		return SIN_MEM;
	}

	OrdenarSeleccion_ALU(&vecCli,cmpBoni_ALU);

	VectorIterador it;


	crearVecIterador_ALU(&vecCli,&it);

	Cliente *cli = primerVecIterador_ALU(&it);

	while(!finVecIterador_ALU(&it))
	{
		printf("%15s  %16s  %0.3f\n",cli->codigo,cli->nombre,cli->porcBonif);

		cli = siguienteVecIterador_ALU(&it);
	}

	vectorDestruir_ALU(&vecCli);
	fclose(arch);
	return TODO_OK;
}

void OrdenarSeleccion_ALU(Vector *v,Cmp cmp)
{
	void *ult = v->vec + (v->ce-1) *v->tamElem;
	void * elem;

	for(void *i=v->vec ; i<=ult ; i+=v->tamElem)
	{
		elem = buscarMayor_ALU(v,i,cmp);
		intercambiar_ALU(elem,i);
	}
}

void *buscarMayor_ALU(Vector *v,void *pos,Cmp cmp)
{
	void *ult = v->vec + (v->ce-1) *v->tamElem;
	void * elem = pos;

	for(void *i = pos; i<ult ; i+=v->tamElem) // i <=ult ??
	{
		if(cmp(i,i+v->tamElem)>0)
		{
			elem = i;
		}
	}

	return elem;
}

int cmpBoni_ALU(const void*a,const void*b)
{
	const Cliente *a1 = a;
	const Cliente *a2 = b;

	return a1->porcBonif -a2->porcBonif;
}



void intercambiar_ALU(void *elem,void *act)
{
	Cliente aux;

	memcpy(&aux,elem,sizeof(Cliente));
	memcpy(elem,act,sizeof(Cliente));
	memcpy(act,&aux,sizeof(Cliente));

}

int vectorBuscarOrd(Vector *v,void *elem,Cmp cmp)
{
	if(v->ce==0)
		return -1;

	const void *li= v->vec;
	const void *ls = v->vec + (v->ce-1) *v->tamElem;
	const void *m = li + ((ls-li) /v->tamElem /2) *v->tamElem;
	int comp =cmp(elem,m);

	while (li<=ls && comp !=0)
	{
		if(comp<0)
			ls = m - v->tamElem;
		if(comp>0)
			li = m + v->tamElem;

		m = li + ((ls-li) /v->tamElem /2) *v->tamElem;
		comp =cmp(elem,m);
	}

	if(comp==0)
	{
		memcpy(elem,m,v->tamElem);
		return 1;
	}
	return -1;
}
int cmpCliente_ALU(const void *a,const void *b)
{
	const Cliente *c1 =a;
	const Cliente *c2 =b;

	return strcmpi(c1->nombre,c2->nombre);
}

void crearVecIterador_ALU(Vector *v,VectorIterador *it)
{
	it->vec = v;
	it->act = NULL;
	it->ult = NULL;
	it->fin = true;
}

void *primerVecIterador_ALU(VectorIterador *it)
{
	Vector *v = it->vec;

	if(v->ce==0)
		return NULL;

	it->act = v->vec;
	it->ult = v->vec + (v->ce-1) *v->tamElem;
	it->fin = false;

	return it->act;
}


void *siguienteVecIterador_ALU(VectorIterador *it)
{
	void *sig = it->act + it->vec->tamElem;

	if(sig>it->ult)
	{
		it->fin = true;
		return NULL;
	}

	it->act = sig;
	return it->act;
}


bool finVecIterador_ALU(VectorIterador *it)
{
	return it->fin;
}

bool vectorInsertarAlFinal_ALU(Vector *v,const void *elem)
{
	if(v->ce == v->cap)
	{
		if(!redimensionar_ALU(v,FACINCRE))
		{
			return false;
		}
	}

	if(v->ce ==0)
	{
		memcpy(v->vec,elem,v->tamElem);
		v->ce ++;
		return true;
	}

	void *ult = v->vec + (v->ce) *v->tamElem;

	memcpy(ult,elem,v->tamElem);
	v->ce++;

	return true;
}


bool crearVector_ALU(Vector *v, size_t tamElem)
{
	v->ce=0;
	v->vec = malloc(CAP_INI * tamElem);
	if(!v->vec)
	{
		return false;
	}
	v->tamElem = tamElem;
	v->cap = CAP_INI;
	return true;
}


void vectorDestruir_ALU(Vector *v)
{
	v->ce =0;
	free(v->vec);
	v->vec = NULL;
	v->cap=0;
	v->tamElem=0;
}

bool redimensionar_ALU(Vector *v,float factor)
{
	size_t ncap = factor *v->cap;
	void *nvec = realloc(v->vec,ncap*v->tamElem);
	if(!nvec)
	{
		return false;
	}

	v->vec = nvec;
	v->cap = ncap;

	return true;

}

















