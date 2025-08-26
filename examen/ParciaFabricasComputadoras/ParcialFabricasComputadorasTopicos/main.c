/*****************************************************************************************************************
** Apellido/s, Nombre/s: pereyra roberto                              													**
** DNI:    42251380                                                   													**
******************************************************************************************************************
** Corrección																								   	**
** **********																								   	**
**                      																					   	**
******************************************************************************************************************
** Nota:																									   	**
*****************************************************************************************************************/

#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define TDA_LISTA_IMPL_DINAMICA
#include "../Comun/Comun.h"
#include "../Cadena/Cadena.h"
#include "../Vector/Vector.h"
#include "../ArchivoTopicos/Archivo.h"
#include "../SolucionParcialFabricasComputadorasTopicos/SolucionParcialFabricasComputadorasTopicos.h"

#define PROD_COR "Productos-cor.dat"
#define PROD_CHU_TXT "Productos-chu.txt"
#define CLI_COR "Clientes-cor.dat"
#define CLI_CHU "Clientes-chu.dat"
#define PROD_FUS "Productos-fus.dat"
#define CLI_FUS "Clientes-fus.dat"


#define TAMLINEA 500
#define ERR_LINEA -10
#define CAP_INI 50
#define FAC_INCR 2



// funciones agregadas
bool redimensionar_ALU(Vector *v,float factor);
void destruirVector_ALU(Vector *v);
bool crearVector_ALU(Vector *v,size_t tamElem);
bool insertarVecAlFinal_ALU(Vector *v,const void *elem);
bool fusionarArchivos_ALU(FILE *arch1,FILE *arch2, FILE *archFus,size_t tamElem,Cmp cmp);
void *buscarMenor_ALU(Vector *v,void *pos,Cmp cmp);
bool ordenarSeleccion_ALU(Vector *v,Cmp cmp);

bool eliminarDuplicados_ALU(FILE *arch,size_t tamElem,Cmp cmp,Actualizar actualizar);
bool insertarVecOrd_ALU(Vector *v,const void *elem,Cmp cmp,Actualizar actualizar);

int cmpPro_ALU(const void *a,const void *b);
int cmpCli_ALU(const void *a,const void *b);
void actualizarCli_ALU(void *destino,const void *origen);
void actualizarPro_ALU(void *destino,const void *origen);


//puntero a funcion
typedef int (*Cmp)(const void *a,const void *b);
typedef void (*Actualizar)(void* actualizado, const void* actualizador);

int procesarArchivos_ALU(
	const char* nombreArchivoProductosCor, const char* nombreArchivoProductosChu,
	const char* nombreArchivoClientesCor, const char* nombreArchivoClientesChu,
	const char* nombreArchivoProductosFus, const char* nombreArchivoClientesFus);


int main()
{
    int resp = generarArchivos();

    if(resp != TODO_OK)
    {
        printf("Error al generar los archivos");
        return resp;
    }

    puts("Archivos Pre Fusion:\n");

    puts(PROD_COR);
    puts("=========================================");
    mostrarArchivoProductosBin(PROD_COR);

    puts("\n");

    puts(PROD_CHU_TXT);
    puts("=========================================");
    mostrarArchivoProductosTxt(PROD_CHU_TXT);

    puts("\n");

    puts(CLI_COR);
    puts("=========================================");
    mostrarArchivoClientes(CLI_COR);

    puts("\n");

    puts(CLI_CHU);
    puts("=========================================");
    mostrarArchivoClientes(CLI_CHU);



	//resp = procesarArchivos(PROD_COR, PROD_CHU_TXT, CLI_COR, CLI_CHU, PROD_FUS, CLI_FUS);
	resp = procesarArchivos_ALU(PROD_COR, PROD_CHU_TXT, CLI_COR, CLI_CHU, PROD_FUS, CLI_FUS);


    if(resp != TODO_OK)
    {
        printf("Error al procesar los archivos");
        return resp;
    }

    puts("\n\nArchivos Post Fusion:\n");

    puts(PROD_FUS);
    puts("=========================================");
    mostrarArchivoProductosBin(PROD_FUS);

    puts("\n");

    puts(CLI_FUS);
    puts("=========================================");
    mostrarArchivoClientes(CLI_FUS);

    return 0;
}

