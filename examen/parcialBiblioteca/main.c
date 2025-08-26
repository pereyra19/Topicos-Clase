/***************************************************************************************
******************************* Complete sus datos *************************************
****************************************************************************************
* Apellido, Nombres:pereyra roberto
*
* DNI:42251380
*
*
****************************************************************************************/
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Cadena.h>
#include <Comun.h>
#include <Palabra.h>
#include <SecPal.h>
#include <Vector.h>
#include <Libro.h>
#include <SolucionBiblioteca.h>
#include <ctype.h>


#define NOMBRE_ARCH_LIBROS "Libros.dat"
#define NOMBRE_ARCH_LIBROS_INDICE "Libros.idx"
#define NOMBRE_ARCH_MOVIMIENTOS "Movimientos.txt"


#define FAC_INCR 2
#define CAP_INI 50
#define TAMLINEA 500
#define ERR_LINEA -11
#define SIN_MEN -12

typedef struct
{
	Vector *vec;
	void *act;
	void *ult;
	bool finIter;
}
VectorIterador;

#define ES_LETRA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

#define ES_NUM(c) ((c) >= '0' && (c) <= '9')


//funciones agregadas

//tda
void vectorDestruir_ALU(Vector *v);
bool vectorCrear_ALU(Vector *v, size_t tamElem);
bool redimensionar_ALU(Vector *v, float factor);

void normalizarChar_ALU(char *s1);
bool normalizarNombresLibros_ALU(const char *nomArchLibro);
bool insertarVecOrd_ALU(Vector *v,const void *elem,Cmp cmp,Actualizar actualizar);
int buscarVecOrd_ALU(Vector *v,void *elem,Cmp cmp);
int actualizarArchivoLibros_ALU(const char *nomArchLibro,const char *nomArchIndice,Vector *vecMovs);
//
int cmpIndice_ALU(const void* e1, const void* e2);
void actualizarIndice_ALU(void* actualizado, const void* actualizador);
int cmpMovLibro_ALU(const void* e1, const void* e2);
void actualizarLibro_ALU(void* actualizado, const void* actualizador);

//vector iter
bool finVecIterador(VectorIterador *it);
void *siguienteVecIterador(VectorIterador *it);
void *primerVecIterador_ALU(VectorIterador *it);
void crearVectorIterador_ALU(Vector *v,VectorIterador *it);



//
int cargarVectorMovimientosLibro_ALU(Vector *vecMovs,const char *nomArchLibros);




int main()
{
	generarArchMovimientosLibro(NOMBRE_ARCH_MOVIMIENTOS);
	generarArchLibros(NOMBRE_ARCH_LIBROS, NOMBRE_ARCH_LIBROS_INDICE);

	mostrarLibros(NOMBRE_ARCH_LIBROS);

	Vector vecMovs;
    vectorCrear_ALU(&vecMovs, sizeof(MovLibro));
	//vectorCrear(&vecMovs, sizeof(MovLibro));

	///Debe generar el vector de movimientos, consolidando los movimientos de los libros, a fin de que quede 1 sólo elemento en el vector por cada libro.
	///Aumentando o disminuyendo la cantidad según se trate de una devolución o un préstamo respectivamente.
	///La primitiva vectorOrdInsertar recibe un parámetro adicional, puntero a función Actualizar, para realizar la actualización en caso de que ya exista el elemento.
	///Puede utilizar las funciones resueltas, pero luego deberá reemplazarlas por su propia versión de las mismas.
	cargarVectorMovimientosLibro_ALU(&vecMovs, NOMBRE_ARCH_MOVIMIENTOS);
	//cargarVectorMovimientosLibro(&vecMovs, NOMBRE_ARCH_MOVIMIENTOS);

	///Para test
	puts("Vector movimientos");
	puts("Cod. Libro | Cant.");
	vectorRecorrer(&vecMovs, mostrarMovimientoLibro, NULL);

	puts("");

	///Debe actualizar el archivo binario de Libros, sumando o restando la cantidad.
	///La cantidad en el archivo no puede quedar negativa. Si eso sucede, se deberá descartar el movimiento y continuar con el siguiente.
	///Para acceder a cada registro, debe hacer uso del archivo índice, que tiene cada registro, el código del libro y su ubicación en el archivo(Nro de registro empezando por el 0).
	///Debe cargar el índice en memoria, para trabajar con él.
	actualizarArchivoLibros_ALU(NOMBRE_ARCH_LIBROS, NOMBRE_ARCH_LIBROS_INDICE, &vecMovs);
	//actualizarArchivoLibros(NOMBRE_ARCH_LIBROS, NOMBRE_ARCH_LIBROS_INDICE, &vecMovs);

	puts("");

	///Debe normalizar los nombres de los libros en el archivo.
	///Dejando sólo 1 espacio entre palabras, sin espacios ni caracteres antes de la primer palábra y después de la última.
	///Quedando la primer letra de cada palabra en mayúscula y el resto en minúscula.
	normalizarNombresLibros_ALU(NOMBRE_ARCH_LIBROS);
	//normalizarNombresLibros(NOMBRE_ARCH_LIBROS);

	mostrarLibros(NOMBRE_ARCH_LIBROS);

	vectorDestruir_ALU(&vecMovs);
	//vectorDestruir(&vecMovs);

    return 0;
}

