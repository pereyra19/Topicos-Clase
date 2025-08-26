/**********************************************************************************************************************
******************************* Complete sus datos ********************************************************************
***********************************************************************************************************************
* Apellido, Nombres: roberto pereyra
*
* DNI:
*
***********************************************************************************************************************
 * Correcci�n
***************

***********************************************************************************************************************
 * Nota:
**********************************************************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../Matriz/Matriz.h"
#include "../Vector/Vector.h"
#include "../SolucionMatrizDepositos/SolucionMatrizDepositos.h"

#define ARG_STOCKS 1
#define ARG_MOVIMIENTOS 2


#define CAP_INI 50
#define FAC_INCR 2
#define TAMLINEA 500
#define ERR_LINEA -10

#define DEPORIGEN 10
#define DEPDESTINO 10
#define CANTIDAD 6

#define ES_LETRA(c) ( (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z')) )
#define ES_NUM(c)   ( ((c) >= '0' && (c) <= '9') )

int actualizarDepositos_ALU(const char *nomArchStock, const char *nomArchMovi);
//tda vector
void destruirVector_ALU(Vector *v);
bool crearVector_ALU(Vector *v, size_t tamElem);
bool redimensionar_ALU(Vector *v,float factor);
bool insertarVecOrd(Vector *v,const void *elem,Cmp cmp);
int buscarVecOrd(Vector *v,void *elem,Cmp cmp);

int cmpCodDep_ALU(const void *a,const void *b);
int cmpCodStock_ALU(const void *a,const void *b);
void normalizar_ALU(char *s1);
//matriz
void inicializarMatrizEnCero_ALU(int **mat,size_t filas,size_t columnas);
void **crearMatriz_ALU(size_t filas,size_t columnas,size_t tamElem);
void destruirMatriz_ALU(void **mat, size_t filas);

// puntero
typedef int(*Cmp)(const void *a,const void *b);



int main(int argc, char* argv[])
{
	generarStocks(argv[ARG_STOCKS]);
	generarMovimientos(argv[ARG_MOVIMIENTOS]);

	puts("Stocks antes de actualizar:");
	mostrarStocks(argv[ARG_STOCKS]);

/*********************************************************************************************************************/
	actualizarDepositos_ALU(argv[ARG_STOCKS], argv[ARG_MOVIMIENTOS]);
/************* Descomente la funci�n de arriba y comente la de abajo para ejecutar su c�digo. ************************/
	//actualizarDepositos(argv[ARG_STOCKS], argv[ARG_MOVIMIENTOS]);
/*********************************************************************************************************************/

	puts("\nStocks despues de actualizar:");
	mostrarStocks(argv[ARG_STOCKS]);

	return 0;
}



int actualizarDepositos_ALU(const char *nomArchStock, const char *nomArchMovi)
{
	FILE *archStock = fopen(nomArchStock,"r+b");
	if(!archStock)
	{
		return ERR_ARCHIVO;
	}
	FILE *archMov = fopen(nomArchMovi,"rt");
	if(!archMov)
	{
		fclose(archStock);
		return ERR_ARCHIVO;
	}

	Vector vecStock;

	if(!crearVector_ALU(&vecStock,sizeof(Stock)))
	{
		fclose(archMov);
		fclose(archStock);
		return SIN_MEM;
	}

	Vector vecDep;

	if(!crearVector_ALU(&vecDep,sizeof(CodigoDeposito)))
	{
		destruirVector_ALU(&vecStock);
		fclose(archMov);
		fclose(archStock);
		return SIN_MEM;
	}

	Stock stock;
	CodigoDeposito codDep;
	int ret = TODO_OK;

	while(fread(&stock,sizeof(Stock),1,archStock) && ret == TODO_OK)
	{
		if(!insertarVecOrd(&vecStock,&stock,cmpCodStock_ALU))
			ret = SIN_MEM;

		if(ret == TODO_OK)
		{
			strcpy(codDep.codDeposito,stock.codDeposito);
			if(!insertarVecOrd(&vecDep,&codDep,cmpCodDep_ALU))
				ret = SIN_MEM;
		}
	}

	if(ret == SIN_MEM)
	{
		destruirVector_ALU(&vecDep);
		destruirVector_ALU(&vecStock);
		fclose(archMov);
		fclose(archStock);
		return SIN_MEM;
	}

	int orden = vecDep.ce;
	CodigoDeposito destino;
	CodigoDeposito origen;
	int cantidad;
	int **mat = (int**)crearMatriz_ALU(orden,orden,sizeof(int));
	if(mat == NULL)
	{
		destruirVector_ALU(&vecDep);
		destruirVector_ALU(&vecStock);
		fclose(archMov);
		fclose(archStock);
		return SIN_MEM;
	}

	inicializarMatrizEnCero_ALU(mat,orden,orden);

	// cargo la matriz para desp actualizar el vectorStock
	char linea[TAMLINEA+1];
	char *act;

	while(fgets(linea,TAMLINEA+1,archMov) && ret == TODO_OK)
	{
		act = strrchr(linea,'\n');
		if(act)
		{
			//cantidad
			*act = '\0';
			act -= CANTIDAD;
			normalizar_ALU(act);
			cantidad = atoi(act);
			// destino
			*act = '\0';
			act -=DEPDESTINO;
			normalizar_ALU(act);
			strcpy(destino.codDeposito,act);
			// origen
			*act = '\0';
			normalizar_ALU(linea);
			strcpy(origen.codDeposito,linea);

			int f = buscarVecOrd(&vecDep,&origen,cmpCodDep_ALU);
			int c = buscarVecOrd(&vecDep,&destino,cmpCodDep_ALU);

			if(f!=-1 && c!= -1)
			{
				mat[f][c] += cantidad;
			}
		}
		else
			ret = ERR_LINEA;
	}

	if(ret == ERR_LINEA)
	{
		destruirMatriz_ALU((void**)mat,orden);
		destruirVector_ALU(&vecDep);
		destruirVector_ALU(&vecStock);
		fclose(archMov);
		fclose(archStock);
		return ERR_LINEA;
	}

	Stock *elem = vecStock.vec;
	int cant;

	for(int i=0 ; i< orden ; i++)
	{
		cant =0;
		for(int j=0 ; j< orden ; j++)
		{
			cant += mat[j][i] - mat[i][j];
		}
		elem->stock += cant;
		if(elem->stock < 0)
			elem->stock = 0;
		elem ++;
	}

	fseek(archStock, 0L, SEEK_SET);
	fwrite(vecStock.vec,sizeof(Stock),vecStock.ce,archStock);
	fflush(archStock);

	destruirMatriz_ALU((void**)mat,orden);
	destruirVector_ALU(&vecDep);
	destruirVector_ALU(&vecStock);
	fclose(archMov);
	fclose(archStock);
	return TODO_OK;
}

