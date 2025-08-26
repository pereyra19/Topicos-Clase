#ifndef TIPOS_ARCHIVOS_H
#define TIPOS_ARCHIVOS_H


typedef struct
{
	char codigo[16];
	char nombre[51];
	float porcBonif;
}
Cliente;


typedef struct
{
	char codCliente[16];
	char codProd[16];
	float precioUnit;
	int cantidad;
}
Venta;


typedef struct
{
	char codCliente[16];
	long nroReg;
}
Indice;


#endif // TIPOS_ARCHIVOS_H