bool normalizarNombresLibros_ALU(const char *nomArchLibro)
{
	FILE *arch = fopen(nomArchLibro,"r+b");
	if(!arch)
	{
		return false;
	}

	Libro libro;

	while(fread(&libro,sizeof(Libro),1,arch))
	{
		normalizarChar_ALU(libro.nombre);
		normalizarChar_ALU(libro.codigo);

		fseek(arch,-(long)sizeof(Libro),SEEK_CUR);
		fwrite(&libro,sizeof(Libro),1,arch);
		fseek(arch, 0L, SEEK_CUR);
	}

	fclose(arch);
	return true;
}

void normalizarChar_ALU(char *s1)
{
	char nor[strlen(s1)+1];
	int firts=1, npalabra=0;
	int i=0, j=0;

	while (s1[i] != '\0' && !ES_LETRA(s1[i]) && !ES_NUM(s1[i]))
		i++;


	while(s1[i] !='\0')
	{
		if(ES_LETRA(s1[i]) || ES_NUM(s1[i]))
		{
			if(firts)
			{
				nor[j] = toupper(s1[i]);
				j++;
				firts=0;
				npalabra =1;
			}
			else
			{
				nor[j] = tolower(s1[i]);
				j++;
			}
			i++;
		}
		else
		{
			if(npalabra==1)
			{
				nor[j++] = ' ';
				npalabra = 0;
				firts = 1;
			}

			i++;
		}
	}

	nor[j] = '\0';

	if(nor[j-1] == ' ')
	{
		nor[j-1] = '\0';
	}


	strcpy(s1,nor);
}


int actualizarArchivoLibros_ALU(const char *nomArchLibro,const char *nomArchIndice,Vector *vecMovs)
{
	FILE *archlibro = fopen(nomArchLibro,"r+b");
	if(!archlibro)
	{
		return ERR_ARCHIVO;
	}
	FILE *archIndice = fopen(nomArchIndice,"rb");
	if(!archIndice)
	{
		fclose(archlibro);
		return ERR_ARCHIVO;
	}

	Vector vecInd;

	if(!vectorCrear_ALU(&vecInd,sizeof(IndLibro)))
	{
		fclose(archIndice);
		fclose(archlibro);
		return SIN_MEM;
	}

	IndLibro indice;
	int ret = TODO_OK;

	while(fread(&indice,sizeof(IndLibro),1,archIndice) && ret == TODO_OK)
	{
		if(!insertarVecOrd_ALU(&vecInd,&indice,cmpIndice_ALU,actualizarIndice_ALU))
			ret = SIN_MEM;
	}

	if(ret==SIN_MEM)
	{
		vectorDestruir_ALU(&vecInd);
		fclose(archIndice);
		fclose(archlibro);
		return SIN_MEM;
	}

	VectorIterador itInd;
	Libro libro;
	MovLibro movLibro;
	int band;
	crearVectorIterador_ALU(&vecInd,&itInd);

	IndLibro *act = (IndLibro*)primerVecIterador_ALU(&itInd);

	while(finVecIterador(&itInd))
	{
		fseek(archlibro,act->nroReg *sizeof(IndLibro),SEEK_SET);
		fread(&libro,sizeof(Libro),1,archlibro);

		strcpy(movLibro.codigoLibro,libro.codigo);
		band = buscarVecOrd_ALU(vecMovs,&movLibro,cmpMovLibro_ALU);

		if(band ==1)
		{
			libro.cantidad += movLibro.cantidad;

			if(libro.cantidad >=0)
			{
				fseek(archlibro,-(long)sizeof(IndLibro),SEEK_CUR);
				fwrite(&libro,sizeof(Libro),1,archlibro);
				fflush(archlibro); // no necesario
			}
		}
		act = siguienteVecIterador(&itInd);
	}

	vectorDestruir_ALU(&vecInd);
	fclose(archIndice);
	fclose(archlibro);
	return TODO_OK;
}

