/***************************************************************************************
******************************* Complete sus datos *************************************
****************************************************************************************
* Apellido, Nombres: roberto pereyra
*
* DNI: 
***************************************************************************************/

/************************************ Corrección ***************************************
****************************************************************************************

****************************************************************************************
* Nota:
***************************************************************************************/

#include <stdio.h>
#include "../Vector/Vector.h"
#include "../SolucionParcialEcommerceTopicos/SolucionParcialEcommerce.h"
#include <string.h>
#include <stdlib.h>

#define ARG_CLIENTES    1
#define ARG_PRODUCTOS   2
#define ARG_REGLAS      3
#define ARG_OFERTAS     4

#define CAP_INI 50
#define FAC_INCR 2
#define ERR_LINEA -44

#define TAMLINEA 250

int generarArchivoOfertas_ALU(const char* nombreArchClientes, const char* nombreArchProductos, const char* nombreArchReglas, const char* nombreArchOfertas);
int generarArchivoOfertasTxt_ALU(const char* nombreArchivoOfertas, const char* nombreArchivoOfertasTxt);

//funciones agregadas
bool redimensionar_ALU(Vector *v,float factor);
bool vectorInsertarAlFinal_ALU(Vector *v,void *elem);
void vectorDestruir_ALU(Vector* v);
bool vectorCrear_ALU(Vector* v, size_t tamReg);
int vectorOrdBuscar_ALU(Vector *v,void *elem,Cmp cmp);
int cmpRegla_ALU(const void* e1, const void* e2);

// puntero a funcion
typedef int (*Cmp)(const void* e1, const void* e2);


int main(int argc, char* argv[])
{
    generarArchivoClientes(argv[ARG_CLIENTES], false);
    generarArchivoProductos(argv[ARG_PRODUCTOS], false);
    generarArchivoReglas(argv[ARG_REGLAS]);

    //generarArchivoOfertas(argv[ARG_CLIENTES], argv[ARG_PRODUCTOS], argv[ARG_REGLAS], argv[ARG_OFERTAS]);
    generarArchivoOfertas_ALU(argv[ARG_CLIENTES], argv[ARG_PRODUCTOS], argv[ARG_REGLAS], argv[ARG_OFERTAS]);

    if(mostrarArchivoOfertas(argv[ARG_OFERTAS]) != TODO_OK)
        printf("\n\nError al mostrar el archivo de ofertas.\n");

    char nombreArchivoOfertasTxt[251];
    cambiarExtension(argv[ARG_OFERTAS], nombreArchivoOfertasTxt, ".txt");

    //generarArchivoOfertasTxt(argv[ARG_OFERTAS], nombreArchivoOfertasTxt);
    generarArchivoOfertasTxt_ALU(argv[ARG_OFERTAS], nombreArchivoOfertasTxt);

    if(mostrarArchivoOfertasTxt(nombreArchivoOfertasTxt))
        printf("\n\nError al mostrar el archivo de ofertas txt.\n");

    return 0;
}


int generarArchivoOfertas_ALU(const char* nombreArchClientes, const char* nombreArchProductos, const char* nombreArchReglas, const char* nombreArchOfertas)
{
	FILE *archCli = fopen(nombreArchClientes,"rb");
	if(!archCli)
	{
		return ERR_ARCHIVO;
	}
	FILE *archPro = fopen(nombreArchProductos,"rb");
	if(!archPro)
	{
		fclose(archCli);
		return ERR_ARCHIVO;
	}
	FILE *archReg = fopen(nombreArchReglas,"rt");
	if(!archReg)
	{
		fclose(archCli);
		fclose(archPro);
		return ERR_ARCHIVO;
	}
	FILE *archOfer = fopen(nombreArchOfertas,"wb");
	if(!archOfer)
	{
		fclose(archReg);
		fclose(archCli);
		fclose(archPro);
		return ERR_ARCHIVO;
	}


	Vector vecReg; // TDA REGLA

	if(!vectorCrear_ALU(&vecReg,sizeof(Regla)))
	{
		fclose(archOfer);
		fclose(archReg);
		fclose(archCli);
		fclose(archPro);
		return SIN_MEM;
	}

	char linea[TAMLINEA+1];
	int ret = TODO_OK;
	char *act;
	Regla regla;

	fgets(linea,TAMLINEA+1,archReg); // para leer los campos

	while(fgets(linea,TAMLINEA+1,archReg) && ret == TODO_OK)
	{
		act = strrchr(linea,'\n');
		if(act)
		{
			// envio gratis
			*act = '\0';
			act = strrchr(linea,'\t');
			regla.minimoParaEnvioGratis = atof(act+1);
						// tope descuento
			*act = '\0';
			act = strrchr(linea,'\t');
			regla.topeDescuento = atof(act+1);
						// descuento
			*act = '\0';
			act = strrchr(linea,'\t');
			regla.porcDescuento = atof(act+1);
						// nivel
			*act = '\0';
			regla.nivel = atoi(linea);

			if(!vectorInsertarAlFinal_ALU(&vecReg,&regla))
				ret = SIN_MEM;
		}
		else
			ret = ERR_LINEA;
	}

	if(ret != TODO_OK)
	{
		vectorDestruir_ALU(&vecReg);
		fclose(archOfer);
		fclose(archReg);
		fclose(archCli);
		fclose(archPro);
		return ret;
	}

	Cliente cliente;
	int band;
	Oferta oferta;
	Producto producto;
	float descuento;

	while(fread(&cliente,sizeof(Cliente),1,archCli))
	{
		regla.nivel = cliente.nivel;
		band = vectorOrdBuscar_ALU(&vecReg,&regla,cmpRegla_ALU);
		strcpy(oferta.usuario,cliente.usuario);
		if(band==1)
		{
			rewind(archPro);
			while(fread(&producto,sizeof(Producto),1,archPro))
			{
				strcpy(oferta.producto,producto.codigo);

				descuento = producto.precio * (regla.porcDescuento / 100);
				if(descuento>regla.topeDescuento)
					descuento = regla.topeDescuento;
				oferta.precio = producto.precio - descuento;

				oferta.envioGratis = oferta.precio >= regla.minimoParaEnvioGratis ? true : false;

				fwrite(&oferta,sizeof(Oferta),1,archOfer);
			}
		}
		else
			printf("\nno se encontro regla para el cliente %s",cliente.nombre);
	}

	vectorDestruir_ALU(&vecReg);
	fclose(archOfer);
	fclose(archReg);
	fclose(archCli);
	fclose(archPro);
    return TODO_OK;
}