int buscarVecOrd(Vector *v,void *elem,Cmp cmp)
{
	if(v->ce==0)
		return -1;

	const void *li = v->vec;
	const void *ls = v->vec + (v->ce-1) *v->tamElem;
	const void *m = li + ((ls-li) / v->tamElem / 2) * v->tamElem;
	int comp = cmp(elem,m);

	while(li<=ls && comp!=0)
	{
		if(comp<0)
		{
			ls = m - v->tamElem;
		}
		if(comp>0)
		{
			li = m + v->tamElem;
		}

		m = li + ((ls-li) / v->tamElem / 2) * v->tamElem;
		comp = cmp(elem,m);
	}

	if(comp==0)
	{
		return (m - v->vec) / v->tamElem;
	}

	return -1;
}

void inicializarMatrizEnCero_ALU(int **mat,size_t filas,size_t columnas)
{
	for(int i=0; i < filas ; i++)
	{
		for(int j=0 ; j< columnas; j++)
		{
			mat[i][j] = 0;
		}
	}
}

void **crearMatriz_ALU(size_t filas,size_t columnas,size_t tamElem)
{
	void **mat = malloc(filas * sizeof(void*));
	if(!mat)
	{
		return NULL;
	}

	void **ult = mat + filas -1;

	for(void **i = mat ; i<= ult ; i++)
	{
		*i = malloc(columnas * tamElem);

		if(*i== NULL)
		{
			destruirMatriz_ALU(mat,i - mat);
			return NULL;
		}
	}

	return mat;
}

void destruirMatriz_ALU(void **mat, size_t filas)
{
	void **ult = mat + filas -1;

	for(void **i = mat ; i<= ult ; i++)
	{
		free(*i);
	}
	free(mat);
}

bool insertarVecOrd(Vector *v,const void *elem,Cmp cmp)
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
	void *ult = v->vec + (v->ce-1) *v->tamElem;

	while(act<=ult && cmp(elem,act)>0)
	{
		act += v->tamElem;
	}

	if(cmp(elem,act)==0)
	{
		// actualizar
		return true;
	}

	void *ins = act;

	for(act = ult ; act >= ins ; act-=v->tamElem)
	{
		memcpy(act+v->tamElem, act, v->tamElem);
	}

	memcpy(ins,elem,v->tamElem);
	v->ce++;

	return true;
}

int cmpCodStock_ALU(const void *a,const void *b)
{
	const Stock *a1 = a;
	const Stock *b1 = b;

	return strcmp(a1->codDeposito,b1->codDeposito);
}
int cmpCodDep_ALU(const void *a,const void *b)
{
	const CodigoDeposito *a1 =a;
	const CodigoDeposito *b1 =b;

	return strcmp(a1->codDeposito,b1->codDeposito);
}
bool redimensionar_ALU(Vector *v,float factor)
{
	size_t ncap = factor *v->cap;
	void *nvec = realloc(v->vec, ncap *v->tamElem);
	if(!nvec)
	{
		return false;
	}
	v->vec = nvec;
	v->cap = ncap;
	return true;
}


bool crearVector_ALU(Vector *v, size_t tamElem)
{
	v->ce =0;
	v->vec = malloc(tamElem *CAP_INI);
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
	v->cap =0;
}

void normalizar_ALU(char *s1)
{
	char aux[strlen(s1)+1];
	int firts =1, npalabra =0;
	int i=0, j=0;

	while(s1[i]!='\0')
	{
		if(ES_LETRA(s1[i]) || ES_NUM(s1[i]))
		{
			if(firts)
			{
				firts=0;
				aux[j++] = toupper(s1[i]);
				npalabra = 1;
			}
			else
			{
				aux[j++] = tolower(s1[i]);
			}
			i++;
		}
		else
		{
			if(npalabra)
			{
				firts=1;
				npalabra =0;
			}
			i++;
		}
	}

	aux[j] = '\0';

	if(aux[j-1]== ' ')
		aux[j-1] = '\0';

	strcpy(s1,aux);
}























