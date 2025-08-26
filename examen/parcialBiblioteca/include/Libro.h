#ifndef LIBRO_H
#define LIBRO_H


typedef struct
{
	char codigoSocio[11];
	char codigoLibro[7];
	int cantidad; ///Positivo: más devoluciones, Negativo: más préstamos, Cero: Misma cant. de prést. que dev.
}
MovLibro; ///El tipo de los elementos del TDA Vector.


typedef struct
{
	char codigo[7];
	char nombre[101];
	int cantidad;
}
Libro;



typedef struct
{
	char codigo[7];
	long nroReg;
}
IndLibro;


#endif // LIBRO_H