int generarArchivoOfertasTxt_ALU(const char* nombreArchivoOfertas, const char* nombreArchivoOfertasTxt)
{
	FILE *archBin = fopen(nombreArchivoOfertas,"rb");
	if(!archBin)
	{
		return ERR_ARCHIVO;
	}
	FILE *archTxt = fopen(nombreArchivoOfertasTxt,"wt");
	if(!archTxt)
	{
		fclose(archBin);
		return ERR_ARCHIVO;
	}

	Oferta oferta;
	char anterior[21];

	while(fread(&oferta,sizeof(Oferta),1,archBin))
	{
		strcpy(anterior,oferta.usuario);
		fprintf(archTxt,"%-21s\n",oferta.usuario);
		fprintf(archTxt,"      %-16s  %-10.2f     %s\n",oferta.producto,oferta.precio,oferta.envioGratis? "si":"no");
		while(fread(&oferta,sizeof(Oferta),1,archBin) && strcmp(anterior,oferta.usuario)==0)
		{
			fprintf(archTxt,"      %-16s  %-10.2f     %s\n",oferta.producto,oferta.precio,oferta.envioGratis? "si":"no");
		}
	}


	fclose(archTxt);
	fclose(archBin);
    return TODO_OK;
}


int vectorOrdBuscar_ALU(Vector *v,void *elem,Cmp cmp)
{
	if(v->ce ==0)
		return -1;

	const void *li = v->vec;
	const void *ls = v->vec + (v->ce-1) *v->tamElem;
	const void *m = li + ((ls-li) / v->tamElem / 2) *v->tamElem;
	int comp = cmp(elem,m);

	while(li<=ls && comp!=0)
	{
		if(comp<0)
			ls = m - v->tamElem;
		if(comp>0)
			li = m+ v->tamElem;

		m = li + ((ls-li) / v->tamElem / 2) *v->tamElem;
		comp = cmp(elem,m);
	}

	if(comp==0)
	{
		memcpy(elem,m,v->tamElem);
		return 1;
	}

	return -1;
}

int cmpRegla_ALU(const void* e1, const void* e2)
{
	const Regla *a = e1;
	const Regla *b = e2;

	return a->nivel - b->nivel;
}


bool vectorInsertarAlFinal_ALU(Vector *v,void *elem)
{
	if(v->ce == v->cap)
	{
		if(!redimensionar_ALU(v,FAC_INCR))
			return false;

	}

	if(v->ce ==0)
	{
		memcpy(v->vec,elem,v->tamElem);
		v->ce ++;
		return true;
	}

	void *ult = v->vec + (v->ce) *v->tamElem;

	memcpy(ult,elem,v->tamElem);
	v->ce ++;

	return true;
}

bool redimensionar_ALU(Vector *v,float factor)
{
	size_t ncap = v->cap *factor;
	void *nvec = realloc(v->vec,ncap *v->tamElem);
	if(!nvec)
	{
		return false;
	}

	v->vec = nvec;
	v->cap = ncap;

	return true;
}

bool vectorCrear_ALU(Vector* v, size_t tamReg)
{
	v->ce =0;
	v->vec = malloc(tamReg *CAP_INI);
	if(!v->vec)
	{
		return false;
	}
	v->cap = CAP_INI;
	v->tamElem = tamReg;
    return true;
}

void vectorDestruir_ALU(Vector* v)
{
	v->ce =0;
	free(v->vec);
	v->vec = NULL;
	v->tamElem =0;
	v->cap=0;
}