int buscarVecOrd_ALU(Vector *v,void *elem,Cmp cmp)
{
	if(v->ce ==0)
		return -1;

	const void *li = v->vec;
	const void *ls = v->vec + (v->ce-1) *v->tamElem;
	const void *m = li +((ls-li) / v->tamElem / 2) *v->tamElem;
	int comp = cmp(elem,m);

	while(li<=ls && comp!=0)
	{
		if(comp<0)
			ls = m - v->tamElem;
		if(comp>0)
			li = m + v->tamElem;

		m = li +((ls-li) / v->tamElem / 2) *v->tamElem;
		comp = cmp(elem,m);
	}

	if(comp==0)
	{
		memcpy(elem,m,v->tamElem);
		return 1;
	}

	return -1;
}

int cmpIndice_ALU(const void* e1, const void* e2)
{
	const IndLibro * a = e1;
	const IndLibro * b = e2;

	return strcmpi(a->codigo,b->codigo);

}
void actualizarIndice_ALU(void* actualizado, const void* actualizador) // no hace nada si sin repetidos
{

}


void crearVectorIterador_ALU(Vector *v,VectorIterador *it)
{
	it->vec = v;
	it->act = NULL;
	it->ult = NULL;
	it->finIter = true;
}

void *primerVecIterador_ALU(VectorIterador *it)
{
	Vector *v = it->vec;

	if(v->ce==0)
	{
		return NULL;
	}

	it->act = v->vec;
	it->ult = v->vec + (v->ce-1) *v->tamElem;
	it->finIter = false;

	return it->act;
}

void *siguienteVecIterador(VectorIterador *it)
{
	void *sig = it->act + it->vec->tamElem;
	if(sig > it->ult)
	{
		it->finIter = true;
		return NULL;
	}
	it->act = sig;
	return it->act;
}

bool finVecIterador(VectorIterador *it)
{
	return it->finIter;
}

int cargarVectorMovimientosLibro_ALU(Vector *vecMovs,const char *nomArchLibros)
{
	FILE *arch = fopen(nomArchLibros,"rt");
	if(!arch)
	{
		return ERR_ARCHIVO;
	}

	char *act;
	char linea[TAMLINEA+1];
	int ret = TODO_OK;
	MovLibro movLibro;

	while(fgets(linea,TAMLINEA+1,arch) && ret == TODO_OK)
	{
		act = strrchr(linea,'\n');
		if(act)
		{
			// cantidad
			*act = '\0';
			act = strrchr(linea,'|');
			if(*(act+1) == 'P' || *(act+1) == 'p')
			{
				movLibro.cantidad = -1;
			}
			else
			{
				movLibro.cantidad = 1;
			}
			// codigo sicio
			*act = '\0';
			act = strrchr(linea,'|');
			strcpy(movLibro.codigoSocio,act+1);
			// codigo libro
			*act = '\0';
			strcpy(movLibro.codigoLibro,linea);

			if(!insertarVecOrd_ALU(vecMovs,&movLibro,cmpMovLibro_ALU,actualizarLibro_ALU))
				ret = SIN_MEN;
		}
		else
			ret = ERR_LINEA;
	}

	if(ret != TODO_OK)
	{
		fclose(arch);
		return ret;
	}

	fclose(arch);
	return TODO_OK;
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

	if(v->ce ==0)
	{
		memcpy(v->vec,elem,v->tamElem);
		v->ce++;
		return true;
	}

	void *act = v->vec;
	void *ult = v->vec + (v->ce-1) * v->tamElem;

	while(act<=ult && cmp(elem,act)>0)
	{
		act += v->tamElem;
	}

	if(cmp(act,elem) ==0)
	{
		actualizar(act,elem);
		return true;
	}

	void *ins = act;

	for(act = ult ; act >= ins ; act-=v->tamElem)
	{
		memcpy(act+v->tamElem,act,v->tamElem);
	}


	memcpy(ins,elem,v->tamElem);
	v->ce++;

	return true;
}

int cmpMovLibro_ALU(const void* e1, const void* e2)
{
	const MovLibro *a = e1;
	const MovLibro *b = e2;

	return strcmp(a->codigoLibro,b->codigoLibro);
}
void actualizarLibro_ALU(void* actualizado, const void* actualizador)
{
	MovLibro *dest = actualizado;
	const MovLibro *origen = actualizador;

	dest->cantidad += origen->cantidad;
}


bool vectorCrear_ALU(Vector *v, size_t tamElem)
{
	v->ce =0;
	v->vec = malloc(tamElem *CAP_INI);
	if(!v->vec)
	{
		return false;
	}
	v->cap = CAP_INI;
	v->tamElem = tamElem;
	return true;
}

void vectorDestruir_ALU(Vector *v)
{
	v->ce =0;
	free(v->vec);
	v->vec = NULL;
	v->cap = 0;
	v->tamElem = 0;
}

bool redimensionar_ALU(Vector *v, float factor)
{
	size_t ncap = factor *v->cap;
	void * nvec = realloc(v->vec,ncap *v->tamElem);
	if(!nvec)
	{
		return false;
	}

	v->cap = ncap;
	v->vec = nvec;
	return true;
}