int procesarArchivos_ALU(
	const char* nombreArchivoProductosCor, const char* nombreArchivoProductosChu,
	const char* nombreArchivoClientesCor, const char* nombreArchivoClientesChu,
	const char* nombreArchivoProductosFus, const char* nombreArchivoClientesFus)
{
	FILE *archProCor = fopen(nombreArchivoProductosCor, "rb");
	if(!archProCor)
	{
		return ERR_ARCHIVO;
	}
	FILE *archTxt = fopen(nombreArchivoProductosChu,"rt");
	if(!archTxt)
	{
		fclose(archProCor);
		return ERR_ARCHIVO;
	}
	FILE *archCliCor = fopen(nombreArchivoClientesCor,"rb");
	if(!archCliCor)
	{
		fclose(archTxt);
		fclose(archProCor);
		return ERR_ARCHIVO;
	}
	FILE *archCliChu = fopen(nombreArchivoClientesChu,"rb");
	if(!archCliChu)
	{
		fclose(archTxt);
		fclose(archCliCor);
		fclose(archProCor);
		return ERR_ARCHIVO;
	}

	char texto[strlen(nombreArchivoProductosChu)+1];
	strcpy(texto,nombreArchivoProductosChu);
	char *lim = strrchr(texto,'.');
	*lim = '\0';
	strcat(texto,".dat");

	FILE *archProChu = fopen(texto,"w+b");
	if(!archProChu)
	{
		fclose(archCliChu);
		fclose(archTxt);
		fclose(archCliCor);
		fclose(archProCor);
		return ERR_ARCHIVO;
	}

	FILE *archFUSPro = fopen(nombreArchivoProductosFus,"w+b");
	if(!archFUSPro)
	{
		fclose(archProChu);
		fclose(archCliChu);
		fclose(archTxt);
		fclose(archCliCor);
		fclose(archProCor);
		return ERR_ARCHIVO;
	}

	FILE *archFUScli = fopen(nombreArchivoClientesFus,"w+b");
	if(!archFUScli)
	{
		fclose(archFUSPro);
		fclose(archProChu);
		fclose(archCliChu);
		fclose(archTxt);
		fclose(archCliCor);
		fclose(archProCor);
		return ERR_ARCHIVO;
	}

	char linea[TAMLINEA+1];
	int ret = TODO_OK;
	char *act;
	Producto producto;

	while(fgets(linea,TAMLINEA+1,archTxt) && ret == TODO_OK)
	{
		act = strrchr(linea,'\n');
		if(act)
		{
			//stock i
			*act = '\0';
			act = strrchr(linea,'|');
			producto.stock = atoi(act+1);
			//precio f
			*act = '\0';
			act = strrchr(linea,'|');
			producto.precio = atof(act+1);
			//descripcion c
			*act = '\0';
			act = strrchr(linea,'|');
			strcpy(producto.descripcion,act+1);
			//codigo c
			*act = '\0';
			strcpy(producto.codigo,linea);

			fwrite(&producto,sizeof(Producto),1,archProChu);
		}
		else
			ret = ERR_LINEA;
	}

	if(ret == ERR_LINEA)
	{
		fclose(archFUScli);
		fclose(archFUSPro);
		fclose(archProChu);
		fclose(archCliChu);
		fclose(archTxt);
		fclose(archCliCor);
		fclose(archProCor);
		return ERR_LINEA;
	}

	fseek(archProChu, 0L, SEEK_SET); // volvemos el puntero al principio

	if(!fusionarArchivos_ALU(archCliChu,archCliCor,archFUScli,sizeof(Cliente),cmpCli_ALU))
	{
		fclose(archProChu);
		fclose(archCliChu);
		fclose(archTxt);
		fclose(archCliCor);
		fclose(archProCor);
		fclose(archFUScli);
		fclose(archFUSPro);
		return SIN_MEM;
	}
	if(!fusionarArchivos_ALU(archProChu,archProCor,archFUSPro,sizeof(Producto),cmpPro_ALU))
	{
		fclose(archProChu);
		fclose(archCliChu);
		fclose(archTxt);
		fclose(archCliCor);
		fclose(archProCor);
		fclose(archFUScli);
		fclose(archFUSPro);
		return SIN_MEM;
	}
		// cierro los archivos no utilizados
	fclose(archProChu);
	fclose(archCliChu);
	fclose(archTxt);
	fclose(archCliCor);
	fclose(archProCor);


	if(!eliminarDuplicados_ALU(archFUScli,sizeof(Cliente),cmpCli_ALU,actualizarCli_ALU))
	{
		fclose(archFUScli);
		fclose(archFUSPro);
		return SIN_MEM;
	}

	if(!eliminarDuplicados_ALU(archFUSPro,sizeof(Producto),cmpPro_ALU,actualizarPro_ALU))
	{
		fclose(archFUScli);
		fclose(archFUSPro);
		return SIN_MEM;
	}


	fclose(archFUScli);
	fclose(archFUSPro);
	return TODO_OK;
}

bool eliminarDuplicados_ALU(FILE *arch,size_t tamElem,Cmp cmp,Actualizar actualizar)
{
	Vector vec;
	if(!crearVector_ALU(&vec,tamElem))
	{
		return false;
	}

	void *elem = malloc(tamElem);
	if(!elem)
	{
		destruirVector_ALU(&vec);
		return false;
	}

	int ret = TODO_OK;

	while(fread(elem,tamElem,1,arch)&& ret == TODO_OK)
	{
		if(!insertarVecOrd_ALU(&vec,elem,cmp,actualizar))
			ret = SIN_MEM;
	}

	if(ret == SIN_MEM)
	{
		free(elem);
		destruirVector_ALU(&vec);
		return false;
	}

	fseek(arch, 0L, SEEK_SET);
	fwrite(vec.vec,tamElem,vec.ce,arch);

	free(elem);
	destruirVector_ALU(&vec);
	return true;
}

bool insertarVecOrd_ALU(Vector *v,const void *elem,Cmp cmp,Actualizar actualizar)
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

	while(act<=ult && cmp(elem,act)>0)
	{
		act +=v->tamElem;
	}

	if(cmp(elem,act)==0)
	{
		actualizar(act,elem);
		return true;
	}

	void *ins = act;

	for(act = ult ; act>=ins; act-=v->tamElem)
	{
		memcpy(act+v->tamElem,act,v->tamElem);
	}

	memcpy(ins,elem,v->tamElem);
	v->ce++;

	return true;
}


void actualizarPro_ALU(void *destino,const void *origen)
{
	Producto *des = destino;
	const Producto *ori = origen;

	if(ori->precio > des->precio)
		des->precio = ori->precio;

	des->stock += ori->stock;
}

void actualizarCli_ALU(void *destino,const void *origen) // no hace nada
{

}

bool fusionarArchivos_ALU(FILE *arch1,FILE *arch2, FILE *archFus,size_t tamElem,Cmp cmp)
{
	Vector vec;

	if(!crearVector_ALU(&vec,tamElem))
	{
		return false;
	}

	void *reg = malloc(tamElem);
	if(!reg)
	{
		destruirVector_ALU(&vec);
		return false;
	}

	int ret = TODO_OK;
	while(fread(reg,tamElem,1,arch1) && ret ==TODO_OK)
	{
		if(!insertarVecAlFinal_ALU(&vec,reg))
			ret = SIN_MEM;
	}

	if(ret==SIN_MEM)
	{
		free(reg);
		destruirVector_ALU(&vec);
		return false;
	}

	while(fread(reg,tamElem,1,arch2) && ret ==TODO_OK)
	{
		if(!insertarVecAlFinal_ALU(&vec,reg))
			ret = SIN_MEM;
	}

	if(ret==SIN_MEM)
	{
		free(reg);
		destruirVector_ALU(&vec);
		return false;
	}

	if(!ordenarSeleccion_ALU(&vec,cmp))
	{
		free(reg);
		destruirVector_ALU(&vec);
		return false;
	}

	fwrite(vec.vec,tamElem,vec.ce,archFus);
	fseek(archFus, 0L, SEEK_SET);

	free(reg);
	destruirVector_ALU(&vec);
	return true;
}


bool ordenarSeleccion_ALU(Vector *v,Cmp cmp)
{
	void *ult = v->vec + (v->ce-1) * v->tamElem;
	void *elem;
	void *aux = malloc(v->tamElem);
	if(!aux)
	{
		return false;
	}

	for(void *i=v->vec; i<=ult ; i+=v->tamElem )
	{
		elem = buscarMenor_ALU(v,i,cmp);
		if(elem != i)
		{
			memcpy(aux,i,v->tamElem);
			memcpy(i,elem,v->tamElem);
			memcpy(elem,aux,v->tamElem);
		}
	}

	return true;
}



void *buscarMenor_ALU(Vector *v,void *pos,Cmp cmp)
{
	void *ult = v->vec + (v->ce-1) * v->tamElem;
	void *elem = pos;

	for(void *i=pos; i<=ult ; i+=v->tamElem )
	{
		if(cmp(i,elem)<0)
		{
			elem = i;
		}
	}

	return elem;
}

int cmpPro_ALU(const void *a,const void *b)
{
	const Producto *a1 =a;
	const Producto *b1 =b;

	return strcmp(a1->codigo,b1->codigo);
}
int cmpCli_ALU(const void *a,const void *b)
{
	const Cliente *a1=a;
	const Cliente *b1=b;

	return a1->codigo - b1->codigo;
}

bool insertarVecAlFinal_ALU(Vector *v,const void *elem)
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

	void *ult = v->vec + (v->ce) * v->tamElem;

	memcpy(ult,elem,v->tamElem);
	v->ce++;
	return true;
}

bool crearVector_ALU(Vector *v,size_t tamElem)
{
	v->ce =0;
	v->vec = malloc(tamElem * CAP_INI);
	if(!v->vec)
	{
		return false;
	}
	v->cap = CAP_INI;
	v->tamElem = tamElem;
	return true;
}

void destruirVector_ALU(Vector *v)
{
	v->ce =0;
	free(v->vec);
	v->vec = NULL;
	v->tamElem =0;
	v->cap=0;
}

bool redimensionar_ALU(Vector *v,float factor)
{
	size_t ncap = factor *v->cap;
	void *nvec = realloc(v->vec,ncap *v->tamElem);
	if(!nvec)
	{
		return false;
	}
	v->vec = nvec;
	v->cap = ncap;
	return true;
}



